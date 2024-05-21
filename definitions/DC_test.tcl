#  SpecTclGUI save file created Mon May 20 16:51:59 EDT 2024
#  SpecTclGui Version: 1.0
#      Author: Ron Fox (fox@nscl.msu.edu)

#Tree params:


# Pseudo parameter definitions


# Tree variable definitions:

treevariable -set srs.chargeMethod 9 {charge calc. method}
treevariable -set srs.debugMode 1 debug
treevariable -set srs.posMethod 9 {position calc. method}
treevariable -set srs.timeMethod 9 {time calc. method}

# Gate definitions in reverse dependency order
 
gate gnhits s {srs.nHits {3.715740 7.041290}}
gate postime c {srs.fec2.posCluster srs.fec2.timeCluster {{325.990051 29.547350} {324.641693 26.987370} {323.345642 22.720760} {323.125275 16.563431} {324.447479 10.807680} {328.928284 10.539970} {331.719604 13.618630} {331.572693 22.720760} {329.544830 27.787359}}}
gate all * {gnhits postime}
gate bing c {srs.fec2.posCluster srs.fec2.chargeCluster {{326.259827 879.987976} {326.300385 525.350525} {327.476898 505.648407} {327.314606 913.481506}}}
gate highCharge F {}
gate highNbHits s {srs.nHits {7.860384 17.735777}}
gate tc s {srs.fec2.timeCluster {12.000052 18.156984}}
gate tcAndnbHit * {gnhits tc}

# Spectrum Definitions

spectrum srs.fec2.chargeCluster 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
spectrum srs.fec2.chargeCluster_avg_g 1 srs.fec2.chargeCluster {{0.000000 500.000000 500}} long
spectrum srs.fec2.chargeCluster_bg 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
spectrum srs.fec2.chargeCluster_g 1 srs.fec2.chargeCluster {{0.000000 2000.000000 400}} long
spectrum srs.fec2.chargeCluster_gAll 1 srs.fec2.chargeCluster {{0.000000 2000.000000 400}} long
spectrum srs.fec2.chargeCluster_hnbHits 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
spectrum srs.fec2.chargeCluster_tc 1 srs.fec2.chargeCluster {{0.000000 2000.000000 200}} long
spectrum srs.fec2.posCluster 1 srs.fec2.posCluster {{1.000000 600.000000 600}} long
spectrum srs.fec2.posCluster_g 1 srs.fec2.posCluster {{1.000000 600.000000 600}} long
spectrum srs.fec2.poscharge 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{1.000000 500.000000 500} {1.000000 2000.000000 400}} long
spectrum srs.fec2.poscharge_g 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{0.000000 500.000000 501} {1.000000 2000.000000 400}} long
spectrum srs.fec2.poscharge_gAll 2 {srs.fec2.posCluster srs.fec2.chargeCluster} {{1.000000 500.000000 500} {1.000000 2000.000000 400}} long
spectrum srs.fec2.postimeCluster 2 {srs.fec2.posCluster srs.fec2.timeCluster} {{1.000000 600.000000 600} {-50.000000 100.000000 150}} long
spectrum srs.fec2.postimeCluster_g 2 {srs.fec2.posCluster srs.fec2.timeCluster} {{1.000000 600.000000 600} {-50.000000 100.000000 150}} long
spectrum srs.fec2.timeCluster 1 srs.fec2.timeCluster {{-50.000000 100.000000 150}} long
spectrum srs.fec2.timeCluster_g 1 srs.fec2.timeCluster {{-50.000000 100.000000 150}} long
spectrum srs.fec2.timecharge 2 {srs.fec2.timeCluster srs.fec2.chargeCluster} {{-50.000000 100.000000 150} {1.000000 2000.000000 400}} long
spectrum srs.fec2.timecharge_gAll 2 {srs.fec2.timeCluster srs.fec2.chargeCluster} {{-50.000000 100.000000 150} {1.000000 2000.000000 400}} long
spectrum srs.nHits 1 srs.nHits {{0.000000 50.000000 50}} long

# Gate Applications: 

apply gnhits  srs.fec2.chargeCluster_avg_g
apply bing  srs.fec2.chargeCluster_bg
apply gnhits  srs.fec2.chargeCluster_g
apply all  srs.fec2.chargeCluster_gAll
apply highNbHits  srs.fec2.chargeCluster_hnbHits
apply tcAndnbHit  srs.fec2.chargeCluster_tc
apply gnhits  srs.fec2.posCluster_g
apply gnhits  srs.fec2.poscharge_g
apply all  srs.fec2.poscharge_gAll
apply gnhits  srs.fec2.postimeCluster_g
apply gnhits  srs.fec2.timeCluster_g
apply all  srs.fec2.timecharge_gAll

#  filter definitions: ALL FILTERS ARE DISABLED!!!!!!!


#  - Parameter tab layout: 

set parameter(select) 1
set parameter(Array)  false

#-- Variable tab layout

set variable(Name1) srs.chargeMethod
set variable(Value1) 9
set variable(Unit1) {charge calc. method}
set variable(Name2) srs.posMethod
set variable(Value2) 9
set variable(Unit2) {position calc. method}
set variable(Name3) srs.timeMethod
set variable(Value3) 9
set variable(Unit3) {time calc. method}
set variable(Name4) srs.debugMode
set variable(Value4) 1
set variable(Unit4) debug
set variable(select) 4
set variable(Array)  0
