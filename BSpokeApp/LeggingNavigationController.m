//
//  LeggingNavigationController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/6/25.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"

@interface LeggingNavigationController ()

@end

@implementation LeggingNavigationController

#pragma mark - Custom

#pragma mark - NavigationController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    _curLeggingType = LeggingType_None;
    _curEffectType  = 0;
    _curPatternId   = -1;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    NSLog(@"memeory warming ========================");
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
