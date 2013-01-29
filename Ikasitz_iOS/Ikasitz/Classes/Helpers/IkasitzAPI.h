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
#include "HttpClient.h"
#include "SQLHelper.h"
#include "HttpRequest.h"
#include "cJSON.h"

USING_NS_CC;
USING_NS_CC_EXT;

static const char * URL = "http://ikasitz.net/kontrola/api";

class KategoriaModel;

class IkasitzAPI : public CCObject
{
    
private:
    SQLHelper *m_pSQLHelper;
    
    IkasitzAPI();
    CCHttpRequest* createRequest();
    
    void downloadKategoriaInfo(KategoriaModel *pKat, std::string tag);
    void checkKategoriaData(CCArray *pKategoriak);
    
    //Parser
    CCArray* parseKategoriak(const std::string data);
    KategoriaModel* parseLevels(KategoriaModel *pKat);
    
    //Http Response Callback
    void onKategoriaRequestCompleted(CCNode *sender, void *requestData);
    void onDownloadRequestCompleted(CCNode *sender, void *requestData);
    void onDownloadFinalRequestCompleted(CCNode *sender, void *requestData);
    std::string checkResponse(CCHttpResponse *response);

    
public:
    CC_SYNTHESIZE(CCCallFunc*, m_pCallback, FinishCallback);
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
    CCArray * getUniverses();
};


#endif /* defined(__Ikasitz__IkasitzAPI__) */
