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
    cocos2d::CCLayerGradient::onEnter();

    /*CCActionInterval* move = CCMoveTo::create(5.0, ccp(765, 350));
    CCActionInterval* setPosition = CCMoveTo::create(0.0, ccp(765, 350));
    CCRepeatForever *repeat = CCRepeatForever::create((CCActionInterval*)CCSequence::create(move, setPosition, NULL));
    pCloud1->runAction(repeat);*/
}

void SkyLayer::onExit()
{
    cocos2d::CCLayerGradient::onExit();
}

bool SkyLayer::init()
{
    if (!initWithColor(ccc4(127, 188, 255, 250), ccc4(204, 233, 255, 255)))
    {
        return false;
    }

    pCloud1 = CCSprite::create("cloud_1.png");
    pCloud1->setPosition(ccp(765, 350));
    addChild(pCloud1);
    
    pCloud2 = CCSprite::create("cloud_2.png");
    pCloud2->setPosition(ccp(165, 260));
    addChild(pCloud2);
    
    pCloud3 = CCSprite::create("cloud_3.png");
    pCloud3->setPosition(ccp(780, 550));
    addChild(pCloud3);
    
    pCloud4 = CCSprite::create("cloud_4.png");
    pCloud4->setPosition(ccp(245,405));
    addChild(pCloud4);
    
    pCloud5 = CCSprite::create("cloud_5.png");
    pCloud5->setPosition(ccp(210,560));
    addChild(pCloud5);
    
    
    CCLayerColor *pTrama = CCLayerColor::create(ccc4(0,0,0,45));
    addChild(pTrama);
    
    return true;
}
