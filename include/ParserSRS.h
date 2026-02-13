/* Copyright (C) 2018 European Spallation Source, ERIC. See LICENSE file */
/* Modified version of the original code ParserSRS.h
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
//===----------------------------------------------------------------------===//
///
/// \file
///
/// \brief Class to receive and generate Gd-GEM detector readout
/// from VMM3 ASICS via the SRS readout system
///
//===----------------------------------------------------------------------===//

#pragma once

#include <cinttypes>
#include <string.h>
#include "BitMath.h"
#include "SRSTime.h"
#include "Configuration.h"

namespace Gem
{

  class ParserSRS
  {
  public:
    /// \brief create a data handler for VMM3 SRS data of fixed size Capacity
    /// \param maxelements The maximum number of readout elements
    ParserSRS(int maxelements);
    virtual ~ParserSRS();
    // bytes
    static const int SRSHeaderSize{16};
    static const int HitInRingSize{12};
    static const int HitAndMarkerSize{8};
    static const int Data1Size{4};
    static const int Data2Size{2};
    static const int Data3Size{2};

    ///< Do NOT rearrange fields, used for casting to data pointer
    struct SRSHeader
    {
      uint32_t frameCounter;
      uint32_t dataId : 24; // Data ID: 0x564d33 - VMM3a Data
      uint8_t : 4;          // 4-bit Padding
      uint8_t fecId : 4;
      uint32_t udpTimestamp;
      uint32_t offsetOverflow;
    };

    // \todo no need for this struct
    struct VMM3Marker
    {
      uint64_t fecTimeStamp{0};  /// 42 bit
      uint64_t calcTimeStamp{0}; /// 42 bit
      uint16_t lastTriggerOffset{0};
      bool hasDataMarker{false};
    };

    /// Data common to all hits and markers, or other parser related data
    // struct ParserData
    // {
    //   uint8_t fecId{1};
    //   uint32_t nextFrameCounter{0};
    // };

    /// Data related to a single Hit
    // struct VMM3Data
    // {
    //   uint64_t fecTimeStamp; /// 42 bits can change within a packet so must be here
    //   uint16_t bcid;         /// 12 bit - bcid after graydecode
    //   uint16_t adc;          /// 10 bit - adc value from vmm readout
    //   uint8_t tdc;           ///  8 bit - tdc value from vmm readout
    //   uint8_t chno;          ///  6 bit - channel number from readout
    //   uint8_t overThreshold; ///  1 bit - over threshold flag for channel from readout
    //   uint8_t vmmid;         ///  5 bit - asic identifier - unique id per fec 0 - 15
    //   uint8_t triggerOffset; ///  5 bit
    //   bool hasDataMarker;    ///
    // };
    struct VMM3Data
    {
      uint64_t triggerTimeStamp; /// 42 bits can change within a packet so must be here
      uint64_t eventTimeStamp; /// 42 bits can change within a packet so must be here
      uint16_t bcid;         /// 12 bit - bcid after graydecode
      uint16_t adc;          /// 10 bit - adc value from vmm readout
      uint8_t tdc;           ///  8 bit - tdc value from vmm readout
      uint8_t chno;          ///  6 bit - channel number from readout
      uint16_t chnoMapped;   ///  16 bit - channel number mapped
      uint8_t overThreshold; ///  1 bit - over threshold flag for channel from readout
      uint8_t vmmid;         ///  5 bit - asic identifier - unique id per fec 0 - 15
      uint8_t fecid;         /// 
      uint8_t triggerOffset; ///  5 bit
      bool hasDataMarker;    ///
    };

    /// Data calibrated related to a single Hit
    struct VMM3DataCal
    {
      uint64_t timeStamp;    /// 42 bits can change within a packet so must be here
      uint16_t bcid;         /// 12 bit - bcid after graydecode
      uint16_t adc;          /// 10 bit - adc value from vmm readout
      uint8_t tdc;           ///  8 bit - tdc value from vmm readout
      uint8_t chno;          ///  6 bit - channel number from readout
      uint16_t chnoMapped;   ///  16 bit - channel number mapped
      uint8_t overThreshold; ///  1 bit - over threshold flag for channel from readout
      uint8_t vmmid;         ///  5 bit - asic identifier - unique id per fec 0 - 15
      uint8_t fecid;         /// 
      double correctedTime;  ///
    };

    void reset();

    /// \brief parse the readouts into a data array
    /// \param data1 the raw (unbitreversed) data1 field of a SRS packet
    /// \param data2 the raw (unbitreversed) data2 field of a SRS packet
    /// \param vmd VMM2Data structure holding the parsed data (tdc, bcid, adc, ...)
    // int parse(uint32_t data1, uint16_t data2, VMM3Data *vmd);
    int parse(uint32_t data1, uint16_t data2, uint16_t data3, VMM3Data *vmd);

    uint16_t invertByteOrder(uint16_t data);

    /// Holds data common to all readouts in a packet
    SRSHeader hdr;

    /// See description above
    //ParserData pd;

    /// holds all readout data in a packet (up to max_elems)
    VMM3Data *data{nullptr};

    /// holds time bases for all vmms in a readout
    VMM3Marker *markers{nullptr};

    int maxHits{0}; /// Maximum capacity of data array
    SRSTime srsTime;
  };
}
