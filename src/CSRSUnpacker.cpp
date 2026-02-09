
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
#include "SRSTime.h"

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

  // m_start = std::chrono::high_resolution_clock::now();

  // Dont know how to use this smart pointer for parsing the datagram which is not necesarly a sequence of uint32_t.
  /* TranslatorPointer<uint32_t> p(*rDecoder.getBufferTranslator(), pEvent); */

  uint16_t *p = reinterpret_cast<uint16_t *>(pEvent);
  CTclAnalyzer &a(dynamic_cast<CTclAnalyzer &>(rAnalyzer));

  // Trying to find out the event size
  int eventSize = 0;
  while (1) {
    bool isEndOfEvent = *(p + 2) == 0x1e || *(p + 2) == 0x2;

    if (isEndOfEvent) {
      p = p - eventSize/2;
      break;
    } else {
      eventSize += Gem::ParserSRS::HitInRingSize;
      p = p + Gem::ParserSRS::HitInRingSize/2;
    }
  }

  uint32_t nbBytes = eventSize;
  // nbBytes is an inclusive size
  auto datalen = eventSize;

  // fragAndRiHeader: fragment header 20 bytes + ring item header 8 bytes + ring item body header 20 bytes) + 
  // HitAndMarkerSize: ring item body (data) 8 bytes
  if ((datalen % (Gem::ParserSRS::HitInRingSize)) != 0)
  {
    cout << "CSRSUnpacker - Error should have bytes of data, datalen "<<datalen<< " is not a multiple of " << Gem::ParserSRS::HitInRingSize << " bytes" << endl;
    return kfFALSE;
  }
  uint16_t nbHits = datalen / Gem::ParserSRS::HitInRingSize;

//  std::cout<<"nbHits datalen "<<nbHits<<" "<<datalen<<std::endl;

  // // // With C++14 use unique_ptr and make_unique
  // parser = new Gem::ParserSRS(config.maxHits);
  if (parser == nullptr) {
    parser = new Gem::ParserSRS(config.maxHits);
  } else {
    parser->reset();
  }

  // Pass size (bytes) to analyzer.
  a.SetEventSize(nbBytes);

  // m_end0 = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double, std::nano> elapsed_time0 = m_end0 - m_start; 
  // auto elapsed_time_0_s = elapsed_time0.count()/1e9;
  // std::cout<<"elapsed0 "<<elapsed_time_0_s<<std::endl;

  eventTimestamp++;

  uint32_t *q = reinterpret_cast<uint32_t *>(p);
  return unpack(q, nbHits);
}

Bool_t
CSRSUnpacker::unpack(uint32_t *begin,
                     uint16_t nbHits)
{
  srs.Reset();

  if (srs.debugMode > 0){
    std::cout<<"CSRSUnpacker::unpack -----New cluster-----: "<<std::endl;
  }
  try
  {
    // readoutIndex count all 8 bytes hits
    int readoutIndex = 0;

    // Feed the parser which fills an array of VMM3a data struct (channelNo, adc, tdc...)
    while (nbHits > 0)
    {
      // sizeof(uint32_t) for size of body size 
      auto Data1Offset = Gem::ParserSRS::HitInRingSize*readoutIndex;
      auto Data2Offset = Data1Offset + Gem::ParserSRS::Data1Size;
      auto Data3Offset = Data2Offset + Gem::ParserSRS::Data2Size;
      auto Data4Offset = Data3Offset + Gem::ParserSRS::Data3Size;
      // Fix it for 2 ror now
      parser->data[readoutIndex].fecid = 2;//riHHit->sourceId - 10;

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
      if (srs.debugMode > 0){
        std::cout<<"CSRSUnpacker::unpack - timeStampHit: "<<timeStampHit<<std::endl;
      }

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
    return kfFALSE;
  }

  // Set srs raw data from the array of VMM3a data struct (parser)
  srs.SetDataRaw(parser->data);

  return kfTRUE;
}
