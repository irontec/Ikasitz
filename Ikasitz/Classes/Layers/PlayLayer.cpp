//
//  PlayLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#include "PlayLayer.h"
#include "../Helpers/VisibleRect.h"
#include "../Helpers/SpriteButton.h"
#include "../Helpers/Texture2DMutable.h"

PlayLayer::~PlayLayer()
{
    CC_SAFE_RELEASE(m_pMaskTexture);
}


bool PlayLayer::init()
{
    m_pLevel = m_pScene->getLevel();
    
    winSize = Director::getInstance()->getWinSize();
    
//    setTouchEnabled(true); // TODO: Check this ¿Handled by listener?
    
    loadImage();
    
    // TODO: Check this alternative to registerWithTouchDispatcher()
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [&](cocos2d::Touch* touch, Event* event)
    {
        return PlayLayer::touchBegan(touch, event);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void PlayLayer::showWord()
{
    
}

void PlayLayer::loadImage()
{
    std::string imagePath = FileUtils::getInstance()->getWritablePath().append(m_pLevel->getIrudia());
    
    m_pImage = Sprite::create(imagePath.c_str());
    m_pImage->setPosition(VisibleRect::center());
    
    Size imageSize = m_pImage->getContentSize();
    
    m_pImage->setScaleX(960/imageSize.width);
    m_pImage->setScaleY(640/imageSize.height);
    addChild(m_pImage);
    
    std::string maskPath = FileUtils::getInstance()->getWritablePath().append(m_pLevel->getMarkara());
    
    auto image = new Image();
    image->initWithImageFile(maskPath.c_str());
    
    m_pMaskTexture = new Texture2DMutable();
    m_pMaskTexture->initWithData((void*) image->getData(), image->getDataLen(), Texture2D::PixelFormat::RGBA8888, 960, 640, Size(960, 640));
    
    //m_pMaskTexture->initWithImage(image);
    //m_pMaskTexture->retain();
    image->autorelease();
}

//void PlayLayer::registerWithTouchDispatcher()
//{
//    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
//}

bool PlayLayer::touchBegan(Touch *touch, Event *event)
{
    
    Point touchLocation = touch->getLocation();
    
    float x = touchLocation.x;
    
    float y = winSize.height - (touchLocation.y);
    
    Point p = Point(x, y);
    Color4B color = m_pMaskTexture->pixelAt(p);
    log("R:%d G:%d B:%d AT POINT: (%f,%f)", color.r, color.g, color.b, x , y);
    
    m_pScene->screenTouched(color, touch);
    
	return true;
}
