//
//  NFHybridNewsInfo.h
//  NewsFeedsHybridSDK-NFHybridBundle
//
//  Created by 刘梦桦 on 2018/3/26.
//

#import <Foundation/Foundation.h>

@interface NFHybridNewsInfo : NSObject

// 应用的 appKey。纯 H5 接入时候必须 URL 带入，Hybrid H5 必须在 init 方法中传递
@property (nonatomic, copy) NSString * ak;

// 频道 tag值
@property (nonatomic, copy) NSString * ctag;

// 新闻标题
@property (nonatomic, copy) NSString * dt;

// 新闻ID
@property (nonatomic, copy) NSString * newsID; 

// 推荐策略及权重信息，上传用户行为时需携带，producer为recommendation时才有值
@property (nonatomic, copy) NSString * info;

// 新闻类型，article：文章，picset：图集，video：视频，ad：表示广告
@property (nonatomic, copy) NSString * it;

// 新闻生产者，user：用户，recommendation：推荐系统
@property (nonatomic, copy) NSString * p;

// 推荐唯一标识，上报用户行为是需要携带, producer为recommendation时才有值
@property (nonatomic, copy) NSString * rid;

// 应用的 secretKey。纯 H5 接入时候必须 URL 带入，Hybrid H5 必须在 init 方法中传递
@property (nonatomic, copy) NSString * sk;

// 新闻来源
@property (nonatomic, copy) NSString * st;

// 用户唯一标识。支持 userId、deviceId 和 cookie，
@property (nonatomic, copy) NSString * unid;

// 频道ID
@property (nonatomic, copy) NSString * cid;

@end
