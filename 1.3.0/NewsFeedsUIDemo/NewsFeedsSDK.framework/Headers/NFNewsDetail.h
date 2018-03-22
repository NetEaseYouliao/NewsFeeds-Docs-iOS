//
//  NFNewsDetail.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/22.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NFNewsVideo;
@class NFAdInfo;

/**
 * @class
 * NFNewsDetailImage类
 *
 * @abstract
 * 新闻详情中的图片model类
 */
@interface NFNewsDetailImage : NSObject

//新闻图片url
@property (nonatomic, copy)     NSString *url;

//新闻图片高度
@property (nonatomic, assign)   NSInteger height;

//新闻图片宽度
@property (nonatomic, assign)   NSInteger width;

//新闻图片类型
@property (nonatomic, assign)   NSInteger pictype;

//图片描述，图集的时候该字段才有值
@property (nonatomic, copy)     NSString *note;

@end


/**
 * @class
 * NFNewsDetail类
 *
 * @abstract
 * 新闻详情的model类
 */
@interface NFNewsDetail : NSObject

//新闻ID
@property (nonatomic, copy)     NSString *infoId;

//新闻类型
@property (nonatomic, copy)     NSString *infoType;

//新闻类目
@property (nonatomic, copy)     NSString *category;

//新闻标题
@property (nonatomic, copy)     NSString *title;

//发布时间
@property (nonatomic, copy)     NSString *publishTime;

//新闻来源
@property (nonatomic, copy)     NSString *source;

//原文地址
@property (nonatomic, copy)     NSString *sourceLink;

//新闻正文
@property (nonatomic, copy)     NSString *content;

//标签
@property (nonatomic, copy)     NSString *tag;

//广告信息
@property (nonatomic, strong)   NFAdInfo *ad;

//新闻图片
@property (nonatomic, strong)   NSArray <NFNewsDetailImage *>*imgs;

//新闻视频
@property (nonatomic, strong)   NSArray <NFNewsVideo *>*videos;

@end
