/* Modified version of the original code ConvertFile.h
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
#ifndef __CSRSCALIBRATOR_H
#define __CSRSCALIBRATOR_H

#include <memory>

#include "CSRSUnpacker.h"
#include "ParserSRS.h"

#define MAX_BITS 33
#define NUM_BCID 5

/* struct VMM3DataCal; */

class CSRSCalibrator : public CEventProcessor
{
private:
  Gem::ParserSRS::VMM3DataCal *dataCal{nullptr};

public:
  CSRSCalibrator();
  virtual ~CSRSCalibrator();

  virtual Bool_t operator()(const Address_t pEvent,
                            CEvent &rEvent,
                            CAnalyzer &rAnalyzer,
                            CBufferDecoder &rDecoder);
};

#endif
