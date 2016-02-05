//
//  LookBookViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/19.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LookBookViewController.h"
#import "cocos/CocosView.h"
#import "MyUITabBar.h"
#import "BottomTabBarItemView.h"
#import "BottomCollectionViewCell.h"
#import "BottomCollectionViewLabelCell.h"
#import "EffectViewController.h"
#import "AlertView.h"

enum TopLevelType{
    TopLevelType_Fashion, TopLevelType_Opaque, TopLevelType_Filter, TopLevelType_None
};

@interface LookBookViewController () <UICollectionViewDelegate, UICollectionViewDataSource, MyUITabBarDelegate, AlertViewDelegate>
@property (nonatomic) NSArray *itemFashionArray;
@property (nonatomic) NSArray *itemFilterArray;
@property (nonatomic) NSArray *filterNameArray;
@property (nonatomic) NSArray *filterNameTextArray;

@property (retain, nonatomic) IBOutlet CocosView *cocosView;
@property (retain, nonatomic) IBOutlet MyUITabBar *tabBar;
@property (retain, nonatomic) IBOutlet UICollectionView *collectionView;
@property (retain, nonatomic) IBOutlet UIView *opaqueView;
@property (retain, nonatomic) IBOutlet UISlider *opaqueSlider;
@end

@implementation LookBookViewController{
    enum TopLevelType _curTopLevelSelectType;
    int _curLookbookItem;
}

- (NSArray*)itemFashionArray{
    return @[@"mark_1", @"mark_2", @"mark_3", @"mark_4"];
}

- (NSArray*)itemFilterArray{
    return @[@"filter_1", @"filter_2", @"filter_3", @"filter_4", @"filter_5"];
}

- (NSArray*)filterNameArray{
    return @[@"CIPhotoEffectTransfer", @"CIPhotoEffectInstant", @"CIPhotoEffectNoir", @"CIPhotoEffectFade", @"CIPhotoEffectChrome"];
}

- (NSArray*)filterNameTextArray{
    return @[@"Transfer", @"Instant", @"Noir", @"Fade", @"Chrome"];
}

#pragma mark - custom

- (CIFilter*)createFilterWithName:(NSString*)name{
    CIFilter* filter = [CIFilter filterWithName:name];
    return filter;
}

- (void)showAlertBox{
    NSArray* views = [[NSBundle mainBundle]loadNibNamed:@"AlertView" owner:nil options:nil];
    AlertView* view = [views lastObject];
    [view setFrame:self.view.bounds];
    
    view.delegate = self;
    
    [self.view addSubview:view];
}

- (void)switchBottomCollectionViewContent:(int)type{
    _curTopLevelSelectType          = type;
    
    if(_curTopLevelSelectType == TopLevelType_Fashion){
        [self.collectionView setHidden:NO];
        [self.opaqueView setHidden:YES];
        [self.collectionView reloadData];
    }else if(_curTopLevelSelectType == TopLevelType_Opaque){
        [self.collectionView setHidden:YES];
        [self.opaqueView setHidden:NO];
    }else if(_curTopLevelSelectType == TopLevelType_Filter){
        [self.collectionView setHidden:NO];
        [self.opaqueView setHidden:YES];
        [self.collectionView reloadData];
    }
}

- (void)cocosViewTouchCallback{
    [self performSegueWithIdentifier:@"chooseEffect" sender:self];
}

- (void)cocosViewItemFocusCallback:(id)userInfo{
    if([userInfo isKindOfClass:[NSDictionary class]]){
        NSDictionary* info   = (NSDictionary*)userInfo;
        NSNumber* shadowType = [info objectForKey:@"shadowType"];
        NSNumber* opaque     = [info objectForKey:@"opaque"];
        
        self.opaqueSlider.value = opaque.floatValue;
    }
}

#pragma mark - opaque view
- (IBAction)shadowBtnClick:(UIButton *)sender {
    int tag = (int)sender.tag;
    [self.cocosView applyShadow:tag];
}

- (IBAction)opaqueValueChanged:(UISlider *)sender {
    int value = sender.value;
    [self.cocosView applyOpaque:value];
}

#pragma mark - <AlertboxDelegate>

- (void) onYesButtonClicked:(AlertView*)alertView{
    [alertView removeFromSuperview];
    
    [self.cocosView cleanLookbookItem];
    [self.cocosView applyLookbookItem:_curLookbookItem];
}

- (void) onNoButtonClicked:(AlertView*)alertView{
    [alertView removeFromSuperview];
    _curLookbookItem = 0;
}

#pragma mark - UICollectionViewDataSource UICollectionViewDelegate

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewFlowLayout* layout  = (UICollectionViewFlowLayout*)collectionViewLayout;
    
    CGSize parentSize  = collectionView.bounds.size;
    float width        = layout.itemSize.width;
    float height       = parentSize.height - layout.sectionInset.top - layout.sectionInset.bottom;
    
    return CGSizeMake(width, height);
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    int index = (int)indexPath.item;
    
    if(_curTopLevelSelectType == TopLevelType_Fashion){
        if([self.cocosView hasLookBookItem]){
            _curLookbookItem = index + 1;
            [self showAlertBox];
        }else{
            [self.cocosView applyLookbookItem:index + 1];
        }
    }else if(_curTopLevelSelectType == TopLevelType_Filter){
        NSString* filterName = self.filterNameArray[index];
        CIFilter* filter = [self createFilterWithName:filterName];
        UIImage* image = self.image;
        CIContext* context = [CIContext contextWithOptions:nil];
        CGImageRef cgimage = image.CGImage;
        CIImage* ciimage = [CIImage imageWithCGImage:cgimage];
        
        [filter setValue:ciimage forKey:kCIInputImageKey];
        
        CIImage* result = [filter valueForKey:kCIOutputImageKey];
        CGRect extent = [result extent];
        CGImageRef newCGImage = [context createCGImage:result fromRect:extent];
        
        [self.cocosView applyBackground:newCGImage];
        
        CGImageRelease(newCGImage);
    }
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    NSUInteger count = 0;
    
    if(_curTopLevelSelectType == TopLevelType_Fashion)
        count = self.itemFashionArray.count;
    else if(_curTopLevelSelectType == TopLevelType_Filter)
        count = self.itemFilterArray.count;
    
    return count;
}

// The cell that is returned must be retrieved from a call to -dequeueReusableCellWithReuseIdentifier:forIndexPath:
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    int index = (int)indexPath.item;
    UICollectionViewCell* ret = nil;
    
    if(_curTopLevelSelectType == TopLevelType_Fashion){
        BottomCollectionViewCell* cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"FashionCell" forIndexPath:indexPath];
        
        cell.imageView.image = [UIImage imageNamed:self.itemFashionArray[index]];
        
        ret = cell;
    }else if(_curTopLevelSelectType == TopLevelType_Filter){
        BottomCollectionViewLabelCell* cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"FilterCell" forIndexPath:indexPath];
        
        cell.imageView.image = [UIImage imageNamed:self.itemFilterArray[index]];
        [cell.labelView setText:self.filterNameTextArray[index]];
        
        ret = cell;
    }
    
    
    return ret;
}

#pragma mark - MyTabBarDelegate

- (void)tabBar:(MyUITabBar *)tabBar didSelectItem:(MyUITabBarItem *)item{
    int tag = (int)item.tag;
    [self switchBottomCollectionViewContent:tag];
}

#pragma mark - UIViewController

- (IBAction)preBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
    [self.cocosView leaveAndDestroyLookbookScene];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if([segue.identifier isEqualToString:@"showEffect"]){
        if([segue.destinationViewController isKindOfClass:[EffectViewController class]]){
//            EffectViewController* destinateViewController = (EffectViewController*)segue.destinationViewController;
        }
    }
}

- (void)setup{
    //tabbarItem
    BottomTabBarItemView *item1 = [[BottomTabBarItemView alloc]initWithTitle:@"潮流标签"];
    BottomTabBarItemView *item2 = [[BottomTabBarItemView alloc]initWithTitle:@"透明度"];
    BottomTabBarItemView* item3 = [[BottomTabBarItemView alloc]initWithTitle:@"图片滤镜"];
    
    [item1 autorelease];
    [item2 autorelease];
    [item3 autorelease];
    
    item1.tag = TopLevelType_Fashion;
    item2.tag = TopLevelType_Opaque;
    item3.tag = TopLevelType_Filter;
    
    NSLog(@"%lu", [item1 retainCount]);
    [self.tabBar setItems:@[item1, item2, item3]];
    NSLog(@"%lu", [item1 retainCount]);
    
    
    self.tabBar.delegate      = self;
    self.tabBar.selectedItem  = self.tabBar.items[0];
    
    //collection
    self.collectionView.delegate   = self;
    self.collectionView.dataSource = self;
    
    //opaque view
    [self.opaqueView setHidden:YES];
    [self.opaqueSlider setThumbImage:[UIImage imageNamed:@"sliderdot"] forState:UIControlStateNormal];
    [self.opaqueSlider setThumbImage:[UIImage imageNamed:@"sliderdot_active"] forState:UIControlStateHighlighted];
    [self.opaqueSlider setMinimumTrackImage:[UIImage imageNamed:@"progress_full"] forState:UIControlStateNormal];
    [self.opaqueSlider setMaximumTrackImage:[UIImage imageNamed:@"progress_blank"] forState:UIControlStateNormal];
    
    //cocos view
    [self.cocosView setTouchDelegate:@selector(cocosViewTouchCallback) WithTarget:self];
    [self.cocosView setLookbookSceneDelegate:@selector(cocosViewItemFocusCallback:) WithTarget:self];
    
    _curTopLevelSelectType = TopLevelType_Fashion;
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"LOOKBOOK";
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self setup];
    
    [self.cocosView enterLookbookScene];
    [self.cocosView applyBackground:self.image.CGImage];
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
    [_image release];
    [_cocosView release];
    [_tabBar release];
    [_collectionView release];
    [_opaqueView release];
    [_opaqueSlider release];
    [super dealloc];
}
@end
