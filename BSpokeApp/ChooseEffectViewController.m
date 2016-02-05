//
//  ChooseEffectViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/6/26.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LookbookNavigationController.h"
#import "ChooseEffectViewController.h"
#import "EffectViewController.h"

@interface ChooseEffectViewController ()

@property (nonatomic) NSArray* btnImages;
@property (retain, nonatomic) IBOutletCollection(UIButton) NSArray *effectBtns;
@end

@implementation ChooseEffectViewController{
    BOOL _isFromLegging;
}

#pragma mark - property

- (NSArray*)btnImages{
    return @[@"effect_1", @"effect_2", @"effect_3"];
}

#pragma mark - ViewController

- (IBAction)effectBtnClicked:(UIButton *)sender {
    NSUInteger tag = sender.tag;
    
    if(_isFromLegging){
        LeggingNavigationController* navigationController = (LeggingNavigationController*)self.navigationController;
        
        if(tag == 0){
            navigationController.curEffectType = 5;
        }
    }else{
        LookbookNavigationController* navigationController = (LookbookNavigationController*)self.navigationController;
        
        if(tag == 0){
            navigationController.curEffectType = 5;
        }
    }
    
    if(tag == 0)
        [self performSegueWithIdentifier:@"showEffect" sender:self];
}

- (IBAction)backBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)setup{
    _isFromLegging = YES;
    
    NSBundle* bundle = [NSBundle mainBundle];
    for(int i = 0; i < self.btnImages.count; i++){
        NSString* path = [bundle pathForResource:self.btnImages[i]
                         ofType:@"jpg"
                    inDirectory:@"effect"];
        [self.effectBtns[i] setBackgroundImage:[UIImage imageWithContentsOfFile:path]
                            forState:UIControlStateNormal];
        [self.effectBtns[i] setTag:i];
    }
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"效果选择";
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setup];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    [self setupNavigationBar];
    
    if([self.navigationController isKindOfClass:[LeggingNavigationController class]]){
        _isFromLegging = YES;
    }else{
        _isFromLegging = NO;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_effectBtns release];
    [super dealloc];
}
@end
