#  SpecTclGUI save file created Tue Apr 09 17:48:34 EDT 2024
#  SpecTclGui Version: 1.0
#      Author: Ron Fox (fox@nscl.msu.edu)

#Tree params:


# Pseudo parameter definitions


# Tree variable definitions:

treevariable -set srs.debugMode 0 debug

# Gate definitions in reverse dependency order
 

# Spectrum Definitions

spectrum srs.fec2.adcAvg 1 srs.fec2.adcAvg {{0.000000 1024.000000 512}} long
spectrum srs.fec2.posAvg 1 srs.fec2.posAvg {{0.000000 512.000000 512}} long
spectrum srs.fec2.posAvg.adcAvg 2 {srs.fec2.posAvg srs.fec2.adcAvg} {{0.000000 512.000000 512} {0.000000 1024.000000 512}} long
spectrum srs.fec2.posAvg.tdcAvg 2 {srs.fec2.posAvg srs.fec2.tdcAvg} {{0.000000 512.000000 512} {0.000000 1024.000000 512}} long
spectrum srs.fec2.tdcAvg 1 srs.fec2.tdcAvg {{0.000000 1024.000000 512}} long

# Gate Applications: 


#  filter definitions: ALL FILTERS ARE DISABLED!!!!!!!


#  - Parameter tab layout: 

set parameter(select) 1
set parameter(Array)  false

#-- Variable tab layout

set variable(select) 1
set variable(Array)  0
