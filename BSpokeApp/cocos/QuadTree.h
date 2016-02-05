//
//  QuadTree.h
//  ShaderDemo
//
//  Created by luozhipeng on 15/3/30.
//
//

#ifndef __ShaderDemo__QuadTree__
#define __ShaderDemo__QuadTree__

#include "cocos2d.h"

struct QuadTreeNode{
    cocos2d::Vec2 originCoord;
    cocos2d::Size size;

    QuadTreeNode* leftBottomChild;
    QuadTreeNode* leftTopChild;
    QuadTreeNode* rightTopChild;
    QuadTreeNode* rightBottomChild;
    QuadTreeNode* parent;
};

class QuadTree{

    
    
};

#endif /* defined(__ShaderDemo__QuadTree__) */
