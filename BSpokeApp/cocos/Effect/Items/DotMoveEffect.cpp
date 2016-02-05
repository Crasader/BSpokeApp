//
//  DotMoveEffect.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/8.
//
//

#include "DotMoveEffect.h"

DotMoveEffect* DotMoveEffect::create(){
    DotMoveEffect* ret = new DotMoveEffect();
    if(ret && ret->init()){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

DotMoveEffect* DotMoveEffect::createFromData(DotMoveEffectData* from){
    DotMoveEffect* ret = new DotMoveEffect();
    if(ret && ret->initWithData(from)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool DotMoveEffect::init(){
    //shader
    m_shaderName  = "DotMoveShader";
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    if(!program){
        GLProgram* program = GLProgram::createWithFilenames("shader/base_pos_tex.vert", "shader/dotMove.frag");
        GLProgramCache::getInstance()->addGLProgram(program, m_shaderName);
    }
    
    //dot Texuture
    m_texture = Director::getInstance()->getTextureCache()->addImage("effect/pattern4.png");
    
    m_originCoreCoord = Vec2(0.5f, 0.5f);
    m_coreCoord = Vec2(0.5f, 0.5f);
    
    return true;
}

bool DotMoveEffect::initWithData(DotMoveEffectData* from){
    if(!this->init())
        return false;
    
    m_coreCoord = from->coreCoord;
    m_color = from->color;
    
    return true;
}

void DotMoveEffect::applyToSprite(Sprite* sprite){
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    sprite->setGLProgram(program);
    
    Size maskSize    = sprite->getContentSize();
    Size textureSize = m_texture->getContentSize();
    m_maskContentSizeRatio = Vec2(maskSize.width / textureSize.width, maskSize.height / textureSize.height);
    m_limitRect      = Rect((1.f - m_maskContentSizeRatio.x) / 2.0f,  (1.f - m_maskContentSizeRatio.y) / 2.0f, m_maskContentSizeRatio.x, m_maskContentSizeRatio.y);
    
    m_glProgrameState = sprite->getGLProgramState();
    m_glProgrameState->setUniformVec2("u_texCoreCoord", Vec2(0.5f, 0.5f));
    m_glProgrameState->setUniformVec2("u_coreCoord", m_coreCoord);
    m_glProgrameState->setUniformTexture("u_contentTexture", m_texture);
    m_glProgrameState->setUniformVec2("u_ratio", m_maskContentSizeRatio);    
    
    Color3B color = sprite->getColor();
    this->updateColor(color);
}

void DotMoveEffect::runAction(const Vec2& delta){
    //最大力度一般低于3
    float strengh = delta.length();
    
    m_direction = delta;
    m_direction.normalize();
    
    m_range = strengh / 4;
    m_range = MAX(m_range, 0.05);
    
    m_totalMoveTime = 0.2 / strengh;
    m_moveStep = m_range / m_totalMoveTime;
    
    m_totalTweenTime = 0;
    m_tweenStep = m_range / 100;
}

void DotMoveEffect::convertToData(BaseEffectData* to){
    CCASSERT(to->type == EffectDataType_DotMove, "wrong effect data type to convert");
    
    DotMoveEffectData* data = (DotMoveEffectData*)to;
    data->coreCoord = m_coreCoord;
    data->color = m_color;
}

void DotMoveEffect::updateColor(const cocos2d::Color3B &color){
    m_color = Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f);
    m_glProgrameState->setUniformVec4("u_contentTextureColor", m_color);
}

void DotMoveEffect::update(float dt){
    if(m_totalMoveTime <= 0 && m_range == 0)
        return;
    
    if(m_totalMoveTime > 0){
        m_coreCoord       = m_coreCoord + m_direction * m_moveStep * dt;
        m_originCoreCoord = m_coreCoord;
        m_totalMoveTime -= dt;
    }else{
        m_range = MAX(m_range - m_tweenStep, 0);
        m_coreCoord = m_originCoreCoord + cosf(m_totalTweenTime) * m_range * m_direction;
        m_totalTweenTime += dt * 10.f;
    }
    
    m_coreCoord = fixCoreCoord(m_coreCoord);
    m_glProgrameState->setUniformVec2("u_coreCoord", m_coreCoord);
}

Vec2 DotMoveEffect::fixCoreCoord(const Vec2& coreCoord){
    float x = MIN(MAX(coreCoord.x, m_limitRect.getMinX()), m_limitRect.getMaxX());
    float y = MIN(MAX(coreCoord.y, m_limitRect.getMinY()), m_limitRect.getMaxY());

    return Vec2(x, y);
}