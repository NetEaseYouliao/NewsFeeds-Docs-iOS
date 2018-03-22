//
//  NFNews.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/22.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@class GDTNativeAdData;

/**
 * @class
 * NFNewsImage类
 *
 * @abstract
 * 新闻列表中的新闻图片类
 */
@interface NFNewsImage : NSObject

//缩略图url
@property (nonatomic, copy)     NSString *url;

//缩略图高度
@property (nonatomic, assign)   NSInteger height;

//缩略图宽度
@property (nonatomic, assign)   NSInteger width;

@end


/**
 * @class
 * NFNewsVideo类
 *
 * @abstract
 * 新闻列表Video类
 */
@interface NFNewsVideo: NSObject

//视频封面图
@property (nonatomic, copy)     NSString *cover;

//视频封面大图
@property (nonatomic, copy)     NSString *largeCover;

//播放类型，0：表示4：3宽高比，1：表示16：9宽高比
@property (nonatomic, assign)   NSInteger playsize;

//视频时长，单位为秒
@property (nonatomic, assign)   NSInteger duration;

//mp4格式视频链接，标清
@property (nonatomic, copy)     NSString *mp4SdUrl;

//mp4格式视频链接，高清
@property (nonatomic, copy)     NSString *mp4HdUrl;

// m3u8格式视频链接，标清
@property (nonatomic, copy)     NSString *m3u8SdUrl;

//m3u8格式视频链接，高清
@property (nonatomic, copy)     NSString *m3u8HdUrl;

//flv格式视频链接，标清
@property (nonatomic, copy)     NSString *sdUrl;

//flv格式视频链接，高清
@property (nonatomic, copy)     NSString *hdUrl;

//flv格式视频链接，shd
@property (nonatomic, copy)     NSString *shdUrl;

@end


/**
 * @class
 * NFAdInfo类
 *
 * @abstract
 * 新闻列表中的新闻图片类
 */
@interface NFAdInfo : NSObject

//应用ID
@property (nonatomic, copy)   NSString *mediumId;

//广告来源
@property (nonatomic, copy)   NSString *producer;

//广告位
@property (nonatomic, copy)   NSString *adPlacementId;

//公网ip
@property (nonatomic, copy)   NSString *ip;

//广点通原生广告数据
@property (nonatomic, strong) GDTNativeAdData *data;

//InMoBi和易推广广告曝光url
@property (nonatomic, strong) NSArray <NSString *>*exposureURLArray;

//InMoBi和易推广广告点击url
@property (nonatomic, strong) NSArray <NSString *>*clickURLArray;

//易推广广告特殊曝光url
@property (nonatomic, strong) NSArray <NSString *>*ytgExposureURLArray;

//易推广广告特殊点击url
@property (nonatomic, strong) NSArray <NSString *>*ytgClickURLArray;

//InMoBi广告是否是App
@property (nonatomic, assign) BOOL isApp;

//InMoBi广告落地页
@property (nonatomic, copy)  NSString *landingURL;

//广告辩题
@property (nonatomic, copy)     NSString *title;

//缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
@property (nonatomic, assign)   NSInteger imgType;

//广告图片URL
@property (nonatomic, copy)     NSString *imageUrl;

@end


/**
 * @class
 * NFNewsInfo类
 *
 * @abstract
 * 新闻列表info类
 */
@interface NFNewsInfo : NSObject

//新闻提供者，user表示用户自编辑新闻，recommendation表示来自个性化推荐系统
@property (nonatomic, copy)     NSString *producer;

//单次推荐唯一标识，仅当个性化推荐的文章才有值
@property (nonatomic, copy)     NSString *recId;

//推荐策略及权重信息，仅当个性化推荐的文章才有值
@property (nonatomic, copy)     NSString *algInfo;

//频道ID
@property (nonatomic, copy)     NSString *channelId;

//新闻ID
@property (nonatomic, copy)     NSString *infoId;

//投递ID
@property (nonatomic, assign)   long deliverId;

//新闻类型，文章/图集/视频  article/picset/video
@property (nonatomic, copy)     NSString *infoType;

//新闻标题
@property (nonatomic, copy)     NSString *title;

//新闻简介
@property (nonatomic, copy)     NSString *summary;

//新闻来源
@property (nonatomic, copy)     NSString *source;

//新闻更新时间
@property (nonatomic, copy)     NSString *updateTime;

//发布时间
@property (nonatomic, copy)     NSString *publishTime;

//文章是否包含视频，true：有，false：没有
@property (nonatomic, assign)   BOOL hasVideo;

//缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
@property (nonatomic, assign)   NSInteger imgType;

//缩略图，无图模式下该字段为空
@property (nonatomic, strong)   NSArray <NFNewsImage *>*thumbnails;

//封面图三图
@property (nonatomic, strong)   NSArray <NFNewsImage *>*tripleImgs;

//图集中图片数量,只有在图集情况下才会返回该字段
@property (nonatomic, assign)   NSInteger num;

//视频源信息,只有在视频情况下才会返回该字段
@property (nonatomic, strong)   NSArray <NFNewsVideo *> *videos;

//广告信息
@property (nonatomic, strong)   NFAdInfo *ad;

@end


/**
 * @class
 * NFNews类
 *
 * @abstract
 * 新闻model类
 */
@interface NFNews : NSObject

//普通新闻信息
@property (nonatomic, strong) NSArray <NFNewsInfo *> *infos;

//轮播信息
@property (nonatomic, strong) NSArray <NFNewsInfo *> *banners;

//头条信息
@property (nonatomic, strong) NSArray <NFNewsInfo *> *tops;

@end
