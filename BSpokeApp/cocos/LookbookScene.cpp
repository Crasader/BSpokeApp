//
//  LookbookScene.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/20.
//
//

#include "LookbookScene.h"
#include "EffectScene.h"
#include "util/CocosUtil.h"
#include "ui/UIScale9Sprite.h"
#include "SceneManager.h"

USING_NS_CC;
using namespace pp;
using namespace cocos2d::ui;

//#define MENUITEM_TAG_APPLY 1
#define LOOKBOOK_CONFIG_FILE "config/lookbook.json"
#define DEFAULT_TEXT_COLOR Color3B::WHITE
#define SHADER_IGNORE_SELF_COLOR "ignoreSelfColor"

LookbookItem* LookbookItem::create(int itemId){
    LookbookItem* ret = new LookbookItem();
    if(ret && ret->initWithId(itemId)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

LookbookItem::LookbookItem():
m_selectNode(NULL),
m_closeBtn(NULL),
m_rotateAndScaleBtn(NULL),
m_cancelBtn(NULL),
m_callback(NULL){
    
}

LookbookItem::~LookbookItem(){
}

bool LookbookItem::initWithId(int itemId){
    if(!Node::init()){
        return false;
    }
    m_isApplyEffect     = false;
    m_isRotateScaleMode = false;
    m_shadowType = 0;
    m_isFocus = false;
    m_isEdit  = false;
    
    GLProgram* program = GLProgramCache::getInstance()->getGLProgram(SHADER_IGNORE_SELF_COLOR);
    if(!program){
        program = GLProgram::createWithFilenames("shader/base_pos_tex.vert", "shader/base_ignoreSelfColor.frag");
        GLProgramCache::getInstance()->addGLProgram(program, SHADER_IGNORE_SELF_COLOR);
    }
    
    //eventDispatcher
    MyTouchEventListener* eventListener = MyTouchEventListener::createWithTarget(this);
    eventListener->setSwallowTouches(true);
    eventListener->onClickListener = CC_CALLBACK_2(LookbookItem::onClickCallback, this);
    eventListener->onDragListener = CC_CALLBACK_2(LookbookItem::onDragCallback, this);
    eventListener->onDoubleClickListener = CC_CALLBACK_2(LookbookItem::onDoubleClickCallback, this);
    eventListener->onTouchBeganListener = CC_CALLBACK_2(LookbookItem::onTouchBeganCallback, this);
    eventListener->onTouchEndedListener = CC_CALLBACK_2(LookbookItem::onTouchEndedCallback, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    //parse json
    std::string fullPath    = FileUtils::getInstance()->fullPathForFilename(LOOKBOOK_CONFIG_FILE);
    std::string contentStr  = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document doc;
    doc.Parse<0>(contentStr.c_str());
    if (doc.HasParseError())
    {
        CCASSERT(false, doc.GetParseError());
    }
    
    const rapidjson::Value& dataJson = doc["data"];
    int dataSize = dataJson.Size();
    
    for(int i = 0; i < dataSize; i++){
        const rapidjson::Value& subData = dataJson[i];
        int id = subData["id"].GetInt();
        
        if(itemId == id){
            std::string bg = subData["bg"].GetString();
            char bgPath[128];
            sprintf(bgPath, "lookbook/%s", bg.c_str());
            
            Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(bgPath);
            Size nodeSize = texture->getContentSize();
            
            this->setContentSize(nodeSize);
            
            //select node
            m_selectNode = Scale9Sprite::create("editbox.png", Rect(0, 0, 20, 20), Rect(9, 9, 2, 2));
            m_selectNode->setContentSize(nodeSize);
            m_selectNode->setPosition(Vec2(nodeSize.width / 2, nodeSize.height / 2));
            m_selectNode->setVisible(false);
            this->addChild(m_selectNode);
            
            m_contentTextureNode = Sprite::create();
            m_contentTextureNode->setPosition(Vec2(nodeSize.width / 2, nodeSize.height / 2));
            m_contentTextureNode->setVisible(false);
            m_contentTextureNode->setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(SHADER_IGNORE_SELF_COLOR));
            this->addChild(m_contentTextureNode);
            
            //btn close
            m_closeBtn = Sprite::create("btn_close.png");
            m_closeBtn->setPosition(Vec2(0, nodeSize.height));
            m_selectNode->addChild(m_closeBtn);
            
            //btn scale and rotate
            m_rotateAndScaleBtn = Sprite::create("btn_rotate.png");
            m_rotateAndScaleBtn->setPosition(Vec2(nodeSize.width , 0));
            m_selectNode->addChild(m_rotateAndScaleBtn);
            
            m_cancelBtn = Sprite::create("btn_cancel.png");
            m_cancelBtn->setPosition(Vec2(nodeSize.width, nodeSize.height));
            m_cancelBtn->setVisible(false);
            m_selectNode->addChild(m_cancelBtn);
            
            //bg
            m_contentNode = Sprite::createWithTexture(texture);
            m_contentNode->setPosition(Vec2(nodeSize.width / 2, nodeSize.height / 2));
            m_contentNode->setCascadeOpacityEnabled(true);
            this->addChild(m_contentNode);
            
            
            //labels
            const rapidjson::Value& labelsJson = subData["labels"];
            int labelsSize = labelsJson.Size();
            
            for(int i = 0; i < labelsSize; i++){
                this->generateLabel(labelsJson[i], m_contentNode);
            }
            break;
        }
    }

    return true;
}

void LookbookItem::focus(){
    if(m_isFocus)
        return;
    
    m_isFocus = true;
    m_selectNode->setVisible(true);
    
    if(m_callback){
        m_callback->onFocus(this);
    }
}

void LookbookItem::unfocus(){
    if(!m_isFocus)
        return;
    
    m_isFocus = false;
    m_selectNode->setVisible(false);
}

void LookbookItem::applyShadow(int shadowType){
    if(m_shadowType == shadowType)
        return;
    
    for(cocos2d::Node* node : m_editTextVec){
        MyEditText* editText = (MyEditText*)node;
        editText->applyShadow(shadowType);
    }
    
    m_shadowType = shadowType;
}

void LookbookItem::applyOpaque(GLubyte alpha){
    m_contentNode->setOpacity(alpha);
}

int LookbookItem::getShadowType(){
    return m_shadowType;
}

float LookbookItem::getOpaque(){
    return m_contentNode->getOpacity();
}

void LookbookItem::restore(){
    this->cancelEffectTexture();
    //还原文本
    for(Node* node : m_editTextVec){
        MyEditText* editText = (MyEditText*)node;
        editText->restore();
    }
}

void LookbookItem::applyEffectTexture(cocos2d::Texture2D* texture){
    if(m_contentTextureNode){
        Rect rect    = Rect::ZERO;
        rect.size = texture->getContentSize();
        
        m_contentTextureNode->setTexture(texture);
        m_contentTextureNode->setTextureRect(rect);
        
        Size targetSize = m_contentNode->getContentSize();
        m_contentTextureNode->setScale(targetSize.width / 500, targetSize.height / 500);
        
        if(!m_isApplyEffect){
            m_isApplyEffect = true;
            m_contentTextureNode->setVisible(m_isApplyEffect);
            m_contentNode->setVisible(!m_isApplyEffect);
        }
    }
    
    m_cancelBtn->setVisible(true);
}

void LookbookItem::cancelEffectTexture(){
    if(m_isApplyEffect){
        m_cancelBtn->setVisible(false);
        m_isApplyEffect = false;
        m_contentTextureNode->setVisible(m_isApplyEffect);
        m_contentNode->setVisible(!m_isApplyEffect);
    }
}

void LookbookItem::onBeginEdit(){
    m_isEdit = true;
}

void LookbookItem::onEndEdit(){
    m_isEdit = false;
}

void LookbookItem::onClickCallback(cocos2d::Touch* touch, MyTouchEvent event){
    Vec2 touchLocation = touch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    if(m_isFocus){
        //点击close
        Rect closeRect;
        closeRect.origin = m_closeBtn->getPosition() - Vec2(m_closeBtn->getContentSize().width / 2, m_closeBtn->getContentSize().height / 2);
        closeRect.size   = m_closeBtn->getContentSize();
        
        if(m_closeBtn->getBoundingBox().containsPoint(touchLocation)){
            //删除
            m_callback->onDelete(this);
        }else if(m_cancelBtn->isVisible() && m_cancelBtn->getBoundingBox().containsPoint(touchLocation)){
            //还原
            this->restore();
        }else{
            m_callback->onTouched(this);
        }
    }else{
        this->focus();
    }
}

void LookbookItem::onDragCallback(cocos2d::Touch* touch, MyTouchEvent event){
    Vec2 touchLocation      = touch->getLocation();
    Vec2 positionLocation   = this->getParent()->convertToWorldSpace(this->getPosition());
    
    if(m_isRotateScaleMode){
        //拉伸
        float distanceToCenter = m_touchPrePosition.distance(positionLocation);
        float curDistanceToCenter = touchLocation.distance(positionLocation);
        float scale = curDistanceToCenter / distanceToCenter;
        
        this->setScale(_scaleX * scale);
        
        //旋转
        Vec2 beginVec = m_touchPrePosition - positionLocation;
        Vec2 endVec   = touchLocation - positionLocation;
        float angle = endVec.getAngle(beginVec);
        
        this->setRotation(_rotationZ_X + CC_RADIANS_TO_DEGREES(angle));
        
        m_touchPrePosition = touchLocation;
    }else{
        touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
        this->setPosition(touchLocation);
    }
}

void LookbookItem::onDoubleClickCallback(cocos2d::Touch* touch, MyTouchEvent event){
    if(m_isFocus){
        if(m_isApplyEffect)
            return;
        
        Vec2 touchLocation = touch->getLocation();
        
        for(Node* node : m_editTextVec){
            Vec2 touchInView = node->convertToNodeSpace(touchLocation);
            Rect rect = Rect::ZERO;
            
            rect.size = node->getContentSize();
            
            if(rect.containsPoint(touchInView)){
                MyEditText* editText = (MyEditText*)node;
                editText->attachWithIME();
                
                break;
            }
        }
    }else{
        this->focus();
    }
}

bool LookbookItem::onTouchBeganCallback(cocos2d::Touch* touch, MyTouchEvent event){
    Vec2 touchLocation = touch->getLocation();
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    Rect rect;
    rect.size = _contentSize;
    
    
    if(m_isFocus){
        //点击close
        
        if(m_closeBtn->getBoundingBox().containsPoint(touchLocation)){
            //删除
            return true;
        }
        
        //点击rotate scale btn
        if(m_rotateAndScaleBtn->getBoundingBox().containsPoint(touchLocation)){
            //旋转
            m_isRotateScaleMode  = true;
            m_touchPrePosition = touch->getLocation();
            return true;
        }
        
        //cancelbtn
        if(m_cancelBtn->getBoundingBox().containsPoint(touchLocation)){
            //还原
            return true;
        }
    }
    
    if(rect.containsPoint(touchLocation))
        return true;
    
    if(m_isFocus)
        this->unfocus();
    
    return false;
}

void LookbookItem::onTouchEndedCallback(cocos2d::Touch* touch, MyTouchEvent event){
    if(m_isRotateScaleMode){
        m_isRotateScaleMode = false;
        m_touchPrePosition  = Vec2::ZERO;
    }
}

void LookbookItem::generateLabel(const rapidjson::Value& jsonData, cocos2d::Node* parent){
    std::string strPosition = jsonData["position"].GetString();
    std::string strSize = jsonData["size"].GetString();
    std::string strFont = jsonData["font"].GetString();
    std::string strContent = jsonData["content"].GetString();
    int fontSize = jsonData["fontSize"].GetInt();
    
    char fontPath[128];
    sprintf(fontPath, "fonts/%s", strFont.c_str());
    
    Vec2 position;
    Size size;
    
    CocosUtil::stringToVec2(strPosition, position);
    CocosUtil::stringToSize(strSize, size);
    
    MyEditText *editText = MyEditText::create("", fontPath, fontSize, size);
    editText->setEnable(false);
    editText->setTextColor(DEFAULT_TEXT_COLOR);
    editText->setOriginString(strContent);
    editText->setOrientation(TextFieldOrientation::HORIZONTAL);
    editText->setContentSize(size);
    editText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    editText->setPosition(position);
    editText->setDelegate(this);
    
//    editText->setBackground("editbox.png", Rect(0, 0, 7, 7), Rect(2.5, 2.5, 2, 2));
    editText->setCursor("typeline.png");
    
    parent->addChild(editText);
    
    m_editTextVec.push_back(editText);
}


/**********************   LookbookScene   ***************************/

LookbookScene::~LookbookScene(){
    CC_SAFE_RELEASE(m_renderTexture);
    m_renderTexture = NULL;
}

LookbookScene* LookbookScene::create(){
    LookbookScene *ret = new LookbookScene();
    
    ret->init();
    
    ret->autorelease();
    
    return ret;
}

bool LookbookScene::init(){
    if(!Layer::init())
        return false;
    
    m_onTouchCallback = NULL;
    m_onItemFocusCallback = NULL;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* colorLayer = LayerColor::create(Color4B(17, 21, 27, 255));
    this->addChild(colorLayer);
    
    m_bg = Sprite::create();
    m_bg->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(m_bg);
    
//    Menu* menu = Menu::create();
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu);
//    MenuItemFont *jumpToEffect = MenuItemFont::create("Jump to Effect", CC_CALLBACK_1(LookbookScene::menuCloseCallback, this));
//    jumpToEffect->setPosition(origin + Vec2(80, jumpToEffect->getContentSize().height));
//    jumpToEffect->setTag(MENUITEM_TAG_APPLY);
//    menu->addChild(jumpToEffect);
    
    this->reset();
    
    return true;
}

void LookbookScene::reset(){
    m_focusItem = NULL;
    m_itemsVec.clear();
}

void LookbookScene::cleanItems(){
    for(LookbookItem* item : m_itemsVec){
        item->removeFromParent();
    }
    
    this->reset();
}

/**
void LookbookScene::menuCloseCallback(cocos2d::Ref* pSender){
    Node* node = (Node*)pSender;
    
    switch (node->getTag()) {
        case MENUITEM_TAG_APPLY:
        {
            this->saveFocusItemTexture();
        }
        break;
        
        default:
        break;
    }
}
*/

void LookbookScene::setBackground(cocos2d::Texture2D* texture){
    Rect rect    = Rect::ZERO;
    rect.size = texture->getContentSize();
    
    m_bg->setTexture(texture);
    m_bg->setTextureRect(rect);
}

void LookbookScene::applyLookbookItem(int itemId){
    LookbookItem* item = LookbookItem::create(itemId);
    if(item){
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        item->setPosition(Vec2(m_bg->getContentSize().width / 2, m_bg->getContentSize().height / 2));
        item->setCallback(this);
        m_bg->addChild(item);
        
        m_itemsVec.push_back(item);
    }
}

void LookbookScene::applyShadow(int shadowType){
    if(m_focusItem){
        m_focusItem->applyShadow(shadowType);
    }
}

void LookbookScene::applyOpaque(float alpha){
    if(m_focusItem){
        m_focusItem->applyOpaque(alpha);
    }
}

void LookbookScene::applyTexture(cocos2d::Texture2D* texture){
    if(m_focusItem){
        m_focusItem->applyEffectTexture(texture);
    }
}

void LookbookScene::onDelete(LookbookItem* item){
    for(std::vector<LookbookItem*>::iterator it = m_itemsVec.begin(); it != m_itemsVec.end(); it++){
        if(*it == item){
            m_itemsVec.erase(it);
            item->removeFromParent();
            break;
        }
    }
    m_focusItem = NULL;
}

void LookbookScene::onFocus(LookbookItem* item){
    if(m_focusItem != item && m_focusItem){
        m_focusItem->unfocus();
    }
    
    m_focusItem = item;
    
    if(m_onItemFocusCallback)
        m_onItemFocusCallback(item->getShadowType(), item->getOpaque());
}

void LookbookScene::onTouched(LookbookItem* item){
    if(m_focusItem == item){
        if(!item->isApplyEffect()){
            this->saveFocusItemTexture();
        }
        
        if(m_onTouchCallback)
            m_onTouchCallback();
    }
}

void LookbookScene::setOnTouchDelegate(const std::function<void()>& func){
    m_onTouchCallback = func;
}

void LookbookScene::setOnItemFocusDelegate(const std::function<void(int,float)>& func){
    m_onItemFocusCallback = func;
}

void LookbookScene::saveFocusItemTexture(){
    CC_SAFE_RELEASE(m_renderTexture);
    
    m_focusItemRotate = m_focusItem->getRotation();
    m_focusItemScale = m_focusItem->getScale();
    m_focusItem->setRotation(0);
    m_focusItem->setScale(1.0f);
    
    Node* node = m_focusItem->getContentNode();

    cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    cocos2d::Size pixelSize   = cocos2d::Director::getInstance()->getWinSizeInPixels();
    cocos2d::Vec2 boundNodeZeroToWorld = node->convertToWorldSpace(Vec2::ZERO);
    
    m_renderTexture = RenderTexture::create(node->getContentSize().width, node->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
    m_renderTexture->retain();
    m_renderTexture->setKeepMatrix(true);
    m_renderTexture->setVirtualViewport(boundNodeZeroToWorld, Rect(0, 0, visibleSize.width, visibleSize.height), Rect(0, 0, pixelSize.width, pixelSize.height));
    
    m_renderTexture->beginWithClear(0.f, 0.f, 0.f, 0.f);
    node->visit(Director::getInstance()->getRenderer(), node->getParent()->getNodeToWorldTransform(), true);
    m_renderTexture->end();
    
    m_renderTextureFinishCommand.init(m_renderTexture->getGlobalZOrder());
    m_renderTextureFinishCommand.func = CC_CALLBACK_0(LookbookScene::onSaveTextureCallback, this);
    
    Director::getInstance()->getRenderer()->addCommand(&m_renderTextureFinishCommand);
}

void LookbookScene::onSaveTextureCallback(){
    m_focusItem->setScale(m_focusItemScale);
    m_focusItem->setRotation(m_focusItemRotate);
}

cocos2d::Texture2D* LookbookScene::getFocusItemTexture(){
    Sprite* sprite      = m_renderTexture->getSprite();
    Texture2D* texture  = sprite->getTexture();
    
    Texture2D* newTexture = CocosUtil::resizeTexture2D(texture, Size(500, 500), false);
    newTexture->autorelease();
    
    return newTexture;
}

bool LookbookScene::hasLookBookItem(){
    return m_itemsVec.size() > 0;
}