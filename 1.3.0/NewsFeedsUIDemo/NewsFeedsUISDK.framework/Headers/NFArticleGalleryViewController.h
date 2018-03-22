//
//  NFArticleGalleryViewController.h
//  NewsFeedsUISDK
//
//  Created by shoulei ma on 2017/10/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFArticleGalleryViewController;

/**
 *  @protocol
 *
 *  @abstract
 *  新闻详情图片预览回调
 */
@protocol NFArticleGalleryDelegate <NSObject>

@optional
/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFArticleGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  单击图片的时候
 */
- (void)back:(NFArticleGalleryViewController *)browserController
   extraData:(id)extraData;

@end

@interface NFArticleGalleryViewController : UIViewController

@end
