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
#include <config.h>
#include "CSRS.h"
#include <EventProcessor.h>
#include <TCLAnalyzer.h>
#include <Event.h>
#include <math.h>
#include <Spectrum.h>
#include <Histogrammer.h>
#include <Globals.h>
#include <netinet/in.h>
#include <iostream>
#include <memory>

#include "CSRSUnpacker.h"
#include "Configuration.h"

#ifdef HAVE_STDNAMESPACE
using namespace std;
#endif

//---------------------------------------------------------//
// class CSRS
CSRS::CSRS(string name)
{
    m_name = name;
    // dataAna = new dataAnalyzed;
}

CSRS::~CSRS()
{
    // if (dataRaw != nullptr) {
    //   delete[] dataRaw;
    //   dataRaw = nullptr;
    // }
    // if (dataCal != nullptr) {
    //   delete[] dataCal;
    //   dataCal = nullptr;
    // }
    // if (dataAna != nullptr) {
    //   delete[] dataAna;
    //   dataAna = nullptr;
    // }
}

void CSRS::Initialize()
{
    runtime.Initialize(m_name + ".runtime", "sec");
    nHits.Initialize(m_name + ".nHits", 2000, 0, 2000, "");

    debugMode.Initialize(m_name + ".debugMode", 0, "debug");

    // Set to default, 9 is not a method so will go to switch default 
    timeMethod.Initialize(m_name + ".timeMethod", 9, "time calc. method");
    posMethod.Initialize(m_name + ".posMethod", 9, "position calc. method");
    chargeMethod.Initialize(m_name + ".chargeMethod", 9, "charge calc. method");

    for (int i = 0; i < MaxFECs; i++)
    {
        for (int j = 0; j < MaxVMMsData; j++)
        {
            if (config.pFecVmm[i][j] && !fecInitialized[i])
            {
                string fecIdStr = to_string(i);
                fec[i].Initialize(this, m_name + ".fec" + fecIdStr, i);
                fecInitialized[i] = true;
            }
        }
    }
}

void CSRS::Reset()
{
    runtime.Reset();
    nHits.Reset();

    for (int i = 0; i < MaxFECs; i++)
    {
        for (int j = 0; j < MaxVMMsData; j++)
        {
            if (config.pFecVmm[i][j])
            {
                fec[i].Reset();
                break;
            }
        }
    }
}

//---------------------------------------------------------//
// class CSRSFec
CSRSFec::~CSRSFec()
{
    // if (top != nullptr) {
    //   delete[] top;
    //   top = nullptr;
    // }
}

void CSRSFec::Initialize(CSRS *theTop, string name, int fecId)
{
    top = theTop;

    evt.Initialize(name + ".evt", "");
    nHits.Initialize(name + ".nHits", "");
    adc.Initialize(name + ".adc", 1024, 0.0, 1023.0, "channels", 512, 0);
    tdc.Initialize(name + ".tdc", 1024, 0.0, 1023.0, "channels", 512, 0);
    timeStamp.Initialize(name + ".timeStamp", "tick");
    timeStampDiff.Initialize(name + ".timeStampDiff", "tick");
    adcCal.Initialize(name + ".adcCal", 1024, 0.0, 1023.0, "channels", 512, 0);
    timeCal.Initialize(name + ".timeCal", 5000, 0.0, 50000.0, "channels", 512, 0);

    timeCluster.Initialize(name + ".timeCluster", "");
    posCluster.Initialize(name + ".posCluster", "");
    chargeCluster.Initialize(name + ".chargeCluster", "");


    // overThreshold = false;
    // if (top->debugMode)
    // {
    //     cou<<"debug..."<<endl;
    // }
}

void CSRSFec::Reset()
{
    evt.Reset();
    nHits.Reset();
    adc.Reset();
    tdc.Reset();
    timeStamp.Reset();
    timeStampDiff.Reset();
    adcCal.Reset();
    timeCal.Reset();
    timeCluster.Reset();
    posCluster.Reset();
    chargeCluster.Reset();

}

void CSRS::SetDataRaw(Gem::ParserSRS::VMM3Data *dataArray)
{
    int nbHitsFec[MaxFECs] = {0};

    for (int i = 0; i < srs.nHits; i++)
    {
        auto &d = dataArray[i];
        int fecId = d.fecid;
        int vmmId = d.vmmid;
        int chNo = d.chno;
        int chNoMapped = d.chnoMapped;
        /*!!!! changed chNoMapped here for test !!!!*/
        // int chNoMapped = d.chno;
        /*!!!! changed chNoMapped here for test !!!!*/


        // printf("SetDataRaw: fec: %d, vmm: %d, channel: %d, channelMapped: %d, overThreshold: %d, adc: %d, tdc: %d, fecTimeStamp: %llu, triggerOffset: %d  \n",d.fecid,d.vmmid, d.chno, chNoMapped, d.overThreshold, d.adc, d.tdc,d.fecTimeStamp,d.triggerOffset);
        if (vmmId < MaxVMMsData && config.pFecVmm[fecId][vmmId])
        {
            srs.fec[fecId].timeStamp = d.eventTimeStamp;
            srs.fec[fecId].adc[chNoMapped] = d.adc;
            srs.fec[fecId].tdc[chNoMapped] = d.tdc;
            nbHitsFec[fecId] += 1;
            srs.fec[fecId].nHits = nbHitsFec[fecId];
            if (timeStampPrev[fecId] >= 0){
                srs.fec[fecId].timeStampDiff = d.eventTimeStamp - timeStampPrev[fecId];
                // std::cout<<"Simon - TSdiff "<<srs.fec[fecId].timeStampDiff<<std::endl;
            }
            timeStampPrev[fecId] = d.eventTimeStamp;
        }
        else
        {
            printf("CSRS::SetDataRaw - fecId: %d or/and vmmId: %d not listed in the configuration file, the corresponding data will not be in the treegui \n", fecId, vmmId);
        }
    } // for hits
    dataRaw = dataArray;
}

void CSRS::SetDataCal(Gem::ParserSRS::VMM3DataCal *dataCalArray)
{
    dataCal = dataCalArray;
}

//Set tree parameters after analysis
void CSRS::SetDataAnalyzed(dataAnalyzed *data, std::string var = "all")
{

    // if (var == "all"){
    //     dataAna = data;
    // }
    // else if (var == "time"){
    //     dataAna->timeCluster = data->timeCluster;
    // }
    // else if (var == "position"){
    //     dataAna->posCluster = data->posCluster;
    // }
    // else if (var == "charge"){
    //     dataAna->chargeCluster = data->chargeCluster;
    // }
    // else {
    //     std::cout<<"CSRS::SetDataAnalyzed - var: "<<var<<" not recognized"<<std::endl;
    // }

}

