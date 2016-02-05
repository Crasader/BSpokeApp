//
//  MainViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/13.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "MainViewController.h"
#import "MainPageContentViewController.h"

@interface MainViewController () <UIPageViewControllerDataSource, UIPageViewControllerDelegate>
@property (retain, nonatomic) IBOutlet UIButton *leggingBtn;
@property (retain, nonatomic) IBOutlet UIButton *lookBookBtn;
@property (retain, nonatomic) IBOutlet UIView *pageViewContainer;

@property (nonatomic) NSArray *pageViewImageNames;
@property (retain, nonatomic) UIPageViewController *pageViewController;

@end

@implementation MainViewController

- (NSArray*)pageViewImageNames{
    return @[@"slide_1", @"slide_2"];
}

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
    
    MainPageContentViewController *pageContentViewController = [self.storyboard instantiateViewControllerWithIdentifier:@"MainPageViewContent"];
    NSString* path = [[NSBundle mainBundle]pathForResource:self.pageViewImageNames[index] ofType:@"jpg" inDirectory:@"slider"];
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
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)viewDidLayoutSubviews{
    [super viewDidLayoutSubviews];
    
    self.pageViewController.view.frame = self.pageViewContainer.bounds;
    [self.pageViewContainer addSubview:self.pageViewController.view];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)dealloc {
    if(_pageViewController){
        [_pageViewController release];
        _pageViewController = nil;
    }
    [_leggingBtn release];
    [_lookBookBtn release];
    [_pageViewContainer release];
    [super dealloc];
}
@end
