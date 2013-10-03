//
//  Match.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 10/3/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "General.h"
#include "Document.h"

#ifndef BridgePTM_Match_h
#define BridgePTM_Match_h

class Match
{
    
public:
    
    Match();
    ~Match();
    
    int PageIndex;
    Page* Page;
    Document*  Document;
    cv::Mat Homography;

};

#endif
