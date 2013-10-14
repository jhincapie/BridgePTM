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

    cv::SurfFeatureDetector* detector;
    cv::FREAK *extractor;
    
    void ComputeKeypoitsAndDescriptor(Image* image);
    
public:
    
    double dHessianThreshold;
    int dOctaves;
    int dOctaveLayers;
    bool dUpright;
    bool eOrientationNormalized;
    bool eScaleNormalized;
    float ePatternScale;
    int eNOctaves;
    
	FeatureCreator(double hessianThreshold=400,
                   int octaves=3,
                   int octaveLayers=4,
                   bool upright=false,
                   bool orientationNormalized = true,
                   bool scaleNormalized = true,
                   float patternScale = 22.0f,
                   int nOctaves = 4);
	~FeatureCreator(void);
    
	void ComputeDocument(Document * document);
    void ComputeImage(Image* image);
};

#endif
