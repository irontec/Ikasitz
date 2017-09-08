//
//  SettingScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "SettingScene.h"
#include "../AppDelegate.h"
#include "../Layers/SkyLayer.h"
#include "../Helpers/SpriteButton.h"
#include "MenuScene.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/Slider.h"
#include "SimpleAudioEngine.h"

extern char* getTimeWithFormat(int seconds);

using namespace CocosDenshion;


SettingScene::~SettingScene()
{
    
}

SettingScene::SettingScene()
{
    sound = true;
}

bool SettingScene::init()
{
    SkyLayer *pSkyLayer = SkyLayer::create();
    addChild(pSkyLayer);
    
    Rect visibleRect = VisibleRect::getVisibleRect();
    
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, CC_CALLBACK_1(SettingScene::backClicked, this));
    Point bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(Vec2(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    Sprite *pBackground = Sprite::create("base_settings.png");
    pBackground->setPosition(VisibleRect::center());
    addChild(pBackground);
    
    
    Size backgroundSize = pBackground->getContentSize();
    
    Sprite *pBarra = Sprite::create("barra.png");
    pBarra->setPosition(Vec2(backgroundSize.width/2, 150));
    pBackground->addChild(pBarra);
    
    
    //Rate App
    /*pTexture = Director::getInstance()->getTextureCache()->addImage("rate_app.png");
    SpriteButton *pRate = SpriteButton::create(pTexture, this, callfuncO_selector(SettingScene::backClicked));
    Point bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(Vec2(bottomLeft.x + 70, bottomLeft.y + 60));
    pBackground->addChild(pBackButton);*/
    
    //Read default value
    auto pUserDefaults = UserDefault::getInstance();
    volume = pUserDefaults->getFloatForKey("volume");
    sound = pUserDefaults->getBoolForKey("sound");
    
    //Sound
    
    auto pSoinuaLabel = Label::createWithTTF("Soinua", fontList[0], 35.0);
    pSoinuaLabel->setPosition(Vec2(120, 320));
    pSoinuaLabel->setColor(Color3B(63, 62, 62));
    pSoinuaLabel->setDimensions(250, 40);
    pSoinuaLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
    pBackground->addChild(pSoinuaLabel);
    
    Menu *pSoinuaMenu = Menu::create();
    MenuItemSprite* pMenuOn = MenuItemSprite::create(Sprite::create("check_on.png"), NULL);
    MenuItemSprite* pMenuOff = MenuItemSprite::create(Sprite::create("check_off.png"), NULL);
    MenuItemToggle* pToggle = MenuItemToggle::createWithCallback(
                                                                 CC_CALLBACK_1(SettingScene::soundCallback, this),
                                                                 pMenuOn,
                                                                 pMenuOff,
                                                                 NULL);
    pSoinuaMenu->addChild(pToggle);
    pSoinuaMenu->setPosition(Vec2(290,320));
    pBackground->addChild(pSoinuaMenu);
    
    if (!sound) {
        pToggle->setSelectedIndex(1);
    }
    
    
    //Volume
    
    auto pVolumeLabel = Label::createWithTTF("Bolumena", fontList[0], 35.0);
    pVolumeLabel->setPosition(Vec2(120, 250));
    pVolumeLabel->setColor(Color3B(63, 62, 62));
    pVolumeLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
    pVolumeLabel->setDimensions(250, 40);
    pBackground->addChild(pVolumeLabel);
    
    Sprite *pVolOn = Sprite::create("volume_on.png");
    pVolOn->setPosition(Vec2(600,250));
    pBackground->addChild(pVolOn);

    
    Sprite *pVolOff = Sprite::create("volume_off.png");
    pVolOff->setPosition(Vec2(290,250));
    pBackground->addChild(pVolOff);

    
    cocos2dExt::Slider* slider = cocos2dExt::Slider::SliderWithFiles("slider_track.png", "slider_knob.png", this,  menu_selector(SettingScene::sliderCallback));
    slider->setPosition(Vec2(440,250));
    //slider->setRotation(90);
    slider->SetHeight(100);
    //slider->SetHorizontalPadding(50);
    slider->SetTrackTouchOutsideContent(true);
    slider->SetEvaluateFirstTouch(false);
    slider->SetMinValue(0.0f);
    slider->SetMaxValue(1.0f);
    slider->SetEnabled(true);
    slider->SetValue(volume);
    pBackground->addChild(slider);
    
    //Stadistics
    auto pStadistics = Label::createWithTTF("Estadistikak", fontList[0], 20.0);
    pStadistics->setPosition(Vec2(345, 120));
    pStadistics->setColor(Color3B(63, 62, 62));
    pStadistics->setHorizontalAlignment(TextHAlignment::LEFT);
    pBackground->addChild(pStadistics);
    
    std::string partidak = std::string("Partida jokatuta:   ");
    std::stringstream ss;
    ss << pUserDefaults->getIntegerForKey("partidak", 0);
    partidak.append(ss.str());
    
    auto pPartidak = Label::createWithTTF(partidak.c_str(), fontList[1], 20.0);
    pPartidak->setPosition(Vec2(350, 90));
    pPartidak->setColor(Color3B(63, 62, 62));
    pPartidak->setHorizontalAlignment(TextHAlignment::LEFT);
    pBackground->addChild(pPartidak);
    
    std::string denbora = std::string("Denbora totala:   ");
    denbora.append(getTimeWithFormat(pUserDefaults->getIntegerForKey("denbora")));
    
    auto pDenbora = Label::createWithTTF(denbora.c_str(), fontList[1], 20.0);
    pDenbora->setPosition(Vec2(365, 60));
    pDenbora->setColor(Color3B(63, 62, 62));
    pDenbora->setHorizontalAlignment(TextHAlignment::LEFT);
    pBackground->addChild(pDenbora);
        
    return true;
}

void SettingScene::soundCallback(Ref *sender)
{
    MenuItemToggle *pToggle = (MenuItemToggle*) sender;
    int index = pToggle->getSelectedIndex();
    if (!index) {
        sound = true;
        /*if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
            SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music.wav", true);*/
    } else {
        sound = false;
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void SettingScene::sliderCallback(Ref *sender)
{
    cocos2dExt::Slider *pSlider = (cocos2dExt::Slider*) sender;
    
    volume = pSlider->GetValue();
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
    SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
}

void SettingScene::backClicked(Ref *sender)
{
    //Save preferences
    auto pUserDefaults = UserDefault::getInstance();
    pUserDefaults->setFloatForKey("volume", volume);
    pUserDefaults->setBoolForKey("sound", sound);
    pUserDefaults->flush();
    
    auto pDirector = Director::getInstance();
    
    if (getSceneReturnType() == kPopScene) {
         pDirector->popScene();
    } else {
        MenuScene *scene = MenuScene::create();
        auto tscene = TransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);
    }
   
}
