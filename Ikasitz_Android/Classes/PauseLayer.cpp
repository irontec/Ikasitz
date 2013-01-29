//
//  PauseLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "PauseLayer.h"
#include "VisibleRect.h"
#include "SpriteButton.h"
#include "SettingScene.h"
#include "IkasitzAPI.h"
#include "LevelModel.h"
#include "LevelScene.h"
#include "KategoriaModel.h"


PauseLayer::~PauseLayer()
{
    
}

PauseLayer::PauseLayer()
{
    
}

bool PauseLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSprite *pBackground = CCSprite::create("base_pause.png");
    
    CCSize backgroundSize = pBackground->getContentSize();
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("play_pause.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, m_pScene, callfuncO_selector(PlayScene::resumeGame));
    pButton->setPosition(ccp(backgroundSize.width/2, 420));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("settings_pause.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(PauseLayer::settings));
    pButton->setPosition(ccp(backgroundSize.width/2, 300));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("retry.png");
    pButton = SpriteButton::create(pTexture, m_pScene, callfuncO_selector(PauseLayer::restart));
    pButton->setPosition(ccp(backgroundSize.width/2, 230));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, m_pScene, callfuncO_selector(PauseLayer::nextLevel));
    pButton->setPosition(ccp(backgroundSize.width/2, 160));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, m_pScene, callfuncO_selector(PauseLayer::allLevels));
    pButton->setPosition(ccp(backgroundSize.width/2, 90));
    pBackground->addChild(pButton);
    
    addChild(pBackground);
    
    return true;
}

void PauseLayer::nextLevel(CCObject *sender)
{
    this->unscheduleAllSelectors();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    KategoriaModel *pKat = pAPI->getSelectedKategoria();
    
    pAPI->currentLevel++;
    int levelPosition = pAPI->currentLevel;
    
    if (levelPosition == pKat->getPantailak()->count()) {
        allLevels(NULL);
    } else {
        LevelModel *pLevel = (LevelModel*) pAPI->getSelectedKategoria()->getPantailak()->objectAtIndex(levelPosition);
        PlayScene *scene = PlayScene::create(pLevel);
        CCDirector *pDirector = CCDirector::sharedDirector();
        CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);

    }
}

void PauseLayer::allLevels(CCObject *sender)
{
    this->unscheduleAllSelectors();
    
    LevelScene *scene = LevelScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void PauseLayer::restart(CCObject *sender)
{
    this->unscheduleAllSelectors();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    int levelPosition = pAPI->currentLevel;
    LevelModel *pLevel = (LevelModel*) pAPI->getSelectedKategoria()->getPantailak()->objectAtIndex(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void PauseLayer::settings(CCObject *sender)
{
    SettingScene *scene = SettingScene::create();
    scene->setSceneReturnType(kPopScene);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->pushScene(tscene);
}