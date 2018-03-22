# iOS DATA SDK 集成文档

## SDK概述

网易有料NewsFeedsSDK为移动应用提供内容智能分发功能，对外提供较为简洁的API接口，方便第三方应用快速的集成并实现内容分发功能。SDK兼容iOS 8.0+。

NewsFeedsSDK提供的功能如下：

- 获取频道列表
- 获取新闻列表
- 获取新闻详情
- 新闻详情的webView封装
- 广告模块（v1.2新增）
- 推送相关（v1.2新增）
- 用户行为采集上传

---

## SDK类说明

网易有料NewsFeedsSDK主要提供了以下的类：

- ==NewsFeedsSDK== 整个SDK的主入口，单例，主要提供初始化，配置用户信息，加载频道、新闻列表的功能
- ==NFTracker==用户行为追踪的单例类
- ==NFNewsDetailView==封装了webView的view类，提供展示新闻详情的功能，同时==NFNewsDetailViewDelegate==提供view加载及操作过程中的回调
- ==NFChannel==频道列表的model类
- ==NFNews==新闻列表的model类
- ==NFNewsDetail==新闻详情的model类
- ==GeTuiSdk==个推SDK的入口类

---

## 外部依赖
### 广告（v1.2新增）

从v1.2开始，我们的SDK内嵌了腾讯广点通广告SDK，用户在使用本SDK的时候，确保工程中未使用广点通的SDK，为了确保成功拉取到广告，需在CMS上开启广告业务开关。

### 推送相关（v1.2新增）

使用的是个推的第三方推送SDK，相关使用参考个推官网开发使用文档：http://docs.getui.com/mobile/ios/api/

---

## 开发准备

网易有料NewsFeedsSDK 可通过手动下载，并添加到项目中集成使用。

1. 将下载得到的NewsFeedsSDK.framework和NFBundle.bundle导入到工程中。

    将相关包拖进工程的时候，参考下图选项
    
    ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG81.jpeg)

    保证==Build Phases -> Link Binary With Libraries==中包含==NewsFeedsSDK.framework==

    保证==Build Phases -> Copy Bundle Resources==中包含==NFBundle.bundle==

2. 添加其他的依赖库
    
    -   AdSupport.framework
    -   CoreLocation.framework
    -   UserNotifications.framework
    -   libsqlite3.tbd
    -   libz.tbd
    -   MobileCoreServices.framework
    -   StoreKit.framework
    -   SafariServices.framework

3. 在 ==Build Settings== -> ==Other Linker Flags== 里，添加选项 ==-ObjC==。

4. 由于Appstore禁止不使用广告而采集IDFA的app上架，SDK中采集IDFA作为设备ID以达到更为精确的个性化推荐的结果。如果应用使用SDK而未集成任何广告服务，请按照以下填写Appstore中的IDFA选项：

![image](http://images.9liuda.com/opensdk/web3.0/image/documentJoin/10.png)

这里建议勾选图中所示的选项。

5. 配置App Transport Security

    由于NewsFeedsSDK返回的video和image的url为http的方式，为了保证能够正常播放视频及加载图片，需在info.plist中配置==App Transport Security Settings==，并将==Allow Arbitrary Loads==设为==YES==，具体设置参考下图：
    
    ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG70.jpeg)

    ==注意==：若用户未进行该设置，为了保证图片正常展示以及视频的正常播放，需要调用setHttpsOn接口进行配置，该接口的具体使用参考下文

---

## 初始化SDK

1.在AppDelegate.m中导入头文件#import ==<NewsFeedsSDK/NewsFeedsSDK.h>==

2.添加SDK的初始化方法

```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [[NewsFeedsSDK sharedInstance] startWithAppKey:appKey
                                         appSecret:appSecret];
    
    return YES;
}
```

其中==appKey==和==appSecret==可在官网平台创建产品时获得

## SDK接口说明
### 日志管理

调用api，配置打印日志的最大级别，默认不打印任何日志

- 定义

```objc
- (void)setLogLevel:(NFSDKMaxLogLevel)maxLevel;
```

- 示例

```objc
[[NewsFeedsSDK sharedInstance] setLogLevel:NFSDKMaxLogLevelDebug];
```

maxLevel可配置为4种级别：
*  ==NFSDKMaxLogLevelNone== 关闭日志
*  ==NFSDKMaxLogLevelError==   只打印error日志
*  ==NFSDKMaxLogLevelWarning== 打印error和warning日志
*  ==NFSDKMaxLogLevelInfo==    打印error、warning和info日志
*  ==NFSDKMaxLogLevelDebug==   打印所有日志

---

### 设置URL https状态

- 定义

```objc
/**
 *  @method
 *
 *  @abstract
 *  设置加载网络返回的图片、视频URL为https，默认为NO
 *  YES为打开，NO为关闭
 *
 *  @param httpsOn  图片、视频URL的https的开关状态
 *
 *  @discussion
 *  默认加载信息流返回的网络图片视频URL存在为http的情况
 *  用户可以通过打开该开关，获取URL为https的链接
 */
- (void)setHttpsOn:(BOOL)httpsOn;
```

- 示例

```objc
[[NewsFeedsSDK sharedInstance] setHttpsOn:YES];
```

---

### 配置用户标识

为了获取更加准确的跨平台的个性化推荐内容，鼓励用户配置应用的唯一userId

- 定义

```objc
/**
 *  @method
 *
 *  @abstract
 *  配置用户Id
 *
 *  @param userId  用户Id，记录用户相关信息
 *
 *  @discussion
 *  设置用户的userId可以更精确跨平台的进行内容的推荐
 *  若不调用该接口，系统将默认采用匿名的设备Id进行相关用户行为分析
 *  
 *  若设置userId为nil或者空，则会清除保存的用户信息
 */
- (void)setUserId:(NSString *)userId;
```
若不调用该接口，系统将默认采用匿名的设备Id进行相关用户行为分析。

鼓励用户在登录或者注册时候调用，SDK会记住该字段，原则上只调用一次即可，多次调用以最后一次调用为主，如果用户想清除以前配置的userId，可以将该字段设为空即可。

- 示例

```objc
[[NewsFeedsSDK sharedInstance] setUserId:@"我是userId"];
```

---

### 获取当前SDK的版本号

```objc
/**
 *  @method
 *
 *  @abstract
 *  获取当前SDK的版本号
 */
- (NSString *)version;
```

- 示例

```objc
NSString *version = [[NewsFeedsSDK sharedInstance] version];
```

返回字段格式：==v1.2.4==

---

### 获取deviceId
```objc
/**
 *  @method
 *
 *  @abstract
 *  获取deviceId
 */
- (NSString *)deviceId;
```

- 示例

```objc
NSString *deviceId = [[NewsFeedsSDK sharedInstance] deviceId];
```
---

### 加载频道列表
- 定义

```objc
/**
 *  @method
 *
 *  获取所有新闻频道列表
 *
 *  @param block 查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如频道列表更新等
 */
- (void)loadChannelsWithBlock:(void(^)(NFChannels *channelList, NSError *error))block;
```

- 示例

```objc
 [[NewsFeedsSDK sharedInstance] loadChannelsWithBlock:^(NFChannels *channelList, NSError *error) {
        if (error) {
            //网络请求加载失败
            return;
        }
        
    //网络请求加载成功
    
}];
```

- 网络请求接口回调说明

a. 网络请求成功

error为nil，channelList为网络返回数据，回调中channelList的类型为NFChannels。

其中，频道列表NFChannels的字段说明如下：

名称 | 类型 | 描述
---|---|---
channels| NSArray(NFChannelInfo)| 频道列表

其中，单个频道NFChannelInfo的字段说明如下：

名称 | 类型 | 示例 | 描述
---|---|---|---
channelId| NSString| | 频道ID
channelName | NSString ||频道名称
order | NSInteger | 1 | 频道显示的顺序
channelType | NSInteger | |频道类型，0：按类别聚合   1：按话题聚合   2：按自定义来源聚合    3：地域频道   4：自营频道

b.网络请求失败

channelList为nil，error为包含相应错误code的实例，具体错误码和错误原因说明请参考网易有料Api Server文档。

---

### 加载新闻列表
SDK提供多个加载新闻列表的接口，用户可根据需要传入不同的参数组合。自营频道新闻列表数据返回策略与非自营频道有所不同，具体策略请参考网易有料Api Server文档。

-  定义

```objc
/**
 *  @method
 *
 *  获取所有新闻列表
 *
 *  @param channel  需要获取新闻的频道
 *  @param pageSize 需要获取普通新闻列表的条目数
 *  @param type     0：接口返回普通新闻   1：接口返回头图新闻、置顶新闻以及普通新闻
 *  @param block    查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadNewsWithChannel:(NFChannelInfo *)channel
                   pageSize:(NSUInteger)pageSize
                   loadType:(NSInteger)type
                      block:(void(^)(NFNews *newsList, NSError *error))block;
```

```objc
/**
 *  @method
 *
 *  获取所有新闻列表
 *
 *  @param channel    需要获取新闻的频道
 *  @param pageSize   需要获取普通新闻列表的条目数
 *  @param type       0：接口返回普通新闻   1：接口返回头图新闻、置顶新闻以及普通新闻
 *  @param longitude  经度
 *  @param latitude   维度
 *  @param block      查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadNewsWithChannel:(NFChannelInfo *)channel
                   pageSize:(NSUInteger)pageSize
                   loadType:(NSInteger)type
                  longitude:(double)longitude
                   latitude:(double)latitude
                      block:(void(^)(NFNews *newsList, NSError *error))block;
```

- 请求参数 loadType

    loadType为新闻列表内容组合类型，取值0或1，默认取0。取值说明如下：
    
    1：下拉刷新时，传入1，返回普通新闻、头图新闻、置顶新闻。
    
    0：上拉加载更多时，传入0，只返回普通新闻。

- 请求参数 num

    num为期望返回的新闻列表长度，默认10条。num仅为普通新闻的条数，头图或置顶新闻不计算在内。

- 接口返回NFNews
    
    新闻列表数据模型（NFNews）各字段说明如下：
    
    字段 | 类型 | 描述
    ---|---|---
    infos| NFNewsInfo| 普通新闻
    banners | NFNewsInfo |轮播图或头图新闻
    tops | NFNewsInfo |置顶新闻
    
    其中，单个新闻（NFNewsInfo）按类型（infoType）可分为：文章(article)、图集(picset)、视频(video)。数据模型具体字段说明，请参考附录。
    
    (1) 轮播图
    
    轮播图包含的新闻类型有：文章(article)、图集(picset)、广告(ad)。
    
    这里要注意的是，只有在拉取广告的必要条件满足的情况下，广告才能拉取成功，返回的轮播图中才可能出现广告。
    
    (2) 置顶新闻
    
    置顶新闻包含的新闻类型有：文章(article)、图集(picset)、视频(video)。
    
    这里要注意的是，置顶新闻不含广告(ad)。
    
    (3) 普通新闻列表
    
    普通新闻包含的新闻类型有：文章(article)、图集(picset)、视频(video)、广告(ad)。
    
    这里要注意的是，只有在拉取广告的必要条件满足的情况下，广告才能拉取成功，返回的普通新闻列表中才可能出现广告。
    
    下拉刷新时，接口返回头图新闻、置顶新闻、普通新闻，典型的Feed流展示逻辑是按照从上到下为头图新闻、置顶新闻、普通新闻的顺序，将下拉刷新的数据插入到当前Feed流头部；上拉加载更多时，接口只返回普通新闻，典型的Feed流展示逻辑是将上拉加载更多的数据追加到当前Feed流尾部。App开发人员可参考如下示例代码进行数据展示：

```objc
[[NewsFeedsSDK sharedInstance] loadNewsWithChannel:self.channel pageSize:10 loadType:1 block:^(NFNews *newsList, NSError *error) {
    if (error) {
        //处理错误逻辑
        return;
    }
    
    // 移除旧的头图和置顶
    if (isRefresh) {
        // 移除原有头图
        [self removeBanners];
        // 删除原有置顶
        [self removeTops];
    }
    
    if (newsList) {
        // 解析普通新闻
        NSArray *newsInfos = newsList.infos;
        if (newsInfos.count > 0) {
            int count = newsInfos.count;
            if(isRefresh) {
	            for (int i = 0; i < count; i++) {
	                NFNewsInfo *newsInfo = newsInfos[i];
	                // 在头部插入
	                [self addTopImp:newsInfo];
	            }
            } else {
	            for (int i = 0; i < count; i++) {
	                NFNewsInfo *newsInfo = newsInfos[i];
	                // 添加到尾部
	                [self addBottomImp:newsInfo];
		         }
            }
        }
    }
    
    // 展示新的头图和置顶
    if (isRefresh) {
        // 解析置顶
        NSArray *tops = newsList.tops;
        // 在头部插入
        [self bindTops:tops];

        // 解析头图
        NSArray *banners = newsList.banners;
        // 在头部插入
        [self bindBanner:banners];
    }

    // 刷新视图
    [self.tableView reloadData];
}];
```

单个新闻摘要NFNewsInfo部分字段说明如下，完整字段请参考附录：

名称 | 类型 | 描述
---|---|---|---
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型
title | NSString | 新闻标题
summary | NSString | 新闻简介
source | NSString | 新闻来源
updateTime | NSString | 新闻更新时间
imgType | NSInteger | 缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
num | NSInteger | 图集中图片数量,只有在图集情况下才会返回该字段
thumbnails | NSArray (==NFNewsImage==) | 新闻缩略图
videos | NSArray (==NFNewsVideo==) | 视频源信息,只有在视频情况下才会返回该字段
ad | NFAdInfo | 广告信息

这里要注意的是，单个新闻摘要按照 imgType 可分为三图模式、大图模式、单图模式、无图模式。不管是哪一种imgType，具体的缩略图均存于thumbnails字段。

- 错误码说明

newsList为nil，error为包含相应错误code的实例，具体错误码和错误原因说明请参考网易有料Api Server文档。

==error的特殊处理：== 并不是所有error都是无意义的网络请求失败，其中
    
    error.code == 2000    表示账号过期，需联系网易有料客服进行处理

    error.code == 4000    表示后台管理系统删除了某个频道，App在未重新拉取更新频道列表的情况下，获取该频道下的新闻列表会返回code为4000的错
    
    error.code == 4004    表示该频道下拉取不到任何的新闻列表信息，会返回code为4004的错误

### 加载相关推荐列表
- 定义

```objc
/**
 *  @method
 *
 *  获取相关推荐新闻列表
 *
 *  @param newsInfo   需要获取新闻的相关推荐新闻
 *  @param longitude  经度
 *  @param latitude   维度
 *  @param block      查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻列表更新等
 */
- (void)loadRelatedNewsWithNews:(NFNewsInfo *)newsInfo
                      longitude:(double)longitude
                       latitude:(double)latitude
                          block:(void(^)(NFNews *newsList, NSError *error))block;
```

- 示例 

```objc
//请求相关推荐
[[NewsFeedsSDK sharedInstance] loadRelatedNewsWithNews:self.newsInfo block:^(NFNews *newsList, NSError *error) {
    if (error) {
        //处理加载相关推荐新闻失败的业务逻辑
        
        return;
    }
            
    if (newsList.infos.count == 0) {
         //处理加载相关推荐新闻列表为空的业务逻辑
         
        return;
    }
            
    weakSelf.relatedNewsArray = [newsList.infos copy];
            
    //处理加载相关推荐新闻列表成功的业务逻辑
    
}];

```

----

### 各种类型的新闻展现

当新闻列表中的单个新闻（NFNewsInfo）被点击时，App开发人员需要根据新闻类型（infoType）实现对应的点击事件响应。例如，当infoType为video时，需要开始播放视频或者跳转到视频播放页面；当infoType为ad时，需要跳转到广告落地页面；当infoType为article时，需要跳转到文章详情页面；当infoType为picset时，需要跳转到图集展示页面。可参考如下代码片段：

```objc
NSString *infoType = newsInfo.infoType;
if ([infoType isEqualToString:@"video"]) {
    // 开始视频播放
	   ...
    return;
}

if ([infoType isEqualToString:@"ad"]) {
    // 跳转到广告落地页
	[[NewsFeedsSDK sharedInstance] clickAd:newsInfo.ad.data];
    return;
}

if ([infoType isEqualToString:@"article"]) {
    // 跳转到文章类新闻展示页
	   ...
    return;
}

if ([infoType isEqualToString:@"picset"]) {
    // 跳转图集类新闻展示页
	   ...
    return;
}
```

单个新闻数据模型NFNewsInfo部分字段说明如下：

名称 | 类型 | 描述
---|---|---|---
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型：文章(article)、图集(picset)、视频(video)
imgType | NSInteger | 缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
num | NSInteger | 图集中图片数量,只有在图集情况下才会返回该字段
thumbnails | NSArray (==NFNewsImage==) | 新闻缩略图
videos | NSArray (==NFNewsVideo==) | 视频源信息,只有在视频情况下才会返回该字段
ad | NFAdInfo | 广告信息

#### 1. 视频类新闻展现

由NFNewsInfo数据模型可知，当 infoType 为 video 时，NFNewsInfo中的videos字段会给出视频源信息。

视频源数据模型NFNewsVideo字段说明如下：
名称 | 类型 | 描述
---|---|---
cover | NSString | 视频封面图
largeCover | NSString | 视频封面大图
playsize | NSInteger | 播放类型，0：表示4：3宽高比，1：表示16：9宽高比
duration | NSInteger | 视频时长，单位为秒
mp4SdUrl | NSString | mp4格式视频链接，标清
mp4HdUrl | NSString | mp4格式视频链接，高清
m3u8SdUrl | NSString | m3u8格式视频链接，标清
m3u8HdUrl | NSString | m3u8格式视频链接，高清
sdUrl | NSString | flv格式视频链接，标清
hdUrl | NSString | flv格式视频链接，高清
shdUrl | NSString | flv格式视频链接，shd

App开发人员可根据该视频源信息完成视频的下载、播放等交互逻辑。

#### 2. 广告类新闻展现

当 infoType 为 ad 时，NFNewsInfo中的ad字段会给出广告信息，广告信息包含展示一条广告所需的标题、描述、图片地址等等。广告视图的渲染由App开发人员完成。我们的SDK封装了广告点击后的落地页，点击广告时，用户可以直接调用 clickAd 接口跳转到广告落地页。

- 单条广告数据模型：NFAdInfo

名称 | 类型 | 描述
---|---|---
mediumId | NSString | 应用ID
producer | NSString | 广告来源
adPlacementId | NSString | 广告位
ip | NSString | 公网ip（inmobi类型广告需要）
data | GDTNativeAdData | 原生广告数据
exposureURLArray | NSArray | InMoBi广告曝光url
clickURLArray | NSArray | InMoBi广告点击url
isApp | BOOL | InMoBi广告是否是App
landingURL | NSString | InMoBi广告落地页
title | NSString | 广告标题
imgType | NSInteger | 缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
imageUrl | NSString | 广告图片URL


广告视图的渲染由App开发人员完成。我们的SDK封装了广告点击后的落地页，为了确保广告准确计费，需要App开发人员自行调用广告曝光和广告点击接口，在调用广告点击接口之前，一定要先调用广告曝光接口。

##### (1)广告展示的弹出页面

- 定义

```objc
/**
 *  @method
 *
 *  @abstract
 *  [接入广告模块必选]开发者需传入用来弹出广告目标页的ViewController，一般为当前ViewController
 */
- (void)setAdPresentController:(UIViewController *)controller;
```

- 示例

```objc
[[NewsFeedsSDK sharedInstance] setAdPresentController:(UIViewController *)NFAPPDELEGATE().tabBarController];
```

---

##### (2)广告数据渲染完毕调用方法

- 定义

```objc
/**
 *  @method
 *
 *  广告数据渲染曝光调用的方法
 *  
 *  @param adInfo 广告渲染的adInfo
 *
 *  @discussion
 *  [必选]广告数据渲染完毕，即将展示曝光时需调用本方法。
 *
 */
- (void)attachAd:(NFAdInfo *)adInfo;
```

- 示例

```objc
 [[NewsFeedsSDK sharedInstance] attachAd:newsInfo.ad];
```

---

##### (3)广告点击调用方法

- 定义

```objc
/**
 *  @method
 *
 *  广告点击调用方法
 *
 *  @param adInfo 用户点击的广告adInfo
 *
 *  @discussion
 *  当用户点击广告时，开发者需调用本方法，系统会弹出内嵌浏览器、或内置AppStore、
 *  或打开系统Safari，来展现广告目标页面
 */
- (void)clickAd:(NFAdInfo *)adInfo;
```
- 示例

```objc
 [[NewsFeedsSDK sharedInstance] clickAd:newsInfo.ad];
```

---

#### 3. 文章类新闻展现

NFNewsInfo数据模型只给出了文章类新闻的摘要信息，若要获取文章正文，需要调用`loadNewsDetailWithNews`接口拉取文章正文信息。

- 定义

```objc
/**
 *  @method
 *
 *  获取新闻的详情
 *
 *  @param newsInfo 需要获取新闻
 *  @param block    查询结果回调
 *
 *  @discussion
 *  可以在block中处理自己的业务逻辑，比如新闻详情的展示等
 */
- (void)loadNewsDetailWithNews:(NFNewsInfo *)newsInfo
                         block:(void(^)(NFNewsDetail *newsDetail, NSError *error))block;
```

- 示例

```objc
[[NewsFeedsSDK sharedInstance] loadNewsDetailWithNews:_newsInfo block:^(NFNewsDetail *newsDetail, NSError *error) {
    if (error) {
            //网络请求加载失败
            return;
        }
        
    //网络请求加载成功
}];
```

- 接口返回NFNewsDetail

新闻详情NFNewsDetail字段说明：

名称 | 类型 | 描述
---|---|---|---
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型
category| NSString| 新闻类目
title | NSString | 新闻标题
publishTime | NSString | 发布时间
source | NSString | 新闻来源
sourceLink | NSString | 原文地址
content | NSString | 新闻正文
tag | NSString | 标签
imgs | NSArray (==NFNewsDetailImage==) | 新闻缩略图
videos | NSArray (==NFNewsVideo==) | 视频源信息,只有在视频情况下才会返回该字段
ad | NFAdInfo | 广告信息

从模型字段描述可知，当NFNewsDetail.infoType为article时，文章类新闻正文由content字段给出，为支持图片异步加载，content中img标签被替换成了${{index}}$，index从0开始。正文中图片链接存于imgs字段。具体说明请参考网易有料Api Server文档。

当NFNewsDetail.infoType为picset时，imgs字段表示图集中图片列表。

从v1.2开始，用户请求新闻详情成功后，返回的 NFNewsDetails 中可能包含广告。广告信息存于ad字段。广告的渲染与点击操作与新闻列表中的广告类似。

#### 4. 图集类新闻展现
NFNewsInfo数据模型只给出了图集类新闻的缩略图信息，若要获取图集的图片列表，需要调用`loadNewsDetailWithNews`接口拉取图集详情信息。`loadNewsDetailWithNews`接口定义及示例请参考文章类新闻展现部分。

当NFNewsInfo.infoType为picset时，imgs字段表示图集中图片列表。

新闻详情NFNewsDetail部分字段说明：

名称 | 类型 | 示例 | 描述
---|---|---|---
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型
imgs | NSArray (==NFNewsDetailImage==) | 图片列表

单个图片数据模型NFNewsDetailImage字段说明：

名称 | 类型 | 描述
---|---|---|---
url | NSString | 缩略图url
height | NSInteger | 缩略图高度
width | NSInteger | 缩略图宽度
pictype | NSString | 新闻图片类型
note | NSString | 图片描述，图集该字段才有值

App开发人员需要根据图片列表自行实现图集展示页。

---


### 用户行为分析模块（NFTracker类）
我们鼓励合作方接入用户行为统计接口，通过采集用户行为，结合网易有料专业数据挖掘能力和智能推荐算法得到的用户画像将会更精准。

值得注意的是，用户行为统计接口均封装在 NFTracker 中。请确保 SDK 初始化后再调用 NFTracker 中的接口

#### 曝光事件

为了增强推荐效果，当新闻曝光时，建议调用曝光接口进行曝光的行为事件统计。

-  定义

```objc
/**
 *  @method
 *
 *  @abstract
 *  调用trackNewsExposure接口，表示新闻曝光
 *
 *  @discussion
 *  该接口的调用需在某新闻列表暴露在当前屏幕0.5s以上调用，若是曝光时间太短，影响推荐数据
 *  该接口的使用建议咨询相关开发人员
 *
 *  @param newsInfo       新闻
 */
- (void)trackNewsExposure:(NFNewsInfo *)newsInfo;
```

- 示例

```objc
[[NFTracker sharedTracker] trackNewsExposure:newsInfo];
```

==注意：== 同一篇新闻曝光可以多次上传，后台会过滤，建议为了节省流量，app开发者应尽量避免同一篇新闻的曝光事件多次上传

==使用说明：==

该接口的调用需在某新闻列表暴露在当前屏幕0.5s以上调用，若是曝光时间太短，影响推荐数据。

- 新闻列表曝光说明

##### (1)在cell开始展示的时候记录时间

```objc
((NFNewsListBaseCell *)cell).cellVisibleTime = [[NSDate date] timeIntervalSince1970] * 1000;
```

##### (2)在cell结束展示的时候曝光

```objc
- (void)tableView:(UITableView *)tableView didEndDisplayingCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath {
    NSTimeInterval currentTime = [[NSDate date] timeIntervalSince1970] * 1000;
    if (((NFNewsListBaseCell *)cell).cellVisibleTime && ![_exposureInfos containsObject:((NFNewsListBaseCell *)cell).newsInfo.infoId]) {
        if (currentTime - ((NFNewsListBaseCell *)cell).cellVisibleTime > 500) {
            [[NFTracker sharedTracker] trackNewsExposure:((NFNewsListBaseCell *)cell).newsInfo];
            [_exposureInfos addObject:((NFNewsListBaseCell *)cell).newsInfo.infoId];
        }
    }
}
```

- banner轮播图曝光说明

```objc
- (void)bannerDidScroll:(NFTopicBannerCell *)cell info:(NFNewsInfo *)newsInfo {
    NSArray *cells = [_tableView visibleCells];
    if ([cells containsObject:cell] && ![_exposureInfos containsObject:newsInfo.infoId] && (newsInfo.infoId)) {
        [[NFTracker sharedTracker] trackNewsExposure:newsInfo];
        [_exposureInfos addObject:newsInfo.infoId];
    }
}
```

- 列表未滚动时，页面cell曝光

由于上述列表页的曝光只有在cell结束展示的时候才曝光，当页面未滚动时，停留在当前页面的cell按照上述方法无法上传曝光事件，因此需要额外时机上传当前页面的cell的曝光事件

```objc
- (void)trackCellExposure {
    //获取当前页面的可视的cells
    NSArray *cells = [_tableView visibleCells];
    for (UITableViewCell *cell in cells) {
        if ([cell isKindOfClass:[NFNewsListBaseCell class]] && !(((NFNewsListBaseCell *)cell).newsInfo.ad)) {
            [[NFTracker sharedTracker] trackNewsExposure:((NFNewsListBaseCell *)cell).newsInfo];
            [_exposureInfos addObject:((NFNewsListBaseCell *)cell).newsInfo.infoId];
        }
    }
}
```

调用的时机为：

    1、应用退到后台
    2、不同频道页面的切换
    3、一些其他因素引起的页面不可见（比如app底部tabbar的切换等），需app开发者按需调用

---


#### 点击事件

当文章或图集被点击，视频开始播放时，建议调用该接口进行点击行为上报。

-  定义

```objc
 /**
 *  @method
 *
 *  @abstract
 *  调用trackNewsClick接口，表示点击了一个新闻列表
 *
 *  @param newsInfo       新闻
 */
- (void)trackNewsClick:(NFNewsInfo *)newsInfo;
```

- 示例

```objc
[[NFTracker sharedTracker] trackNewsClick:newsInfo];
```

---

#### 浏览开始的事件收集

- 定义

```objc
/**
 *  @method
 *
 *  @abstract
 *  初始化事件的计时器。
 *
 *  @discussion
 *  若需要统计某个事件的持续时间，先在事件开始时调用 trackBrowseBegin:"news" 记录事件开始时间，该方法并不会真正发
 *  送事件；随后在事件结束时，调用 trackBrowseEnd:"news"，SDK 会追踪 "news" 事件，
 *  并自动将事件持续时间记录
 *
 *  多次调用 trackBrowseBegin:"news" 时，事件 "news" 的开始时间以最后一次调用时为准。
 *
 *  注意：该接口一定是与trackBrowseEnd:"news"接口配合使用，且成对出现
 *
 *  @param newsInfo       新闻
 */
- (void)trackBrowseBegin:(NFNewsInfo *)newsInfo;
```

- 示例
- 
```objc
[[NFTracker sharedTracker] trackBrowseBegin:self.news];
```

---

#### 浏览结束的事件收集

当文章或图集浏览结束，视频播放暂停或结束时，建议调用该接口进行浏览行为上报。用户可以自主统计浏览时长和浏览进度一并上报。

- 定义
- 
```objc
/**
 *  @method
 *
 *  @abstract
 *  调用trackBrowseEnd接口，新闻详情浏览结束
 *
 *  @discussion
 *  注意：该接口一定是与trackBrowseBegin:"news"接口配合使用，且成对出现
 *
 *  @param newsInfo       新闻
 *  @param progress       浏览进度
 */
- (void)trackBrowseEnd:(NFNewsInfo *)newsInfo progress:(double)progress;
```
- 示例
- 
```objc
[[NFTracker sharedTracker] trackBrowseEnd:self.newsInfo progress:progress];
```
- 图集浏览行为统计

```objc
[[NewsFeedsSDK sharedInstance] loadNewsDetailWithNews:_newsInfo block:^(NFNewsDetail *newsDetail, NSError *error) {
        if (error) {
            //加载失败的业务逻辑处理
            return;
        }

        //加载成功，首先trackBegin统计时长
        [[NFTracker sharedTracker] trackBrowseBegin:weakSelf.newsInfo];
        
    }];
```

页面退出，例如页面被销毁时，可以认为图集浏览结束，调用trackBrowseEnd自动计算时长，并上报浏览事件。

```objc
double progress = (double)browseIndex / (double)imageCount;
[[NFTracker sharedTracker] trackBrowseEnd:self.newsInfo progress:progress];
```

==注意：== 两个一定配合使用

- 视频浏览行为统计
 
监听视频播放各个过程。开始视频播放时，调用trackBrowseBegin记录视频开始。当视频暂停或播放结束时，调用trackBrowseEnd自动计算时长，并上报视频浏览事件。示例代码片段如下：

```objc
- (void)play {
    //浏览开始事件
    [[NFTracker sharedTracker] trackBrowseBegin:self.news];
    
    //点击事件
    [[NFTracker sharedTracker] trackNewsClick:self.news];
}

- (void)pause {
    //浏览结束事件
    [[NFTracker sharedTracker] trackBrowseEnd:self.news progress:progress];
}

- (void)playerReachEnd {
    //浏览结束事件
    [[NFTracker sharedTracker] trackBrowseEnd:self.news progress:progress];
}

- (void)stop {
    //浏览结束事件
    [[NFTracker sharedTracker] trackBrowseEnd:self.news progress:progress];
}

```
获取视频播放进度示例代码如下：

```objc
//当前播放时长/总时长
double progress = [self currentPlaybackTime] / [self playerItemDuration];

//当前播放时长
- (NSTimeInterval)currentPlaybackTime {
    AVPlayerItem *playerItem = [_player currentItem];
    if (playerItem.status == AVPlayerItemStatusReadyToPlay) {
        return(CMTimeGetSeconds([playerItem currentTime]));
    }
    return 0;
}

//总时长
- (NSTimeInterval)playerItemDuration {
    AVPlayerItem *playerItem = [_player currentItem];
    if (playerItem.status == AVPlayerItemStatusReadyToPlay) {
        return(CMTimeGetSeconds([playerItem duration]));
    }
    return 0;
}

```

- 文章类新闻浏览事件

文章详情会被渲染到webview上，webview不可见的时间不应该计算到浏览时间内。

可以在webView加载页面成功后，调用trackBrowseBegin记录文章浏览的开始，示例代码如下：

```objc
- (void)webViewDidFinishLoad:(UIWebView *)webView {
    //文章浏览开始
    [[NFTracker sharedTracker] trackBrowseBegin:self.currentNews];
}
 
```

当页面销毁时，调用trackBrowseEnd自动计算时长，并上报文章浏览事件。示例代码片段如下：

```objc
- (void)dealloc {
    [[NFTracker sharedTracker] trackBrowseEnd:self.currentNews progress:self.progress];
}
```
其中，progress的获取示例如下：

```objc
- (CGFloat)getProgress {
    CGFloat progress = (_webView.scrollView.contentOffset.y + kStatusHeight + kNavigationBarHeight + _webView.frame.size.height) / (_webView.scrollView.contentSize.height + kStatusHeight + kNavigationBarHeight);
    
    _progress = _progress > progress ? _progress : progress;
    
    if (_progress > 1) {
        _progress = 1;
    }
    
    return _progress;
}
```

---


## 附录
### 数据模型

- 频道列表：NFChannels

名称 | 类型 | 描述
---|---|---
channels| NSArray(NFChannelInfo)| 频道列表

- 单个频道：NFChannelInfo

名称 | 类型 | 示例 | 描述
---|---|---|---
channelId| NSString| | 频道ID
channelName | NSString ||频道名称
order | NSInteger | 1 | 频道显示的顺序
channelType | NSInteger | |频道类型，0：按类别聚合   1：按话题聚合   2：按自定义来源聚合    3：地域频道   4：自营频道

- 新闻：NFNews

名称 | 类型 | 描述
---|---|---|---
infos| NSArray(NFNewsInfo)| 普通新闻列表
banners| NSArray(NFNewsInfo)| 轮播列表
tops| NSArray(NFNewsInfo)| 头条列表

其中，NFNewsInfo为

名称 | 类型 | 描述
---|---|---|---
producer | NSString | 新闻提供者
recId | NSString | 单次推荐唯一标示
algInfo| NSString| 推荐策略及权重信息
channelId | NSString | 频道ID
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型
title | NSString | 新闻标题
summary | NSString | 新闻简介
source | NSString | 新闻来源
updateTime | NSString | 新闻更新时间
hasVideo | BOOL | 文章是否包含视频
imgType | NSInteger | 缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
num | NSInteger | 图集中图片数量,只有在图集情况下才会返回该字段
thumbnails | NSArray (==NFNewsImage==) | 新闻缩略图
tripleImgs | NSArray (==NFNewsImage==) | 封面图三图
videos | NSArray (==NFNewsVideo==) | 视频源信息,只有在视频情况下才会返回该字段
ad | NFAdInfo | 广告信息

其中，缩略图模型：NFNewsImage

名称 | 类型 | 描述
---|---|---
url | NSString | 缩略图url
height | NSInteger | 缩略图高度
width | NSInteger | 缩略图宽度

视频模型：NFNewsVideo

名称 | 类型 | 描述
---|---|---
cover | NSString | 视频封面图
largeCover | NSString | 视频封面大图
playsize | NSInteger | 播放类型，0：表示4：3宽高比，1：表示16：9宽高比
duration | NSInteger | 视频时长，单位为秒
mp4SdUrl | NSString | mp4格式视频链接，标清
mp4HdUrl | NSString | mp4格式视频链接，高清
m3u8SdUrl | NSString | m3u8格式视频链接，标清
m3u8HdUrl | NSString | m3u8格式视频链接，高清
sdUrl | NSString | flv格式视频链接，标清
hdUrl | NSString | flv格式视频链接，高清
shdUrl | NSString | flv格式视频链接，shd

广告模型：NFAdInfo

名称 | 类型 | 描述
---|---|---
mediumId | NSString | 应用ID
producer | NSString | 广告来源
adPlacementId | NSString | 广告位
ip | NSString | 公网ip（inmobi类型广告需要）
data | GDTNativeAdData | 原生广告数据
exposureURLArray | NSArray | InMoBi广告曝光url
clickURLArray | NSArray | InMoBi广告点击url
isApp | BOOL | InMoBi广告是否是App
landingURL | NSString | InMoBi广告落地页
title | NSString | 广告标题
imgType | NSInteger | 缩略图模式，3：三图模式， 2：大图模式，1：缩略图模式，0：无图
imageUrl | NSString | 广告图片URL

- 新闻详情：NFNewsDetail

名称 | 类型 | 描述
---|---|---|---
infoId| NSString| 新闻ID
infoType| NSString| 新闻类型
category| NSString| 新闻类目
title | NSString | 新闻标题
publishTime | NSString | 发布时间
source | NSString | 新闻来源
sourceLink | NSString | 原文地址
content | NSString | 新闻正文
tag | NSString | 标签
imgs | NSArray (==NFNewsDetailImage==) | 新闻缩略图
videos | NSArray (==NFNewsVideo==) | 视频源信息,只有在视频情况下才会返回该字段
ad | NFAdInfo | 广告信息

其中，图片模型：NFNewsDetailImage

名称 | 类型 | 描述
---|---|---
url | NSString | 缩略图url
height | NSInteger | 缩略图高度
width | NSInteger | 缩略图宽度
pictype | NSString | 新闻图片类型
note | NSString | 图片描述，图集该字段才有值