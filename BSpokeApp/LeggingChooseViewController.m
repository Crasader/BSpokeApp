//
//  LeggingChooseViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/13.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#include "Legging.h"
#import "LeggingNavigationController.h"
#import "LeggingChooseViewController.h"
#import "LeggingInfoViewController.h"
#import "CocosView.h"

@interface LeggingChooseViewController ()

@property (retain, nonatomic) IBOutlet CocosView *calculateCocosView;
@property (retain, nonatomic) IBOutlet UIButton *sevenStyleBtn;
@property (retain, nonatomic) IBOutlet UIButton *nineStyleBtn;

@end

@implementation LeggingChooseViewController


- (IBAction)homeBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController dismissViewControllerAnimated:YES
                                            completion:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    
    LeggingNavigationController* navigationController = (LeggingNavigationController*)self.navigationController;
    if([segue.identifier isEqualToString:@"chooseSeven"]){
        navigationController.curLeggingType = LeggingType_7Cutting;
    }else if([segue.identifier isEqualToString:@"chooseNine"]){
        navigationController.curLeggingType = LeggingType_9Cutting;
    }
}

- (void)setup{
    
    NSString *path1 = [[NSBundle mainBundle]pathForResource:@"product_1_1" ofType:@"jpg" inDirectory:@"products"];
    [self.sevenStyleBtn setBackgroundImage:[UIImage imageWithContentsOfFile:path1]
                                  forState:UIControlStateNormal];
    //    self.sevenStyleBtn.contentMode = UIViewContentModeCenter;
    //    [self.sevenStyleBtn.imageView setImage:[UIImage imageWithContentsOfFile:path1]];
    //    [self.sevenStyleBtn.imageView setContentMode:UIViewContentModeCenter];
    NSString *path2 = [[NSBundle mainBundle]pathForResource:@"product_2_1" ofType:@"jpg" inDirectory:@"products"];
    [self.nineStyleBtn setBackgroundImage:[UIImage imageWithContentsOfFile:path2]
                                 forState:UIControlStateNormal];
    //    self.nineStyleBtn.contentMode = UIViewContentModeCenter;
    //    [self.nineStyleBtn.imageView setImage:[UIImage imageWithContentsOfFile:path2]];
    //    [self.nineStyleBtn.imageView setContentMode:UIViewContentModeCenter];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"新品有售";
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setup];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    [self setupNavigationBar];
}

- (void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    
    [self.calculateCocosView updateGlobalCocosViewSize];
}

- (void)dealloc {
    [_sevenStyleBtn release];
    [_nineStyleBtn release];
    [_calculateCocosView destroyEAGLView];
    [_calculateCocosView release];
    [super dealloc];
}
@end
