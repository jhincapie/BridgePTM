//
//  Match.cpp
//  BridgePTM
//
//  Created by Juan David Hincapie Ramos on 10/3/13.
//  Copyright (c) 2013 Juan David Hincapié-Ramos. All rights reserved.
//

#include "Match.h"

Match::Match()
{
    this->Document = NULL;
    this->Page = NULL;
    this->PageIndex = -1;
}

Match::~Match()
{
    delete this->MatcherMatches;
}