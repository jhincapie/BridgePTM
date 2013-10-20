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
    
}

- (IBAction)btnStopRegistration:(id)sender
{
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
}

@end
