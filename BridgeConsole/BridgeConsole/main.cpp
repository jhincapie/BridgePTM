//
//  main.cpp
//  BridgeConsole - This is a command line interface to the BridgePTM library
//
//  Created by Juan David Hincapié-Ramos on 9/22/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//
#include <iostream>

#include "BridgePTM/BridgePTM.h"

int main(int argc, const char * argv[])
{
    char* number = "7";
    FeatureCreator* creator = new FeatureCreator();
    creator->HelloWorld(number);
    cvWaitKey();
}

