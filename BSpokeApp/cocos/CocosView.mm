//
//  CocosView.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/4.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "CocosView.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGlView-ios.h"
#import "SceneManager.h"

static bool s_isMakeSureSize  = false;
static CGSize s_cocosViewSize = CGSizeMake(0, 0);

@interface CocosView()

@property (assign, nonatomic) CCEAGLView* eaglView;

@end

@implementation CocosView{
    EffectScene* _effectScene;
    ArrangeScene* _arrangeScene;
    LookbookScene* _lookbookScene;
    LeggingPreviewScene* _leggingPreviewScene;
    
    BOOL _isLazyCreate;
}

- (CCEAGLView*)eaglView{
    if(_eaglView == nil){
        
        Application* app = cocos2d::Application::getInstance();
        CCEAGLView* view = (CCEAGLView*)app->getEAGlView();
        
        if(!view){
            CGRect superViewFrame = self.bounds;
            superViewFrame.size = s_cocosViewSize;
            
            view = (CCEAGLView*)app->createEAGlView(superViewFrame.origin.x, superViewFrame.origin.y, superViewFrame.size.width, superViewFrame.size.height);
        }
        _eaglView = view;
        [_eaglView retain];
        
        NSLog(@"%lu", (unsigned long)_eaglView.retainCount);
    }
    
    return _eaglView;
}

- (void)awakeFromNib{
    [super awakeFromNib];
    
    if(s_isMakeSureSize){
        [self addSubview:self.eaglView];
    }
}

- (void)updateGlobalCocosViewSize{
    s_cocosViewSize  = self.bounds.size;
    s_isMakeSureSize = true;
}

- (void)appear{
    NSLog(@"cocosView appear");
//    [self addSubview:self.eaglView];

    [self addSubview:self.eaglView];
    NSLog(@"%lu", (unsigned long)_eaglView.retainCount);
    
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
}

- (void)disappear{
    NSLog(@"cocosView disappear");
    
    Director::getInstance()->pause();
    Director::getInstance()->stopAnimation();
    
    [self.eaglView removeFromSuperview];
    NSLog(@"%lu", (unsigned long)_eaglView.retainCount);
}

- (void)destroyEAGLView{
    Director::getInstance()->end();
    Director::getInstance()->mainLoop();
    
    s_isMakeSureSize = false;
}

- (void)setTouchDelegate:(SEL)delegate WithTarget:(id)target{
    _cocosViewDelegate = delegate;
    _delegateTarget    = target;
}

- (void)setLookbookSceneDelegate:(SEL)delegate WithTarget:(id)target{
    _lookbookSceneDelegate = delegate;
    _delegateTarget = target;
}

- (void)setEffectSceneDelegate:(SEL)delegate WithTarget:(id)target{
    _effectSceneDelegate = delegate;
    _delegateTarget = target;
}

- (void)setArrangeSceneBtnClickedDelegate:(SEL)delegate WithTarget:(id)target{
    _arrangeSceneBtnClickedDelegate = delegate;
    _delegateTarget = target;
}

- (void)setArrangeSceneItemClickedDelegate:(SEL)delegate WithTarget:(id)target{
    _arrangeSceneItemClickedDelegate = delegate;
    _delegateTarget = target;
}

- (void)setArrangeSceneItemSelectStateDelegate:(SEL)delegate WithTarget:(id)target{
    _arrangeSceneItemSelectStateDelegate = delegate;
    _delegateTarget = target;
}

- (void)enterEffectScene{
    SceneManager::getInstance()->enterEffectScene();
    
    _effectScene = SceneManager::getInstance()->getEffectScene();
    _effectScene->setOnEffectActionCallback([=](bool isBegin){
        if(self.delegateTarget != nil && self.effectSceneDelegate != nil){
            BOOL b = isBegin ? YES : NO;
            
            [self.delegateTarget performSelector:self.effectSceneDelegate withObject:@{@"isBegin":[NSNumber numberWithBool:b]}];
        }
    });
}

- (void)saveEffectInEffectScene:(BOOL)isToArrange{
    SceneManager::getInstance()->saveEffectTexture(isToArrange);
}

- (void)leaveAndDestroyEffectScene:(BOOL)isDelay{
    SceneManager::getInstance()->leaveEffectScene();
    if(isDelay)
        SceneManager::getInstance()->setDelayDestroyEffectScene();
    else
        SceneManager::getInstance()->destroyEffectScene();

    _effectScene = NULL;
}

- (void)enterArrangeScene{
    SceneManager::getInstance()->enterArrangeScene();
    
    _arrangeScene = SceneManager::getInstance()->getArrangeScene();
    _arrangeScene->setOnTouchDelegate([=](){
        if(self.delegateTarget != nil && self.cocosViewDelegate != nil){
            [self.delegateTarget performSelector:self.cocosViewDelegate];
        }
    });
    _arrangeScene->setOnArrangeBtnClickedCallback([=](){
        if(self.delegateTarget != nil && self.arrangeSceneBtnClickedDelegate){
            [self.delegateTarget performSelector:self.arrangeSceneBtnClickedDelegate];
        }
    });
    _arrangeScene->setOnArrangeItemClickedCallback([=](){
        if(self.delegateTarget != nil && self.arrangeSceneItemClickedDelegate){
            [self.delegateTarget performSelector:self.arrangeSceneItemClickedDelegate];
        }
    });
    _arrangeScene->setOnArrangeItemSelectStateCallback([=](bool isSelected){
        if(self.delegateTarget != nil && self.arrangeSceneItemSelectStateDelegate){
            BOOL b = isSelected ? YES : NO;
            
            [self.delegateTarget performSelector:self.arrangeSceneItemSelectStateDelegate withObject:@{@"isSelected": [NSNumber numberWithBool:b]}];
        }
    });
}

- (void)leaveAndDestroyArrangeScene{
    SceneManager::getInstance()->leaveArrangeScene();
    SceneManager::getInstance()->destroyArrangeScene();
    _arrangeScene = NULL;
}

- (void)enterLookbookScene{
    SceneManager::getInstance()->enterLookbookScene();
    
    _lookbookScene = SceneManager::getInstance()->getLookbookScene();
    _lookbookScene->setOnTouchDelegate([=](){
        if(self.delegateTarget && self.cocosViewDelegate){
            [self.delegateTarget performSelector:self.cocosViewDelegate];
        }
    });
    _lookbookScene->setOnItemFocusDelegate([=](int shadowType, float opaque){
        if(self.delegateTarget && self.lookbookSceneDelegate){
            [self.delegateTarget performSelector:self.lookbookSceneDelegate withObject:@{@"shadowType":[NSNumber numberWithInt:shadowType], @"opaque":[NSNumber numberWithFloat:opaque]}];
        }
    });
}

- (void)leaveAndDestroyLookbookScene{
    SceneManager::getInstance()->leaveLookbookScene();
    SceneManager::getInstance()->destroyLookbookScene();
}

- (void)enterLegggingPreviewScene{
    SceneManager::getInstance()->enterLeggingPreviewScene();
    _leggingPreviewScene = SceneManager::getInstance()->getLeggingPreviewScene();
}

- (void)leaveLeggingPreviewScene{
    SceneManager::getInstance()->leaveLeggingPreviewScene();
    SceneManager::getInstance()->destroyLeggingPreviewScene();
    _leggingPreviewScene = NULL;
}

- (void)applyPattern:(int)patternId{
    if(_effectScene)
        _effectScene->setPattern(patternId);
}

- (void)applyPatternFromArrangeItem{
    SceneManager::getInstance()->setEffectTextureFromArrange();
}

- (void)applyPatternFromLookbookItem{
    SceneManager::getInstance()->setEffectTextureFromLookbook();
}

- (void)applyEffect:(int)type{
    if(_effectScene)
        _effectScene->setEffect(type);
}

- (void)applyColor:(UIColor*)color{
    cocos2d::Color3B cocosColor;
    CGFloat r = 0, g = 0, b = 0, a = 0;
    
    [color getRed:&r green:&g blue:&b alpha:&a];
    cocosColor.r = r * 255;
    cocosColor.g = g * 255;
    cocosColor.b = b * 255;
    
    if(_effectScene)
        _effectScene->setColor(cocosColor);
}

- (void)pauseEffect{
    if(_effectScene)
        _effectScene->pauseEffect();
}

- (void)resumeEffect{
    if(_effectScene)
        _effectScene->resumeEffect();
}

- (void)resetEffect{
    if(_effectScene)
        _effectScene->resetEffect();
}

- (void)randomRunEffect{
    if(_effectScene)
        _effectScene->randomRunEffect();
}

- (void)applySelectedItemPattern:(int)patternId{
    if(_arrangeScene)
        _arrangeScene->setSelectedItemPattern(patternId);
}

- (void)applyArrange:(int)arrangeId{
//    if(_arrangeScene)
//        _arrangeScene->setArrange(arrangeId);
}

- (void)applyLeggingColor:(NSString*)color{
    if(_arrangeScene)
        _arrangeScene->setLeggingStyle(LeggingType_None, [color UTF8String], LeggingOrientation_None);
}

- (void)applyLeggingOrientation:(int)orientation{
    if(_arrangeScene)
        _arrangeScene->setLeggingStyle(LeggingType_None, nil, (LeggingOrientation)orientation);
}

- (void)applyScale:(BOOL)isScale{
    if(_arrangeScene)
        _arrangeScene->setScale(isScale);
}

- (void)applyRecoverSelectedItem{
    if(_arrangeScene)
        _arrangeScene->recoverSelectedItem();
}

- (BOOL)hasLookBookItem{
    if(_lookbookScene)
        return _lookbookScene->hasLookBookItem();
    
    return NO;
}

- (void)cleanLookbookItem{
    if(_lookbookScene)
        return _lookbookScene->cleanItems();
}

- (void)applyBackground:(CGImageRef)data{
    CGImageRef imageRef = data;
    
    NSUInteger width = CGImageGetWidth(imageRef);
    NSUInteger height = CGImageGetHeight(imageRef);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    unsigned long dataLength = height * width * 4;
    unsigned char* rawData = (unsigned char*) calloc(dataLength, sizeof(unsigned char));
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height, bitsPerComponent, bytesPerRow, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    
    Texture2D* tex = new Texture2D();
    tex->autorelease();
    tex->initWithData(rawData, dataLength, Texture2D::PixelFormat::RGBA8888, (int)width, (int)height, cocos2d::Size((float)width, (float)height));
    
    if(rawData)
        free(rawData);
    
    if(_lookbookScene)
        _lookbookScene->setBackground(tex);
}

- (void)applyLookbookItem:(int)itemId{
    if(_lookbookScene)
        _lookbookScene->applyLookbookItem(itemId);
}

- (void)applyShadow:(int)shadowId{
    if(_lookbookScene)
        _lookbookScene->applyShadow(shadowId);
}

- (void)applyOpaque:(float)opaque{
    if(_lookbookScene)
        _lookbookScene->applyOpaque(opaque);
}

- (void)applyLeggingOrientationInPreview:(int)orientation{
    if(_leggingPreviewScene)
        _leggingPreviewScene->updateLeggingStyle(LeggingType_None, nil, (LeggingOrientation)orientation);
}

- (void)dealloc{
    [_eaglView release];
    [super dealloc];
}

@end
