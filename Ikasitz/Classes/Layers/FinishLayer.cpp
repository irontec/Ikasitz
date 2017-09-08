//
//  FinishLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 24/01/13.
//
//

#include "FinishLayer.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/IkasitzAPI.h"
#include "../Scenes/PlayScene.h"
#include "../Models/LevelModel.h"
#include "../Models/KategoriaModel.h"
#include "../Scenes/LevelScene.h"
#include "../AppDelegate.h"

FinishLayer::~FinishLayer()
{
    delete time;
}

FinishLayer::FinishLayer()
{
    
}

bool FinishLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Sprite *pBackground = Sprite::create("base_pause.png");
    
    Size backgroundSize = pBackground->getContentSize();
    
    Sprite *pZorionak = Sprite::create("zorionak.png");
    pZorionak->setPosition(Vec2(backgroundSize.width/2, 470));
    pBackground->addChild(pZorionak);
    
    auto pLevelName = Label::createWithTTF(title, fontList[0], 30.0);
    pLevelName->setPosition(Vec2(backgroundSize.width/2 - 20, pZorionak->getPosition().y - pZorionak->getContentSize().height/2 - 45));
    pLevelName->setHorizontalAlignment(TextHAlignment::LEFT);
    pLevelName->setDimensions(200, 50);
    pLevelName->setColor(Color3B(63, 62, 62));
    pBackground->addChild(pLevelName);
    
    auto pTimeName = Label::createWithTTF("Denbora", fontList[0], 30.0);
    pTimeName->setPosition(Vec2(pLevelName->getPosition().x - 20, pLevelName->getPosition().y - pLevelName->getContentSize().height/2 - 15));
    pTimeName->setHorizontalAlignment(TextHAlignment::LEFT);
    pTimeName->setDimensions(0, 50);
    pTimeName->setColor(Color3B(63, 62, 62));
    pBackground->addChild(pTimeName);
    
    auto pTime = Label::createWithTTF(time, fontList[1], 30.0);
    pTime->setPosition(Vec2(pTimeName->getPosition().x + pTimeName->getContentSize().width - 5, pTimeName->getPosition().y));
    pTime->setDimensions(0, 50);
    pTime->setColor(Color3B(63, 62, 62));
    pBackground->addChild(pTime);

    int xPosition = pTimeName->getPosition().x - pTimeName->getContentSize().width/2;
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("retry.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(FinishLayer::restart, this));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 250));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(FinishLayer::nextLevel, this));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 170));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(FinishLayer::allLevels, this));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 100));
    pBackground->addChild(pButton);
    
    addChild(pBackground);
    
    /*Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("retry.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 290));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 225));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition, 160));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("fb.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition + 5, 105));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("twitter.png");
    pButton = SpriteButton::create(pTexture, this, callfuncO_selector(FinishLayer::restart));
    pButton->setAnchorPoint(Vec2(0, 0.5));
    pButton->setPosition(Vec2(xPosition + 5, 40));
    pBackground->addChild(pButton);
    
    addChild(pBackground);*/
    
    return true;
}

void FinishLayer::nextLevel(Ref *sender)
{
    this->unscheduleAllCallbacks();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    KategoriaModel *pKat = pAPI->getSelectedKategoria();
    
    pAPI->currentLevel++;
    int levelPosition = pAPI->currentLevel;
    
    if (levelPosition == (int) pKat->getPantailak()->size()) {
        allLevels(NULL);
    } else {
        LevelModel *pLevel = pAPI->getSelectedKategoria()->getPantailak()->at(levelPosition);
        PlayScene *scene = PlayScene::create(pLevel);
        auto pDirector = Director::getInstance();
        auto tscene = TransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);
        
    }
}

void FinishLayer::allLevels(Ref *sender)
{    
    LevelScene *scene = LevelScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void FinishLayer::restart(Ref *sender)
{
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    int levelPosition = pAPI->currentLevel;
    LevelModel *pLevel = pAPI->getSelectedKategoria()->getPantailak()->at(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
