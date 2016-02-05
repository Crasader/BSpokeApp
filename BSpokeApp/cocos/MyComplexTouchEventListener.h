//
//  MyComplexTouchEventListener.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/1.
//
//

#ifndef __ShaderDemo__MyComplexTouchEventListener__
#define __ShaderDemo__MyComplexTouchEventListener__

#include "cocos2d.h"

USING_NS_CC;

#define MyComplexTouchEventListener_Angle_Noise 0.02
#define MyComplexTouchEventListener_Distance_Noise 4

class MyComplexTouchEventListener : public EventListenerTouchAllAtOnce{

public:
    enum EventState{
        EventState_None, EventState_Click, EventState_Move, EventState_Spin, EventState_Stretch
    };
    
public:
    static MyComplexTouchEventListener* createWithTarget(Node* node);
    
public:
    std::function<bool(const std::vector<Touch*>& touches, Event* event)> onTouchesBeganSuccess;
    std::function<void(Event* event, MyComplexTouchEventListener* listener)> onTouchesFinished;
    
public:
    std::function<void()> onClick;
    std::function<void(const Vec2& prePosition, const Vec2& curPosition)> onDrag;
    std::function<void(float angle)> onSpin;
    std::function<void(float preDistance, float curDistance)> onStretch;
    
public:
    void onTouchesBeganCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMovedCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEndedCallback(const std::vector<Touch*>& touches, Event* event);
    void onTouchesCancelledCallback(const std::vector<Touch*>& touches, Event* event);
    
public:
    MyComplexTouchEventListener::EventState getCurEventState(){ return m_eventState; }
    
protected:
    MyComplexTouchEventListener();
    virtual ~MyComplexTouchEventListener();
    
    bool initWithTarget(cocos2d::Node* node);
    
protected:
    void handleSingleTouchBegan(Touch* touch);
    void handleSingleTouchMoved(Touch* touch);
    void handleSingleTouchEnded(Touch* touch, cocos2d::Event* event);
    
    void handleDoubleTouchesBegan(const std::vector<Touch*>& touches);
    void handleDoubleTouchesMoved(const std::vector<Touch*>& touches);
    void handleDoubleTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event);
    
protected:
    bool isValidTouch(Touch* touch);
    
protected:
    Vec2 m_touchBeganPosition;
    Vec2 m_prePosition;
    
    Vec2 m_preDirection;
    float m_preDistance;
    
private:
    Node* m_target;
    bool m_isValid;
    EventState m_eventState;
    
private:
    int m_touchCount;
    bool m_isSingleTouchMode;
    bool m_isDoubleTouchMode;
};

#endif /* defined(__ShaderDemo__MyComplexTouchEventListener__) */
