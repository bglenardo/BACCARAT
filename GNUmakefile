################################################################################
# GNUmakefile for the overall Bacc package
#
# Change log:
# 18 March 2009 - Initial submission (Kareem)
# 17 March 2010 - Added make commands for the tools directory (Kareem)
# 07 July 2015 - Imported in to BACCARAT from LUXSim. (Vic)
# 07 July 2015 - Edited to include the PROJECTDIRS definition and processing
#		 (Kareem)
# 14 January 2016 - Edited to parse ProjectDirs automatically, and to generate
#                   source codes needed to call these projects. (Jingke)
# 9 March 2016 - Include "tools" in SUBDIRS so it gets called automatically
#		 but excluding it from extra libs (Jingke)
# 18 March 2016 - Enable users to easily make new BACCARAT project (Jingke)
#
################################################################################

#directories that have a makefile and will be called automatically
#warning: tools doesn't build a library and can not be linked against
SUBDIRS = generator geometry io management physicslist processing tools
#directories that don't have makefiles or that will run make separately
BACCDIRS = BACCARAT BaccConfig BaccMacros Images BaccLibraries
G4LIB_BUILD_SHARED =
#the project dirs have to have a GNUmakefile, only 1 makefile per top level folder
PROJECTDIRS := $(shell find . -name GNUmakefile -type f | grep -v "\./GNUmakefile" \
                | sed 's/.\///' | sed 's/\/GNUmakefile//' | grep -v "\/" )
#PROJECTDIRS := $(shell find . -maxdepth 1 -type d -printf "%f\n"  | grep -v "\.")
PROJECTDIRS := $(filter-out $(SUBDIRS),$(PROJECTDIRS))
PROJECTDIRS := $(filter-out $(BACCDIRS),$(PROJECTDIRS))
ALLDIRS = $(SUBDIRS) $(PROJECTDIRS)

.PHONY: makeall clean project

makeall:
	@echo "********************************************************************************"
	@echo " Building BACCARAT"
	@echo "********************************************************************************"
	@for dir in $(filter-out geometry, $(ALLDIRS)); do (echo; echo; echo Building $$dir...;\
	cd $$dir; $(MAKE)); done
	@echo; echo; echo Building geometry...; cd geometry; $(MAKE) setup; $(MAKE); cd ..
	@echo
	@echo
	@echo Building BACCARAT...
	@echo
	@echo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@cd BACCARAT; $(MAKE) "SUBDIRS=$(filter-out tools,$(ALLDIRS))"
	@cd ..
	@rm -f BACCARATExecutable
	@ln -s BACCARAT/bin/$(G4SYSTEM)/BACCARAT ./BACCARATExecutable

clean:
	@for dir in $(ALLDIRS); do (echo; echo; echo Cleaning $$dir...; cd $$dir; $(MAKE) clean); done
	@echo
	@echo
	@echo Cleaning Bacc...
	rm -rf BACCARAT/bin BACCARAT/tmp
	@echo Cleaning libraries and executable...
	rm -rf BaccLibraries
	rm -f BACCARATExecutable
	@echo Removing extraneous, system-dependent files
	@rm -rf $(shell find . -name .DS_Store)
	@rm -rf $(shell find . -name ._\*)	
	@rm -rf $(shell find . -name g4_\*.wrl)

archive: clean
	@cd ..; tar zcvf Bacc.tgz Bacc

#Enable users to easily make new BACCARAT project
include ./BaccConfig/BuildProject.gmk
