//
//  LeggingPreviewScene.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/7/8.
//
//

#ifndef __ShaderDemo__LeggingPreviewScene__
#define __ShaderDemo__LeggingPreviewScene__

#include "cocos2d.h"
#include "Legging.h"
#include "Arrange/ArrangeItem.h"

class LeggingPreviewScene : public cocos2d::Layer{
    
public:
    static LeggingPreviewScene* createWithData(const LeggingType& leggingType, const char* color, const std::vector<ArrangeData>& arrangeDataVec);
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithData(const LeggingType& leggingType, const char* color, const std::vector<ArrangeData>& arrangeDataVec);
    
    // a selector callback
//    void menuItemCallback(cocos2d::Ref* pSender);
    
public:
    void updateLeggingStyle(const LeggingType& leggingType, const char* color, const LeggingOrientation& orientation);

protected:
    void initMask();
    void initConfig();
    void updateArrangeItem(const std::vector<ArrangeData>& arrangeDataVec);
    ArrangeItem* generateArrangeItem(ArrangeData* data);
    
private:
    std::vector<cocos2d::Texture2D*> m_leftMaskTextureVec;
    std::vector<cocos2d::Texture2D*> m_rightMaskTextureVec;
    
    cocos2d::Sprite* m_image;
    
    cocos2d::ClippingNode* m_leftPatternContainerNode;
    cocos2d::ClippingNode* m_rightPatternContainerNode;
    
    cocos2d::Node* m_leftPatternSliceNode;  //左切片
    cocos2d::Node* m_rightPatternSliceNode; //右切片1
    cocos2d::Node* m_rightPatternSliceNodeCopy; //右切片2
    
private:
    std::vector<ArrangeData> m_arrangeDataVec;
    
private:
    LeggingType m_pantType;
    std::string m_pantColor;
    LeggingOrientation m_pantOrientation;
    
    ArrangeConfig m_arrangeConfig;
};

#endif /* defined(__ShaderDemo__LeggingPreviewScene__) */
