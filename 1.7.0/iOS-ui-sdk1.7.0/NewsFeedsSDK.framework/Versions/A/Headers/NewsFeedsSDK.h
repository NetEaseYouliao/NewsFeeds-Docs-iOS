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
#import "NFTracker.h"
#import <UIKit/UIKit.h>
#import "NSObject+NFModel.h"

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
typedef void (^NFDeleteCacheBlock)(void);

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
 *  设置缓存的开启状态
 *
 *  @param enabled  缓存开启状态，默认YES
 *
 *  @discussion
 *  默认开启状态
 *  注意：若用户同时使用UI SDK，强烈不建议用户关闭缓存
 */
- (void)setCacheEnabled:(BOOL)enabled;

/**
 *  @method
 *
 *  @abstract
 *  配置最大缓存时长
 *
 *  @param maxCachedTime  最大缓存时长，单位秒，默认24*60*60，即一天
 *
 *  @discussion
 *  清除过期缓存的时机：SDK初始化的时候以及进程杀死的时候
 */
- (void)setMaxCachedTime:(NSInteger)maxCachedTime;

/**
 *  @method
 *
 *  @abstract
 *  配置每个频道最大缓存新闻数量
 *
 *  @param maxCachedNum  最大缓存新闻数量，默认60条
 *
 *  @discussion
 *  单个频道下能够缓存的新闻条数的最大值
 */
- (void)setMaxCachedNum:(NSInteger)maxCachedNum;

/**
 *  @method
 *
 *  @abstract
 *  配置自动刷新时间间隔
 *
 *  @param autoRefreshTime  自动刷新时间间隔，单位是秒
 *
 *  @discussion
 *  定义自动刷新距离上次刷新时间的间隔
 */
- (void)setAutoRefreshTime:(NSTimeInterval)autoRefreshTime;

/**
 *  @method
 *
 *  @abstract
 *  [接入广告模块必选]开发者需传入用来弹出广告目标页的ViewController，一般为当前ViewController
 */
//- (void)setAdPresentController:(UIViewController *)controller;

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
 *  删除过期的新闻
 *
 *  @param tagId  待删除的频道 channelId is deprecated, use tagId instead.
 *  @param completeBlock  删除完成的回调
 *
 *  @discussion
 *  提供删除过期缓存的调用接口
 */
- (void)deleteExpireCachedNews:(NSString *)tagId
                      complete:(NFDeleteCacheBlock)completeBlock;

/**
 *  @method
 *
 *  @abstract
 *  删除单条新闻缓存
 *
 *  @param infoId         待删除的新闻Id
 *  @param completeBlock  删除完成的回调
 *
 *  @discussion
 *  提供删除单条新闻缓存的调用接口
 */
- (void)deleteCachedNews:(NSString *)infoId
                complete:(NFDeleteCacheBlock)completeBlock;

/**
 *  @method
 *
 *  @abstract
 *  删除所有缓存的新闻
 *
 *  @param completeBlock  删除完成的回调
 *
 *  @discussion
 *  completeBlock中实现刷新UI的操作
 */
- (void)deleteAllCachedNews:(NFDeleteCacheBlock)completeBlock;

/**
 *  @method
 *
 *  @abstract
 *  计算缓存的大小，
 *
 *  @return
 *  缓存的大小，单位KB
 *
 *  @discussion
 *  缓存较大的时候时候计算时间较长
 */
- (float)cacheSize;

/**
 *  @method
 *
 *  @abstract
 *  判断新闻列表是否要自动刷新
 *
 *  @param tagId  频道Id channelId is deprecated, use tagId instead.
 *
 *  @disscussion
 *  该接口已废弃
 */
- (BOOL)autoRefresh:(NSString *)tagId __attribute__((deprecated("Use autoRefresh:block: instead.")));

/**
 *  @method
 *
 *  @abstract
 *  判断新闻列表是否要自动刷新
 *
 *  @param tagId  频道Id channelId is deprecated, use tagId instead.
 *
 *
 *  @discusson
 *  保证数据库的操作在子线程执行，结果以block返回
 */
- (void)autoRefresh:(NSString *)tagId
              block:(void(^)(BOOL autoRefresh))block;

/**
 *  @method
 *
 *  @abstract
 *  将新闻标记为已读
 *
 *  @param infoId  新闻Id
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
 * @method
 *
 * @abstract
 * 举报当前文章存在的问题
 *
 * @param newsInfo   当前新闻信息
 * @param type       举报的问题类型,
 *                   类型分为
 *                   1:广告
 *                   2:低俗色情
 *                   3:反动不良
 *                   4:老旧重复内容
 *                   5:标题党
 *                   6:内容排版有误
 *                   7:疑似抄袭
 *                   8:谣言
 *                   9:其他
 * @param reason     举报的原因，如果类型为其他时需要填写reason
 * @param block      请求回调
 */
- (void)accuseNews:(NFNewsInfo *)newsInfo
          withType:(NSInteger)type
            reason:(NSString *)reason
             block:(void(^)(NSError *error))block;
@end