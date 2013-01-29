//
//  MenuScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#include "MenuScene.h"
#include "UniverseScene.h"
#include "SpriteButton.h"
#include "SkyLayer.h"
#include "SettingScene.h"
#include "VisibleRect.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include "CCApplication.h"

using namespace CocosDenshion;


MenuScene::~MenuScene()
{

}

MenuScene::MenuScene()
{
    
}

bool MenuScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    CCRect visibleRect = VisibleRect::getVisibleRect();
    
    CCSprite *pLogo = CCSprite::create("logo.png");
    pLogo->setPosition(VisibleRect::center());
    addChild(pLogo);
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("play.png");
    pPlayButton = SpriteButton::create(pTexture, this, callfuncO_selector(MenuScene::playClicked));
    pPlayButton->setPosition(ccp(477, 185));
    addChild(pPlayButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("about.png");
    pInfoButton = SpriteButton::create(pTexture, this, callfuncO_selector(MenuScene::infoClicked));
    pInfoButton->setPosition(ccp(visibleRect.origin.x + 205, visibleRect.origin.y + 60));
    addChild(pInfoButton);
    
    pTexture = CCTextureCache::sharedTextureCache()->addImage("settings.png");
    pSettingsButton = SpriteButton::create(pTexture, this, callfuncO_selector(MenuScene::settingsClicked));
    pSettingsButton->setPosition(ccp(visibleRect.origin.x + 110, visibleRect.origin.y + 60));
    addChild(pSettingsButton);
    
    //Read preferences
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    
    float volume = pUserDefaults->getFloatForKey("volume");
    bool sound = pUserDefaults->getBoolForKey("sound");
        
    if (sound) {
        bool isPlaying = SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
        
        if (!isPlaying)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/music.wav", true);
        
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
    }
   
    return true;
}


void MenuScene::playClicked(CCObject *sender)
{
    UniverseScene *scene = UniverseScene::create();
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}

void MenuScene::infoClicked(CCObject *sender)
{
    CCApplication::openURL("http://www.ikasitz.net/honi-buruz/");
}

void MenuScene::settingsClicked(CCObject *sender)
{
    SettingScene *scene = SettingScene::create();
    scene->setSceneReturnType(kReplaScene);
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}