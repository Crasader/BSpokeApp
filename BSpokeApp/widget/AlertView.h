//
//  AlertView.h
//  ModalSegueDemo
//
//  Created by luozhipeng on 15/6/7.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AlertView;
@protocol AlertViewDelegate <NSObject>

@required
- (void) onYesButtonClicked:(AlertView*)alertView;
- (void) onNoButtonClicked:(AlertView*)alertView;
@end

@interface AlertView : UIView

@property (assign, nonatomic) id<AlertViewDelegate> delegate;

@end
