//
//  MyGestureEventListener.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/1.
//
//

#ifndef __ShaderDemo__MyGestureEventListener__
#define __ShaderDemo__MyGestureEventListener__

#include "cocos2d.h"
#include "MyTouchEventListener.h"

USING_NS_CC;

#define MyGestureEventListener_Angle_Noise 0.02
#define MyGestureEventListener_Distance_Noise 4

enum MyGestureEvent{
    MyGestureEvent_Spin, MyGestureEvent_Stretch, MyGestureEvent_TouchBegan, MyGestureEvent_None
};

class MyGestureEventListener : public cocos2d::EventListenerTouchAllAtOnce{
    
public:
    static MyGestureEventListener* createWithTarget(cocos2d::Node* node);
    
public:
    std::function<void(const Vec2& preTouchPosition, const Vec2& curTouchPosition, MyGestureEvent)> onDragListener;
    
public:
    std::function<void(float angle, MyGestureEvent)> onSpinListener;
    std::function<void(float preDistance, float curDistance, MyGestureEvent)> onStretchListener;
    
public:
    void onTouchesBeganCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMovedCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEndedCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesCancelledCallback(const std::vector<Touch*>& touches, Event* event);
    
public:
    virtual ~MyGestureEventListener();
    
protected:
    bool initWithTarget(cocos2d::Node* node);
    void reset();
    
protected:
    void handleSingleTouchBegan(Touch* touch, Event* event);
    void handleSingleTouchMoved(Touch* touch, Event* event);
    void handleSingleTouchEnded(Touch* touch, Event* event);
    
private:
    bool isSingleTouches(){ return m_touchCount == 1; }
    
private:
    Vec2 m_preSingleTouchPosition;
    int m_touchCount;
    
private:
    cocos2d::Node* m_target;
    
private:
    std::vector<Vec2> m_preTouchesPosition;
private:
    float m_preDistance;
    Vec2 m_preDirection;
    
private:
    MyTouchEventListener* m_touchEventListener;
};


#endif /* defined(__ShaderDemo__MyGestureEventListener__) */
