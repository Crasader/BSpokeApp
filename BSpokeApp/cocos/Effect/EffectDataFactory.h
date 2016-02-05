//
//  EffectDataFactory.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/4.
//
//

#ifndef __ShaderDemo__EffectDataFactory__
#define __ShaderDemo__EffectDataFactory__

#include <stdio.h>
#include "IEffect.h"
#include "data/BaseEffectData.h"

class EffectDataFactory{

public:
    static EffectDataFactory* getInstance();
    
public:
    void convertToData(IEffect* e, BaseEffectData* to);
    void convertFromData(IEffect* e, BaseEffectData* from);
};

#endif /* defined(__ShaderDemo__EffectDataFactory__) */
