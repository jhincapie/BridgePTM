//
//  FeatureCreator.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/27/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "FeatureCreator.h"

FeatureCreator::FeatureCreator()
{
}

FeatureCreator::~FeatureCreator(void)
{
}

void FeatureCreator::HelloWorld(char* text)
{
    cvNamedWindow( "My Window", 1 );
    IplImage *img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 1 );
    CvFont font;
    double hScale = 1.0;
    double vScale = 1.0;
    int lineWidth = 1;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
    cvPutText(img, text, cvPoint( 200, 400 ), &font, cvScalar(255, 255, 0));
    cvShowImage("My Window", img);
}