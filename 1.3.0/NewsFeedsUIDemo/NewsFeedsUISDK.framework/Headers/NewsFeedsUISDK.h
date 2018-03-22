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

@end
