//
//  UniverseScene.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#ifndef __Ikasitz__UniverseScene__
#define __Ikasitz__UniverseScene__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "IkasitzAPI.h"
#include "CCScrollLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class UniverseScene : public CCScene, public CCScrollLayerDelegate
{
public:
	UniverseScene();
	~UniverseScene();
    
	virtual bool init();
	CREATE_FUNC(UniverseScene);
    
private:
    CCArray *m_pUniverses;
    CCArray *m_pBolasSprites;
    IkasitzAPI *m_pAPI;
    float imageScale;
    
    void createUniversesLayer();
    
    void universeClicked(CCObject *sender);
    void backClicked(CCObject *sender);
    
    void scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page);
    
};

#endif /* defined(__Ikasitz__UniverseScene__) */
