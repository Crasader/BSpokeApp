//
//  TestUIView.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/7.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "TestUIView.h"

@implementation TestUIView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (id)initWithFrame:(CGRect)frame{
    self = [super initWithFrame:frame];
    if(self){
    
    }
    NSLog(@"log1");
    return self;
}

- (void)awakeFromNib{
    [super awakeFromNib];
    NSLog(@"log %@",  NSStringFromCGRect(self.frame));
}

- (void)didAddSubview:(UIView *)subview{
    [super didAddSubview:subview];
    NSLog(@"log2 %@",  NSStringFromCGRect(self.frame));
}

- (void)didMoveToSuperview{
    [super didMoveToSuperview];
    NSLog(@"log3 %@",  NSStringFromCGRect(self.frame));
}

- (void)didMoveToWindow{
    [super didMoveToWindow];
    NSLog(@"log4 %@",  NSStringFromCGRect(self.frame));
}

- (void)layoutSubviews{
    [super layoutSubviews];
    NSLog(@"log5 %@",  NSStringFromCGRect(self.frame));
}

- (void)layoutIfNeeded{
    [super layoutIfNeeded];
    NSLog(@"log6 %@",  NSStringFromCGRect(self.frame));
}

- (void)didChange:(NSKeyValueChange)changeKind valuesAtIndexes:(NSIndexSet *)indexes forKey:(NSString *)key{
    [super didChange:changeKind valuesAtIndexes:indexes forKey:key];
    NSLog(@"log7 %@",  NSStringFromCGRect(self.frame));
}

- (void) dealloc{
    [super dealloc];
}

@end
