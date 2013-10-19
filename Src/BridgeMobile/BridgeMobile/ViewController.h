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
    UIImageView* ivImageHolder;
    
    IBOutlet UIScrollView* svImageNavigator;
    IBOutlet UIButton* btnRegistration;
}

- (IBAction)btnStartRegistration :(id)sender;

- (IBAction)btnStopRegistration :(id)sender;

@end
