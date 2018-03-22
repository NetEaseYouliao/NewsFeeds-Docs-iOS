//
//  SeperateViewController.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "SeperateViewController.h"
#import <NewsFeedsSDK/NewsFeedsSDK.h>
#import "ArticleDetailViewController.h"
#import "WXApiRequestHandler.h"
#import <SDWebImage/SDImageCache.h>
#import <SDWebImage/SDWebImageManager.h>
#import <CommonCrypto/CommonCrypto.h>

@interface SeperateViewController () <NFeedsViewDelegate, NewsMarkReadDelegate,NFPicSetGalleryDelegate,NFVideoBrowserDelegate,NewsFeedsUISDKDelegate>

@property (nonatomic, strong) NFeedsView *feedsView;

@end

@implementation SeperateViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    //设置广告的presentViewController
    [[NewsFeedsSDK sharedInstance] setAdPresentController:self];

    [NewsFeedsUISDK setDelegate:self];

    _feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"seperate"];
    _feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:_feedsView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - NFeedsViewDelegate.

- (void)newsListDidSelectNews:(NFeedsView *)pageControllerView newsInfo:(NFNewsInfo *)newsInfo extraData:(id)extraData {
    if ([newsInfo.infoType isEqualToString:@"article"]) {
        ArticleDetailViewController *detailVC = [[ArticleDetailViewController alloc] initWithNews:newsInfo];
        detailVC.delegate = self;
        [self.navigationController pushViewController:detailVC animated:YES];
    } else if([newsInfo.infoType isEqualToString:@"picset"]) {
        NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
        
        [self.navigationController pushViewController:browserVC animated:YES];
    } else if ([newsInfo.infoType isEqualToString:@"video"]) {
        NFVideoBrowserViewController *videoBrowserVC = [NewsFeedsUISDK createVideoBrowserViewController:newsInfo delegate:self extraData:@"VideoBrowserViewController"];
        [self.navigationController pushViewController:videoBrowserVC animated:YES];
    }
}

#pragma mark - NewsMarkReadDelegate.

- (void)markRead {
    [_feedsView markReadNews];
}


#pragma mark - NFPicSetGalleryDelegate.

- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
     [_feedsView markReadNews];
}

- (void)back:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
    [browserController.navigationController popViewControllerAnimated:YES];
}

- (void)relatedNewsDidSelect:(NFPicSetGalleryViewController *)browserController newsInfo:(NFNewsInfo *)newsInfo extraData:(id)extraData {
    NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
    
    [browserController.navigationController pushViewController:browserVC animated:YES];
}


#pragma mark - NFVideoBrowserDelegate.

- (void)videoDidLoadContent:(NFVideoBrowserViewController *)browserController extraData:(id)extraData {
    [_feedsView markReadNews];
}

- (void)videoBack:(NFVideoBrowserViewController *)browserController extraData:(id)extraData {
    [browserController.navigationController popViewControllerAnimated:YES];
}

#pragma mark - share
- (void)onShareClick:(NSDictionary *)shareInfo
                   type:(NSInteger)type {
    NSMutableDictionary *userInfo = [@{} mutableCopy];
    [userInfo setObject:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]] forKey:@"iOSOpenUrl"];
    //        [userInfo setObject:@"https://itunes.apple.com/app/id893031254" forKey:@"iOSDownUrl"];
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
