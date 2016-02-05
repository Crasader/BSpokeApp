//
//  MyUITabBar.h
//  UITabBarDemo
//
//  Created by luozhipeng on 15/5/15.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MyUITabBar;
@class MyUITabBarItem;
@protocol MyUITabBarDelegate;

@interface MyUITabBarItem : UIView

@property (assign, nonatomic) UIView* selectedView;
@property (nonatomic) BOOL isSelect;

@end

@interface MyUITabBar : UIControl

@property (assign, nonatomic) id<MyUITabBarDelegate> delegate;

@property (assign, nonatomic) NSArray *items;
@property (assign, nonatomic) MyUITabBarItem *selectedItem;

@end

@protocol MyUITabBarDelegate<NSObject>
@optional

- (void)tabBar:(MyUITabBar *)tabBar didSelectItem:(MyUITabBarItem *)item; // called when a new view is selected by the user (but not programatically)

@end