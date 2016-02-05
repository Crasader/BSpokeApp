//
//  PageContentViewController.m
//  UIPageViewDemo
//
//  Created by luozhipeng on 15/5/13.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "MainPageContentViewController.h"

@interface MainPageContentViewController ()
@property (retain, nonatomic) IBOutlet UIImageView *viewImage;

@end

@implementation MainPageContentViewController

- (void)setup{
    self.viewImage.image = self.image;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setup];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)dealloc {
    [_viewImage release];
    [super dealloc];
}
@end
