//
//  main.cpp
//  BridgeConsole - This is a command line interface to the BridgePTM library
//
//  Created by Juan David Hincapié-Ramos on 9/22/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>

#include "stdafx.h"

#include "BridgePTM/BridgePTM.h"

void printUsage();
void printFolderNotExist();

int main(int argc, const char * argv[])
{
    if(argc != 2)
    {
        printUsage();
        return 1;
    }
    
    DIR *pDIR = opendir(argv[1]);
    if(pDIR == NULL)
    {
        printFolderNotExist();
        printUsage();
        return 1;
    }
    closedir(pDIR);

    //Creates the document object - loads the pages and content on the constructor
    Document * document = new Document(argv[1]);
    
    //Computes the document features
    FeatureCreator* creator = new FeatureCreator();
    creator->ComputeDocument(document);
    
    //Trains the matcher for the features of the document
    BridgeMatcher* matcher = new BridgeMatcher();
    matcher->Train(document->GetDescriptors());
    
    char* testvideo = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    testvideo[0] = 0;
    strlcat(testvideo, argv[1], MAX_STRING_SIZE);
    strlcat(testvideo, "//", MAX_STRING_SIZE);
    strlcat(testvideo, "test.MOV", MAX_STRING_SIZE);
    
    CvCapture *capture = cvCaptureFromFile(testvideo);
    if(capture == NULL)
        return 1;

    IplImage *frame;
    int key = 'a';
    /* get fps, needed to set the delay */
    int fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    /* display video */
    cvNamedWindow("video", 0);
    while(key != 'q')
    {
        /* get a frame */
        frame = cvQueryFrame( capture );
        /* always check */
        
        if( !frame ) break;
        /* display frame */
        cvShowImage( "video", frame );
        /* quit if user press 'q' */
        cvWaitKey( 1000 / fps );
    }
    
    /* free memory */
    cvReleaseCapture( &capture );
    cvDestroyWindow( "video" );

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