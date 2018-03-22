//
//  ViewController.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/11.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "WholeViewController.h"
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>
#import <NewsFeedsSDK/NewsFeedsSDK.h>
#import "WXApiRequestHandler.h"
#import <SDWebImage/SDImageCache.h>
#import <SDWebImage/SDWebImageManager.h>
#import <CommonCrypto/CommonCrypto.h>

@interface WholeViewController () <NFeedsViewDelegate, NewsFeedsUISDKDelegate>

@end

@implementation WholeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    //测试用
    self.title = @"信息流demo";
    
    //设置广告的presentViewController
    [[NewsFeedsSDK sharedInstance] setAdPresentController:self];
    
    [NewsFeedsUISDK setDelegate:self];
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self config:nil  extraData:@"whole"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self.navigationController.navigationBar setBarTintColor:[UIColor  colorWithRed:59/255.0 green:155/255.0 blue:253/255.0 alpha:1]];
    [self.navigationController.navigationBar setTitleTextAttributes:[NSDictionary dictionaryWithObjectsAndKeys: [UIColor whiteColor], NSForegroundColorAttributeName, [UIFont fontWithName:@"PingFangSC-Medium" size:19], NSFontAttributeName, nil]];
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)onShareClick:(NSDictionary *)shareInfo
                   type:(NSInteger)type {
    NSMutableString *url = [shareInfo[@"shareUrl"] mutableCopy];
    [url appendFormat:@"&iou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];
    [url appendFormat:@"&aou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];
    [WXApiRequestHandler sendLinkURL:url
                             TagName:shareInfo[@"infoType"]
                               Title:shareInfo[@"title"]
                         Description:shareInfo[@"summary"] ?  : shareInfo[@"source"]
                          ThumbImage:shareInfo[@"thumbnail"]
                             InScene:type];

}

- (NSString *)encodeParameter:(NSString *)originalPara {
    CFStringRef encodeParaCf = CFURLCreateStringByAddingPercentEscapes(NULL, (__bridge CFStringRef)originalPara, NULL, CFSTR("!*'();:@&=+$,/?%#[]"), kCFStringEncodingUTF8);
    NSString *encodePara = (__bridge NSString *)(encodeParaCf);
    CFRelease(encodeParaCf);
    return encodePara;
}

@end
