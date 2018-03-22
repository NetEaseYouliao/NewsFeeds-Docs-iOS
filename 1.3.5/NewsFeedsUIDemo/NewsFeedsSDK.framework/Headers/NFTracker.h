//
//  NFTracker.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/20.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NFNewsInfo;
@class NFNewsDetailView;

/**
 * @class
 * NFTracker类
 *
 * @abstract
 * 用户行为数据统计上报的功能类
 */
@interface NFTracker : NSObject

/**
 *  @method
 *
 *  @abstract
 *  初始化并返回一个<code>NFTracker</code>的单例
 *
 *  @return 返回的单例
 */
+ (instancetype)sharedTracker;

/**
 *  @method
 *
 *  @abstract
 *  设置曝光新闻上传的时间间隔，默认是15秒
 *
 *  @param uploadInterval       时间间隔
 */
- (void)setUploadInterval:(NSInteger)uploadInterval;

/**
 *  @method
 *
 *  @abstract
 *  调用trackNewsExposure接口，表示新闻曝光
 *
 *  @discussion
 *  该接口的调用需在某新闻列表暴露在当前屏幕0.5s以上调用，若是曝光时间太短，影响推荐数据
 *  该接口的使用建议咨询相关开发人员
 *
 *  @param newsInfo       新闻
 */
- (void)trackNewsExposure:(NFNewsInfo *)newsInfo;

/**
 *  @method
 *
 *  @abstract
 *  调用trackNewsClick接口，表示点击了一个新闻列表
 *
 *  @param newsInfo       新闻
 */
- (void)trackNewsClick:(NFNewsInfo *)newsInfo;


/**
 *  @method
 *
 *  @abstract
 *  调用trackNewsShare接口，表示分享一个新闻
 *
 *  @param newsInfo       新闻
 */
- (void)trackNewsShare:(NFNewsInfo *)newsInfo;

/**
 *  @method
 *
 *  @abstract
 *  初始化事件的计时器。
 *
 *  @discussion
 *  若需要统计某个事件的持续时间，先在事件开始时调用 trackBrowseBegin:"news" 记录事件开始时间，该方法并不会真正发
 *  送事件；随后在事件结束时，调用 trackBrowseEnd:"news"，SDK 会追踪 "news" 事件，
 *  并自动将事件持续时间记录
 *
 *  多次调用 trackBrowseBegin:"news" 时，事件 "news" 的开始时间以最后一次调用时为准。
 *
 *  注意：该接口一定是与trackBrowseEnd:"news"接口配合使用，且成对出现
 *
 *  @param newsInfo       新闻
 */
- (void)trackBrowseBegin:(NFNewsInfo *)newsInfo;

/**
 *  @method
 *
 *  @abstract
 *  调用trackBrowseEnd接口，新闻详情浏览结束
 *
 *  @discussion
 *  注意：该接口一定是与trackBrowseBegin:"news"接口配合使用，且成对出现
 *
 *  @param newsInfo       新闻
 *  @param progress       浏览进度
 */

- (void)trackBrowseEnd:(NFNewsInfo *)newsInfo progress:(double)progress;

/**
 *  @method
 *
 *  @abstract
 *  调用trackArticleBrowseEnd接口，NFNewsDetailView的新闻详情浏览结束
 *
 *  @discussion
 *  该接口方便用户在使用SDK提供的NFNewsDetailView展示新闻详情时，无需计算progress
 *  调用该接口会自动计算出progress
 *
 *  可以不使用该接口，调用trackBrowseBegin：和trackBrowseEnd：progress：配合使用代替此接口
 *
 *  注意：使用该接口无需与trackBrowseBegin配合使用，SDK会在文章加载成功后自动调用
 *
 *  @param detailView     NFNewsDetailView实例
 */
- (void)trackArticleBrowseEnd:(NFNewsDetailView *)detailView;

/**
 *  @method
 *
 *  @abstract
 *  强制试图把数据传到对应的NFTracker数据采集的服务器上
 *
 *  @discussion
 *  主动调用上传接口，则不论定时上传的时间间隔网络类型的限制条件是否满足，都尝试向服务器上传一次数据
 */
- (void)upload;

@end
