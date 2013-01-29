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

USING_NS_CC;

typedef enum
{
    kPopScene,
    kReplaScene
} ReturnType;

class SettingScene : public CCScene
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
    
    void backClicked(CCObject *sender);
    void sliderCallback(CCObject *sender);
    void soundCallback(CCObject *sender);
};

#endif /* defined(__Ikasitz__SettingScene__) */
