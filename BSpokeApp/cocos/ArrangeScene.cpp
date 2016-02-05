//
//  ArrangeScene.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/1.
//
//
//#include "SceneManager.h"
#include "ArrangeScene.h"
#include "json/document.h"
#include "util/CocosUtil.h"
#include "LeggingHelper.h"

using namespace pp;
USING_NS_CC;

/**
#define MENUITEM_TAG_FRONT 1
#define MENUITEM_TAG_SIDE 2
#define MENUITEM_TAG_BACK 3

#define MENUITEM_TAG_ARRANGE1 5
#define MENUITEM_TAG_SCALE 9
#define MENUITEM_TAG_CANCEL 10
*/

#define ARRANGE_CONFIG_FILE "config/arrange.json"
#define ARRANGE_SCENE_BTN_ZORDER 1


//#define PATTERN_NODE_INDEX 1

ArrangeScene::ArrangeScene():
m_onArrangeBtnClickedCallback(NULL),
m_onArrangeItemClickedCallback(NULL),
m_onArrangeItemSelectStateCallback(NULL),
m_onTouchCallback(NULL),
m_selectedArrangeItem(NULL),
m_selectedArrangeBtn(NULL),
m_texture(NULL)
{

}

ArrangeScene::~ArrangeScene(){
    CC_SAFE_RELEASE(m_texture);
}

bool ArrangeScene::init(){
    
    if(!Layer::init()){
        return false;
    }
    
    m_isLeggingStyleDirty = false;
    m_leftMaskTextureVec.resize(4);
    m_rightMaskTextureVec.resize(4);
    
    this->initConfig();
    this->initMask();
    this->parseArrangeJson();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();
    
    m_area.size   = visibleSize;
    m_area.origin = origin;
    
    LayerColor* colorLayer = LayerColor::create(Color4B(240, 245, 248, 255));
    this->addChild(colorLayer);
    
    //legging image
    m_image = Sprite::create();
    m_image->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(m_image);
    
    Size pantNodeSize = m_arrangeConfig.pantNodeSize;
    
    //左脚pattern视图。
    m_leftPatternContainerNode = ClippingNode::create();
//    m_leftPatternContainerNode = Node::create();
    m_leftPatternContainerNode->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->setAlphaThreshold(0.1f);
    m_leftPatternContainerNode->setInverted(false);
    
    Node* stencil = Node::create();
    stencil->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->setStencil(stencil);
    
    //右脚pattern视图。
    m_rightPatternContainerNode = ClippingNode::create();
//    m_rightPatternContainerNode = Node::create();
    m_rightPatternContainerNode->setContentSize(pantNodeSize);
    m_rightPatternContainerNode->setAlphaThreshold(0.1f);
    m_rightPatternContainerNode->setInverted(false);
    
    stencil = Node::create();
    stencil->setContentSize(pantNodeSize);
    m_rightPatternContainerNode->setStencil(stencil);
    
    m_image->addChild(m_rightPatternContainerNode);
    m_image->addChild(m_leftPatternContainerNode);
    
    m_leftPatternSliceNode = Node::create();
    m_leftPatternSliceNode->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->addChild(m_leftPatternSliceNode);
    
    m_rightPatternSliceNode = Node::create();
    m_rightPatternSliceNode->setContentSize(pantNodeSize);
    m_rightPatternContainerNode->addChild(m_rightPatternSliceNode);
    
    m_rightPatternSliceNodeCopy = Node::create();
    m_rightPatternSliceNodeCopy->setContentSize(pantNodeSize);
    m_rightPatternSliceNodeCopy->setVisible(false);
    m_rightPatternContainerNode->addChild(m_rightPatternSliceNodeCopy);
    
    this->setScale(true);
    this->setLeggingStyle(LeggingType_9Cutting, "black", LeggingOrientation_Back);
    
    MyComplexTouchEventListener* listener = MyComplexTouchEventListener::createWithTarget(m_image);
    listener->onClick = [=](){
        this->selectItem(NULL);
    };
    listener->onDrag = [=](const Vec2& preTouchPosition, const Vec2& curTouchPosition){
        if(m_isScale)
            return;
        
        Vec2 offset   = curTouchPosition - preTouchPosition;
        Vec2 position = m_image->getPosition() + offset;
        
        this->fixImageMovePosition(position);
        
        m_image->setPosition(position);
    };
    listener->onSpin = CC_CALLBACK_1(ArrangeScene::onSpin, this);
    listener->onStretch = CC_CALLBACK_2(ArrangeScene::onStretch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_image);
    
    //menuitem
	/**
    Color3B color = Color3B::BLACK;
    float y = 0;
    auto leaveBtn = MenuItemFont::create("cancel", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y += leaveBtn->getContentSize().height;
    leaveBtn->setColor(color);
    leaveBtn->setPosition(origin + Vec2(visibleSize.width - leaveBtn->getContentSize().width, y));
    leaveBtn->setTag(MENUITEM_TAG_CANCEL);
    
    auto arrange1Btn = MenuItemFont::create("arrange1", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y += arrange1Btn->getContentSize().height;
    arrange1Btn->setColor(color);
    arrange1Btn->setPosition(origin + Vec2(visibleSize.width - arrange1Btn->getContentSize().width, y));
    arrange1Btn->setTag(MENUITEM_TAG_ARRANGE1);
    
    auto scaleBtn = MenuItemFont::create("scale", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y += scaleBtn->getContentSize().height;
    scaleBtn->setColor(color);
    scaleBtn->setPosition(origin + Vec2(visibleSize.width - scaleBtn->getContentSize().width, y));
    scaleBtn->setTag(MENUITEM_TAG_SCALE);
    
    auto frontBtn = MenuItemFont::create("front", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y = frontBtn->getContentSize().height;
    frontBtn->setColor(color);
    frontBtn->setPosition(origin + Vec2(frontBtn->getContentSize().width, y));
    frontBtn->setTag(MENUITEM_TAG_FRONT);
    
    auto sideBtn = MenuItemFont::create("side", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y += sideBtn->getContentSize().height;
    sideBtn->setColor(color);
    sideBtn->setPosition(origin + Vec2(sideBtn->getContentSize().width, y));
    sideBtn->setTag(MENUITEM_TAG_SIDE);
    
    auto backBtn = MenuItemFont::create("back", CC_CALLBACK_1(ArrangeScene::menuItemCallback, this));
    y += backBtn->getContentSize().height;
    backBtn->setColor(color);
    backBtn->setPosition(origin + Vec2(backBtn->getContentSize().width, y));
    backBtn->setTag(MENUITEM_TAG_BACK);
    
    Menu* menu = Menu::create(leaveBtn, arrange1Btn, frontBtn, sideBtn, backBtn, scaleBtn, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
*/
    return true;
}

void ArrangeScene::initConfig(){
    m_arrangeConfig = ArrangeConfig(m_pantType);
}

void ArrangeScene::initMask(){
    //init mask
    TextureCache* cache     = Director::getInstance()->getTextureCache();
//    m_leftFrontMaskTexture  = cache->addImage("leggings/cover/1_1_l.png");
//    m_rigthFrontMaskTexture = cache->addImage("leggings/cover/1_1_r.png");
//    
//    m_leftSideMaskTexture  = cache->addImage("leggings/cover/1_2_l.png");
//    m_rightSideMaskTexture = cache->addImage("leggings/cover/1_2_r.png");
//    
//    m_leftBackMaskTexture  = cache->addImage("leggings/cover/1_3_l.png");
//    m_rightBackMaskTexture = cache->addImage("leggings/cover/1_3_r.png");
    
    m_leftMaskTextureVec[LeggingOrientation_Front] = cache->addImage("leggings/cover/1_1_l.png");
    m_leftMaskTextureVec[LeggingOrientation_Side] = cache->addImage("leggings/cover/1_2_l.png");
    m_leftMaskTextureVec[LeggingOrientation_Back] = cache->addImage("leggings/cover/1_3_l.png");
    
    m_rightMaskTextureVec[LeggingOrientation_Front] = cache->addImage("leggings/cover/1_1_r.png");
    m_rightMaskTextureVec[LeggingOrientation_Side] = cache->addImage("leggings/cover/1_2_r.png");
    m_rightMaskTextureVec[LeggingOrientation_Back] = cache->addImage("leggings/cover/1_3_r.png");
}

/**
// a selector callback
void ArrangeScene::menuItemCallback(cocos2d::Ref* pSender){
    Node* node = static_cast<Node*>(pSender);
    
    switch (node->getTag()) {
        case MENUITEM_TAG_CANCEL:
            Director::getInstance()->popScene();
            break;
            
        case MENUITEM_TAG_ARRANGE1:
        {
            this->setArrange(1);
            break;
        }
        case MENUITEM_TAG_SCALE:
        {
            this->setScale(!m_isScale);
        }
            break;
            
        case MENUITEM_TAG_FRONT:
        {
            this->setLeggingStyle(LeggingType_9Cutting, "black", LeggingOrientation_Front);
        }
            break;
            
        case MENUITEM_TAG_SIDE:
        {
            this->setLeggingStyle(LeggingType_9Cutting, "black", LeggingOrientation_Side);
        }
            break;
            
        case MENUITEM_TAG_BACK:
        {
            this->setLeggingStyle(LeggingType_9Cutting, "black", LeggingOrientation_Back);
        }
            break;
            
        default:
            break;
    }
}
*/

void ArrangeScene::resetPattern(){
    
}

void ArrangeScene::setTexture(cocos2d::Texture2D* texture){
    if(m_texture == texture)
        return;
    
    CC_SAFE_RELEASE(m_texture);
    m_texture = texture;
    m_texture->retain();
}

void ArrangeScene::setSelectedItemTexture(cocos2d::Texture2D* texture){
    if(m_selectedArrangeBtn == NULL && m_selectedArrangeItem == NULL)
        return;
    
    if(m_selectedArrangeItem == NULL){
        ArrangeItem* item = this->generateArrangeItem(m_selectedArrangeBtn->getArrangeData());
        this->selectItem(item);
    }
    
    m_selectedArrangeItem->setTexture(texture);
}

void ArrangeScene::setSelectedItemPattern(int patternId){
    char imageFile[128];
    sprintf(imageFile, "pattern/pattern_%d.png", patternId);
    
    Texture2D* texture  = Director::getInstance()->getTextureCache()->addImage(imageFile);
    
    this->setSelectedItemTexture(texture);
}

cocos2d::Texture2D* ArrangeScene::getTexture(){
    return m_texture;
}

void ArrangeScene::setLeggingStyle(const LeggingType& type, const char* color, const LeggingOrientation& orientation){
    
    if(m_pantType == type && m_pantColor == std::string(color) && m_pantOrientation == orientation)
        return;
    
    if(type != LeggingType_None){
        m_pantType = type;
        m_isLeggingTypeDirty = true;
    }
    
    if(color)
        m_pantColor = std::string(color);
    
    if(orientation != LeggingOrientation_None)
        m_pantOrientation = orientation;
    
    m_isLeggingStyleDirty = true;
}

void ArrangeScene::setScale(bool isScale){
    if(isScale){
        m_image->setScale(0.5f);
    }else{
        m_image->setScale(1.0f);
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();
    //居中放置
    m_image->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
    
    m_isScale = isScale;
}

void ArrangeScene::recoverSelectedItem(){
    if(!m_selectedArrangeItem)
        return;
    
    if(m_selectedArrangeItem->hasTrack()){
        //恢复原状
        m_selectedArrangeItem->recoverFromTrack();
    }else{
        //删除该item
        this->deleteArrangeItem(m_selectedArrangeItem);
        m_selectedArrangeItem = NULL;
        
        if(m_onArrangeItemSelectStateCallback)
            m_onArrangeItemSelectStateCallback(false);
    }
}

cocos2d::Texture2D* ArrangeScene::getSelectedItemTexture(){
    if(m_selectedArrangeItem){
        return m_selectedArrangeItem->getTexture();
    }
    
    return NULL;
}

std::vector<ArrangeData> ArrangeScene::getArrangeInfo(){
    std::vector<ArrangeData> ret;
    for(ArrangeItem* arrangeItem : m_arrangeItemVec){
        ArrangeData data;
        arrangeItem->generateArrangeData(data);
        
        ret.push_back(data);
    }
    
    return ret;
}

void ArrangeScene::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags){
    if(m_isLeggingTypeDirty){
        this->updateLeggingType();
        m_isLeggingTypeDirty = false;
    }
    
    if(m_isLeggingStyleDirty){
        this->updateLeggingStyle();
        m_isLeggingStyleDirty = false;
    }
    
    Layer::visit(renderer, parentTransform, parentFlags);
}

void ArrangeScene::onArrangeBtnClicked(ArrangeBtn* btn){
    ArrangeData* data = btn->getArrangeData();
    bool isFindItem = false;
    for(ArrangeItem* item : m_arrangeItemVec){
        if(data == item->getArrangeData()){
            isFindItem = true;
            break;
        }
    }
    
    if(isFindItem)
        return;
    
//    ArrangeItem* item   = this->generateArrangeItem(data);
//    
//    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage("pattern/pattern_1.png");
//    item->setTexture(texture);
    
    m_selectedArrangeBtn = btn;
    this->selectItem(NULL);
    
    if(m_onArrangeBtnClickedCallback)
        m_onArrangeBtnClickedCallback();
}

void ArrangeScene::onArrangeItemClicked(ArrangeItem* item){
    if(m_selectedArrangeItem != item){
        this->selectItem(item);
    }else{
        if(m_onArrangeItemClickedCallback)
            m_onArrangeItemClickedCallback();
    }
}

void ArrangeScene::onSpin(float angle){
    if(fabs(angle) > MyComplexTouchEventListener_Angle_Noise){
        if(m_selectedArrangeItem)
            m_selectedArrangeItem->setRotation(m_selectedArrangeItem->getRotation() + CC_RADIANS_TO_DEGREES(angle));
    }
    CCLOG("%f", angle);
}

void ArrangeScene::onStretch(float preDistance, float curDistance){
    if(fabs(curDistance - preDistance) > MyComplexTouchEventListener_Distance_Noise){
        if(m_selectedArrangeItem)
            m_selectedArrangeItem->setScale(m_selectedArrangeItem->getScale() * curDistance / preDistance);
    }
    CCLOG("%f %f", preDistance, curDistance);
}

void ArrangeScene::onArrangeItemTouchesFinished(cocos2d::Event* event, MyComplexTouchEventListener* listener){
    MyComplexTouchEventListener::EventState curState = listener->getCurEventState();
    if(curState == MyComplexTouchEventListener::EventState_None || curState == MyComplexTouchEventListener::EventState_Click)
        return;
    
    ArrangeItem* item = NULL;
    if(curState == MyComplexTouchEventListener::EventState_Move){
        item = (ArrangeItem*)event->getCurrentTarget();
    }else{
        item = m_selectedArrangeItem;
    }
    
    if(item){
        item->recordTrack();
    }
}

void ArrangeScene::updateLeggingType(){
    this->initConfig();
    Size pantNodeSize = m_arrangeConfig.pantNodeSize;
    
    m_leftPatternContainerNode->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->getStencil()->setContentSize(pantNodeSize);
    m_leftPatternSliceNode->setContentSize(pantNodeSize);
    
    m_rightPatternContainerNode->setContentSize(pantNodeSize);
    m_rightPatternContainerNode->getStencil()->setContentSize(pantNodeSize);
    m_rightPatternSliceNode->setContentSize(pantNodeSize);
    m_rightPatternSliceNodeCopy->setContentSize(pantNodeSize);
    
    this->generateArrangeBtn();
}

void ArrangeScene::updateLeggingStyle(){
    
    const LeggingType& type = m_pantType;
    const char* color       = m_pantColor.c_str();
    const LeggingOrientation& orientation = m_pantOrientation;
    Size pantNodeSize       = m_arrangeConfig.pantNodeSize;
    
    //更新btn的valid属性
    for(ArrangeBtn* btn : m_arrangeBtnVec){
        btn->setIsValid(orientation);
    }
    
    this->loadTexture(m_pantType, m_pantColor.c_str(), m_pantOrientation);
    
    std::string frameName = LeggingHelper::getInstance()->convertLeggingInfoToString(type, color, true, orientation);
    frameName.append(".png");
    
    m_image->setSpriteFrame(frameName);
    m_rightPatternSliceNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    m_rightPatternSliceNode->setRotation(0);
    m_rightPatternSliceNode->setPosition(Vec2::ZERO);
    m_rightPatternSliceNodeCopy->setVisible(false);
    
    Node* stencil = m_leftPatternContainerNode->getStencil();
    stencil->removeAllChildren();
    Sprite* cover = Sprite::createWithTexture(m_leftMaskTextureVec[orientation]);
    cover->setPosition(Vec2(stencil->getContentSize().width / 2, stencil->getContentSize().height / 2));
    stencil->addChild(cover);
    
    stencil = m_rightPatternContainerNode->getStencil();
    stencil->removeAllChildren();
    cover = Sprite::createWithTexture(m_rightMaskTextureVec[orientation]);
    cover->setPosition(Vec2(stencil->getContentSize().width / 2, stencil->getContentSize().height / 2));
    stencil->addChild(cover);
    
    //pattern视图的摆位
    if(orientation == LeggingOrientation_Front){
        //正面左边是右脚，右边是左脚
        //right pattern
        Vec2 position(200, 0);
        m_rightPatternContainerNode->setPosition(position);
        
        //left pattern
        position.x += pantNodeSize.width;
        m_leftPatternContainerNode->setPosition(position);
        
    }else if(orientation == LeggingOrientation_Side){
        //侧面 里面是右脚，外面是左脚
        //rigth pattern
        Vec2 position(433, 0);
        m_rightPatternContainerNode->setPosition(position);
        m_rightPatternSliceNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        m_rightPatternSliceNode->setRotation(-30);
        m_rightPatternSliceNode->setPosition(Vec2(100, pantNodeSize.height / 2 + 20));
        m_rightPatternSliceNodeCopy->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        m_rightPatternSliceNodeCopy->setRotation(-30);
        m_rightPatternSliceNodeCopy->setPosition(Vec2(100, pantNodeSize.height / 2 + 20));
        m_rightPatternSliceNodeCopy->setVisible(true);
        
        //left pattern
        position.x = 390;
        m_leftPatternContainerNode->setPosition(position);
        
    }else if(orientation == LeggingOrientation_Back){
        //背面 左边是左脚，右边是右脚
        //left pattern
        Vec2 position(200, 0);
        m_leftPatternContainerNode->setPosition(position);
        
        //right pattern
        position = Vec2(640, 0);
        m_rightPatternContainerNode->setPosition(position);
    }
}

void ArrangeScene::updateArrangeBtnState(){
    for(ArrangeBtn* btn : m_arrangeBtnVec){
        ArrangeData* data = btn->getArrangeData();
        btn->setVisible(true);
        
        for(ArrangeItem* item : m_arrangeItemVec){
            if(data == item->getArrangeData()){
                btn->setVisible(false);
                break;
            }
        }
    }
}

void ArrangeScene::loadTexture(const LeggingType& type, const char* color/* = "black"*/, const LeggingOrientation& orientation/* = LeggingOrientation_Front*/){
    
    char plistFile[128], textureFile[128];
    std::string s = LeggingHelper::getInstance()->convertLeggingInfoToString(type, color, false);
    
    sprintf(plistFile, "leggings/%s.plist", s.c_str());
    sprintf(textureFile, "leggings/%s.png", s.c_str());
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile, textureFile);
}

void ArrangeScene::fixImageMovePosition(cocos2d::Vec2& position){
    Rect rect        = Rect::ZERO;
    Size contentSize = m_image->getContentSize();
    
    rect.origin = position - Vec2(contentSize.width / 2, contentSize.height / 2);
    rect.size = contentSize;
    
    if(rect.getMinX() > m_area.getMinX())
        rect.origin.x = m_area.origin.x;
    
    if(rect.getMinY() > m_area.getMinY())
        rect.origin.y = m_area.origin.y;
    
    if(rect.getMaxX() < m_area.getMaxX())
        rect.origin.x = m_area.getMaxX() - contentSize.width;
    
    if(rect.getMaxY() < m_area.getMaxY())
        rect.origin.y = m_area.getMaxY() - contentSize.height;
    
    position.x = rect.getMidX();
    position.y = rect.getMidY();
}

void ArrangeScene::parseArrangeJson(){
    m_arrangeDataVec.clear();
    
    std::string fullPath    = FileUtils::getInstance()->fullPathForFilename(ARRANGE_CONFIG_FILE);
    std::string contentStr  = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document doc;
    doc.Parse<0>(contentStr.c_str());
    if (doc.HasParseError())
    {
        CCASSERT(false, doc.GetParseError());
    }
    
    const rapidjson::Value& arrangesJson = doc["arranges"];
    int arrangeSize = arrangesJson.Size();
    
    for(int i = 0; i < arrangeSize; i++){
        const rapidjson::Value& arrangeJson = arrangesJson[i];
        
        //left
        const rapidjson::Value& leftItemsJson = arrangeJson["left_items"];
        
        for(int j = 0; j < leftItemsJson.Size(); j++){
            const rapidjson::Value& itemJson = leftItemsJson[j];
            ArrangeData arrangeData(itemJson);
            
            m_arrangeDataVec.push_back(arrangeData);
        }
        
        const rapidjson::Value& itemsJson = arrangeJson["right_items"];
        
        for(int j = 0; j < itemsJson.Size(); j++){
            const rapidjson::Value& itemJson = itemsJson[j];
            ArrangeData arrangeData(itemJson);
            
            m_arrangeDataVec.push_back(arrangeData);
        }
    }
}

void ArrangeScene::generateArrangeBtn(){
    for(ArrangeBtn* node : m_arrangeBtnVec){
        node->removeFromParent();
    }
    m_arrangeBtnVec.clear();
    
    for(ArrangeData& data : m_arrangeDataVec){
        this->generateArrangeBtn(&data);
    }
}

ArrangeItem* ArrangeScene::generateArrangeItem(ArrangeData* data){
    ArrangeItem* item = ArrangeItem::createWithData(m_arrangeConfig.ratio, m_arrangeConfig.pantNodeOriginOffset);
    item->setInfoFromArrangeData(data);
    item->setClickedCallback(CC_CALLBACK_1(ArrangeScene::onArrangeItemClicked, this));
    item->setSpinCallback(CC_CALLBACK_1(ArrangeScene::onSpin, this));
    item->setStretchCallback(CC_CALLBACK_2(ArrangeScene::onStretch, this));
    item->setTouchesFinishedCallback(CC_CALLBACK_2(ArrangeScene::onArrangeItemTouchesFinished, this));
    
    Node* parent = NULL;
    if(item->isLeft())
        parent = m_leftPatternSliceNode;
    else
        parent = m_rightPatternSliceNode;
    
    item->setMoveArea(parent->getBoundingBox());
    item->recordTrack();
    
    parent->addChild(item);
    m_arrangeItemVec.push_back(item);
    
    if(!item->isLeft()){
        ArrangeItem* copyItem = ArrangeItem::createWithItem(item);
        item->setCopyItem(copyItem);
        copyItem->setEnableTouch(false);
        m_rightPatternSliceNodeCopy->addChild(copyItem);
    }
    
    this->updateArrangeBtnState();
    
    return item;
}

ArrangeBtn* ArrangeScene::generateArrangeBtn(ArrangeData* data){
    ArrangeBtn* btn = ArrangeBtn::createWithData(m_arrangeConfig.ratio, m_arrangeConfig.pantNodeOriginOffset);
    btn->setInfoFromArrangeData(data);
    btn->setClickedCallback(CC_CALLBACK_1(ArrangeScene::onArrangeBtnClicked, this));
    btn->setSpinCallback(CC_CALLBACK_1(ArrangeScene::onSpin, this));
    btn->setStretchCallback(CC_CALLBACK_2(ArrangeScene::onStretch, this));
    
    if(btn->isLeft())
        m_leftPatternSliceNode->addChild(btn, ARRANGE_SCENE_BTN_ZORDER);
    else
        m_rightPatternSliceNode->addChild(btn, ARRANGE_SCENE_BTN_ZORDER);
    
    m_arrangeBtnVec.push_back(btn);
    
    return btn;
}

void ArrangeScene::deleteArrangeItem(ArrangeItem* item){
    if(!item)
        return;
    
    item->removeFromParent();
    ArrangeItem* copy = item->getCopyItem();
    if(copy)
        copy->removeFromParent();
    
    for(std::vector<ArrangeItem*>::iterator it = m_arrangeItemVec.begin(); it != m_arrangeItemVec.end(); it++){
        if(*it == item){
            m_arrangeItemVec.erase(it);
            break;
        }
    }
    
    this->updateArrangeBtnState();
}

void ArrangeScene::selectItem(ArrangeItem* item){
    if(m_selectedArrangeItem == item)
        return;
    
    if(m_selectedArrangeItem)
        m_selectedArrangeItem->unSelect();
    
    m_selectedArrangeItem = item;
    
    if(m_selectedArrangeItem)
        m_selectedArrangeItem->select();
    
    if(m_onArrangeItemSelectStateCallback){
        m_onArrangeItemSelectStateCallback(m_selectedArrangeItem != NULL);
    }
}