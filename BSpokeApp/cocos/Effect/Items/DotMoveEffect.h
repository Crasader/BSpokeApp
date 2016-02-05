//
//  DotMoveEffect.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/8.
//
//

#ifndef __ShaderDemo__DotMoveEffect__
#define __ShaderDemo__DotMoveEffect__

#include "cocos2d.h"
#include "IEffect.h"
#include "DotMoveEffectData.h"

USING_NS_CC;


class DotMoveEffect : public IEffect{

public:
    static DotMoveEffect* create();
    static DotMoveEffect* createFromData(DotMoveEffectData* from);
    bool init();
    bool initWithData(DotMoveEffectData* from);
    
public:
    virtual void applyToSprite(Sprite* sprite);
    virtual void cancelToSprite(Sprite* sprite){};
    virtual void runAction(const Vec2& delta);
    virtual void updateColor(const Color3B& color);
    
public:
    virtual void convertToData(BaseEffectData* to);
    
public:
    virtual void update(float dt);
    
protected:
    DotMoveEffect(){
        m_effectType = EffectType_DotMoveEffect;
    }

protected:
    //make sure coord not exceed the limit Rect
    Vec2 fixCoreCoord(const Vec2& coreCoord);
    
    
protected:
    //当前重心
    Vec2 m_coreCoord;
    
protected:
    Texture2D* m_texture;
    //遮罩与内容的大小比例
    Vec2 m_maskContentSizeRatio;
    //范围
    Rect m_limitRect;
    //动画开始的原始重心
    Vec2 m_originCoreCoord;
    
protected:
    Vec2 m_direction;
    //最大移动的范围
    float m_range;
    //没单位时间移动距离
    
    float m_moveStep;
    //逐渐减少的频率
    float m_tweenStep;
    
    //移动的总时间;
    float m_totalMoveTime;
    //tween的总时间
    float m_totalTweenTime;
    
};

#endif /* defined(__ShaderDemo__DotMoveEffect__) */
