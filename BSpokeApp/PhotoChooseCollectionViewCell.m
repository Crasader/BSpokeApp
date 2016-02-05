//
//  PhotoChooseCollectionViewCell.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/18.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "PhotoChooseCollectionViewCell.h"

@interface PhotoChooseCollectionViewCell()
@property (retain, nonatomic) IBOutlet UIImageView *imageView;

@end

@implementation PhotoChooseCollectionViewCell

-(void)awakeFromNib{
    NSLayoutConstraint *widthConstrant = [NSLayoutConstraint constraintWithItem:self.imageView
                                                                      attribute:NSLayoutAttributeWidth
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeWidth
                                                                     multiplier:1.0
                                                                       constant:0.0];
    NSLayoutConstraint *heightConstrant = [NSLayoutConstraint constraintWithItem:self.imageView
                                                                      attribute:NSLayoutAttributeHeight
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeHeight
                                                                     multiplier:1.0
                                                                       constant:0.0];
    
    [self addConstraints:@[widthConstrant, heightConstrant]];
}

- (void)setImage:(UIImage *)image{
    _image = image;
    self.imageView.image = _image;
}

- (void)dealloc {
    [_imageView release];
    [super dealloc];
}
@end
