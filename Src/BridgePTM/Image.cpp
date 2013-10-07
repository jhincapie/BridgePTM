//
//  Image.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "Image.h"

Image::Image(const char* imageFile)
{
    this->Capture = NULL;
	this->Features = NULL;
    this->FileName = imageFile;
}

Image::Image(cv::Mat* capture)
{
    this->Features = NULL;
    this->FileName = NULL;
    this->Capture = capture;
}

Image::~Image()
{
    delete this->FileName;
	delete this->Features;
}
