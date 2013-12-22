//
//  PMOpaques.h
//  BridgeMobile
//
//  Created by Juan David Hincapie Ramos on 12/15/13.
//  Copyright (c) 2013 Bridge PTM, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

struct PMDocRefImpl;

@interface PMDocRef : NSObject
{
@public
	struct PMDocRefImpl *impl;
}

-(id) initWithFilename:(char *)aFilename;
@end