/* Modified version of the original source code XXXX.cpp
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2014.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

    Author:
             Simon Giraud
       FRIB
       Michigan State University
       East Lansing, MI 48824-1321
*/
#include <config.h>
#include <Event.h>
#include <EventProcessor.h>
#include <TCLAnalyzer.h>
#include <memory>

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

#include "CSRSAnalyzer.h"
#include "Configuration.h"
#include "CSRS.h"

CSRSAnalyzer::CSRSAnalyzer()
{
}

CSRSAnalyzer::~CSRSAnalyzer()
{
}

Bool_t
CSRSAnalyzer::operator()(const Address_t pEvent,
                           CEvent &rEvent,
                           CAnalyzer &rAnalyzer,
                           CBufferDecoder &rDecoder)
{

  cout << "SRSAnalyzer - begin " << endl;

  for (int i = 0; i < srs.nHits; i++)
  {
    // auto &d = srs.GetDataCal(i);
    // cout<<"CSRSAnalyzer - correctedTime "<<d.correctedTime<<endl;
  }
  
  return kfTRUE;
};
