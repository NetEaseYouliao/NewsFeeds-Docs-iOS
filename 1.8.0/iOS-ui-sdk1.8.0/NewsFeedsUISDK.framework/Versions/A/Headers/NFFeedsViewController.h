//
//  NFFeedsViewViewController.h
//  FLAnimatedImage
//
//  Created by neal on 2017/12/19.
//

#import <UIKit/UIKit.h>

@class NFChannelInfo, NFNewsInfo;
@protocol NFeedsViewDelegate;

NS_ASSUME_NONNULL_BEGIN

@interface NFFeedsViewController : UIViewController

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
animated:(BOOL)animated;


/// 表示需要跳转的channel，可为空。默认显示第一个频道
@property (nonatomic, strong) NFChannelInfo * _Nullable channel;

/// 从入口带进来的新闻列表 需要放在频道的最前面，可为空。
@property (nonatomic, strong) NFNewsInfo * _Nullable newsInfo;

@property (nonatomic, strong) id _Nullable extraData;

@property (nonatomic, weak) id <NFeedsViewDelegate> _Nullable delegate;

- (instancetype _Nonnull)initWithSelectedChannel:(NFChannelInfo * __nullable)channel openNews:(NFNewsInfo * __nullable)newsInfo delegate:(nullable id<NFeedsViewDelegate>)delegate extraData:(nullable id)extraData;

@end

NS_ASSUME_NONNULL_END
