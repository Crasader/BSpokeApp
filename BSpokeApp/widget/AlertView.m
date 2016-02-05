//
//  AlertView.m
//  ModalSegueDemo
//
//  Created by luozhipeng on 15/6/7.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "AlertView.h"

@interface AlertView()
@property (assign, nonatomic) IBOutlet UIButton *yesBtn;
@property (assign, nonatomic) IBOutlet UIButton *noBtn;

@end

@implementation AlertView

- (IBAction)btnClicked:(UIButton *)sender {
    if(sender == self.yesBtn){
        [self.delegate onYesButtonClicked:self];
    }else{
        [self.delegate onNoButtonClicked:self];
    }
}

@end
