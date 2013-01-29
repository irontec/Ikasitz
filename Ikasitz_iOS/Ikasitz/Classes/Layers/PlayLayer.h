//
//  PlayLayer.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#ifndef __Ikasitz__PlayLayer__
#define __Ikasitz__PlayLayer__

#include "cocos2d.h"
#include "PlayScene.h"
#include "LevelModel.h"

USING_NS_CC;

class CCTexture2DMutable;

class PlayLayer : public CCLayer
{
public:
    CCTexture2DMutable *m_pMaskTexture;
    
    virtual bool init();
    
    static PlayLayer* create(PlayScene *pScene)
    {
        PlayLayer *pRet = new PlayLayer();
        pRet->m_pScene = pScene;
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
    
private:
    CCSprite *m_pImage;
    LevelModel *m_pLevel;
    PlayScene *m_pScene;
    CCSize winSize;
    
    void loadImage();
    void showWord();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};


#endif /* defined(__Ikasitz__PlayLayer__) */
