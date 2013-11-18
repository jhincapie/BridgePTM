//
//  PMViewController.m
//

#import "PMViewController.h"
#import "PMAppContext.h"

@interface PMViewController ()

@end

@implementation PMViewController

- (id)initWithContext:(PMAppContext*)context{
    
    self = [self init];
    
    if(self){
        
        self.context = context;
        
    }
    
    return self;
    
}

@end
