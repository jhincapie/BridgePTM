//
//  Document.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>

#include "Constants.h"
#include "Image.h"

#ifndef BridgePTM_Document_h
#define BridgePTM_Document_h

class Page : public Image
{
    
public:
    
    Page(const char* pageFile);

};


class Document
{
    
private:
    
    std::vector<Page *> * GetPages(const char* documentFolder);
    
public:
    
    const char* Root = NULL;
    std::vector<Page *> * Pages = NULL;
    
    Document(const char* documentRoot);
    ~Document();
    
    std::vector<std::vector<cv::KeyPoint>> * GetKeypoints();
    std::vector<cv::Mat> * GetDescriptors();
    
};

#endif
