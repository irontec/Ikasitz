//
//  IkasitzAPI.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#ifndef __Ikasitz__IkasitzAPI__
#define __Ikasitz__IkasitzAPI__

#include <iostream>

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "../Models/KategoriaModel.h"
#include "../Helpers/SQLHelper.h"
#include "../Helpers/json/cJSON.h"

using namespace cocos2d;
//USING_NS_CC_EXT;

using namespace cocos2d::network;

static const char * URL = "http://ikasitz.net/kontrola/api";

class KategoriaModel;

class IkasitzAPI : public Ref
{
    
private:
    SQLHelper *m_pSQLHelper;
    
    IkasitzAPI();
    HttpRequest* createRequest();
    
    void downloadKategoriaInfo(KategoriaModel *pKat, std::string tag);
    void checkKategoriaData(Vector<KategoriaModel *> *pKategoriak);
    
    //Parser
    Vector<KategoriaModel *> * parseKategoriak(const std::string data);
    KategoriaModel* parseLevels(KategoriaModel *pKat);
    
    //Http Response Callback
    void onKategoriaRequestCompleted(HttpClient* client, HttpResponse* response);
    void onDownloadRequestCompleted(HttpClient* client, HttpResponse* response);
    void onDownloadFinalRequestCompleted(HttpClient* client, HttpResponse* response);
    std::string checkResponse(HttpResponse *response);

    
public:
    CC_SYNTHESIZE(CallFunc*, m_pCallback, FinishCallback);
    CC_SYNTHESIZE(float, m_UniverseImageScale, Scale);
    CC_SYNTHESIZE_READONLY(KategoriaModel*, m_pSelectedKategoria, SelectedKategoria);
    int currentLevel;
    
    static void *hello_helper(void *context)
    {
        return ((IkasitzAPI *)context)->checkDataUpdates();
    }
    
    virtual ~IkasitzAPI();
    static IkasitzAPI* sharedInstance();
    
    void* checkDataUpdates();
    void loadCurrentUniverse(KategoriaModel *pkat);
    Vector<KategoriaModel *> * getUniverses();
};


#endif /* defined(__Ikasitz__IkasitzAPI__) */
