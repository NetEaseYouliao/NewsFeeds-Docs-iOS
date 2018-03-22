//
//  NFNewsDetailView.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFNewsInfo;
@class NFNewsDetailImage;

@protocol NFNewsDetailViewDelegate;

/**
 * @class
 * NFNewsDetailView类
 *
 * @abstract
 * 继承自UIView，用以展示新闻详情页面用
 */
@interface NFNewsDetailView : UIView

/**
 * @property
 *
 * @abstract
 * NFNewsDetailViewDelegate回调
 */
@property (nonatomic, weak) id<NFNewsDetailViewDelegate> delegate;

/**
 * @property
 *
 * @abstract
 * 获取展示新闻详情的webView
 */
@property (nonatomic, strong, readonly) UIWebView *webView;

/**
 * @property
 *
 * @abstract
 * 获取当前分享按钮的显示状态
 */
@property (nonatomic, assign, readonly) BOOL showShareButton;

/**
 *  @method
 *
 *  @abstract
 *  NFNewsDetailView初始化
 *
 *  @param isShowShareButton   是否显示分享按钮,默认NO,如果YES则可实现delegate中的方法进行处理
 *
 *  @return NFNewsDetailView实例
 */
- (instancetype)initWithShowShare:(BOOL)isShowShareButton;

/**
 *  @method
 *
 *  @abstract
 *  开始加载新闻内容
 *
 *  @param newsInfo  待加载详情的新闻
 *
 *  @discussion
 *  在此接口调用前，需要先初始化实例
 */
- (void)startLoadContentWithNews:(NFNewsInfo *)newsInfo;

/**
 *  @method
 *
 *  @abstract
 *  获取图片的frame
 *
 *  @param index  图片的索引值
 *  @param block  返回的结果
 *
 *  @discussion
 *  在此接口调用前，需要先初始化实例
 */
- (void)frameWithIndex:(NSInteger)index block:(void (^)(CGRect frame))block;

@end



/**
 * @abstract
 * 新闻详情页面的回调
 */
@protocol  NFNewsDetailViewDelegate <NSObject>
@optional
/**
 *  @method
 *
 *  @abstract
 *  新闻内容获取成功，即将开始展示
 */
- (void)detailViewWillLoadContent:(NFNewsDetailView *)detailView;

/**
 *  @method
 *
 *  @abstract
 *  新闻内容展示成功
 */
- (void)detailViewDidLoadContent:(NFNewsDetailView *)detailView;

/**
 *  @method
 *
 *  @abstract
 *  新闻内容展示失败
 *
 *  @param error  错误信息
 */
- (void)detailView:(NFNewsDetailView *)detailView didFailToLoadContentWithError:(NSError *)error;

/**
 *  @method
 *
 *  @abstract
 *  点击新闻内容相关推荐
 *
 *  @param newsInfo 点击的新闻
 */
- (void)detailView:(NFNewsDetailView *)detailView didClickNews:(NFNewsInfo *)newsInfo;

/**
 *  @method
 *
 *  @abstract
 *  点击新闻内容中图片
 *
 *  @param imageArray 图片集合
 *  @param index      当前点击图片下标
 *  @param frame      点击图片的frame
 */
- (void)detailView:(NFNewsDetailView *)detailView didClickImageArray:(NSArray <NFNewsDetailImage *>*)imageArray atIndex:(NSUInteger)index frame:(CGRect)frame;

/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错页面的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param issueDescription issue相关描述
 */
- (void)onIssueReport:(NFNewsDetailView *)detailView
            issueDesc:(NSString *)issueDescription;

/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错完成返回到主页的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 */
- (void)onIssueReportFinished:(NFNewsDetailView *)detailView;

@end
