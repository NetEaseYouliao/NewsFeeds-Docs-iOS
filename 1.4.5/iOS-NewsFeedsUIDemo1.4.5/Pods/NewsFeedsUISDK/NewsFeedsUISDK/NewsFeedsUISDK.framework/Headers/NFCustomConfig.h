//
//  NFCofiguration.h
//  NewsFeedsUISDK
//
//  Created by neal on 2017/12/4.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NFTitleBoardChannelUIOption, NFTitleBoardChannelFuncOption;
@class NFFeedsUIOption, NFFeedsFuncOption;
@class NFArticleUIOption, NFArticleFuncOption;
@class NFDataFuncOption;

@interface NFCustomConfig : NSObject

+ (instancetype)defaultConfig;

#pragma mark - Func Config
@property (nonatomic, strong) NFDataFuncOption *dataFuncOption;

@property (nonatomic, strong) NFTitleBoardChannelFuncOption *channelFuncOption;

@property (nonatomic, strong) NFFeedsFuncOption *feedsFuncOption;

@property (nonatomic, strong) NFArticleFuncOption *articleFuncOption;

#pragma mark - UI Config

@property (nonatomic, strong) NFTitleBoardChannelUIOption *channelUIOption;

@property (nonatomic, strong) NFFeedsUIOption *feedsUIOption;

@property (nonatomic, strong) NFArticleUIOption *articleUIOption;

@end
