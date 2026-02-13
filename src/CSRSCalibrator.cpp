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
#include <config.h>
#include <Event.h>
#include <EventProcessor.h>
#include <TCLAnalyzer.h>
#include <memory>

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

#include "CSRSCalibrator.h"
#include "CalibrationFile.h"
#include "Configuration.h"
#include "CSRS.h"

CSRSCalibrator::CSRSCalibrator()
{
  dataCal = new Gem::ParserSRS::VMM3DataCal[config.maxHits];
}

CSRSCalibrator::~CSRSCalibrator()
{
  delete[] dataCal;
  dataCal = nullptr;
}

Bool_t
CSRSCalibrator::operator()(const Address_t pEvent,
                           CEvent &rEvent,
                           CAnalyzer &rAnalyzer,
                           CBufferDecoder &rDecoder)
{

  if (config.useCalibration)
  {
    if (config.pDataFormat == "SRS")
    {
      double firstTime = 0;

      for (int i = 0; i < srs.nHits; i++)
      {
        auto &d = srs.GetDataRaw(i);
        if (d.eventTimeStamp > 0)
        {
          if (firstTime == 0)
          {
            firstTime = d.eventTimeStamp;
          }

          dataCal[i].timeStamp = d.eventTimeStamp - firstTime;
          // dataCal[i].timeStamp = d.fecTimeStamp;

          // cout<<"SRSCalibrator - srsTimeStamp after t0 "<<dataCal[i].timeStamp<<" "<<config.pBCTime_ns<<" "<<endl;
          auto cal = calib.getCalibration(d.fecid, d.vmmid, d.chno);
          /* std::cout<<"SRSCalibrator - time_offset "<<calib.time_offset<<std::endl; */

          // double chiptime =
          //     static_cast<double>(d.bcid) * config.pBCTime_ns +
          //     (1.5 * config.pBCTime_ns -
          //      static_cast<double>(d.tdc) *
          //          static_cast<double>(config.pTAC) / 255.0 -
          //      cal.time_offset) *
          //         cal.time_slope;

          // not the same than in vmmdc because here we use ext. trigger 
          // remove all constant terms that ultimately is just a constant, this constant will be calibrated with mask...
          double chiptime = static_cast<double>(d.bcid)*config.pBCTime_ns 
		            + static_cast<double>(d.triggerOffset)*4096.*config.pBCTime_ns
			    - d.triggerTimeStamp*config.pBCTime_ns + 1.5*config.pBCTime_ns
                            - (static_cast<double>(d.tdc)*static_cast<double>(config.pTAC)/255.0 - cal.time_offset)*cal.time_slope;
          //  cout<<"SRSCalibrator - corrected time "<<chiptime<<" "<<static_cast<double>(d.bcid)<<" "<<config.pBCTime_ns <<" "<<static_cast<double>(d.tdc) <<" "<<static_cast<double>(config.pTAC)<<" "<<cal.time_offset<<" "<<cal.time_slope <<" "<<endl;


          uint16_t corrected_adc = static_cast<uint16_t>(
              (static_cast<double>(d.adc) - cal.adc_offset) *
              cal.adc_slope);

          if (corrected_adc > 1023)
          {
            printf(
                "After correction, ADC value larger than 1023 "
                "(10bit)!\nUncorrected ADC value %d, uncorrected ADC "
                "value %d\n",
                d.adc, corrected_adc);

            corrected_adc = 1023;
          }
          else if (corrected_adc < 0)
          {
            printf(
                "After correction, ADC value smaller than 0!"
                "\nUncorrected ADC value %d, uncorrected ADC "
                "value %d\n",
                d.adc, corrected_adc);
            corrected_adc = 0;
          }

          // cout<<"SRSCalibrator - corrected Adc "<<(int)d.fecid<<" "<<(int)d.vmmid<<" "<<(int)d.chno<<" "<<d.adc<<" "<<corrected_adc<<" "<<cal.adc_offset<<" "<<cal.adc_slope<<endl;

          uint16_t adc = static_cast<uint16_t>(corrected_adc);
          dataCal[i].adc = adc;

          double timewalk_correction =
              cal.timewalk_d +
              (cal.timewalk_a - cal.timewalk_d) /
                  (1 +
                   pow(corrected_adc / cal.timewalk_c, cal.timewalk_b));

          double corrected_time = chiptime - timewalk_correction;
          dataCal[i].correctedTime = corrected_time;
          dataCal[i].tdc = d.tdc;
          dataCal[i].chno = d.chno;
          dataCal[i].chnoMapped = d.chnoMapped;
          /*!!!! changed chNoMapped here for test !!!!*/
          // dataCal[i].chnoMapped = d.chno;
          /*!!!! changed chNoMapped here for test !!!!*/
          dataCal[i].bcid = d.bcid;
          dataCal[i].overThreshold = d.overThreshold;
          dataCal[i].vmmid = d.vmmid;
          dataCal[i].fecid = d.fecid;

          // cout<<"SRSCalibrator - corrected time "<<chiptime<<" "<<corrected_time<<" "<<MaxVMMsData<<" "<<config.pFecVmm[d.fecid][d.vmmid]<<" "<<endl;

          if (d.vmmid < MaxVMMsData && config.pFecVmm[d.fecid][d.vmmid])
          {
            srs.fec[(int)d.fecid].adcCal[dataCal[i].chnoMapped] = adc;
            srs.fec[(int)d.fecid].timeCal[dataCal[i].chnoMapped] = corrected_time;
          }

        } //(.fecTimeStamp > 0)
      } // for nhits
      srs.SetDataCal(dataCal);
    }
  }

  return kfTRUE;
};
