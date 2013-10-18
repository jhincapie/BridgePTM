//
//  ViewController.h
//  BridgeMobile
//
//  Created by Juan David Hincapie Ramos on 10/17/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
{
    int counter;
    
    IBOutlet UILabel* lbHelloWorld; 
}

- (IBAction)btnHelloWorld :(id)sender;

@end
