//
//  LeggingItemChooseViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/6/11.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingItemChooseViewController.h"

@interface LeggingItemChooseViewController ()
@property (retain, nonatomic) IBOutletCollection(UIButton) NSArray *buttons;
@end

@implementation LeggingItemChooseViewController

#pragma mark - ViewController

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
}

- (IBAction)preBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.6 green:0.48 blue:0.76 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor whiteColor];
    self.navigationItem.title = @"功能选择";
}

- (void)setup{
    for(int i = 1; i <= 3; i++){
        NSString* fileName = [NSString stringWithFormat:@"function_%d", i];
        NSString* filePath = [[NSBundle mainBundle] pathForResource:fileName ofType:@"jpg" inDirectory:@"leggingItem"];
        UIButton* btn = self.buttons[i - 1];
        
        [btn setBackgroundImage:[UIImage imageWithContentsOfFile:filePath]
             forState:UIControlStateNormal];
    }
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

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_buttons release];
    [super dealloc];
}
@end
