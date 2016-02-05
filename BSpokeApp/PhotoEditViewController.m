//
//  PhotoEditViewController.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/19.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "PhotoEditViewController.h"
#import "LookBookViewController.h"
#import "TestViewController1.h"

@interface PhotoEditViewController ()<UIScrollViewDelegate>

@property (retain, nonatomic) IBOutlet UIScrollView *scrollView;
@property (retain, nonatomic) UIImageView *imageView;
@property (retain, nonatomic) IBOutlet UIView *coverView;
@property (retain, nonatomic) IBOutlet UIButton *button;

@end

@implementation PhotoEditViewController{
    float _ratio;       //分辨率比例
    CGRect _blankRect;
    PHImageRequestID _curRequestID;
}

- (UIImageView*)imageView{
    if(_imageView == nil){
        _imageView = [[UIImageView alloc]init];
//        _imageView.contentMode = UIViewContentModeCenter;
        NSLog(@"%lu", [_imageView retainCount]);
    }
    
    return _imageView;
}

#pragma mark - customer

-(UIImage*)resizeImage:(UIImage*)image WithSize:(CGSize)newSize{
    // Create a graphics image context
    UIGraphicsBeginImageContext(newSize);
    
    // Tell the old image to draw in this new context, with the desired
    // new size
    [image drawInRect:CGRectMake(0, 0, newSize.width,newSize.height)];
    
    // Get the new image from the context
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    // End the context
    UIGraphicsEndImageContext();
    
    // Return the new image.
    return newImage;
}


- (UIImage*)resizeImage:(UIImage*)image FixedWidth:(CGFloat)width IsPNG:(BOOL)isPng{
    float scale  = image.size.width / width;
    NSData* data = nil;
    if(isPng){
        data = UIImagePNGRepresentation(image);
    }else{
        data = UIImageJPEGRepresentation(image, 1);
    }
    
    UIImage* newImage = [UIImage imageWithData:data scale:scale];
    
    return newImage;
}

-(UIImage*)clipImage:(UIImage*)image withRect:(CGRect)rect{
    CGSize imageSize = image.size;
    
    // Begin the context
    UIGraphicsBeginImageContext(rect.size);
    [image drawInRect:CGRectMake(rect.origin.x, rect.origin.y, imageSize.width, imageSize.height)];
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    // End the context
    UIGraphicsEndImageContext();

    return newImage;
}

- (void)requireAndSetImage{
    PHAsset* asset = self.asset;
    float screenScale = [UIScreen mainScreen].scale;
    CGFloat imageRequestWidth = self.scrollView.bounds.size.width * screenScale;
    float scale = asset.pixelWidth / imageRequestWidth;
    CGSize imageSize = CGSizeMake(imageRequestWidth, asset.pixelHeight / scale);
    CGSize imageViewSize = CGSizeMake(imageSize.width / screenScale, imageSize.height / screenScale);
    
    static PHImageRequestOptions *requestOptions1;
    if(!requestOptions1){
        requestOptions1 = [[PHImageRequestOptions alloc] init];
        requestOptions1.networkAccessAllowed = YES;
        requestOptions1.deliveryMode = PHImageRequestOptionsDeliveryModeOpportunistic;
    }
    
    if(_curRequestID != 0){
        [[PHImageManager defaultManager] cancelImageRequest:_curRequestID];
        _curRequestID = 0;
    }
    _curRequestID = [[PHImageManager defaultManager] requestImageDataForAsset:asset
                                                      options:requestOptions1
                                                resultHandler:^(NSData *imageData, NSString *dataUTI, UIImageOrientation orientation, NSDictionary *info) {
                                                    UIImage* newImage = [UIImage imageWithData:imageData scale:scale];
                                                    [self.imageView setImage:newImage];
                                                    
                                                    _curRequestID = 0;
                                                }];
    
    CGRect rect = CGRectZero;
    rect.size = imageViewSize;
    self.imageView.bounds = rect;
    self.scrollView.contentSize = imageViewSize;
}

#pragma mark - UIScrollViewDelegate
- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView{
    return self.imageView;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    NSLog(@"%@", NSStringFromCGPoint(scrollView.contentOffset));
}

- (void)scrollViewDidZoom:(UIScrollView *)scrollView NS_AVAILABLE_IOS(3_2){
    CGRect frame = self.imageView.frame;
    frame.origin = CGPointZero;
    
    if(scrollView.contentSize.width < scrollView.bounds.size.width)
        frame.origin.x = (scrollView.bounds.size.width - scrollView.contentSize.width) * 0.5f;
    
    if(scrollView.contentSize.height < scrollView.bounds.size.height)
        frame.origin.y = (scrollView.bounds.size.height - scrollView.contentSize.height) * 0.5f;
    
    self.imageView.frame = frame;
    NSLog(@"%@", NSStringFromCGSize(scrollView.bounds.size));
    NSLog(@"%@", NSStringFromCGSize(scrollView.contentSize));
    NSLog(@"%@", NSStringFromCGPoint(self.imageView.frame.origin));
}

#pragma mark - UIViewController

- (IBAction)perBtnClicked:(UIBarButtonItem *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnClick:(id)sender {
    [self performSegueWithIdentifier:@"showLookbook" sender:self];
//    [self performSegueWithIdentifier:@"test" sender:self];
    NSMutableArray* controllers = [self.navigationController.viewControllers mutableCopy];
    [controllers removeObject:self];
    [self.navigationController setViewControllers:controllers];
    [controllers release];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if([segue.identifier isEqualToString:@"showLookbook"]){
        if([segue.destinationViewController isKindOfClass:[LookBookViewController class]]){
            LookBookViewController* controller = (LookBookViewController*)segue.destinationViewController;
            
            CGPoint clipRectLeftTopPoint = [self.coverView convertPoint:_blankRect.origin toView:self.imageView];
            CGPoint clipRectRightBottomPoint = [self.coverView convertPoint:CGPointMake(CGRectGetMaxX(_blankRect), CGRectGetMaxY(_blankRect))
                                                                     toView:self.imageView];
            
            float screenScale = [UIScreen mainScreen].scale;
            CGRect clipRect = CGRectMake(clipRectLeftTopPoint.x, clipRectLeftTopPoint.y, clipRectRightBottomPoint.x - clipRectLeftTopPoint.x, clipRectRightBottomPoint.y - clipRectLeftTopPoint.y);
            clipRect.origin.x = -clipRect.origin.x;
            clipRect.origin.y = -clipRect.origin.y;
            clipRect.origin.x *= screenScale;
            clipRect.origin.y *= screenScale;
            clipRect.size.width *= screenScale;
            clipRect.size.height *= screenScale;
            
            //clip image
            UIImage* image = [self clipImage:self.imageView.image withRect:clipRect];
            //resize image
            CGSize newImageSize = _blankRect.size;
            newImageSize.width *= screenScale;
            newImageSize.height *= screenScale;
            
            image = [self resizeImage:image WithSize:newImageSize];
            
            controller.image = image;
        }
    }else if([segue.identifier isEqualToString:@"test"]){
        TestViewController1* controller = (TestViewController1*)segue.destinationViewController;
        
        CGPoint clipRectLeftTopPoint = [self.coverView convertPoint:_blankRect.origin toView:self.imageView];
        CGPoint clipRectRightBottomPoint = [self.coverView convertPoint:CGPointMake(CGRectGetMaxX(_blankRect), CGRectGetMaxY(_blankRect))
                                                                 toView:self.imageView];
        float screenScale = [UIScreen mainScreen].scale;
        CGRect clipRect = CGRectMake(clipRectLeftTopPoint.x, clipRectLeftTopPoint.y, clipRectRightBottomPoint.x - clipRectLeftTopPoint.x, clipRectRightBottomPoint.y - clipRectLeftTopPoint.y);
        clipRect.origin.x = -clipRect.origin.x;
        clipRect.origin.y = -clipRect.origin.y;
        clipRect.origin.x *= screenScale;
        clipRect.origin.y *= screenScale;
        clipRect.size.width *= screenScale;
        clipRect.size.height *= screenScale;
        
        
        UIImage* image = [self clipImage:self.imageView.image withRect:clipRect];
        NSLog(@"%@", NSStringFromCGSize(image.size));
        
        image = [self resizeImage:image WithSize:_blankRect.size];
        NSLog(@"%@", NSStringFromCGSize(image.size));
        controller.image = image;
    }
}

- (void)setup{
    _curRequestID  = 0;

    [self.scrollView addSubview:self.imageView];
    NSLog(@"%lu", [self.imageView retainCount]);
    
    self.scrollView.maximumZoomScale = 2.0;
    self.scrollView.minimumZoomScale = 1.0;
    self.scrollView.delegate = self;
//    self.scrollView.zoomScale = 0.2f;
    //cover mask
    self.coverView.userInteractionEnabled = NO;
    self.coverView.alpha = 0.8f;
    
    //button
    [self.button setTitle:@"开始制作" forState:UIControlStateNormal];
}

- (void)setupNavigationBar{
    //navigation
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:[UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0], NSForegroundColorAttributeName, [UIFont systemFontOfSize:18], NSFontAttributeName, nil];
    
    self.navigationController.navigationBar.titleTextAttributes = dict;
    self.navigationController.navigationBar.barTintColor = [UIColor colorWithRed:0.06 green:0.08 blue:0.1 alpha:1.0];
    self.navigationItem.title = @"照片裁剪";
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
    
    CGSize coverSize = self.coverView.bounds.size;
    
//    _ratio     = coverSize.width / 320;
//    _blankRect = CGRectMake((coverSize.width - 290 * _ratio) / 2, 5, 290 * _ratio, 290 * _ratio);
    
    _blankRect.origin = CGPointMake(self.scrollView.frame.origin.x - self.coverView.frame.origin.x, self.scrollView.frame.origin.y - self.coverView.frame.origin.y);
    _blankRect.size = self.scrollView.frame.size;
    
    CAShapeLayer *layer   = [[CAShapeLayer alloc]init];
    [layer autorelease];
    
    CGMutablePathRef path = CGPathCreateMutable();
    //top
    CGPathAddRect(path, nil, CGRectMake(0, 0, coverSize.width, _blankRect.origin.y));
    //left
    CGPathAddRect(path, nil, CGRectMake(0, 0, _blankRect.origin.x, coverSize.height));
    //right
    CGPathAddRect(path, nil, CGRectMake(_blankRect.origin.x + _blankRect.size.width, 0, _blankRect.origin.x, coverSize.height));
    //bottom
    CGPathAddRect(path, nil, CGRectMake(0, _blankRect.origin.y + _blankRect.size.height, coverSize.width, coverSize.height - (_blankRect.origin.y + _blankRect.size.height)));
    
    layer.path = path;
    
    CALayer* preLayer = self.coverView.layer.mask;
    if(preLayer && [preLayer isKindOfClass:[CAShapeLayer class]]){
        CAShapeLayer* preShapeLayer = (CAShapeLayer*)preLayer;
        CGPathRelease(preShapeLayer.path);
    }
    
    self.coverView.layer.mask = layer;
    
    [self requireAndSetImage];
    [self scrollViewDidZoom:self.scrollView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)dealloc {
    if(_asset)
        [_asset release];
    
    if(_imageView){
        [_imageView release];
        _imageView = nil;
    }
    
    [_scrollView release];
    
    [_coverView release];
    //释放coverView中mask的path
    CALayer* preLayer = _coverView.layer.mask;
    if(preLayer && [preLayer isKindOfClass:[CAShapeLayer class]]){
        CAShapeLayer* preShapeLayer = (CAShapeLayer*)preLayer;
        CGPathRelease(preShapeLayer.path);
    }
    
    [_button release];
    
    [super dealloc];
}
@end
