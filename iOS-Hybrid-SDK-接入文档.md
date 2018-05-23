## SDK概述

网易有料NewsFeedsHybridSDK是网易有料出品的一种轻量化解决方案。SDK兼容iOS 8.0+。

SDK通过Web方式接入网易有料内容，并使用jsbridge使native能够完成如微信分享等功能。


## 开发准备

1. #### SDK导入

   * cocoapod集成(推荐)

     1.首先编辑Podfile, 如果没有先在工程目录下运行`pod init`

        ```ruby
        #Podfile
        platform :ios, '8.0'
        #添加有料源
        source 'https://github.com/CocoaPods/Specs.git'
        source 'https://github.com/NetEaseYouliao/Specs.git'

        ...
          
        target :your_project_target do
          pod 'NewsFeedsHybridSDK'
          ...  #any other pod your project needed
        end
        ```

  	  2.运行`pod install`
     
     
    * 手动集成
     
    	网易有料NewsFeedsHybridSDK 可通过手动下载，并添加到项目中集成使用。
     
	 	1. 将下载得到的NewsFeedsHybridSDK.framework和NFHybridBundle.bundle手动导入到工程中。
	 	2. 下载NFUtilityFoundation，导入到工程中，下载链接为:[NFUtilityFoundation](https://github.com/NetEaseYouliao/NFUtilityFoundation)
	 	3. 添加系统依赖库: WebKit，CoreFoundation
					
		 	<font color=red size=2 face="黑体">手动导入的时候系统库CoreFoundation.framework需要设置为Optional，如下图</font>![image](http://odotlq87m.bkt.clouddn.com/WechatIMG14.jpeg)
	 	4. 在 Build Settings -> Other Linker Flags 里，添加选项 -ObjC。

     ​

     当SDK导入工程后，需要修改一些配置

     1. 配置App Transport Security

        由于Web访问的部分资源为http，为了保证能够正常使用SDK，需在info.plist中配置App Transport Security Settings，并将Allow Arbitrary Loads设为YES，具体设置参考下图：

        ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG70.jpeg)

2. #### SDK使用

##### 2.1 配置NewsFeedsHybridSDK相关参数

首先在合适的地方调用NewsFeedsHybridSDK中的配置方法进行SDK相关参数的配置，推荐在
AppDelegate的`application:didFinishLaunchingWithOptions:`中进行配置。

```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	
	...
	
	// 配置appKey和appSecret
	[[NewsFeedsHybridSDK sharedSDK] startWithAppKey:@"您的的appKey" appSecret:@"您的appSecret"];

	...
	return YES;
}
```

##### 2.2 获取SDK主入口

NewsFeedsHybridSDK中提供的`instantiateInitialContainer`方法为信息流主入口方法，用于展示信息流列表页面。
用户接入可以有两种选择:直接作为独立的controller、嵌入用户自定义的controller中。推荐放入UINavigationController的栈中。

1. 直接作为独立的controller

```objc
 // 初始化
 NFHybridContainerController *hybridContainerController = [[NewsFeedsHybridSDK sharedSDK] instantiateInitialContainer];
 hybridContainerController.delegate = SomeOtherClass<NFHybridDelegate> instance; 
  
 [self.navigationController pushViewController:hybridContainerController animated:YES];
 
```

2. 嵌入用户自定义的controller中

```objc
- (void)viewDidLoad {
   		 
   	...
   		 
   	// 初始化
   	NFHybridContainerController *hybridContainerController = [[NewsFeedsHybridSDK sharedSDK] instantiateInitialContainer];
   	hybridContainerController.delegate = SomeOtherClass<NFHybridDelegate> instance;
   		
   	// 嵌入用户自定义的controller中
   	[self.view addSubview:hybridContainerController.view];
    [self addChildViewController:hybridContainerController];
}
```
   
##### 2.3 实现NFHybridDelegate，完成web和native的通信
实现NFHybridDelegate，完成web和native的通信。当前HybridSDK支持四种事件，分别是init, openLink, openLinkByParam, negativeFeedBack。



* init 

  init是web页面加载时发起的事件，需要用户返回`appKey`， `secretKey`，`supportActions`，`supportSharePlatforms`。
    
	参数  | 描述
	------------- | -------------
	appKey		    | 用户appKey
	secretKey     | 用户secretKey
	supportActions| 当前native支持的事件名。因为web端的更新会超前于native，所以web会隐藏native不支持的action。
	supportSharePlatforms|当前native支持的分享平台，目前只支持微信和朋友圈，其中0表示微信分享用户，1表示微信朋友圈。native需要自行判断微信的安装情况来返回参数。暂不支持其它平台。
   closePullRefresh     | 关闭下拉刷新，设置此配置项。默认开启
   customNegativeFeedBack | 使用自定义负反馈，设置此配置，实现对应negativeFeedBack事件。

 
```objc
- (BOOL)webViewContainer:(NFHybridContainerController *_Nonnull)container receiveEvent:(NSString *_Nonnull)event params:(NSDictionary *_Nullable)params completion:(NFHybridSDKCallback _Nullable )completion{
    
    if ([event isEqualToString:@"init"]) {
        if (completion &&
            [NewsFeedsHybridSDK sharedSDK].appKey.length > 0 &&
            [NewsFeedsHybridSDK sharedSDK].appSecret.length > 0)
        {
            NSMutableArray *platform = [@[] mutableCopy];
                [platform addObject:@(0)];
                [platform addObject:@(1)];
            completion(@{
                         @"appKey": [NewsFeedsHybridSDK sharedSDK].appKey,
                         @"secretKey": [NewsFeedsHybridSDK sharedSDK].appSecret,
                         @"supportActions": @[],
                         @"supportSharePlatforms": platform
                         //关闭下拉刷新，设置此配置项
                         //@"closePullRefresh": @(YES),
                         //使用自定义负反馈，设置此配置，实现对应negativeFeedBack事件
                         //@"customNegativeFeedBack": @(YES)

                         }
                       , nil);
        }
        return YES;
    }
  return NO;
} 
```
  
* openLinkByParam

  当打开新闻详情时，会发起`openLink`事件，同时携带`url`参数
  用户可通过该事件拿到`url`参数以及结构化的数据初始化新闻详情页。数据字段在`NFHybridNewsInfo`中有详细说明，用户只需调用`instantiateContainerWithNewsInfo:newsInfo`方传入获取到的`NFHybridNewsInfo`字段即可初始化新闻详情页面。该事件不关心返回结果。
  
  字段 | 说明
  ----------- | --------
  ak |  应用的 appKey。Hybrid 必须在 init 方法中传递
  sk | 应用的 secretKey。Hybrid 必须在 init 方法中传递
  ctag|   频道 tag值 
  dt | 新闻标题
  newsID | 新闻ID
  info | 推荐策略及权重信息，上传用户行为时需携带，producer为recommendation时才有值
  it | 新闻类型，article：文章，picset：图集，video：视频，ad：表示广告
  p | 新闻生产者，user：用户，recommendation：推荐系统
  rid | 推荐唯一标识，上报用户行为时需要携带, producer为recommendation时才有值
  st | 新闻来源
  unid | 用户唯一标识。支持 userId、deviceId 和 cookie
  cid | 频道ID
  
  示例代码：
  
```objc
 - (BOOL)webViewContainer:(NFHybridContainerController *_Nonnull)container receiveEvent:(NSString *_Nonnull)event params:(NSDictionary *_Nullable)params completion:(NFHybridSDKCallback _Nullable )completion{
  
  if ([event isEqualToString:@"openLinkByParam"]){
        
        NFHybridNewsInfo * newsInfo = [NFHybridNewsInfo convertToModel:params[@"search"]];
        
        // 初始化详情页
        NFHybridContainerController * detailVC = [[NewsFeedsHybridSDK sharedSDK] instantiateContainerWithNewsInfo:newsInfo];
        detailVC.navTitle = newsInfo.st;
        
       	// 跳转到详情页
       	...
        
        return YES;
        }
      return NO;
}
```

* openLink

  在配置广告的情况下，点击打开广告时，会发起`openLink`事件。
  用户可通过该事件拿到`url`参数跳转广告页。
  如果未实现该事件，默认使用Safari打开广告。

```objc
- (BOOL)webViewContainer:(NFHybridContainerController *)container receiveEvent:(NSString *)event params:(NSDictionary *)params completion:(NFHybridSDKCallback)completion {
    
    if ([event isEqualToString:@"openLink"]) {
        
        // 获取url参数
        NSString *url = params[@"url"];
        NSString *type = params[@"type"];
        
        if ([type isEqualToString:@"ad"]) {
            //根据url跳转广告页
            ...
        } 
        return YES;
    }
    return NO;
}
```

* negativeFeedBack

点击列表页负反馈按钮(x关闭按钮)，如果在init事件中设置@"customNegativeFeedBack": @(YES)，会发起`negativeFeedBack`事件。
用户可通过该事件拿到`feedbackInfo`参数自定义负反馈页面

```objc
- (BOOL)webViewContainer:(NFHybridContainerController *)container receiveEvent:(NSString *)event params:(NSDictionary *)params completion:(NFHybridSDKCallback)completion {

if ([event isEqualToString:@"negativeFeedBack"]) {

	NSArray *fbInfo = params[@"feedbackInfo"];
	self.feedbackInfo = fbInfo;

	//根据info绘制UI
	...
	//回传选中的value数组
	completion(@{
			//注意：此处value传递选中数组
			@"dreason": @[self.feedbackInfo[0][@"value"]]
		}
		, nil);
		return YES;
	}
	return NO;
}
```



#### 3. 自定义导航栏

SDK中提供了自定义导航栏的接口，若用户需要使用自定义导航栏，需设置`NFHybridContainerCustomNavigationBarDelegate`，并传当前的控制器。

```objc
   NFHybridContainerController * detailVC = [[NewsFeedsHybridSDK sharedSDK] instantiateContainerWithNewsInfo:newsInfo]; 
   detailVC.customNavigationBarDelegate = self;
   detailVC.parentVC = self.hybridContainerController; // 传入当前VC，用于隐藏NavigationBar以及pop到上一页面
   detailVC.navTitle = newsInfo.st; // 导航栏标题 
   [self.navigationController pushViewController:detailVC animated:YES];
```

`NFHybridContainerCustomNavigationBarDelegate`提供的接口方便用户自定义实现相关推荐的返回操作。如：在相关推荐中默认返回到信息流列表页，用户可通过代理方法拦截返回实现只返回到上一级页面，若用户不使用自定义导航栏，可获取到`webView`自行实现。

```objc
- (void)hybridContainerCustomNavLeftItemClicked {
    if ([self.detailVC.webView canGoBack]) {
        [self.detailVC.webView goBack];
    } else {
        if (self.navigationController.viewControllers.count > 1){
            [self.navigationController popViewControllerAnimated:YES];
            return;
        }
        
        if (self.presentingViewController){
            [self.presentingViewController dismissViewControllerAnimated:YES completion:^{
                
            }];
            return;
        }
    }
}
```
#### 4.设置分享点击回调
```objc
/**
*  @method
*
*  @abstract
*  设置NewsFeedsHybridSDKDelegate的分享点击回调
*/
+ (void)setDelegate:(id<NewsFeedsHybridSDKDelegate>)delegate;
```

其中，NewsFeedsHybridSDKDelegate定义为：

```objc
/**
*  @method
*
*  @abstract
*  详情页面的分享按钮点击
*
*  @param shareInfo  分享需要用到的字段集合
*  @param type       分享类型，0：微信好友  1：朋友圈
*
*  @discussion
*  实现该回调，则分享按钮显示
*  未实现该回调，则分享按钮隐藏
*/
- (void)onShareClick:(NSDictionary *)shareInfo
		type:(NSInteger)type;
```

注意：

- 实现该回调，则新闻详情页面、图集详情页面、视频页面都会显示分享按钮；未实现该回调，则不会显示分享按钮

参考代码

```objc
- (void)onShareClick:(NSDictionary *)shareInfo
		type:(NSInteger)type 
{
	NSMutableString *url = [shareInfo[@"shareUrl"] mutableCopy];

	//拼接ios open url,scheme替换成用户app的。
	[url appendFormat:@"&iou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];

	//拼接android open url,scheme替换成用户app的。
	[url appendFormat:@"&aou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];

	//此处注意微信限制数据大小，超出则无法分享
	[WXApiRequestHandler sendLinkURL:url
			   TagName:shareInfo[@"infoType"]
			     Title:shareInfo[@"title"]
		   Description:shareInfo[@"summary"] ?  : shareInfo[@"source"]
			ThumbImage:shareInfo[@"thumbnail"]
			   InScene:type];
}
```
