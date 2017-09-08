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

#include "../Helpers/IkasitzAPI.h"
#include "../Helpers/ScrollLayer.h"

using namespace cocos2d;
USING_NS_CC_EXT;

class UniverseScene : public Scene, public ScrollLayerDelegate
{
public:
	UniverseScene();
	~UniverseScene();
    
	virtual bool init();
	CREATE_FUNC(UniverseScene);
    
private:
    Vector<KategoriaModel *> *m_pUniverses;
    Vector<Sprite *> *m_pBolasSprites;
    IkasitzAPI *m_pAPI;
    float imageScale;
    
    void createUniversesLayer();
    
    void universeClicked(Ref *sender);
    void backClicked(Ref *sender);
    
    void scrollLayerScrolledToPageNumber(ScrollLayer* sender, unsigned int page);
    
};

#endif /* defined(__Ikasitz__UniverseScene__) */
