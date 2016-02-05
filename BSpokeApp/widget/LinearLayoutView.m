//
//  LinearLayoutView.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/8.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LinearLayoutView.h"

@implementation LinearLayoutView{
    UIView* _noLayoutView;
}

-(void)layoutSubviews{
    [super layoutSubviews];
    
    NSUInteger viewCount = 0;
    for(UIView* view in self.subviews){
        if(view != _noLayoutView)
            viewCount++;
    }
    
    CGSize size = self.frame.size;
    GLfloat perX  = size.width / (viewCount + 1);
    GLfloat perY  = size.height / 2;            //middle
    
    NSUInteger index = 1;
    for(UIView* view in self.subviews){
        if(view == _noLayoutView)
            continue;
        
        view.center = CGPointMake(perX * index, perY);
        
        index++;
    }
}

-(void)addSubviewNoLayout:(UIView *)view{
    [super addSubview:view];
    
    _noLayoutView = view;
}

@end
