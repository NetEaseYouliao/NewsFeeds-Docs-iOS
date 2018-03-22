//
//  NFConfigDefines.h
//  Pods
//
//  Created by neal on 2017/12/25.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, NFArticleNavType) {
    NFArticleNavTypeNone,
    NFArticleNavTypeSource,
    NFArticleNavTypeTitle,
    NFArticleNavTypeCustom
};

typedef NS_ENUM(NSUInteger, NFSharePlatform) {
    NFSharePlatformWXSession,
    NFSharePlatformWXTimeline
};

typedef NS_ENUM(NSUInteger, NFFeedsOptionThumbMod) {
    NFFeedsOptionThumbModNone, //无图
    NFFeedsOptionThumbModSingle,     //单图
    NFFeedsOptionThumbModNormal
};

extern NSString *const NFDataFuncOptionKey;

extern NSString *const NFTitleBoardChannelFuncOptionKey;
extern NSString *const NFTitleBoardChannelUIOptionKey;

extern NSString *const NFFeedsFuncOptionKey;
extern NSString *const NFFeedsUIOptionKey;

extern NSString *const NFArticleFuncOptionKey;
extern NSString *const NFArticleUIOptionKey;

extern NSString *const NFContainerClassKey;
