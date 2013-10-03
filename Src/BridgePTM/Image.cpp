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
	this->Features = NULL;
    this->FileName = imageFile;
}

Image::~Image()
{
	delete this->Features;
}
