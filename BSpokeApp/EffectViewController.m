//
//  EffectViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/3.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LookbookNavigationController.h"
#import "LeggingViewController.h"
#import "LookbookViewController.h"
#import "EffectViewController.h"
#import "LinearLayoutView.h"
#import "cocos/CocosView.h"
#import "MyUITabBar.h"

#define EFFECTSCENE_TABBAR_EFFECT 1
#define EFFECTSCENE_TABBAR_COLOR 2

enum TopLevelType{
    TopLevelType_Effect, TopLevelType_Color, TopLevelType_None
};

@interface EffectViewController (){
    BOOL _isFromLegging;
}

@property (retain, nonatomic) IBOutlet UIButton *pauseBtn;
@property (retain, nonatomic) IBOutlet CocosView *cocosView;
@property (retain, nonatomic) UIImageView* pauseTipImageView;
@end

@implementation EffectViewController{
    BOOL _isPauseEffect;
}

-(UIImageView*)pauseTipImageView{
    if(!_pauseTipImageView){
        _pauseTipImageView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"btn_stopshake.png"]];
        CGRect frame = _pauseTipImageView.frame;
        CGPoint point = self.pauseBtn.frame.origin;
        point.x -= 10;
        point.y -= 40;
        frame.origin = point;
        _pauseTipImageView.frame = frame;
        LinearLayoutView* superView = (LinearLayoutView*)self.pauseBtn.superview;
        [superView addSubviewNoLayout:_pauseTipImageView];
    }
    
    return _pauseTipImageView;
}

#pragma mark - custom
- (void)onEffectActionCallback:(id)userInfo{
    if([userInfo isKindOfClass:[NSDictionary class]]){
        NSDictionary* info   = (NSDictionary*)userInfo;
        NSNumber* isBegin = [info objectForKey:@"isBegin"];
        [self.pauseBtn setEnabled:isBegin.boolValue];
        
        _isPauseEffect = NO;
        [self switchPauseBtn];
        [self showOrHidePauseTip];
    }
}

- (void)switchPauseBtn{
    if(_isPauseEffect){
        [self.pauseBtn setImage:[UIImage imageNamed:@"btn_effect_play"]
                       forState:UIControlStateNormal];
    }else{
        [self.pauseBtn setImage:[UIImage imageNamed:@"btn_effect_pause"]
                       forState:UIControlStateNormal];
    }
}

- (void)showOrHidePauseTip{
    if(_isPauseEffect){
        [self.pauseTipImageView setHidden:YES];
    }else{
        [self.pauseTipImageView setHidden:NO];
    }
}

#pragma mark - UIViewController

- (IBAction)applyBtnClicked:(UIBarButtonItem *)sender {
    Class destinateClass = nil;
    
    if(_isFromLegging){
        destinateClass = [LeggingViewController class];
    }else{
        destinateClass = [LookBookViewController class];
    }
    
    for(UIViewController *temp in self.navigationController.viewControllers){
        if ([temp isKindOfClass:destinateClass]) {
            [self.navigationController popToViewController:temp animated:YES];
            break;
        }
    }
    
    [self.cocosView saveEffectInEffectScene:_isFromLegging];
    [self.cocosView leaveAndDestroyEffectScene:YES];
}


- (IBAction)backBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
    [self.cocosView leaveAndDestroyEffectScene:NO];
}

- (IBAction)resetBtnClicked:(UIButton *)sender {
    [self.cocosView resetEffect];
}

- (IBAction)pauseBtnClicked:(UIButton *)sender {
    _isPauseEffect = !_isPauseEffect;
    if(_isPauseEffect){
        [self.cocosView pauseEffect];
    }else{
        [self.cocosView resumeEffect];
    }
    [self switchPauseBtn];
    [self showOrHidePauseTip];
}

- (void)setup{
    [self.pauseBtn setEnabled:NO];
    _isPauseEffect = NO;
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"摇一摇";
    self.navigationItem.hidesBackButton = YES;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do any additional setup after loading the view.
    [self setup];
    
    [self.cocosView enterEffectScene];
    [self.cocosView setEffectSceneDelegate:@selector(onEffectActionCallback:) WithTarget:self];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    [self setupNavigationBar];
    
    [self.cocosView appear];
    
    if([self.navigationController isKindOfClass:[LeggingNavigationController class]]){
        _isFromLegging = YES;
    }else{
        _isFromLegging = NO;
    }
    
    int curEffectType = 0;
    if(_isFromLegging){
        LeggingNavigationController* nc = (LeggingNavigationController*)self.navigationController;
        
        if(nc.isChoosePattern){
            [self.cocosView applyPattern:nc.curPatternId];
        }else{
            [self.cocosView applyPatternFromArrangeItem];
        }
        curEffectType = nc.curEffectType;
    }else{
        LookbookNavigationController* nc = (LookbookNavigationController*)self.navigationController;
        
        [self.cocosView applyPatternFromLookbookItem];
        
        curEffectType = nc.curEffectType;
    }
    
    [self.cocosView applyEffect:curEffectType];
}

- (void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    
    [self.cocosView disappear];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_cocosView release];
    [_pauseBtn release];
    [super dealloc];
}
@end
