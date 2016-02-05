//
//  MyTouchEventListener.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/26.
//
//  单击 双击 拖动的点击事件监听

#ifndef __ShaderDemo__MyTouchEventListener__
#define __ShaderDemo__MyTouchEventListener__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

enum MyTouchEvent{
    MyTouchEvent_Click, MyTouchEvent_DoubleClick, MyTouchEvent_Drag, MyTouchEvent_TouchBegan, MyTouchEvent_TouchEnded, MyTouchEvent_TouchCancelled, MyTouchEvent_None
};

static float MyTouchEventDoubleClickInterval = 0.2f;
static float MyTouchEventMoveInterval = 0.1f;

class MyTouchEventListener : public cocos2d::EventListenerTouchOneByOne{

public:
    static MyTouchEventListener* createWithTarget(cocos2d::Node* node);
    
public:
    bool handlerTouchBegan(cocos2d::Touch* touch);
    
public:
    typedef std::function<bool(cocos2d::Touch*, MyTouchEvent)> myTouchEventCallbackWithResult;
    typedef std::function<void(cocos2d::Touch*, MyTouchEvent)> myTouchEventCallback;
    myTouchEventCallback onClickListener;
    myTouchEventCallback onDoubleClickListener;
    myTouchEventCallback onDragListener;
    myTouchEventCallbackWithResult onTouchBeganListener;
    myTouchEventCallback onTouchEndedListener;
    
    std::function<void(const Vec2& preTouchPosition, const Vec2& curTouchPosition, MyTouchEvent)> onSimpleDragListener;
    
public:
    bool onTouchBeganCallback(Touch* touch, Event* event);
    void onTouchMovedCallback(Touch* touch, Event* event);
    void onTouchEndedCallback(Touch* touch, Event* event);
    void onTouchCancelledCallback(Touch* touch, Event* event);
    
public:
    void update(float dt);
    MyTouchEventListener();
    virtual ~MyTouchEventListener();
    
protected:
    bool initWithTarget(cocos2d::Node* node);
    void resetState();
    
private:
    cocos2d::Node* m_target;
    
private:
    //是否需要计时
    bool m_isNeedTiming;
    float m_timing;
    
    int m_eventState;
    cocos2d::Touch* m_finalTouch;
    cocos2d::Vec2 m_preTouchPosition;
};

#endif /* defined(__ShaderDemo__MyTouchEventListener__) */
