INSTDIR = /usr/opt/spectcl/5.13-005
INCDIR = $(PWD)/include
SRCDIR = $(PWD)/src

include $(INSTDIR)/etc/SpecTcl_Makefile.include

#  If you have any switches that need to be added to the default c++ compilation
# rules, add them to the definition below:

USERCXXFLAGS = -std=c++11 -I$(INCDIR)
# Include path for nlohmann/json library
USERCXXFLAGS += -I/simon/SRS/json/include

#  If you have any switches you need to add to the default c compilation rules,
#  add them to the defintion below:

USERCCFLAGS = $(USERCXXFLAGS)

#  If you have any switches you need to add to the link add them below:
USERLDFLAGS =

#
#   Append your objects to the definitions below:
#

OBJECTS = $(addprefix $(SRCDIR)/, MySpecTclApp.o CSRSUnpacker.o CSRSCalibrator.o CSRS.o CSRSAnalyzer.o Configuration.o ParserSRS.o SRSTime.o Statistics.o CalibrationFile.o)
#
#  Finally the makefile targets.
#

# Move objects to src directory
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(USERCXXFLAGS) $(CXXFLAGS) -o $@ $<
	
SpecTcl: $(OBJECTS)
	$(CXXLD)  -o SpecTcl $(OBJECTS) $(USERLDFLAGS) \
			$(LDFLAGS)


clean:
	rm -f $(OBJECTS) SpecTcl

depend:
	makedepend -p$(SRCDIR)/ $(USERCXXFLAGS) $(addprefix $(SRCDIR)/, *.cpp *.c)
# makedepend $(USERCXXFLAGS) *.cpp *.c
	

help:
	echo "make                 - Build customized SpecTcl"
	echo "make clean           - Remove objects from previous builds"
	echo "make depend          - Add dependencies to the Makefile. "
