//
//  NFChannel.h
//  NewsFeedsSDK
//
//  Created by shoulei ma on 2017/6/22.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * @class
 * NFChannel类
 *
 * @abstract
 * 频道的model类
 */
@interface NFChannelInfo : NSObject

//频道ID
@property (nonatomic, copy)   NSString *channelId;

//频道名称
@property (nonatomic, copy)   NSString *channelName;

//频道Tag
@property (nonatomic, copy)   NSString *channelTag;

//频道显示的顺序
@property (nonatomic, assign) NSInteger channelOrder;

//频道类型，0：按类别聚合   1：按话题聚合   2：按自定义来源聚合    3：地域频道   4：自营频道
@property (nonatomic, assign) NSInteger channelType;

//banner展现类型，0：自动轮播图和轮播图置顶均关闭，1：自动轮播图，2：轮播图置顶
@property (nonatomic, assign) NSInteger bannerType;

- (NSString *)tagID;

@end


/**
 * @class
 * NFChannelList类
 *
 * @abstract
 * 频道列表
 */
@interface NFChannels : NSObject

@property (nonatomic, strong)   NSArray <NFChannelInfo *>*channels;

@end
