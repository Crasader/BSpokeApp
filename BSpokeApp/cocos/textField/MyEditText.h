//
//  MyEditText.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/21.
//
//

#ifndef __ShaderDemo__MyEditText__
#define __ShaderDemo__MyEditText__

#include "MyTextField.h"
#include "ui/UIScale9Sprite.h"


class MyEditTextDelegate{

public:
    virtual void onBeginEdit() = 0;
    virtual void onEndEdit() = 0;
    
};


class MyEditText : public cocos2d::Node, public MyTextFieldDelegate{
  
public:
    static MyEditText* create(const std::string& placehold, const std::string& fontName, float fontSize, const cocos2d::Size& constraintSize);
    bool init(const std::string& placehold, const std::string& fontName, float fontSize, const cocos2d::Size& constraintSize);
    
public:
    void setOrientation(TextFieldOrientation orientation);
    void setVerticalAlignment(cocos2d::TextVAlignment alignment);
    void setHorizontalAlignment(cocos2d::TextHAlignment alignment);
    
public:
    void setBackground(const std::string& background, const cocos2d::Rect& rect, const cocos2d::Rect& rectInsets);

public:
    //还原成原来的文字
    void restore();
    void setOriginString(const std::string& str);
    void setString(const std::string& str);
    std::string getString();
    
    void setTextColor(const cocos2d::Color3B& color);
    
    void applyShadow(int shadow);
    
    void setEnable(bool enable){ m_isEnable = enable; }
    bool isEnable(){ return m_isEnable; }
    
    void setDelegate(MyEditTextDelegate* delegate){ m_delegate = delegate; }

public:
    void setCursor(const std::string& cursor);
    virtual void setContentSize(cocos2d::Size contentSize);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void update(float delta);
    
public:
    void attachWithIME();
    
public:
    virtual void onAttachWithIME();
    virtual void onDetachWithIME();
    virtual void onTextChange(const cocos2d::Size& contentSize);
    
protected:
    MyEditText();
    virtual ~MyEditText();
    
protected:
    bool m_isEnable;
    
    cocos2d::ui::Scale9Sprite *m_background;
    MyTextField *m_textField;

    bool m_isAttachWithIME;
    
    cocos2d::Sprite* m_cursor;
    float m_cursorFlashInterval;
    bool m_isCursorDirty;
    
    std::string m_originText;
    
protected:
    MyEditTextDelegate* m_delegate;

};

#endif /* defined(__ShaderDemo__MyEditText__) */
