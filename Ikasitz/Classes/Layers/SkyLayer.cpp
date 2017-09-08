//
//  SkyLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 21/01/13.
//
//

#include "SkyLayer.h"


SkyLayer::~SkyLayer()
{

}

SkyLayer::SkyLayer()
{

}

void SkyLayer::onEnter()
{
    cocos2d::LayerGradient::onEnter();

    /*ActionInterval* move = MoveTo::create(5.0, Vec2(765, 350));
    ActionInterval* setPosition = MoveTo::create(0.0, Vec2(765, 350));
    RepeatForever *repeat = RepeatForever::create((ActionInterval*)Sequence::create(move, setPosition, NULL));
    pCloud1->runAction(repeat);*/
}

void SkyLayer::onExit()
{
    cocos2d::LayerGradient::onExit();
}

bool SkyLayer::init()
{
    if (!initWithColor(Color4B(127, 188, 255, 250), Color4B(204, 233, 255, 255)))
    {
        return false;
    }

    pCloud1 = Sprite::create("cloud_1.png");
    pCloud1->setPosition(Vec2(765, 350));
    addChild(pCloud1);
    
    pCloud2 = Sprite::create("cloud_2.png");
    pCloud2->setPosition(Vec2(165, 260));
    addChild(pCloud2);
    
    pCloud3 = Sprite::create("cloud_3.png");
    pCloud3->setPosition(Vec2(780, 550));
    addChild(pCloud3);
    
    pCloud4 = Sprite::create("cloud_4.png");
    pCloud4->setPosition(Vec2(245,405));
    addChild(pCloud4);
    
    pCloud5 = Sprite::create("cloud_5.png");
    pCloud5->setPosition(Vec2(210,560));
    addChild(pCloud5);
    
    
    LayerColor *pTrama = LayerColor::create(Color4B(0,0,0,45));
    addChild(pTrama);
    
    return true;
}
