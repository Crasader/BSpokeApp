//
//  ArrangeScene.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/4/1.
//
//

#ifndef __ShaderDemo__ArrangeScene__
#define __ShaderDemo__ArrangeScene__

#include "cocos2d.h"
#include "Legging/Legging.h"
#include "Arrange/ArrangeItem.h"
#include "Arrange/ArrangeBtn.h"
#include "MyComplexTouchEventListener.h"

class ArrangeScene : public cocos2d::Layer{
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void initConfig();
    void initMask();
    
    // a selector callback
//    void menuItemCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ArrangeScene);
    
public:
    void setOnTouchDelegate(const std::function<void()>& func){ m_onTouchCallback = func; }
    void setOnArrangeBtnClickedCallback(const std::function<void()>& func){ m_onArrangeBtnClickedCallback = func; }
    void setOnArrangeItemClickedCallback(const std::function<void()>& func){ m_onArrangeItemClickedCallback = func; }
    void setOnArrangeItemSelectStateCallback(const std::function<void(bool)>& func){ m_onArrangeItemSelectStateCallback = func; }
    
public:
//    void setEffectData();
    void resetPattern();
    void setTexture(cocos2d::Texture2D* texture);
    void setSelectedItemTexture(cocos2d::Texture2D* texture);
    void setSelectedItemPattern(int patternId);
    cocos2d::Texture2D* getTexture();
    void setLeggingStyle(const LeggingType& type, const char* color, const LeggingOrientation& orientation);
    void setScale(bool isScale);
    //恢复选中的item
    void recoverSelectedItem();
    
public:
    cocos2d::Texture2D* getPatternTexture(){
        return m_texture;
    }
    cocos2d::Texture2D* getSelectedItemTexture();
    
    std::vector<ArrangeData> getArrangeInfo();
    LeggingType getLeggingType(){ return m_pantType; }
    std::string getLeggingColor(){ return m_pantColor; }
    
public:
    void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    
    void onArrangeBtnClicked(ArrangeBtn* btn);
    void onArrangeItemClicked(ArrangeItem* item);
    void onSpin(float angle);
    void onStretch(float preDistance, float curDistance);
    void onArrangeItemTouchesFinished(cocos2d::Event* event, MyComplexTouchEventListener* listener);
    
protected:
    ArrangeScene();
    virtual ~ArrangeScene();
    
private:
    void updateLeggingType();
    void updateLeggingStyle();
    void updateArrangeBtnState();
    
    void loadTexture(const LeggingType& type, const char* color = "black", const LeggingOrientation& orientation = LeggingOrientation_Front);
    void fixImageMovePosition(cocos2d::Vec2& position);
    
private:
    void parseArrangeJson();
    
private:
    void generateArrangeBtn();
    ArrangeItem* generateArrangeItem(ArrangeData* data);
    ArrangeBtn* generateArrangeBtn(ArrangeData* data);
    
    void deleteArrangeItem(ArrangeItem* item);
    
    void selectItem(ArrangeItem* item);
    
private:
    cocos2d::Texture2D* m_texture;
    cocos2d::Texture2D* m_leftFrontMaskTexture;
    cocos2d::Texture2D* m_rigthFrontMaskTexture;
    cocos2d::Texture2D* m_leftSideMaskTexture;
    cocos2d::Texture2D* m_rightSideMaskTexture;
    cocos2d::Texture2D* m_leftBackMaskTexture;
    cocos2d::Texture2D* m_rightBackMaskTexture;
    
    std::vector<cocos2d::Texture2D*> m_leftMaskTextureVec;
    std::vector<cocos2d::Texture2D*> m_rightMaskTextureVec;
    
    cocos2d::Sprite* m_image;
    
    cocos2d::ClippingNode* m_leftPatternContainerNode;
    cocos2d::ClippingNode* m_rightPatternContainerNode;
//    cocos2d::Node* m_leftPatternContainerNode;
//    cocos2d::Node* m_rightPatternContainerNode;
    
    cocos2d::Node* m_leftPatternSliceNode;  //左切片
    cocos2d::Node* m_rightPatternSliceNode; //右切片1
    cocos2d::Node* m_rightPatternSliceNodeCopy; //右切片2
    
    std::vector<ArrangeBtn*> m_arrangeBtnVec;
    std::vector<ArrangeItem*> m_arrangeItemVec;
    
    ArrangeBtn* m_selectedArrangeBtn;
    ArrangeItem* m_selectedArrangeItem;
    
    ArrangeConfig m_arrangeConfig;
    
private:
    std::vector<ArrangeData> m_arrangeDataVec;
    
private:
    LeggingType m_pantType;
    std::string m_pantColor;
    LeggingOrientation m_pantOrientation;

    bool m_isScale;
    cocos2d::Vec2 m_touchBeginPoint;
    
    cocos2d::Rect m_area;
    
private:
    bool m_isLeggingTypeDirty;
    bool m_isLeggingStyleDirty;
    
private:
    std::function<void()> m_onTouchCallback;
    std::function<void()> m_onArrangeBtnClickedCallback;
    std::function<void()> m_onArrangeItemClickedCallback;
    std::function<void(bool)> m_onArrangeItemSelectStateCallback;
};

#endif /* defined(__ShaderDemo__ArrangeScene__) */
