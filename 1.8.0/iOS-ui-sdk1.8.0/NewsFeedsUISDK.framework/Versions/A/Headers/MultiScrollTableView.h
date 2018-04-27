//
//  MultiScrollTableView.h
//  AFNetworking
//
//  Created by neal on 2018/1/18.
//

#import <UIKit/UIKit.h>

@protocol MultiScrollViewDelegate <NSObject>
@optional
- (void)nf_scrollViewDidScroll:(UIScrollView *)scrollView;

- (void)nf_scrollViewWillBeginDragging:(UIScrollView *)scrollView;
- (void)nf_scrollViewWillEndDragging:(UIScrollView *)scrollView withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset;
- (void)nf_scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate;

- (void)nf_scrollViewWillBeginDecelerating:(UIScrollView *)scrollView;
- (void)nf_scrollViewDidEndDecelerating:(UIScrollView *)scrollView;

- (void)nf_scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView;
@end

@interface MultiScrollTableView : UITableView <UIGestureRecognizerDelegate>

@property (nonatomic, strong) UIScrollView *simultaneousScrollView;

@end
