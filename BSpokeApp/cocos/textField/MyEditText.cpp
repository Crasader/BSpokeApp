//
//  MyEditText.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/21.
//
//

#include "MyEditText.h"

#define CURSOR_FLASH_INTERVAL 0.6
#define CURSOR_OFFSET 4

USING_NS_CC;

MyEditText::MyEditText():
m_background(NULL),
m_cursor(NULL),
m_delegate(NULL)
{

}

MyEditText::~MyEditText(){
    
}

MyEditText* MyEditText::create(const std::string& placehold, const std::string& fontName, float fontSize, const Size& constraintSize){
    MyEditText* ret = new MyEditText();
    if(ret && ret->init(placehold, fontName, fontSize, constraintSize)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    
    return NULL;
}

bool MyEditText::init(const std::string& placehold, const std::string& fontName, float fontSize, const Size& constraintSize){
    if(!Node::init())
        return false;
    
    this->setCascadeOpacityEnabled(true);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    //touch
    EventListenerTouchOneByOne *touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = [=](Touch* touch, Event* event)->bool{
        if(event->getCurrentTarget() == this){
            Vec2 touchLocation = touch->getLocation();
            touchLocation = this->convertToNodeSpace(touchLocation);
            
            Rect rect;
            rect.size = _contentSize;
            
            if(rect.containsPoint(touchLocation)){
                if(m_isEnable || m_isAttachWithIME)
                    return true;
            }else{
                if(m_isAttachWithIME){
                    m_textField->detachWithIME();
                    return true;
                }
            }
        }
        return false;
    };
    touchEventListener->onTouchEnded = [=](Touch* touch, Event* event){
        if(m_isEnable && event->getCurrentTarget() == this){
            m_textField->attachWithIME();
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    //textField
    m_textField = MyTextField::create(placehold, fontName, fontSize);
    m_textField->setContentSize(constraintSize);
    m_textField->setPosition(Vec2::ZERO);
    m_textField->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    m_textField->setVerticalAlignment(TextVAlignment::TOP);
    m_textField->setHorizontalAlignment(TextHAlignment::LEFT);
    m_textField->setOrientation(TextFieldOrientation::HORIZONTAL);
    //限制宽度
    m_textField->setConstraintSize(constraintSize);
    m_textField->setIsUseContraintSize(true);
    this->addChild(m_textField);
    
    m_textField->setDelegate(this);
    
    m_isAttachWithIME = false;
    m_isCursorDirty = false;
    m_isEnable = true;
    
    return true;
}

void MyEditText::setOrientation(TextFieldOrientation orientation){
    m_textField->setOrientation(orientation);
}

void MyEditText::setVerticalAlignment(TextVAlignment alignment){
    m_textField->setVerticalAlignment(alignment);
}

void MyEditText::setHorizontalAlignment(TextHAlignment alignment){
    m_textField->setHorizontalAlignment(alignment);
}

void MyEditText::setContentSize(Size contentSize){
    Node::setContentSize(contentSize);
    
    //bg
    if(m_background){
        m_background->setContentSize(_contentSize);
        m_background->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
    }
}

void MyEditText::setBackground(const std::string& background, const Rect& rect, const Rect& rectInsets){
    if(m_background){
        m_background->removeFromParent();
        m_background = NULL;
    }
    m_background = ui::Scale9Sprite::create(background, rect, rectInsets);
    m_background->setContentSize(_contentSize);
    m_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_background->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
    this->addChild(m_background);
}

void MyEditText::restore(){
    this->setString(m_originText);
}

void MyEditText::setOriginString(const std::string& str){
    m_originText = str;
    
    this->setString(str);
}

void MyEditText::setString(const std::string& str){
    m_textField->setString(str);
}

std::string MyEditText::getString(){
    return m_textField->getString();
}

void MyEditText::setTextColor(const Color3B& color){
    m_textField->setTextColor(Color4B(color.r, color.g, color.b, 255));
}

void MyEditText::applyShadow(int shadow){
    switch (shadow) {
        case 0:
            m_textField->disableEffect();
            break;
            
        case 1:
            m_textField->enableShadow();
            break;
            
        default:
            break;
    }
}

void MyEditText::setCursor(const std::string& cursor){
    if(m_cursor){
        m_cursor->removeFromParent();
        m_cursor = NULL;
    }
    
    m_cursor = Sprite::create(cursor);
    m_cursor->setPosition(Vec2(CURSOR_OFFSET, _contentSize.height / 2));
    m_cursor->setVisible(false);
    this->addChild(m_cursor);
    
    m_isCursorDirty = true;
}

void MyEditText::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags){
    
    if(_contentSizeDirty || m_isCursorDirty){
        if(m_cursor){
            m_cursor->setPositionY(_contentSize.height / 2);
            
            float scale = (_contentSize.height - 10) / m_cursor->getContentSize().height;
            m_cursor->setScaleY(scale);
            
            m_isCursorDirty = false;
        }
    }
    
    Node::visit(renderer, parentTransform, parentFlags);
}

void MyEditText::update(float delta){
    if(m_cursor){
        m_cursorFlashInterval += delta;
        if(m_cursorFlashInterval > CURSOR_FLASH_INTERVAL){
            m_cursor->setVisible(!m_cursor->isVisible());
            m_cursorFlashInterval = 0;
        }
    }
}

void MyEditText::attachWithIME(){
    m_textField->attachWithIME();
}

void MyEditText::onAttachWithIME(){
    this->scheduleUpdate();
    
    m_isAttachWithIME = true;
    
    if(m_delegate)
        m_delegate->onBeginEdit();
}

void MyEditText::onDetachWithIME(){
    this->unscheduleUpdate();
    
    m_isAttachWithIME = false;
    
    if(m_cursor){
        m_cursor->setVisible(false);
    }
    
    if(m_delegate)
        m_delegate->onEndEdit();
}

void MyEditText::onTextChange(const Size& contentSize){
    if(m_cursor)
        m_cursor->setPositionX(contentSize.width + CURSOR_OFFSET);
}