//
//  LeggingInfoSlideView.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/14.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingInfoSlideView.h"

@interface LeggingInfoSlideView()

@property (retain, nonatomic) IBOutlet UIButton *button;
@property (retain, nonatomic) IBOutlet UIImageView *imageView;

@end

@implementation LeggingInfoSlideView

- (IBAction)btnClick:(UIButton *)sender {
    if(_delegate){
        [_delegate onBtnClicked:sender WithSender:self];
    }
}
- (void)awakeFromNib{
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
}

- (void)setImage:(UIImage*)image{
    _image = image;
    self.imageView.image = self.image;
    
    NSLayoutConstraint *viewHeightConstraint = [NSLayoutConstraint constraintWithItem:self
                                                                            attribute:NSLayoutAttributeHeight
                                                                            relatedBy:NSLayoutRelationEqual
                                                                               toItem:nil
                                                                            attribute:NSLayoutAttributeNotAnAttribute
                                                                           multiplier:1.0
                                                                             constant:70 + self.image.size.height];
    [self addConstraint:viewHeightConstraint];
}

- (void)dealloc {
    [_button release];
    [_imageView release];
    
    [super dealloc];
}
@end
