//
//  NFPicSetGalleryViewController.h
//  NewsFeedsUISDK
//
//  Created by shoulei ma on 2017/10/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFNewsInfo;
@class NFPicSetGalleryViewController;

/**
 *  @protocol
 *
 *  @abstract
 *  图集的回调
 *
 *  @discussion
 *  图集预览的时候会用到该回调
 */
@protocol NFPicSetGalleryDelegate <NSObject>

@optional

/**
 *  @method
 *
 *  @abstract
 *  图集加载成功时的回调
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时刷新列表已读状态
 */
- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController
                   extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  图集相关推荐新闻的点击回调
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param newsInfo             点击的新闻
 *  @param extraData            用户的自定义字段
 */
- (void)relatedNewsDidSelect:(NFPicSetGalleryViewController *)browserController
                    newsInfo:(NFNewsInfo *)newsInfo
                   extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  点击返回按钮的时候
 */
- (void)back:(NFPicSetGalleryViewController *)browserController
   extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  视频分享按钮点击
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param shareInfo            分享需要用到的字段集合
 *  @param type                 分享类型，0：微信好友  1：朋友圈
 *  @param extraData            用户的自定义字段
 */
- (void)onPicSetShareClick:(NFPicSetGalleryViewController *)browserController
                newsDetail:(NSDictionary *)shareInfo
                      type:(NSInteger)type
                 extraData:(id)extraData;
@end

@interface NFPicSetGalleryViewController : UIViewController

@end
