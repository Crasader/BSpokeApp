//
//  MyUITabBar.m
//  UITabBarDemo
//
//  Created by luozhipeng on 15/5/15.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "MyUITabBar.h"

@implementation MyUITabBarItem

-(instancetype)init{
    self = [super init];
    
    if(self){
        self.userInteractionEnabled = NO;
        self.translatesAutoresizingMaskIntoConstraints = NO;
        self.backgroundColor = [UIColor colorWithRed:0.92 green:0.18 blue:0.37 alpha:1.0];
        
        _isSelect = NO;
    }
    
    return self;
}

-(void)setSelectedView:(UIView*)selectedView{
    if(_selectedView){
        [_selectedView release];
        _selectedView = nil;
    }
    
    _selectedView = selectedView;
    [_selectedView retain];
    
    if(!_isSelect){
        [_selectedView setHidden:YES];
    }
}

-(void)setIsSelect:(BOOL)isSelect{
    if(_isSelect == isSelect)
        return;
    
    _isSelect = isSelect;
    if(_isSelect){
        [_selectedView setHidden:NO];
    }else{
        [_selectedView setHidden:YES];
    }
}

-(void)dealloc{
    if(_selectedView){
        [_selectedView release];
        _selectedView = nil;
    }
    [super dealloc];
}

@end


@implementation MyUITabBar

-(void)clickAction:(id)sender forEvent:(UIEvent *)event{
    NSSet* touches = [event touchesForView:self];
    UITouch *touch = [touches anyObject];
    
    
    [self.items enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        MyUITabBarItem* item = (MyUITabBarItem*)obj;
        BOOL isInside = [item pointInside:[touch locationInView:item] withEvent:nil];
        
        if(isInside){
            [self setSelectedItem:obj];
            *stop = YES;
        }
    }];
}

-(void)setClickEvent{
    [self addTarget:self action:@selector(clickAction:forEvent:) forControlEvents:UIControlEventTouchUpInside];
}

-(void)awakeFromNib{
    [self setClickEvent];
}

-(instancetype)init{
    self = [super init];
    
    if(self){
        [self setClickEvent];
    }
    
    return self;
}

-(void)removeChilds{
    [self.items makeObjectsPerformSelector:@selector(removeFromSuperview)];
}

-(void)addChilds{
    
    __block UIView* preView  = nil;
    
    [self.items enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        BOOL isUIView = [obj isKindOfClass:[MyUITabBarItem class]];
        NSAssert(isUIView, @"wrong item kind in items");
        
        MyUITabBarItem* view = (MyUITabBarItem*)obj;
        
        [self addSubview:view];
        if(idx == 0){
            NSLayoutConstraint *marginHead = [NSLayoutConstraint constraintWithItem:view
                                                                          attribute:NSLayoutAttributeLeading
                                                                          relatedBy:NSLayoutRelationEqual
                                                                             toItem:self
                                                                          attribute:NSLayoutAttributeLeading
                                                                         multiplier:1.0
                                                                           constant:0];
            [self addConstraint:marginHead];
        }
        
        if(preView != nil){
            NSLayoutConstraint *marginPre = [NSLayoutConstraint constraintWithItem:view
                                                                          attribute:NSLayoutAttributeLeading
                                                                          relatedBy:NSLayoutRelationEqual
                                                                             toItem:preView
                                                                          attribute:NSLayoutAttributeTrailing
                                                                         multiplier:1.0
                                                                           constant:0];
            [self addConstraint:marginPre];
            
            NSLayoutConstraint *equalWidth = [NSLayoutConstraint constraintWithItem:view
                                                                           attribute:NSLayoutAttributeWidth
                                                                           relatedBy:NSLayoutRelationEqual
                                                                              toItem:preView
                                                                           attribute:NSLayoutAttributeWidth
                                                                          multiplier:1.0
                                                                            constant:0];
            [self addConstraint:equalWidth];
        }
        
        NSLayoutConstraint *heightContraint = [NSLayoutConstraint constraintWithItem:view
                                                                      attribute:NSLayoutAttributeHeight
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeHeight
                                                                     multiplier:1.0
                                                                       constant:0];
        [self addConstraint:heightContraint];
        
        NSLayoutConstraint *vertiseContraint = [NSLayoutConstraint constraintWithItem:view
                                                                           attribute:NSLayoutAttributeCenterY
                                                                           relatedBy:NSLayoutRelationEqual
                                                                              toItem:self
                                                                           attribute:NSLayoutAttributeCenterY
                                                                          multiplier:1.0
                                                                            constant:0];
        [self addConstraint:vertiseContraint];
        
        preView = view;
    }];
    
    if(preView != nil){
        NSLayoutConstraint *marginFoot = [NSLayoutConstraint constraintWithItem:preView
                                                                      attribute:NSLayoutAttributeTrailing
                                                                      relatedBy:NSLayoutRelationEqual
                                                                         toItem:self
                                                                      attribute:NSLayoutAttributeTrailing
                                                                     multiplier:1.0
                                                                       constant:0];
        [self addConstraint:marginFoot];
    }
}

-(void)setItems:(NSArray*)items{
    [self removeChilds];
    
    _items = [items copy];
    NSLog(@"%lu", [_items retainCount]);
    
    [self addChilds];
}

-(void)setSelectedItem:(MyUITabBarItem *)selectedItem{
    NSUInteger index = [_items indexOfObject:selectedItem];
    
    NSAssert(index != NSNotFound, @"wrong selected item because it not in items");
    
    
    if(_selectedItem == nil){
        [_items makeObjectsPerformSelector:@selector(setIsSelect:) withObject:@(NO)];
    }else{
        [_selectedItem setIsSelect:NO];
    }
    
    _selectedItem = selectedItem;
    [_selectedItem setIsSelect:YES];
    
    [self.delegate tabBar:self didSelectItem:_selectedItem];
}

- (void)dealloc{
    if(_items){
        [_items release];
    }
    [super dealloc];
}

@end
