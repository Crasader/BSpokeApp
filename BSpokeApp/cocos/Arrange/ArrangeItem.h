//
//  ArrangeItem.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/28.
//
//

#ifndef __ShaderDemo__ArrangeItem__
#define __ShaderDemo__ArrangeItem__

#include "cocos2d.h"
#include "Legging.h"
#include "json/document.h"
#include "MyComplexTouchEventListener.h"

struct ArrangeData{
    ArrangeData();
    ArrangeData(const rapidjson::Value& json);
    ArrangeData(const ArrangeData* copy);
    cocos2d::Size size;
    float rotate;
    cocos2d::Vec2 position;
    std::string validOrientation;
    bool isLeft;
    cocos2d::Texture2D* texture;
    float scale;
};

struct ArrangeConfig{
    ArrangeConfig();
    ArrangeConfig(const LeggingType& leggingType);
    ArrangeConfig(const ArrangeConfig* config);
    cocos2d::Size pantNodeSize;
    cocos2d::Vec2 pantNodeOriginOffset;
    float ratio;
};

struct ArrangeItemTrack{
    float scale;
    float rotate;
    cocos2d::Vec2 position;
};

class ArrangeItem : public cocos2d::Node{
    
public:
    static ArrangeItem* createWithItem(const ArrangeItem* copy);
    static ArrangeItem* createWithData(float ratio, const cocos2d::Vec2& origin);
    bool initWithData(float ratio, const cocos2d::Vec2& origin);
    
public:
    void select();
    void unSelect();
    
    void setInfoFromArrangeData(ArrangeData* data);
    void setMoveArea(const cocos2d::Rect& area){ m_area = area; }
    void setTexture(cocos2d::Texture2D* texture);
    
    void setClickedCallback(const std::function<void(ArrangeItem*)>& func){ m_clickedCallback = func; }
    void setSpinCallback(const std::function<void(float)>& func){ m_spinCallback = func; }
    void setStretchCallback(const std::function<void(float, float)>& func){ m_stretchCallback = func; }
    void setTouchesFinishedCallback(const std::function<void(cocos2d::Event* event, MyComplexTouchEventListener* listener)>& func){ m_touchesFinishedCallback = func; }
    
    void setEnableTouch(bool isEnable);
    
    void setCopyItem(ArrangeItem* item);
    ArrangeItem* getCopyItem(){ return m_copyItem; }
    
    virtual void setPosition(const cocos2d::Vec2 &position);
    virtual void setRotation(float rotation);
    virtual void setScale(float scale);
    
public:
    ArrangeData* getArrangeData(){ return m_data; }
    void generateArrangeData(ArrangeData& target);          //用当前的状态生成新的ArrangeData;
    
    cocos2d::Texture2D* getTexture(){ return m_texture; }
    bool isLeft(){ return m_isLeft; }
    
public:
    void recordTrack();
    void recoverFromTrack();
    bool hasTrack(){ return m_trackList.size() > 0; }
    
protected:
    ArrangeItem();
    virtual ~ArrangeItem();
    
    void fixPosition(cocos2d::Vec2& position);
    
private:
    ArrangeData* m_data;
    
    //texture
    cocos2d::Texture2D* m_texture;
    
    //显示的texture的node
    cocos2d::Sprite* m_content;
    
    //选中的框
    cocos2d::Node* m_selectNode;
    
private:
    float m_ratio;
    cocos2d::Vec2 m_originVec;
    bool m_isLeft;
    
private:
    cocos2d::Rect m_area;
    
private:
    ArrangeItem* m_copyItem;    //绑定的另一个copy
    
private:
    std::function<void(ArrangeItem*)> m_clickedCallback;
    std::function<void(float)> m_spinCallback;
    std::function<void(float, float)> m_stretchCallback;
    std::function<void(cocos2d::Event* event, MyComplexTouchEventListener* listener)> m_touchesFinishedCallback;
    
private:
    std::list<ArrangeItemTrack> m_trackList;
};

#endif /* defined(__ShaderDemo__ArrangeItem__) */
