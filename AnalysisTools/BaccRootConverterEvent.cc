////////////////////////////////////////////////////////////////////////////////
/*	BaccRootConverterEvent.cc
*
* This file defines the BaccRootConverterEvent class used by the
* BaccRootConverter code.
*
********************************************************************************
* Change log
*	23 Feb 2016 - Initial submission (Kareem Kazkaz)
*       16 Dec 2016 - Constructor and ClearEverything removed and defined in
*                     BaccRootConverterEvent.hh as was causing undefined reference
*                     error when compiling BaccMCTruth as per LUXSim where the
*                     BaccRootConverterEvent_dict.cc library is compiled as 
*                     part of the gcc BaccMCTruth.cc command to avoid problems
*                     finding the BaccRootConverterEvent library at runtime. 
*                     This .cc file basically does nothing now but leaving in 
*                     place is minimal change. We should switch to cmake or 
*                     cleaner solution in long term, no reason to be forced to
*                     define methods in header file (Jim Dobson)
*                     
*/
////////////////////////////////////////////////////////////////////////////////

#include "BaccRootConverterEvent.hh"
//#include "BaccRootConverterEvent_dict.h"

using namespace std;

