//
//  PlayScene.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#include "PlayScene.h"
#include "PlayLayer.h"
#include "VisibleRect.h"
#include "AppDelegate.h"
#include "PauseLayer.h"
#include "FinishLayer.h"
#include "SimpleAudioEngine.h"
#include "KategoriaModel.h"
#include "LevelModel.h"

using namespace CocosDenshion;

void shrinkFontSizeToFitWidth(CCLabelTTF *pLabel)
{
    float fontSize = 40.0f;
    pLabel->setFontSize(fontSize);
    
    float fontAdjustmentStep = 1.0f;
    float labelWith = pLabel->getContentSize().width;
    float labelMaxWith = 400.0f;
    
    while(labelWith > labelMaxWith)
    {
        fontSize -= fontAdjustmentStep;
        pLabel->setFontSize(fontSize);
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
    m_pCurrentWord = (HitzaModel*) m_pLevel->getHitzak()->objectAtIndex(currentWordIndex);
    setHitzak(m_pLevel->getHitzak());
    
    isMoving = false;
    isPaused = false;
    seconds = 0;
    particleEmissionRate = 1;
        
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    soundEnabled = pUserDefaults->getBoolForKey("sound");
}

bool PlayScene::init()
{
    PlayLayer *layer = PlayLayer::create(this);
    addChild(layer);
    
    m_pMaskTexture = layer->m_pMaskTexture;
 
    CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage("pause.png");
    pPauseButton = SpriteButton::create(pTexture, this, callfuncO_selector(PlayScene::pauseClicked));
    CCPoint bottomLeft = VisibleRect::leftBottom();
    pPauseButton->setPosition(ccp(bottomLeft.x + 70, bottomLeft.y + 60));
    addChild(pPauseButton);
    
    
    m_pHelpParticle = CCParticleSystemQuad::create("Galaxy.plist");
    m_pHelpParticle->stopSystem();
    addChild(m_pHelpParticle);
    
    m_pWordContainer = CCSprite::create("word_container.png");
    m_pWordContainer->setPosition(ccp(VisibleRect::bottom().x , VisibleRect::bottom().y - m_pWordContainer->getContentSize().height/2));
    m_pWordLabel = CCLabelTTF::create("", fontList[0], 40.0);
    m_pWordLabel->setAnchorPoint(ccp(0.5,1));
    m_pWordLabel->setPosition(ccp(m_pWordContainer->getContentSize().width/2, m_pWordContainer->getContentSize().height/2 + 10));
    m_pWordLabel->setColor(ccc3(63, 62, 62));
    //m_pWordLabel->setContentSize(CCSizeMake(400, 0));
    m_pWordLabel->setDimensions(CCSizeMake(0, 50));
    m_pWordContainer->addChild(m_pWordLabel);
    addChild(m_pWordContainer);

   
    m_pClockContainer = CCSprite::create("clock.png");
    m_pClockContainer->setPosition(ccp(VisibleRect::rightTop().x - 100, VisibleRect::rightTop().y - 50));
    m_pClockLabel = CCLabelTTF::create("", fontList[1], 30.0);
    m_pClockLabel->setAnchorPoint(ccp(0.5,1));
    m_pClockLabel->setString("00:00");
    m_pClockLabel->setPosition(ccp(m_pClockContainer->getContentSize().width/2, m_pClockContainer->getContentSize().height/2 + m_pClockLabel->getContentSize().height/2 + 5));
    m_pClockLabel->setColor(ccc3(63, 62, 62));
    //m_pWordLabel->setContentSize(CCSizeMake(400, 0));
    //m_pClockLabel->setDimensions(CCSizeMake(0, 50));
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
    
    CCScene::onEnter();
    
    if (isPaused) {
        this->pauseSchedulerAndActions();
    }
    //Read preferences
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    soundEnabled = pUserDefaults->getBoolForKey("sound");
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(pUserDefaults->getFloatForKey("volume"));
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(pUserDefaults->getFloatForKey("volume"));
}

void PlayScene::screenTouched(ccColor4B color, CCTouch *touch)
{
    ccColor3B wordColor = m_pCurrentWord->getKolorea();
    
    if (!isMoving && !isPaused && !pPauseButton->containsTouchLocation(touch)) {
        if (wordColor.r == color.r && wordColor.g == color.g && wordColor.b == color.b) {
            if (soundEnabled) {
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/bien.wav");
            }
            wordExit();
        } else if (pPauseButton->containsTouchLocation(touch)) {
            //BUG
        } else {
            if (soundEnabled) {
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/mal.wav");
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
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int counth = m_pMaskTexture->getPixelsHigh();
    int countw = m_pMaskTexture->getPixelsWide();
    
    ccColor3B wordColor = m_pCurrentWord->getKolorea();
    
    CCArray *list = CCArray::create();
    
    for (int i = 0; i < countw; i++) {
        for (int j = 0; j < counth; j++) {
            CCPoint *p = new CCPoint(i,j);
            ccColor4B color = m_pMaskTexture->pixelAt(*p);
            if (color.r == wordColor.r && color.g == wordColor.g && color.b == wordColor.b) {
                list->addObject(p);
            }
        }
    }
    
    int numElem = list->count();
    
    CCPoint left = ccp(5000, 5000);
    CCPoint right = ccp(0, 0);
    
    for (int k = 0; k < numElem; k++) {
        CCPoint *tmp = (CCPoint*) list->objectAtIndex(k);
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
    
    CCPoint media = ccpMidpoint(right, left);
    m_pHelpParticle->setPosition(media);
    list->release();

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
        
    if (currentWordIndex == m_pLevel->getHitzak()->count()) {
        isMoving = false;
        levelFinished();
    } else {
        
        m_pCurrentWord = (HitzaModel*) m_pLevel->getHitzak()->objectAtIndex(currentWordIndex);
        m_pWordLabel->setString(m_pCurrentWord->getIzena());
        shrinkFontSizeToFitWidth(m_pWordLabel);
        wordEnter();
    }
}

void PlayScene::finishWordEnter()
{
    isMoving = false;
    if (soundEnabled) {
        std::string soundPath(CCFileUtils::sharedFileUtils()->getWriteablePath());
        soundPath.append(m_pCurrentWord->getSoinua());
        SimpleAudioEngine::sharedEngine()->playEffect(soundPath.c_str());
    }
    
    this->scheduleOnce(schedule_selector(PlayScene::startHelp), kHelpStartTime);
}

void PlayScene::wordEnter()
{
    isMoving = true;

    CCPoint p = ccp(VisibleRect::bottom().x, VisibleRect::bottom().y + m_pWordContainer->getContentSize().height/2);
    CCFiniteTimeAction *actionMove = CCMoveTo::create(1.0, p);
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(PlayScene::finishWordEnter));
    //CCFiniteTimeAction *actionDelay = CCDelayTime::create(1.0);
    m_pWordContainer->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
    
    this->schedule(schedule_selector(PlayScene::secondsCounter), 1.0f);
}

void PlayScene::wordExit()
{
    isMoving = true;
    particleEmissionRate = 1;
    
    this->unscheduleAllSelectors();
    
    if (m_pHelpParticle != NULL)
    {
        m_pHelpParticle->stopSystem();
    }
    
    CCPoint p = ccp(VisibleRect::bottom().x , VisibleRect::bottom().y - m_pWordContainer->getContentSize().height/2);
    CCFiniteTimeAction *actionMove = CCMoveTo::create(1.0, p);
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(PlayScene::changeWord));
    CCFiniteTimeAction *actionDelay = CCDelayTime::create(2.0);
    m_pWordContainer->runAction(CCSequence::create(actionDelay, actionMove, actionMoveDone, NULL));
}

void PlayScene::levelFinished()
{
    isPaused = true;
    
    this->unscheduleAllSelectors();
    
    CCUserDefault *pUserDefaults = CCUserDefault::sharedUserDefault();
    int partidak = pUserDefaults->getIntegerForKey("partidak");
    partidak++;
    pUserDefaults->setIntegerForKey("partidak", partidak);
    int denbora = pUserDefaults->getIntegerForKey("denbora");
    denbora+=seconds;
    pUserDefaults->setIntegerForKey("denbora", denbora);
    pUserDefaults->flush();
    
    IkasitzAPI *pAPI = IkasitzAPI::sharedInstance();    
    LevelModel *pLevel = (LevelModel*) pAPI->getSelectedKategoria()->getPantailak()->objectAtIndex(pAPI->currentLevel);
    
    FinishLayer *pFinishLayer = FinishLayer::create(m_pClockLabel->getString(), pLevel->getIzena());
    pFinishLayer->setPosition(VisibleRect::center());
    addChild(pFinishLayer);
}

void PlayScene::pauseClicked(CCObject *sender)
{
    isPaused = true;
    
    this->pauseSchedulerAndActions();
    
    pPauseButton->setEnabled(false);
    pPauseLayer = PauseLayer::create(this);
    pPauseLayer->setPosition(VisibleRect::center());
    addChild(pPauseLayer);
}

//Pause Layer callbacks
void PlayScene::resumeGame(CCObject *sender)
{
    isPaused = false;
    
    pPauseButton->setEnabled(true);
    pPauseLayer->removeAllChildrenWithCleanup(true);
    this->resumeSchedulerAndActions();
    this->schedule(schedule_selector(PlayScene::secondsCounter), 1.0f);
}