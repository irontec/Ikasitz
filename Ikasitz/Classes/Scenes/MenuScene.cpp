//
//  MenuScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#include "MenuScene.h"
#include "UniverseScene.h"
#include "../Helpers/SpriteButton.h"
#include "../Layers/SkyLayer.h"
#include "SettingScene.h"
#include "../Helpers/VisibleRect.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

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
    
    Rect visibleRect = VisibleRect::getVisibleRect();
    
    Sprite *pLogo = Sprite::create("logo.png");
    pLogo->setPosition(VisibleRect::center());
    addChild(pLogo);
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("play.png");
    pPlayButton = SpriteButton::create(pTexture, CC_CALLBACK_1(MenuScene::playClicked, this));
    pPlayButton->setPosition(Vec2(477, 185));
    addChild(pPlayButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("about.png");
    pInfoButton = SpriteButton::create(pTexture, CC_CALLBACK_1(MenuScene::infoClicked, this));
    pInfoButton->setPosition(Vec2(visibleRect.origin.x + 205, visibleRect.origin.y + 60));
    addChild(pInfoButton);
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("settings.png");
    pSettingsButton = SpriteButton::create(pTexture, CC_CALLBACK_1(MenuScene::settingsClicked, this));
    pSettingsButton->setPosition(Vec2(visibleRect.origin.x + 110, visibleRect.origin.y + 60));
    addChild(pSettingsButton);
    
    //Read preferences
    auto pUserDefaults = UserDefault::getInstance();
    
    float volume = pUserDefaults->getFloatForKey("volume");
    bool sound = pUserDefaults->getBoolForKey("sound");
        
    if (sound) {
        bool isPlaying = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
        
        if (!isPlaying)
            SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music.wav", true);
        
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
        SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
    }
   
    return true;
}


void MenuScene::playClicked(Ref *sender)
{
    if (!IkasitzAPI::sharedInstance()->getUniverses()->empty()) {
        UniverseScene *scene = UniverseScene::create();
        auto pDirector = Director::getInstance();
        auto tscene = TransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);
    } else {
        CCLOGWARN("Kategoria db table is empty. Can NOT play.");
    }
    
}

void MenuScene::infoClicked(Ref *sender)
{
    Application::getInstance()->openURL("http://www.ikasitz.net/honi-buruz/");
}

void MenuScene::settingsClicked(Ref *sender)
{
    SettingScene *scene = SettingScene::create();
    scene->setSceneReturnType(kReplaScene);
    auto pDirector = Director::getInstance();
    auto tscene = TransitionFade::create(0.5, scene);
    pDirector->replaceScene(tscene);
}
