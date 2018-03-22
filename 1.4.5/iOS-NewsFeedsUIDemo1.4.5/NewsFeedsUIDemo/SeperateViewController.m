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
