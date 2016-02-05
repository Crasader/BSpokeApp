//
//  LeggingPreviewViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/7/7.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LeggingPreviewViewController.h"
#import "LinearLayoutView.h"
#import "CocosView.h"

@interface LeggingPreviewViewController ()
@property (retain, nonatomic) IBOutlet CocosView *cocosView;
@property (retain, nonatomic) IBOutlet UIButton *orderBtn;
@property (retain, nonatomic) UIImageView* orderTipImageView;
@end

@implementation LeggingPreviewViewController{
    enum LeggingOrientation _curPantOrientation;
}

-(UIImageView*)orderTipImageView{
    if(!_orderTipImageView){
        _orderTipImageView = [[UIImageView alloc]initWithImage:[UIImage imageNamed:@"btn_order.png"]];
        CGRect frame = _orderTipImageView.frame;
        CGPoint origin = self.orderBtn.frame.origin;
        origin.y -= 39;
        origin.x -= 15;
        frame.origin = origin;
        
        _orderTipImageView.frame = frame;
        LinearLayoutView* superView = (LinearLayoutView*)self.orderBtn.superview;
        [superView addSubviewNoLayout:_orderTipImageView];
    }
    return _orderTipImageView;
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
    
    [self.cocosView applyLeggingOrientationInPreview:orientation];
    _curPantOrientation = orientation;
}

- (IBAction)favBtnClicked:(UIButton *)sender {
    
}

- (IBAction)cartBtnClicked:(UIButton *)sender {

}

- (IBAction)backBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"完成";
    self.navigationItem.hidesBackButton = YES;
}

- (void)setup{
    _curPantOrientation = LeggingOrientation_Back;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setup];
}

- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    
    [self setupNavigationBar];
    [self.cocosView appear];
    [self.cocosView enterLegggingPreviewScene];
}

- (void)viewDidAppear:(BOOL)animated{
    [super viewDidAppear:animated];
    self.orderTipImageView;
}

- (void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    
    [self.cocosView disappear];
    [self.cocosView leaveLeggingPreviewScene];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_cocosView release];
    [_orderBtn release];
    [super dealloc];
}
@end
