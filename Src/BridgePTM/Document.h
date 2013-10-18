//
//  Document.h
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "General.h"
#include "Image.h"

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

#ifndef BridgePTM_Document_h
#define BridgePTM_Document_h

class Page : public Image
{
    
private:
    
public:
    
    Page(const char* pageFile);

};


class Document
{
    
private:
    
    std::vector<std::vector<cv::KeyPoint> > * documentKP;
    std::vector<cv::Mat> * documentDesc;
    
    std::vector<Page *> * GetPages(const char* documentFolder);
    
public:
    
    const char* Root;
    std::vector<Page *> * Pages;
    
    Document(const char* documentRoot);
    ~Document();
    
    std::vector<std::vector<cv::KeyPoint> > * GetKeypoints();
    std::vector<cv::Mat> * GetDescriptors();
    void ClearCache();
    
};

#endif
