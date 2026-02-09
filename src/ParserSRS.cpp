/* This software is Copyright by the Board of Trustees of Michigan
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
//===----------------------------------------------------------------------===//
///
/// \file
///
/// \brief Class to process datagram from Gd-GEM detector readout
/// from VMM3 ASICS via the SRS readout system
///
//===----------------------------------------------------------------------===//

#include <arpa/inet.h>
#include <cinttypes>
#include <cstdio>
#include <string.h>
#include <iostream>
#include "ParserSRS.h"

#include <iomanip>
#include <bitset>

namespace Gem
{

  ParserSRS::ParserSRS(int maxelements) : maxHits(maxelements)
  {
    data = new VMM3Data[maxHits];
  }

  ParserSRS::~ParserSRS()
  {
    if (data != nullptr) {
      delete[] data;
      data = nullptr;
    }
    
    // delete[] markers;
    // markers = nullptr;
  }

  void ParserSRS::reset()
  {
    if (data != nullptr) {
      delete[] data;
    }
    // Allocate new memory based on the original maxHits
    data = new VMM3Data[maxHits];
  }

  uint16_t ParserSRS::invertByteOrder(uint16_t data) {
    uint8_t lowerHalf = data & 0x00FF;
    uint8_t upperHalf = (data & 0xFF00) >> 8;
    return (lowerHalf << 8) | upperHalf;
  }

  int ParserSRS::parse(uint32_t data1, uint16_t data2, uint16_t data3, VMM3Data *vd)
  {
    int dataflag = (data2 >> 15) & 0x1;
    if (dataflag)
    {
      /// Data

      // uint64_t fecTimeStamp; /// 42 bits can change within a packet so must be here
      // uint16_t bcid;         /// 12 bit - bcid after graydecode
      // uint16_t adc;          /// 10 bit - adc value from vmm readout
      // uint8_t tdc;           ///  8 bit - tdc value from vmm readout
      // uint8_t chno;          ///  6 bit - channel number from readout
      // uint8_t overThreshold; ///  1 bit - over threshold flag for channel from readout
      // uint8_t vmmid;         ///  5 bit - asic identifier - unique id per fec 0 - 15
      // uint8_t triggerOffset; ///  5 bit


      // std::cout<<"bit rep of data1: "<<std::bitset<32>( data1 )<<std::endl;
      // std::cout<<"bit rep of data2: "<<std::bitset<16>( data2 )<<std::endl;
      // std::cout<<"bit rep of chno: "<<std::bitset<8>( (data2 >> 8) & 0x3f )<<std::endl;
      // std::cout<<"bit rep of tdc: "<<std::bitset<8>( data2 & 0xff )<<std::endl;
      // std::cout<<"bit rep of vmmid: "<<std::bitset<8>( (data1 >> 22) & 0x1F )<<std::endl;

      vd->overThreshold = (data2 >> 14) & 0x01;
      vd->chno = (data2 >> 8) & 0x3f;
      vd->tdc = data2 & 0xff;
      vd->vmmid = (data1 >> 22) & 0x1F;
      vd->adc = (data1 >> 12) & 0x3FF;
      vd->bcid = BitMath::gray2bin32(data1 & 0xFFF);
      vd->triggerOffset = (data1 >> 27) & 0x1F;
      // Added mapped channel to vmm data
      vd->chnoMapped = invertByteOrder(data3);

      return 1;
    }
    else
    {
      // No markers expected
      return 0;
    }
  }

}
