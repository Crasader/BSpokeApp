//
//  MySlideMenuController.m
//  MySlideMenuDemo
//
//  Created by luozhipeng on 15/5/11.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "MySlideMenuController.h"

@interface MySlideMenuController ()

@property (assign, nonatomic) UIPanGestureRecognizer *panGestureRecognizer;

@property CGPoint dragBeginPoint;
@end

@implementation MySlideMenuController{
    CGPoint _beginPoint;
    CGPoint _endPoint;
    CGPoint _directionNormalizePoint;
    CGPoint _curPoint;
}

-(CGPoint)clampPoint:(CGPoint)p InPoint:(CGPoint)p1 AndPoint:(CGPoint)p2{
    float minX = MIN(p1.x, p2.x);
    float maxX = MAX(p1.x, p2.x);
    float minY = MIN(p1.y, p2.y);
    float maxY = MAX(p1.y, p2.y);
    
    CGPoint ret;
    
    ret.x = MAX(MIN(p.x, maxX), minX);
    ret.y = MAX(MIN(p.y, maxY), minY);
    
    return ret;
}

-(void)panDetected:(UIPanGestureRecognizer*)panGestureRecognizer{
    CGPoint point   = [panGestureRecognizer translationInView:self.menuView];
    
    if(panGestureRecognizer.state == UIGestureRecognizerStateBegan){
        self.dragBeginPoint = point;
    }else if(panGestureRecognizer.state == UIGestureRecognizerStateChanged){
        //求出menu移动方向上的偏移
        CGPoint pOffset = CGPointMake(point.x - self.dragBeginPoint.x, point.y - self.dragBeginPoint.y);
        float offset    = pOffset.x * _directionNormalizePoint.x + pOffset.y * _directionNormalizePoint.y;
        
        CGPoint destinate = CGPointMake(_curPoint.x + _directionNormalizePoint.x * offset, _curPoint.y + _directionNormalizePoint.y * offset);
        
        destinate = [self clampPoint:destinate InPoint:_beginPoint AndPoint:_endPoint];
        
        CGRect frame = self.menuView.frame;
        frame.origin = destinate;
        self.menuView.frame = frame;
        _curPoint    = destinate;
        self.dragBeginPoint = point;
        
    }else if(panGestureRecognizer.state == UIGestureRecognizerStateEnded || panGestureRecognizer.state == UIGestureRecognizerStateCancelled){
    
    }
}

-(UIPanGestureRecognizer*)panGestureRecognizer{
    if(_panGestureRecognizer == nil){
        _panGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panDetected:)];
        [_panGestureRecognizer retain];
    }
    
    return _panGestureRecognizer;
}

-(void)setMenuView:(UIView*)view{
    if(_menuView != view){
        if(_menuView != nil){
            [_menuView removeFromSuperview];
            [_menuView removeGestureRecognizer:self.panGestureRecognizer];
        }
    }
    
    _menuView = view;
    
    [_menuView addGestureRecognizer:self.panGestureRecognizer];
    [self.view addSubview:_menuView];
}

-(UIView*)loadMenuViewFromNib:(NSString*)nibName{
    NSArray* views = [[NSBundle mainBundle]loadNibNamed:nibName owner:nil options:nil];
    
    NSAssert([views count] == 1, @"no view or more than 1 view in nibFiles");
    
    self.menuView = [views lastObject];
    
    return self.menuView;
}

-(void)setBeginPoint:(CGPoint)beginPoint AndEndPoint:(CGPoint)endPoint{
    _beginPoint = beginPoint;
    _endPoint   = endPoint;
    //normalize direction
    _directionNormalizePoint = CGPointMake(_endPoint.x - _beginPoint.x, _endPoint.y - _beginPoint.y);
    float n = _directionNormalizePoint.x * _directionNormalizePoint.x + _directionNormalizePoint.y * _directionNormalizePoint.y;
    n = sqrt(n);
    n = 1.0f / n;
    _directionNormalizePoint.x *= n;
    _directionNormalizePoint.y *= n;
    
    //update menuView to begin point;
    CGRect r = self.menuView.frame;
    
    r.origin = beginPoint;
    
    self.menuView.frame = r;
    _curPoint = r.origin;
}

-(void)openMenu{
    if(CGPointEqualToPoint(_curPoint, _endPoint))
        return;
    
    CGRect endFrame = self.menuView.frame;
    endFrame.origin = _endPoint;
    
    [UIView animateWithDuration:1.0
                     animations:^{
                         self.menuView.frame = endFrame;
                     }
                     completion:^(BOOL completed) {
                         _curPoint = endFrame.origin;
                     }];
}

-(void)closeMenu{
    if(CGPointEqualToPoint(_curPoint, _beginPoint))
        return;
    
    CGRect endFrame = self.menuView.frame;
    endFrame.origin = _beginPoint;
    
    [UIView animateWithDuration:1.0
                     animations:^{
                         self.menuView.frame = endFrame;
                     }
                     completion:^(BOOL completed) {
                         _curPoint = endFrame.origin;
                     }];
}

-(void)toggleMenu{
    if(CGPointEqualToPoint(_curPoint, _beginPoint)){
        [self openMenu];
    }else if(CGPointEqualToPoint(_curPoint, _endPoint)){
        [self closeMenu];
    }
}

- (void)dealloc{
    if(_panGestureRecognizer){
        [_panGestureRecognizer release];
        _panGestureRecognizer = nil;
    }
    [super dealloc];
}

@end
