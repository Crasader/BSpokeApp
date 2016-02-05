//
//  LookbookScene.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/20.
//
//

#ifndef __ShaderDemo__LookbookScene__
#define __ShaderDemo__LookbookScene__

#include <stdio.h>
#include "cocos2d.h"
#include "json/document.h"
#include "MyTouchEventListener.h"
#include "textField/MyEditText.h"

class LookbookItem;
class LookbookItemCallback{
public:
    virtual void onDelete(LookbookItem* item) = 0;
    virtual void onFocus(LookbookItem* item) = 0;
    virtual void onTouched(LookbookItem* item) = 0;
};

class LookbookItem : public cocos2d::Node, public MyEditTextDelegate{
    
public:
    static LookbookItem* create(int itemId);
    bool initWithId(int itemId);
    
public:
    void setCallback(LookbookItemCallback* callback){ m_callback = callback; }
    
public:
    void focus();
    void unfocus();
    
    void applyShadow(int shadowType);
    void applyOpaque(GLubyte alpha);
    int getShadowType();
    float getOpaque();
    
public:
    void restore();
    
public:
    void applyEffectTexture(cocos2d::Texture2D* texture);
    void cancelEffectTexture();
    
public:
    virtual void onBeginEdit();
    virtual void onEndEdit();
    
public:
    
    void onClickCallback(cocos2d::Touch* touch, MyTouchEvent event);
    void onDragCallback(cocos2d::Touch* touch, MyTouchEvent event);
    void onDoubleClickCallback(cocos2d::Touch* touch, MyTouchEvent event);
    bool onTouchBeganCallback(cocos2d::Touch* touch, MyTouchEvent event);
    void onTouchEndedCallback(cocos2d::Touch* touch, MyTouchEvent event);
    
public:
    cocos2d::Node* getContentNode(){ return m_contentNode; }
    bool isApplyEffect(){ return m_isApplyEffect; }
    
public:
    virtual ~LookbookItem();
    
protected:
    LookbookItem();
    void generateLabel(const rapidjson::Value& jsonData, cocos2d::Node* parent);
    
private:
    bool m_isEdit;
    bool m_isFocus;
    bool m_isApplyEffect;
    bool m_isRotateScaleMode;
    cocos2d::Vec2 m_touchPrePosition;
    
    int m_shadowType;
    
    cocos2d::Node* m_selectNode;
    cocos2d::Node* m_closeBtn;
    cocos2d::Node* m_rotateAndScaleBtn;
    cocos2d::Node* m_cancelBtn;
    cocos2d::Node* m_contentNode;
    cocos2d::Sprite* m_contentTextureNode;
    
private:
    std::vector<cocos2d::Node*> m_editTextVec;
    
private:
    LookbookItemCallback* m_callback;
};

class LookbookScene : public cocos2d::Layer, LookbookItemCallback{
  
    struct LookbookLabelData{
        cocos2d::Vec2 position;
        cocos2d::Size size;
        std::string font;
        int fontSize;
    };
    
public:
    static LookbookScene* create();
    bool init();
    void reset();
    void cleanItems();
//    void menuCloseCallback(cocos2d::Ref* pSender);

public:
    void setBackground(cocos2d::Texture2D* texture);
    void applyLookbookItem(int itemId);
    void applyShadow(int shadowType);
    void applyOpaque(float alpha);
    
public:
    void applyTexture(cocos2d::Texture2D* texture);
    
public:
    virtual void onDelete(LookbookItem* item);
    virtual void onFocus(LookbookItem* item);
    virtual void onTouched(LookbookItem* item);
    
public:
    void setOnTouchDelegate(const std::function<void()>& func);
    void setOnItemFocusDelegate(const std::function<void(int,float)>& func);
    
public:
    void saveFocusItemTexture();
    cocos2d::Texture2D* getFocusItemTexture();
    void onSaveTextureCallback();
    
public:
    bool hasLookBookItem();

protected:
    virtual ~LookbookScene();
    
private:
    cocos2d::Sprite* m_bg;
    
private:
    std::vector<LookbookItem*> m_itemsVec;
    LookbookItem* m_focusItem;
    float m_focusItemScale;
    float m_focusItemRotate;
    
private:
    std::function<void()> m_onTouchCallback;
    std::function<void(int,float)> m_onItemFocusCallback;
    
private:
    cocos2d::CustomCommand m_renderTextureFinishCommand;
    cocos2d::RenderTexture* m_renderTexture;
};


#endif /* defined(__ShaderDemo__LookbookScene__) */
