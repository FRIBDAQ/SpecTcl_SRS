/*
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
#ifndef CSRSUnpacker_H
#define CSRSUnpacker_H

#include <config.h>
#include <EventProcessor.h>
#include <TreeParameter.h>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <chrono>

#include "ParserSRS.h"

static const uint32_t ALLH_TYPEMASK(0x7000000);
static const uint32_t ALLH_TYPESHIFT(24);
static const uint32_t ALLH_GEOMASK(0xf8000000);
static const uint32_t ALLH_GEOSHIFT(27);

    // High part of header.

static const uint32_t HDRH_CRATEMASK(0x00ff0000);
static const uint32_t HDRH_CRATESHIFT(16);

    // Low part of header.
static const uint32_t HDRL_COUNTMASK(0X3f00);
static const uint32_t HDRL_COUNTSHIFT(8);

    // High part of data:

static const uint32_t DATAH_CHANMASK(0x3f0000);
static const uint32_t DATAH_CHANSHIFT(16);

    // Low part of data

static const uint32_t DATAL_UNBIT(0x2000);
static const uint32_t DATAL_OVBIT(0x1000);
static const uint32_t DATAL_VBIT(0x40000);
static const uint32_t DATAL_DATAMASK(0x0fff);

    //  High part of trailer:- index in event to the first unprocessed word of the event.

static const uint32_t TRAILH_EVHIMASK(0x00ff0000);

    // Word types:

static const uint32_t HEADER(2);
static const uint32_t DATA(0);
static const uint32_t TRAILER(4);
static const uint32_t INVALID(6);


class CSRSUnpacker : public CEventProcessor
{
private:
  // With C++14 use unique_ptr
  // std::unique_ptr<Gem::ParserSRS> parser;
  Gem::ParserSRS *parser{nullptr};

  //non built physics event RI
  struct RIwBH
  {
    uint32_t size;
    uint32_t type;
    uint32_t sizeHeader;
    uint32_t timeStampLower;
    uint32_t padding; 
    uint32_t sourceId;
    uint32_t barrierType;
  };

  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_end0, m_end1, m_end2, m_end3;

  // //built physics event RI
  // struct FragRIwBH
  // {
  //   //total inclusinve size
  //   uint32_t nbBytes;
  //   //below is the fragment header
  //   uint32_t timeStampLowerFrag;
  //   uint32_t paddingFrag; 
  //   uint32_t sourceIdFrag;
  //   uint32_t payloadSizeFrag;
  //   uint32_t barrierTypeFrag;
  //   //below is the ring item header + the ring item body header
  //   uint32_t size;
  //   uint32_t type;
  //   uint32_t sizeHeader;
  //   uint32_t timeStampLower;
  //   uint32_t padding; 
  //   uint32_t sourceId;
  //   uint32_t barrierType;
  // };

  uint64_t timeStamp;

public:
  CSRSUnpacker();
  virtual ~CSRSUnpacker();

  virtual Bool_t operator()(const Address_t pEvent,
                            CEvent &rEvent,
                            CAnalyzer &rAnalyzer,
                            CBufferDecoder &rDecoder);

  uint32_t *unpack(std::uint32_t *begin,
                   std::uint16_t sizeInNbHit,
                   std::uint32_t &offset);

  uint32_t *unpackVME(std::uint32_t *begin,
                      std::uint32_t &offset);

  static const int fragAndRiHeader{48};
  static const int fragHeader{20};
};

#endif
