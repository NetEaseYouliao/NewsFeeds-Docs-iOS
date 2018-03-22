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

@interface SeperateViewController () <NFeedsViewDelegate, NewsMarkReadDelegate,NFPicSetGalleryDelegate>

@property (nonatomic, strong) NFeedsView *feedsView;

@end

@implementation SeperateViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    //设置广告的presentViewController
    [[NewsFeedsSDK sharedInstance] setAdPresentController:self];

    _feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"seperate"];
    _feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:_feedsView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - NFMainPageControllerViewDelegate.

- (void)newsListDidSelectNews:(NFeedsView *)pageControllerView newsInfo:(NFNewsInfo *)newsInfo extraData:(id)extraData {
    if ([newsInfo.infoType isEqualToString:@"article"]) {
        ArticleDetailViewController *detailVC = [[ArticleDetailViewController alloc] initWithNews:newsInfo];
        detailVC.delegate = self;
        [self.navigationController pushViewController:detailVC animated:YES];
    } else if([newsInfo.infoType isEqualToString:@"picset"]) {
        NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
        
        [self.navigationController pushViewController:browserVC animated:YES];
    }
}

#pragma mark - NewsMarkReadDelegate.

- (void)markRead {
    [_feedsView markReadNews];
}


#pragma mark - NFPicBrowserDelegate.

- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
     [_feedsView markReadNews];
}

- (void)back:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)relatedNewsDidSelect:(NFPicSetGalleryViewController *)browserController newsInfo:(NFNewsInfo *)newsInfo extraData:(id)extraData {
    NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
    
    [browserController.navigationController pushViewController:browserVC animated:YES];
}

@end
