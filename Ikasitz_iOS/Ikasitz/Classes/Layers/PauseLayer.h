//
//  PauseLayer.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#ifndef __Ikasitz__PauseLayer__
#define __Ikasitz__PauseLayer__

#include "cocos2d.h"
#include "PlayScene.h"

USING_NS_CC;

class PauseLayer : public CCLayer
{
public:
	PauseLayer();
	~PauseLayer();
    
	virtual bool init();
	
    static PauseLayer* create(PlayScene *pScene)
    {
        PauseLayer *pRet = new PauseLayer();
        pRet->m_pScene = pScene;
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        } \
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }
    
private:
    PlayScene *m_pScene;
    
    void settings(CCObject *sender);
    void restart(CCObject *sender);
    void allLevels(CCObject *sender);
    void nextLevel(CCObject *sender);
};

#endif /* defined(__Ikasitz__PauseLayer__) */
