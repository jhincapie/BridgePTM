//
//  FeatureCreator.h
//  BridgePTM
//
//  This class creates the features in each page of a document.
//
//  Created by Juan David Hincapie Ramos on 9/26/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "General.h"
#include "Document.h"

#ifndef BridgePTM_FeatureCreator_h
#define BridgePTM_FeatureCreator_h

class FeatureCreator
{
    
private:
    
    cv::FREAK *extractor;
	cv::SurfFeatureDetector* detector;
    
    void ComputeKeypoitsAndDescriptor(Image* image);
    
public:
    
	FeatureCreator();
	~FeatureCreator(void);
    
	void ComputeDocument(Document * document);
    void ComputeImage(Image* image);
};

#endif
