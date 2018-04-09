//
//  NFViewController.h
//  WKWebViewJSBridge
//
//  Created by nealsun on 01/31/2018.
//  Copyright (c) 2018 nealsun. All rights reserved.
//

@import UIKit;
@import WebKit;

NS_ASSUME_NONNULL_BEGIN

@class NFHybridContainerController;

///native收到js发送的事件后，如果需要返回结果，使用NFHybridSDKCallback定义的接口返回
typedef void (^NFHybridSDKCallback)(NSDictionary * _Nullable result, NSError * _Nullable error);

@protocol NFHybridDelegate <NSObject>

/**
 * @method
 *
 * @param container 当前sdk的webview容器
 * @param event js发起的事件
 * @param params js事件携带的参数
 * @param completion native通过这个block返回结果，如果为nil，则js不需要返回结果。
 *
 * @discussion
 * NFHybridDelegate回调，js通过此回调通知native需要执行的命令。
 */
- (BOOL)webViewContainer:(NFHybridContainerController *_Nonnull)container receiveEvent:(NSString *_Nonnull)event params:(NSDictionary *_Nullable)params completion:(NFHybridSDKCallback _Nullable )completion;

@end

@interface NFHybridContainerController : UIViewController

@property (strong, nonatomic, readonly) WKWebView *webView;

@property (weak, nonatomic) id<NFHybridDelegate> delegate;

@property (assign, nonatomic) BOOL injectJSBridge;  //是否需要注入jsbride。 default is YES


- (instancetype)initWithURL:(NSURL *)url;
- (instancetype)initWithRequest:(NSURLRequest *)request;

/**
 * @method
 *
 * @param action native发起的事件
 * @param params native事件携带的参数
 * @param completion js通过这个block返回结果，如果native不关心返回结果，只需要几天。
 
 * @discussion
 * native调用js接口, js只响应sdk支持的事件。
 */
- (void)invokeJavascriptAction:(NSString *)action params:(NSDictionary *)params completion:(void (^)(NSDictionary *))completion;

/// set these property to custom ui
@property(nonatomic, strong, nullable) UIColor* progressTintColor;  //在navigationbar上显示的进度条颜色

/**
 * @method
 *
 * @abstruct
 * 用户可以调用这个接口手动刷新
 */
- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
