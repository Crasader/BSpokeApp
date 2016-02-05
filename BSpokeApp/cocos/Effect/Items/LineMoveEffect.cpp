//
//  LineMoveEffect.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/10.
//
//

#include "LineMoveEffect.h"

LineMoveEffect* LineMoveEffect::create(){
    LineMoveEffect* ret = new LineMoveEffect();
    if(ret && ret->initWithCompletedCallback(NULL)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

LineMoveEffect* LineMoveEffect::createWithCompletedCallback(const std::function<void(IEffect*)>& callback){
    LineMoveEffect* ret = new LineMoveEffect();
    if(ret && ret->initWithCompletedCallback(callback)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}


bool LineMoveEffect::initWithCompletedCallback(const std::function<void(IEffect*)>& callback){
    m_completedCallback = callback;
    m_shaderName = "LineMoveShader";
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    if(!program){
        GLProgram* program = GLProgram::createWithFilenames("shader/base_pos_tex.vert", "shader/lineMove.frag");
        GLProgramCache::getInstance()->addGLProgram(program, m_shaderName);
    }
    
    //dot Texuture, set to repeat
    m_texture = Director::getInstance()->getTextureCache()->addImage("effect/pattern1.png");
    Texture2D::TexParams params;
    params.minFilter = GL_LINEAR;
    params.magFilter = GL_LINEAR;
    params.wrapS = GL_REPEAT;
    params.wrapT = GL_REPEAT;
    
    m_texture->setTexParameters(params);
    
    return true;
}

void LineMoveEffect::applyToSprite(Sprite* sprite){
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    sprite->setGLProgram(program);
    
    const Size& markSize    = sprite->getContentSize();
    const Size& textureSize = m_texture->getContentSize();
    Vec2 ratio = Vec2(markSize.width / textureSize.width, markSize.height / textureSize.height);
    
    Vec2 baseOffset;
    baseOffset.x = random<float>(0.25f, 0.25f);
    baseOffset.y = random<float>(0.0f, 0.0f);
    
    m_angle  = 0.f;
    m_offset = baseOffset.y;
	
    m_glProgrameState = sprite->getGLProgramState();
    m_glProgrameState->setUniformTexture("u_contentTexture", m_texture);
    m_glProgrameState->setUniformVec2("u_baseOffset", baseOffset);
    m_glProgrameState->setUniformFloat("u_contentTextureOffset", m_offset);
    m_glProgrameState->setUniformVec2("u_ratio", ratio);
    m_glProgrameState->setUniformFloat("u_angle", m_angle);
    
    Color3B color = sprite->getColor();
    this->updateColor(color);
}

void LineMoveEffect::runAction(const Vec2& delta){
    Vec2 axleY(0.0f, 1.0f);
    m_angle = axleY.getAngle(delta);
    m_glProgrameState->setUniformFloat("u_angle", m_angle);
    
    m_step = delta.length();
    
    printf("angle : %f  step :%f \n", m_angle, m_step);
}

void LineMoveEffect::updateColor(const cocos2d::Color3B &color){
    m_color = Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f);
    m_glProgrameState->setUniformVec4("u_contentTextureColor", m_color);
}

void LineMoveEffect::update(float dt){
    
    if(m_step <= 0.0) return;
    
    m_offset += m_step * 0.01;
    m_step -= dt * 0.3;
    
    m_offset = fixOffsetCoord(m_offset);
    m_glProgrameState->setUniformFloat("u_contentTextureOffset", m_offset);
    
    if(m_step <= 0.0 && m_completedCallback)
        m_completedCallback(this);
    
//    m_angle += dt * 0.1;
//    m_glProgrameState->setUniformFloat("u_angle", m_angle);
}

float LineMoveEffect::fixOffsetCoord(const float& offsetCoord){
    float ret = offsetCoord;
    if(ret < 0.0f){
        ret += 1.0f;
    }else if(ret > 1.0f){
        ret -= 1.0f;
    }
    
    return ret;
}