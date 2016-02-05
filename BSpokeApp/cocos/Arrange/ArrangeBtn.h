//
//  ArrangeBtn.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/6/29.
//
//

#ifndef __ShaderDemo__ArrangeBtn__
#define __ShaderDemo__ArrangeBtn__

#include "cocos2d.h"
#include "ArrangeItem.h"

class ArrangeBtn : public cocos2d::Sprite{
    
public:
    static ArrangeBtn* createWithData(float ratio, cocos2d::Vec2& origin);
    bool initWithData(float ratio, cocos2d::Vec2& origin);
    
public:
    void setInfoFromArrangeData(ArrangeData* data);
    ArrangeData* getArrangeData(){ return m_data; }
    
    void setClickedCallback(const std::function<void(ArrangeBtn*)>& func){ m_clickedCallback = func; }
    void setSpinCallback(const std::function<void(float)>& func){ m_spinCallback = func; }
    void setStretchCallback(const std::function<void(float, float)>& func){ m_stretchCallback = func; }
    
public:
    void setIsValid(int orientation);
    bool isLeft(){ return m_isLeft; }
    
protected:
    ArrangeBtn();
    
private:
    ArrangeData* m_data;
    bool m_isLeft;
    std::string m_validOrientation;
    bool m_isValid;
    
    float m_ratio;
    cocos2d::Vec2 m_originVec;
    
    std::function<void(ArrangeBtn*)> m_clickedCallback;
    std::function<void(float)> m_spinCallback;
    std::function<void(float, float)> m_stretchCallback;
};

#endif /* defined(__ShaderDemo__ArrangeBtn__) */
