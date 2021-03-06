# iOS UI SDK集成文档

## SDK概述

网易有料NewsFeedsUISDK为移动应用提供内容智能分发功能，在NewsFeedsUISDK数据层SDK的基础上，方便第三方应用快速的集成并实现信息流各个页面的开发。SDK兼容iOS 9.0+。

在使用NewsFeedsUISDK的前提要同时导入NewsFeedsSDK。

NewsFeedsUISDK提供的功能如下：

- 信息流首页视图
- 文章详情页视图
- 图集浏览页面
- 文章图片浏览页面

接入的模式有两种：
- 快速集成信息流

    > 信息流的基本功能包含信息流主页、文章类新闻展示页、图集类新闻展示页等，若用户选择快速集成方式，则使用UI SDK提供的所有默认页面。
- 自定义集成信息流

    > 用户可以根据UI SDK提供的回调，自定义交互逻辑。例如，点击新闻列表后的目标调转页面、点击相关推荐后的目标调转页面等等。

---

## SDK类说明

网易有料NewsFeedsUISDK主要提供了以下的类：

- ==NewsFeedsUISDK== 整个SDK的主入口，单例，主要提供创建信息流首页视图、创建新闻详情视图、创建图集浏览页面、创建文章详情图片浏览页面实例。==提供的四种UI，实例创建必须通过该接口生成。==
- ==NFeedsView==信息流首页视图类和==NFeedsViewDelegate==相关的回调接口
- ==NFArticleDetailView==文章详情的视图和==NFArticleDetailViewDelegate==相关的回调接口
- ==NFPicSetGalleryViewController==图集页面和==NFPicSetGalleryDelegate==相关的回调接口
- ==NFArticleGalleryViewController==新闻详情的图片浏览页面和==NFArticleGalleryDelegate==相关的回调接口

---

## 开发准备

### 1. SDK导入
网易有料NewsFeedsSDK、NewsFeedsUISDK 可通过手动下载，并添加到项目中集成使用。

1. 将下载得到的NewsFeedsSDK.framework和NFBundle.bundle手动导入到工程中。
2. 将下载得到的NewsFeedsUISDK.framework和NFUIBundle.bundle手动导入到工程中。

    将相关包拖进工程的时候，参考下图选项
    
    ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG81.jpeg)

    保证==Build Phases -> Link Binary With Libraries==中包含==NewsFeedsSDK.framework==和==NewsFeedsUISDK.framework==

    保证==Build Phases -> Copy Bundle Resources==中包含==NFBundle.bundle==和==NFUIBundle.bundle==

3. 添加系统依赖库
    
    -   libsqlite3.tbd
    -   libz.tbd
    -   libc++.tbd
    -   AdSupport.framework
    -   CoreLocation.framework
    -   AVFoundation.framework
    -   Security.framework
    -   MobileCoreServices.framework
    -   SystemConfiguration.framework
    -   CoreTelephony.framework
    -   UserNotifications.framework
    -   StoreKit.framework
    -   SafariServices.framework

4. 在 ==Build Settings== -> ==Other Linker Flags== 里，添加选项 ==-ObjC==。


5. 使用NewsFeedsUISDK，需依赖的第三方库：

    -   UITableView+FDTemplateLayoutCell
    -   Masonry
    -   SDWebImage/WebP
    -   SDWebImage/GIF

    建议使用CocoaPods导入上述5个库

    ```
      pod 'UITableView+FDTemplateLayoutCell', '~> 1.4'
      pod 'Masonry'
      pod 'SDWebImage/WebP'
      pod 'SDWebImage/GIF'
    ```

    若原工程中已包含，可以跳过。

    若在CocoaPods导入过程中，出现错误，用户可以将相关的第三方库源码导入工程即可。
    
6. 配置App Transport Security

    由于NewsFeedsSDK返回的video和image的url为http的方式，为了保证能够正常播放视频及加载图片，需在info.plist中配置==App Transport Security Settings==，并将==Allow Arbitrary Loads==设为==YES==，具体设置参考下图：
    
    ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG70.jpeg)

7. 配置访问相册权限
    
    由于图集和新闻正文详情图片浏览时，图片可以保存到本地相册，需开通访问相册权限

    info.plist中添加==Privacy - Photo Library Usage Description==，如上图所示

### 2. NewsFeedsSDK初始化

首先调用NewsFeedsSDK初始化，否则后续UI功能无法使用，具体使用可参考demo
```objc
[[NewsFeedsSDK sharedInstance] startWithAppKey:@"appkey" appSecret:@"appsecret"];
```
配置广告的弹出页，否则广告功能无法使用，需在列表加载之前调用此接口，具体可参考demo
```objc
[[NewsFeedsSDK sharedInstance] setAdPresentController:presentController];
```

部分初始化接口及参数说明

接口 | 参数 | 类型 | 描述
---|---|---|---
setCacheEnabled | enabled | BOOL | 是否开启新闻正文文本和图片缓存，默认开启
setMaxCachedTime | maxCachedTime | NSInteger | 配置新闻正文文本和图片最大缓存时长, 单位毫秒，默认7天
setLogLevel | maxLevel | NFSDKMaxLogLevel | 配置日志的打印级别

其余配置请参考NewsFeedsSDK的使用接入文档

---

## SDK接口说明
### NewsFeedsUISDK类（主入口）
NewsFeedsUISDK类为ui-sdk主入口，提供多页面的实例化调用。


#### 1、获取当前SDK的版本号

```objc
/**
 *  @method
 *
 *  @abstract
 *  获取当前SDK的版本号
 *
 *  @discussion
 *  UI SDK需配合NewsFeedsSDK使用，在使用时应保证版本兼容
 */
+ (NSString *)version;
```

- 示例
```objc
NSString *version = [NewsFeedsUISDK version];
```

返回字段格式：==v1.2.4==

---

#### 2、创建信息流主页NFeedsView实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFeedsView实例
 *
 *  @param navController    页面跳转需要的UINavigationController实例
 *  @param delegate         NFeedsViewDelegate回调
 *  @param extraData        用户可以传入自定义字段，该字段会在回调中返回给用户
 *
 *  @discussion
 *  若用户未实现回调，则SDK会自动跳转新闻详情页面，需传入navController变量
 *  若用户实现回调，则跳转到用户回调，无需传navController参数
 */
+ (NFeedsView *)createFeedsView:(UINavigationController *)navController
                       delegate:(id<NFeedsViewDelegate>)delegate
                      extraData:(id)extraData;
```

==注意==：提供两种模式，具体使用参考demo

- 1种是快速集成，则未实现delegate回调，则所有页面已经整合到一起，及文章详情页面、图集页面、新闻正文图片浏览页面都已封装，此时需要用户传入navController

    ```objc
    //接入信息流UI SDK，快速集成信息流主页 NFeedsView
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"whole"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
    ```
    ==注意==：不要实现delegate回调

- 另外一种是自定义集成，需实现delegate回调，那么用户在点击新闻列表的时候跳转到用户回调，由用户自定义页面跳转，无需传navController参数

    ```objc
    // 第一步：接入信息流UI SDK，自定义集成信息流主页 NNFeedsFragment
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"seperate"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
    ```
    
    ==注意==：实现delegate回调

    ```objc
    //第二步：为信息流主页 NFeedsView 设置点击事件回调
    - (void)newsListDidSelectNews:(NFeedsView *)feedsView newsInfo:(NFNewsInfo *)newsInfo extraData:(id)extraData {
        if ([newsInfo.infoType isEqualToString:@"article"]) {
        //第三步：自定义文章类新闻展示页面
            ArticleDetailViewController *detailVC = [[ArticleDetailViewController alloc] initWithNews:newsInfo];
            detailVC.delegate = self;
            [self.navigationController pushViewController:detailVC animated:YES];
        } else if([newsInfo.infoType isEqualToString:@"picset"]) {
        //第四步：自定义图集类新闻展示页面
            NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
            [self.navigationController pushViewController:browserVC animated:YES];
        }// 目前只提供文章类和图集类新闻的点击调转，后续会扩展更多类型
    }
    ```
---

#### 信息流主页回调接口说明(NFeedsViewDelegate)

NFeedsViewDelegate为NFeedsView类对应的回调，提供信息流主页交互事件回调，目前支持的交互事件回调有：新闻被点击。

- 新闻被点击

```objc
/**
 *  @method
 *
 *  @abstract
 *  新闻列表的点击回调
 *
 *  @param feedsView    当前的NFeedsView实例
 *  @param newsInfo     点击的新闻
 *  @param extraData    用户的自定义字段
 *
 *  @discussion
 *  若用户未实现该回调，则SDK会自动跳转新闻详情页面，需传入navController变量
 *  若用户实现该回调，则跳转到用户回调
 */
- (void)newsListDidSelectNews:(NFeedsView *)feedsView
                     newsInfo:(NFNewsInfo *)newsInfo
                    extraData:(id)extraData;
```
注意这里的extraData是初始化NFeedsViewt实例时，传入的用户自定义数据。

---

#### 3、创建文章类新闻展示页NFArticleDetailView实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFArticleDetailView实例
 *
 *  @param newsInfo    待加载新闻详情的newsInfo
 *  @param delegate    NFArticleDetailViewDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFArticleDetailView *)createArticleDetailView:(NFNewsInfo *)newsInfo
                                        delegate:(id<NFArticleDetailViewDelegate>)delegate
                                       extraData:(id)extraData;
```
用户选择自己创建新闻详情页面时，可以通过该接口创建新闻详情视图实例，其中newsInfo为待加载新闻详情的newsInfo，delegate为 NFArticleDetailViewDelegate回调，extraData为用户可以自定义传入的字段。

==注意==：用户选择自定义集成的文章详情页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo

```objc
// 第一步：接入信息流UI SDK，自定义集成文章类展示页NFArticleDetailView
NFArticleDetailView *articledetailView = [NewsFeedsUISDK createArticleDetailView:_newsInfo delegate:self extraData:@"articledetailView"];
[self.view addSubview:articledetailView];
```
==注意==：实现delegate回调

```objc
//第二步：自定义相关推荐新闻展示页面
- (void)relatedNewsDidSelect:(NFArticleDetailView *)detailView
                    newsInfo:(NFNewsInfo *)newsInfo
                   extraData:(id)extraData {
    ArticleDetailViewController *detailViewController = [[ArticleDetailViewController alloc] initWithNews:newsInfo];
    [self.navigationController pushViewController:detailViewController animated:YES];
}

//第三步：点击文章正文内的图片的点击事件
- (void)webImageDidSelect:(NFArticleDetailView *)detailView
               imageArray:(NSArray<NFNewsDetailImage *> *)imageArray
                  atIndex:(NSUInteger)index
                    frame:(CGRect)frame
                extraData:(id)extraData {
    NFArticleGalleryViewController *browserVC = [NewsFeedsUISDK createArticleGalleryViewController:imageArray imageIndex:index delegate:self extraData:@"ArticleGalleryViewController"];
    [self presentViewController:browserVC animated:YES completion:nil];
}

//第四步：通知新闻已阅，信息流主页UI刷新
- (void)articleDidLoadContent:(NFArticleDetailView *)detailView extraData:(id)extraData {
    __weak typeof(self)weakSelf = self;

    [[NewsFeedsSDK sharedInstance] hasRead:self.newsInfo.infoId block:^(BOOL hasRead) {
        if (!hasRead) {
            [[NewsFeedsSDK sharedInstance] markRead:weakSelf.newsInfo.infoId];
            if ([weakSelf.delegate respondsToSelector:@selector(markRead)]) {
                [weakSelf.delegate markRead];
            }
        }
    }];
}

//第五步：点击文章底部的举报按钮时，弹出举报选择框，此时，WebView标题发生变化，将当前Title展示为WebView标题
- (void)onIssueReport:(NFArticleDetailView *)detailView issueDesc:(NSString *)issueDescription extraData:(id)extraData {
    __weak typeof(self)weakSelf = self;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        weakSelf.title = issueDescription;
    });
}

//第六步：举报完成后，展示原来的标题
- (void)onIssueReportFinished:(NFArticleDetailView *)detailView extraData:(id)extraData {
    __weak typeof(self)weakSelf = self;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        weakSelf.title = weakSelf.newsInfo.source;
    });
}
```
---

#### 文章类新闻展示页回调接口说明(NFArticleDetailViewDelegate)

- 相关推荐新闻的点击回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  相关推荐新闻的点击回调
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param newsInfo      点击的新闻
 *  @param extraData     用户的自定义字段
 */
- (void)relatedNewsDidSelect:(NFArticleDetailView *)detailView
                    newsInfo:(NFNewsInfo *)newsInfo
                   extraData:(id)extraData;
```
新闻详情页面相关推荐新闻点击时的回调

---

- 新闻正文中图片点击
```objc
/**
 *  @method
 *
 *  @abstract
 *  新闻正文中图片点击
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param imageArray    图片集合
 *  @param index         当前点击图片下标
 *  @param frame         点击图片的frame
 *  @param extraData     用户的自定义字段
 */
- (void)webImageDidSelect:(NFArticleDetailView *)detailView
               imageArray:(NSArray <NFNewsDetailImage *>*)imageArray
                  atIndex:(NSUInteger)index
                    frame:(CGRect)frame
                extraData:(id)extraData;
```
新闻正文中图片点击的回调，其中，imageArray为图片集合,元素model类型为NFNewsDetailImage，index为当前点击图片下标，frame为点击图片的frame，方便做转场动画用，extraData为用户的自定义字段

---

-  文章新闻详情加载成功
```objc
/**
 *  @method
 *
 *  @abstract
 *  文章新闻详情加载成功
 *
 *  @param detailView    当前的NFArticleDetailView实例
 *  @param extraData     用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时，
 *  调用NewsFeedsSDK的markRead接口将新闻标记为已读，同时刷新列表已读状态
 */
- (void)articleDidLoadContent:(NFArticleDetailView *)detailView
                    extraData:(id)extraData;
```
该回调主要方便用户在新闻加载成功时，调用NewsFeedsSDK的markRead接口将新闻标记为已读，同时刷新列表已读状态

---

- 详情页面跳转到报错页面的回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错页面的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param issueDescription issue相关描述
 *  @param extraData        用户的自定义字段
 */
- (void)onIssueReport:(NFArticleDetailView *)detailView
            issueDesc:(NSString *)issueDescription
            extraData:(id)extraData;
```
在详情页点击报错跳转详情页面的回调，其中issueDescription为报错页面的相关描述，方便更改页面title之类的参数

---

#### 详情页面跳转到报错完成返回到主页的回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  详情页面跳转到报错完成返回到主页的回调
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param extraData        用户的自定义字段
 */
- (void)onIssueReportFinished:(NFArticleDetailView *)detailView
                    extraData:(id)extraData;
```
报错操作结束后触发该回调

---


#### 4、创建图集类新闻展示页NFPicSetGalleryViewController实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFPicSetGalleryViewController实例
 *
 *  @param newsInfo    待加载图集的newsInfo
 *  @param delegate    NFPicSetGalleryDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFPicSetGalleryViewController *)createPicSetGalleryViewController:(NFNewsInfo *)newsInfo
                                                            delegate:(id<NFPicSetGalleryDelegate>)delegate
                                                           extraData:(id)extraData;
```
可以通过该接口返回图集页面的实例，其中newsInfo为待加载图集新闻的newsInfo，delegate为NFPicSetGalleryDelegate回调，extraData为用户可以自定义传入的字段，会在相应的回调中回传

==注意==：用户选择自定义集成的图集类新闻展示页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo

```objc
// 第一步：接入信息流UI SDK，自定义集成图集类展示页 NFPicSetGalleryViewController，展示图集类新闻
NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
[self.navigationController pushViewController:browserVC animated:YES];
```

==注意==：实现delegate回调

```objc
//第二步：通知新闻已阅，信息流主页UI刷新
- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
     [_feedsView markReadNews];
}

//第三步：设置图集展示页左上角返回按钮点击后的行为
- (void)back:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
    [self.navigationController popViewControllerAnimated:YES];
}
```

---

#### 图集类新闻展示页回调接口说明(NFPicSetGalleryDelegate)

NFPicSetGalleryDelegate提供图集类新闻展示页交互事件回调，目前支持的交互事件回调有：图集加载成功时的回调、左上角返回按钮的点击响应。

- 图集加载成功时的回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  图集加载成功时的回调
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时刷新列表已读状态
 */
- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController
                   extraData:(id)extraData;
```
该回调主要方便用户在新闻加载成功时刷新列表已读状态

---

- 页面的返回回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  点击返回按钮的时候
 */
- (void)back:(NFPicSetGalleryViewController *)browserController
   extraData:(id)extraData;
```
点击返回按钮的时候

---

#### 5、创建文章类新闻正文图片集展示页NFArticleGalleryViewController实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFArticleGalleryViewController实例
 *
 *  @param imageArray           图片数组
 *  @param currentImageIndex    当前图片索引
 *  @param delegate             NFArticleGalleryDelegate回调
 *  @param extraData            用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFArticleGalleryViewController *)createArticleGalleryViewController:(NSArray <NFNewsDetailImage *>*)imageArray
                                                            imageIndex:(NSInteger)currentImageIndex
                                                              delegate:(id<NFArticleGalleryDelegate>)delegate
                                                             extraData:(id)extraData;
```
可以通过该接口返回新闻详情图片浏览页面的实例，其中imageArray为新闻详情的图片数组,图片model类型为NFNewsDetailImage，可以通过点击图片的回调获取，imageIndex为当前的图片的索引，可以通过点击图片的回调获取，delegate为NFArticleGalleryDelegate回调，extraData为用户可以自定义传入的字段，该字段会在回调中返回给用户

==注意==：用户选择自定义集成的文章新闻正文图片集展示页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo:

```objc
// 第一步：接入信息流UI SDK，创建文章新闻正文图片集展示页NFArticleGalleryViewController实例
NFArticleGalleryViewController *browserVC = [NewsFeedsUISDK createArticleGalleryViewController:imageArray imageIndex:index delegate:self extraData:@"ArticleGalleryViewController"];
[self presentViewController:browserVC animated:YES completion:nil];
```

==注意==：实现delegate回调

```objc
//第二步：单击图片的回调，返回
- (void)back:(NFArticleGalleryViewController *)browserController extraData:(id)extraData {
    [browserController dismissViewControllerAnimated:YES completion:nil];
}

```
---

#### 文章类新闻正文图片集展示页回调接口说明(NFArticleGalleryDelegate)

- 页面的返回回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFArticleGalleryViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  单击图片的时候
 */
- (void)back:(NFArticleGalleryViewController *)browserController
   extraData:(id)extraData;
```
单击图片的时候，触发该回调

---


### NFeedsView类接口说明

#### 刷新已读新闻的状态

```objc
/**
 *  @method
 *
 *  @abstract
 *  刷新已读新闻的状态
 *
 *  @discussion
 *  使用SDK整合的View，无需调用该接口；
 *  用户自定义跳转页面，需在新闻详情加载成功后，将新闻标记为已读，并主动调用该接口
 */
- (void)markReadNews
```
 使用SDK整合的View，无需调用该接口；
 用户自定义跳转页面，需在新闻详情加载成功后，将新闻标记为已读，并主动调用该接口，刷新新闻列表的已读状态
---

#### 强制刷新当前列表
```objc
/**
 *  @method
 *
 *  @abstract
 *  强制刷新当前列表
 *
 *  @discussion
 *  可调用该接口实现强制刷新当前列表的功能
 */
- (void)forceRefresh
```
可调用该接口实现强制下拉刷新当前列表的功能

---
