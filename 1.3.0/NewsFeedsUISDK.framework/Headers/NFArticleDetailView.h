//
//  NFArticleDetailView.h
//  NewsFeedsUISDK
//
//  Created by shoulei ma on 2017/10/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFArticleDetailView;
@class NFNewsInfo;
@class NFNewsDetailImage;

/**
 *  @protocol
 *
 *  @abstract
 *  新闻详情的回调
 */
@protocol NFArticleDetailViewDelegate <NSObject>
@optional
/**
 *  @method
 *
 *  @abstract
 *  相关推荐新闻的点击回调
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param newsInfo      点击的新闻
 *  @param extraData     用户的自定义字段
 */
- (void)relatedNewsDidSelect:(NFArticleDetailView *)detailView
                    newsInfo:(NFNewsInfo *)newsInfo
                   extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  新闻正文中图片点击
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param imageArray    图片集合
 *  @param index         当前点击图片下标
 *  @param frame         点击图片的frame
 *  @param extraData     用户的自定义字段
 */
- (void)webImageDidSelect:(NFArticleDetailView *)detailView
               imageArray:(NSArray <NFNewsDetailImage *>*)imageArray
                  atIndex:(NSUInteger)index
                    frame:(CGRect)frame
                extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  文章新闻详情加载成功
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param extraData     用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时，
 *  调用NewsFeedsSDK的markRead接口将新闻标记为已读，同时刷新列表已读状态
 */
- (void)articleDidLoadContent:(NFArticleDetailView *)detailView
                    extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错页面的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param issueDescription issue相关描述
 *  @param extraData        用户的自定义字段
 */
- (void)onIssueReport:(NFArticleDetailView *)detailView
            issueDesc:(NSString *)issueDescription
            extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错完成返回到主页的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param extraData        用户的自定义字段
 */
- (void)onIssueReportFinished:(NFArticleDetailView *)detailView
                    extraData:(id)extraData;

@end

@interface NFArticleDetailView : UIView

@end
