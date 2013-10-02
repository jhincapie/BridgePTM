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
	this->descriptors = NULL;
    this->matcher = new cv::FlannBasedMatcher(new cv::flann::LshIndexParams(4, 25, 0));
}

BridgeMatcher::~BridgeMatcher()
{
    free(this->matcher);
}

void BridgeMatcher::Train(std::vector<cv::Mat> * descriptors)
{
    this->descriptors = descriptors;
    this->matcher->add(*this->descriptors);
	this->matcher->train();
}

void BridgeMatcher::Match(cv::Mat capture)
{
//	std::vector<cv::KeyPoint> deviceKeypoints;
//	cv::Mat deviceImageDescriptors;
//    
//	if (isCameraInUse_)
//		fastDetectorCamImg->detect(deviceImage, deviceKeypoints);
//	else
//		surfDetectorCamImg->detect(deviceImage, deviceKeypoints);
//	extractor->compute(deviceImage, deviceKeypoints, deviceImageDescriptors);
//	
//	if (deviceImageDescriptors.rows > 4)
//	{
//		std::vector<std::vector<cv::DMatch>> dmatches;
//		fMatcher->knnMatch(deviceImageDescriptors, dmatches, 2);
//        
//		//set votingPageIndices to null
//		int count = dbKeyPoints.size();
//		cv::vector<int> votingPageIndices;
//		for (int i = 0; i < count; i++)
//			votingPageIndices.push_back(0);
//        
//		std::vector<cv::Point2f> mpts_1, mpts_2; // Used for homography
//		std::vector<std::vector<cv::DMatch>>::iterator endIterator = dmatches.end();
//		for (std::vector<std::vector<cv::DMatch>>::iterator iter = dmatches.begin(); iter != endIterator; ++iter)
//		{
//			std::vector<cv::DMatch>::iterator firstMatch = iter->begin();
//			if(!iter->empty())
//			{
//				if (firstMatch->distance < 0.7*(++(iter->begin()))->distance)
//				{
//					mpts_1.push_back(deviceKeypoints[firstMatch->queryIdx].pt);
//					mpts_2.push_back(dbKeyPoints[firstMatch->imgIdx][firstMatch->trainIdx].pt);
//					votingPageIndices[firstMatch->imgIdx]++;
//				}
//			}
//		}
//        
//		int max = 0;
//		PageIdx = -1;
//		for (unsigned int i = 0; i < votingPageIndices.size(); ++i)
//		{
//			if (votingPageIndices[i] > max)
//			{
//				max = votingPageIndices[i];
//				PageIdx = i;
//			}
//		}
//        
//		if (mpts_1.size() >= 4)
//			return findHomography(mpts_1, mpts_2, cv::RANSAC);
//	}
//    
//	return cv::Mat();
}
