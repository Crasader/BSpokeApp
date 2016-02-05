//
//  LeggingNavigationController.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/6/25.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Legging.h"
#include "Effect.h"

@interface LeggingNavigationController : UINavigationController

@property enum LeggingType curLeggingType;
@property enum EffectType curEffectType;
//1开始
@property int curPatternId;
@property BOOL isChoosePattern;

@end
