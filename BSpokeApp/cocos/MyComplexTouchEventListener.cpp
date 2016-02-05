//
//  MyComplexTouchEventListener.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/1.
//
//

#include "MyComplexTouchEventListener.h"

#define MyComplexTouchEventListener_Move_Threshold 5
#define MyComplexTouchEventListener_Spin_Threshold 3.14
#define MyComplexTouchEventListener_Stretch_Threshold 0.1

static bool s_isCatchingTouch = false;

MyComplexTouchEventListener::MyComplexTouchEventListener():
onTouchesBeganSuccess(NULL),
onTouchesFinished(NULL),
onClick(NULL),
onDrag(NULL),
onSpin(NULL),
onStretch(NULL)
{

}

MyComplexTouchEventListener::~MyComplexTouchEventListener(){

}

MyComplexTouchEventListener* MyComplexTouchEventListener::createWithTarget(Node* node){
    MyComplexTouchEventListener* ret = new MyComplexTouchEventListener();
    if(ret && ret->initWithTarget(node)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool MyComplexTouchEventListener::initWithTarget(cocos2d::Node* node){
    if(!EventListenerTouchAllAtOnce::init())
        return false;
    
    m_isValid = false;
    m_touchCount = 0;
    m_isSingleTouchMode = false;
    m_isDoubleTouchMode = false;
    m_eventState = EventState_None;
    m_target = node;
    
    this->onTouchesBegan = CC_CALLBACK_2(MyComplexTouchEventListener::onTouchesBeganCallback, this);
    this->onTouchesMoved = CC_CALLBACK_2(MyComplexTouchEventListener::onTouchesMovedCallback, this);
    this->onTouchesEnded = CC_CALLBACK_2(MyComplexTouchEventListener::onTouchesEndedCallback, this);
    
    return true;
}

void MyComplexTouchEventListener::onTouchesBeganCallback(const std::vector<Touch*>& touches, Event* event){
    
    if(s_isCatchingTouch && !m_isValid)
        return;
    
    if(!m_target->isVisible())
        return;
    
    if(onTouchesBeganSuccess && !onTouchesBeganSuccess(touches, event))
        return;
    
    for(Touch* touch : touches){
        if(isValidTouch(touch)){
            m_isValid = true;
            s_isCatchingTouch = true;
            break;
        }
    }
    
    if(!m_isValid)
        return;
    
    m_touchCount += touches.size();
    
    if(m_touchCount == 1){
        m_isSingleTouchMode = true;
        m_isDoubleTouchMode = false;
    }else if(m_touchCount == 2){
        m_isSingleTouchMode = false;
        m_isDoubleTouchMode = true;
    }
    
    if(m_isSingleTouchMode)
        handleSingleTouchBegan(touches[0]);
    else if(m_isDoubleTouchMode)
        handleDoubleTouchesBegan(touches);

    event->stopPropagation();
}

void MyComplexTouchEventListener::onTouchesMovedCallback(const std::vector<Touch*>& touches, Event* event){
    if(!m_isValid)
        return;
    
    if(m_isSingleTouchMode)
        handleSingleTouchMoved(touches[0]);
    else if(m_isDoubleTouchMode)
        handleDoubleTouchesMoved(touches);
    
    event->stopPropagation();
}

void MyComplexTouchEventListener::onTouchesEndedCallback(const std::vector<Touch*>& touches, Event* event){
    
    if(!m_isValid)
        return;
    
    if(m_isSingleTouchMode){
        handleSingleTouchEnded(touches[0], event);
    }else if(m_isDoubleTouchMode){
        handleDoubleTouchesEnded(touches, event);
    }
    
    event->stopPropagation();
    
    m_touchCount -= touches.size();
    
    if(m_touchCount <= 0){
        s_isCatchingTouch = false;
        m_isValid = false;
        m_isSingleTouchMode = false;
        m_isDoubleTouchMode = false;
        m_eventState = EventState_None;
    }
}

void MyComplexTouchEventListener::onTouchesCancelledCallback(const std::vector<Touch*>& touches, Event* event){

}

void MyComplexTouchEventListener::handleSingleTouchBegan(Touch* touch){
    m_touchBeganPosition = touch->getLocation();
    m_prePosition = m_touchBeganPosition;
    m_eventState  = EventState_Click;
}

void MyComplexTouchEventListener::handleSingleTouchMoved(Touch* touch){
    Vec2 curTouchPosition = touch->getLocation();
    if(curTouchPosition.distance(m_touchBeganPosition) > MyComplexTouchEventListener_Move_Threshold){
        //move
        m_eventState = EventState_Move;
    }
    
    if(m_eventState == EventState_Move){
        Vec2 curPosition = touch->getLocation();
        
        if(onDrag)
            onDrag(m_prePosition, curPosition);
        
        m_prePosition = curPosition;
    }
}

void MyComplexTouchEventListener::handleSingleTouchEnded(Touch* touch, cocos2d::Event* event){
    if(isValidTouch(touch) && m_eventState == EventState_Click){
        if(onClick)
            onClick();
    }
    
    if(onTouchesFinished)
        onTouchesFinished(event, this);
}

void MyComplexTouchEventListener::handleDoubleTouchesBegan(const std::vector<Touch*>& touches){
    if(touches.size() != 2)
        return;
    
    CCLOG("Double Begin");
    
    m_eventState = EventState_None;
    
    m_preDirection  = touches[1]->getLocation() - touches[0]->getLocation();
    m_preDistance   = m_preDirection.length();
}

void MyComplexTouchEventListener::handleDoubleTouchesMoved(const std::vector<Touch*>& touches){
    if(touches.size() != 2)
        return;
    
    CCLOG("Double moved");
    
    Vec2 curDirection = touches[1]->getLocation() - touches[0]->getLocation();
    float curDistance = curDirection.length();
    float angle = curDirection.getAngle(m_preDirection);
    
    if(angle < MyComplexTouchEventListener_Spin_Threshold){
        m_eventState = EventState_Spin;
        if(onSpin)
            onSpin(angle);
    }
    
    if(m_preDistance > MyComplexTouchEventListener_Stretch_Threshold && curDistance > MyComplexTouchEventListener_Stretch_Threshold){
        m_eventState = EventState_Stretch;
        if(onStretch)
            onStretch(m_preDistance, curDistance);
    }
    
    m_preDirection = curDirection;
    m_preDistance  = curDistance;

}

void MyComplexTouchEventListener::handleDoubleTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event){
    CCLOG("Double end");
    
    if(onTouchesFinished)
        onTouchesFinished(event, this);
    
    m_preDirection = Vec2::ZERO;
    m_preDistance  = 0;
}

bool MyComplexTouchEventListener::isValidTouch(Touch* touch){
    Vec2 posInNode = m_target->convertTouchToNodeSpace(touch);
    Rect bound = Rect::ZERO;
    
    bound.size = m_target->getContentSize();
    
    return bound.containsPoint(posInNode);
}