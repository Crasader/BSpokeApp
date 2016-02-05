//
//  Legging.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/6.
//
//  legging数据类

#ifndef __ShaderDemo__Legging__
#define __ShaderDemo__Legging__

#include <stdio.h>

enum LeggingType{
    LeggingType_None,
    LeggingType_9Cutting,
    LeggingType_7Cutting
};

enum LeggingOrientation{
    LeggingOrientation_None,
    LeggingOrientation_Front,
    LeggingOrientation_Side,
    LeggingOrientation_Back
};

enum LeggingSide{
    LeggingSide_None,
    LeggingSide_Left,
    LeggingSide_Right
};

#endif /* defined(__ShaderDemo__Legging__) */
