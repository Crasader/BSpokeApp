//
//  ArrangeItem.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/28.
//
//

#include "ArrangeItem.h"
#include "CocosUtil.h"
//#include "MyTouchEventListener.h"
#include "MyComplexTouchEventListener.h"
#include "ui/UIScale9Sprite.h"

USING_NS_CC;
using namespace pp;
using namespace cocos2d::ui;

#define ARRANGE_CONFIG_FILE "config/arrange.json"
#define PANTSIZE_CONFIG_FILE "config/pantSize.json"

/////////////////////   ArrangeData   /////////////////////////

ArrangeData::ArrangeData(){
    size = Size::ZERO;
    position = Vec2::ZERO;
    rotate = 0.f;
    validOrientation = "0";
    isLeft = false;
    texture = NULL;
    scale = 1.0f;
}

ArrangeData::ArrangeData(const rapidjson::Value& json){
    CocosUtil::stringToSize(json["size"].GetString(), this->size);
    CocosUtil::stringToVec2(json["position"].GetString(), this->position);
    this->rotate = json["rotate"].GetDouble();
    this->validOrientation = json["orietation"].GetString();
    this->isLeft = json["isLeft"].GetBool();
    this->texture = NULL;
    this->scale = 1.0f;
}

ArrangeData::ArrangeData(const ArrangeData* copy){
    size = copy->size;
    position = copy->position;
    rotate = copy->rotate;
    validOrientation = copy->validOrientation;
    isLeft = copy->isLeft;
    texture = copy->texture;
    scale = copy->scale;
}

/////////////////////   ArrangeConfig   /////////////////////////

ArrangeConfig::ArrangeConfig(){
    ratio = 0.f;
    pantNodeSize = Size::ZERO;
    pantNodeOriginOffset = Vec2::ZERO;
}

ArrangeConfig::ArrangeConfig(const LeggingType& leggingType){
    //真实切片数据与视图大小比例
    std::string fullPath    = FileUtils::getInstance()->fullPathForFilename(ARRANGE_CONFIG_FILE);
    std::string contentStr  = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document doc1;
    doc1.Parse<0>(contentStr.c_str());
    if (doc1.HasParseError())
    {
        CCASSERT(false, doc1.GetParseError());
    }
    
    ratio = doc1["ratio"].GetDouble();
    
    //裤形大小
    fullPath    = FileUtils::getInstance()->fullPathForFilename(PANTSIZE_CONFIG_FILE);
    contentStr  = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    rapidjson::Document doc2;
    doc2.Parse<0>(contentStr.c_str());
    if (doc2.HasParseError())
    {
        CCASSERT(false, doc2.GetParseError());
    }
    
    const rapidjson::Value& json = doc2["data"];
    for(int i = 0; i < json.Size(); i++){
        const rapidjson::Value& subJson = json[i];
        if(subJson["type"].GetInt() == leggingType){
            Size pantSize;
            Vec2 originOffset;
            
            CocosUtil::stringToSize(subJson["size"].GetString(), pantSize);
            CocosUtil::stringToVec2(subJson["originOffset"].GetString(), originOffset);
            
            pantNodeSize = pantSize * ratio;
            pantNodeOriginOffset = originOffset * ratio;
            //因为是以左上角为原点，换成cocos2dx坐标系
            pantNodeOriginOffset.y = pantNodeSize.height - pantNodeOriginOffset.y;
            break;
        }
    }
}

ArrangeConfig::ArrangeConfig(const ArrangeConfig* config){
    ratio        = config->ratio;
    pantNodeSize = config->pantNodeSize;
    pantNodeOriginOffset = config->pantNodeOriginOffset;
}

/////////////////////   ArrangeItem   /////////////////////////

ArrangeItem::ArrangeItem():
m_texture(NULL),
m_clickedCallback(NULL),
m_spinCallback(NULL),
m_stretchCallback(NULL),
m_touchesFinishedCallback(NULL),
m_copyItem(NULL)
{
    
}

ArrangeItem::~ArrangeItem(){
    CC_SAFE_RELEASE(m_texture);
    CC_SAFE_RELEASE(m_copyItem);
}

ArrangeItem* ArrangeItem::createWithItem(const ArrangeItem* copy){
    ArrangeItem* ret = ArrangeItem::createWithData(copy->m_ratio, copy->m_originVec);
    ret->setInfoFromArrangeData(copy->m_data);
    return ret;
}

ArrangeItem* ArrangeItem::createWithData(float ratio, const cocos2d::Vec2& origin){
    ArrangeItem* ret = new ArrangeItem();
    if(ret && ret->initWithData(ratio, origin)){
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool ArrangeItem::initWithData(float ratio, const cocos2d::Vec2& origin){
    if(!Node::init())
        return false;
    
    m_area = Rect::ZERO;
    m_clickedCallback = NULL;
    m_content = NULL;
    m_ratio = ratio;
    m_originVec = origin;
    m_isLeft = true;
    
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    m_selectNode = Scale9Sprite::create("editbox.png", Rect(0, 0, 20, 20), Rect(9, 9, 2, 2));
    m_selectNode->setContentSize(_contentSize);
    m_selectNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
    m_selectNode->setVisible(false);
    this->addChild(m_selectNode);
    
    this->setEnableTouch(true);
    
    return true;
}

void ArrangeItem::select(){
    m_selectNode->setVisible(true);
}

void ArrangeItem::unSelect(){
    m_selectNode->setVisible(false);
}

void ArrangeItem::setInfoFromArrangeData(ArrangeData* data){
    m_data   = data;
    m_isLeft = data->isLeft;
    
    this->setContentSize(data->size * m_ratio);
    this->setPosition(m_originVec + data->position * m_ratio);
    this->setRotation(data->rotate);
    this->setScale(data->scale);
    
    m_selectNode->setContentSize(_contentSize);
    m_selectNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
    
    if(data->texture)
        this->setTexture(data->texture);
}

void ArrangeItem::setTexture(cocos2d::Texture2D* texture){
    if(!m_content){
        m_content = Sprite::create();
        m_content->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
        this->addChild(m_content);
    }
    
    CC_SAFE_RELEASE(m_texture);
    m_texture = texture;
    CC_SAFE_RETAIN(m_texture);
    
    if(!texture)
        return;
    
    Rect rect = Rect::ZERO;
    rect.size = texture->getContentSizeInPixels();
    
    m_content->setTexture(texture);
    m_content->setTextureRect(rect);
    m_content->setScale(_contentSize.width / rect.size.width, _contentSize.height / rect.size.height);
    
    if(m_copyItem)
        m_copyItem->setTexture(texture);
}

void ArrangeItem::setEnableTouch(bool isEnable){
    _eventDispatcher->removeEventListenersForTarget(this);
    
    if(isEnable){
        MyComplexTouchEventListener* listener = MyComplexTouchEventListener::createWithTarget(this);
        listener->onClick = [=](){
            if(m_clickedCallback)
                m_clickedCallback(this);
        };
        listener->onDrag = [=](const Vec2& preTouchPosition, const Vec2& curTouchPosition){
            Vec2 offset = curTouchPosition - preTouchPosition;
            
            this->setPosition(this->getPosition() + offset);
        };
        listener->onSpin = [=](float angle){
            if(m_spinCallback)
                m_spinCallback(angle);
        };
        listener->onStretch = [=](float preDistance, float curDistance){
            if(m_stretchCallback)
                m_stretchCallback(preDistance, curDistance);
        };
        listener->onTouchesFinished = [=](Event* event, MyComplexTouchEventListener* listener){
            if(m_touchesFinishedCallback)
                m_touchesFinishedCallback(event, listener);
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
}

void ArrangeItem::setCopyItem(ArrangeItem* item){
    CC_SAFE_RELEASE(m_copyItem);
    m_copyItem = item;
    CC_SAFE_RETAIN(m_copyItem);
}

void ArrangeItem::setPosition(const cocos2d::Vec2& position){
    Vec2 fixedPosition = position;
    
    this->fixPosition(fixedPosition);
    
    Node::setPosition(fixedPosition);
    
    if(m_copyItem)
        m_copyItem->setPosition(fixedPosition);
}

void ArrangeItem::setRotation(float rotation){
    Node::setRotation(rotation);
    
    if(m_copyItem)
        m_copyItem->setRotation(rotation);
}

void ArrangeItem::setScale(float scale){
    Node::setScale(scale);
    
    if(m_copyItem)
        m_copyItem->setScale(scale);
}

void ArrangeItem::generateArrangeData(ArrangeData& target){
    target.isLeft = m_data->isLeft;
    target.rotate = _rotationZ_X;
    target.position = (_position - m_originVec) / m_ratio;
    target.size = _contentSize / m_ratio;
    target.texture = m_texture;
    target.scale = _scaleX;
}

void ArrangeItem::recordTrack(){
    ArrangeItemTrack track;
    track.scale  = _scaleX;
    track.rotate = _rotationZ_X;
    track.position = _position;
    
    m_trackList.push_back(track);
}

void ArrangeItem::recoverFromTrack(){
    if(!this->hasTrack())
        return;
    
    ArrangeItemTrack track = m_trackList.back();
    
    this->setPosition(track.position);
    this->setScale(track.scale);
    this->setRotation(track.rotate);
    
    m_trackList.pop_back();
}

void ArrangeItem::fixPosition(cocos2d::Vec2& position){
    if(m_area.equals(Rect::ZERO))
        return;
    
    Rect rect = Rect::ZERO;
    
    rect.origin = position - Vec2(_contentSize.width / 2, _contentSize.height / 2);
    rect.size = _contentSize;
    
    if(rect.getMinX() < m_area.getMinX())
        rect.origin.x = m_area.origin.x;
    
    if(rect.getMinY() < m_area.getMinY())
        rect.origin.y = m_area.origin.y;
    
    if(rect.getMaxX() > m_area.getMaxX())
        rect.origin.x = m_area.getMaxX() - _contentSize.width;
    
    if(rect.getMaxY() > m_area.getMaxY())
        rect.origin.y = m_area.getMaxY() - _contentSize.height;
    
    position.x = rect.getMidX();
    position.y = rect.getMidY();
}