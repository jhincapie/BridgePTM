//
//  BridgeMatcher.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "BridgeMatcher.h"

BridgeMatcher::BridgeMatcher(int ipTableNumber, int ipKeySize, int ipMultiProbeLevel,
                             int spChecks, float spEPS, bool spSorted)
{
	this->document = NULL;
    
    this->ipTableNumber = ipTableNumber;
    this->ipKeySize = ipKeySize;
    this->ipMultiProbeLevel = ipMultiProbeLevel;
    this->spChecks = spChecks;
    this->spEPS = spEPS;
    this->spSorted = spSorted;
    this->matcher = new cv::FlannBasedMatcher(new cv::flann::LshIndexParams(ipTableNumber, ipKeySize, ipMultiProbeLevel),
                                              new cv::flann::SearchParams(spChecks, spEPS, spSorted));
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
    std::vector<std::vector<cv::DMatch>> cvmatches;
    this->matcher->knnMatch(*capture->Features->descriptors, cvmatches, 2);
    
    //-- Initializes the vote indices to 0 in each page
    int nrPages = (int)this->document->Pages->size();
    cv::vector<int> votingIndices;
    for (int i = 0; i < nrPages; i++)
        votingIndices.push_back(0);
    
    //-- Initializes the vectors for the matched points in each page
    std::vector<std::vector<cv::Point2f>> mpCapture, mpPage;
    std::vector< std::vector<cv::DMatch>* >* mtPage = new std::vector< std::vector<cv::DMatch>* >();
    for(int pageIndex = 0; pageIndex < nrPages; pageIndex++)
    {
        mtPage->push_back(new std::vector<cv::DMatch>());
        mpCapture.push_back(std::vector<cv::Point2f>());
        mpPage.push_back(std::vector<cv::Point2f>());
    }
    
    //-- Quick calculation of max and min distances between keypoints
    double max_dist = 0; double min_dist = 100;
    for(int pageIndex = 0; pageIndex < nrPages; pageIndex++)
    {
        for(int descIndex = 0 ; descIndex < cvmatches[pageIndex].size() ; descIndex++)
        {
            double dist = cvmatches[pageIndex][descIndex].distance;
            if( dist < min_dist )
                min_dist = dist;
            if( dist > max_dist )
                max_dist = dist;
        }
    }
    
    std::vector<std::vector<cv::DMatch>>::iterator endIterator = cvmatches.end();
    for (std::vector<std::vector<cv::DMatch>>::iterator iter = cvmatches.begin(); iter != endIterator; ++iter)
    {
        std::vector<cv::DMatch>::iterator match = iter->begin();
        if(!iter->empty())
        {
            if (match->distance < 2*min_dist)
            {
                mtPage->at(match->imgIdx)->push_back(*match);
                mpCapture[match->imgIdx].push_back(capture->Features->keypoints->at(match->queryIdx).pt);
                mpPage[match->imgIdx].push_back(docKeypoints->at(match->imgIdx).at(match->trainIdx).pt);
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
    if(max >= 4)
    {
        result = new class Match();
        result->Document = this->document;
        result->Page = this->document->Pages->at(pageIdx);
        result->PageIndex = pageIdx;
        result->MatcherMatches = mtPage->at(pageIdx);
        result->Homography = cv::findHomography(mpCapture[pageIdx], mpPage[pageIdx], cv::RANSAC);
    }
    
    clock_t end = clock();
    std::cout << "  -- Matching Time: " << (end - start) / (CLOCKS_PER_SEC/1000) << " ms" << std::endl;
    return result;
}
