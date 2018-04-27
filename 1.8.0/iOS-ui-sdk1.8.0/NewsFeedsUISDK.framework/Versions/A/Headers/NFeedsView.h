//
//  NFView.h
//  NewsFeedsUISDK
//
//  Created by shoulei ma on 2017/10/11.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MultiScrollTableView.h"

@class NFeedsView;
@class NFNewsInfo;
@class NFChannelInfo;
@protocol NFNewsMarkReadDelegate;

/**
 *  @protocol
 *
 *  @abstract
 *  新闻列表的点击回调
 */
@protocol NFeedsViewDelegate <NSObject>

/**
 *  @method
 *
 *  @abstract
 *  新闻列表的点击回调
 *
 *  @param feedsView    当前的NFeedsView实例
 *  @param newsInfo     点击的新闻
 *  @param extraData    用户的自定义字段
 *
 *  @discussion
 *  若用户未实现该回调，则SDK会自动跳转新闻详情页面，需传入navController变量
 *  若用户实现该回调，则跳转到用户回调
 */
@optional
- (void)newsListDidSelectNews:(NFeedsView *)feedsView
                     newsInfo:(NFNewsInfo *)newsInfo
                    extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  自定义插入新闻
 *
 *  @param channelInfo     频道
 *  @param loadType        0代表上拉加载更多，1代表下拉刷新
 *  @param block           用户获取到新闻信息block返回
 *
 *  @discussion
 *  若用户无需实现自定义新闻列表插入数据，则无需实现该接口，
 *  自定义的infoType不可以为article/picset/video字段，用户自定义设置，并在点击回调中实现相应的点击跳转逻辑
 */
@optional
- (void)customNewsWithChannel:(NFChannelInfo *)channelInfo
                     loadType:(NSInteger)type
               comletionBlock:(void(^)(NSArray  <NFNewsInfo *>*newsList, NSIndexSet *indexSet))block;

@end

/**
 *  @class
 *  NFeedsView类
 *
 *  @abstract
 *  继承自UIView，用以展示信息流首页的pageController页面
 */
@interface NFeedsView : UIView <NFNewsMarkReadDelegate>

/**
 *  @method
 *
 *  @abstract
 *  刷新已读新闻的状态
 *
 *  @discussion
 *  使用SDK整合的View，无需调用该接口；
 *  用户自定义跳转页面，需在新闻详情加载成功后，将新闻标记为已读，并主动调用该接口
 */
- (void)markReadNews;

/**
 *  @method
 *
 *  @abstract
 *  强制刷新当前列表
 *
 *  @discussion
 *  可调用该接口实现强制刷新当前列表的功能
 */
- (void)forceRefresh;

/**
 * 允许用户直接定位到某一个频道
 * 
 * @param channelId    频道ID
 * @param animated     是否显示动画
 */
- (void)setSelectedWithChannel:(NSString *)channelId
                      animated:(BOOL)animated
__attribute__((deprecated("Use setSelectedWithChannelTag:animated: instead.")));

/**
 * 允许用户直接定位到某一个频道
 * 
 * @param channelTag    频道tag
 * @param animated     是否显示动画
 */
- (void)setSelectedWithChannelTag:(NSString *)channelTag
                         animated:(BOOL)animated;

/**
 * 设置simultaneousScrollView，允许新闻列表的table和simultaneousScrollView一起相应滑动手势。
 */
@property (nonatomic, strong) UIScrollView *simultaneousScrollView;

@property (nonatomic, weak) id<MultiScrollViewDelegate> multiScrollDelegate;
@property (nonatomic, strong, readonly) UIScrollView *currentNewsListScrollView;

@end
