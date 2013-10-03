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
    this->extractor = new cv::FREAK(true, false, 13.0F, 2);
    this->detector = new cv::FastFeatureDetector(30, true);
}

FeatureCreator::~FeatureCreator(void)
{
    delete this->extractor;
    delete this->detector;
}

void FeatureCreator::ComputeDocument(Document* document)
{
    if(document == NULL)
        return;
    
    std::cout << "Computing Document Keypoints: " << document->Root << std::endl;
    
    //0- Gets the page files from the document folder
    std::cout << "Pages Found: " << document->Pages->size() << std::endl;
    
    char * pageImagePath = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    for(int pageIndex = 0 ; pageIndex < document->Pages->size() ; pageIndex++)
    {
        Page* page = document->Pages->at(pageIndex);
        size_t requiredLenght = strlen(document->Root) + strlen(page->FileName) + 1;
        if(requiredLenght >= MAX_STRING_SIZE)
        {
            std::cout << "Page Path Too Long: " << page->FileName << std::endl;
            continue;
        }
        
        pageImagePath[0] = 0; //clears the string
        strlcat(pageImagePath, document->Root, MAX_STRING_SIZE);
        strlcat(pageImagePath, "/", MAX_STRING_SIZE);
        strlcat(pageImagePath, page->FileName, MAX_STRING_SIZE);
        
        //1- Detects the features or keypoints
        cv::Mat pageImage = cv::imread(pageImagePath, CV_LOAD_IMAGE_GRAYSCALE);
        page->Capture = &pageImage;
        this->ComputeKeypoitsAndDescriptor(page);
    }
    
    free(pageImagePath);
}

void FeatureCreator::ComputeImage(Image* image)
{
    if(image == NULL)
        return;
    this->ComputeKeypoitsAndDescriptor(image);
}

void FeatureCreator::ComputeKeypoitsAndDescriptor(Image* image)
{
    if(image == NULL)
    {
        std::cout << "ComputeKeypoitsAndDescriptor: Image* image == NULL" << std::endl;
        return;
    }
    
    if(image->Capture == NULL)
    {
        std::cout << "ComputeKeypoitsAndDescriptor: image->Capture == NULL" << std::endl;
        return;
    }
    
    if(image->FileName != NULL)
        std::cout << "Computing Page Keypoints: " << image->FileName << std::endl;
    else
        std::cout << "Computing Page Keypoints (Capture)" << std::endl;
    clock_t start = clock();
    
    cv::vector<cv::KeyPoint> * pageKeyPoints = new cv::vector<cv::KeyPoint>();
    this->detector->detect(*image->Capture, *pageKeyPoints);
    std::cout << "  -- Keypoints Found: " << pageKeyPoints->size() << std::endl;
    
    //2- Creates a descriptor for the gathered features
    cv::Mat * pageImageDescriptors = new cv::Mat();
    extractor->compute(*image->Capture, *pageKeyPoints, *pageImageDescriptors);
    std::cout << "  -- Descriptors Created: " << pageImageDescriptors->size() << std::endl;
    
    clock_t end = clock();
    std::cout << "  -- Computing Time: " << (end - start) / (CLOCKS_PER_SEC/1000) << " ms" << std::endl;
    
    FeatureSet* set = new FeatureSet();
    set->keypoints = pageKeyPoints;
    set->descriptors = pageImageDescriptors;
    image->Features = set;
}
