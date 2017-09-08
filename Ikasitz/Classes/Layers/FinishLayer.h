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

using namespace cocos2d;

class FinishLayer : public Layer
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
    void restart(Ref *sender);
    void allLevels(Ref *sender);
    void nextLevel(Ref *sender);
};

#endif /* defined(__Ikasitz__FinishLayer__) */
