//
//  ShakeAction.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/10.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#ifndef __BSpokeApp__ShakeAction__
#define __BSpokeApp__ShakeAction__

#include "cocos2d.h"

namespace pp{
    class ShakeAction : public cocos2d::ActionInterval{
        
    public:
        static ShakeAction* create(float strength, float during);
        
    public:
        bool init(float strength, float during);
        virtual void update(float time);
        virtual void startWithTarget(cocos2d::Node *target) override;
        virtual void stop();
        
    protected:
        float m_strength;
        cocos2d::Vec2 m_originPosition;
    };
};



#endif /* defined(__BSpokeApp__ShakeAction__) */
