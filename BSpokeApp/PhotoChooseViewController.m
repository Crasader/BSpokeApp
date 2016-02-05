//
//  PhotoChooseViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/18.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "PhotoChooseViewController.h"
#import "PhotoChooseCollectionViewCell.h"
#import <Photos/Photos.h>
#import "PhotoEditViewController.h"
#import "CocosView.h"

@interface PhotoChooseViewController ()<UICollectionViewDelegateFlowLayout, UICollectionViewDataSource, PHPhotoLibraryChangeObserver, UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (retain, nonatomic) IBOutlet UICollectionView *collectionView;
@property (retain, nonatomic) IBOutlet CocosView *calculateCocosView;

@property (retain, nonatomic) PHFetchResult *fetchResult;
@property (assign, nonatomic) PHImageManager *imageManager;

@end

@implementation PhotoChooseViewController{
    NSUInteger _selectedIndex;
    BOOL _isTakeNewPhoto;
}

- (PHFetchResult*)fetchResult{
    if(_fetchResult == nil){
        PHFetchOptions *option = [[PHFetchOptions alloc]init];
        option.sortDescriptors = @[[NSSortDescriptor sortDescriptorWithKey:@"creationDate" ascending:NO]];
        _fetchResult = [PHAsset fetchAssetsWithOptions:option];
        [option release];
        [_fetchResult retain];
    }
    
    return _fetchResult;
}

- (PHImageManager*)imageManager{
    if(_imageManager == nil){
        _imageManager = [PHImageManager defaultManager];
    }
    return _imageManager;
}

#pragma mark - custom

- (void)addNewAssetWithImage:(UIImage*)image toAlbum:(PHAssetCollection*)album{
    
    [[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
        
        PHAssetChangeRequest* request = [PHAssetChangeRequest creationRequestForAssetFromImage:image];
        
        if(album){
            PHAssetCollectionChangeRequest* albumRequest = [PHAssetCollectionChangeRequest changeRequestForAssetCollection:album];
            PHObjectPlaceholder *assetPlaceholder = [request placeholderForCreatedAsset];
            [albumRequest addAssets:@[ assetPlaceholder ]];
        }
    }
     
                                      completionHandler:^(BOOL success, NSError *error) {
                                          NSLog(@"Finished adding asset. %@", (success ? @"Success" : error));
                                      }];
}

- (void)openCamera{
    UIImagePickerControllerSourceType sourceType = UIImagePickerControllerSourceTypeCamera;
    if ([UIImagePickerController isSourceTypeAvailable: UIImagePickerControllerSourceTypeCamera])
    {
        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.delegate = self;
        //设置拍照后的图片可被编辑
        picker.allowsEditing = NO;
        picker.sourceType = sourceType;
        
        [self presentViewController:picker animated:YES completion:nil];
    }else
    {
        NSLog(@"模拟其中无法打开照相机,请在真机中使用");
    }
}

#pragma mark - UICollectionViewDelegate

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout*)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath{
    UICollectionViewFlowLayout* layout  = (UICollectionViewFlowLayout*)collectionViewLayout;
    
    CGSize parentSize  = collectionView.bounds.size;
    float width = floor((parentSize.width - layout.sectionInset.left - layout.sectionInset.right - 2 * layout.minimumInteritemSpacing) / 3);
    float height = width;
    
    return CGSizeMake(width, height);
}


- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger index = indexPath.item;
    
    if(index == 0){
        //照相机
        [self openCamera];
    }else{
        _selectedIndex = index - 1;
        [self performSegueWithIdentifier:@"editPhoto" sender:self];
    }
}

#pragma mark - UICollectionViewDataSource

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    NSInteger count = [self.fetchResult count];
    
    return count + 1;
}

// The cell that is returned must be retrieved from a call to -dequeueReusableCellWithReuseIdentifier:forIndexPath:
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    // Configure the cell
    
    NSUInteger index = indexPath.item;
    UICollectionViewCell *ret = nil;
    
    if(index == 0){
        //拍照
        UICollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"CameraCell" forIndexPath:indexPath];
        ret = cell;
    }else{
        PhotoChooseCollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"PhotoCell" forIndexPath:indexPath];
        CGSize cellSize = cell.bounds.size;
        NSUInteger curTag = cell.tag + 1;
        cell.tag = curTag;
        
        static PHImageRequestOptions *requestOptions;
        if(!requestOptions){
            requestOptions = [[PHImageRequestOptions alloc] init];
            requestOptions.networkAccessAllowed = YES;
            requestOptions.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
            requestOptions.resizeMode = PHImageRequestOptionsResizeModeFast;
        }
    
        PHAsset *asset = self.fetchResult[index - 1];
        
        [self.imageManager requestImageForAsset:asset
                                     targetSize:cellSize
                                    contentMode:PHImageContentModeAspectFill
                                        options:requestOptions
                                  resultHandler:^(UIImage *result, NSDictionary *info) {
                                      if(cell.tag == curTag){
                                          cell.image = result;
                                      }
                                  }];
        
        ret = cell;
    }
    
    return ret;
}

#pragma mark <PHPhotoLibraryChangeObserver>

- (void)photoLibraryDidChange:(PHChange *)changeInstance{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        PHFetchResultChangeDetails* fetchChangeDetail = [changeInstance changeDetailsForFetchResult:self.fetchResult];
        
        if(fetchChangeDetail){
            self.fetchResult = fetchChangeDetail.fetchResultAfterChanges;
            [self.collectionView reloadData];
        }
        
        if(_isTakeNewPhoto){
            _selectedIndex = 0;
            [self performSegueWithIdentifier:@"editPhoto" sender:self];
            
            _isTakeNewPhoto = NO;
            
        }
    });
}

#pragma mark - <UIImagePickerControllerDelegate>

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    NSString *type = [info objectForKey:UIImagePickerControllerMediaType];
    if([type isEqualToString:@"public.image"]){
        NSLog(@"take photo");
        UIImage* image = [info objectForKey:UIImagePickerControllerOriginalImage];
        
        [self addNewAssetWithImage:image
                           toAlbum:nil];
        
        _isTakeNewPhoto = YES;
    }
    
    [picker dismissViewControllerAnimated:YES completion:nil];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker{
    NSLog(@"您取消了选择图片");
    [picker dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - UIViewController

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if([segue.identifier isEqualToString:@"editPhoto"]){
        if([segue.destinationViewController isKindOfClass:[PhotoEditViewController class]]){
            PhotoEditViewController *destinate = (PhotoEditViewController*)segue.destinationViewController;
            PHAsset *asset = self.fetchResult[_selectedIndex];
            
            destinate.asset = asset;
        }
    }
}

- (IBAction)backBtnClick:(UIBarButtonItem *)sender {
    [self.navigationController dismissViewControllerAnimated:YES
                                                  completion:nil];
}

- (void)setup{
    //collectionView
    self.collectionView.delegate    = self;
    self.collectionView.dataSource  = self;
    
    [[PHPhotoLibrary sharedPhotoLibrary] registerChangeObserver:self];
    
    _selectedIndex  = 0;
    _isTakeNewPhoto = NO;
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"照片选择";
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

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [[PHPhotoLibrary sharedPhotoLibrary] unregisterChangeObserver:self];
    
    [_calculateCocosView destroyEAGLView];
    [_collectionView release];
    [_fetchResult release];
    [_calculateCocosView release];
    [super dealloc];
}
@end
