//
//  CocosUtil.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/5/5.
//
//

#ifndef __ShaderDemo__CocosUtil__
#define __ShaderDemo__CocosUtil__

#include "cocos2d.h"
#include <stdio.h>

namespace pp{
    class CocosUtil{
        
    public:
        static void stringToSize(const std::string& str, cocos2d::Size& size);
        static void stringToVec2(const std::string& str, cocos2d::Vec2& vec);
        static void stringToVec2Array(const std::string& str, std::vector<cocos2d::Vec2>& vecArray);
        static void stringToIntArray(const std::string& str, std::vector<int>& intArray);
        static cocos2d::Texture2D* resizeTexture2D(cocos2d::Texture2D* oldTexture, const cocos2d::Size& newSize, bool isFlipped = false);
    };
}

#endif /* defined(__ShaderDemo__CocosUtil__) */
