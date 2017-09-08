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
#include "../Models/LevelModel.h"
#include "../Models/HitzaModel.h"
#include "../Helpers/IkasitzAPI.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/Texture2DMutable.h"

#define kHelpStartTime 6

using namespace cocos2d;

class PauseLayer;

class PlayScene : public Scene//, public CCTouchDelegate // TODO: Check if needed
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
    
    void screenTouched(Color4B color, Touch *touch);
    
private:
    IkasitzAPI *m_pAPI;
    Sprite *m_pWordContainer;
    Sprite *m_pClockContainer;
    Label *m_pWordLabel, *m_pClockLabel;
    int currentWordIndex;
    HitzaModel *m_pCurrentWord;
    bool soundEnabled;
    CC_SYNTHESIZE_READONLY(LevelModel*, m_pLevel, Level);
    CC_SYNTHESIZE(Vector<HitzaModel *> *, m_pHitza, Hitzak);
    Texture2DMutable *m_pMaskTexture;
    ParticleSystemQuad* m_pHelpParticle;
    PauseLayer *pPauseLayer;
    SpriteButton *pPauseButton;
    
    float particleEmissionRate;
    int seconds;
    bool isPaused;
    
    virtual void onEnter();
    void pauseClicked(Ref *sender);
    void resumeGame(Ref *sender);
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
