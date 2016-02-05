//
//  ArrangeBtn.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/29.
//
//

#include "ArrangeBtn.h"
#include "MyComplexTouchEventListener.h"
#include "CocosUtil.h"

USING_NS_CC;
using namespace pp;

#define ARRANGE_BUTTON_FILE "btn_add_pattern.png"

ArrangeBtn::ArrangeBtn():
m_clickedCallback(NULL),
m_spinCallback(NULL),
m_stretchCallback(NULL)
{
    
}

ArrangeBtn* ArrangeBtn::createWithData(float ratio, cocos2d::Vec2& origin){
    ArrangeBtn* btn = new ArrangeBtn();
    if(btn && btn->initWithData(ratio, origin)){
        btn->autorelease();
        return btn;
    }
    
    CC_SAFE_DELETE(btn);
    return NULL;
}

bool ArrangeBtn::initWithData(float ratio, cocos2d::Vec2& origin){
    if(!Sprite::initWithFile(ARRANGE_BUTTON_FILE))
        return false;
    
    m_clickedCallback = NULL;
    m_ratio = ratio;
    m_originVec = origin;
    m_validOrientation = "0";
    m_isValid = false;
    m_isLeft = true;
    
    MyComplexTouchEventListener* listener = MyComplexTouchEventListener::createWithTarget(this);
    listener->onTouchesBeganSuccess = [=](const std::vector<Touch*>& touches, Event* event)->bool{
        return m_isValid;
    };
    listener->onClick = [=](){
        if(m_isValid && m_clickedCallback)
            m_clickedCallback(this);
    };
    listener->onSpin = [=](float angle){
        if(m_spinCallback)
            m_spinCallback(angle);
    };
    listener->onStretch = [=](float preDistance, float curDistance){
        if(m_stretchCallback)
            m_stretchCallback(preDistance, curDistance);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    FadeTo* fadeOut = FadeOut::create(1.0);
    FadeTo* fadeIn  = fadeOut->reverse();
    RepeatForever* repeat = RepeatForever::create(Sequence::create(fadeOut, fadeIn, NULL));
    this->runAction(repeat);
    
    return true;
}

void ArrangeBtn::setInfoFromArrangeData(ArrangeData* data){
    m_data = data;
    m_validOrientation = data->validOrientation;
    m_isLeft = data->isLeft;
    
    this->setScale(2.f, 2.f);
    this->setPosition(m_originVec + data->position * m_ratio);
}

void ArrangeBtn::setIsValid(int orientation){
    std::vector<int> orientations;
    CocosUtil::stringToIntArray(m_validOrientation, orientations);
    
    m_isValid = false;
    
    for(int& temp : orientations){
        if(temp == orientation){
            m_isValid = true;
        }
    };
}