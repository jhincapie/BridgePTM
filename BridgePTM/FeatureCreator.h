//
//  FeatureCreator.h
//  BridgePTM
//
//  This class creates the features in each page of a document.
//
//  Created by Juan David Hincapie Ramos on 9/26/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "Constants.h"
#include "Document.h"

#ifndef BridgePTM_FeatureCreator_h
#define BridgePTM_FeatureCreator_h

class FeatureCreator
{
    
private:
    
    cv::FREAK *extractor = NULL;
	cv::FastFeatureDetector* detector = NULL;
    
public:
    
	FeatureCreator();
	~FeatureCreator(void);
    
	void ComputeDocument(Document * document);
    void ComputeImage(Image* image);
};

#endif
