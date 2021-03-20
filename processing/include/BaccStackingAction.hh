////////////////////////////////////////////////////////////////////////////////
/*    BaccStackingAction.hh
*
* This is the header file for the BaccStackingAction class.
*
********************************************************************************
* Change log
*     06-Oct-2015 - Initial submission (David W)
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef LUXSIMSTACKINGACTION_H
#define LUXSIMSTACKINGACTION_H 1


//
//    C/C++ includes
//
#include "globals.hh"
#include "G4UserStackingAction.hh"
#include <vector>

//
//    Bacc includes
//
#include "BaccManager.hh"

//
//    Class forward declarations
//
class G4Track;

//------++++++------++++++------++++++------++++++------++++++------++++++------
class BaccStackingAction : public G4UserStackingAction {

        public:
                BaccStackingAction();
                virtual ~BaccStackingAction();

                virtual G4ClassificationOfNewTrack ClassifyNewTrack( const G4Track* aTrack );

        private:
                BaccManager *baccManager;
                double lastTime;
};
#endif

