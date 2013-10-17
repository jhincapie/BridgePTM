//
//  Document.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "Document.h"

Page::Page(const char * pageImage) : Image(pageImage)
{
}

Document::Document(const char* documentRoot)
{
    this->Root = documentRoot;
	this->Pages = NULL;
    this->Pages = this->GetPages(this->Root);
    this->documentDesc = NULL;
    this->documentKP = NULL;
}

Document::~Document()
{
	delete this->Pages;
    delete this->documentKP;
    delete this->documentDesc;
}

std::vector<Page *> * Document::GetPages(const char* documentFolder)
{
    path * folder = new path(documentFolder);
    if(folder == NULL)
        return this->Pages;
    
    if(this->Pages == NULL)
        this->Pages = new std::vector<Page *>();
    else
        this->Pages->clear();
    
    directory_iterator end_itr; // default construction yields past-the-end
    for (directory_iterator itr(*folder); itr != end_itr; ++itr )
    {
        if (is_directory(itr->status()))
            continue;
        
        path * imageName = new path(itr->path().leaf());
        if(imageName->extension() != ".jpg")
            continue;
        
        Page* page = new Page(imageName->c_str());
        this->Pages->push_back(page);
        break;
    }
    
    return this->Pages;
}

std::vector<std::vector<cv::KeyPoint>> * Document::GetKeypoints()
{
    if(this->Pages == NULL || this->Pages->size() == 0)
        return NULL;
    if(this->documentKP != NULL)
        return this->documentKP;
    
    documentKP = new std::vector<std::vector<cv::KeyPoint>>();
    for(int pageIndex = 0 ; pageIndex < this->Pages->size() ; pageIndex++)
        documentKP->push_back(* this->Pages->at(pageIndex)->Features->keypoints);
    
    return documentKP;
}

std::vector<cv::Mat> * Document::GetDescriptors()
{
    if(this->Pages == NULL || this->Pages->size() == 0)
        return NULL;
    if(this->documentDesc != NULL)
        return this->documentDesc;
    
    documentDesc = new std::vector<cv::Mat>();
    for(int pageIndex = 0 ; pageIndex < this->Pages->size() ; pageIndex++)
        documentDesc->push_back(* this->Pages->at(pageIndex)->Features->descriptors);
    
    return documentDesc;
}

void Document::ClearCache()
{
    delete this->documentKP;
    this->documentKP = NULL;
    
    delete this->documentDesc;
    this->documentDesc = NULL;
}