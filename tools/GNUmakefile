################################################################################
# GNUmakefile for the BaccSim tools
#
# Change log:
# 17 March 2010 - Initial submission (Melinda)
# 20 April 2010 - Added BaccExampleAnalysis (Melinda)
# 22 April 2010 - Added BaccRoot2Dat and a -gstabs for macosx to
#                 get rid of .dSYM bundles created on OS X (Michael Woods)
# 27 Sep 2010 - Added BaccExampleAnalysis and NMDAnalysis to the cleanup section
#		(Kareem)
# 14 Feb 2012 - Removed funny ^M newline characters, and added BaccAsciiReader
#		(Matthew)
# 15 Aug 2012 - Changed order of arguments and corrected LIBDIRS to match
#               needs of latest g++ (Rich)
# 11 May 2015 - Removed the -g flag from the compilation, as well as the -gstabs
#               flag from the linking (Kareem)
# 20 Jan 2016 - Added support for LUXRootConverter (Kareem) 
# 23 Feb 2016 - Removed the rootcint creation of the BaccRootConverter
#               dictionaries, and added them as separate files to avoid issues
#               with parallel make jobs (Kareem)
# 24 Feb 2016 - Fixed the makefile so that the shared library is statically
#               compiled (Kareem)
# 9 March 2016 - Rename cleanup to clean for consistency (Jingke)
# 16 Aug 2016 - Remove the BaccASCIIReader, BaccRootReader, NMDAnalysis,
#               and BaccExampleAnalysis (Kareem)
# 18 Aug 2016 - Restore BaccRootReader for some lingering validation (Kareem)
# 16 Dec 2016 - Update BaccPMTStream and BaccMCTruth to recompile BaccRootConverterEvent.cc
#               instead of linking to existing .so file. This is needed to avoid 
#               problem finding library at runtime. This is how it is done in LUXSim
#               since commit c41f1740c6d7b08436f819e6e287b3030169b544 
################################################################################

CXX          = g++
CCFLAGS		 += -std=c++17
INCLUDE		 = -I$(ROOTSYS)/include

ifeq ($(OSTYPE),darwin)
OSFLAGS	= -bind_at_load
endif

ifdef ROOTSYS
COMPILEJOBS	= BaccRootReader BaccRootConverter libBaccRootConverterEvent.so
RC     := root-config
ROOTLIBS	= $(shell $(RC) --libs --cflags)
LIBDIRS		= -L$(shell $(RC) --libdir)
INCLUDE		= -I$(shell $(RC) --incdir)
LDFLAGS		= $(shell $(RC) --ldflags)
PLATFORM	= $(shell $(RC) --platform)
OBJLIST		= BaccRootReader.o BaccRootConverter.o
ifeq ($(shell which $(RC) 2>&1 | sed -ne "s@.*/$(RC)@$(RC)@p"),$(RC))
MKARCH := $(wildcard $(shell $(RC) --etcdir)/Makefile.arch)
RCONFIG := $(wildcard $(shell $(RC) --incdir)/RConfigure.h)
endif
ifneq ($(MKARCH),)
include $(MKARCH)
else
ifeq ($(ROOTSYS),)
ROOTSYS = ..
endif
include $(ROOTSYS)/etc/Makefile.arch
endif
endif

ALLFLAGS	= $(CCFLAGS) $(LDFLAGS) $(OSFLAGS) $(INCLUDE)
ALLLIBS		= $(LIBDIRS) $(ROOTLIBS)

All:		$(COMPILEJOBS)

BaccRootReader:  BaccRootReader.cc
			@echo
			$(CXX) $(ALLFLAGS) $(ALLLIBS) BaccRootReader.cc -o BaccRootReader

BaccRootConverter:  BaccRootConverter.cc BaccRootConverterEvent.cc BaccRootConverterEvent.hh BaccRootConverterEvent_dict.cc BaccRootConverterEvent_LinkDef.h
			@echo
			$(CXX) $(ALLFLAGS) $(ALLLIBS) -I./ BaccRootConverterEvent.cc BaccRootConverterEvent_dict.cc BaccRootConverter.cc -o BaccRootConverter

libBaccRootConverterEvent.so: BaccRootConverterEvent.cc BaccRootConverterEvent.hh BaccRootConverterEvent_dict.cc BaccRootConverterEvent_LinkDef.h
			@echo
			$(LD) -std=c++17 $(SOFLAGS) $(LDFLAGS) $(INCLUDE) $(ALLLIBS) -fPIC BaccRootConverterEvent.cc BaccRootConverterEvent_dict.cc -o libBaccRootConverterEvent.so

#libBaccMCTruthEvent.so: BaccMCTruthEvent.cc BaccMCTruthEvent.hh BaccMCTruthEvent_dict.cc BaccMCTruthEvent_LinkDef.h
#			@echo
#			$(LD) -std=c++17 $(SOFLAGS) $(LDFLAGS) $(INCLUDE) $(ALLLIBS) -fPIC BaccMCTruthEvent.cc BaccMCTruthEvent_dict.cc -o libBaccMCTruthEvent.so
			
#BaccPMTStream:	        BaccPMTStream.cc BaccRootConverterEvent.hh BaccRootConverterEvent_dict.cc BaccRootConverterEvent_LinkDef.h
#			@echo
#			$(CXX) BaccPMTStream.cc BaccRootConverterEvent_dict.cc $(ALLFLAGS) $(ALLLIBS) -fPIC -o BaccPMTStream

#BaccMCTruth:	        BaccMCTruth.cc BaccRootConverterEvent.hh BaccRootConverterEvent_dict.cc BaccRootConverterEvent_LinkDef.h BaccMCTruth.cc BaccMCTruthEvent.hh BaccMCTruthEvent_dict.cc BaccMCTruthEvent_LinkDef.h
#			@echo
#			$(CXX) BaccMCTruthEvent.cc BaccMCTruthEvent_dict.cc BaccMCTruth.cc BaccRootConverterEvent_dict.cc $(ALLFLAGS) $(ALLLIBS) -o BaccMCTruth


.PHONY: BaccSim2evt
BaccSim2evt:		
			@cd BaccSim2evt && make #-C BaccSim2evt
			@#cd BaccSim2evt && make -B

neat:
		rm -rf *.o

clean::
		rm -rf *.o BaccRootConverter libBaccRootConverterEvent.so BaccPMTStream BaccSim2evt/BaccSim2evt BaccMCTruth

