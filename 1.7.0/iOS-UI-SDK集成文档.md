## SDK概述

网易有料NewsFeedsUISDK为移动应用提供内容智能分发功能，在NewsFeedsUISDK数据层SDK的基础上，方便第三方应用快速的集成并实现信息流各个页面的开发。SDK兼容iOS 9.0+。

在使用NewsFeedsUISDK的前提要同时导入NewsFeedsSDK。

NewsFeedsUISDK提供的功能如下：

- 信息流首页视图
- 文章详情页视图
- 图集浏览页面
- 视频浏览页面
- 文章图片浏览页面
- 信息流入口

接入的模式有两种：

- 快速集成信息流
    > 信息流的基本功能包含信息流主页、文章类新闻展示页、图集类新闻展示页等，若用户选择快速集成方式，则使用UI SDK提供的所有默认页面。
    
- 自定义集成信息流
   > 用户可以根据UI SDK提供的回调，自定义交互逻辑。例如，点击新闻列表后的目标调转页面、点击相关推荐后的目标调转页面等等。

---

## SDK类说明

网易有料NewsFeedsUISDK主要提供了以下的类：

类|描述
---- | -----
NewsFeedsUISDK | 整个SDK的主入口，单例，主要提供创建信息流首页视图、创建新闻详情视图、创建图集浏览页面、创建文章详情图片浏览页面实例、创建信息流入口实例。提供的五种UI，实例创建必须通过该接口生成。
NFeedsView | 信息流首页视图类和`NFeedsViewDelegate`相关的回调接口
NFArticleDetailView | 文章详情的视图和`NFArticleDetailViewDelegate`相关的回调接口
NFPicSetGalleryViewController | 图集页面和`NFPicSetGalleryDelegate`相关的回调接口
NFVideoBrowserViewController | 视频浏览页面和`NFVideoBrowserDelegate`相关的回调接口
NFArticleGalleryViewController  | 新闻详情的图片浏览页面和`NFArticleGalleryDelegate`相关的回调接口
UIViewController<NFChannelControllerProtocol> | 信息流小入口和`NFChannelControllerDelegate`相关的回调接口

---

## 开发准备

### 1. SDK导入
 
#### cocoapod集成(preferred)

  有料SDK支持cocoapod集成，只需要添加相应的pod就行

  1. 首先编辑Podfile, 如果没有先在工程目录下运行`pod init`

  ```ruby
  #Podfile
  platform :ios, '9.0'
  #添加有料源
  source 'https://github.com/CocoaPods/Specs.git'
  source 'https://github.com/NetEaseYouliao/Specs.git'

  ...
    
  target :your_project_target do
    pod 'NewsFeedsSDK'
    pod 'NewsFeedsUISDK'
    ...  #any other pod your project needed
  end
  ```

  2. 运行`pod install`

  ​

#### 手动导入

网易有料NewsFeedsSDK、NewsFeedsUISDK 可通过手动下载，并添加到项目中集成使用。

1. 将下载得到的NewsFeedsSDK.framework手动导入到工程中。
2. 将下载得到的NewsFeedsUISDK.framework和NFUIBundle.bundle手动导入到工程中。
3. 下载广点通SDK，导入到工程中，下载链接为:[广点通SDK](https://github.com/NetEaseYouliao/YLGDTMobSDK)
4. 若使用推送功能，下载个推SDK，导入到工程中，下载链接为:[个推SDK](http://docs.getui.com/download.html)

    将相关包拖进工程的时候，参考下图选项

    ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG81.jpeg)

    保证Build Phases -> Link Binary With Libraries中包含NewsFeedsSDK.framework和NewsFeedsUISDK.framework

    保证Build Phases -> Copy Bundle Resources中包含NFBundle.bundle和NFUIBundle.bundle

5. 添加系统依赖库

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

6. 在 Build Settings -> Other Linker Flags 里，添加选项 -ObjC。


7. 使用NewsFeedsUISDK，需依赖的第三方库：

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

    注意 ：**保证 SDWebImagede的版本大于 4.0**

    若在CocoaPods导入过程中，出现错误，用户可以将相关的第三方库源码导入工程即可。


  当SDK导入工程后，需要修改一些配置

1. 配置App Transport Security

   由于NewsFeedsSDK返回的video和image的url为http的方式，为了保证能够正常播放视频及加载图片，需在info.plist中配置App Transport Security Settings，并将Allow Arbitrary Loads设为YES，具体设置参考下图：

   ![image](http://ofwsr8cl0.bkt.clouddn.com/WechatIMG70.jpeg)

2. 配置访问相册权限

   由于图集和新闻正文详情图片浏览时，图片可以保存到本地相册，需开通访问相册权限

   info.plist中添加Privacy - Photo Library Usage Description和Privacy - Photo Library Additions Usage Description

### 2. NewsFeedsUISDK初始化

首先调用NewsFeedsUISDK初始化，否则后续UI功能无法使用，具体使用可参考demo

```objective-c
[[NewsFeedsUISDK sharedInstance] startWithAppKey:@"appkey" appSecret:@"appsecret" config: nil];
```
config配置请参考[UI定制化](https://github.com/NetEaseYouliao/NewsFeedsUISDK/wiki/UI定制化)

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

返回字段格式：v1.2.4

---

#### 2、设置分享点击回调
```objc
/**
 *  @method
 *
 *  @abstract
 *  设置NewsFeedsUISDKDelegate的分享点击回调
 */
 + (void)setDelegate:(id<NewsFeedsUISDKDelegate>)delegate;
```

其中，NewsFeedsUISDKDelegate定义为：

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

- 快速集成模式：实现该回调，则新闻详情页面、图集详情页面、视频页面都会显示分享按钮；未实现该回调，则不会显示分享按钮
- 自定义集成模式：该分享回调和各个页面的单独的分享回调只要实现一个则会显示分享按钮，若两个回调都实现，则点击分享的时候，各个页面的单独的分享回调调用优先

具体的分享链接使用有料封装的h5页面，使用参考demo

```objc
- (void)onShareClick:(NSDictionary *)shareInfo
                   type:(NSInteger)type {
    NSMutableString *url = [shareInfo[@"shareUrl"] mutableCopy];
                     
    //拼接ios open url,scheme替换成用户app的。
    [url appendFormat:@"&iou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];
                     
    //拼接android open url,scheme替换成用户app的。
    [url appendFormat:@"&aou=%@", [self encodeParameter:[NSString stringWithFormat:@"youliao://youliao.163yun.com?infoId=%@&infoType=%@&producer=%@", shareInfo[@"infoId"], shareInfo[@"infoType"], shareInfo[@"producer"]]]];
                     
    [WXApiRequestHandler sendLinkURL:url
                             TagName:shareInfo[@"infoType"]
                               Title:shareInfo[@"title"]
                         Description:shareInfo[@"summary"] ?  : shareInfo[@"source"]
                          ThumbImage:shareInfo[@"thumbnail"]
                             InScene:type];

}
```

---

#### 3、创建信息流主页NFeedsView实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFeedsView实例
 *
 *  @param navController    页面跳转需要的UINavigationController实例
 *  @param delegate         NFeedsViewDelegate回调
 *  @param configuration    提供配置的选项
 *  @param extraData        用户可以传入自定义字段，该字段会在回调中返回给用户
 *
 *  @discussion
 *  若用户未实现回调，则SDK会自动跳转新闻详情页面，需传入navController变量
 *  若用户实现回调，则跳转到用户回调，无需传navController参数
 */
+ (NFeedsView *)createFeedsView:(UINavigationController *)navController
                       delegate:(id<NFeedsViewDelegate>)delegate
                         config:(NSDictionary * __nullable)configuration
                      extraData:(id)extraData;
```

注意：提供两种模式，具体使用参考demo

- 一种是快速集成，则未实现delegate回调，则所有页面已经整合到一起，及文章详情页面、图集页面、新闻正文图片浏览页面都已封装，此时需要用户传入navController

    ```objc
    //接入信息流UI SDK，快速集成信息流主页 NFeedsView
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self config:nil extraData:@"seperate"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
    ```
    注意：不要实现delegate回调

- 另外一种是自定义集成，需实现delegate回调，那么用户在点击新闻列表的时候跳转到用户回调，由用户自定义页面跳转，无需传navController参数

    ```objc
    // 第一步：接入信息流UI SDK，自定义集成信息流主页 NNFeedsFragment
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"seperate"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
    ```

    注意：实现delegate回调

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
        } else if ([newsInfo.infoType isEqualToString:@"video"]) {
        //第五步：自定义视频类新闻展示页面
            NFVideoBrowserViewController *videoBrowserVC = [NewsFeedsUISDK createVideoBrowserViewController:newsInfo delegate:self extraData:@"VideoBrowserViewController"];
            [self.navigationController pushViewController:videoBrowserVC animated:YES];
        }
    }
    ```
---

#### 信息流主页回调接口说明(NFeedsViewDelegate)

NFeedsViewDelegate为NFeedsView类对应的回调，提供信息流主页交互事件回调，目前支持的交互事件回调有：新闻被点击，自定义model。

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



- 自定义model

```objc
/**
 *  @method
 *
 *  @abstract
 *  自定义插入新闻
 *
 *  @param channelInfo     频道
 *  @param loadType        0代表上拉加载更多，1代表下拉刷新
 *  @param block           用户获取到新闻信息block返回
 *
 *  @discussion
 *  若用户无需实现自定义新闻列表插入数据，则无需实现该接口，
 *  自定义的infoType不可以为article/picset/video字段，用户自定义设置，并在点击回调中实现相应的点击跳转逻辑
 */
@optional
- (void)customNewsWithChannel:(NFChannelInfo *)channelInfo
                     loadType:(NSInteger)type
               comletionBlock:(void(^)(NSArray  <NFNewsInfo *>*newsList, NSIndexSet *indexSet))block;
```

---

#### 4、创建文章类新闻展示页NFArticleDetailView实例
```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFArticleDetailView实例
 *
 *  @param newsInfo    待加载新闻详情的newsInfo
 *  @param delegate    NFArticleDetailViewDelegate回调
 *  @param configuration  提供配置的选项
 *  @param extraData        用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFArticleDetailView *)createArticleDetailView:(NFNewsInfo *)newsInfo
                                        delegate:(id<NFArticleDetailViewDelegate>)delegate
                                          config:(NSDictionary * __nullable)configuration
                                       extraData:(id)extraData;
```
用户选择自己创建新闻详情页面时，可以通过该接口创建新闻详情视图实例，其中newsInfo为待加载新闻详情的newsInfo，delegate为 NFArticleDetailViewDelegate回调，extraData为用户可以自定义传入的字段。

注意：用户选择自定义集成的文章详情页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo

```objc
// 第一步：接入信息流UI SDK，自定义集成文章类展示页NFArticleDetailView
NFArticleDetailView *articledetailView = [NewsFeedsUISDK createArticleDetailView:_newsInfo delegate:self config:nil extraData:@"articledetailView"];
[self.view addSubview:articledetailView];
```
注意：实现delegate回调

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

//第七步：分享按钮点击
- (void)onWebShareClick:(NFArticleDetailView *)detailView
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData {
    //分享操作，具体实现参考NewsFeedsUISDKDelegate的分享实现
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

示例代码：

```objc
- (void)articleDidLoadContent:(NFArticleDetailView *)detailView extraData:(id)extraData {
    __weak typeof(self)weakSelf = self;

	// 标记为已读
    [[NewsFeedsSDK sharedInstance] hasRead:self.newsInfo.infoId block:^(BOOL hasRead) {
        if (!hasRead) {
            [[NewsFeedsSDK sharedInstance] markRead:weakSelf.newsInfo.infoId];
            if ([weakSelf.delegate respondsToSelector:@selector(articleDetailNewsMarkRead)]) {
                [weakSelf.delegate articleDetailNewsMarkRead];
            }
        }
    }];
}
```

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

- 详情页面跳转到报错完成返回到主页的回调

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

- 详情页面点击分享的回调

```objc
/**
 *  @method
 *
 *  @abstract
 *  详情页面的分享按钮点击
 *
 *  @param detailView       当前的NFArticleDetailView实例
 *  @param shareInfo        分享需要用到的字段集合
 *  @param type             分享类型，0：微信好友  1：朋友圈
 *  @param extraData        用户的自定义字段
 */
- (void)onWebShareClick:(NFArticleDetailView *)detailView
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData;
```
实现该回调，则在文章分享按钮点击时调用，未实现该回调，则会调用NewsFeedsUISDKDelegate中的分享回调

---


#### 5、创建图集类新闻展示页NFPicSetGalleryViewController实例

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

注意：用户选择自定义集成的图集类新闻展示页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo

```objc
// 第一步：接入信息流UI SDK，自定义集成图集类展示页 NFPicSetGalleryViewController，展示图集类新闻
NFPicSetGalleryViewController *browserVC = [NewsFeedsUISDK createPicSetGalleryViewController:newsInfo delegate:self extraData:@"PicSetGalleryViewController"];
[self.navigationController pushViewController:browserVC animated:YES];
```

注意：实现delegate回调

```objc
//第二步：通知新闻已阅，信息流主页UI刷新
- (void)picSetDidLoadContent:(NFPicSetGalleryViewController *)browserController extraData:(id)extraData {
     [_feedsView markReadNews];
}

//第三步：用户点击分享按钮的回调
- (void)onPicSetShareClick:(NFPicSetGalleryViewController *)browserController
        newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData {
    //具体实现逻辑参考NewsFeedsUISDKDelegate的示例                  
}

//第四步：设置图集展示页左上角返回按钮点击后的行为
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

- 图集页面点击分享的回调

```objc
/**
 *  @method
 *
 *  @abstract
 *  图集分享按钮点击
 *
 *  @param browserController    当前的NFPicSetGalleryViewController实例
 *  @param shareInfo            分享需要用到的字段集合
 *  @param type                 分享类型，0：微信好友  1：朋友圈
 *  @param extraData            用户的自定义字段
 */
- (void)onPicSetShareClick:(NFPicSetGalleryViewController *)browserController
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData;

```
实现该回调，则在图集分享按钮点击时调用，未实现该回调，则会调用NewsFeedsUISDKDelegate中的分享回调

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

#### 6、创建视频类新闻展示页NFVideoBrowserViewController实例

```objc
/**
 *  @method
 *
 *  @abstract
 *  创建NFVideoBrowserViewController实例
 *
 *  @param newsInfo    待加载图集的newsInfo
 *  @param delegate    NFVideoBrowserDelegate回调
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 */
+ (NFVideoBrowserViewController *)createVideoBrowserViewController:(NFNewsInfo *)newsInfo
                                                          delegate:(id<NFVideoBrowserDelegate>)delegate
                                                         extraData:(id)extraData;
```
可以通过该接口返回视频页面的实例，其中newsInfo为待加载图集新闻的newsInfo，delegate为NFVideoBrowserDelegate回调，extraData为用户可以自定义传入的字段，会在相应的回调中回传

注意：用户选择自定义集成的视频类新闻展示页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo

```objc
// 第一步：接入信息流UI SDK，自定义集视频类展示页 NFVideoBrowserViewController *videoBrowserVC = [NewsFeedsUISDK createVideoBrowserViewController:newsInfo delegate:self extraData:@"VideoBrowserViewController"];
[self.navigationController pushViewController:videoBrowserVC animated:YES];
```

注意：实现delegate回调

```objc
//第二步：通知新闻已阅，信息流主页UI刷新
- (void)videoDidLoadContent:(NFVideoBrowserViewController *)browserController extraData:(id)extraData {
    [_feedsView markReadNews];
}

//第三步：用户点击分享按钮的回调
- (void)onVideoShareClick:(NFVideoBrowserViewController *)browserController
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData {
    //具体实现逻辑参考NewsFeedsUISDKDelegate的示例                  
}

//第四步：设置视频展示页左上角返回按钮点击后的行为
- (void)videoBack:(NFVideoBrowserViewController *)browserController extraData:(id)extraData {
    [browserController.navigationController popViewControllerAnimated:YES];
}
```

---

#### 视频类新闻展示页回调接口说明(NFVideoBrowserDelegate)

NFVideoBrowserDelegate提供视频类新闻展示页交互事件回调，目前支持的交互事件回调有：视频加载成功时的回调、左上角返回按钮的点击响应。

- 视频加载成功时的回调

```objc
/**
 *  @method
 *
 *  @abstract
 *  视频及相关推荐加载成功时的回调
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *  @param extraData            用户的自定义字段
 *
 *  @discussion
 *  该回调主要方便用户在新闻加载成功时刷新列表已读状态
 */
- (void)videoDidLoadContent:(NFVideoBrowserViewController *)browserController
                  extraData:(id)extraData;
```
该回调主要方便用户在新闻加载成功时刷新列表已读状态

- 视频页面点击分享的回调

```objc
/**
 *  @method
 *
 *  @abstract
 *  视频分享按钮点击
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *  @param shareInfo            分享需要用到的字段集合
 *  @param type                 分享类型，0：微信好友  1：朋友圈
 *  @param extraData            用户的自定义字段
 */
- (void)onVideoShareClick:(NFVideoBrowserViewController *)browserController
             newsDetail:(NSDictionary *)shareInfo
                   type:(NSInteger)type
              extraData:(id)extraData;

```
实现该回调，则在视频分享按钮点击时调用，未实现该回调，则会调用NewsFeedsUISDKDelegate中的分享回调

---

- 页面的返回回调

```objc
/**
 *  @method
 *
 *  @abstract
 *  页面的返回回调
 *
 *  @param browserController    当前的NFVideoBrowserViewController实例
 *
 *  @discussion
 *  回调的时机：点击返回按钮的时候
 */
- (void)videoBack:(NFVideoBrowserViewController *)browserController
        extraData:(id)extraData;
```
点击返回按钮的时候

---


#### 7、创建文章类新闻正文图片集展示页NFArticleGalleryViewController实例

```objective-c
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

注意：用户选择自定义集成的文章新闻正文图片集展示页的时候需要调用，若用户选择快速集成，不需要使用该类。

具体使用参考demo:

```objc
// 第一步：接入信息流UI SDK，创建文章新闻正文图片集展示页NFArticleGalleryViewController实例
NFArticleGalleryViewController *browserVC = [NewsFeedsUISDK createArticleGalleryViewController:imageArray imageIndex:index delegate:self extraData:@"ArticleGalleryViewController"];
[self presentViewController:browserVC animated:YES completion:nil];
```

注意：实现delegate回调

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

------

#### 8、创建信息流入口实例

信息流入口滚动显示某些频道的新闻摘要，并提供了跳转到feedsView的功能。信息流入口允许用户嵌入到app页面的局部中，增加新闻的曝光。

信息流入口分成以下几类：

* 标题滚动类入口

  ![](http://ojwwhf19y.bkt.clouddn.com/test04.jpeg)

* 后续添加更多入口...



```objective-c
/**
 *  @method
 *
 *  @abstract
 *  创建UIViewController<NFChannelControllerProtocol>实例
 *
 *  @param channelTag   创建的频道Tag
 *  @param type        channel入口的展现形式，用NFChannelControllerType枚举类型表示
 *  @param delegate    NFChannelControllerDelegate回调
 *  @param config      用来配置功能逻辑和UI样式
 *  @param extraData   用户可以传入自定义字段，该字段会在回调中返回给用户
 *
 *  @warning 如果传入不支持的NFChannelControllerType，该方法会返回nil
 */
+ (UIViewController<NFChannelControllerProtocol> *)
                    createChannelContentViewController:(NSString *)channelTag
                                                  type:(NFChannelControllerType)type
                                              delegate:(id<NFChannelControllerDelegate>)delegate
                                                config:(NSDictionary *)config
                                             extraData:(id)extraData;
```



具体使用参考demo:

```objective-c
UIViewController<NFChannelControllerProtocol> * channelVC = 
  [NewsFeedsUISDK createChannelContentViewController:@"a_channel_tag" type:NFChannelControllerTypeTitleBoard delegate:self config:nil extraData:userInfo]; //如果是一键集成，delegate只需要传nil。

//self指当前view controller, view指用户想要嵌入的view，可以是cell的contentView或其他类似小块区域
[channelVC addToController:self inView:view];


//如果用户想移除这个入口
[channelVC removeFromController];
```

当点击入口时会产生回调，如果用户使用一键集成，默认将打开`NFeedsView`，并会跳转到入口所示的频道，当前显示的新闻条目会置顶显示。

如果用户采用分开集成，需要实现`NFChannelControllerDelegate`,根据需要打开文章详情或者是`NFeedsView`

入口不会自动刷新，需要用户控制刷新时机，当需要刷新时只要调用`[channelVC refreshList]`。

入口也提供了切换当前频道的功能，`[channelVC changeChannelTag:@"a_new_channel_tag"]`。

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

#### 定位到固定的频道

```objc
/**
 * 允许用户直接定位到某一个频道
 *
 * @param channelTag    频道Tag
 * @param animated     是否显示动画
 */
- (void)setSelectedWithChannel:(NSString *)channelTag
                      animated:(BOOL)animated;
```
可调用该接口实现定位到某个频道的功能

---