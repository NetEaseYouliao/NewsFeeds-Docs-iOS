//
//  NewsFeedsUISDK.h
//  NewsFeedsUISDK
//
//  Created by shoulei ma on 2017/10/10.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "NFeedsView.h"
#import "NFArticleDetailView.h"
#import "NFPicSetGalleryViewController.h"
#import "NFArticleGalleryViewController.h"
#import "NFVideoBrowserViewController.h"

@protocol NewsFeedsUISDKDelegate <NSObject>

/**
 *  @method
 *
 *  @abstract
 *  详情页面的分享按钮点击
 *
 *  @param shareInfo  分享需要用到的字段集合
 *  @param type       分享类型，0：微信好友  1：朋友圈
 *
 *  @discussion
 *  实现该回调，则分享按钮显示
 *  未实现该回调，则分享按钮隐藏
 */
- (void)onShareClick:(NSDictionary *)shareInfo
                type:(NSInteger)type;

@end

@interface NewsFeedsUISDK : NSObject

/**
 *  @method
 *
 *  @abstract
 *  获取当前SDK的版本号
 *
 *  @discussion
 *  UI SDK需配合NewsFeedsSDK使用，在使用时应保证版本兼容
 */
+ (NSString *)version;

/**
 *  @method
 *
 *  @abstract
 *  实现一个全局的delegate
 *  现阶段实现了分享的协议，sdk提供分享的数据，客户端自行实现分享行为
 */
+ (void)setDelegate:(id<NewsFeedsUISDKDelegate>)delegate;

/**
 *  @method
 *
 *  @abstract
 *  创建NFeedsView实例
 *
 *  @param navController    页面跳转需要的UINavigationController实例
 *  @param delegate         NFeedsViewDelegate回调
 *  @param extraData        用户可以传入自定义字段，该字段会在回调中返回给用户
 *
 *  @discussion
 *  若用户未实现回调，则SDK会自动跳转新闻详情页面，需传入navController变量
 *  若用户实现回调，则跳转到用户回调，无需传navController参数
 */
+ (NFeedsView *)createFeedsView:(UINavigationController *)navController
                       delegate:(id<NFeedsViewDelegate>)delegate
                      extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  创建NFArticleDetailView实例
 *
 *  @param newsInfo    待加载新闻详情的newsInfo
 *  @param delegate    NFArticleDetailViewDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFArticleDetailView *)createArticleDetailView:(NFNewsInfo *)newsInfo
                                        delegate:(id<NFArticleDetailViewDelegate>)delegate
                                       extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  创建NFPicSetGalleryViewController实例
 *
 *  @param newsInfo    待加载图集的newsInfo
 *  @param delegate    NFPicSetGalleryDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFPicSetGalleryViewController *)createPicSetGalleryViewController:(NFNewsInfo *)newsInfo
                                                            delegate:(id<NFPicSetGalleryDelegate>)delegate
                                                           extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  创建NFArticleGalleryViewController实例
 *
 *  @param imageArray           图片数组
 *  @param currentImageIndex    当前图片索引
 *  @param delegate             NFArticleGalleryDelegate回调
 *  @param extraData            用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFArticleGalleryViewController *)createArticleGalleryViewController:(NSArray <NFNewsDetailImage *>*)imageArray
                                                            imageIndex:(NSInteger)currentImageIndex
                                                              delegate:(id<NFArticleGalleryDelegate>)delegate
                                                             extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  创建NFVideoBrowserViewController实例
 *
 *  @param newsInfo    待加载图集的newsInfo
 *  @param delegate    NFVideoBrowserDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFVideoBrowserViewController *)createVideoBrowserViewController:(NFNewsInfo *)newsInfo
                                                          delegate:(id<NFVideoBrowserDelegate>)delegate
                                                         extraData:(id)extraData;

@end
