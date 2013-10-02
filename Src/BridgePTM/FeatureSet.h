//
//  FeatureSet.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#ifndef BridgePTM_FeatureSet_h
#define BridgePTM_FeatureSet_h

class FeatureSet
{

public:

    std::vector<cv::KeyPoint> * keypoints;
    cv::Mat * descriptors;

	FeatureSet();
	~FeatureSet();
};

#endif
