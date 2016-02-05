//
//  CocosView.h
//  BSpokeApp
//
//  Created by luozhipeng on 15/5/4.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CocosView : UIView

@property (readonly,nonatomic) SEL cocosViewDelegate;
@property (readonly,nonatomic) SEL lookbookSceneDelegate;
@property (readonly,nonatomic) SEL effectSceneDelegate;
@property (readonly,nonatomic) SEL arrangeSceneBtnClickedDelegate;
@property (readonly,nonatomic) SEL arrangeSceneItemClickedDelegate;
@property (readonly,nonatomic) SEL arrangeSceneItemSelectStateDelegate;

@property (assign, nonatomic) id delegateTarget;

- (void)updateGlobalCocosViewSize;

- (void)appear;
- (void)disappear;

- (void)destroyEAGLView;

- (void)setTouchDelegate:(SEL)delegate WithTarget:(id)target;
- (void)setLookbookSceneDelegate:(SEL)delegate WithTarget:(id)target;
- (void)setEffectSceneDelegate:(SEL)delegate WithTarget:(id)target;

- (void)setArrangeSceneBtnClickedDelegate:(SEL)delegate WithTarget:(id)target;
- (void)setArrangeSceneItemClickedDelegate:(SEL)delegate WithTarget:(id)target;
- (void)setArrangeSceneItemSelectStateDelegate:(SEL)delegate WithTarget:(id)target;

- (void)saveEffectInEffectScene:(BOOL)isToArrange;
- (void)enterEffectScene;
- (void)leaveAndDestroyEffectScene:(BOOL)isDelay;

- (void)enterArrangeScene;
- (void)leaveAndDestroyArrangeScene;

- (void)enterLookbookScene;
- (void)leaveAndDestroyLookbookScene;

- (void)enterLegggingPreviewScene;
- (void)leaveLeggingPreviewScene;
/**
    effect scene method
 */
- (void)applyPattern:(int)patternId;
- (void)applyPatternFromArrangeItem;
- (void)applyPatternFromLookbookItem;
- (void)applyEffect:(int)type;
- (void)applyColor:(UIColor*)color;
- (void)pauseEffect;
- (void)resumeEffect;
- (void)resetEffect;
- (void)randomRunEffect;

/**
    arrange scene method
 */
- (void)applySelectedItemPattern:(int)patternId;
- (void)applyArrange:(int)arrangeId;
- (void)applyLeggingColor:(NSString*)color;
- (void)applyLeggingOrientation:(int)orientation;
- (void)applyScale:(BOOL)isScale;
- (void)applyRecoverSelectedItem;

/**
    lookbook scene method
 */
- (BOOL)hasLookBookItem;
- (void)cleanLookbookItem;
- (void)applyBackground:(CGImageRef)data;
- (void)applyLookbookItem:(int)itemId;
- (void)applyShadow:(int)shadowId;
- (void)applyOpaque:(float)opaque;

/**
    leggingPreview scene method
 */
- (void)applyLeggingOrientationInPreview:(int)orientation;

@end
