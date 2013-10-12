//
//  Image.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "General.h"
#include "FeatureSet.h"

#ifndef BridgePTM_Image_h
#define BridgePTM_Image_h

class Image
{
    
public:

    cv::Mat* Capture;
    const char* FileName;
    FeatureSet* Features;
    
    Image(const char* imageFile);
    Image(cv::Mat* capture);
    ~Image();
    
    void UpdateData(cv::Mat* capture);
};

#endif
