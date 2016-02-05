//
//  EffectManager.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/8.
//
//

#include "EffectManager.h"
#include "DotMoveEffect.h"
#include "LineMoveEffect.h"
#include "PaintEffect.h"
#include "LinePaintEffect.h"
#include "ClipEffect.h"

static EffectManager* s_instance = NULL;
static Node* s_shareEffectParent = NULL;

EffectManager* EffectManager::getInstance(){
    if(!s_instance){
        s_instance = new EffectManager();
        s_instance->init();
    }
    
    return s_instance;
}

bool EffectManager::init(){
    
    m_delegate = NULL;
    
    return true;
}

void EffectManager::setShareEffectParent(Node* node){
    s_shareEffectParent = node;
}

void EffectManager::renderSpriteEffect(Sprite* sprite){
    IEffect* effect = this->getSpriteEffect(sprite);
    
    if(effect){
        effect->render();
    }
}

void EffectManager::updateEffectColorToSprite(Sprite* sprite){
    IEffect* effect = this->getSpriteEffect(sprite);
    
    if(effect){
        effect->updateColor(sprite->getColor());
    }
}

void EffectManager::applyEffectToSprite(Sprite* sprite, EffectType effectType){
    if(sprite == NULL) return;
    
    if(!checkSpriteIsAvailable(sprite)) return;
    
    if(checkEffectIsApplied(sprite, effectType)) return;
    
    cancelEffectFromSprite(sprite);
    
    IEffect* effect = NULL;
    
    
    switch (effectType) {
        case EffectType_DotMoveEffect:
        {
            effect = DotMoveEffect::create();
            effect->applyToSprite(sprite);
        }
            break;
            
        case EffectType_LineMoveEffect:
        {
            effect = LineMoveEffect::createWithCompletedCallback(std::bind(&EffectManager::stopEffectAction, this, std::placeholders::_1));
            effect->applyToSprite(sprite);
        }
            break;
            
        case EffectType_PaintEffect:
        {
            effect = PaintEffect::create();
            effect->applyToSpriteWithParent(sprite, s_shareEffectParent);
        }
            break;
            
        case EffectType_LinePaintEffect:
        {
            effect = LinePaintEffect::create();
            effect->applyToSpriteWithParent(sprite, s_shareEffectParent);
        }
            break;
            
        case EffectType_ClipEffect:
        {
            effect = ClipEffect::createWithCompletedCallback(std::bind(&EffectManager::stopEffectAction, this, std::placeholders::_1));
            effect->applyToSprite(sprite);
        }
            break;
            
        default:
            break;
    }
    
    CCASSERT(effect, "effect is NULL");
    
    m_effectVector.pushBack(effect);
    m_spriteEffectMapping[sprite] = effect;
}

void EffectManager::cancelEffectFromSprite(Sprite* sprite){
    if(sprite == NULL) return;
    
    //stop sprite effect action
    this->stopEffectActionForSprite(sprite);
    
    //remove sprite effect
    std::map<Sprite*, IEffect*>::iterator it = m_spriteEffectMapping.find(sprite);
    if(it != m_spriteEffectMapping.end()){
        //remove effect and set default
        IEffect* effect = (*it).second;
        
        effect->cancelToSprite(sprite);
//        sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName("ignoreSelfColor"));
        sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
        
        m_effectVector.eraseObject(effect);
        m_spriteEffectMapping.erase(it);
    }
}

void EffectManager::runEffectActionForSprite(Sprite* sprite, const Vec2& delta){
    IEffect* effect = this->getSpriteEffect(sprite);
    
    if(effect){
        effect->runAction(delta);
        if(!m_runningActionEffectVector.contains(effect)){
            m_runningActionEffectVector.pushBack(effect);
            this->startAndStopSchedule();
        }
    }
}

void EffectManager::resetEffectToSprite(Sprite* sprite){
    this->stopEffectActionForSprite(sprite);
    
    IEffect* effect = this->getSpriteEffect(sprite);
    if(effect)
        effect->reset();
}

void EffectManager::stopEffectActionForSprite(Sprite* sprite){
    IEffect* effect = this->getSpriteEffect(sprite);
    stopEffectAction(effect);
}

void EffectManager::stopEffectAction(IEffect* effect){
    if(effect){
        m_runningActionEffectVector.eraseObject(effect);
        this->startAndStopSchedule();
    }
}

void EffectManager::pauseEffect(){
    m_isPause = true;
}

void EffectManager::resumeEffect(){
    m_isPause = false;
}

void EffectManager::update(float dt){
    if(m_isPause)
        return;
    
    for(IEffect* effect : m_runningActionEffectVector){
        effect->update(dt);
    }
}

bool EffectManager::checkSpriteIsAvailable(Sprite* sprite){
    Size textureSize = sprite->getTextureRect().size;
    return textureSize.width > 0 && textureSize.height > 0;
}

bool EffectManager::checkEffectIsApplied(Sprite* sprite, EffectType effectType){
    IEffect* effect = this->getSpriteEffect(sprite);
    
    if(effect && effect->getType() == effectType)
        return true;
    else
        return false;
}

void EffectManager::startAndStopSchedule(){
    m_isPause = false;
    
    if(m_runningActionEffectVector.empty()){
        Director::getInstance()->getScheduler()->unscheduleUpdate(this);
        if(m_delegate)
            m_delegate->onEffectsEnded();
    }else{
        Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
        if(m_delegate)
            m_delegate->onEffectsBegan();
    }
}

IEffect* EffectManager::getSpriteEffect(Sprite* sprite){
    std::map<Sprite*, IEffect*>::iterator it = m_spriteEffectMapping.find(sprite);
    if(it != m_spriteEffectMapping.end()){
        IEffect* effect = (*it).second;
        return effect;
    }
    
    return NULL;
}
