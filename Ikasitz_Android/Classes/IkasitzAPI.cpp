//
//  IkasitzAPI.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#include "IkasitzAPI.h"
#include "unzip.h"
#include "KategoriaModel.h"
#include "LevelModel.h"
#include "HitzaModel.h"

#define MAX_FILENAME 512
#define READ_SIZE 8192

void *unzipFile(void *savedPath);

static IkasitzAPI* _instance = NULL;


IkasitzAPI* IkasitzAPI::sharedInstance()
{
    if (_instance == NULL)
        _instance = new IkasitzAPI();
    return _instance;
}


IkasitzAPI::~IkasitzAPI()
{
    CC_SAFE_DELETE(m_pSQLHelper);
}

IkasitzAPI::IkasitzAPI()
{
    m_pSQLHelper = SQLHelper::sharedInstance();
}

void* IkasitzAPI::checkDataUpdates()
{
    CCHttpRequest *pRequest = createRequest();
    CCHttpClient::getInstance()->send(pRequest);
    pRequest->release();
    
    return NULL;
}

void IkasitzAPI::checkKategoriaData(CCArray *pKategoriak)
{
    int numKategoriak = pKategoriak->count();
    for (int i = 0; i < numKategoriak; i++) {
        
        std::string tag;
        
        if (i == numKategoriak-1) {
            tag.append("last");
        } else {
            tag.append("nonlast");
        }
        
        
        KategoriaModel *pKat = (KategoriaModel*) pKategoriak->objectAtIndex(i);
        
        downloadKategoriaInfo(pKat, tag);
        m_pSQLHelper->insertKategoria(pKat);
    }
}

CCArray * IkasitzAPI::getUniverses()
{
    return m_pSQLHelper->queryKategoriak(-1);
}

void IkasitzAPI::loadCurrentUniverse(KategoriaModel *pkat)
{
    parseLevels(pkat);
    m_pSelectedKategoria = pkat;
    m_pSelectedKategoria->retain();
}

CCHttpRequest* IkasitzAPI::createRequest()
{
    const char *requestData = "{\"method\":\"kategoriak\", \"id\":1, \"jsonrpc\":\"2.0\", \"params\":[]}";
    
    CCHttpRequest* pRequest = new CCHttpRequest();
    pRequest->setUrl(URL);
    pRequest->setRequestType(CCHttpRequest::kHttpPost);
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/x-www-form-urlencoded");
    pRequest->setHeaders(headers);
    pRequest->setResponseCallback(this, callfuncND_selector(IkasitzAPI::onKategoriaRequestCompleted));
    pRequest->setRequestData(requestData, strlen(requestData));
    
    return pRequest;
}


CCArray* IkasitzAPI::parseKategoriak(const std::string data)
{
    CCArray *pKategoriakTmp = CCArray::create();
    
    cJSON *pRoot = cJSON_Parse(data.c_str());
    cJSON *pResult = cJSON_GetObjectItem(pRoot, "result");
    cJSON *pKategoriakJSON = cJSON_GetArrayItem(pResult, 0);
    
    if (pKategoriakJSON != NULL) {
        
        int numKategoriak = cJSON_GetArraySize(pKategoriakJSON);
        
        for (int i = 0; i < numKategoriak; i++)
        {
            cJSON * pKatJSON = cJSON_GetArrayItem(pKategoriakJSON, i);
            KategoriaModel *pKat = new KategoriaModel();
                       
            pKat->setID(cJSON_GetObjectItem(pKatJSON, "id")->valueint);
            pKat->setIzena(strdup(cJSON_GetObjectItem(pKatJSON, "izena")->valuestring));
            pKat->setDeskribapena(strdup(cJSON_GetObjectItem(pKatJSON, "deskribapena")->valuestring));
            pKat->setFileName(strdup(cJSON_GetObjectItem(pKatJSON, "izenGarbia")->valuestring));
            pKat->setZipURL(strdup(cJSON_GetObjectItem(pKatJSON, "zipUrl")->valuestring));
            pKat->setToken(strdup(cJSON_GetObjectItem(pKatJSON, "token")->valuestring));
            
            pKategoriakTmp->addObject(pKat);
            
            pKat->release();
        }
    }
    
    cJSON_Delete(pRoot);
    
    return pKategoriakTmp;
}

KategoriaModel* IkasitzAPI::parseLevels(KategoriaModel *pKat)
{
    std::string filePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    filePath.append(pKat->getFileName()).append(".json");
    std::ifstream fin(filePath.c_str());
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string content(buffer.str());
    
    CCArray *pLevels = CCArray::create();
    
    cJSON *pRoot = cJSON_Parse(content.c_str());
    cJSON *pPantailakJSON = cJSON_GetArrayItem(pRoot, 4);
    
    if (pPantailakJSON != NULL) {
        
        int numPantailak = cJSON_GetArraySize(pPantailakJSON);
        
        for (int i = 0; i < numPantailak; i++)
        {
            cJSON * pPantailaJSON = cJSON_GetArrayItem(pPantailakJSON, i);
            LevelModel *pLevel = new LevelModel();
            
            pLevel->setID(cJSON_GetObjectItem(pPantailaJSON, "id")->valueint);
            pLevel->setIzena(strdup(cJSON_GetObjectItem(pPantailaJSON, "izena")->valuestring));
            pLevel->setIrudia(strdup(cJSON_GetObjectItem(pPantailaJSON, "irudiFile")->valuestring));
            pLevel->setMarkara(strdup(cJSON_GetObjectItem(pPantailaJSON, "maskaraFile")->valuestring));
            pLevel->setList(strdup(cJSON_GetObjectItem(pPantailaJSON, "listFile")->valuestring));
            
            cJSON *pHitzakJSON = cJSON_GetArrayItem(pPantailaJSON, 5);
            
            if (pHitzakJSON != NULL) {
                
                int numHitzak = cJSON_GetArraySize(pHitzakJSON);
                
                CCArray *pHitzakTmp = CCArray::create();
                
                for (int i = 0; i < numHitzak; i++)
                {
                    cJSON * pHitzaJSON = cJSON_GetArrayItem(pHitzakJSON, i);
                    HitzaModel *pHitza = new HitzaModel();
                    
                    pHitza->setID(cJSON_GetObjectItem(pHitzaJSON, "id")->valueint);
                    pHitza->setIzena(strdup(cJSON_GetObjectItem(pHitzaJSON, "izena")->valuestring));
                    pHitza->setSoinua(strdup(cJSON_GetObjectItem(pHitzaJSON, "soinuPath")->valuestring));
                    
                    GLubyte r = cJSON_GetObjectItem(pHitzaJSON, "r")->valueint;
                    GLubyte g = cJSON_GetObjectItem(pHitzaJSON, "g")->valueint;
                    GLubyte b = cJSON_GetObjectItem(pHitzaJSON, "b")->valueint;
                    
                    pHitza->setKolorea(ccc3(r, g, b));
                    
                    pHitzakTmp->addObject(pHitza);
                    pHitza->release();
                }
                
                
                pLevel->setHitzak(pHitzakTmp);
                
            }
            
            pLevels->addObject(pLevel);
            pLevel->release();
        }
    }
    
    pKat->setPantailak(pLevels);
    
    cJSON_Delete(pRoot);
        
    return pKat;
}

void IkasitzAPI::downloadKategoriaInfo(KategoriaModel *pKat, std::string tag)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(pKat->getZipURL());
    request->setRequestType(CCHttpRequest::kHttpGet);
    
    if (!tag.compare("last")) {
          request->setResponseCallback(this, callfuncND_selector(IkasitzAPI::onDownloadFinalRequestCompleted));
    } else {
          request->setResponseCallback(this, callfuncND_selector(IkasitzAPI::onDownloadRequestCompleted));
    }
    
    request->setTag(pKat->getFileName());
    CCHttpClient::getInstance()->send(request);
    request->release();
}


//Http Response Callback
void IkasitzAPI::onKategoriaRequestCompleted(CCNode *sender, void *requestData)
{
    CCHttpResponse *response = (CCHttpResponse*)requestData;
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    bool firstTime = pUserDefaults->getBoolForKey("firstTime", true);
    
    if (response->getResponseCode() != 200) {
        if (firstTime) {
            CCMessageBox("Datuak kargatzeko interneta behar da", "Error");
        } else {
            m_pCallback->execute();
        }
        
    } else {
        
        std::string data = checkResponse(response);
        
        if (!data.empty()) {
            CCArray *pKategoriak = parseKategoriak(data);
            CCArray *pKategoriakToUpdate = CCArray::create();
            
            if (pKategoriak != NULL) {
                
                int numKategoriak = pKategoriak->count();
                for (int i = 0; i < numKategoriak; i++) {
                    
                    KategoriaModel *pKat = (KategoriaModel*) pKategoriak->objectAtIndex(i);
                    
                    CCArray *pKategoriaArray = m_pSQLHelper->queryKategoriak(pKat->getID());
                    
                    //Arraya hutsik badago kategoria ez da existitze, berria da.
                    if (pKategoriaArray->count() == 0) {
                        pKategoriakToUpdate->addObject(pKat);
                    } else {
                        KategoriaModel *pDBKat = (KategoriaModel*) pKategoriaArray->objectAtIndex(0);
                        
                        if (strcmp(pKat->getToken(), pDBKat->getToken()))
                        {
                            pKategoriakToUpdate->addObject(pKat);
                        }
                    }
                }
                
                if (pKategoriakToUpdate->count() == 0) {
                    m_pCallback->execute();
                } else {
                    CCMessageBox("Pantailak deskargatzen. Minutu pare bat itxaron", "Abisua");
                    checkKategoriaData(pKategoriakToUpdate);
                }

            }
        } else {
            if (firstTime) {
                CCMessageBox("Datuak kargatzeko interneta behar da", "Error");
            } else {
                m_pCallback->execute();
            }
        }
    }
}

void IkasitzAPI::onDownloadRequestCompleted(CCNode *sender, void *requestData)
{
    CCHttpResponse *response = (CCHttpResponse*)requestData;
        
    std::string data = checkResponse(response);
    
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    std::string path = fileUtils->getWriteablePath();
    std::string *savepath = new std::string(path);
    savepath->append(response->getHttpRequest()->getTag()).append(".zip");
    std::ofstream myfile(savepath->c_str(), std::ios::out | std::ios::binary);
    myfile.write(data.c_str(), data.length());
    myfile.flush();
    myfile.close();
    
    const char *threadPath = savepath->c_str();
    
    /*pthread_t inc_x_thread;
    pthread_create(&inc_x_thread, NULL, unzipFile, (void *)threadPath);
    pthread_join( inc_x_thread, NULL);*/
    unzipFile((void *)threadPath);

}

void IkasitzAPI::onDownloadFinalRequestCompleted(CCNode *sender, void *requestData)
{
    onDownloadRequestCompleted(sender, requestData);
    
    //Save preferences
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    bool firstTime = pUserDefaults->getBoolForKey("firstTime", true);
    
    if (firstTime) {
        pUserDefaults->setFloatForKey("volume", 0.5f);
        pUserDefaults->setBoolForKey("sound", true);
        pUserDefaults->setBoolForKey("firstTime", false);
        pUserDefaults->flush();
    }

    m_pCallback->execute();
}

std::string IkasitzAPI::checkResponse(CCHttpResponse *response)
{
    if (!response)
        return NULL;
    
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        CCLog("HTTP Status Code: %d", response->getResponseCode());
        
        std::vector<char> *buffer = response->getResponseData();
        
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string data(buffer->begin(), buffer->end());
    
    return data;
}

void *unzipFile(void *path)
{
    const char *savePath = (const char*) path;
    std::string writablePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
    
    unzFile zipfile = unzOpen(savePath);
    
    if (zipfile == NULL) {
        CCLog("Error opening zip file");
        exit(1);
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    
    if (unzGetGlobalInfo(zipfile, &global_info ) != UNZ_OK)
    {
        printf( "could not read file global info\n" );
        unzClose(zipfile);
        exit(2);
    }
    
    // Buffer to hold data read from the zip file.
    char read_buffer[ READ_SIZE ];
    
    // Loop to extract all files
    uLong i;
    for ( i = 0; i < global_info.number_entry; ++i )
    {
        // Get info about current file.
        unz_file_info file_info;
        char filename[MAX_FILENAME];
        if ( unzGetCurrentFileInfo(
                                   zipfile,
                                   &file_info,
                                   filename,
                                   MAX_FILENAME,
                                   NULL, 0, NULL, 0 ) != UNZ_OK )
        {
            printf( "could not read file info\n" );
            unzClose( zipfile );
            exit(3);
        }
        
        // Check if this entry is a directory or file.
        const size_t filename_length = strlen( filename );
        if ( filename[ filename_length-1 ] == '/')
        {
            //strcpy(pathName, filename);
            // Entry is a directory, so create it.
            //printf( "dir:%s\n", pathName );
            /*std::string command = std::string("mkdir ") + path + std::string(filename);
             system(command.c_str());*/
            //fileUtils->createDirectoryInPath(pathName);
        }
        else
        {
            // Entry is a file, so extract it.
            printf( "file:%s\n", filename );
            if ( unzOpenCurrentFile( zipfile ) != UNZ_OK )
            {
                printf( "could not open file\n" );
                unzClose( zipfile );
                exit(4);
            }
            
            // Open a file to write out the data.
            
            
            char *p = &filename[0];
            int pos = 1;
            
            while (*p != '\0') {
                if (*p == '/')
                    break;
                else
                    pos++;
                p++;
            }
            
            std::string filePath(writablePath);
            filePath.append(filename);
            CCLog("PATH: %s", filePath.c_str());
            std::ofstream out(filePath.c_str(), std::ios::out | std::ios::binary);
            
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile( zipfile, read_buffer, READ_SIZE );
                if ( error < 0 )
                {
                    printf( "error %d\n", error );
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    exit(5);
                }
                
                out.write(read_buffer, error);
                
            } while ( error > 0 );
            
            out.flush();
            out.close();
        }
        
        unzCloseCurrentFile( zipfile );
        
        // Go the the next entry listed in the zip file.
        if ( ( i+1 ) < global_info.number_entry )
        {
            if ( unzGoToNextFile( zipfile ) != UNZ_OK )
            {
                printf( "cound not read next file\n" );
                unzClose( zipfile );
                exit(6);
            }
        }
    }
    
    unzClose( zipfile );
    //pthread_exit(NULL);
}
