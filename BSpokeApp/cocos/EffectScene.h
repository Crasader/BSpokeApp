#ifndef __EFFECT_SCENE_H__
#define __EFFECT_SCENE_H__

#include "cocos2d.h"
#include "IEffect.h"
#include "EffectManager.h"
#include "ui/UIEditBox/UIEditBox.h"

class EffectScene : public cocos2d::Layer, public EffectManagerDelegate
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static EffectScene* create();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
//    void menuCloseCallback(cocos2d::Ref* pSender);
	
public:
    //设置效果
    void setEffect(int effect);
    //设置图案效果颜色
    void setColor(cocos2d::Color3B color);
    //设置图案
	void setPattern(Texture2D* texture);
    //根据id设置图案
    void setPattern(int patternId);
    
    //还原效果
    void resetEffect();
    //暂停效果
    void pauseEffect();
    //继续效果
    void resumeEffect();
    //随机run效果
    void randomRunEffect();
    
    void setOnEffectActionCallback(const std::function<void(bool)>& func){ m_effectActionCallback = func; }
	
    void saveTexture(const std::function<void(Texture2D* texture)>& callback);
    void onSaveTextureCallback();
    
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    //effectManagerDelegate
    virtual void onEffectsBegan();
    virtual void onEffectsEnded();
    
protected:
    void runEffect(const cocos2d::Vec2& delta);
    void playSoundEffect();
    void runSceneAction();
    
protected:
    virtual ~EffectScene();
    
private:
    cocos2d::Node* m_boundNode;
    
    cocos2d::CustomCommand m_renderTextureFinishCommand;
    cocos2d::RenderTexture* m_renderTexture;
    std::function<void(Texture2D* texture)> m_saveTextureCallback;
    
    std::function<void(bool)> m_effectActionCallback;
    
private:
    cocos2d::Sprite* m_sprite;
    uint m_shakeCount;
    
private:
    EffectType m_effectType;
    bool m_isSleep;
};

#endif // __EFFECT_SCENE_H__
