//
//  LeggingInfoSlideView.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/14.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>

@class LeggingInfoSlideView;

@protocol LeggingInfoSlideViewDelegate <NSObject>

- (void)onBtnClicked:(UIButton*)btn WithSender:(LeggingInfoSlideView*)sender;

@end

@interface LeggingInfoSlideView : UIView

@property (assign, nonatomic) UIImage *image;
@property (assign, nonatomic) id<LeggingInfoSlideViewDelegate> delegate;

@end
