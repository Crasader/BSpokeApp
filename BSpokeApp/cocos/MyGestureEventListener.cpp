//
//  MyGestureEventListener.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/1.
//
//

#include "MyGestureEventListener.h"

MyGestureEventListener::~MyGestureEventListener(){
//    CC_SAFE_RELEASE(m_touchEventListener);
}

MyGestureEventListener* MyGestureEventListener::createWithTarget(cocos2d::Node* node){
    MyGestureEventListener* ret = new MyGestureEventListener();
    if(ret && ret->initWithTarget(node)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return ret;
}

bool MyGestureEventListener::initWithTarget(cocos2d::Node* node){
    if(!EventListenerTouchAllAtOnce::init())
        return false;
    
    m_touchCount = 0;
    
    this->onTouchesBegan = CC_CALLBACK_2(MyGestureEventListener::onTouchesBeganCallback, this);
    this->onTouchesMoved = CC_CALLBACK_2(MyGestureEventListener::onTouchesMovedCallback, this);
    this->onTouchesEnded = CC_CALLBACK_2(MyGestureEventListener::onTouchesEndedCallback, this);
    this->onTouchesCancelled = CC_CALLBACK_2(MyGestureEventListener::onTouchesCancelledCallback, this);
    
    return true;
}

void MyGestureEventListener::reset(){
    m_preDirection = Vec2::ZERO;
    m_preDistance  = 0;
}

void MyGestureEventListener::handleSingleTouchBegan(Touch* touch, Event* event){
    CCLOG("SINGLE TOUCH");
    if(!this->isSingleTouches())
        return;
    
    m_preSingleTouchPosition = touch->getLocation();
}

void MyGestureEventListener::handleSingleTouchMoved(Touch* touch, Event* event){
    if(!this->isSingleTouches())
        return;
    
    Vec2 curSingleTouchPosition = touch->getLocation();
    
    if(onDragListener)
        onDragListener(m_preSingleTouchPosition, curSingleTouchPosition, MyGestureEvent_None);
    
    m_preSingleTouchPosition = curSingleTouchPosition;
}

void MyGestureEventListener::handleSingleTouchEnded(Touch* touch, Event* event){
}

void MyGestureEventListener::onTouchesBeganCallback(const std::vector<Touch*>& touches, Event* event){
    CCLOG("ABC");
    
    m_preTouchesPosition.clear();
    
    m_touchCount += touches.size();
    
    if(touches.size() < 2){
        this->handleSingleTouchBegan(touches[0], event);
        this->reset();
        return;
    }
    
    for(Touch* touch : touches){
        m_preTouchesPosition.push_back(touch->getLocation());
    }
    
    //只处理两点
    m_preDirection  = m_preTouchesPosition.at(1) - m_preTouchesPosition.at(0);
    m_preDistance   = m_preDirection.length();
}

void MyGestureEventListener::onTouchesMovedCallback(const std::vector<Touch*>& touches, Event* event){
    CCLOG("ABC1");
    if(touches.size() < 2){
        this->handleSingleTouchMoved(touches[0], event);
        this->reset();
        return;
    }
    
    std::vector<Vec2> curTouchesPosition;
    for(Touch* touch : touches){
        curTouchesPosition.push_back(touch->getLocation());
    }
    
    Vec2 curDirection = curTouchesPosition.at(1) - curTouchesPosition.at(0);
    float curDistance = curDirection.length();
    float angle = curDirection.getAngle(m_preDirection);
    
    if(onSpinListener && angle < 3.14)
        onSpinListener(angle, MyGestureEvent_Spin);
    
    
    if(onStretchListener && m_preDistance > 0.1 && curDistance > 0.1)
        onStretchListener(m_preDistance, curDistance, MyGestureEvent_Stretch);
    
    m_preDirection = curDirection;
    m_preDistance  = curDistance;
}

void MyGestureEventListener::onTouchesEndedCallback(const std::vector<Touch*>& touches, Event* event){
    CCLOG("ABC2");
    
    m_touchCount -= touches.size();
    
    if(touches.size() < 2){
        this->handleSingleTouchEnded(touches[0], event);
        this->reset();
        return;
    }
    
    std::vector<Vec2> curTouchesPosition;
    for(Touch* touch : touches){
        curTouchesPosition.push_back(touch->getLocation());
    }
    
    Vec2 curDirection = curTouchesPosition.at(1) - curTouchesPosition.at(0);
    float curDistance = curDirection.length();
    float angle = curDirection.getAngle(m_preDirection);
    
    if(onSpinListener && angle < 3.14)
        onSpinListener(angle, MyGestureEvent_Spin);
    
    if(onStretchListener && m_preDistance > 0.1 && curDistance > 0.1)
        onStretchListener(m_preDistance, curDistance, MyGestureEvent_Stretch);
    
    this->reset();
}

void MyGestureEventListener::onTouchesCancelledCallback(const std::vector<Touch*>& touches, Event* event){
    CCLOG("ABC3");
    this->reset();
}