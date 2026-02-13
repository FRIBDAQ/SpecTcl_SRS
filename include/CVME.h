/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2026.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

    Author:
             Genie Jhang
         FRIB
         Michigan State University
         East Lansing, MI 48824-1321
*/
#ifndef __CVME_H
#define __CVME_H

#include <stdlib.h>
#include <string>
#include <TreeParameter.h>

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

extern class CVME vme;
    
// VME 
class CVME
{
  public:
    CVME(string name);
    virtual ~CVME();
    void Reset();
    void Initialize();
    string m_name;
  
    CTreeParameterArray adc;
};

#endif
