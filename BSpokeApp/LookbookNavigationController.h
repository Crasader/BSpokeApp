//
//  LookbookNavigationController.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/9.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Legging.h"
#include "Effect.h"

@interface LookbookNavigationController : UINavigationController

@property enum LeggingType curLeggingType;
@property enum EffectType curEffectType;

@end
