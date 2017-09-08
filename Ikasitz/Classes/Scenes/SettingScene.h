//
//  SettingScene.h
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 23/01/13.
//
//

#ifndef __Ikasitz__SettingScene__
#define __Ikasitz__SettingScene__

#include "cocos2d.h"

using namespace cocos2d;

typedef enum
{
    kPopScene,
    kReplaScene
} ReturnType;

class SettingScene : public Scene
{    
public:
	SettingScene();
	~SettingScene();
    
	virtual bool init();
	CREATE_FUNC(SettingScene);
    
    CC_SYNTHESIZE(ReturnType, m_ReturnType, SceneReturnType);
    
private:
    bool sound;
    float volume;
    
    void backClicked(Ref *sender);
    void sliderCallback(Ref *sender);
    void soundCallback(Ref *sender);
};

#endif /* defined(__Ikasitz__SettingScene__) */
