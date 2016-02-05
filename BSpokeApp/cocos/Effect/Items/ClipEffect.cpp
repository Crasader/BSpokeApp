//
//  ClipEffect.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/8.
//
//

#include "ClipEffect.h"
#include <vector>

static std::vector<float> s_basePointPos;
static std::vector<float> s_offset;

ClipEffect::ClipEffect()
{
    m_effectType = EffectType_ClipEffect;
}

ClipEffect::~ClipEffect(){
    CCLOG("destory ClipEffect");
    if(m_glProgrameState){
        m_glProgrameState->setUniformCallback("u_basePointPos", NULL);
        m_glProgrameState->setUniformCallback("u_offset", NULL);
    }
}

ClipEffect* ClipEffect::create(){
    ClipEffect* ret = new ClipEffect();
    if(ret && ret->initWithCompletedCallback(NULL)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

ClipEffect* ClipEffect::createWithCompletedCallback(const std::function<void (IEffect *)> &callback){
    ClipEffect* ret = new ClipEffect();
    if(ret && ret->initWithCompletedCallback(callback)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool ClipEffect::initWithCompletedCallback(const std::function<void (IEffect *)> &callback){
    m_completedCallback = callback;
    m_shaderName  = "ClipEffectShader";
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(m_shaderName);
    if(!program){
        GLProgram* program = GLProgram::createWithFilenames("shader/clip.vert", "shader/clip.frag");
        GLProgramCache::getInstance()->addGLProgram(program, m_shaderName);
    }
    
    this->resetArgs();
    
    return true;
}

void ClipEffect::applyToSprite(Sprite* sprite){
    m_glProgrameState = GLProgramState::getOrCreateWithGLProgramName(m_shaderName);
    
    sprite->setGLProgramState(m_glProgrameState);

    this->resetArgs();
    
    m_glProgrameState->setUniformInt("u_lineCount", m_lineCount);
    m_glProgrameState->setUniformFloat("u_angle", m_angle);
    m_glProgrameState->setUniformInt("u_isXSpread", m_isXSpread);
    m_glProgrameState->setUniformCallback("u_basePointPos", CC_CALLBACK_2(ClipEffect::onUniformBasePointXCallback, this));
    m_glProgrameState->setUniformCallback("u_offset", CC_CALLBACK_2(ClipEffect::onUniformOffsetCallback, this));
    
    Color3B color = sprite->getColor();
    this->updateColor(color);
}

bool decreaseComp(const float& a, const float& b){
    return a > b;
}

void sortArray(std::vector<float>& array, const int& length, bool increase){
    std::vector<float>::iterator endIterator = array.end();
    int index = 0;
    
    for(std::vector<float>::iterator it = array.begin(); it != array.end(); it++){
        if(index == length){
            endIterator = it;
            break;
        }
        index++;
    }
    
    if(increase)
        sort(array.begin(), endIterator);
    else
        sort(array.begin(), endIterator, &decreaseComp);
}

void ClipEffect::runAction(const Vec2& delta){
    float strength = delta.length();
    float angle    = delta.getAngle();
    
    this->resetArgs();
    
    m_lineCount = ceil(strength) * 3;
    m_lineCount = MIN(m_lineCount, CLIPEFFECT_LINE_COUNT_MAX);
    m_angle = angle;
    
//    m_lineCount = 3;
//    m_angle = 2.0f;
    
    for(int i = 0; i < m_lineCount; i++){
        m_offsetRange[i] = random<float>(-0.05, 0.05);
        s_basePointPos[i]  = random<float>(0.05, 0.95);
        s_offset[i] = 0.f;
    };
    
    float a = tan(m_angle);
    //是否以x轴分布
    if(-1 < a && a < 1){
        m_isXSpread = 0;
    }else{
        m_isXSpread = 1;
    }
    
    if(m_isXSpread && a > 0)
        //升序
        sortArray(s_basePointPos, m_lineCount, true);
    else
        sortArray(s_basePointPos, m_lineCount, false);
        
    m_glProgrameState->setUniformInt("u_lineCount", m_lineCount);
    m_glProgrameState->setUniformFloat("u_angle", m_angle);
    m_glProgrameState->setUniformInt("u_isXSpread", m_isXSpread);
    
    m_totalTime = 0.0f;
}

void ClipEffect::updateColor(const cocos2d::Color3B &color){
    m_color = Vec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, 1.0f);
}

void ClipEffect::reset(){
    this->resetArgs();
    
    m_glProgrameState->setUniformInt("u_lineCount", m_lineCount);
    m_glProgrameState->setUniformFloat("u_angle", m_angle);
    m_glProgrameState->setUniformInt("u_isXSpread", m_isXSpread);
}

void ClipEffect::onUniformBasePointXCallback(GLProgram* program, Uniform* uniform){
    program->setUniformLocationWith1fv(uniform->location, (GLfloat*)(&s_basePointPos[0]), (unsigned int)s_basePointPos.size());
//    program->setUniformLocationWith1f(uniform->location, m_basePointX[0]);
}

void ClipEffect::onUniformOffsetCallback(GLProgram* program, Uniform* uniform){
    program->setUniformLocationWith1fv(uniform->location, (GLfloat*)(&s_offset[0]), (unsigned int)s_offset.size());
//    program->setUniformLocationWith1f(uniform->location, m_offset[0]);
}

void ClipEffect::update(float dt){
    if(m_totalTime > 6.0f){
        if(m_completedCallback)
            m_completedCallback(this);
        return;
    }
    
    m_totalTime += dt;
    for(int i = 0; i < m_lineCount; i++){
        s_offset[i] = sin(m_totalTime * 40) * m_offsetRange[i];
    }
}

void ClipEffect::resetArgs(){
    s_basePointPos.assign(CLIPEFFECT_LINE_COUNT_MAX, 0.f);
    s_offset.assign(CLIPEFFECT_LINE_COUNT_MAX, 0.f);
    m_lineCount = 0;
    m_angle = 0.f;
    m_isXSpread = 1;
}