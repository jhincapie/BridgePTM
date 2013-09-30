//
//  Image.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "FeatureSet.h"

#ifndef BridgePTM_Image_h
#define BridgePTM_Image_h

class Image
{
    
public:

    const char* FileName = NULL;
    FeatureSet* Features = NULL;
    
    Image(const char* imageFile);
    
};

#endif
