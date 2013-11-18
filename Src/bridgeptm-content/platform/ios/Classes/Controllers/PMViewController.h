//
//  PMViewController.h
//

#import <UIKit/UIKit.h>

@class PMAppContext;

@interface PMViewController : UIViewController

@property (nonatomic, strong) PMAppContext *context;

- (id)initWithContext:(PMAppContext*)context;

@end
