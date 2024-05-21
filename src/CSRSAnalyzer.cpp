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

#include "CSRSAnalyzer.h"
#include "Configuration.h"
#include "CSRS.h"

// Flags for time and position calc. method
#define AVERAGE 0
#define COG   1
#define COG2   2
#define TIME_FIRST_HIT   3
#define TIME_LAST_HIT   4
#define TOTAL   1

CSRSAnalyzer::CSRSAnalyzer()
{
}

CSRSAnalyzer::~CSRSAnalyzer()
{
}

Bool_t
CSRSAnalyzer::operator()(const Address_t pEvent,
                           CEvent &rEvent,
                           CAnalyzer &rAnalyzer,
                           CBufferDecoder &rDecoder)
{

  switch ((int)srs.timeMethod) {
  case AVERAGE:
    averageT();
    break;
  case COG:
    cogT();
    break;
  case COG2:
    cog2T();
    break;
  case TIME_FIRST_HIT:
    firstTime();
    break;
  case TIME_LAST_HIT:
    lastTime();
    break;
  default:
    firstTime();
    break;
  }

  switch ((int)srs.posMethod) {
  case AVERAGE:
    averageP();
    break;
  case COG:
    cogP();
    break;
  case COG2:
    cog2P();
    break;
  default:
    cog2P();
    break;
  }

  switch ((int)srs.chargeMethod) {
  case AVERAGE:
    averageQ();
    break;
  case TOTAL:
    totalQ();
    break;
  default:
    totalQ();
    break;
  }
  
  return kfTRUE;
};


/// Methods for cluster time (position e-drift axis)

// Simple average
void CSRSAnalyzer::averageT(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumTimeFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumTimeFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumTimeFec[fecId] += d.correctedTime;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].timeCluster = sumTimeFec[fecId] / srs.fec[fecId].nHits;

      srs.fec[fecId].timeCluster = data[fecId].timeCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::averageT - fecId, vmmId, channel, adc, chargeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<d.adc<<" "<<data[fecId].timeCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "time";
  srs.SetDataAnalyzed(data, var);
}


// Adc-weighted average
void CSRSAnalyzer::cogT(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdcFec[MaxFECs];
  double sumWTimeFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdcFec[i] = 0;
    sumWTimeFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    int adc = d.adc;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdcFec[fecId] += adc;
      sumWTimeFec[fecId] += d.correctedTime*adc;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].timeCluster = sumWTimeFec[fecId] / sumAdcFec[fecId];

      srs.fec[fecId].timeCluster = data[fecId].timeCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::cogT - fecId, vmmId, channel, adc, chargeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<adc<<" "<<data[fecId].timeCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "time";
  srs.SetDataAnalyzed(data, var);
}


// Adc^2-weighted average
void CSRSAnalyzer::cog2T(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdc2Fec[MaxFECs];
  double sumW2TimeFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdc2Fec[i] = 0;
    sumW2TimeFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    double adc = d.adc;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdc2Fec[fecId] += pow(d.adc,2);
      sumW2TimeFec[fecId] += d.correctedTime*pow(d.adc,2);

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].timeCluster = sumW2TimeFec[fecId] / sumAdc2Fec[fecId];

      srs.fec[fecId].timeCluster = data[fecId].timeCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::cog2T - fecId, vmmId, channel, adc, chargeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<adc<<" "<<data[fecId].timeCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "time";
  srs.SetDataAnalyzed(data, var);
}


// First hit time
void CSRSAnalyzer::firstTime(){
  CSRS::dataAnalyzed data[MaxFECs];
  double firstTimeArrival[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    firstTimeArrival[i] = 9e9;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    double corrTime = d.correctedTime;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      if (corrTime < firstTimeArrival[fecId]){
        data[fecId].timeCluster = corrTime;
        firstTimeArrival[fecId] = corrTime;

        srs.fec[fecId].timeCluster = data[fecId].timeCluster;

        if (srs.debugMode > 0){
          std::cout<<"CSRSAnalyzer::firstTime - fecId, vmmId, channel, timeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<data[fecId].timeCluster<<std::endl;
        }
      }
    }
  }//for loop nHits
  std::string var = "time";
  srs.SetDataAnalyzed(data, var);
}


// Last hit time
void CSRSAnalyzer::lastTime(){
  CSRS::dataAnalyzed data[MaxFECs];
  double lastTimeArrival[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    lastTimeArrival[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    double corrTime = d.correctedTime;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      if (corrTime > lastTimeArrival[fecId]){
        data[fecId].timeCluster = corrTime;
        lastTimeArrival[fecId] = corrTime;

        srs.fec[fecId].timeCluster = data[fecId].timeCluster;

        if (srs.debugMode > 0){
          std::cout<<"CSRSAnalyzer::lastTime - fecId, vmmId, channel, timeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<data[fecId].timeCluster<<std::endl;
        }
      }
    }
  }//for loop nHits
  std::string var = "time";
  srs.SetDataAnalyzed(data, var);
}



/// Methods for cluster position (channel axis)

// Simple average 
void CSRSAnalyzer::averageP(){
  CSRS::dataAnalyzed data[MaxFECs];
  int sumPosFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumPosFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    int chNoMapped = d.chnoMapped;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumPosFec[fecId] += chNoMapped;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].posCluster = sumPosFec[fecId] / srs.fec[fecId].nHits;

      srs.fec[fecId].posCluster = data[fecId].posCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::averageP - fecId, vmmId, channel, adc, posCluster: "<<fecId<<" "<<vmmId<<" "<<chNoMapped<<" "<<d.adc<<" "<<data[fecId].posCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "position";
  srs.SetDataAnalyzed(data, var);
}


// Adc-weighted average
void CSRSAnalyzer::cogP(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdcFec[MaxFECs];
  double sumWPosFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdcFec[i] = 0;
    sumWPosFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    int chNoMapped = d.chnoMapped;
    double adc = d.adc;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdcFec[fecId] += adc;
      sumWPosFec[fecId] += chNoMapped*adc;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].posCluster = sumWPosFec[fecId] / sumAdcFec[fecId];

      srs.fec[fecId].posCluster = data[fecId].posCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::cogP - fecId, vmmId, channel, adc, posCluster: "<<fecId<<" "<<vmmId<<" "<<chNoMapped<<" "<<adc<<" "<<data[fecId].posCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "position";
  srs.SetDataAnalyzed(data, var);
}


// Adc^2-weighted average
void CSRSAnalyzer::cog2P(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdc2Fec[MaxFECs];
  double sumW2PosFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdc2Fec[i] = 0;
    sumW2PosFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    int chNoMapped = d.chnoMapped;
    double adc = d.adc;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdc2Fec[fecId] += pow(d.adc,2);
      sumW2PosFec[fecId] += chNoMapped*pow(d.adc,2);

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].posCluster = sumW2PosFec[fecId] / sumAdc2Fec[fecId];

      srs.fec[fecId].posCluster = data[fecId].posCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::cog2P - fecId, vmmId, channel, adc, posCluster: "<<fecId<<" "<<vmmId<<" "<<chNoMapped<<" "<<adc<<" "<<data[fecId].posCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "position";
  srs.SetDataAnalyzed(data, var);
}


/// Methods for cluster charge:

// Simple average
void CSRSAnalyzer::averageQ(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdcFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdcFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    int chNoMapped = d.chnoMapped;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdcFec[fecId] += d.adc;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].chargeCluster = sumAdcFec[fecId] / srs.fec[fecId].nHits;

      srs.fec[fecId].chargeCluster = data[fecId].chargeCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::averageQ - fecId, vmmId, channel, adc, chargeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<d.adc<<" "<<data[fecId].chargeCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "charge";
  srs.SetDataAnalyzed(data, var);
}


// Sum
void CSRSAnalyzer::totalQ(){
  CSRS::dataAnalyzed data[MaxFECs];
  double sumAdcFec[MaxFECs];
  for (int i=0; i<MaxFECs; i++){
    sumAdcFec[i] = 0;
  }

  for (int i = 0; i < srs.nHits; i++)
  {
    auto &d = srs.GetDataCal(i);
    int fecId = d.fecid;
    int vmmId = d.vmmid;
    double adc = d.adc;

    if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
    {
      sumAdcFec[fecId] += d.adc;

      //overwrite data[fecId] quantities, the last iteration gives set the good values.
      data[fecId].chargeCluster = sumAdcFec[fecId];

      srs.fec[fecId].chargeCluster = data[fecId].chargeCluster;

      if (srs.debugMode > 0){
        std::cout<<"CSRSAnalyzer::totalQ - fecId, vmmId, channel, adc, chargeCluster: "<<fecId<<" "<<vmmId<<" "<<(int)d.chnoMapped<<" "<<adc<<" "<<data[fecId].chargeCluster<<std::endl;
      }
    }
  }//for loop nHits
  std::string var = "charge";
  srs.SetDataAnalyzed(data, var);
}