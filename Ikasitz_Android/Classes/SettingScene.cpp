//
//  SettingScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#include "SettingScene.h"
#include "AppDelegate.h"
#include "SkyLayer.h"
#include "SpriteButton.h"
#include "MenuScene.h"
#include "VisibleRect.h"
#include "CCSlider.h"
#include "SpriteButton.h"
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
    
    CCRect visibleRect = VisibleRect::getVisibleRect();
    
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("back.png");
    SpriteButton *pBackButton = SpriteButton::create(pTexture, this, callfuncO_selector(SettingScene::backClicked));
    CCPoint bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(ccp(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pBackButton);
    
    CCSprite *pBackground = CCSprite::create("base_settings.png");
    pBackground->setPosition(VisibleRect::center());
    addChild(pBackground);
    
    
    CCSize backgroundSize = pBackground->getContentSize();
    
    CCSprite *pBarra = CCSprite::create("barra.png");
    pBarra->setPosition(ccp(backgroundSize.width/2, 150));
    pBackground->addChild(pBarra);
    
    
    //Rate App
    /*pTexture = CCTextureCache::sharedTextureCache()->addImage("rate_app.png");
    SpriteButton *pRate = SpriteButton::create(pTexture, this, callfuncO_selector(SettingScene::backClicked));
    CCPoint bottomLeft = VisibleRect::leftBottom();
    pBackButton->setPosition(ccp(bottomLeft.x + 70, bottomLeft.y + 60));
    pBackground->addChild(pBackButton);*/
    
    //Read default value
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    volume = pUserDefaults->getFloatForKey("volume");
    sound = pUserDefaults->getBoolForKey("sound");
    
    //Sound
    
    CCLabelTTF *pSoinuaLabel = CCLabelTTF::create("Soinua", fontList[0], 35.0);
    pSoinuaLabel->setPosition(ccp(120, 320));
    pSoinuaLabel->setColor(ccc3(63, 62, 62));
    pSoinuaLabel->setDimensions(CCSizeMake(250, 40));
    pSoinuaLabel->setHorizontalAlignment(kCCTextAlignmentRight);
    pBackground->addChild(pSoinuaLabel);
    
    CCMenu *pSoinuaMenu = CCMenu::create();
    CCMenuItemSprite* pMenuOn = CCMenuItemSprite::create(CCSprite::create("check_on.png"), NULL);
    CCMenuItemSprite* pMenuOff = CCMenuItemSprite::create(CCSprite::create("check_off.png"), NULL);
    CCMenuItemToggle* pToggle = CCMenuItemToggle::createWithTarget( this,
                                                              menu_selector(SettingScene::soundCallback),
                                                              pMenuOn,
                                                              pMenuOff,
                                                              NULL );
    pSoinuaMenu->addChild(pToggle);
    pSoinuaMenu->setPosition(ccp(290,320));
    pBackground->addChild(pSoinuaMenu);
    
    if (!sound) {
        pToggle->setSelectedIndex(1);
    }
    
    
    //Volume
    
    CCLabelTTF *pVolumeLabel = CCLabelTTF::create("Bolumena", fontList[0], 35.0);
    pVolumeLabel->setPosition(ccp(120, 250));
    pVolumeLabel->setColor(ccc3(63, 62, 62));
    pVolumeLabel->setHorizontalAlignment(kCCTextAlignmentRight);
    pVolumeLabel->setDimensions(CCSizeMake(250, 40));
    pBackground->addChild(pVolumeLabel);
    
    CCSprite *pVolOn = CCSprite::create("volume_on.png");
    pVolOn->setPosition(ccp(600,250));
    pBackground->addChild(pVolOn);

    
    CCSprite *pVolOff = CCSprite::create("volume_off.png");
    pVolOff->setPosition(ccp(290,250));
    pBackground->addChild(pVolOff);

    
    cocos2dExt::CCSlider* slider = cocos2dExt::CCSlider::SliderWithFiles("slider_track.png", "slider_knob.png", this,  menu_selector(SettingScene::sliderCallback));
    slider->setPosition(ccp(440,250));
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
    CCLabelTTF *pStadistics = CCLabelTTF::create("Estadistikak", fontList[0], 20.0);
    pStadistics->setPosition(ccp(345, 120));
    pStadistics->setColor(ccc3(63, 62, 62));
    pStadistics->setHorizontalAlignment(kCCTextAlignmentLeft);
    pBackground->addChild(pStadistics);
    
    std::string partidak = std::string("Partida jokatuta:   ");
    std::stringstream ss;
    ss << pUserDefaults->getIntegerForKey("partidak", 0);
    partidak.append(ss.str());
    
    CCLabelTTF *pPartidak = CCLabelTTF::create(partidak.c_str(), fontList[1], 20.0);
    pPartidak->setPosition(ccp(350, 90));
    pPartidak->setColor(ccc3(63, 62, 62));
    pPartidak->setHorizontalAlignment(kCCTextAlignmentLeft);
    pBackground->addChild(pPartidak);
    
    std::string denbora = std::string("Denbora totala:   ");
    denbora.append(getTimeWithFormat(pUserDefaults->getIntegerForKey("denbora")));
    
    CCLabelTTF *pDenbora = CCLabelTTF::create(denbora.c_str(), fontList[1], 20.0);
    pDenbora->setPosition(ccp(365, 60));
    pDenbora->setColor(ccc3(63, 62, 62));
    pDenbora->setHorizontalAlignment(kCCTextAlignmentLeft);
    pBackground->addChild(pDenbora);
        
    return true;
}

void SettingScene::soundCallback(CCObject *sender)
{
    CCMenuItemToggle *pToggle = (CCMenuItemToggle*) sender;
    int index = pToggle->getSelectedIndex();
    if (!index) {
        sound = true;
        /*if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/music.wav", true);*/
    } else {
        sound = false;
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
}

void SettingScene::sliderCallback(CCObject *sender)
{
    cocos2dExt::CCSlider *pSlider = (cocos2dExt::CCSlider*) sender;
    
    volume = pSlider->GetValue();
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
}

void SettingScene::backClicked(CCObject *sender)
{
    //Save preferences
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    pUserDefaults->setFloatForKey("volume", volume);
    pUserDefaults->setBoolForKey("sound", sound);
    pUserDefaults->flush();
    
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    if (getSceneReturnType() == kPopScene) {
         pDirector->popScene();
    } else {
        MenuScene *scene = MenuScene::create();
        CCTransitionFade *tscene = CCTransitionFade::create(0.5, scene);
        pDirector->replaceScene(tscene);
    }
   
}