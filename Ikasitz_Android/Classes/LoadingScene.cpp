//
//  LoadingScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#include "LoadingScene.h"
#include "SkyLayer.h"
#include "AppDelegate.h"
#include "VisibleRect.h"
#include "MenuScene.h"


LoadingScene::~LoadingScene()
{
    CC_SAFE_DELETE(m_pAPI);
}

LoadingScene::LoadingScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    CCCallFunc *pCallback = CCCallFunc::create(this, callfunc_selector(LoadingScene::finishLoading));
    pCallback->retain();
    m_pAPI->setFinishCallback(pCallback);
        
    m_pAPI->checkDataUpdates();
    
}


bool LoadingScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    
    CCRect visibleRect = VisibleRect::getVisibleRect();
    
    CCSprite *pBack = CCSprite::create("base_settings.png");
    pBack->setPosition(VisibleRect::center());
    
    CCSprite *pSpinner = CCSprite::create("loading.png");
    pSpinner->setPosition(ccp(pBack->getContentSize().width/2, pBack->getContentSize().height/2 - 30));
    pBack->addChild(pSpinner);
    
    CCLabelTTF *pLabel = CCLabelTTF::create("Kargatzen...", fontList[0], 25.0f);
    pLabel->setPosition(ccp(pBack->getContentSize().width/2, pBack->getContentSize().height/2 + 60));
    pLabel->setColor(ccc3(63, 62, 62));
    pLabel->setDimensions(CCSizeMake(200, 50));
    pBack->addChild(pLabel);

    addChild(pBack);
    
    CCActionInterval* rotate = CCRotateBy::create(5.0f, 360.0f);
    CCRepeatForever *repeat = CCRepeatForever::create(rotate);
    pSpinner->runAction(repeat);

    
    return true;
}

void LoadingScene::finishLoading()
{
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    
    MenuScene *scene = MenuScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}