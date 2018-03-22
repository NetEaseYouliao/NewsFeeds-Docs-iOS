//
//  NFChannelPotocol.h
//  Pods
//
//  Created by neal on 2017/12/14.
//


#import <Foundation/Foundation.h>

@class NFNewsInfo;
@class NFChannelInfo;
/**
 * @enum
 *
 * NFChannelControllerType定义了ChannelController的各种显示方式
 */
typedef NS_ENUM(NSUInteger, NFChannelControllerType) {
    /// 用单行新闻标题来显示，自动上下滚动显示全部新闻列表
    NFChannelControllerTypeTitleBoard
};

/**
 * @protocol
 */
@protocol NFControllerDelegate <NSObject>

/**
 * 
 */
- (void)addToController:(UIViewController *)controller inView:(UIView *)view;

/**
 */
- (void)removeFromController;

@end

/**
 @protocol
 
 */
@protocol NFChannelControllerProtocol <NFControllerDelegate>

/**
 *  @method
 *
 *  @abstract
 *  刷新新闻列表
 *  @discussion
 *  用户可以主动调用该方法刷新当前的新闻列表，
 */
- (void)refreshList;

/**
 *  @method
 *
 *  @abstract
 *  改变当前的channel
 *
 *  @param channelId    想要切换的channelId
 *
 */
- (void)changeChannelId:(NSString *)channelId;

@end

@protocol NFChannelControllerDelegate <NSObject>

@optional

/**
 *  @method
 *
 *  @abstract
 *  频道入口的点击回调
 *
 *  @param controller    当前的channelController实例
 *  @param newsInfo     点击的新闻
 *  @param channelInfo  当前频道
 *  @param extraData    用户的自定义字段
 *
 *  @discussion
 *  若用户未实现该回调，则SDK会自动跳转到信息流页面的当前频道列表页，并把入口可见的新闻放在列表最前面。
 *
 *  点击入口的新闻会触发这个方法，这时newsInfo就是当前点击的新闻，channelInfo是当前频道。
 *  点击空白区域也会触发这个方法，这时newsInfo是nil。其他同上。
 *
 */
- (void)channelController:(UIViewController<NFChannelControllerProtocol> *)controller
            didSelectNews:(NFNewsInfo *)newsInfo
                inChannel:(NFChannelInfo *)channelInfo
                extraData:(id)extraData;


@end
