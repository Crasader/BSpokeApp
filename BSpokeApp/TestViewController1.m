//
//  TestViewController1.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/11.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "TestViewController1.h"

@interface TestViewController1 ()
@property (assign, nonatomic) UIImageView* imageView;
@end

@implementation TestViewController1

- (UIImageView*)imageView{
    if(_imageView == nil){
        _imageView = [[UIImageView alloc] init];
        _imageView.backgroundColor = [UIColor colorWithRed:1.0 green:0 blue:0 alpha:1.0];
        _imageView.contentMode = UIViewContentModeCenter;
    }
    return _imageView;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    CGRect rect = CGRectZero;
    rect.size = self.image.size;
    self.imageView.bounds = rect;
    
    rect.origin.y = 100;
    rect.origin.x = 10;
    self.imageView.frame = rect;
    [self.view addSubview:self.imageView];
    [self.imageView setImage:self.image];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_image release];
    [super dealloc];
}
@end
