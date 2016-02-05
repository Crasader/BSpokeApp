//
//  BottomTabBarItemView.m
//  UITabBarDemo
//
//  Created by luozhipeng on 15/5/15.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "BottomTabBarItemView.h"

@interface BottomTabBarItemView()

@property (retain, nonatomic) UILabel* label;

@end

@implementation BottomTabBarItemView

-(instancetype)initWithTitle:(NSString*)title{
    self = [super init];
    
    if(self){
        //选中背景
//        UIView *selectBg = [[UIView alloc]init];
        UIImageView *selectBg = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"hover"]];
        [selectBg autorelease];
        
        selectBg.translatesAutoresizingMaskIntoConstraints = NO;
//        selectBg.backgroundColor = [UIColor blueColor];
        [self addSubview:selectBg];
        //铺满
//        NSLayoutConstraint* widthContraint = [NSLayoutConstraint constraintWithItem:selectBg
//                                                                          attribute:NSLayoutAttributeWidth
//                                                                          relatedBy:NSLayoutRelationEqual
//                                                                             toItem:self
//                                                                          attribute:NSLayoutAttributeWidth
//                                                                         multiplier:1.0
//                                                                           constant:-10];
//        NSLayoutConstraint* heightContraint = [NSLayoutConstraint constraintWithItem:selectBg
//                                                                           attribute:NSLayoutAttributeHeight
//                                                                           relatedBy:NSLayoutRelationEqual
//                                                                              toItem:self
//                                                                           attribute:NSLayoutAttributeHeight
//                                                                          multiplier:1.0
//                                                                            constant:0];
        
        //居中
        NSLayoutConstraint* hContraint = [NSLayoutConstraint constraintWithItem:selectBg
                                                                      attribute:NSLayoutAttributeCenterX
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeCenterX
                                                                     multiplier:1.0
                                                                       constant:0];
        NSLayoutConstraint* vContraint = [NSLayoutConstraint constraintWithItem:selectBg
                                                                      attribute:NSLayoutAttributeBottom
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeBottom
                                                                     multiplier:1.0
                                                                       constant:0];
        [self addConstraints:@[hContraint, vContraint]];
        
        //文字
        UILabel *label      = [[UILabel alloc]init];
        [label autorelease];
        
        label.translatesAutoresizingMaskIntoConstraints = NO;
        label.font          = [UIFont systemFontOfSize:17];
        label.textColor     = [UIColor whiteColor];
        label.text          = title;
        
        [label sizeToFit];
        [self addSubview:label];
        
        self.label = label;
        
        //居中
        hContraint = [NSLayoutConstraint constraintWithItem:label
                                                  attribute:NSLayoutAttributeCenterX
                                                  relatedBy:NSLayoutRelationEqual
                                                     toItem:self
                                                  attribute:NSLayoutAttributeCenterX
                                                 multiplier:1.0
                                                   constant:0];
        vContraint = [NSLayoutConstraint constraintWithItem:label
                                                  attribute:NSLayoutAttributeCenterY
                                                  relatedBy:NSLayoutRelationEqual
                                                     toItem:self
                                                  attribute:NSLayoutAttributeCenterY
                                                 multiplier:1.0
                                                   constant:0];
        [self addConstraints:@[hContraint, vContraint]];
        
        [self setSelectedView:selectBg];
    }
    
    return self;
}

-(void)setIsSelect:(BOOL)isSelect{
    [super setIsSelect:isSelect];
    
    if(isSelect){
        self.label.textColor = [UIColor whiteColor];
    }else{
        self.label.textColor = [UIColor whiteColor];
    }
}

-(void)dealloc{
    [_label release];
    [super dealloc];
}

@end
