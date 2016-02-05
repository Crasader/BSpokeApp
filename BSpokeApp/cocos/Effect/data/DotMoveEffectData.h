//
//  DotMoveEffectData.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/4.
//
//

#ifndef ShaderDemo_DotMoveEffectData_h
#define ShaderDemo_DotMoveEffectData_h

#include "cocos2d.h"
#include "BaseEffectData.h"

class DotMoveEffectData : public BaseEffectData{

public:
    DotMoveEffectData(){
        type = EffectDataType_DotMove;
    }
public:
    cocos2d::Vec2 coreCoord;
    cocos2d::Vec4 color;
};

#endif
