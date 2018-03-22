//
//  NewsFeedsSDK.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/19.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NFChannel.h"
#import "NFNews.h"
#import "NFNewsDetail.h"
#import "NFNewsDetailView.h"
#import "NFTracker.h"
#import "NSObject+NFModel.h"
#import "GeTuiSdk.h"

typedef NS_ENUM(NSInteger, NFSDKSNSType) {
    NFSDKSNSTypeWXSession = 0, //微信会话
    NFSDKSNSTypeWXTimeline,    //微信朋友圈
};

typedef NS_ENUM(NSInteger, NFSDKMaxLogLevel) {
    NFSDKMaxLogLevelNone = 0, //关闭日志
    NFSDKMaxLogLevelError,    //Error
    NFSDKMaxLogLevelWarning,  //Warning
    NFSDKMaxLogLevelInfo,     //Info
    NFSDKMaxLogLevelDebug,    //Debug
};

typedef void (^NFLoadNewsBlock)(NFNews *newsList, NSError *error);
typedef void (^NFLoadImageBlock)(UIImage *image, NSError *error);
typedef void (^NFWebCachedImageBlock)();

/**
 * @class
 * NewsFeedsSDK类
 *
 * @abstract
 * 在应用中中嵌入NewsFeedsSDK，并进行使用的主要类
 */
@interface NewsFeedsSDK : NSObject

/**
 *  @method
 *
 *  @abstract
 *  初始化并返回一个<code>NewsFeedsSDK</code>的单例
 *
 *  @return 返回的单例
 */
+ (instancetype)sharedInstance;

/**
 *  @method
 *
 *  @abstract
 *  配置SDK相关参数，需要在应用启动时候调用
 *  在AppDelegate中<code>application:didFinishLaunchingWithOptions:</code>中调用
 *
 *  @param appKey    应用的唯一标识，在官网申请后获取
 *  @param appSecret 在官网申请后获取
 */
- (void)startWithAppKey:(NSString *)appKey appSecret:(NSString *)appSecret;

/**
 *  @method
 *
 *  @abstract
 *  配置日志的打印级别
 *
 *  @param maxLevel  日志level
 *
 *  @discussion
 *  默认值为NFSDKMaxLogLevelNone，不打印任何日志
 *  NFSDKMaxLogLevelError   只打印error日志
 *  NFSDKMaxLogLevelWarning 打印error和warning日志
 *  NFSDKMaxLogLevelInfo    打印error和warning和info日志
 *  NFSDKMaxLogLevelDebug   打印所有日志
 */
- (void)setLogLevel:(NFSDKMaxLogLevel)maxLevel;

/**
 *  @method
 *
 *  @abstract
 *  配置用户Id
 *
 *  @param userId  用户Id，记录用户相关信息
 *
 *  @discussion
 *  设置用户的userId可以更精确跨平台的进行内容的推荐
 *  若不调用该接口，系统将默认采用匿名的设备Id进行相关用户行为分析
 *  
 *  若设置userId为nil或者空，则会清除保存的用户信息
 */
- (void)setUserId:(NSString *)userId;

/**
 *  @method
 *
 *  @abstract
 *  设置加载网络返回的图片、视频URL为https，默认为NO
 *  YES为打开，NO为关闭
 *
 *  @param httpsOn  图片、视频URL的https的开关状态
 *
 *  @discussion
 *  默认加载信息流返回的网络图片视频URL存在为http的情况
 *  用户可以通过打开该开关，获取URL为https的链接
 */
- (void)setHttpsOn:(BOOL)httpsOn;

/**
 *  @method
 *
 *  @abstract
 *  设置正文缓存的开启状态
 *
 *  @param enabled  缓存开启状态，默认YES
 *
 *  @discussion
 *  默认开启状态
 */
- (void)setCacheEnabled:(BOOL)enabled;

/**
 *  @method
 *
 *  @abstract
 *  配置新闻详情页面最大缓存时长
 *
 *  @param maxCachedTime  最大缓存时长，单位秒，默认7*24*60*60，即一周
 *
 *  @discussion
 *  清除缓存的时机：退到后台或者进程杀死的时候
 */
- (void)setMaxCachedTime:(NSInteger)maxCachedTime;

/**
 *  @method
 *
 *  @abstract
 *  [接入广告模块必选]开发者需传入用来弹出广告目标页的ViewController，一般为当前ViewController
 */
- (void)setAdPresentController:(UIViewController *)controller;

/**
 *  @method
 *
 *  @abstract
 *  获取当前SDK的版本号
 */
- (NSString *)version;

/**
 *  @method
 *
 *  @abstract
 *  获取deviceId
 */
- (NSString *)deviceId;

/**
 *  @method
 *
 *  读取缓存的频道列表
 *
 *  @disscussion
 *  该接口已废弃
 */
- (NFChannels *)loadCanchedChannels __attribute__((deprecated("Use loadCanchedChannels:(void(^)(NFChannels *channelList))block instead.")));

/**
 *  @method
 *
 *  读取缓存的频道列表
 *
 *  @param block    查询结果回调
 */
- (void)loadCanchedChannels:(void(^)(NFChannels *channelList))block;

/**
 *  @method
 *
 *  获取所有新闻频道列表
 *
 *  @param block 查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如频道列表更新等
 */
- (void)loadChannelsWithBlock:(void(^)(NFChannels *channelList, NSError *error))block;

/**
 *  @method
 *
 *  获取所有缓存的新闻列表
 *
 *  @param channel 需要获取新闻的频道
 *
 *  @disscussion
 *  该接口已废弃
 */
- (NFNews *)loadCachedNewsWithChannel:(NFChannelInfo *)channel __attribute__((deprecated("Use loadCachedNewsWithChannel:block: instead.")));

/**
 *  @method
 *
 *  获取所有缓存的新闻列表
 *
 *  @param channel 需要获取新闻的频道
 *  @param block    查询结果回调
 *
 */
- (void)loadCachedNewsWithChannel:(NFChannelInfo *)channel
                            block:(void(^)(NFNews *newsList))block;

/**
 *  @method
 *
 *  获取所有新闻列表
 *
 *  @param channel  需要获取新闻的频道
 *  @param pageSize 需要获取普通新闻列表的条目数
 *  @param type     0：接口返回普通新闻   1：接口返回头图新闻、置顶新闻以及普通新闻
 *  @param block    查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadNewsWithChannel:(NFChannelInfo *)channel
                   pageSize:(NSUInteger)pageSize
                   loadType:(NSInteger)type
                      block:(void(^)(NFNews *newsList, NSError *error))block;

/**
 *  @method
 *
 *  获取所有新闻列表
 *
 *  @param channel    需要获取新闻的频道
 *  @param pageSize   需要获取普通新闻列表的条目数
 *  @param type       0：接口返回普通新闻   1：接口返回头图新闻、置顶新闻以及普通新闻
 *  @param longitude  经度
 *  @param latitude   维度
 *  @param block      查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadNewsWithChannel:(NFChannelInfo *)channel
                   pageSize:(NSUInteger)pageSize
                   loadType:(NSInteger)type
                  longitude:(double)longitude
                   latitude:(double)latitude
                      block:(void(^)(NFNews *newsList, NSError *error))block;

/**
 *  @method
 *
 *  获取相关推荐新闻列表
 *
 *  @param newsInfo   需要获取新闻的相关推荐新闻
 *  @param block      查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadRelatedNewsWithNews:(NFNewsInfo *)newsInfo
                          block:(void(^)(NFNews *newsList, NSError *error))block;

/**
 *  @method
 *
 *  获取相关推荐新闻列表
 *
 *  @param newsInfo   需要获取新闻的相关推荐新闻
 *  @param longitude  经度
 *  @param latitude   维度
 *  @param block      查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadRelatedNewsWithNews:(NFNewsInfo *)newsInfo
                      longitude:(double)longitude
                       latitude:(double)latitude
                          block:(void(^)(NFNews *newsList, NSError *error))block;

/**
 *  @method
 *
 *  获取新闻的详情
 *
 *  @param newsInfo 需要获取新闻
 *  @param block    查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻详情的展示等
 */
- (void)loadNewsDetailWithNews:(NFNewsInfo *)newsInfo
                         block:(void(^)(NFNewsDetail *newsDetail, NSError *error))block;

/**
 *  @method
 *
 *  @abstract
 *  获取新闻详情中url对应的图片
 *
 *  @param url    待加载图片的url
 *  @param block  返回的结果，可以在block中处理自己的业务逻辑
 *
 *  @discussion
 *  SDK本身会对新闻详情加载的图片做缓存，用户调用该接口可以获取已经缓存的图片
 *  防止使用第三方图片库会再对图片进行缓存，造成不必要的缓存空间浪费
 *
 *  若用户需要对详情页面的图片进行操作处理，当调用 NFNewsDetailView类的detailView:didClickImageArray:atIndex:方法时，
 *  建议用户使用该接口获取SDK的缓存图片
 */
- (void)loadImageWithUrl:(NSString *)url block:(NFLoadImageBlock)block;

/**
 *  @method
 *
 *  广告数据渲染曝光调用的方法
 *  
 *  @param adInfo 广告渲染的adInfo
 *
 *  @discussion
 *  [必选]广告数据渲染完毕，即将展示曝光时需调用本方法。
 *
 */
- (void)attachAd:(NFAdInfo *)adInfo;

/**
 *  @method
 *
 *  广告点击调用方法
 *
 *  @param adInfo 用户点击的广告adInfo
 *
 *  @discussion
 *  当用户点击广告时，开发者需调用本方法，系统会弹出内嵌浏览器、或内置AppStore、
 *  或打开系统Safari，来展现广告目标页面
 */
- (void)clickAd:(NFAdInfo *)adInfo;

/**
 *  @method
 *
 *  @abstract
 *  删除过期的新闻
 *
 *  @param channelId  待删除的频道
 *
 *  @discussion
 *  提供删除过期缓存的调用接口，会同时删除过期的新闻
 */
- (void)deleteOldCachedNews:(NSString *)channelId;

/**
 *  @method
 *
 *  @abstract
 *  判断新闻列表是否要自动刷新
 *
 *  @param channelId  频道Id
 *
 *  @disscussion
 *  该接口已废弃
 */
- (BOOL)autoRefresh:(NSString *)channelId __attribute__((deprecated("Use autoRefresh:block: instead.")));

/**
 *  @method
 *
 *  @abstract
 *  判断新闻列表是否要自动刷新
 *
 *  @param channelId  频道Id
 *
 *
 *  @discusson
 *  保证数据库的操作在子线程执行，结果以block返回
 */
- (void)autoRefresh:(NSString *)channelId
              block:(void(^)(BOOL autoRefresh))block;

/**
 *  @method
 *
 *  @abstract
 *  将新闻标记为已读
 *
 *  @param infoId  新闻Id
 *
 */
- (void)markRead:(NSString *)infoId;

/**
 *  @method
 *
 *  @abstract
 *  判断新闻的已读状态
 *
 *  @param infoId  新闻Id
 *
 *  @disscussion
 *  该接口已废弃
 */
- (BOOL)hasRead:(NSString *)infoId __attribute__((deprecated("Use hasRead:block: instead.")));

/**
 *  @method
 *
 *  @abstract
 *  判断新闻的已读状态
 *
 *  @param infoId  新闻Id
 *
 *  @discusson
 *  保证数据库的操作在子线程执行，结果以block返回
 */
- (void)hasRead:(NSString *)infoId
          block:(void(^)(BOOL hasRead))block;

/**
 *  @method
 *
 *  @abstract
 *  清除某一条新闻的正文缓存
 *
 *  @param infoId    新闻Id
 *  @param block     删除完成后的回调
 *
 *  @discussion
 *  会同时删除正文的缓存和正文图片的缓存
 */
- (void)removeCachedDetail:(NSString *)infoId block:(NFWebCachedImageBlock)block;

/**
 *  @method
 *
 *  @abstract
 *  清除所有的正文缓存
 *
 *  @param block  删除完成后的回调
 *
 *  @discussion
 *  会同时清除所有的正文缓存和正文图片的缓存
 */
- (void)clearCachedDetails:(NFWebCachedImageBlock)block;

/**
 *  @method
 *
 *  @abstract
 *  删除过期的缓存
 *
 *  @param block  删除完成后的回调
 *
 *  @discussion
 *  提供删除过期缓存的调用接口，会同时删除过期的正文缓存和正文图片
 */
- (void)removeOldCachedDetails:(NFWebCachedImageBlock)block;
@end
