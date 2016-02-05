//
//  BottomCollectionViewCell.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/7.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "BottomCollectionViewCell.h"

@implementation BottomCollectionViewCell

-(void)prepareForReuse{
    self.imageView.image = nil;
    self.backgroundColor = [UIColor whiteColor];
}

- (void)dealloc {
    [_imageView release];
    [super dealloc];
}
@end
