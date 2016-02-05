//
//  LinePaintEffect.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/5.
//
//

#ifndef __ShaderDemo__LinePaintEffect__
#define __ShaderDemo__LinePaintEffect__

#include "IEffect.h"
#include "cocos2d.h"

class LinePaintEffect : public IEffect{
    
    struct PaintItem{
        float originDuring;
        float during;
        float rotate;
        float scale;
        Vec2 diretion;
        Vec2 perpDirection;
        Sprite* sprite;
    };
    
public:
    static LinePaintEffect* create();
    bool init();
    
public:
    virtual void applyToSprite(Sprite* sprite);
    virtual void applyToSpriteWithParent(Sprite* sprite, Node* parent);
    virtual void cancelToSprite(Sprite* sprite);
    virtual void runAction(const Vec2& delta);
    virtual void updateColor(const cocos2d::Color3B &color);
    virtual void render();
    
public:
    virtual void convertToData(BaseEffectData* to){};
    
public:
    void applyOriginEffect();
    
public:
    virtual void update(float dt);
    
public:
    virtual ~LinePaintEffect();
    
protected:
    LinePaintEffect();
    
private:
    void testColorArea(Sprite* sprite);
    void onTestColorAreaCallback();
    
    PaintItem generatePaintItem(const Vec2 dir);
    bool applyAnimate(PaintItem& paintItem, float dt);
    
private:
    cocos2d::RenderTexture* m_testRenderTexture;
    cocos2d::CustomCommand m_testTextureFinishCommand;
    std::vector<cocos2d::Vec2> m_insidePointVec;
    
protected:
    SpriteBatchNode* m_paintNode;
    RenderTexture* m_renderTexture;
    cocos2d::Vec2 m_spritePosOffsetCenter;
    
protected:
    std::vector<PaintItem> m_runningPaintItem;
    
protected:
    float m_range;
    Vec2 m_direction;
    Vec2 m_dirPerp;
};

#endif /* defined(__ShaderDemo__LinePaintEffect__) */
