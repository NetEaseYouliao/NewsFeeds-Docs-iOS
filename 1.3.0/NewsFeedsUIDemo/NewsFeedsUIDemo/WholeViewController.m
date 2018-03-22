//
//  ViewController.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/11.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "WholeViewController.h"
#import <NewsFeedsUISDK/NewsFeedsUISDK.h>
#import <NewsFeedsSDK/NewsFeedsSDK.h>

@interface WholeViewController () <NFeedsViewDelegate>

@end

@implementation WholeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    //设置广告的presentViewController
    [[NewsFeedsSDK sharedInstance] setAdPresentController:self];
    
    NFeedsView *feedsView = [NewsFeedsUISDK createFeedsView:self.navigationController delegate:self extraData:@"whole"];
    feedsView.frame = CGRectMake(0, 20 + 44, self.view.bounds.size.width, self.view.bounds.size.height - 64);
    [self.view addSubview:feedsView];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
