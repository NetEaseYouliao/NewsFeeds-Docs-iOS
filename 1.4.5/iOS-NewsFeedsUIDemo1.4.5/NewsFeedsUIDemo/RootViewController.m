//
//  RootViewController.m
//  NewsFeedsUIDemo
//
//  Created by shoulei ma on 2017/10/13.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "RootViewController.h"
#import "WholeViewController.h"
#import "SeperateViewController.h"

@interface RootViewController ()

@end

@implementation RootViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    UIButton *wholeButton = [[UIButton alloc] initWithFrame:CGRectMake(108, 200 , 150, 50)];
    [self.view addSubview:wholeButton];
    [wholeButton setTitle:@"View整合" forState:UIControlStateNormal];
    [wholeButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    wholeButton.backgroundColor = [UIColor grayColor];
    [wholeButton addTarget:self action:@selector(wholeButtonClick) forControlEvents:UIControlEventTouchUpInside];
    
    UIButton *seperateButton = [[UIButton alloc] initWithFrame:CGRectMake(108, 400 , 150, 50)];
    [self.view addSubview:seperateButton];
    [seperateButton setTitle:@"View拆分" forState:UIControlStateNormal];
    [seperateButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    seperateButton.backgroundColor = [UIColor grayColor];
    [seperateButton addTarget:self action:@selector(seperateButtonClick) forControlEvents:UIControlEventTouchUpInside];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self.navigationController.navigationBar setBarTintColor:[UIColor  colorWithRed:255/255.0 green:255/255.0 blue:255/255.0 alpha:1]];
    [self.navigationController.navigationBar setTitleTextAttributes:[NSDictionary dictionaryWithObjectsAndKeys: [UIColor whiteColor], NSForegroundColorAttributeName, [UIFont fontWithName:@"PingFangSC-Medium" size:19], NSFontAttributeName, nil]];
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleDefault;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)wholeButtonClick {
    WholeViewController *wholeVC = [[WholeViewController alloc] init];
    [self.navigationController pushViewController:wholeVC animated:YES];
}

- (void)seperateButtonClick {
    SeperateViewController *SeperateVC = [[SeperateViewController alloc] init];
    [self.navigationController pushViewController:SeperateVC animated:YES];
}

@end
