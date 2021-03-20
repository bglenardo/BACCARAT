################################################################################
# GNUmakefile for the Bacc2evt
#
# Change log:
# ## Month 2010 - Initial submission (Michael Woods)
################################################################################

CC			 = g++
CCFLAGS		 = -g -O3 -Wall

COMPILEJOBS	= Bacc2evt

SOURCES     = Bacc2evt.cc Bacc2evtMethods.cc Bacc2evtPulse.cc Bacc2evtTrigger.cc Bacc2evtReader.cc XMLtoVector.cc
HEADERS     = Bacc2evt.hh Bacc2evtMethods.hh Bacc2evtPulse.hh Bacc2evtTrigger.hh Bacc2evtReader.hh XMLtoVector.hh

ifeq ($(shell uname), Darwin)
OSFLAGS 	+=  -gstabs
endif

ALLFLAGS	= $(CCFLAGS) $(LDFLAGS) $(OSFLAGS) $(INCLUDE)
ALLLIBS		= $(LIBDIRS)

All:		$(COMPILEJOBS)

Bacc2evt: 		$(SOURCES) $(HEADERS)
			$(CC) $(ALLFLAGS) $(ALLLIBS) $(SOURCES) -o Bacc2evt

debug: 		$(SOURCES) $(HEADERS)
			$(CC) -save-temps $(ALLFLAGS) $(ALLLIBS) $(SOURCES) -O0 -o Bacc2evt

neat:
		rm -rf *.o

clean:
		rm -rf *.o Bacc2evt

run:
		@#./Bacc2evt BaccNest/Nest_Bacc2evt/LUXOut50.bin
		./Bacc2evt S1S2_ForPaper1110482182.bin
