//
//  LeggingViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/4/30.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LeggingViewController.h"
#import "cocos/CocosView.h"
#import "MyUITabBar.h"
#import "EffectViewController.h"

#define LEGGINGSCENE_TABBAR_PATTERN 1
#define LEGGINGSCENE_TABBAR_STYLE 2
#define LEGGINGSCENE_TABBAR_COLOR 3

enum TopLevelType{
    TopLevelType_Pattern, TopLevelType_Style, TopLevelType_Color, TopLevelType_None
};

enum SecondLevelType{
    SecondLevelType_BaseItem, SecondLevelType_Fashion, SecondLevelType_LookBook, SecondLevelType_Label, SecondLevelType_None
};

@interface LeggingViewController ()

@property (retain, nonatomic) IBOutlet CocosView *cocosView;
@property (retain, nonatomic) IBOutlet UIButton *resetBtn;
@property (retain, nonatomic) IBOutlet UIButton *viewBtn;
@property (retain, nonatomic) IBOutlet UIButton *colorBtn;
@property (retain, nonatomic) IBOutlet UIButton *zoomBtn;

@property (nonatomic) NSArray *itemBasePatternArray;
@property (nonatomic) NSArray *itemStyleArray;
@property (nonatomic) NSArray *itemColorArray;

@property (nonatomic) NSArray *applyLeggingColorArray;

@end

@implementation LeggingViewController{
    enum TopLevelType _curTopLevelSelectType;
    enum SecondLevelType _curSecondLevelSelectType;
    enum LeggingOrientation _curPantOrientation;
    BOOL _isScale;
    BOOL _isBlack;
}

#pragma mark - property
- (NSArray*)itemBasePatternArray{
    return @[@"card_pattern_1", @"card_pattern_2", @"card_pattern_3"];
}

- (NSArray*)itemStyleArray{
    return @[@"arrange_1", @"arrange_2", @"arrange_3"];
}

- (NSArray*)itemColorArray{
    return @[
             [UIColor colorWithRed:0 green:0 blue:0 alpha:1],
             [UIColor colorWithRed:1 green:1 blue:1 alpha:1],
             [UIColor colorWithRed:0 green:0.62 blue:0.91 alpha:1],
             [UIColor colorWithRed:1 green:1 blue:0 alpha:1],
             [UIColor colorWithRed:0.37 green:0.09 blue:0.52 alpha:1],
             [UIColor colorWithRed:0.81 green:0 blue:0.15 alpha:1]
             ];
}

- (NSArray*)applyLeggingColorArray{
    return @[@"black", @"white", @"blue", @"yellow", @"darkpurple", @"darkred"];
}

#pragma mark - Custom

- (void)applyPatternToSelectedItem:(NSUInteger)patternId{
    [self.cocosView applySelectedItemPattern:patternId];
}

- (void)arrangeItemSelectStateCallback:(id)userInfo{
    if([userInfo isKindOfClass:[NSDictionary class]]){
        NSDictionary* dict = (NSDictionary*)userInfo;
        NSNumber* number = [dict objectForKey:@"isSelected"];
        
        [self.resetBtn setEnabled:number.boolValue];
    }
}

- (void)switchColorBtn:(UIButton*)item{
    NSString* imageName;
    if(_isBlack){
        imageName = @"btn_color_grey";
    }else{
        imageName = @"btn_color_black";
    }
    
    [item setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
}

- (void)switchZoomBtn:(UIButton*)item{
    NSString* imageName;
    if(_isScale){
        imageName = @"btn_zoomin";
    }else{
        imageName = @"btn_zoomout";
    }
    
    [item setImage:[UIImage imageNamed:imageName] forState:UIControlStateNormal];
}

- (void)switchBottomCollectionViewContent:(int)type{
    _curTopLevelSelectType      = type;
    _curSecondLevelSelectType   = SecondLevelType_None;
}

- (void)switchBottomCollectionViewContent:(int)type AndSubType:(int)subtype{
    _curTopLevelSelectType      = type;
    _curSecondLevelSelectType   = subtype;
}


#pragma mark - CocosView

- (void)cocosViewTouchCallback{
//    [self performSegueWithIdentifier:@"chooseEffect" sender:self];
}


- (void)arrangeBtnClickedCallback{
    LeggingNavigationController* nc = (LeggingNavigationController*)self.navigationController;
    nc.isChoosePattern = YES;
    [self performSegueWithIdentifier:@"choosePattern" sender:self];
}

- (void)arrangeItemClickedCallback{
    LeggingNavigationController* nc = (LeggingNavigationController*)self.navigationController;
    nc.isChoosePattern = NO;
    [self performSegueWithIdentifier:@"chooseEffect" sender:self];
}

#pragma mark - UIViewController

- (IBAction)resetBtnClicked:(UIButton *)sender {
    [self.cocosView applyRecoverSelectedItem];
}

- (IBAction)viewBtnClicked:(UIButton *)sender {
    enum LeggingOrientation orientation = LeggingOrientation_None;
    switch(_curPantOrientation){
        case LeggingOrientation_Front:
            orientation = LeggingOrientation_Side;
            break;
            
        case LeggingOrientation_Side:
            orientation = LeggingOrientation_Back;
            break;
            
        case LeggingOrientation_Back:
            orientation = LeggingOrientation_Front;
            break;
            
        default:
            break;
    }
    
    [self.cocosView applyLeggingOrientation:orientation];
    _curPantOrientation = orientation;
}

- (IBAction)colorBtnClicked:(UIButton *)sender {
    _isBlack = !_isBlack;
    if(_isBlack)
        [self.cocosView applyLeggingColor:@"black"];
    else
        [self.cocosView applyLeggingColor:@"gray"];
    [self switchColorBtn:sender];
}

- (IBAction)zoomBtnClicked:(UIButton *)sender {
    _isScale = !_isScale;
    [self.cocosView applyScale:_isScale];
    [self switchZoomBtn:sender];
}

- (IBAction)preBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
    
    [self.cocosView leaveAndDestroyArrangeScene];
}
- (IBAction)previewBtnClicked:(UIBarButtonItem *)sender {
    [self performSegueWithIdentifier:@"preview" sender:self];
}

- (void)setup{
    _curTopLevelSelectType = TopLevelType_None;
    _curSecondLevelSelectType = SecondLevelType_None;
    _curPantOrientation = LeggingOrientation_Back;
    _isBlack = YES;
    _isScale = YES;
    
    [self switchColorBtn:self.colorBtn];
    [self switchZoomBtn:self.zoomBtn];
    [self.resetBtn setEnabled:NO];
    
    [self.cocosView setTouchDelegate:@selector(cocosViewTouchCallback) WithTarget:self];
    [self.cocosView setArrangeSceneBtnClickedDelegate:@selector(arrangeBtnClickedCallback) WithTarget:self];
    [self.cocosView setArrangeSceneItemClickedDelegate:@selector(arrangeItemClickedCallback) WithTarget:self];
    [self.cocosView setArrangeSceneItemSelectStateDelegate:@selector(arrangeItemSelectStateCallback:) WithTarget:self];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"REMIX!";
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    [self setup];
    
    [self.cocosView enterArrangeScene];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    [self setupNavigationBar];
    
    [self.cocosView appear];
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
    [_resetBtn release];
    [_viewBtn release];
    [_colorBtn release];
    [_zoomBtn release];
    [super dealloc];
}
@end
