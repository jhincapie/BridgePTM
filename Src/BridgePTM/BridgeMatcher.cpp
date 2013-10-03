//
//  BridgeMatcher.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "BridgeMatcher.h"

BridgeMatcher::BridgeMatcher()
{
	this->document = NULL;
    this->matcher = new cv::FlannBasedMatcher(new cv::flann::LshIndexParams(4, 25, 0));
}

BridgeMatcher::~BridgeMatcher()
{
    delete this->matcher;
}

void BridgeMatcher::Train(Document* document)
{
    this->document = document;
    std::cout << "Training the Matcher for " << this->document->Pages->size() << " descriptors." << std::endl;
    clock_t start = clock();

    std::vector<cv::Mat> * docDescriptors = this->document->GetDescriptors();    
    this->matcher->add(*docDescriptors);
	this->matcher->train();
    
    clock_t end = clock();
    std::cout << "  -- Training Time: " << (end - start) / (CLOCKS_PER_SEC/1000) << " ms" << std::endl;
}

Match* BridgeMatcher::Match(Image* capture)
{
    if(capture->Features->descriptors->rows < 4)
        return NULL;
    
    std::cout << "Matching Image" << std::endl;
    clock_t start = clock();
    
    std::vector<std::vector<cv::KeyPoint>> * docKeypoints = this->document->GetKeypoints();
    std::vector<std::vector<cv::DMatch>> dmatches;
    this->matcher->knnMatch(*capture->Features->descriptors, dmatches, 2);
    
    //Initializes the vote indices to 0 in each page
    int nrPages = (int)this->document->Pages->size();
    cv::vector<int> votingIndices;
    for (int i = 0; i < nrPages; i++)
        votingIndices.push_back(0);
    
    std::vector<cv::Point2f> mpts_1, mpts_2; // Used for homography
    std::vector<std::vector<cv::DMatch>>::iterator endIterator = dmatches.end();
    for (std::vector<std::vector<cv::DMatch>>::iterator iter = dmatches.begin(); iter != endIterator; ++iter)
    {
        std::vector<cv::DMatch>::iterator match = iter->begin();
        if(!iter->empty())
        {
            if (match->distance < 0.7*(++(iter->begin()))->distance)
            {
                mpts_1.push_back(capture->Features->keypoints->at(match->queryIdx).pt);
                mpts_2.push_back(docKeypoints->at(match->imgIdx).at(match->trainIdx).pt);
                votingIndices[match->imgIdx]++;
            }
        }
    }
    
    int max = 0;
    int pageIdx = -1;
    for (unsigned int i = 0; i < votingIndices.size(); ++i)
    {
        if (votingIndices[i] <= max)
            continue;
        max = votingIndices[i];
        pageIdx = i;
    }
    
    class Match* result = NULL;
    if (mpts_1.size() >= 4)
    {
        result = new class Match();
        result->Document = this->document;
        result->Page = this->document->Pages->at(pageIdx);
        result->PageIndex = pageIdx;
        result->Homography = cv::findHomography(mpts_1, mpts_2, cv::RANSAC);
    }
    
    clock_t end = clock();
    std::cout << "  -- Matching Time: " << (end - start) / (CLOCKS_PER_SEC/1000) << " ms" << std::endl;
    return result;
}
