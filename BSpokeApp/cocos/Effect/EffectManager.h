//
//  EffectManager.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/8.
//
//

#ifndef __ShaderDemo__EffectManager__
#define __ShaderDemo__EffectManager__

#include "cocos2d.h"
#include "IEffect.h"

USING_NS_CC;

class EffectManagerDelegate{

public:
    virtual void onEffectsBegan() = 0;
    virtual void onEffectsEnded() = 0;
};

class EffectManager{
    
public:
    static EffectManager* getInstance();
    bool init();
    void setShareEffectParent(Node* node);
    void setEffectManagerDelegate(EffectManagerDelegate* delegate){ m_delegate = delegate; }
    
public:
    void renderSpriteEffect(Sprite* sprite);
    
public:
    void updateEffectColorToSprite(Sprite* sprite);
    
    void resetEffectToSprite(Sprite* sprite);
    void applyEffectToSprite(Sprite* sprite, EffectType effectType);
    void cancelEffectFromSprite(Sprite* sprite);
    
public:
    void runEffectActionForSprite(Sprite* sprite, const Vec2& delta);
    void stopEffectActionForSprite(Sprite* sprite);
    void stopEffectAction(IEffect* effect);
    
public:
    void pauseEffect();
    void resumeEffect();
    
    void start();
    void stop();
    
public:
    void update(float dt);
    bool isPause(){ return m_isPause; }
    
protected:
    bool checkSpriteIsAvailable(Sprite* sprite);
    bool checkEffectIsApplied(Sprite* sprite, EffectType effectType);
    void startAndStopSchedule();
    
private:
    IEffect* getSpriteEffect(Sprite* sprite);
    
private:
    std::map<Sprite*, IEffect*> m_spriteEffectMapping;
    Vector<IEffect*> m_effectVector;
    Vector<IEffect*> m_runningActionEffectVector;
    EffectManagerDelegate* m_delegate;
    bool m_isPause;
};

#endif /* defined(__ShaderDemo__EffectManager__) */
