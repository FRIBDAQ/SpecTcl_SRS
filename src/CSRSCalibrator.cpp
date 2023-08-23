/* Modified version of the original source code ConvertFile.cpp
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

  cout << "SRSCalibrator - begin " << endl;
  /* cout<<"SRSCalibrator - use calib - "<<srs.GetConfiguration().useCalibration<<endl; */

  /* m_config = srs.GetConfiguration(); */

  if (config.useCalibration)
  {
    cout << "SRSCalibrator - begin " << endl;
    if (!config.CreateMapping())
    {
      return -1;
    }
    // parsing bugger for the transform or dont know how to define .txt file...
    /* if (!config.CalculateTransform()) { */
    /*   return -1; */
    /* } */

    cout << "SRSCalibrator - after mapping and transform " << endl;

    /* Clusterer *m_Clusterer = new Clusterer(config, m_stats); */

    if (config.pDataFormat == "SRS")
    {
      double firstTime = 0;

      Gem::CalibrationFile calfile(config.pCalFilename);

      /* total_hits += srs.nHits; */
      for (int i = 0; i < srs.nHits; i++)
      {
        auto &d = srs.GetDataRaw(i);
        auto &d0 = srs.GetDataRaw(0);
        auto pd = srs.GetDataParser();

        double triggerOffset = -1.0;

        // triggerOffset goes from -1 to 15
        // but presented as uint8_t
        // latency violation
        if (d.triggerOffset <= 15.0)
        {
          triggerOffset = static_cast<double>(d.triggerOffset);
        }
        else if (d.triggerOffset == 31)
        {
          triggerOffset = -1.0;
        }
        else if (d.triggerOffset == 16)
        {
          triggerOffset = -99.0;
        }
        /* cout<<"SRSCalibrator - hasDatamarker "<<d.hasDataMarker<<" "<<d.fecTimeStamp<<" "<<triggerOffset<<endl; */
        if (d.hasDataMarker && d.fecTimeStamp > 0 && triggerOffset != -99)
        {
          double srs_timestamp =
              (static_cast<double>(d.fecTimeStamp) * config.pBCTime_ns +
               config.pOffsetPeriod * triggerOffset);
          if (firstTime == 0)
          {
            firstTime = srs_timestamp;
          }
          double t0_correction = 0;
          std::pair<uint8_t, uint8_t> fec_vmm =
              std::make_pair(pd.fecId, d.vmmid);
          auto searchMap = config.pFecVMM_time0.find(fec_vmm);
          if (searchMap != config.pFecVMM_time0.end())
          {
            std::string t0 = config.pFecVMM_time0[fec_vmm];
            if (t0 == "run")
            {
              t0_correction = firstTime;
            }
            else
            {
              t0_correction = std::stod(t0);
            }
          }
          srs_timestamp = srs_timestamp - t0_correction;
          dataCal[i].timeStamp = srs_timestamp;

          /* cout<<"SRSCalibrator - srsTimeStamp "<<srs_timestamp<<" "<<endl; */
          auto calib =
              calfile.getCalibration(pd.fecId, d0.vmmid, d0.chno);
          /* std::cout<<"SRSCalibrator - time_offset "<<calib.time_offset<<std::endl; */

          double chiptime =
              static_cast<double>(d.bcid) * config.pBCTime_ns +
              (1.5 * config.pBCTime_ns -
               static_cast<double>(d.tdc) *
                   static_cast<double>(config.pTAC) / 255.0 -
               calib.time_offset) *
                  calib.time_slope;

          /* cout<<"SRSCalibrator - corrected time "<<chiptime<<" "<<static_cast<double>(d.bcid)<<" "<<config.pBCTime_ns<<" "<< config.pBCTime_ns  <<" "<<static_cast<double>(d.tdc) <<" "<<static_cast<double>(config.pTAC)<<" "<<calib.time_offset<<" "<<calib.time_slope <<" "<<endl; */

          if (calib.adc_slope == 0)
          {
            // no correction
            calib.adc_slope = 1.0;
          }

          uint16_t corrected_adc = static_cast<uint16_t>(
              (static_cast<double>(d.adc) - calib.adc_offset) *
              calib.adc_slope);

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

          // cout<<"SRSCalibrator - corrected Adc "<<d.adc<<" "<<corrected_adc<<" "<<calib.adc_offset<<" "<<calib.adc_slope<<endl;

          uint16_t adc = static_cast<uint16_t>(corrected_adc);
          dataCal[i].adc = adc;

          double timewalk_correction =
              calib.timewalk_d +
              (calib.timewalk_a - calib.timewalk_d) /
                  (1 +
                   pow(corrected_adc / calib.timewalk_c, calib.timewalk_b));

          double corrected_time = chiptime - timewalk_correction;
          dataCal[i].correctedTime = corrected_time;
          dataCal[i].tdc = d.tdc;
          dataCal[i].chno = d.chno;
          dataCal[i].bcid = d.bcid;
          dataCal[i].overThreshold = d.overThreshold;
          dataCal[i].vmmid = d.vmmid;

          // cout<<"SRSCalibrator - corrected time "<<chiptime<<" "<<corrected_time<<" "<<" "<<endl;

          // following for another analysis class
          /* bool result = m_Clusterer->AnalyzeHits( */
          /* srs_timestamp, parser->pd.fecId, d.vmmid, d.chno, d.bcid, */
          /* d.tdc, adc, d.overThreshold != 0, corrected_time); */
          /* if (result == false || */
          /* (total_hits >= config.nHits && config.nHits > 0)) { */
          /* doContinue = false; */
          /* break; */
          /* } */
        }
      } // for nhits

      srs.SetDataCal(dataCal);

      /* srs.SetDataCal(*this); */
      // use dataCal for analysis
      //...
    }
  }

  return kfTRUE;
};
