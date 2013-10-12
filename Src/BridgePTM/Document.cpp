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
    DIR *pDIR = opendir(documentFolder);
    if(pDIR == NULL)
        return this->Pages;
 
    if(this->Pages == NULL)
        this->Pages = new std::vector<Page *>();
    else
        this->Pages->clear();
    
    dirent * file = 0;
    while((file = readdir(pDIR)) != false)
    {
        if(strcasestr(file->d_name, ".jpg") == NULL)
            continue;
        char* imageName = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
		imageName[0] = '\0';
        strlcat(imageName, file->d_name, MAX_STRING_SIZE);
        
        Page* page = new Page(imageName);
        this->Pages->push_back(page);
        break;
    }
    closedir(pDIR);
    
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