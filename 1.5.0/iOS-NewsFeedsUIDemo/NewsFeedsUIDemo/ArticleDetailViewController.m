//
//  ArticleDetailViewController.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "ArticleDetailViewController.h"
#import <NewsFeedsSDK/NewsFeedsSDK.h>
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>

@interface ArticleDetailViewController () <NFArticleDetailViewDelegate,NFArticleGalleryDelegate>

@property (nonatomic, strong) NFNewsInfo *newsInfo;
@property (nonatomic, strong) NFArticleDetailView *articledetailView;

@property (nonatomic, strong) NSArray *imageArray;

@end

@implementation ArticleDetailViewController

- (instancetype)initWithNews:(NFNewsInfo *)newsInfo {
    if (self = [super init]) {
        _newsInfo = newsInfo;
    }
    
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _articledetailView = [NewsFeedsUISDK createArticleDetailView:_newsInfo delegate:self extraData:@"articledetailView"];
    [self.view addSubview:_articledetailView];
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    
    _articledetailView.frame = self.view.bounds;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


#pragma mark - delegate.

- (void)relatedNewsDidSelect:(NFArticleDetailView *)detailView
                    newsInfo:(NFNewsInfo *)newsInfo
                   extraData:(id)extraData {
    ArticleDetailViewController *detailViewController = [[ArticleDetailViewController alloc] initWithNews:newsInfo];
    [self.navigationController pushViewController:detailViewController animated:YES];
    
}

- (void)webImageDidSelect:(NFArticleDetailView *)detailView
               imageArray:(NSArray<NFNewsDetailImage *> *)imageArray
                  atIndex:(NSUInteger)index
                    frame:(CGRect)frame
                extraData:(id)extraData {
    _imageArray = [imageArray copy];
    
    NFArticleGalleryViewController *browserVC = [NewsFeedsUISDK createArticleGalleryViewController:imageArray imageIndex:index delegate:self extraData:@"ArticleGalleryViewController"];

    [self presentViewController:browserVC animated:YES completion:nil];
}

- (void)articleDidLoadContent:(NFArticleDetailView *)detailView extraData:(id)extraData {
    __weak typeof(self)weakSelf = self;

    [[NewsFeedsSDK sharedInstance] hasRead:self.newsInfo.infoId block:^(BOOL hasRead) {
        if (!hasRead) {
            [[NewsFeedsSDK sharedInstance] markRead:weakSelf.newsInfo.infoId];
            if ([weakSelf.delegate respondsToSelector:@selector(markRead)]) {
                [weakSelf.delegate markRead];
            }
        }
    }];
}

- (void)onIssueReport:(NFArticleDetailView *)detailView issueDesc:(NSString *)issueDescription extraData:(id)extraData {
    
}

- (void)onIssueReportFinished:(NFArticleDetailView *)detailView extraData:(id)extraData {
    
}

//- (void)onWebShareClick:(NFArticleDetailView *)detailView
//                   newsDetail:(NFNewsDetail *)detail
//                   type:(NSInteger)type
//              extraData:(id)extraData {
//
//}

- (void)back:(NFArticleGalleryViewController *)browserController extraData:(id)extraData {
    [browserController dismissViewControllerAnimated:YES completion:nil];
}

@end
