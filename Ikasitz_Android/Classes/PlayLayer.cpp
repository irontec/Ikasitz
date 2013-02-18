//
//  PlayLayer.cpp
//  Ikasitz
//
//  Created by Iker Mendilibar Fernandez on 22/01/13.
//
//

#include "PlayLayer.h"
#include "VisibleRect.h"
#include "SpriteButton.h"
#include "CCTexture2DMutable.h"

PlayLayer::~PlayLayer()
{
    CC_SAFE_RELEASE(m_pMaskTexture);
}


bool PlayLayer::init()
{
    m_pLevel = m_pScene->getLevel();
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    setTouchEnabled(true);
    
    loadImage();
    

    return true;
}

void PlayLayer::showWord()
{
    
}

void PlayLayer::loadImage()
{
    std::string imagePath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(m_pLevel->getIrudia());
    
    m_pImage = CCSprite::create(imagePath.c_str());
    m_pImage->setPosition(VisibleRect::center());
    
    CCSize imageSize = m_pImage->getContentSize();
    
    m_pImage->setScaleX(960/imageSize.width);
    m_pImage->setScaleY(640/imageSize.height);
    addChild(m_pImage);
    
    std::string maskPath = CCFileUtils::sharedFileUtils()->getWriteablePath().append(m_pLevel->getMarkara());
    
    CCImage *image = new CCImage();
    image->initWithImageFile(maskPath.c_str());
    
    m_pMaskTexture = new CCTexture2DMutable();
    m_pMaskTexture->initWithData((void*) image->getData(),  kCCTexture2DPixelFormat_RGBA8888, 960, 640, CCSizeMake(960, 640));
    
    //m_pMaskTexture->initWithImage(image);
    //m_pMaskTexture->retain();
    image->autorelease();
}

void PlayLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool PlayLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    
    CCPoint touchLocation = touch->getLocation();
    
    float x = touchLocation.x;
    
    float y = winSize.height - (touchLocation.y);
    
    CCPoint p = CCPointMake(x, y);
    ccColor4B color = m_pMaskTexture->pixelAt(p);
    CCLog("R:%d G:%d B:%d AT POINT: (%f,%f)", color.r, color.g, color.b, x , y);
    
    m_pScene->screenTouched(color, touch);
    
	return true;
}
