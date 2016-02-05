//
//  LookbookNavigationController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/9.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LookbookNavigationController.h"

@interface LookbookNavigationController ()

@end

@implementation LookbookNavigationController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    _curLeggingType = LeggingType_None;
    _curEffectType  = 0;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
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
