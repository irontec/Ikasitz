//
//  SkyLayer.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#ifndef __Ikasitz__SkyLayer__
#define __Ikasitz__SkyLayer__

#include "cocos2d.h"

using namespace cocos2d;

class SkyLayer : public LayerGradient
{
public:
	SkyLayer();
	~SkyLayer();
    
	virtual bool init();
	CREATE_FUNC(SkyLayer);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    Sprite *pCloud1, *pCloud2, *pCloud3, *pCloud4, *pCloud5;
};

#endif /* defined(__Ikasitz__SkyLayer__) */
