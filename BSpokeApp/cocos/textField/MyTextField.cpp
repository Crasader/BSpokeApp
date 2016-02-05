//
//  MyTextField.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/20.
//
//

#include "MyTextField.h"

USING_NS_CC;

#define SHADER_IGNORE_SELF_COLOR "ignoreSelfColor"

float s_maxTextFieldWidth = -0.f;

int _calcCharCount(const char * text)
{
    int n = 0;
    char ch = 0;
    while ((ch = *text))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++text;
    }
    return n;
}

MyTextField::MyTextField():
m_delegate(NULL){
    
}

MyTextField* MyTextField::create(const std::string& placehold, const std::string& fontName, float fontSize){
    MyTextField *ret = new MyTextField();
    if(ret && ret->initWithPlaceHolder(placehold, fontName, fontSize)){
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool MyTextField::init(){
    //delegate
    TextFieldTTF::setDelegate(this);
    
    m_isUseConstraintSize = false;
    m_orientation = TextFieldOrientation::HORIZONTAL;
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(SHADER_IGNORE_SELF_COLOR);
    if(!program){
        program = GLProgram::createWithFilenames("shader/base_pos_tex.vert", "shader/base_ignoreSelfColor.frag");
        GLProgramCache::getInstance()->addGLProgram(program, SHADER_IGNORE_SELF_COLOR);
    }
    
    return true;
}

void MyTextField::insertText(const char * text, size_t len){
    
    std::string insert(text, len);
    
    if(m_orientation == TextFieldOrientation::VERTICAL){
        //在非\n字符的后面加\n
        if(memcmp("\n", text, len) != 0){
            insert.append("\n");
        }
    }
    
    if (len > 0)
    {
        if (_delegate && _delegate->onTextFieldInsertText(this, insert.c_str(), insert.length()))
        {
            // delegate doesn't want to insert text
            return;
        }
        
        _charCount += _calcCharCount(insert.c_str());
        std::string sText(_inputText);
        sText.append(insert);
        setString(sText);
    }
}

void MyTextField::deleteBackward(){
    TextFieldTTF::deleteBackward();
    
    if(m_orientation == TextFieldOrientation::VERTICAL){
        //删非\n的字符
        if(_inputText.length() > 0 && _inputText.at(_inputText.length() - 1) != '\n'){
            TextFieldTTF::deleteBackward();
        }
    }
    
    if(_charCount == 0){
        updateContent();
    }
}

void MyTextField::updateContent(){
    TextFieldTTF::updateContent();
    _textSprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(SHADER_IGNORE_SELF_COLOR));
    
    if(m_isUseConstraintSize){
        Size contentSize = _textSprite->getContentSize();
        if(m_orientation == TextFieldOrientation::HORIZONTAL){
            if(contentSize.width > m_constraintSize.width){
                float scaleX = m_constraintSize.width / contentSize.width;
                _textSprite->setScaleX(scaleX);
            }else{
                _textSprite->setScaleX(1.0);
            }
        }
    }
    
    if(m_delegate){
        Size contentSize = _textSprite->getContentSize();
        
        contentSize.width *= _textSprite->getScaleX();
        contentSize.height *= _textSprite->getScaleY();
        
        m_delegate->onTextChange(contentSize);
    }
}

void MyTextField::setConstraintSize(const Size& constraint){
    m_constraintSize = constraint;
    
    if(m_orientation == TextFieldOrientation::HORIZONTAL){
        this->setDimensions(s_maxTextFieldWidth, m_constraintSize.height);
    }
}

void MyTextField::setOrientation(TextFieldOrientation orientation){
    m_orientation = orientation;
    
    if(m_orientation == TextFieldOrientation::HORIZONTAL){
        this->setDimensions(s_maxTextFieldWidth, m_constraintSize.height);
    }
}

bool MyTextField::onTextFieldAttachWithIME(TextFieldTTF * sender){
    if(m_delegate)
        m_delegate->onAttachWithIME();
    
    return false;
}

bool MyTextField::onTextFieldDetachWithIME(TextFieldTTF * sender){
    if(m_delegate)
        m_delegate->onDetachWithIME();
    
    return false;
}

bool MyTextField::onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen){
    if(m_orientation == TextFieldOrientation::HORIZONTAL){
        if(memcmp("\n", text, nLen) == 0){
            return true;
        }
    }
    return false;
}

bool MyTextField::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen){
    return false;
}
