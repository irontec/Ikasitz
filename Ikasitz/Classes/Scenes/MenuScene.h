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
#include "../Helpers/SpriteButton.h"

using namespace cocos2d;

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();
    
	virtual bool init();
	CREATE_FUNC(MenuScene);
    
private:
    SpriteButton *pPlayButton, *pInfoButton, *pSettingsButton;
    
    void playClicked(Ref *sender);
    void infoClicked(Ref *sender);
    void settingsClicked(Ref *sender);

};

#endif /* defined(__Ikasitz__MenuScene__) */
