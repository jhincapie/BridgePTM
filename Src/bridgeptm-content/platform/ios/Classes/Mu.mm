//
//  Mu.mm
//  BridgeMobile
//
//  Created by Andrés Castaño on 10/11/2013.
//

#import "Mu.h"

#import "opencv2/opencv.hpp"

#import "Image.h"

@implementation Mu

-(void)test
{
    cv::Mat image;
    image.resize(10);
    Image* test = new Image("pathtoimage.png");
}

@end
