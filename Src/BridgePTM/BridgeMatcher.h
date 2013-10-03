//
//  BridgeMatcher.h
//  BridgePTM
//
//  This class is in charge of executing a very efficient search on a single document.
//  It's stateful, meaning that it takes into account the results of previous search requests
//   when perforning each new search. This allows optimizations like searching neighborgs first.
//
//  The current implementation has only one matcher. However, a more robust implementation
//  could have different matchers according to the descriptors he's using at a given moment. 
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "General.h"

#ifndef BridgePTM_BridgeMatcher_h
#define BridgePTM_BridgeMatcher_h

class BridgeMatcher
{
private:
    
    std::vector<cv::Mat> * descriptors;
    cv::FlannBasedMatcher* matcher;

public:
    
	BridgeMatcher();
	~BridgeMatcher(void);
    
	void Train(std::vector<cv::Mat> * descriptors);
    void Match(cv::Mat capture);
};

#endif
