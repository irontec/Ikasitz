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
#include "../Helpers/IkasitzAPI.h"
#include "../Helpers/ScrollLayer.h"
#include "../Models/KategoriaModel.h"

using namespace cocos2d;

class LevelScene : public Scene, public ScrollLayerDelegate
{
public:
	LevelScene();
	~LevelScene();
    
	virtual bool init();
	CREATE_FUNC(LevelScene);
    
private:
    IkasitzAPI *m_pAPI;
    Vector<Sprite *> *m_pBolasSprites;
    KategoriaModel *m_pKategoria;
    float imageScale;
    
    void levelClicked(Ref *sender);
    void backClicked(Ref *sender);
    void scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page);
    void createLevelsLayer();
    
};

#endif /* defined(__Ikasitz__LevelScene__) */
