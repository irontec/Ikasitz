//
//  FinishLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 24/01/13.
//
//

#include "FinishLayer.h"
#include "SpriteButton.h"
#include "IkasitzAPI.h"
#include "PlayScene.h"
#include "LevelModel.h"
#include "KategoriaModel.h"
#include "LevelScene.h"
#include "AppDelegate.h"

FinishLayer::~FinishLayer()
{
    delete time;
}

FinishLayer::FinishLayer()
{
    
}

bool FinishLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    CCSprite *pBackground = CCSprite::create("base_pause.png");
    
    CCSize backgroundSize = pBackground->getContentSize();
    
    CCSprite *pZorionak = CCSprite::create("zorionak.png");
    pZorionak->setPosition(ccp(backgroundSize.width/2, 470));
    pBackground->addChild(pZorionak);
    
    CCLabelTTF *pLevelName = CCLabelTTF::create(title, fontList[0], 30.0);
    pLevelName->setPosition(ccp(backgroundSize.width/2 - 20, pZorionak->getPosition().y - pZorionak->getContentSize().height/2 - 45));
    pLevelName->setHorizontalAlignment(kCCTextAlignmentLeft);
    pLevelName->setDimensions(CCSizeMake(200, 50));
    pLevelName->setColor(ccc3(63, 62, 62));
    pBackground->addChild(pLevelName);
    
    CCLabelTTF *pTimeName = CCLabelTTF::create("Denbora", fontList[0], 30.0);
    pTimeName->setPosition(ccp(pLevelName->getPosition().x - 20, pLevelName->getPosition().y - pLevelName->getContentSize().height/2 - 15));
    pTimeName->setHorizontalAlignment(kCCTextAlignmentLeft);
    pTimeName->setDimensions(CCSizeMake(0, 50));
    pTimeName->setColor(ccc3(63, 62, 62));
    pBackground->addChild(pTimeName);
    
    CCLabelTTF *pTime = CCLabelTTF::create(time, fontList[1], 30.0);
    pTime->setPosition(ccp(pTimeName->getPosition().x + pTimeName->getContentSize().width - 5, pTimeName->getPosition().y));
    pTime->setDimensions(CCSizeMake(0, 50));
    pTime->setColor(ccc3(63, 62, 62));
    pBackground->addChild(pTime);

    int xPosition = pTimeName->getPosition().x - pTimeName->getContentSize().width/2;
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("retry.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 250));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::nextLevel));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 170));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::allLevels));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 100));
    pBackground->addChild(pButton);
    
    addChild(pBackground);
    
    /*CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("retry.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 290));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 225));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition, 160));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("fb.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition + 5, 105));
    pBackground->addChild(pButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("twitter.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(ccp(0, 0.5));
    pButton->setPosition(ccp(xPosition + 5, 40));
    pBackground->addChild(pButton);
    
    addChild(pBackground);*/
    
    return true;
}

void FinishLayer::nextLevel(CCObject *sender)
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

void FinishLayer::allLevels(CCObject *sender)
{    
    LevelScene *scene = LevelScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void FinishLayer::restart(CCObject *sender)
{
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    int levelPosition = pAPI->currentLevel;
    LevelModel *pLevel = (LevelModel*) pAPI->getSelectedKategoria()->getPantailak()->objectAtIndex(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}