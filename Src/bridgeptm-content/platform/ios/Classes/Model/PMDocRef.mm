//
//  PMOpaques.m
//  BridgeMobile
//
//  Created by Juan David Hincapie Ramos on 12/15/13.
//  Copyright (c) 2013 Bridge PTM, Inc. All rights reserved.
//

#import "PMDocRef.h"

#import "BridgePTM.h"


struct PMDocRefImpl
{
    Document *document;
};

@implementation PMDocRef

-(id) initWithFilename:(char *)aFilename;
{
	self = [super init];
	if (self)
	{
        impl = new PMDocRefImpl;
        impl->document = new Document(aFilename);
	}
	return self;
}

-(void) dealloc
{
    delete impl->document;
    delete impl;
}

@end