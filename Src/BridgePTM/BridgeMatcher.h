//
//  BridgeMatcher.h
//  BridgePTM
//
//  This class is in charge of executing a very efficient search on a single document.
//  It's stateful, meaning that it takes into account the results of previous search requests
//   when perforning each new search. This allows optimizations like searching neighborgs first.
//
//  The current implementation has only one matcher. However, a more robust implementation
//  could have different matchers according to the descriptors he's using at a given moment. 
//
//  Created by Juan David Hincapie Ramos on 9/29/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "General.h"
#include "Image.h"
#include "Document.h"
#include "Match.h"

#ifndef BridgePTM_BridgeMatcher_h
#define BridgePTM_BridgeMatcher_h

class BridgeMatcher
{
private:
        
    Document* document;
    cv::FlannBasedMatcher* matcher;
    
public:
    
	BridgeMatcher(int ipTableNumber = 4, int ipKeySize = 25, int ipMultiProbeLevel = 0,
                  int spChecks = 32, float spEPS = 0, bool spSorted = true);
	~BridgeMatcher(void);
    
    int ipTableNumber;
    int ipKeySize;
    int ipMultiProbeLevel;
    int spChecks;
    float spEPS;
    bool spSorted;
    
	void Train(Document* document);
    Match* Match(Image* capture);
};

#endif
