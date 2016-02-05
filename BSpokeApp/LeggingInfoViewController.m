//
//  LeggingInfoViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/13.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LeggingInfoViewController.h"
#import "MainPageContentViewController.h"
#import "MySlideMenuController.h"
#import "LeggingInfoSlideView.h"

@interface LeggingInfoViewController ()<UIPageViewControllerDataSource, UIPageViewControllerDelegate, LeggingInfoSlideViewDelegate>

@property (assign, nonatomic) UIPageViewController *pageViewController;
@property (assign, nonatomic) LeggingInfoSlideView *leggingInfoView;

@property (retain, nonatomic) IBOutlet UIView *pageViewContainer;
@property (nonatomic) NSArray *pageViewImageNames;



@end

@implementation LeggingInfoViewController

#pragma mark - property

- (LeggingInfoSlideView*)leggingInfoView{
    if(_leggingInfoView == nil){
        NSArray* views = [[NSBundle mainBundle]loadNibNamed:@"LeggingInfoView" owner:self options:nil];
        _leggingInfoView = [views lastObject];
        _leggingInfoView.delegate = self;
        [_leggingInfoView retain];
    }
    
    return _leggingInfoView;
}

- (NSArray*)pageViewImageNames{
    return @[@"product_%d_1", @"product_%d_2"];
}

#pragma mark - LeggingInfoSlideViewDelegate
- (void)onBtnClicked:(UIButton*)btn WithSender:(LeggingInfoSlideView*)sender{
//    [self performSegueWithIdentifier:@"startEdit" sender:self];
    [self performSegueWithIdentifier:@"startEdit" sender:self];
}

#pragma mark - PageViewController
- (UIPageViewController*)pageViewController{
    if(_pageViewController == nil){
        
        _pageViewController = [[UIPageViewController alloc]initWithTransitionStyle:UIPageViewControllerTransitionStyleScroll
                                                             navigationOrientation:UIPageViewControllerNavigationOrientationHorizontal
                                                                           options:nil];
        _pageViewController.delegate    = self;
        _pageViewController.dataSource  = self;
        MainPageContentViewController* first = [self viewControllerAtIndex:0];
        
        NSArray* viewControllers = @[first];
        [_pageViewController setViewControllers:viewControllers
                                      direction:UIPageViewControllerNavigationDirectionForward
                                       animated:YES
                                     completion:nil];
        [_pageViewController retain];
    }
    
    return _pageViewController;
}

- (MainPageContentViewController*)viewControllerAtIndex:(NSUInteger)index{
    
    if([self.pageViewImageNames count] == 0 || index >= [self.pageViewImageNames count])
        return nil;
    
    int typeIndex = 0;
    LeggingNavigationController* navigationController = (LeggingNavigationController*)self.navigationController;
    MainPageContentViewController *pageContentViewController = [self.storyboard instantiateViewControllerWithIdentifier:@"MainPageViewContent"];
    
    if(navigationController.curLeggingType == LeggingType_7Cutting){
        typeIndex = 1;
    }else if(navigationController.curLeggingType == LeggingType_9Cutting){
        typeIndex = 2;
    }
    
    NSString* fileName = [NSString stringWithFormat:self.pageViewImageNames[index], typeIndex];
    NSString* path = [[NSBundle mainBundle]pathForResource:fileName
                                                    ofType:@"jpg"
                                               inDirectory:@"products"];
    
    pageContentViewController.image     = [UIImage imageWithContentsOfFile:path];
    pageContentViewController.pageIndex = index;
    
    return pageContentViewController;
}

#pragma mark - UIPageViewControllerDataSource, UIPageViewControllerDelegate

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerBeforeViewController:(UIViewController *)viewController{
    
    NSUInteger index = ((MainPageContentViewController*)viewController).pageIndex;
    
    if(index == 0 || self.pageViewImageNames.count == 0)
        return nil;
    
    index--;
    
    return [self viewControllerAtIndex:index];
}

- (UIViewController *)pageViewController:(UIPageViewController *)pageViewController viewControllerAfterViewController:(UIViewController *)viewController{
    NSUInteger index = ((MainPageContentViewController*)viewController).pageIndex;
    
    if(self.pageViewImageNames.count == 0)
        return nil;
    
    index++;
    
    if(index >= self.pageViewImageNames.count)
        return nil;
    
    return [self viewControllerAtIndex:index];
}

- (NSInteger)presentationCountForPageViewController:(UIPageViewController *)pageViewController{
    return self.pageViewImageNames.count;
}

- (NSInteger)presentationIndexForPageViewController:(UIPageViewController *)pageViewController{
    return 0;
}

#pragma mark - UIViewController

- (void)setup{
    
    NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:self.pageViewController.view
                                                                       attribute:NSLayoutAttributeWidth
                                                                       relatedBy:NSLayoutRelationEqual
                                                                          toItem:self.pageViewContainer
                                                                       attribute:NSLayoutAttributeWidth
                                                                      multiplier:1.0
                                                                        constant:0];
    NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:self.pageViewController.view
                                                                        attribute:NSLayoutAttributeHeight
                                                                        relatedBy:NSLayoutRelationEqual
                                                                           toItem:self.pageViewContainer
                                                                        attribute:NSLayoutAttributeHeight
                                                                       multiplier:1.0
                                                                         constant:0];
    
    self.pageViewController.view.frame = self.pageViewContainer.bounds;
    [self.pageViewContainer addSubview:self.pageViewController.view];
    
    [self.pageViewContainer addConstraint:widthConstraint];
    [self.pageViewContainer addConstraint:heightConstraint];
    
    NSString *infoImagePath = [[NSBundle mainBundle]pathForResource:@"product_info" ofType:@"png" inDirectory:@"products"];
    [self.leggingInfoView setImage:[UIImage imageWithContentsOfFile:infoImagePath]];
    [self setMenuView:self.leggingInfoView];
    
    NSLayoutConstraint *constraint1 = [NSLayoutConstraint constraintWithItem:self.menuView
                                                                   attribute:NSLayoutAttributeWidth
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:self.view
                                                                   attribute:NSLayoutAttributeWidth
                                                                  multiplier:1.0
                                                                    constant:0];
    [self.view addConstraints:@[constraint1]];
    
    NSLayoutConstraint *constraint2 = [NSLayoutConstraint constraintWithItem:self.menuView
                                                                   attribute:NSLayoutAttributeCenterX
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:self.view
                                                                   attribute:NSLayoutAttributeCenterX
                                                                  multiplier:1.0
                                                                    constant:0];
    [self.view addConstraint:constraint2];
    
    
    NSLayoutConstraint *constraint3 = [NSLayoutConstraint constraintWithItem:self.menuView
                                                                   attribute:NSLayoutAttributeTop
                                                                   relatedBy:NSLayoutRelationEqual
                                                                      toItem:self.pageViewContainer
                                                                   attribute:NSLayoutAttributeBottom
                                                                  multiplier:1.0
                                                                    constant:0];
    [self.view addConstraint:constraint3];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"详情";
}

- (IBAction)preBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
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

- (void)viewDidLayoutSubviews{
    [super viewDidLayoutSubviews];
    CGRect frame = self.leggingInfoView.frame;
    
    [self setBeginPoint:frame.origin
            AndEndPoint:CGPointMake(frame.origin.x, frame.origin.y - frame.size.height)];
}

- (void)dealloc {
    if(_pageViewController){
        [_pageViewController release];
        _pageViewController = nil;
    }
    if(_leggingInfoView){
        [_leggingInfoView release];
        _leggingInfoView = nil;
    }
    [_pageViewContainer release];
    [super dealloc];
}
@end
