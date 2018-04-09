## SDK概述

网易有料NewsFeedsHybridSDK是网易有料出品的一种轻量化解决方案。SDK兼容iOS 9.0+。

SDK通过Web方式接入网易有料内容，并使用jsbridge使native能够完成如微信分享等功能。



## 开发准备

1. #### SDK导入

   * 手动集成

     网易有料NewsFeedsHybridSDK 可通过手动下载，并添加到项目中集成使用。

     1. 将下载得到的NewsFeedsHybridSDK.framework和NFHybridBundle.bundle手动导入到工程中。
     2. 添加系统依赖库
        * WebKit
     3. 在 Build Settings -> Other Linker Flags 里，添加选项 -ObjC。

   * cocoapod集成(preferred )

     1. 首先编辑Podfile, 如果没有先在工程目录下运行`pod init`

        ```ruby
        #Podfile
        platform :ios, '9.0'
        #添加有料源
        source 'https://github.com/NetEaseYouliao/Specs.git'

        ...
          
        target :your_project_target do
          pod 'NewsFeedsHybridSDK'
          ...  #any other pod your project needed
        end
        ```

     2. 运行`pod install`

     ​

     当SDK导入工程后，需要修改一些配置

     1. 配置App Transport Security

        由于Web访问的部分资源为http，为了保证能够正常使用SDK，需在info.plist中配置App Transport Security Settings，并将Allow Arbitrary Loads设为YES，具体设置参考下图：

        ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG70.jpeg)

2. #### SDK使用

首先调用NewsFeedsHybridSDK初始化

```objective-c
[[NewsFeedsHybridSDK sharedSDK] startWithAppKey:@"appKey" appSecret:@"appSecret"];
```

获取SDK主入口

```objective-c
 NFHybridContainerController*_hybridContainerController = [[NewsFeedsHybridSDK sharedSDK] instantiateInitialContainer];
_hybridContainerController.delegate = SomeOtherClass<NFHybridDelegate> instance;  
```

用户接入可以有两种选择，推荐放入UINavigationController的栈中。

1. 直接作为独立的controller

   ```objective-c
   [[UINavigationController alloc] initWithRootViewController:_hybridContainerController];
   ```

   ​

2. 嵌入用户自定义的controller

   ```objective-c
   - (void)viewDidLoad {
   	...
   	[self.view addSubview:_hybridContainerController.view];
       [self addChildViewController:_hybridContainerController];
   }
   ```

实现NFHybridDelegate，完成web和native的通信。当前HybridSDK支持三种事件，分别是`init`, `openLink`, `share`.

* init 

  init是web页面加载时发起的事件，需要用户返回`appKey`， `secretKey`，`supportActions`，`supportSharePlatforms`。

  其中`supportActions`返回当前native支持的事件名，因为web端的更新会超前于native，所以web会隐藏native不支持的action。

  `supportSharePlatforms`返回当前native支持的分享平台，其中0表示微信分享用户，1表示微信朋友圈。native需要自行判断微信的安装情况来返回参数。暂不支持其它平台。

* openLink

  当打开新闻详情时，会发起`openLink`事件，同时携带`url`参数。该事件不关心返回结果。

* share

  当用户点击分享时，会发起该事件。事件参数是`platform`，`link`，`imgUrl`，`title`，`desc`。`platform`和`supportSharePlatforms`相同。该事件不关心返回结果。

  示例代码如下：

```objective-c
- (BOOL)webViewContainer:(NFHybridContainerController *)container receiveEvent:(NSString *)event params:(NSDictionary *)params completion:(NFHybridSDKCallback)completion {

    if ([event isEqualToString:@"init"]) {
        if (completion) {
            NSMutableArray *platform = [@[] mutableCopy];
            if ([WXApi isWXAppInstalled]) {
                [platform addObject:@(0)];
                [platform addObject:@(1)];
            }
            completion(@{
                         @"appKey": [NewsFeedsHybridSDK sharedSDK].appKey,
                         @"secretKey": [NewsFeedsHybridSDK sharedSDK].appSecret,
                         @"supportActions": @[],
                         @"supportSharePlatforms": platform
                         }
                       , nil);
        }
        return YES;
    } else if ([event isEqualToString:@"openLink"]) {
        NSString *url = params[@"url"];
        NSString *type = params[@"type"];
        if ([type isEqualToString:@"ad"]) {
            [self openADBySafariWithURL:url];
        } else {
            [self routeToDetailWithURL:url withSource:@""];
        }
        
        return YES;
        
    }else if ([event isEqualToString:@"openLinkByParam"]){ // 结构化数据拼接成URL
        
        NFHybridNewsInfo * newsInfo = [[NFHybridNewsInfo alloc]init];
        newsInfo.ak = params[@"search"][@"ak"];
        newsInfo.ctag = params[@"search"][@"ctag"];
        newsInfo.dt = params[@"search"][@"dt"];
        newsInfo.newsID = params[@"search"][@"id"];
        newsInfo.info = params[@"search"][@"info"];
        newsInfo.it = params[@"search"][@"it"];
        newsInfo.p = params[@"search"][@"p"];
        newsInfo.rid = params[@"search"][@"rid"];
        newsInfo.sk = params[@"search"][@"sk"];
        newsInfo.st = params[@"search"][@"st"];
        newsInfo.unid = params[@"search"][@"unid"];
        newsInfo.cid = params[@"search"][@"cid"];
        
        // 环境判断
        NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
        NSString *env = [infoDict objectForKey:@"HostEnvironment"];
        NSString *url;
        if ([env isEqualToString:@"qa"]) {
            url = @"http://youliao-qa.163yun.com/h5/detail/?s=seve";
        } else if([env isEqualToString:@"dev"]) {
            url = @"http://testyouliao.163yun.com/h5/detail/?s=seve";
        } else {
            url = @"http://youliao.163yun.com/h5/detail/?s=seve";
        }
        
        NSString * urlStr = [NSString stringWithFormat:@"%@&ak=%@&ctag=%@&dt=%@&id=%@&info=%@&it=%@&p=%@&rid=%@&sk=%@&st=%@&unid=%@&cid=%@",
                             url,
                             newsInfo.ak,
                             newsInfo.ctag,
                             newsInfo.dt,
                             newsInfo.newsID,
                             newsInfo.info,
                             newsInfo.it,
                             newsInfo.p,
                             newsInfo.rid,
                             newsInfo.sk,
                             newsInfo.st,
                             newsInfo.unid,
                             newsInfo.cid];
        
         [self routeToDetailWithURL:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding] withSource:[newsInfo.st stringByRemovingPercentEncoding]];
        
        
    }else if ([event isEqualToString:@"share"])  {
        NSString *imageURLString = params[@"imgUrl"];
        NSMutableString *originLink = [params[@"link"] mutableCopy];
        NSURLComponents *c = [NSURLComponents componentsWithString:originLink];
        NSMutableArray *queryItems = [[c queryItems] mutableCopy];
        NSString *infoType;
        NSString *infoId;
        NSString *producer;
        
        for (NSURLQueryItem *item in queryItems) {
            if ([item.name isEqualToString:@"id"]) {
                infoId = item.value;
            } else if ([item.name isEqualToString:@"it"]) {
                infoType = item.value;
            } else if ([item.name isEqualToString:@"p"]) {
                producer = item.value;
            }
        }
        
        [originLink appendString:@"&fss=1"];
        [originLink appendFormat:@"&ak=%@", [self encodeParameter:[NewsFeedsHybridSDK sharedSDK].appKey]];
        [originLink appendFormat:@"&sk=%@", [self encodeParameter:[NewsFeedsHybridSDK sharedSDK].appSecret]];
        [originLink appendFormat:@"&iou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", infoId, infoType, producer]]];
        [originLink appendFormat:@"&aou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", infoId, infoType, producer]]];
        
        if (imageURLString.length > 4) {
            [[SDWebImageDownloader sharedDownloader] downloadImageWithURL:[NSURL URLWithString:imageURLString] options:0 progress:nil completed:^(UIImage * image, NSData * data, NSError * error, BOOL finished) {
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    [WXApiRequestHandler sendLinkURL:originLink
                                             TagName:@"share"
                                               Title:params[@"title"]
                                         Description:params[@"desc"] ? : @""
                                          ThumbImage:[UIImage thumbnailForImageOfData:data maxSidePixels:100]
                                             InScene:[params[@"platform"] intValue]];
                });
                
                
            }];
        } else {
            [WXApiRequestHandler sendLinkURL:originLink
                                     TagName:@"share"
                                       Title:params[@"title"]
                                 Description:params[@"desc"]
                                  ThumbImage:nil
                                     InScene:[params[@"platform"] intValue]];
        }
        
        return YES;
    }
    return NO;
}
```

以上delegate的返回值，表示native有没有响应此事件。如果响应请返回YES，否则返回NO。sdk将根据函数返回值判断是否执行默认操作。