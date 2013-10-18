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

- (IBAction)btnHelloWorld:(id)sender
{
    if(counter%2 == 0)
        lbHelloWorld.text = @"Juan David Hincapie-Ramos";
    else
        lbHelloWorld.text = @"Hello World!";
    counter++;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    counter = 0;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
