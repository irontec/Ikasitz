
#include "Slider.h"

namespace cocos2dExt
{
    
	Slider::Slider():m_minValue(0),m_maxValue(0),m_value(0),m_target(NULL),m_selector(NULL),m_trackingTouch(false),m_trackTouchOutsideContent(false),m_evaluateFirstTouch(false),m_enabled(true),
    m_width(0),m_height(0),m_track(NULL),m_knob(NULL)
    {
        // do nothing
    }
    
	Slider* Slider::SliderWithFiles(const char* trackFile, const char* knobFile, Ref* target, SEL_MenuHandler selector)
	{
		Slider* slider = new Slider();
		if (slider->InitWithFiles(trackFile, knobFile, target, selector))
		{
			slider->autorelease();
			return slider;
		}
		else
		{
			CC_SAFE_DELETE(slider);
			return NULL;
		}
	}
    
	Slider* Slider::SliderWithSprites(Sprite* trackSprite, Sprite* knobSprite, Ref* target, SEL_MenuHandler selector)
	{
		Slider* slider = new Slider();
		if (slider->InitWithSprites(trackSprite, knobSprite, target, selector))
		{
			slider->autorelease();
			return slider;
		}
		else
		{
			CC_SAFE_DELETE(slider);
			return NULL;
		}
	}
    
	//! init from files
	bool Slider::InitWithFiles(const char* trackFile, const char* knobFile, Ref* target, SEL_MenuHandler selector)
	{
		//if (Node::init())
		{
            // TODO: Chcek this
//			m_track = Sprite::spriteWithFile(trackFile);
            m_track = Sprite::create();
            m_track->initWithFile(trackFile);
//			m_knob = Sprite::spriteWithFile(knobFile);
            m_knob = Sprite::create();
            m_knob->initWithFile(knobFile);
			if (!m_track || !m_knob) return false;
            
			m_target = target;
			m_selector = selector;
            
			m_minValue = 0;
			m_maxValue = 100;
            
			// NOTE: Attention the addChild sequence
			addChild(m_track);
			addChild(m_knob);
            
			m_width = m_track->getContentSize().width;
			m_height = 45;
            
			m_enabled = false;
            
			return true;
		}
		//return false;
	}
    
	//! init from sprites
	bool Slider::InitWithSprites(Sprite* trackSprite, Sprite* knobSprite, Ref* target, SEL_MenuHandler selector)
	{
		//if (Layer::init())
		{
			m_track = trackSprite;
			m_knob = knobSprite;
			if (!m_track || !m_knob) return false;
            
			m_target = target;
			m_selector = selector;
            
			m_minValue = 0;
			m_maxValue = 100;
            
			// NOTE: Attention the addChild sequence
			addChild(m_track);
			addChild(m_knob);
            
			m_width = m_track->getContentSize().width;
			m_height = 45;
            
			m_enabled = false;
            
			return true;
		}
		//return false;
	}
    
	void Slider::SetValue(float value)
	{
		if (!m_enabled) return;
        
		// clamp values
		value = value < m_minValue ? m_minValue : value;
		value = value > m_maxValue ? m_maxValue : value;
        
		m_value = value;
		float x = (m_value - m_minValue) / (m_maxValue - m_minValue) * m_width;
		m_knob->setPosition(Vec2(x - m_width * 0.5f, m_knob->getPosition().y));
        
		// do the callback
		if (m_target)
		{
			(m_target->*m_selector)(this);
		}
	}
    
	bool Slider::touchBegan(Touch* pTouch, Event *pEvent)
	{
		if (!isVisible() || !m_enabled) return false;
        
		Point touchPoint = getTouchPoint(pTouch);
        
		if (containsPoint(touchPoint))
		{
			if (m_evaluateFirstTouch)
			{
				setValueByX(convertToNodeSpace(touchPoint).x);
				m_trackingTouch = true;
				return true;
			}
			else
			{
				m_trackingTouch = knobTouched(touchPoint);
				return m_trackingTouch;
			}
            
		}
        
		return false;
	}
    
	void Slider::touchMoved(Touch* pTouch, Event *pEvent)
	{
		if (!isVisible() || !m_enabled) return;
        
		if (m_trackingTouch)
		{
			Point touchPos = getTouchPoint(pTouch);
            
			if (m_trackTouchOutsideContent)
			{
				setValueByX(convertToNodeSpace(touchPos).x);
			}
			else
			{
				if(containsPoint(touchPos))
				{
					setValueByX(convertToNodeSpace(touchPos).x);
				}
				else
				{
					m_trackingTouch = false;
				}
			}
		}
	}
    
	void Slider::touchEnded(Touch* pTouch, Event *pEvent)
	{
		m_trackingTouch = false;
	}
    
	void Slider::touchCancelled(Touch *pTouch, Event *pEvent)
	{
		m_trackingTouch = false;
	}
    
	void Slider::onEnter()
	{
		Node::onEnter();
//		CCDirector* pDirector = Director::getInstance();
//        pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        
        listener->onTouchBegan = [&](cocos2d::Touch* touch, Event* event)
        {
            return Slider::touchBegan(touch, event);
        };
        
        listener->onTouchMoved = [=](Touch* touch, Event* event)
        {
            Slider::touchMoved(touch, event);
        };
        
        listener->onTouchEnded = [=](Touch* touch, Event* event)
        {
            Slider::touchEnded(touch, event);
        };
        
        listener->onTouchCancelled = [=](Touch* touch, Event* event)
        {
            Slider::touchCancelled(touch, event);
        };

        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
	
	void Slider::onExit()
	{
//		CCDirector* pDirector = Director::getInstance();
//        pDirector->getTouchDispatcher()->removeDelegate(this);
        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
		Node::onExit();
	}
    
	// get touch point
	Point Slider::getTouchPoint(Touch* pTouch)
	{
		Point location = pTouch->getLocation();
		return location;
	}
    
	// check contain given point
	bool Slider::containsPoint(const Point& point)
	{
		Point pos = convertToNodeSpace(point);
		Rect rect = Rect(-m_width * 0.5f, -m_height * 0.5f, m_width, m_height); // NOTE: attention here
		if (rect.containsPoint(pos))
		{
			return true;
		}
		return false;
	}
    
	// set value by x coordinate
	void Slider::setValueByX(float xpos)
	{
		xpos = xpos < -m_width * 0.5f ? - m_width * 0.5f : xpos;
		xpos = xpos > m_width * 0.5f ? m_width * 0.5f : xpos;
		m_knob->setPosition(Vec2(xpos, m_knob->getPosition().y));
		m_value = (xpos + m_width * 0.5f) / m_width * (m_maxValue - m_minValue) + m_minValue;
		// do callback
		if (m_target)
		{
			(m_target->*m_selector)(this);
		}
	}
    
	bool Slider::knobTouched(const Point& point)
	{
		if (containsPoint(point)) 
		{
			Point pos = convertToNodeSpace(point);
			return  fabs(m_knob->getPosition().x - pos.x) < m_knob->getContentSize().width * 0.5f;
		}
		return false;
	}
    
}
