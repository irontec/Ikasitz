//
//  PlayScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#include "PlayScene.h"
#include "../Layers/PlayLayer.h"
#include "../Helpers/VisibleRect.h"
#include "../AppDelegate.h"
#include "../Layers/PauseLayer.h"
#include "../Layers/FinishLayer.h"
#include "SimpleAudioEngine.h"
#include "../Models/KategoriaModel.h"
#include "../Models/LevelModel.h"

using namespace CocosDenshion;

void shrinkFontSizeToFitWidth(Label *pLabel)
{
    // TODO: Check this
//    float fontSize = 40.0f;
//    pLabel->setFontSize(fontSize);
    float fontSize = pLabel->getTTFConfig().fontSize;
    
    float fontAdjustmentStep = 1.0f;
    float labelWith = pLabel->getContentSize().width;
    float labelMaxWith = 400.0f;
    
    while(labelWith > labelMaxWith)
    {
        fontSize -= fontAdjustmentStep;
//        pLabel->setFontSize(fontSize);
        auto ttfConfig = pLabel->getTTFConfig();
        ttfConfig.fontSize = fontSize;
        pLabel->setTTFConfig(ttfConfig);
        
        labelWith = pLabel->getContentSize().width;
        if( fontSize < 5)
        {
            break;
        }
    }
}

char* getTimeWithFormat(int seconds)
{
    char secText[3];
    char minText[3];
    
    int minutes = seconds / 60;
    int sec = seconds % 60;
    
    if (minutes < 10) {
        sprintf(minText, "0%d", minutes);
    } else {
        sprintf(minText, "%d", minutes);
    }
    
    if (sec < 10) {
        sprintf(secText, "0%d", sec);
    } else {
        sprintf(secText, "%d", sec);
    }
    
    char *clockText;
    clockText = (char*) malloc(sizeof(char) * 5);
    sprintf(clockText, "%s:%s", minText, secText);
    
    return clockText;
}


PlayScene::~PlayScene()
{
    
}

PlayScene::PlayScene(LevelModel *pLevel)
{
    m_pLevel = pLevel;
    
    m_pAPI = IkasitzAPI::sharedInstance();
    imageScale = m_pAPI->getScale();
    
    currentWordIndex = 0;
    m_pCurrentWord = (HitzaModel*) m_pLevel->getHitzak()->at(currentWordIndex);
    setHitzak(m_pLevel->getHitzak());
    
    isMoving = false;
    isPaused = false;
    seconds = 0;
    particleEmissionRate = 1;
        
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    auto pUserDefaults = UserDefault::getInstance();
    soundEnabled = pUserDefaults->getBoolForKey("sound");
}

bool PlayScene::init()
{
    PlayLayer *layer = PlayLayer::create(this);
    addChild(layer);
    
    m_pMaskTexture = layer->m_pMaskTexture;
 
    Texture2D *pTexture = Director::getInstance()->getTextureCache()->addImage("pause.png");
    pPauseButton = SpriteButton::create(pTexture, CC_CALLBACK_1(PlayScene::pauseClicked, this));
    Point bottomLeft = VisibleRect::leftBottom();
    pPauseButton->setPosition(Vec2(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pPauseButton);
    
    
    m_pHelpParticle = ParticleSystemQuad::create("Galaxy.plist");
    m_pHelpParticle->stopSystem();
    addChild(m_pHelpParticle);    
    
    
    pTexture = Director::getInstance()->getTextureCache()->addImage("word_container.png");
    m_pWordContainer = Sprite::createWithTexture(pTexture);
    m_pWordContainer->setPosition(Vec2(VisibleRect::bottom().x , VisibleRect::bottom().y - m_pWordContainer->getContentSize().height/2));
    m_pWordLabel = Label::createWithTTF("", fontList[0], 40.0);

    m_pWordLabel->setAnchorPoint(Vec2(0.5,1));
    m_pWordLabel->setPosition(Vec2(m_pWordContainer->getContentSize().width/2, m_pWordContainer->getContentSize().height/2 + 10));
    m_pWordLabel->setColor(Color3B(63, 62, 62));
    //m_pWordLabel->setContentSize(Size(400, 0));
    m_pWordLabel->setDimensions(0, 50);
    
    m_pWordContainer->addChild(m_pWordLabel);
    addChild(m_pWordContainer);

   
    m_pClockContainer = Sprite::create("clock.png");
    m_pClockContainer->setPosition(Vec2(VisibleRect::rightTop().x - 100, VisibleRect::rightTop().y - 50));
    m_pClockLabel = Label::createWithTTF("", fontList[1], 30.0);
    m_pClockLabel->setAnchorPoint(Vec2(0.5,1));
    m_pClockLabel->setString("00:00");
    m_pClockLabel->setPosition(Vec2(m_pClockContainer->getContentSize().width/2, m_pClockContainer->getContentSize().height/2 + m_pClockLabel->getContentSize().height/2 + 5));
    m_pClockLabel->setColor(Color3B(63, 62, 62));
    //m_pWordLabel->setContentSize(Size(400, 0));
    //m_pClockLabel->setDimensions(0, 50);
    m_pClockContainer->addChild(m_pClockLabel);
    addChild(m_pClockContainer);
    
    
    //Load first word
    m_pWordLabel->setString(m_pCurrentWord->getIzena());

    shrinkFontSizeToFitWidth(m_pWordLabel);
    wordEnter();
    
    return true;
}

void PlayScene::onEnter()
{
    //Para resolver el problema del pop de la escena settings
    
    Scene::onEnter();
    
    if (isPaused) {
        this->pause();
    }
    //Read preferences
    auto pUserDefaults = UserDefault::getInstance();
    soundEnabled = pUserDefaults->getBoolForKey("sound");
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(pUserDefaults->getFloatForKey("volume"));
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(pUserDefaults->getFloatForKey("volume"));
}

void PlayScene::screenTouched(Color4B color, Touch *touch)
{
    auto wordColor = m_pCurrentWord->getKolorea();

    if (m_pWordContainer->getBoundingBox().containsPoint(convertTouchToNodeSpace(touch))) {
        if (soundEnabled) {
            std::string soundPath(FileUtils::getInstance()->getWritablePath());
            soundPath.append(m_pCurrentWord->getSoinua());
            SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
            
            //Puede pasar que la caja este por encima del objeto que hay qu tocar. se comprueba si es así y se da por bueno
            if (wordColor.r == color.r && wordColor.g == color.g && wordColor.b == color.b) {
                if (soundEnabled) {
                    SimpleAudioEngine::getInstance()->playEffect("sounds/bien.wav");
                }
                wordExit();
            }
        }
    } else if (!isMoving && !isPaused && !pPauseButton->containsTouchLocation(touch)) {
        if (wordColor.r == color.r && wordColor.g == color.g && wordColor.b == color.b) {
            if (soundEnabled) {
                SimpleAudioEngine::getInstance()->playEffect("sounds/bien.wav");
            }
            wordExit();
        } else {
            if (soundEnabled) {
                SimpleAudioEngine::getInstance()->playEffect("sounds/mal.wav");
            }
        }
    }
}

void PlayScene::secondsCounter(float dt)
{
    seconds++;
    char *clockText = getTimeWithFormat(seconds);
    m_pClockLabel->setString(clockText);
    free(clockText);
}

void PlayScene::startHelp(float dt)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    int counth = m_pMaskTexture->getPixelsHigh();
    int countw = m_pMaskTexture->getPixelsWide();
    
    auto wordColor = m_pCurrentWord->getKolorea();
    
    auto list = new std::vector<Point *>();
    
    for (int i = 0; i < countw; i++) {
        for (int j = 0; j < counth; j++) {
            auto p = new Point(i,j);
            Color4B color = m_pMaskTexture->pixelAt(*p);
            if (color.r == wordColor.r && color.g == wordColor.g && color.b == wordColor.b) {
                list->push_back(p);
            }
            // TODO: Check this (Memory leak?)
//            CC_SAFE_RELEASE(p);
        }
    }
    
    int numElem = (int) list->size();
    
    Point left = Vec2(5000, 5000);
    Point right = Vec2(0, 0);
    
    for (int k = 0; k < numElem; k++) {
        auto tmp = list->at(k);
        int x = tmp->x;
        int y = winSize.height - tmp->y;
        
        if (x < left.x) {
            left.x = x;
        } else {
            if (x > right.x) {
                right.x = x;
            }
        }
        
        if (y < left.y) {
            left.y = y;
        } else {
            if (y > right.y) {
                right.y = y;
            }
        }
    }
    
    
    Point media = right.getMidpoint(left); // TODO: ccpMidpoint(right, left);
    m_pHelpParticle->setPosition(media);

    this->schedule(schedule_selector(PlayScene::showHelp), 4);
}

void PlayScene::showHelp(float dt)
{
    m_pHelpParticle->setEmissionRate(particleEmissionRate);
    
    if (!m_pHelpParticle->isActive()) {
        m_pHelpParticle->resetSystem();
    }

    particleEmissionRate++;
}


void PlayScene::changeWord()
{
    currentWordIndex++;
        
    if (currentWordIndex == (int) m_pLevel->getHitzak()->size()) {
        isMoving = false;
        levelFinished();
    } else {
        
        m_pCurrentWord = m_pLevel->getHitzak()->at(currentWordIndex);
        m_pWordLabel->setString(m_pCurrentWord->getIzena());
        shrinkFontSizeToFitWidth(m_pWordLabel);
        wordEnter();
    }
}

void PlayScene::finishWordEnter()
{
    isMoving = false;
    if (soundEnabled) {
        std::string soundPath(FileUtils::getInstance()->getWritablePath());
        soundPath.append(m_pCurrentWord->getSoinua());
        SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
    }
    
    this->scheduleOnce(schedule_selector(PlayScene::startHelp), kHelpStartTime);
}

void PlayScene::wordEnter()
{
    isMoving = true;

    Point p = Vec2(VisibleRect::bottom().x, VisibleRect::bottom().y + m_pWordContainer->getContentSize().height/2);
    FiniteTimeAction *actionMove = MoveTo::create(1.0, p);
    // TODO: Check this
//    FiniteTimeAction* actionMoveDone = CC_CALLBACK_1(PlayScene::finishWordEnter, this);
    
    //FiniteTimeAction *actionDelay = DelayTime::create(1.0);
    //auto callback = CC_CALLBACK_1(ScrollLayer::moveToPageEnded, this);
    auto callback = CallFunc::create( [this]() {
        this->finishWordEnter();
    });
    m_pWordContainer->runAction(Sequence::create(actionMove, callback, nullptr));
    
    
    this->schedule(schedule_selector(PlayScene::secondsCounter), 1.0f);
}

void PlayScene::wordExit()
{
    isMoving = true;
    particleEmissionRate = 1;
    
    this->unscheduleAllCallbacks();
    
    if (m_pHelpParticle != NULL)
    {
        m_pHelpParticle->stopSystem();
    }
    
    Point p = Vec2(VisibleRect::bottom().x , VisibleRect::bottom().y - m_pWordContainer->getContentSize().height/2);
    FiniteTimeAction *actionMove = MoveTo::create(1.0, p);
    // TODO: Check this
//    FiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(PlayScene::changeWord));
    FiniteTimeAction *actionDelay = DelayTime::create(2.0);
    auto callback = CallFunc::create( [this]() {
        this->changeWord();
    });
    m_pWordContainer->runAction(Sequence::create(actionDelay, actionMove, callback, nullptr));
}

void PlayScene::levelFinished()
{
    isPaused = true;
    
    pPauseButton->setEnabled(false);
    
    this->unscheduleAllCallbacks();
    
    auto pUserDefaults = UserDefault::getInstance();
    int partidak = pUserDefaults->getIntegerForKey("partidak");
    partidak++;
    pUserDefaults->setIntegerForKey("partidak", partidak);
    int denbora = pUserDefaults->getIntegerForKey("denbora");
    denbora+=seconds;
    pUserDefaults->setIntegerForKey("denbora", denbora);
    pUserDefaults->flush();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();    
    auto pLevel = pAPI->getSelectedKategoria()->getPantailak()->at(pAPI->currentLevel);
    FinishLayer *pFinishLayer = FinishLayer::create(m_pClockLabel->getString().c_str(), pLevel->getIzena());// TODO: Check c_str()
    pFinishLayer->setPosition(VisibleRect::center());
    addChild(pFinishLayer);
}

void PlayScene::pauseClicked(Ref *sender)
{
    isPaused = true;
    
    this->pause();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
    
    pPauseButton->setEnabled(false);
    pPauseLayer = PauseLayer::create(this);
    pPauseLayer->setPosition(VisibleRect::center());
    addChild(pPauseLayer);
}

//Pause Layer callbacks
void PlayScene::resumeGame(Ref *sender)
{
    isPaused = false;
    
    pPauseButton->setEnabled(true);
    pPauseLayer->removeAllChildrenWithCleanup(true);
    this->resume();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
    this->schedule(schedule_selector(PlayScene::secondsCounter), 1.0f);
}
