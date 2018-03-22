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
@class NFNewsDetail;
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


/**
 *  @method
 *
 *  @abstract
 *  详情页面的分享按钮点击
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param shareInfo        分享需要用到的字段集合
 *  @param type             分享类型，0：微信好友  1：朋友圈
 *  @param extraData        用户的自定义字段
 */
- (void)onWebShareClick:(NFArticleDetailView *)detailView
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData;

@end

@interface NFArticleDetailView : UIView

/**
 * @property
 *
 * @abstract
 * 获取当前新闻详情的detail。 如果页面为加载完成，此项为空。
 */
@property (nonatomic, strong) NFNewsDetail *newsDetail;

/**
 *  @method
 *
 *  @abstract
 *  调用trackArticleBrowseEnd接口，新闻详情浏览结束
 *
 *  @discussion
 *  该接口方便用户在使用SDK提供的NFArticleDetailView展示新闻详情时，无需计算progress
 *  调用该接口会自动计算出progress
 *
 *  可以不使用该接口，调用trackBrowseBegin：和trackBrowseEnd：progress：配合使用代替此接口
 *
 *  注意：使用该接口无需与trackBrowseBegin配合使用，SDK会在文章加载成功后自动调用
 */
- (void)trackArticleBrowseEnd;

@end
