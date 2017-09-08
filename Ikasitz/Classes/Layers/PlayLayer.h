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
#include "../Scenes/PlayScene.h"
#include "../Models/LevelModel.h"

using namespace cocos2d;

class Texture2DMutable;

class PlayLayer : public Layer
{
public:
    Texture2DMutable *m_pMaskTexture;
    
    virtual bool init();
    
    ~PlayLayer();
    
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
    Sprite *m_pImage;
    LevelModel *m_pLevel;
    PlayScene *m_pScene;
    Size winSize;
    
    void loadImage();
    void showWord();
//    virtual void registerWithTouchDispatcher();
    virtual bool touchBegan(Touch* touch, Event* event);
};


#endif /* defined(__Ikasitz__PlayLayer__) */
