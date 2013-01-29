/*!
 \file CCSlider.h
 \brief simple implementation of slider for cocos2d-x
 */

#ifndef __CCSLIDER_H__
#define __CCSLIDER_H__

#include "cocos2d.h"

USING_NS_CC;

namespace cocos2dExt
{
    
	//! NOTE: now CCSlider is just a simple node with touch handler
	class CCSlider : public CCNode, public CCTouchDelegate
	{
	public:
		//! static method to get the sliders
		static CCSlider* SliderWithFiles(const char* trackFile, const char* knobFile, CCObject* target, SEL_MenuHandler selector);
		static CCSlider* SliderWithSprites(CCSprite* trackSprite, CCSprite* knobSprite, CCObject* target, SEL_MenuHandler selector);
	public:
		//! default constructor
		CCSlider();
		//! init from files
		bool InitWithFiles(const char* trackFile, const char* knobFile, CCObject* target, SEL_MenuHandler selector);
		//! init from sprites
		bool InitWithSprites(CCSprite* trackSprite, CCSprite* knobSprite, CCObject* target, SEL_MenuHandler selector);
        
		//! get horizontal padding
		float GetHorizontalPadding() const { return (m_width - m_track->getContentSize().width) * 0.5f; }
		//! set horizontal padding
		void SetHorizontalPadding(float padding) { m_width = m_track->getContentSize().width + 2 * padding; }
		//! get slider value
		float GetValue() const { return m_value; }
		//! set slider value
		void SetValue(float value);
        
		//! normal setters & getters
		//////////////////////////////////////////////////////////////////////////
		float GetMinValue() const { return m_minValue; }
		void SetMinValue(float minValue) { m_minValue = minValue; }
		float GetMaxValue() const { return m_maxValue; }
		void SetMaxValue(float maxValue) { m_maxValue = maxValue; }
		bool GetTrackTouchOutsideContent() const { return m_trackTouchOutsideContent; }
		void SetTrackTouchOutsideContent(bool track) { m_trackTouchOutsideContent = track; }
		float GetHeight() const { return m_height; }
		void SetHeight(float height) { m_height = height; }
		bool GetEvaluateFirstTouch() const { return m_evaluateFirstTouch; }
		void SetEvaluateFirstTouch(bool eval) { m_evaluateFirstTouch = eval; }
		bool GetEnabled() const { return m_enabled; }
		void SetEnabled(bool enabled) { m_enabled = enabled; }
        //////////////////////////////////////////////////////////////////////////
        
		//! enter & exit method
		virtual void onEnter();
		virtual void onExit();
        
		//! touches handlers
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
        
	protected:
		// get touch point by touches set
		CCPoint getTouchPoint(CCTouch* touch);
		// check contain given point
		bool containsPoint(const CCPoint& point);
		// set value by x coordinate
		void setValueByX(float xpos);
		// check if knob is touched
		bool knobTouched(const CCPoint& point);
	protected:
		float m_minValue; //< min value
		float m_maxValue; //< max value
		float m_value; //< current value
        
		CCObject* m_target; //< target node
		SEL_MenuHandler m_selector; //< target selector
        
		bool m_trackingTouch; //< indicate if the slider is tracking touch
		bool m_trackTouchOutsideContent; //< indicate if the touch is outside of the content
		bool m_evaluateFirstTouch; //< first touch flag
		bool m_enabled; //< enable flag
        
		float m_width; //< width of the slider
		float m_height; //< height of the slider
        
		CCSprite* m_track; //< track sprite
		CCSprite* m_knob; //< knob sprite
	};
    
} // end of namespace cocos2dExt

#endif // __CCSLIDER_H__