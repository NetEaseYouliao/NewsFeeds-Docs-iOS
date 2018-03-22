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
    
//    [self.navigationController.navigationBar setBarStyle:UIBarStyleDefault];
    
    //设置广告的presentViewController
    [[NewsFeedsSDK sharedInstance] setAdPresentController:self];
    
    [NewsFeedsUISDK setDelegate:self];
    
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"whole"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self.navigationController.navigationBar setBarTintColor:[UIColor  colorWithRed:59/255.0 green:155/255.0 blue:253/255.0 alpha:1]];
    [self.navigationController.navigationBar setTitleTextAttributes:[NSDictionary dictionaryWithObjectsAndKeys: [UIColor whiteColor], NSForegroundColorAttributeName, [UIFont fontWithName:@"PingFangSC-Medium" size:19], NSFontAttributeName, nil]];
    
    [self.navigationController.navigationBar setBarStyle:UIBarStyleBlack];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)onShareClick:(NSDictionary *)shareInfo
                   type:(NSInteger)type {
    NSMutableDictionary *userInfo = [@{} mutableCopy];
    [userInfo setObject:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]] forKey:@"iOSOpenUrl"];
    [userInfo setObject:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]] forKey:@"androidOpenUrl"];

    NSString *newsImageUrl = shareInfo[@"thumbnail"];
    UIImage *thumbnail = nil;
    if (newsImageUrl) {
        thumbnail = [[SDImageCache sharedImageCache] imageFromCacheForKey: [[SDWebImageManager sharedManager] cacheKeyForURL:[NSURL URLWithString:newsImageUrl]]];
    }

    NSString *baseUrl;

    baseUrl = @"https://youliao.163yun.com";

    NSMutableString *url = [[NSString stringWithFormat:@"%@/h5/index.html#/info", baseUrl] mutableCopy];

    NSMutableDictionary *params = [@{} mutableCopy];
    [params setValuesForKeysWithDictionary:userInfo];
    [params setObject:@"1" forKey:@"fss"];

    [params setObject:@"3c11d60d903e49d5a47ad2a58bb0db97" forKey:@"appkey"];
    [params setObject:@"ca5137e40b874abd893e762f1d53d839" forKey:@"secretkey"];

    [params setObject:[[NSNumber numberWithLongLong:[[NSDate date] timeIntervalSince1970] * 1000] stringValue] forKey:@"timestamp"];
    [params setObject:shareInfo[@"infoId"] forKey:@"infoid"];
    [params setObject:shareInfo[@"infoType"] forKey:@"infotype"];
    [params setObject:shareInfo[@"producer"] forKey:@"producer"];
    [params setObject:@"2" forKey:@"platform"];
    [params setObject:@"2" forKey:@"attachPlatform"];
    [params setObject:[NewsFeedsUISDK version] forKey:@"version"];
    [params setObject:shareInfo[@"source"] forKey:@"source"];
    NSMutableString *signature = [@"" mutableCopy];
    NSMutableString *queryString = [@"" mutableCopy];

    for (NSString *key in params) {
        NSString *value = params[key];
        [signature appendFormat:@"%@%@", key, value];
        if ([queryString isEqualToString:@""]) {
            [queryString appendFormat:@"%@=%@", key, [self encodeParameter:value]];
        } else {
            [queryString appendFormat:@"&%@=%@", key, [self encodeParameter:value]];
        }
    }
    [queryString appendFormat:@"&signature=%@", [self md5:signature]];

    [url appendFormat:@"?%@", queryString];

    [WXApiRequestHandler sendLinkURL:url
                             TagName:shareInfo[@"infoType"]
                               Title:shareInfo[@"title"]
                         Description:shareInfo[@"summary"] ?  : shareInfo[@"source"]
                          ThumbImage:thumbnail
                             InScene:type];

}

- (NSString *)md5:(NSString *)str {
    const char *cStr = [str UTF8String];
    unsigned char result[16];
    CC_MD5(cStr, (unsigned int)strlen(cStr), result); // This is the md5 call
    return [NSString stringWithFormat:
            @"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
}

- (NSString *)encodeParameter:(NSString *)originalPara {
    CFStringRef encodeParaCf = CFURLCreateStringByAddingPercentEscapes(NULL, (__bridge CFStringRef)originalPara, NULL, CFSTR("!*'();:@&=+$,/?%#[]"), kCFStringEncodingUTF8);
    NSString *encodePara = (__bridge NSString *)(encodeParaCf);
    CFRelease(encodeParaCf);
    return encodePara;
}

@end
