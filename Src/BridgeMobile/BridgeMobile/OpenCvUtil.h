//
//  OpenCvUtil.h
//  BridgeMobile
//
//  Created by Juan David Hincapie Ramos on 10/20/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#import <Foundation/Foundation.h>

using namespace cv;

@interface OpenCvUtil : NSObject
{
}

// Convert from UIImage to cvMat - color and gray
- (cv::Mat)cvMatFromUIImage:(UIImage *)image;
- (cv::Mat)cvMatGrayFromUIImage:(UIImage *)image;

// Convert to from cvMat to UIImage
- (UIImage *)UIImageFromCVMat:(cv::Mat)cvMat;

@end
