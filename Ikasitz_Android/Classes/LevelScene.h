//
//  LevelScene.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#ifndef __Ikasitz__LevelScene__
#define __Ikasitz__LevelScene__

#include "cocos2d.h"
#include "IkasitzAPI.h"
#include "CCScrollLayer.h"
#include "KategoriaModel.h"

USING_NS_CC;

class LevelScene : public CCScene, public CCScrollLayerDelegate
{
public:
	LevelScene();
	~LevelScene();
    
	virtual bool init();
	CREATE_FUNC(LevelScene);
    
private:
    IkasitzAPI *m_pAPI;
    CCArray *m_pBolasSprites;
    KategoriaModel *m_pKategoria;
    float imageScale;
    
    void levelClicked(CCObject *sender);
    void backClicked(CCObject *sender);
    void scrollLayerScrolledToPageNumber(CCScrollLayer* sender, unsigned int page);
    void createLevelsLayer();
    
};

#endif /* defined(__Ikasitz__LevelScene__) */
