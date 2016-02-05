//
//  EffectDataFactory.cpp
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/4.
//
//

#include "EffectDataFactory.h"
#include "DotMoveEffect.h"
#include "DotMoveEffectData.h"

static EffectDataFactory* s_effectDataFactory = NULL;

EffectDataFactory* getInstance(){
    if(!s_effectDataFactory){
        s_effectDataFactory = new EffectDataFactory();
    }
    
    return s_effectDataFactory;
}

void EffectDataFactory::convertToData(IEffect* e, BaseEffectData* to){
    if(e->getType() == EffectType_DotMoveEffect){
        DotMoveEffect* effect   = (DotMoveEffect*) e;
        DotMoveEffectData* data = (DotMoveEffectData*) to;
        
        CCASSERT(effect && data, "wrong param in EffectDataFactory");
        
        effect->convertToData(data);
    }
}

void EffectDataFactory::convertFromData(IEffect* e, BaseEffectData* from){
    
}