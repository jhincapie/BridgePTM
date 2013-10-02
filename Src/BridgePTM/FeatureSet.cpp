//
//  FeatureSet.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "FeatureSet.h"

FeatureSet::FeatureSet()
{
	this->keypoints = NULL;
	this->descriptors = NULL;
}

FeatureSet::~FeatureSet()
{
	delete this->descriptors;
	delete this->keypoints;
}