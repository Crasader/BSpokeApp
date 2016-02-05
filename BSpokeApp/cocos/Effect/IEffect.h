//
//  IEffect.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/9.
//
//

#ifndef __ShaderDemo__IEffect__
#define __ShaderDemo__IEffect__

#include "cocos2d.h"
#include "BaseEffectData.h"
#include "Effect.h"

USING_NS_CC;

class IEffect : public Ref{
    
public:
    EffectType getType(){ return m_effectType; }
    
public:
    virtual void applyToSprite(Sprite* sprite) = 0;
    virtual void applyToSpriteWithParent(Sprite* sprite, Node* parent){};
    virtual void cancelToSprite(Sprite* sprite) = 0;
    virtual void runAction(const Vec2& delta) = 0;
    virtual void reset(){};
    
public:
    virtual void updateColor(const Color3B& color) = 0;
    
public:
    virtual void convertToData(BaseEffectData* to) = 0;

public:
    virtual void render(){};
    
public:
    virtual void update(float dt){};
    
protected:
    IEffect(){
        m_glProgrameState = NULL;
        m_shaderName = "";
        m_effectType = EffectType_NONE;
        m_color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    virtual ~IEffect(){};
    
protected:
    std::string m_shaderName;
    EffectType m_effectType;
    GLProgramState* m_glProgrameState;
    
    //当前颜色
    Vec4 m_color;
    
protected:
    std::function<void(IEffect*)> m_completedCallback;
};

#endif /* defined(__ShaderDemo__IEffect__) */
