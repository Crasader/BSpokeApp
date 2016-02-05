//
//  LeggingPatternChooseViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/6/11.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "LeggingNavigationController.h"
#import "LeggingPatternChooseViewController.h"
#import "BottomCollectionViewCell.h"
#import "LeggingViewController.h"

@interface LeggingPatternChooseViewController ()<UICollectionViewDelegate, UICollectionViewDataSource>
@property (retain, nonatomic) IBOutlet UICollectionView *collectionView;
@property (nonatomic) NSArray* patterns;

@end

@implementation LeggingPatternChooseViewController


#pragma mark - property

- (NSArray*)patterns{
    return @[@"pattern_1", @"pattern_2", @"pattern_3", @"pattern_4", @"pattern_5", @"pattern_6", @"pattern_7", @"pattern_8", @"pattern_9"];
}

#pragma mark - <UICollectionViewDelegate>

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewFlowLayout* layout  = (UICollectionViewFlowLayout*)collectionViewLayout;
    
    CGSize parentSize  = collectionView.bounds.size;
    float width = floor((parentSize.width - layout.sectionInset.left - layout.sectionInset.right - 2 * layout.minimumInteritemSpacing) / 3);
    float height = width / 160 * 180;
    
    return CGSizeMake(width, height);
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    
    NSUInteger index = indexPath.row;
    LeggingNavigationController* navigationController = (LeggingNavigationController*)self.navigationController;
    
    navigationController.curPatternId = (int)index + 1;
    
//    [self performSegueWithIdentifier:@"chooseEffect" sender:self];
    
    for(UIViewController* controller in self.navigationController.viewControllers){
        if([controller isKindOfClass:[LeggingViewController class]]){
            LeggingViewController* ln = (LeggingViewController*)controller;
            [ln applyPatternToSelectedItem:navigationController.curPatternId];
            break;
        }
    }
    [self.navigationController popViewControllerAnimated:YES];
}

#pragma mark - <UICollectionViewDataSource>

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return [self.patterns count];
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    BottomCollectionViewCell* cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"Cell" forIndexPath:indexPath];
    NSUInteger index    = indexPath.row;
    NSString* imagePath = [[NSBundle mainBundle] pathForResource:self.patterns[index] ofType:@"png" inDirectory:@"pattern"];
    
    cell.imageView.image = [UIImage imageWithContentsOfFile:imagePath];
    
    return cell;
}

#pragma mark - UIViewController

- (IBAction)preBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18.0], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"图案选择";
}

- (void)setup{
    self.collectionView.delegate    = self;
    self.collectionView.dataSource  = self;
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
    if(_collectionView){
        _collectionView.delegate = nil;
        _collectionView.dataSource = nil;
        [_collectionView release];
        _collectionView = nil;
    }
    [super dealloc];
}
@end
