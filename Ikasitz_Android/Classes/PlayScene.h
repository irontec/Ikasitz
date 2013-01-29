//
//  PlayScene.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#ifndef __Ikasitz__PlayScene__
#define __Ikasitz__PlayScene__

#include "cocos2d.h"
#include "LevelModel.h"
#include "HitzaModel.h"
#include "IkasitzAPI.h"
#include "SpriteButton.h"
#include "CCTexture2DMutable.h"

#define kHelpStartTime 6

USING_NS_CC;

class PauseLayer;

class PlayScene : public CCScene, public CCTouchDelegate
{
public:
	PlayScene(LevelModel *pLevel);
	~PlayScene();
    
	virtual bool init();
	
    static PlayScene* create(LevelModel *pLevel)
    {
        PlayScene *pRet = new PlayScene(pLevel);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else 
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
    float imageScale;
    bool isMoving;
    
    void screenTouched(ccColor4B color, CCTouch *touch);
    
private:
    IkasitzAPI *m_pAPI;
    CCSprite *m_pWordContainer;
    CCSprite *m_pClockContainer;
    CCLabelTTF *m_pWordLabel, *m_pClockLabel;
    int currentWordIndex;
    HitzaModel *m_pCurrentWord;
    bool soundEnabled;
    CC_SYNTHESIZE_READONLY(LevelModel*, m_pLevel, Level);
    CC_SYNTHESIZE(CCArray*, m_pHitza, Hitzak);
    CCTexture2DMutable *m_pMaskTexture;
    CCParticleSystemQuad* m_pHelpParticle;
    PauseLayer *pPauseLayer;
    SpriteButton *pPauseButton;
    
    float particleEmissionRate;
    int seconds;
    bool isPaused;
    
    virtual void onEnter();
    void pauseClicked(CCObject *sender);
    void resumeGame(CCObject *sender);
    void showHelp(float dt);
    void startHelp(float dt);
    void secondsCounter(float dt);
    void changeWord();
    void wordEnter();
    void wordExit();
    void finishWordEnter();
    void levelFinished();
    
};

#endif /* defined(__Ikasitz__PlayScene__) */
