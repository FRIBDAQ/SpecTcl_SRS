
/* Modified version of the original source code Clusterer.cpp
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
#include <BufferDecoder.h>
#include <TCLAnalyzer.h>
#include <iostream>
#include <netinet/in.h>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include <netinet/in.h>
#include <memory>

#include "CSRSUnpacker.h"
#include "Configuration.h"
#include "CSRS.h"
#include "SRSTime.h"

using namespace std;

CSRSUnpacker::CSRSUnpacker()
{
}

CSRSUnpacker::~CSRSUnpacker()
{
  delete parser;
  parser = nullptr;
}

Bool_t
CSRSUnpacker::operator()(const Address_t pEvent,
                         CEvent &rEvent,
                         CAnalyzer &rAnalyzer,
                         CBufferDecoder &rDecoder)
{
  // dont know how to use this smart pointer for parsing the datagram which is not necesarly a sequence of uint32_t, cannot cast srsHdr etc. (?)
  /* TranslatorPointer<uint32_t> p(*rDecoder.getBufferTranslator(), pEvent); */
  uint32_t *p = reinterpret_cast<uint32_t *>(pEvent);
  CTclAnalyzer &a(dynamic_cast<CTclAnalyzer &>(rAnalyzer));

  // dirty trick, pEvent points to the beginning of the datagram (body), but udpBroker makes a ring item with header, so knowing the pointer to the datagram one can point to the inclusive size in the ring item header.

  size_t sizeHeader = *(p - 5);
  size_t size = *(p - 7);
  /* size_t  size = 8976;*/

  /* for (int i = 1; i <= 7; i++) { */
  /*     uint32_t *byte = p -i; */
  /*     cout << i<<" "<<dec <<*byte<<" "<< ntohl(*byte)<<endl; */

  /* } */

  Gem::SRSTime srs_time;
  srs_time.bc_clock_MHz(config.pBC);
  srs_time.tac_slope_ns(config.pTAC);

  // With C++14 use unique_ptr and make_unique
  parser = new Gem::ParserSRS(config.maxHits, srs_time);
  // parser = std::unique_ptr<Gem::ParserSRS>(new Gem::ParserSRS(2000, srs_time));
  // parser = std::make_unique<Gem::ParserSRS>(2000, srs_time);

  std::cout << "Size " << size << " " << sizeHeader << " " << size - sizeHeader << dec << std::endl;
  std::cout << "Time "
            << " " << srs_time.bc_clock_MHz() << " " << srs_time.tac_slope_ns() << std::endl;
  size = size - sizeHeader;
  // pass size (bytes) to analyzer.
  a.SetEventSize(size);

  return unpack(p, size);
}

Bool_t
/* CSRSUnpacker::unpack(TranslatorPointer<uint32_t> begin, */
CSRSUnpacker::unpack(uint32_t *begin,
                     size_t size)
{
  Gem::ParserSRS::SRSHeader *srsHdr = reinterpret_cast<Gem::ParserSRS::SRSHeader *>(begin);
  uint32_t datagramSourceId = ntohl(srsHdr->dataId);
  parser->pd.fecId = srsHdr->fecId;

  // maybe add a protocol error counter here
  if ((datagramSourceId & 0xffffff00) != 0x564d3300)
  {
    std::cout << "CSRSUnpacker - Error no VMM3a data, datagramSourceId (hex): " << hex << datagramSourceId << dec << std::endl;
    return kfFALSE;
  }
  /* std::cout<<" dataId hex: "<<hex<<datagramSourceId<<dec<<std::endl; */

  try
  {
    // Reset parameters
    srs.Reset();
    int readoutIndex = 0;
    int dataIndex = 0;

    auto datalen = size - Gem::ParserSRS::SRSHeaderSize - 8;
    if ((datalen % 6) != 0)
    {
      cout << "CSRSUnpacker - Error datalen should be %6 " << datalen << " datalen/6 " << datalen / 6. << endl;
      return kfFALSE;
    }
    /* cout<<dec<<"datalen "<<datalen<<" datalen/6 "<<datalen/6.<<endl; */

    srs.eventlength = datalen; // in bytes

    // Feed the parser which fills an array of VMM3a data struct (channelNo, adc, tdc...)
    while (datalen >= Gem::ParserSRS::HitAndMarkerSize)
    {
      Gem::ParserSRS::VMM3Data event;
      auto Data1Offset = Gem::ParserSRS::SRSHeaderSize + Gem::ParserSRS::HitAndMarkerSize * readoutIndex;
      auto Data2Offset = Data1Offset + Gem::ParserSRS::Data1Size;
      /* std::cout<<"data offset "<<Data1Offset<<" "<<Data2Offset<<std::endl; */
      uint32_t *p1 = reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(begin) + Data1Offset);
      uint16_t *p2 = reinterpret_cast<uint16_t *>(reinterpret_cast<uint8_t *>(begin) + Data2Offset);
      uint32_t data1 = htonl(*p1);
      uint16_t data2 = htons(*p2);

      /* printf("data1: 0x%08x, data2: 0x%04x \n", data1, data2); */
      /* cout<<"bit rep of data1: "<<std::bitset<32>( data1 )<<endl; */
      /* cout<<"bit rep of data2: "<<std::bitset<16>( data2 )<<endl; */
      /* cout<<"bit rep of data1 >> 12: "<<std::bitset<32>( data1 >> 12 )<<endl; */
      /* cout<<"bit rep of data1 >> 12: "<<std::bitset<32>( 0x3FF)<<endl; */
      /* cout<<"bit rep of data1 >> 12: "<<std::bitset<32>( data1 >> 12 & 0x3FF)<<endl; */

      int res = parser->parse(data1, data2, &parser->data[dataIndex]);
      if (res == 1)
      { // This was data
        /* hits++; */
        dataIndex++;
        srs.nHits = dataIndex;
      }
      else
      {
      }
      readoutIndex++;

      datalen -= 6;
      if (dataIndex == config.maxHits && datalen > 0)
      {
        printf("Data overflow, skipping %d bytes", datalen);
        break;
      }
      /* std::cout<<" nHits "<<srs.nHits<<std::endl; */
    } // while
  }
  catch (exception &exc)
  {
    cout << "CSRSUnpacker - Parsing Failed! Reason=" << exc.what() << endl;
    return kfFALSE;
  }

  // Set srs raw data from the array of VMM3a data struct (parser)
  // DataParser is set first because it has fecId, needed in the next steps
  srs.SetDataParser(parser->pd);
  srs.SetDataRaw(parser->data);

  return kfTRUE;
}
