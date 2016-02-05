//
//  AppDelegate.m
//  BSpokeApp
//
//  Created by luozhipeng on 15/4/27.
//  Copyright (c) 2015年 luozhipeng. All rights reserved.
//

#import "AppDelegate.h"
#import "cocos/CocosAppDelegate.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"

static CocosAppDelegate s_cocosAppDelegate;

@interface AppDelegate ()

@end

@implementation AppDelegate

void caughtException(NSException* exception){
    NSLog(@"Crash: %@", exception);
    NSLog(@"stack track: %@", [exception callStackSymbols]);
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    NSSetUncaughtExceptionHandler(&caughtException);
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    cocos2d::GLView* glView = cocos2d::Director::getInstance()->getOpenGLView();
    if(glView){
        CCEAGLView* eaglView = (CCEAGLView*)glView->getEAGLView();
        [eaglView release];
    }
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application{
    NSLog(@"memory Warming!!!");
}

@end
