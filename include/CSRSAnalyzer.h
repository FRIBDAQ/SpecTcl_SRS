/* Modified version of the original code XXXX.h
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
#ifndef __CSRSANALYZER_H
#define __CSRSANALYZER_H

#include <memory>

#include "CSRSUnpacker.h"
#include "CSRS.h"


class CSRSAnalyzer : public CEventProcessor
{
private:

public:
  CSRSAnalyzer();
  virtual ~CSRSAnalyzer();

  virtual Bool_t operator()(const Address_t pEvent,
                            CEvent &rEvent,
                            CAnalyzer &rAnalyzer,
                            CBufferDecoder &rDecoder);
};

#endif
