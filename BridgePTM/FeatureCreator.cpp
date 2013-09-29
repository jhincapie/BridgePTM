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
    free(this->extractor);
    free(this->detector);
}

void FeatureCreator::ComputeDocumentKP(const char* documentFolder)
{
    //1- Detects the features
    
    //2- Creates a descriptor for the gathered features
//    
//    documentKP->clear();
//    dbKeyPoints.clear();
//	cv::vector<cv::Mat> dbDescriptors;
//    
//	//load pages
//	std::string path = dir_path + "\\*";
//	std::wstring wsTmp(path.begin(), path.end());
//	std::wstring ws = wsTmp;
//	std::vector<std::string> fileNameList;
//	getFiles(ws, fileNameList);
//    
//    for (unsigned int i = 0; i < fileNameList.size(); i++)
//	{
//		std::string imagePath = dir_path + "/" + fileNameList[i];
//		cv::Mat pageImage = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
//        
//		cv::vector<cv::KeyPoint> pageKeyPoints;
//		if (_isCameraInUse)
//			fastDetectorPageImg->detect(pageImage, pageKeyPoints);
//		else surfDetectorPageImg->detect(pageImage, pageKeyPoints);
//        
//		cv::Mat pageImageDescriptors;
//		extractor->compute(pageImage, pageKeyPoints, pageImageDescriptors);
//        
//		dbDescriptors.push_back(pageImageDescriptors);
//		dbKeyPoints.push_back(pageKeyPoints);
//	}
//    
//    matcher->add(dbDescriptors);
//	matcher->train();
}
