//
//  ClipEffect.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/8.
//
//

#ifndef __ShaderDemo__ClipEffect__
#define __ShaderDemo__ClipEffect__

#include "IEffect.h"
#define CLIPEFFECT_LINE_COUNT_MAX 12

class ClipEffect : public IEffect{
    
public:
    static ClipEffect* create();
    static ClipEffect* createWithCompletedCallback(const std::function<void(IEffect*)>& callback);
    bool initWithCompletedCallback(const std::function<void(IEffect*)>& callback);
    
public:
    virtual void applyToSprite(Sprite* sprite);
    virtual void cancelToSprite(Sprite* sprite){};
    virtual void runAction(const Vec2& delta);
    virtual void updateColor(const cocos2d::Color3B &color);
    virtual void reset();
    
public:
    virtual void convertToData(BaseEffectData* to){};
    
public:
    void onUniformBasePointXCallback(GLProgram* program, Uniform* uniform);
    void onUniformOffsetCallback(GLProgram* program, Uniform* uniform);
    virtual void update(float dt);
    
protected:
    ClipEffect();
    virtual ~ClipEffect();
    
    void resetArgs();
    
protected:
    //shader uniform
    int m_lineCount;
    float m_angle;
    int m_isXSpread;
    
protected:
    float m_offsetRange[CLIPEFFECT_LINE_COUNT_MAX];
    float m_totalTime;
};

#endif /* defined(__ShaderDemo__ClipEffect__) */
