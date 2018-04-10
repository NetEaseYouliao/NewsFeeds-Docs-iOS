//
//  NewsFeedsHybridSDK.h
//  AFNetworking
//
//  Created by neal on 2018/2/9.
//

#import <Foundation/Foundation.h>
#import "NFHybridContainerController.h"
#import "NFHybridNewsInfo.h"

@interface NewsFeedsHybridSDK : NSObject

@property (nonatomic, copy)  NSString *appKey;
@property (nonatomic, copy) NSString *appSecret;

///获取当前SDK的版本号
@property (nonatomic, readonly) NSString *version;

+ (instancetype)sharedSDK;

/**
 *  @method
 *
 *  @abstract
 *  配置SDK相关参数，需要在应用启动时候调用
 *  在AppDelegate中<code>application:didFinishLaunchingWithOptions:</code>中调用
 *
 *  @param appKey    应用的唯一标识，在官网申请后获取
 *  @param appSecret 在官网申请后获取
 */
- (void)startWithAppKey:(NSString *)appKey appSecret:(NSString *)appSecret;

/**
 *  @method
 *
 *  @abstract
 *  创建hybrid sdk入口NFHybridContainerController
 *
 */
- (NFHybridContainerController *)instantiateInitialContainer;

/**
 *  @method
 *
 *  @param url 指定的url
 *  @abstract
 *  用户指定url创建NFHybridContainerController
 *
 */
- (NFHybridContainerController *)instantiateContainerWithURL:(NSURL *)url;

/**
 * @method
 *
 * @param newsInfo 指定的newsInfo
 * @abstruct
 * 用户指定newsInfo创建NFHybridContainerController
 *
 */
- (NFHybridContainerController *)instantiateContainerWithNewsInfo:(NFHybridNewsInfo *)newsInfo;

@end
