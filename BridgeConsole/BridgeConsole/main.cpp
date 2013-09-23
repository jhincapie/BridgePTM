//
//  main.cpp
//  BridgeConsole - This is a command line interface to the BridgePTM library
//
//  Created by Juan David Hincapié-Ramos on 9/22/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(int argc, const char * argv[])
{
    cvNamedWindow( "My Window", 1 );
    IplImage *img = cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 1 );
    CvFont font;
    double hScale = 1.0;
    double vScale = 1.0;
    int lineWidth = 1;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);
    cvPutText(img, "Hello World!", cvPoint( 200, 400 ), &font, cvScalar(255, 255, 0));
    cvShowImage("My Window", img);
    
    // insert code here...
    std::cout << "Hello, World!\n";
    cvWaitKey();
    return 0;
}

