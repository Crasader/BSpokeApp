//
//  BottomCollectionViewLabelCell.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/21.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "BottomCollectionViewLabelCell.h"

@implementation BottomCollectionViewLabelCell

-(void)prepareForReuse{
    self.imageView.image = nil;
    [self.labelView setText:nil];
    self.backgroundColor = [UIColor whiteColor];
}

- (void)dealloc {
    [_imageView release];
    [_labelView release];
    [super dealloc];
}
@end
