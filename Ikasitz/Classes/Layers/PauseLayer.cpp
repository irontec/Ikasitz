//
//  PauseLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "PauseLayer.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SpriteButton.h"
#include "../Scenes/SettingScene.h"
#include "../Helpers/IkasitzAPI.h"
#include "../Models/LevelModel.h"
#include "../Scenes/LevelScene.h"
#include "../Models/KategoriaModel.h"


PauseLayer::~PauseLayer()
{
    
}

PauseLayer::PauseLayer()
{
    
}

bool PauseLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Sprite *pBackground = Sprite::create("base_pause.png");
    
    Size backgroundSize = pBackground->getContentSize();
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("play_pause.png");
    SpriteButton *pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PlayScene::resumeGame, m_pScene));
    
    pButton->setPosition(Vec2(backgroundSize.width/2, 420));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("settings_pause.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PauseLayer::settings, this));
    pButton->setPosition(Vec2(backgroundSize.width/2, 300));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("retry.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PauseLayer::restart, this));
    pButton->setPosition(Vec2(backgroundSize.width/2, 230));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("next.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PauseLayer::nextLevel, this));
    pButton->setPosition(Vec2(backgroundSize.width/2, 160));
    pBackground->addChild(pButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("menu.png");
    pButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PauseLayer::allLevels, this));
    pButton->setPosition(Vec2(backgroundSize.width/2, 90));
    pBackground->addChild(pButton);
    
    addChild(pBackground);
    
    return true;
}

void PauseLayer::nextLevel(Ref *sender)
{
    this->unscheduleAllCallbacks();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    KategoriaModel *pKat = pAPI->getSelectedKategoria();
    
    pAPI->currentLevel++;
    int levelPosition = pAPI->currentLevel;
    
    if (levelPosition == (int) pKat->getPantailak()->size()) {
        allLevels(NULL);
    } else {
        auto pLevel = pAPI->getSelectedKategoria()->getPantailak()->at(levelPosition);
        PlayScene *scene = PlayScene::create(pLevel);
        auto pDirector = Director::getInstance();
        auto tscene = TransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);

    }
}

void PauseLayer::allLevels(Ref *sender)
{
    this->unscheduleAllCallbacks();
    
    LevelScene *scene = LevelScene::create();
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void PauseLayer::restart(Ref *sender)
{
    this->unscheduleAllCallbacks();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();
    int levelPosition = pAPI->currentLevel;
    auto pLevel = pAPI->getSelectedKategoria()->getPantailak()->at(levelPosition);
    PlayScene *scene = PlayScene::create(pLevel);
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void PauseLayer::settings(Ref *sender)
{
    SettingScene *scene = SettingScene::create();
    scene->setSceneReturnType(kPopScene);
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->pushScene(tscene);
}
