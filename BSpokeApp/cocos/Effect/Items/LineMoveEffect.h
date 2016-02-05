//
//  LineMoveEffect.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/10.
//
//

#ifndef __ShaderDemo__LineMoveEffect__
#define __ShaderDemo__LineMoveEffect__

#include "IEffect.h"

class LineMoveEffect : public IEffect{
    
public:
    static LineMoveEffect* create();
    static LineMoveEffect* createWithCompletedCallback(const std::function<void(IEffect*)>& callback);
    bool initWithCompletedCallback(const std::function<void(IEffect*)>& callback);
    
public:
    virtual void applyToSprite(Sprite* sprite);
    virtual void cancelToSprite(Sprite* sprite){};
    virtual void runAction(const Vec2& delta);
    virtual void updateColor(const cocos2d::Color3B &color);
public:
    virtual void convertToData(BaseEffectData* to){};
    
public:
    virtual void update(float dt);
    
protected:
    LineMoveEffect(){
        m_effectType = EffectType_LineMoveEffect;
    }
    
protected:
    float fixOffsetCoord(const float& offset);
    
protected:
    Texture2D* m_texture;
    
protected:
    //texture rotate angle
    float m_angle;
    
    //texture offset
    float m_offset;
    
protected:
    float m_step;
    
protected:
    std::function<void(IEffect*)> m_completedCallback;
};

#endif /* defined(__ShaderDemo__LineMoveEffect__) */
