//
//  LeggingPreviewScene.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/8.
//
//

#include "LeggingPreviewScene.h"
#include "LeggingHelper.h"

USING_NS_CC;

//#define MENUITEM_TAG_FRONT 1
//#define MENUITEM_TAG_SIDE 2
//#define MENUITEM_TAG_BACK 3

LeggingPreviewScene* LeggingPreviewScene::createWithData(const LeggingType& leggingType, const char* color, const std::vector<ArrangeData>& arrangeDataVec){
    LeggingPreviewScene* scene = new LeggingPreviewScene();
    
    if(scene && scene->initWithData(leggingType, color, arrangeDataVec)){
        scene->autorelease();
        return scene;
    }
    
    CC_SAFE_DELETE(scene);
    return NULL;
}

bool LeggingPreviewScene::initWithData(const LeggingType& leggingType, const char* leggingColor, const std::vector<ArrangeData>& arrangeDataVec){
    if(!Layer::init())
        return false;
    
    m_leftMaskTextureVec.resize(4);
    m_rightMaskTextureVec.resize(4);
    
    m_pantType = leggingType;
    
    this->initMask();
    this->initConfig();
    
    Size pantNodeSize = m_arrangeConfig.pantNodeSize;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin      = Director::getInstance()->getVisibleOrigin();
    
    LayerColor* colorLayer = LayerColor::create(Color4B(240, 245, 248, 255));
    this->addChild(colorLayer);
    
    m_image = Sprite::create();
    m_image->setScale(0.5f);
    m_image->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(m_image);
    
    m_leftPatternContainerNode = ClippingNode::create();
    m_leftPatternContainerNode->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->setAlphaThreshold(0.1f);
    m_leftPatternContainerNode->setInverted(false);
    
    Node* stencil = Node::create();
    stencil->setContentSize(pantNodeSize);
    m_leftPatternContainerNode->setStencil(stencil);
    
    //右脚pattern视图。
    m_rightPatternContainerNode = ClippingNode::create();
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
    
    this->updateLeggingStyle(leggingType, leggingColor, LeggingOrientation_Back);
    this->updateArrangeItem(arrangeDataVec);
    
    /**
    Color3B color = Color3B::BLACK;
    float y = 0;
    auto frontBtn = MenuItemFont::create("front", CC_CALLBACK_1(LeggingPreviewScene::menuItemCallback, this));
    y = frontBtn->getContentSize().height;
    frontBtn->setColor(color);
    frontBtn->setPosition(origin + Vec2(frontBtn->getContentSize().width, y));
    frontBtn->setTag(MENUITEM_TAG_FRONT);
    
    auto sideBtn = MenuItemFont::create("side", CC_CALLBACK_1(LeggingPreviewScene::menuItemCallback, this));
    y += sideBtn->getContentSize().height;
    sideBtn->setColor(color);
    sideBtn->setPosition(origin + Vec2(sideBtn->getContentSize().width, y));
    sideBtn->setTag(MENUITEM_TAG_SIDE);
    
    auto backBtn = MenuItemFont::create("back", CC_CALLBACK_1(LeggingPreviewScene::menuItemCallback, this));
    y += backBtn->getContentSize().height;
    backBtn->setColor(color);
    backBtn->setPosition(origin + Vec2(backBtn->getContentSize().width, y));
    backBtn->setTag(MENUITEM_TAG_BACK);
    
    Menu* menu = Menu::create(frontBtn, sideBtn, backBtn, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
     */
    
    return true;
}

/**
void LeggingPreviewScene::menuItemCallback(cocos2d::Ref* pSender){
    Node* node = static_cast<Node*>(pSender);
    
    switch (node->getTag()) {
            
        case MENUITEM_TAG_FRONT:
        {
            this->updateLeggingStyle(LeggingType_None, NULL, LeggingOrientation_Front);
        }
            break;
            
        case MENUITEM_TAG_SIDE:
        {
            this->updateLeggingStyle(LeggingType_None, NULL, LeggingOrientation_Side);
        }
            break;
            
        case MENUITEM_TAG_BACK:
        {
            this->updateLeggingStyle(LeggingType_None, NULL, LeggingOrientation_Back);
        }
            break;
            
        default:
            break;
    }
}
*/

void LeggingPreviewScene::updateLeggingStyle(const LeggingType& leggingType, const char* color, const LeggingOrientation& orientation){
    
    if(m_pantType == leggingType && m_pantColor == std::string(color) && m_pantOrientation == orientation)
        return;
    
    if(leggingType != LeggingType_None)
        m_pantType = leggingType;
    
    if(color)
        m_pantColor = std::string(color);
    
    if(orientation != LeggingOrientation_None)
        m_pantOrientation = orientation;
    
    
    std::string frameName = LeggingHelper::getInstance()->convertLeggingInfoToString(m_pantType, m_pantColor.c_str(), true, m_pantOrientation);
    frameName.append(".png");
    
    m_image->setSpriteFrame(frameName);
    m_rightPatternSliceNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    m_rightPatternSliceNode->setRotation(0);
    m_rightPatternSliceNode->setPosition(Vec2::ZERO);
    m_rightPatternSliceNodeCopy->setVisible(false);
    
    Size pantNodeSize = m_arrangeConfig.pantNodeSize;
    
    Node* stencil = m_leftPatternContainerNode->getStencil();
    stencil->removeAllChildren();
    Sprite* cover = Sprite::createWithTexture(m_leftMaskTextureVec[m_pantOrientation]);
    cover->setPosition(Vec2(stencil->getContentSize().width / 2, stencil->getContentSize().height / 2));
    stencil->addChild(cover);
    
    stencil = m_rightPatternContainerNode->getStencil();
    stencil->removeAllChildren();
    cover = Sprite::createWithTexture(m_rightMaskTextureVec[m_pantOrientation]);
    cover->setPosition(Vec2(stencil->getContentSize().width / 2, stencil->getContentSize().height / 2));
    stencil->addChild(cover);
    
    //pattern视图的摆位
    if(m_pantOrientation == LeggingOrientation_Front){
        //正面左边是右脚，右边是左脚
        //right pattern
        Vec2 position(200, 0);
        m_rightPatternContainerNode->setPosition(position);
        
        //left pattern
        position.x += pantNodeSize.width;
        m_leftPatternContainerNode->setPosition(position);
        
    }else if(m_pantOrientation == LeggingOrientation_Side){
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
        
    }else if(m_pantOrientation == LeggingOrientation_Back){
        //背面 左边是左脚，右边是右脚
        //left pattern
        Vec2 position(200, 0);
        m_leftPatternContainerNode->setPosition(position);
        
        //right pattern
        position = Vec2(640, 0);
        m_rightPatternContainerNode->setPosition(position);
    }
}

void LeggingPreviewScene::initMask(){
    //init mask
    TextureCache* cache     = Director::getInstance()->getTextureCache();
    
    m_leftMaskTextureVec[LeggingOrientation_Front] = cache->addImage("leggings/cover/1_1_l.png");
    m_leftMaskTextureVec[LeggingOrientation_Side] = cache->addImage("leggings/cover/1_2_l.png");
    m_leftMaskTextureVec[LeggingOrientation_Back] = cache->addImage("leggings/cover/1_3_l.png");
    
    m_rightMaskTextureVec[LeggingOrientation_Front] = cache->addImage("leggings/cover/1_1_r.png");
    m_rightMaskTextureVec[LeggingOrientation_Side] = cache->addImage("leggings/cover/1_2_r.png");
    m_rightMaskTextureVec[LeggingOrientation_Back] = cache->addImage("leggings/cover/1_3_r.png");
}

void LeggingPreviewScene::initConfig(){
    m_arrangeConfig = ArrangeConfig(m_pantType);
}

void LeggingPreviewScene::updateArrangeItem(const std::vector<ArrangeData>& arrangeDataVec){
    m_arrangeDataVec = arrangeDataVec;
    m_leftPatternSliceNode->removeAllChildren();
    m_rightPatternSliceNode->removeAllChildren();
    m_rightPatternSliceNodeCopy->removeAllChildren();
    
    for(ArrangeData& arrangeData : m_arrangeDataVec){
        this->generateArrangeItem(&arrangeData);
    }
}

ArrangeItem* LeggingPreviewScene::generateArrangeItem(ArrangeData* data){
    ArrangeItem* item = ArrangeItem::createWithData(m_arrangeConfig.ratio, m_arrangeConfig.pantNodeOriginOffset);
    item->setInfoFromArrangeData(data);
    item->setEnableTouch(false);
    
    Node* parent = NULL;
    if(item->isLeft())
        parent = m_leftPatternSliceNode;
    else
        parent = m_rightPatternSliceNode;
    
    item->setMoveArea(parent->getBoundingBox());
    parent->addChild(item);
    
    if(!item->isLeft()){
        ArrangeItem* copyItem = ArrangeItem::createWithItem(item);
        item->setCopyItem(copyItem);
        copyItem->setEnableTouch(false);
        m_rightPatternSliceNodeCopy->addChild(copyItem);
    }
    
    return item;

}