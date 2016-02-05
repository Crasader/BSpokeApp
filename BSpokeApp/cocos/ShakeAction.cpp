//
//  ShakeAction.cpp
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/10.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#include "ShakeAction.h"

using namespace pp;
USING_NS_CC;

ShakeAction* ShakeAction::create(float strength, float during){
    ShakeAction* ret = new ShakeAction();
    if(ret && ret->init(strength, during)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool ShakeAction::init(float strength, float during){
    if(!ActionInterval::initWithDuration(during))
        return false;
    
    m_strength = strength;
    
    return true;
}

void ShakeAction::update(float time){
    float randomX = random<float>(-m_strength, m_strength);
    float randomY = random<float>(-m_strength, m_strength);
    
    _target->setPosition(randomX, randomY);
}

void ShakeAction::startWithTarget(cocos2d::Node *target){
    ActionInterval::startWithTarget(target);
    
    m_originPosition = target->getPosition();
}

void ShakeAction::stop(){
    _target->setPosition(m_originPosition);
    
    ActionInterval::stop();
}