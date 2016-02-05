//
//  BaseEffectData.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/25.
//
//

#ifndef __ShaderDemo__BaseEffectData__
#define __ShaderDemo__BaseEffectData__

enum EffectDataType{
    EffectDataType_DotMove,
    EffectDataType_LineMove,
    EffectDataType_Paint,
    EffectDataType_None
};

class BaseEffectData{
    
public:
    BaseEffectData(){
        type = EffectDataType_None;
    }
    
public:
    EffectDataType type;
};

#endif /* defined(__ShaderDemo__BaseEffectData__) */
