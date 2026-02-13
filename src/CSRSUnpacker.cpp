
/*  This software is Copyright by the Board of Trustees of Michigan
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
#include <BufferDecoder.h>
#include <TCLAnalyzer.h>
#include <iostream>
#include <netinet/in.h>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <netinet/in.h>
#include <memory>
#include <chrono>

#include "CSRSUnpacker.h"
#include "Configuration.h"
#include "CSRS.h"
#include "CVME.h"
#include "SRSTime.h"
#include "C785Unpacker.h"

using namespace std;

int eventTimestamp = 0;

CSRSUnpacker::CSRSUnpacker()
{
  // parser = nullptr; // Initialize to nullptr initially
}

CSRSUnpacker::~CSRSUnpacker()
{
  // if (parser != nullptr) {
  //   delete parser;
  //   parser = nullptr;
  // }
}

Bool_t
CSRSUnpacker::operator()(const Address_t pEvent,
                         CEvent &rEvent,
                         CAnalyzer &rAnalyzer,
                         CBufferDecoder &rDecoder)
{
  uint32_t *p = reinterpret_cast<uint32_t *>(pEvent);
  CTclAnalyzer &a(dynamic_cast<CTclAnalyzer &>(rAnalyzer));

  // nbBytes is an inclusive size
  uint32_t nbBytes = *p++;
  a.SetEventSize(nbBytes);

  uint32_t offset = 4;

  if (parser == nullptr) {
    parser = new Gem::ParserSRS(config.maxHits);
  } else {
    parser -> reset();
  }

  srs.Reset();
  srs.nHits = 0;

  while (1) {
    // Skipping timestamp
    p++; p++;
    offset += 8;

    uint32_t sourceid = *p++;
    offset += 4;

    uint32_t dataBytes = *p++ - 28; // subtracting RingItemHeader, BodyHeader, VME nwords, and VME scaler (8)
    offset += 4;

    // Skipping Barrier type
    p++;
    offset += 4;

    // Skipping RingItem Header and BodyHeader
    p += 7;
    offset += 28;

    switch (sourceid) {
      case 0:  // VME
        p = unpackVME(p, offset);
        break;

      case 12: // SRS
        // fragAndRiHeader: fragment header 20 bytes + ring item header 8 bytes + ring item body header 20 bytes) + 
        // HitAndMarkerSize: ring item body (data) multiple of 12 bytes
        if ((dataBytes % Gem::ParserSRS::HitInRingSize) != 0)
        {
      	  cout << "CSRSUnpacker - Error should have 12 bytes of data, datalen "<< dataBytes << " is not a multiple of 12 bytes" << endl;
      	  return kfFALSE;
        }
        uint16_t nbHits = dataBytes / Gem::ParserSRS::HitInRingSize;
      
        // Pass size (bytes) to analyzer.
        p = unpack(p, nbHits, offset);
        break;
    }

    if (nbBytes == offset)
      return kfTRUE;
    else
      return kfFALSE;
  }
}

uint32_t *
CSRSUnpacker::unpack(uint32_t *begin,
                     uint16_t nbHits,
		     uint32_t &offset)
{
  if (srs.debugMode > 0){
    std::cout<<"CSRSUnpacker::unpack -----New cluster-----: "<<std::endl;
  }
  try
  {
    // readoutIndex count all 12 bytes hits
    int readoutIndex = 0;

    // Feed the parser which fills an array of VMM3a data struct (channelNo, adc, tdc...)
    while (nbHits > 0)
    {
      // sizeof(uint32_t) for size of body size 
      //auto Data1Offset =  sizeof(uint32_t) + fragAndRiHeader + (fragAndRiHeader + Gem::ParserSRS::HitAndMarkerSize)*readoutIndex;
      auto Data1Offset = 0;
      auto Data2Offset = Data1Offset + Gem::ParserSRS::Data1Size;
      auto Data3Offset = Data2Offset + Gem::ParserSRS::Data2Size;
      auto Data4Offset = Data3Offset + Gem::ParserSRS::Data3Size;
//      auto headerOffset = Data1Offset - sizeof(RIwBH);
//      RIwBH* riHHit = reinterpret_cast<RIwBH*>(reinterpret_cast<uint8_t *>(begin) + headerOffset);
      // Assume we can have data comming from different fec.
      // dirty trick, now sourceId is fecid + 10, so: 
      parser->data[readoutIndex].fecid = 2;

      // Timestamping operations are done in udpBroker, no markers expected at this stage.
      // Shouldn't take the timestamp of build event but instead timestamp in riHHit.
      // If glom is large enought one can have hits with different timestamps.
      // Get the timestamp of the hit, start at padding and shift left by 32 bits.

      //std::cout<<"data offsets "<<Data1Offset<<" "<<Data2Offset<<" "<<Data3Offset<<std::endl;
      // std::cout<<"riHHit->sourceId and size "<<(int)riHHit->sourceId<<" "<<(int)riHHit->size<<std::endl;
      uint32_t *p1 = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(begin) + Data1Offset);
      uint16_t *p2 = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(begin) + Data2Offset);
      uint16_t *p3 = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(begin) + Data3Offset);
      uint32_t *p4 = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(begin) + Data4Offset);
      uint32_t data1 = htonl(*p1);
      uint16_t data2 = htons(*p2);
      uint16_t data3 = htons(*p3);
      uint32_t data4 = *p4;

      uint32_t timeStampHit = data4;

      int res = parser->parse(data1, data2, data3, &parser->data[readoutIndex]);
      parser->data[readoutIndex].triggerTimeStamp = timeStampHit;
      parser->data[readoutIndex].eventTimeStamp = eventTimestamp;

      //std::cout<<" parse result vmm "<<(int)parser->data[readoutIndex].vmmid<<" chno "<<(int)parser->data[readoutIndex].chno<<" "<<parser->data[readoutIndex].fecTimeStamp<<std::endl;
      if (res == 1)
      { // This was data
      }
      else
      {
        // Shouldn't find markers here 
        cout << "CSRSUnpacker - found a marker !" << endl;
      }
      readoutIndex++;
      srs.nHits = readoutIndex;
      nbHits--;

      begin++; begin++; begin++;
      offset += 12;

      if (readoutIndex == 2000)
      {
        printf("CSRSUnpacker - Data overflow, skipping %d hits", nbHits);
        break;
      }
    } // while nbHits
  }
  catch (exception &exc)
  {
    cout << "CSRSUnpacker - Parsing Failed! Reason=" << exc.what() << endl;
    return begin;
  }

  // Set srs raw data from the array of VMM3a data struct (parser)
  srs.SetDataRaw(parser->data);

  return begin;
}

uint32_t *
CSRSUnpacker::unpackVME(uint32_t *begin, uint32_t &offset)
{
  // Skipping VME word size
  uint16_t *p = reinterpret_cast<uint16_t *>(begin);
  p++;
  offset += 2;

  begin = reinterpret_cast<uint32_t *>(p);
  // Skipping VME scaler
  begin++; begin++;
  offset += 8;

  // Get the 'header' .. ensure that it is one and that it matches our VSN.

  unsigned long header = *begin++;
  if (header == 0xffffffff) {
    offset += 4;    // If immed BERR skip the BERR word and give up
    return begin;
  }

  int           vsn   = (header & ALLH_GEOMASK) >> ALLH_GEOSHIFT;
//  if(vsn != pMap->vsn) return offset;

  // Ok this is our data:

  offset += 4;      // Next longword..

  // I've seen cases where all I get is a _trailer_.. in that case
  // we're done so skip the analysis:

  if (((header & ALLH_TYPEMASK) >> ALLH_TYPESHIFT) != TRAILER) {

    unsigned long datum   = *begin++;
    offset += 4;      // skip even if its not a data long as it's a trailer then.

    while (((datum & ALLH_TYPEMASK) >> ALLH_TYPESHIFT) == DATA) {
      bool underflow = (datum & DATAL_UNBIT) != 0;
      bool overflow  = (datum & DATAL_OVBIT) != 0;

      // Must have valid and neither of the underflow/overflow bits.

      if (!(overflow || underflow)) {
        // Extract channel and the data:
      
        int channel = (datum & DATAH_CHANMASK) >> DATAH_CHANSHIFT;
        int value   = datum & DATAL_DATAMASK;
        vme.adc[channel] = value;
      }

      datum = *begin++;
      offset += 4;
    }
    // And damned if I havn't seen duplicated trailers as well so:

    while(((datum & ALLH_TYPEMASK) >> ALLH_TYPESHIFT) == TRAILER) {
      datum = *begin++;
      offset += 4;
    }
    begin--;
    offset -= 4;    // Don't count the non trailer longword.
  }

  // An extra 32 bits of 0xffffffff was read if not in a chain or if at
  // end of chain:

  if (*begin++ == 0xffffffff) {
    offset += 4;
  }

  return begin;
}
