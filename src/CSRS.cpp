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
}

CSRS::~CSRS()
{
    delete[] dataRaw;
    dataRaw = nullptr;
    delete[] dataCal;
    dataCal = nullptr;
}

void CSRS::Initialize()
{
    runtime.Initialize(m_name + ".runtime", "sec");
    eventlength.Initialize(m_name + ".eventlength", 8193, 0, 8192, "bytes");
    nHits.Initialize(m_name + ".nHits", 2000, 0, 2000, "");
    debugMode.Initialize(m_name + ".debugMode", 0, "debug");

    for (int i = 0; i < MaxFECs; i++)
    {
        string fecIdStr = to_string(i);
        fec[i].Initialize(this, m_name + ".fec" + fecIdStr, i);
    }
}

void CSRS::Reset()
{
    runtime.Reset();
    eventlength.Reset();
    nHits.Reset();

    for (int i = 0; i < MaxFECs; i++)
    {
        fec[i].Reset();
    }
}

//---------------------------------------------------------//
// class CSRSFec
CSRSFec::~CSRSFec()
{
    delete top;
    top = nullptr;
}

void CSRSFec::Initialize(CSRS *theTop, string name, int fecId)
{
    top = theTop;

    for (int i = 0; i < MaxVMMs; i++)
    {
        string vmmIdStr = to_string(i);
        vmm[i].Initialize(top, name + ".vmm" + vmmIdStr, fecId, i);
    }

    overThreshold = false;
    // if (top->debugMode)
    // {
    //     cou<<"debug..."<<endl;
    // }
}

void CSRSFec::Reset()
{
    for (int i = 0; i < MaxVMMs; i++)
    {
        vmm[i].Reset();
    }
}

//---------------------------------------------------------//
// class CSRSVmm
CSRSVmm::~CSRSVmm()
{
    delete top;
    top = nullptr;
}

void CSRSVmm::Initialize(CSRS *theTop, string name, int fecId, int vmmId)
{
    top = theTop;
    if (config.pFecVmm[fecId][vmmId])
    {
        cout << "Name - " << name << endl;
        adc.Initialize(name + ".adc", 1024, 0.0, 1023.0, "channels", 64, 0);
        adcCal.Initialize(name + ".adcCal", 1024, 0.0, 1023.0, "channels", 64, 0);
        tdc.Initialize(name + ".tdc", 256, 0.0, 255.0, "channels", 64, 0);
        correctedTime.Initialize(name + ".correctedTime", 10000, 0.0, 1000000.0, "a.u.", 64, 0);
    }
    // if (top->debugMode)
    // {
    //     cou<<"debug..."<<endl;
    // }
}

void CSRSVmm::Reset()
{
    adc.Reset();
    adcCal.Reset();
    tdc.Reset();
    correctedTime.Reset();
    hasdata = kfFALSE;
}

void CSRS::SetDataRaw(Gem::ParserSRS::VMM3Data *dataArray)
{
    for (int i = 0; i < srs.nHits; i++)
    {
        auto &d = dataArray[i];
        int fecId = dataParser.fecId;
        int vmmId = d.vmmid;
        int chNo = d.chno;

        // printf("SRS Data: fec: %d, vmm: %d, channel: %d, overThreshold: %d, adc: %d, tdc: %d, fecTimeStamp: %d, triggerOffset: %d  \n",dataParser.fecId,d.vmmid, d.chno, d.overThreshold, d.adc, d.tdc,d.fecTimeStamp,d.triggerOffset);
        if (config.pFecVmm[fecId][vmmId])
        {
            srs.fec[fecId].vmm[vmmId].adc[chNo] = d.adc;
            srs.fec[fecId].vmm[vmmId].tdc[chNo] = d.tdc;
            srs.fec[fecId].vmm[vmmId].hasdata = true;
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
    for (int i = 0; i < srs.nHits; i++)
    {
        auto &d = dataCalArray[i];
        int fecId = dataParser.fecId;
        int vmmId = d.vmmid;
        int chNo = d.chno;

        // printf("SRS Data Cal: fec: %d, vmm: %d, channel: %d, overThreshold: %d, adc cal.: %d, tdc: %d, timeStamp: %d, correctedTime: %d  \n",dataParser.fecId,d.vmmid, d.chno, d.overThreshold, d.adc, d.tdc,d.timeStamp,d.correctedTime);
        if (config.pFecVmm[fecId][vmmId])
        {
            srs.fec[fecId].vmm[vmmId].adcCal[chNo] = d.adc;
            srs.fec[fecId].vmm[vmmId].correctedTime[chNo] = d.correctedTime;
        }
        // else {
        // warning already printed for raw data
        // printf("CSRS::SetDataCal - fecId: %d or/and vmmId: %d not listed in the configuration file, the corresponding data will not be in the treegui \n",fecId,vmmId);
        // }

    } // for hits
    dataCal = dataCalArray;
}
