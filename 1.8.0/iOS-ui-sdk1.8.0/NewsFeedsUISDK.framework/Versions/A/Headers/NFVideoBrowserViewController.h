//
//  NTVideoBrowserViewController.h
//  NewsFeedsDemo
//
//  Created by JianfengZhu on 2017/11/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFNewsInfo;
@class NFVideoBrowserViewController;

@protocol NFVideoBrowserDelegate <NSObject>
@optional
/**
 *  @method
 *
 *  @abstract
 *  视频及相关推荐加载成功时的回调
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时刷新列表已读状态
 */
- (void)videoDidLoadContent:(NFVideoBrowserViewController *)browserController
                  extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *
 *  @discussion
 *  回调的时机：点击返回按钮的时候
 */
- (void)videoBack:(NFVideoBrowserViewController *)browserController
        extraData:(id)extraData;

/**
 *  @method
 *
 *  @abstract
 *  视频分享按钮点击
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *  @param shareInfo            分享需要用到的字段集合
 *  @param type                 分享类型，0：微信好友  1：朋友圈
 *  @param extraData            用户的自定义字段
 */
- (void)onVideoShareClick:(NFVideoBrowserViewController *)browserController
               newsDetail:(NSDictionary *)shareInfo
                     type:(NSInteger)type
                extraData:(id)extraData;
@end

@interface NFVideoBrowserViewController : UIViewController

@end
