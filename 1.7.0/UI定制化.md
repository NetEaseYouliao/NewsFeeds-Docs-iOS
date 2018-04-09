### 简介

为了信息流的界面风格能与集成有料UI SDK 的 APP 能够整体统一，有料SDK 提供了简洁的 UI 和功能自定义配置选项。对于配置分为全局配置和局部配置，局部配置的优先级高于全局配置，针对每个页面做了功能配置和UI配置的区分。

- 新闻详情页

  可通过`NFArticleFuncOptionKey`作为键值，配置功能参数。可通过`NFArticleUIOptionKey`作为键值，配置UI样式参数。

- 信息流首页

  可通过`NFFeedsFuncOptionKey`作为键值，配置功能参数。可通过`NFFeedsUIOptionKey`作为键值，配置UI样式参数。

- 信息流入口

  其中针对不同类型的入口具备不同的配置项。

  * `NFChannelControllerTypeTitleBoard`类型

  自定义时可通过`NFTitleBoardChannelFuncOptionKey`作为键值，配置功能参数。可通过`NFTitleBoardChannelUIOptionKey`作为键值，配置UI样式参数。 ​

### 使用方式

---------

#### 全局配置

##### 手动方式

```objective-c
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>

[NewsFeedsUISDK startWithAppKey:appKey 
                      appSecret:secretKey 
                         config:@{
                                  @"maxCacheNum": 30,
                                  @"autoRefreshInterval":@(60*60*3)
                                 }];

NSMutableDictionary *feedsUIConfig = [@{} mutableCopy];
feedsUIConfig[@"cellTitleColor"] = @"0x333333";
feedsUIConfig[@"sourceTextColor"] = @"0x999999";
feedsUIConfig[@"separatorColor"] = @"0xd1d1d1 0.8";

NSMutableDictionary *feedsFuncConfig = [@{} mutableCopy];
feedsFuncConfig[@"thumbMode"] = @(NFFeedsOptionThumModSingle);
feedsFuncConfig[@"refreshCount"] = @(15);
//....添加其他需要的配置，如没有则省略
[NewsFeedsUISDK configWith:@{
                              NFFeedsUIOptionKey: feedsUIConfig,
                            NFFeedsFuncOptionKey: feedsFuncConfig
                             }];
```

##### 读取配置文件

1. json

   ```objective-c
   #import <NewsFeedsUISDK/NewsFeedsUISDK.h>

   [NewsFeedsUISDK startWithAppKey:appKey appSecret:secretKey];

   NSData *jsonString = [NSData dataWithContentsOfFile:@"path/to/your/config.json"];
   NSDictionary *config = [NSJSONSerialization JSONObjectWithData:jsonString options:0 error:nil];

   [NewsFeedsUISDK configWith:config];
   ```

   json文件格式请参杂[json模版文件](resources/NFConfig.json)

2. plist

   ```objective-c
   #import <NewsFeedsUISDK/NewsFeedsUISDK.h>

   [NewsFeedsUISDK startWithAppKey:appKey appSecret:secretKey];

   NSDictionary *config = [NSDictionary dictionaryWithContentsOfFile:@"path/to/your/config.plist"];

   [NewsFeedsUISDK configWith:config];
   ```

   plist文件格式请参照[plist模版文件](resources/NFConfig.plist)




#### 单独配置

单独配置允许用户配置更加个性化，比如用户需要显示两个信息流入口，可以为这两个入口提供不一样的视觉和功能。

SDK为创建每个模块提供了更加详细的接口:

1. `createFeedsView: delegate:config:extraData:`
2. `createArticleDetailView:delegate:config:extraData:extraData`
3. `createChannelContentViewController:type:delegate:config:extraData:`



以NFeedsView为例：

```objective-c
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>

NSMutableDictionary *feedsUIConfig = [@{} mutableCopy];
feedsUIConfig[@"cellTitleColor"] = @"0x333333";
feedsUIConfig[@"sourceTextColor"] = @"0x999999";
feedsUIConfig[@"separatorColor"] = @"0xd1d1d1 0.8";

NSMutableDictionary *feedsFuncConfig = [@{} mutableCopy];
feedsFuncConfig[@"thumbMode"] = @(NFFeedsOptionThumModSingle);
feedsFuncConfig[@"refreshCount"] = @(15);

NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController 
 					 delegate:self 
 					   config:@{
                                  NFFeedsUIOptionKey: feedsUIConfig,
                                  NFFeedsFuncOptionKey: feedsFuncConfig
                                }
 					extraData:@"seperate"];
```

同样的，单独配置也支持使用`json`等文件格式，不在赘述。

##### NFContainerClassKey

单独配置又增加了一个`NFContainerClassKey`用来限定UI配置生效的场景，`FuncOption`不需要此key。

为了说明这个参数，简要介绍一下SDK用于实现UI配置的原理。

SDK采用`UIAppearance`proxy来实现UI的配置，该`UIAppearance`引入了`UIAppearanceContainer`协议，其中`UIView`和`UIViewController`及其子类都实现了`UIAppearanceContainer`协议。`UIAppearance`proxy会根据view在不同的`UIAppearanceContainer`应用不同的配置。

所以传给`NFContainerClassKey`的值是实现`UIAppearanceContainer`的`Class`。

举例说明：

假设我们有这么一个场景，app需要添加两个信息流小入口在首页列表上，这两个入口需要配置不同的UI，我们可以如下设置

```objective-c
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>

NSMutableDictionary *channelUIConfig1 = [@{} mutableCopy];
channelUIConfig1[@"borderColor"] = @"0x333333 0.7";
channelUIConfig1[@"titleColor"] = @"0x999999";
channelUIConfig1[@"lineSpace"] = @(6);

NSMutableDictionary *channelFuncConfig1 = [@{} mutableCopy];
channelFuncConfig1[@"countInPage"] = @(1);
channelFuncConfig1[@"refreshCount"] = @(5);

UIViewController<NFChannelControllerProtocol> *cvc1 = 
	[NewsFeedsUISDK createChannelContentViewController:@"1001"                                                                                 											 type:NFChannelControllerTypeTitleBoard
                                          delegate:self
                                            config:@{
                                    NFTitleBoardChannelFuncOptionKey: channelFuncConfig1,
                                      NFTitleBoardChannelUIOptionKey: channelUIConfig1,
                                       NFContainerClassKey: NSStringFromClass([CustomCell1 class])
                                                   }                                                                                  
                                         extraData:@"entrance1"];

NSMutableDictionary *channelUIConfig2 = [@{} mutableCopy];
channelUIConfig2[@"borderColor"] = @"0x444444 0.7";
channelUIConfig2[@"titleColor"] = @"0x888888";
channelUIConfig2[@"lineSpace"] = @(7);

NSMutableDictionary *channelFuncConfig2 = [@{} mutableCopy];
channelFuncConfig2[@"countInPage"] = @(2);
channelFuncConfig2[@"refreshCount"] = @(10);

UIViewController<NFChannelControllerProtocol> *cvc2 = 
	[NewsFeedsUISDK createChannelContentViewController:@"1002"                                                                                 											 type:NFChannelControllerTypeTitleBoard
                                          delegate:self
                                            config:@{
                                    NFTitleBoardChannelFuncOptionKey: channelFuncConfig2,
                                      NFTitleBoardChannelUIOptionKey: channelUIConfig2,
                                       NFContainerClassKey: NSStringFromClass([CustomCell2 class])
                                                   }                                                                                  
                                         extraData:@"entrance2"];
```

`CustomCell1`和`CustomCell2`都是继承自`UITableViewCell`，所以自然也是遵守`UIAppearanceContainer`，只要这两个入口分别`addSubview:`到`CustomCell1`,`CustomCell2`, cvc1和cvc2就会实现不同的UI。

如果用户想把这两个入口显示在不同的`UIViewController`，那么只要将`CustomCell1`,`CustomCell2`替换成`CustomViewController1`和`CustomViewController2`。



**注意：**如果不提供`NFContainerClassKey`，单独的配置会覆盖全局配置。


### 相关定制属性详解

----------

#### 数据SDK

| 参数名                 | 类型             | 参数说明                                   | 取值说明                           |
| ------------------- | -------------- | -------------------------------------- | ------------------------------ |
| maxCacheNum         | NSInteger      | 最大缓存新闻摘要条数，即信息流页面获取缓存后展示的最大数目          | 取值范围10-100<sup>[[1]](#1)</sup> |
| autoRefreshInterval | NSTimeInterval | 每个频道自动刷新间隔，即当前时间与上次刷新时间间隔超过该值就刷新，用秒表示。 | 默认值是 60 * 60，0代表从不自动刷新。        |

*注意：*

数据sdk属性请在sdk初始化后立即配置，本次运行中不得更改。

#### 新闻详情页

-----------

- **功能自定义**

| 参数名                    | 类型                 | 参数说明          | 取值说明                                     |
| ---------------------- | ------------------ | ------------- | ---------------------------------------- |
| showRelated            | BOOL               | 文章详情内是否显示相关推荐 | 默认显示：YES，隐藏：NO                           |
| supportedSharePlatform | NSArray<NSNumber>* | 支持社交平台的枚举类型   | NFSharePlatform：NFSharePlatformWXSession(微信好友)          NFSharePlatformWXTimeline (微信朋友圈) |

- **UI自定义**

| 参数名                          | 类型                        | 参数说明                                   | 取值说明                                     |
| ---------------------------- | ------------------------- | -------------------------------------- | ---------------------------------------- |
| navBackgroundColor           | Color<sup>[[2]](#2)</sup> | navigationbar颜色                        | 颜色值                                      |
| navBottomLineColor           | Color                     | navigationbar阴影颜色                      | 颜色值                                      |
| navTintColor                 | Color                     | navigationbar title文字颜色                | 颜色值                                      |
| navBackButtonImage           | NSString*                 | navigationbar返回按钮图片                    | 图片名<sup>[[3]](#3)</sup>                  |
| navBackButtonHighlightImage  | NSString*                 | navigationbar返回按钮高亮图片                  | 图片名                                      |
| navShareButtonImage          | NSString*                 | navigationbar分享图片                      | 图片名                                      |
| navShareButtonHighlightImage | NSString*                 | navigationbar分享高亮图片                    | 图片名                                      |
| navType                      | NFArticleNavType          | 新闻页NavigationBar显示类型                   | NFArticleNavType: NFArticleNavTypeSource(显示新闻来源，默认值), NFArticleNavTypeTitle(显示新闻标题),            NFArticleNavTypeCustom(自定义文字),        NFArticleNavTypeNone(不显示) |
| customNavTitle               | NSString*                 | 如果navType=NFArticleNavTypeCustom时显示的文字 |                                          |
| backgroundColor              | Color                     | 正文背景颜色                                 | 颜色值                                      |
| titleColor                   | Color                     | 正文标题颜色                                 | 颜色值                                      |
| sourceTextColor              | Color                     | 文章来源颜色                                 | 颜色值                                      |
| timeTextColor                | Color                     | 文章发布时间颜色                               | 颜色值                                      |
| separatorColor               | Color                     | 标题和正文分割线颜色                             | 颜色值                                      |
| articleTextColor             | Color                     | 正文字体颜色                                 | 颜色值                                      |
| articleTextSize              | CGFloat                   | 正文字体大小                                 |                                          |
| paragraphIndent              | CGFloat                   | 正文段落缩进                                 | 可选值0，1，2。默认0                             |
| paragraphSpace               | CGFloat                   | 正文段落间距                                 | 可选值1.5，2，2.5。默认2。                        |
| lineSpace                    | CGFloat                   | 正文行间距                                  | 可选值1.2，1.5，2。默认1.5。                      |
| imagePlaceholderColor        | Color                     | 正文图片占位符背景色                             | 颜色值                                      |
| relatedUIOption              | NFFeedsUIOption           | 正文推荐相关设置                               | 请参考[信息流配置](UI自定义)                        |

#### 信息流小入口

- **功能自定义**

| 参数名                | 类型             | 参数说明         | 取值说明                                     |
| ------------------ | -------------- | ------------ | ---------------------------------------- |
| countInPage        | NSInteger      | 入口每页滚动显示的新闻数 | 取值1，2，默认值1                               |
| refreshCount       | NSInteger      | 每次刷新获取的新闻数   | 当countInPage=1时，范围1-5，默认值3。当countInPage=2时，范围2-10，默认值6. |
| autoScrollInterval | NSTimeInterval | 自动翻页的时间间隔    | 范围1-5，默认值1                               |

- **UI自定义**

| 参数名                   | 类型                       | 参数说明                | 取值说明                 |
| --------------------- | ------------------------ | ------------------- | -------------------- |
| iconImage             | NSString*                | navigationbar返回按钮图片 | 图片名， 建议大小32px*32px以下 |
| backgroundColor       | Color                    | 入口背景颜色              | 颜色值                  |
| selectBackgroundColor | Color                    | 入口选中背景颜色            | 颜色值                  |
| borderColor           | Color                    | 上下边界颜色              | 颜色值                  |
| titleColor            | Color                    | 标题颜色                | 颜色值                  |
| titleFont             | Font<sup>[[4]](#4)</sup> | 标题文字                | 字体                   |
| lineSpace             | CGFloat                  | 两行标题之间的间隔           | 只有当countInPage=2时生效  |

#### 信息流首页

- **功能自定义**

| 参数名              | 类型                   | 参数说明                 | 取值说明                                     |
| :--------------- | :------------------- | :------------------- | :--------------------------------------- |
| defaultChannelID | NSString*            | 一键集成默认进入的频道ID        | 默认进入切换到第一个                               |
| slidable         | BOOL                 | 频道是否可以滑动切换           | 默认可滑动：YES，不可滑动：NO                        |
| thumbMode        | NFFeedsOptionThumMod | 当前信息列表页每个cell的图片显示模式 | NFFeedsOptionThumModNone(无图)、 NFFeedsOptionThumModSingle(单图)、 NFFeedsOptionThumModNormal(按照新闻摘要的图片数显示, 默认值) |
| refreshCount     | NSInteger            | 每次下拉刷新请求的条数          | 范围5-30， 默认值10                            |

- **UI自定义**

| 参数名                           | 类型                            | 参数说明           |           取值说明            |
| :---------------------------- | :---------------------------- | :------------- | :-----------------------: |
| channelBackgroundColor        | Color                         | 频道条目背景色        |            颜色值            |
| normalTitleColor              | Color                         | 未选中状态下标题颜色     |            颜色值            |
| selectedTitleColor            | Color                         | 选中状态下标题颜色      |            颜色值            |
| selectedIndicatorColor        | Color                         | 选中状态下指示器颜色     |            颜色值            |
| showBottomLine                | BOOL                          | 是否展示底部分割线      |       默认显示YES，隐藏：NO       |
| bottomLineColor               | Color                         | 底部分割线颜色        |            颜色值            |
| backgroundColor               | Color                         | 信息流背景色         |            颜色值            |
| cellBackgroundColor           | Color                         | 单条新闻cell的背景色   |            颜色值            |
| highlightCellBackgroundColor  | Color                         | 点击新闻cell的高亮背景色 |            颜色值            |
| cellTitleColor                | Color                         | 新闻cell的标题字体颜色  |            颜色值            |
| cellTitleFont                 | Font                          | 新闻cell的标题字号    |        可取值14，16，18        |
| sourceTextColor               | Color                         | 新闻cell的来源字体颜色  |            颜色值            |
| sourceTextFont                | Font                          | 新闻cell的来源字体    |            字体             |
| contentInset                  | EdgeInset<sup>[[5]](#5)</sup> | 新闻cell的缩紧      |    默认值{16, 16, 16, 16}    |
| leftToRight                   | BOOL                          | 单图新闻图片饿方向      |  YES：图片在右边，默认。NO: 图片在左边   |
| smallImageRatio               | CGFloat                       | 小图长宽比          |           默认3/2           |
| largeImageRatio               | CGFloat                       | 大图长宽比          |           默认2/1           |
| trippleImageHSpace            | CGFloat                       | 三图新闻的图片间距      |            默认7            |
| titleImageVSpace              | CGFloat                       | 标题和图片的垂直方向间距   |            默认8            |
| imageSourceVSpace             | CGFloat                       | 图片和来源的垂直方向间距   |           默认10            |
| showSeparator                 | BOOL                          | 是否显示信息流列表分割线   |      默认显示：YES，隐藏：NO       |
| separatorColor                | Color                         | 信息流列表分割线颜色     | 如果showSeparator=NO，该值不起作用 |
| separatorInset                | EdgeInset                     | 新闻cell分割线锁进    |       默认{left: 16}        |
| lastReadBackgroundColor       | Color                         | 上次已读信息提示背景色    |            颜色值            |
| lastReadBorderColor           | Color                         | 上次已读信息提示边框色    |            颜色值            |
| lastReadTextColor             | Color                         | 上次已读信息提示字体颜色   |            颜色值            |
| pullingText                   | Color                         | 下拉刷新控件一般状态文字   |         默认值"下拉刷新"         |
| releaseToRefreshText          | NSString*                     | 下拉刷新控件松开状态文字   |         默认值"松开刷新"         |
| loadingText                   | NSString*                     | 下拉刷新控件加载中状态文字  |         默认值"加载中"          |
| refreshTextColor              | Color                         | 下拉刷新控件的字体颜色    |            颜色值            |
| refreshSuccessBackgroundColor | Color                         | 刷新成功后提示控件背景色   |            颜色值            |
| refreshSuccessTextColor       | Color                         | 刷新成功后提示控件字体颜色  |            颜色值            |
| loadMoreText                  | NSString*                     | 加载更多控件正在加载文字   |        默认值"正在载入更多"        |
| loadMoreTextColor             | Color                         | 加载更多控件字体颜色     |            颜色值            |
| noMoreTextColor               | Color                         | 没有更多控件字体颜色     |            颜色值            |

*注释：*
1. <a name="1" />保证使用取值范围内的值来配置UI，超出范围部分会导致UI显示出错。
2. <a name="2" />Color支持两种格式，如果是不透明的颜色，Color直接传颜色的16进制字符串，比如`0xcccccc`。如果是有透明度的颜色，Color支持传入一个颜色的字典表示，比如`@{"rgb": "0xdddddd", "alpha": "0.7"}`，其中`rgb`代表颜色的16进制字符串表示，`alpha`则是透明度表示，格式都是字符串
3. <a name="3" />图片传入图片名，现在只支持和程序一起打包的main bundle中的图片
4. <a name="4" />Font支持两种格式，如果使用系统字体，只需要传入一个float就行，表示字体大小。如果需要自定义字体，需要传入一个字体的字典表示，比如`@{"name": "Helvetica", "size": "15"}`
5. <a name="5" />EdgeInset是个字典，表示方式为`@{"top": "0", "left": "10", "bottom": "0", "right": "10"}`。如果某一个方向的缩紧是0，可以省略该方向的值，比如设置tableView separator inset, 则可以表示为`@{"left": "10"}`，表示左边锁进10pt