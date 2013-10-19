//
//  ViewController.m
//  BridgeMobile
//
//  Created by Juan David Hincapie Ramos on 10/17/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (IBAction)btnStartRegistration:(id)sender
{
    UIColor *highlightRed = [[UIColor alloc]initWithRed:255 green:0 blue:0 alpha:1];
    [btnRegistration setBackgroundColor:highlightRed];
}

- (IBAction)btnStopRegistration:(id)sender
{
    UIColor *highlightBlue = [[UIColor alloc]initWithRed:0 green:0 blue:255 alpha:1];
    [btnRegistration setBackgroundColor:highlightBlue];
}

- (void)viewDidLoad
{
    UIImage * pageImage = [UIImage imageNamed: @"interruptme-system-final-0.jpg"];
    ivImageHolder = [[UIImageView alloc]initWithImage:pageImage];
    
    [svImageNavigator setScrollEnabled:true];
    [svImageNavigator addSubview:ivImageHolder];
    [svImageNavigator setContentSize:ivImageHolder.frame.size];
    
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
