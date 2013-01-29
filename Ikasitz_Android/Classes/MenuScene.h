//
//  MenuScene.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#ifndef __Ikasitz__MenuScene__
#define __Ikasitz__MenuScene__

#include "cocos2d.h"
#include "SpriteButton.h"

USING_NS_CC;

class MenuScene : public CCScene
{
public:
	MenuScene();
	~MenuScene();
    
	virtual bool init();
	CREATE_FUNC(MenuScene);
    
private:
    SpriteButton *pPlayButton, *pInfoButton, *pSettingsButton;
    
    void playClicked(CCObject *sender);
    void infoClicked(CCObject *sender);
    void settingsClicked(CCObject *sender);

};

#endif /* defined(__Ikasitz__MenuScene__) */
