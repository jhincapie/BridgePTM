//
//  main.cpp
//  BridgeConsole - This is a command line interface to the BridgePTM library
//
//  Created by Juan David Hincapié-Ramos on 9/22/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/stat.h>

#include "stdafx.h"

#include "BridgePTM/BridgePTM.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

void printUsage();
void printFolderNotExist();
void showMatch(const char* window, Image* capture, Match* match);

int main(int argc, const char * argv[])
{
    if(argc != 2)
    {
        printUsage();
        return 1;
    }

    if(!exists(argv[1]))
    {
        printFolderNotExist();
        printUsage();
        return 1;
    }

    //Creates the document object - loads the pages and content on the constructor
    Document * document = new Document(argv[1]);
    
    //Computes the document features
    FeatureCreator* creator = new FeatureCreator();
    creator->ComputeDocument(document);
    
    //Trains the matcher for the features of the document
    BridgeMatcher* matcher = new BridgeMatcher();
    matcher->Train(document);
    
    char* testvideo = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    testvideo[0] = 0;
    strlcat(testvideo, argv[1], MAX_STRING_SIZE);
#ifdef WIN32
    strlcat(testvideo, "\\", MAX_STRING_SIZE);
#else
	strlcat(testvideo, "//", MAX_STRING_SIZE);
#endif
    strlcat(testvideo, "test.MOV", MAX_STRING_SIZE);
	if(!exists(testvideo))
	{
		std::cerr << "Video does not exist [" << testvideo << "]" <<  std::endl;
		return 1;
	}

    cv::VideoCapture *capture = new cv::VideoCapture(testvideo);
	if (!capture->isOpened())
	{
		std::cerr << "Unable to load the video [" << testvideo << "]" <<  std::endl;
		return 1;
	}

    cv::Mat frame;// = new cv::Mat();
	cv::Mat halfframe;// = new cv::Mat();
    *capture >> frame;
    
    /* calculates the actual frame size */
    cv::Size smallSize((int)(frame.cols/4), (int)(frame.rows/4));
    
    int key = 'a';
    /* get fps, needed to set the delay */
    int fps = (int)capture->get(CV_CAP_PROP_FPS);
    
	Image *bridgeIMG = NULL;

    /* display video */
    cv::namedWindow("matches");
    while(key != 'q')
    {
        /* get a frame */
        *capture >> frame;
        /* always check */
        if(frame.empty())
            break;
        
        /* reduce in size */
		cv::resize(frame, halfframe, smallSize);
        cv::transpose(halfframe, halfframe);
        cv::flip(halfframe, halfframe, 1);
        cv::cvtColor(halfframe, halfframe, CV_BGR2GRAY);
        
		if(bridgeIMG == NULL)
			bridgeIMG = new Image(&halfframe);
		else
			bridgeIMG->UpdateData(&halfframe);
		creator->ComputeImage(bridgeIMG);

        Match* match = matcher->Match(bridgeIMG);
        if(match == NULL)
            continue;

        showMatch("matches", bridgeIMG, match);
		cvWaitKey(1000/fps);
    }
    
    /* free memory */
    capture->release();
    cvDestroyWindow("video");
    
    delete capture;
	delete creator;
	delete matcher;
	delete document;
}

void printUsage()
{
    std::cout << "Usage: BridgeConsole \"path to document folder\"" << std::endl;
}

void printFolderNotExist()
{
    std::cout << "Given folder does not exist, please use full path." << std::endl;
}

void showMatch(const char* window, Image* capture, Match* match)
{
    char * pageImagePath = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    
    pageImagePath[0] = 0; //clears the string
    strlcat(pageImagePath, match->Document->Root, MAX_STRING_SIZE);
    strlcat(pageImagePath, "/", MAX_STRING_SIZE);
    strlcat(pageImagePath, match->Page->FileName, MAX_STRING_SIZE);

    cv::Mat pageImage = cv::imread(pageImagePath, CV_LOAD_IMAGE_GRAYSCALE);
    
    std::vector<cv::Point2f> device_corners(5);
	device_corners[0] = cvPoint(0,0);
	device_corners[1] = cvPoint(capture->Capture->cols, 0 );
	device_corners[2] = cvPoint(capture->Capture->cols, capture->Capture->rows );
	device_corners[3] = cvPoint(0, capture->Capture->rows );
	device_corners[4] = cvPoint(capture->Capture->cols/2.0f, capture->Capture->rows/2.0f);
    
	if (!match->Homography.empty())
	{
		cv::perspectiveTransform(device_corners, device_corners, match->Homography);
        
		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		cv::line( pageImage, device_corners[0], device_corners[1] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[1], device_corners[2] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[2], device_corners[3] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[3] , device_corners[0] , cv::Scalar( 0, 255, 0), 4 );
	}
    
    //-- Draw only "good" matches
    cv::Mat matchesImg;
    cv::drawMatches(*capture->Capture, *capture->Features->keypoints,
                    pageImage, *match->Page->Features->keypoints,
                    *match->MatcherMatches, matchesImg, cv::Scalar::all(-1), cv::Scalar::all(-1),
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    
    //-- Show detected matches
    cv::imshow(window, matchesImg);

	delete pageImagePath;
}