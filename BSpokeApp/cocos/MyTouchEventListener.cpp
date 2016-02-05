//
//  MyTouchEventListener.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/26.
//
//

#include "MyTouchEventListener.h"

enum MyTouchEventState{
    MyTouchEventState_Silence, MyTouchEventState_Begin, MyTouchEventState_Drag, MyTouchEventState_Click, MyTouchEventState_DoubleClick
};

MyTouchEventListener::MyTouchEventListener():
onClickListener(NULL),
onDoubleClickListener(NULL),
onDragListener(NULL),
onTouchBeganListener(NULL),
onTouchEndedListener(NULL),
onSimpleDragListener(NULL),
m_finalTouch(NULL)
{

}

MyTouchEventListener::~MyTouchEventListener(){
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    
    CC_SAFE_RELEASE(m_finalTouch);
    m_finalTouch = NULL;
}

MyTouchEventListener* MyTouchEventListener::createWithTarget(cocos2d::Node* node){
    MyTouchEventListener* ret = new MyTouchEventListener();
    if(ret && ret->initWithTarget(node)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool MyTouchEventListener::initWithTarget(cocos2d::Node *node){
    if(!EventListenerTouchOneByOne::init())
        return false;
    
    m_target = node;
    
    this->onTouchBegan = CC_CALLBACK_2(MyTouchEventListener::onTouchBeganCallback, this);
    this->onTouchMoved = CC_CALLBACK_2(MyTouchEventListener::onTouchMovedCallback, this);
    this->onTouchEnded = CC_CALLBACK_2(MyTouchEventListener::onTouchEndedCallback, this);
    this->onTouchCancelled = CC_CALLBACK_2(MyTouchEventListener::onTouchCancelledCallback, this);
    
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    
    this->resetState();
    
    return true;
}

void MyTouchEventListener::resetState(){
    m_eventState = MyTouchEventState_Silence;
    m_isNeedTiming = false;
    m_timing = 0;
    CC_SAFE_RELEASE(m_finalTouch);
    m_finalTouch = NULL;
}

bool MyTouchEventListener::handlerTouchBegan(cocos2d::Touch* touch){
    Vec2 touchLocation = touch->getLocation();
    touchLocation = m_target->convertToNodeSpace(touchLocation);
    
    Rect rect;
    rect.size = m_target->getContentSize();
    
    return rect.containsPoint(touchLocation);
}

bool MyTouchEventListener::onTouchBeganCallback(Touch* touch, Event* event){
    bool result = false;
    
    if(event->getCurrentTarget() == m_target){
        
        if(this->onTouchBeganListener){
            result = this->onTouchBeganListener(touch, MyTouchEvent_TouchBegan);
        }else{
            result = this->handlerTouchBegan(touch);
        }
        
        if(result){
            if(m_eventState == MyTouchEventState_Silence)
                m_eventState = MyTouchEventState_Begin;
            
            m_isNeedTiming = true;
            m_timing = 0;
            
            m_preTouchPosition = touch->getLocation();
        }
    }
    
    return result;
}

void MyTouchEventListener::onTouchMovedCallback(Touch* touch, Event* event){
    if(m_eventState == MyTouchEventState_Drag){
        Vec2 curTouchPosition = touch->getLocation();
        
        if(this->onSimpleDragListener)
            this->onSimpleDragListener(m_preTouchPosition, curTouchPosition, MyTouchEvent_Drag);
        if(this->onDragListener)
            this->onDragListener(touch, MyTouchEvent_Drag);
        
        m_preTouchPosition = curTouchPosition;
        return;
    }
    if(m_isNeedTiming){
        if(m_timing > MyTouchEventMoveInterval){
            m_eventState = MyTouchEventState_Drag;
            m_isNeedTiming = false;
            m_timing = 0;
        }
    }
}

void MyTouchEventListener::onTouchEndedCallback(Touch* touch, Event* event){
    if(m_eventState == MyTouchEventState_Begin){
        m_isNeedTiming = true;
        m_timing = 0;
        m_eventState = MyTouchEventState_Click;
        
        CC_SAFE_RELEASE(m_finalTouch);
        m_finalTouch = touch;
        m_finalTouch->retain();
    }else{
        if(m_eventState == MyTouchEventState_Click){
            if(this->onDoubleClickListener){
                this->onDoubleClickListener(touch, MyTouchEvent_DoubleClick);
            }
        }
        this->resetState();
    }
    
    if(this->onTouchEndedListener)
        this->onTouchEndedListener(touch, MyTouchEvent_TouchEnded);
}

void MyTouchEventListener::onTouchCancelledCallback(Touch* touch, Event* event){
    this->resetState();
    
    if(this->onTouchEndedListener)
        this->onTouchEndedListener(touch, MyTouchEvent_TouchCancelled);
}

void MyTouchEventListener::update(float dt){
    
    if(!m_isNeedTiming)
        return;
    
    this->retain();
    m_timing += dt;
    
    if(m_eventState == MyTouchEventState_Click && m_timing > MyTouchEventDoubleClickInterval){
        if(this->onClickListener){
            this->onClickListener(m_finalTouch, MyTouchEvent_Click);
        }
        this->resetState();
    }
    this->release();
}