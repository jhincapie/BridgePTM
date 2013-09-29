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


#ifndef BridgePTM_FeatureCreator_h
#define BridgePTM_FeatureCreator_h

class FeatureCreator
{
private:
    
    cv::FREAK *extractor;
	cv::FastFeatureDetector* detector;
    
	std::vector<std::vector<cv::KeyPoint>> documentKP;
    cv::vector<cv::Mat> documentDes;

    void GetPages(char* documentFolder, std::vector<char *> * pages);
    
public:
    
	FeatureCreator();
	~FeatureCreator(void);
    
	void ComputeDocumentKP(const char* documentFolder);
    std::vector<std::vector<cv::KeyPoint>> GetDocumentKP();
};

#endif
