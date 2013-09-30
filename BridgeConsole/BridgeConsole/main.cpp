//
//  main.cpp
//  BridgeConsole - This is a command line interface to the BridgePTM library
//
//  Created by Juan David Hincapié-Ramos on 9/22/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>

#include "BridgePTM/BridgePTM.h"

void printUsage();
void printFolderNotExist();

int main(int argc, const char * argv[])
{
    if(argc != 2)
    {
        printUsage();
        return 1;
    }
    
    DIR *pDIR = opendir(argv[1]);
    if(pDIR == NULL)
    {
        printFolderNotExist();
        printUsage();
        return 1;
    }
    closedir(pDIR);

    //Creates the document object - loads the pages and content on the constructor
    Document * document = new Document(argv[1]);
    
    //Computes the document features
    FeatureCreator* creator = new FeatureCreator();
    creator->ComputeDocument(document);
    
    //Trains the matcher for the features of the document
    BridgeMatcher* matcher = new BridgeMatcher();
    matcher->Train(document->GetDescriptors());
    
    cvWaitKey();
}

void printUsage()
{
    std::cout << "Usage: BridgeConsole \"path to document folder\"" << std::endl;
}

void printFolderNotExist()
{
    std::cout << "Given folder does not exist, please use full path." << std::endl;
}