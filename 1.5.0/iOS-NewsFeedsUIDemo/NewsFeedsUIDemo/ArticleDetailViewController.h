//
//  ArticleDetailViewController.h
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <UIKit/UIKit.h>

@class NFNewsInfo;

@protocol NewsMarkReadDelegate <NSObject>

- (void)markRead;

@end

@interface ArticleDetailViewController : UIViewController

@property (nonatomic, weak)   id<NewsMarkReadDelegate> delegate;
- (instancetype)initWithNews:(NFNewsInfo *)newsInfo;

@end
