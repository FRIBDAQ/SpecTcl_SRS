/*
    This software is Copyright by the Board of Trustees of Michigan
    State University (c) Copyright 2026.

    You may use this software under the terms of the GNU public license
    (GPL).  The terms of this license are described at:

     http://www.gnu.org/licenses/gpl.txt

    Author:
             Genie Jhang
       FRIB
       Michigan State University
       East Lansing, MI 48824-1321
*/

#include "CVME.h"

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

//---------------------------------------------------------//
// class CVME
CVME::CVME(string name)
{
  m_name = name;
}

CVME::~CVME()
{
}

void CVME::Initialize()
{
  adc.Initialize(m_name + ".adc", 4096, 0.0, 4096.0, "channels", 32, 0);
}

void CVME::Reset()
{
  adc.Reset();
}
