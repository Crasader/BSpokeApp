//
//  MySlideMenuController.h
//  MySlideMenuDemo
//
//  Created by luozhipeng on 15/5/11.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MySlideMenuController : UIViewController

@property (assign, nonatomic) UIView* menuView;

-(UIView*)loadMenuViewFromNib:(NSString*)nibName;
-(void)setBeginPoint:(CGPoint)beginPoint AndEndPoint:(CGPoint)endPoint;

-(void)openMenu;
-(void)closeMenu;
-(void)toggleMenu;
@end
