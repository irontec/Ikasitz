//
//  LoadingScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 18/01/13.
//
//

#include "LoadingScene.h"
#include "../Layers/SkyLayer.h"
#include "../AppDelegate.h"
#include "../Helpers/VisibleRect.h"
#include "MenuScene.h"


LoadingScene::~LoadingScene()
{
    //CC_SAFE_DELETE(m_pAPI);
}

LoadingScene::LoadingScene()
{
    m_pAPI = IkasitzAPI::sharedInstance();
    
    CallFunc *pCallback = CallFunc::create(CC_CALLBACK_0(LoadingScene::finishLoading, this));
    pCallback->retain();
    m_pAPI->setFinishCallback(pCallback);
        
    m_pAPI->checkDataUpdates();
    
}


bool LoadingScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    
    Rect visibleRect = VisibleRect::getVisibleRect();
    
    Sprite *pBack = Sprite::create("base_settings.png");
    pBack->setPosition(VisibleRect::center());
    
    Sprite *pSpinner = Sprite::create("loading.png");
    pSpinner->setPosition(Vec2(pBack->getContentSize().width/2, pBack->getContentSize().height/2 - 30));
    pBack->addChild(pSpinner);
    
    auto pLabel = Label::createWithTTF("Kargatzen...", fontList[0], 25.0f);
    pLabel->setPosition(Vec2(pBack->getContentSize().width/2, pBack->getContentSize().height/2 + 60));
    pLabel->setColor(Color3B(63, 62, 62));
    pLabel->setDimensions(200, 50);
    pBack->addChild(pLabel);

    addChild(pBack);
    
    ActionInterval* rotate = RotateBy::create(5.0f, 360.0f);
    RepeatForever *repeat = RepeatForever::create(rotate);
    pSpinner->runAction(repeat);

    
    return true;
}

void LoadingScene::finishLoading()
{    
    MenuScene *scene = MenuScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
