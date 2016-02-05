//
//  MyTextField.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/20.
//
//

#ifndef __ShaderDemo__MyTextField__
#define __ShaderDemo__MyTextField__

#include "cocos2d.h"

enum TextFieldOrientation{
    VERTICAL, HORIZONTAL
};

class MyTextFieldDelegate{

public:
    virtual void onAttachWithIME() = 0;
    virtual void onDetachWithIME() = 0;
    virtual void onTextChange(const cocos2d::Size& contentSize) = 0;
    
};

class MyTextField : public cocos2d::TextFieldTTF, cocos2d::TextFieldDelegate{
    
public:
    static MyTextField *create(const std::string& placehold, const std::string& fontName, float fontSize);
    bool init();
    
public:
    virtual void insertText(const char * text, size_t len) override;
    virtual void deleteBackward() override;
    virtual void updateContent();
    
public:
    void setIsUseContraintSize(bool isUse){m_isUseConstraintSize = isUse;}
    void setConstraintSize(const cocos2d::Size& constraint);
    
public:
    void setOrientation(TextFieldOrientation orientation);
    
public:
    void setDelegate(MyTextFieldDelegate *delegate){m_delegate = delegate;}
    
public:
    /**
     @brief    If the sender doesn't want to attach to the IME, return true;
     */
    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
    
    /**
     @brief    If the sender doesn't want to detach from the IME, return true;
     */
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
    
    /**
     @brief    If the sender doesn't want to insert the text, return true;
     */
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
    
    /**
     @brief    If the sender doesn't want to delete the delText, return true;
     */
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

protected:
    MyTextField();
    
protected:
    bool m_isUseConstraintSize;
    cocos2d::Size m_constraintSize;
    
    TextFieldOrientation m_orientation;
    
    MyTextFieldDelegate* m_delegate;
};

#endif /* defined(__ShaderDemo__MyTextField__) */
