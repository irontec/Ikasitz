//
//  FinishLayer.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 24/01/13.
//
//

#ifndef __Ikasitz__FinishLayer__
#define __Ikasitz__FinishLayer__

#include "cocos2d.h"

USING_NS_CC;

class FinishLayer : public CCLayer
{
public:
	FinishLayer();
	~FinishLayer();
    
	virtual bool init();
	
    static FinishLayer* create(const char *pTime, char *pTitle)
    {
        FinishLayer *pRet = new FinishLayer();
        pRet->time = strdup(pTime);
        pRet->title = strdup(pTitle);
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
    char *time, *title;
    void restart(CCObject *sender);
    void allLevels(CCObject *sender);
    void nextLevel(CCObject *sender);
};

#endif /* defined(__Ikasitz__FinishLayer__) */
