//
//  AppDelegate.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/11.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "AppDelegate.h"
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>

#import "RootViewController.h"
#import "WXApi.h"
#import "WholeViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    [NewsFeedsUISDK startWithAppKey:@"3c11d60d903e49d5a47ad2a58bb0db97" appSecret:@"ca5137e40b874abd893e762f1d53d839" config:nil];
    
    [NewsFeedsUISDK configWith:nil];
    
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:[[RootViewController alloc] init]];
//    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:[[WholeViewController alloc] init]];
    self.window.rootViewController = nav;
    
    [self.window makeKeyAndVisible];
    
    [WXApi startLogByLevel:WXLogLevelDetail logBlock:^(NSString *log) {
        NSLog(@"log : %@", log);
    }];
    
    //向微信注册
    [WXApi registerApp:@"wx46a02c42801af566" enableMTA:YES];
    
    //向微信注册支持的文件类型
    UInt64 typeFlag = MMAPP_SUPPORT_TEXT | MMAPP_SUPPORT_PICTURE | MMAPP_SUPPORT_LOCATION | MMAPP_SUPPORT_VIDEO |MMAPP_SUPPORT_AUDIO | MMAPP_SUPPORT_WEBPAGE | MMAPP_SUPPORT_DOC | MMAPP_SUPPORT_DOCX | MMAPP_SUPPORT_PPT | MMAPP_SUPPORT_PPTX | MMAPP_SUPPORT_XLS | MMAPP_SUPPORT_XLSX | MMAPP_SUPPORT_PDF;
    
    [WXApi registerAppSupportContentFlag:typeFlag];
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

- (UIImage *)imageWithColor:(UIColor *)color {
    CGRect rect = CGRectMake(0, 0, 32, 32);
    
    UIGraphicsBeginImageContextWithOptions(rect.size, false, 1.0);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, color.CGColor);
    CGContextFillRect(context, rect);
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}
@end
