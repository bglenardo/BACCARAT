////////////////////////////////////////////////////////////////////////////////
/*	BaccSteppingAction.cc

This is the code file to control the simulation step-by-step.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/01/03 - Code cleanup (Vic)
  2014/09/08 - Fixed bug in the timing of decay particles from 
               radioisotopes (Brian)
  2014/09/09 - Added include fo G4SystemOfUnits.hh for geant4.10 compatibility
               (Brian)
  2017/02/21 - Record level 5 added (TD and AC)
  2017/02/22 - Store creation positions of thermal electrons as fetched from storage in BaccManager as positions in step 0s (AC and TD)
*/
////////////////////////////////////////////////////////////////////////////////

//
//	GEANT4 includes
//
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

//
//	Bacc includes
//
#include "BaccSteppingAction.hh"
#include "BaccManager.hh"
#include "BaccDetectorComponent.hh"
#include "BaccMaterials.hh"
#include "BaccEventAction.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"

//
//	Definitions
//
#define DEBUGGING 0

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				BaccSteppingAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccSteppingAction::BaccSteppingAction()
{
	baccManager = BaccManager::GetManager();
	baccManager->Register( this );
	
	BaccMaterials *baccMaterials = BaccMaterials::GetMaterials();
	blackiumMat = baccMaterials->Blackium();

	optPhotRecordLevel = 0;
	thermElecRecordLevel = 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				~BaccSteppingAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccSteppingAction::~BaccSteppingAction() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//				UserSteppingAction()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccSteppingAction::UserSteppingAction( const G4Step* theStep )
{
	//	Initialize step specifics
	theTrack = theStep->GetTrack();
	trackPosition = theStep->GetPostStepPoint()->GetPosition();
	particleDirection = theStep->GetPreStepPoint()->GetMomentumDirection();
	recordLevel = baccManager->GetComponentRecordLevel(
			(BaccDetectorComponent*)theTrack->GetVolume() );
	optPhotRecordLevel = baccManager->GetComponentRecordLevelOptPhot(
			(BaccDetectorComponent*)theTrack->GetVolume() );
	thermElecRecordLevel = baccManager->GetComponentRecordLevelThermElec(
                        (BaccDetectorComponent*)theTrack->GetVolume() );
	
	//	Record relevant parameters in the step record
	aStepRecord.stepNumber = theTrack->GetCurrentStepNumber();
	aStepRecord.particleID = theTrack->GetDefinition()->GetPDGEncoding();
	aStepRecord.particleName = theTrack->GetDefinition()->GetParticleName();
	if( theTrack->GetCreatorProcess() )
		aStepRecord.creatorProcess =
				theTrack->GetCreatorProcess()->GetProcessName();
	else 
		aStepRecord.creatorProcess = "primary";
	aStepRecord.stepProcess = theStep->GetPostStepPoint()->
		GetProcessDefinedStep()->GetProcessName();

	aStepRecord.trackID = theTrack->GetTrackID();
	aStepRecord.parentID = theTrack->GetParentID();
	aStepRecord.particleEnergy =
			theStep->GetPreStepPoint()->GetKineticEnergy()/keV;
	aStepRecord.particleDirection[0] = particleDirection.x();
	aStepRecord.particleDirection[1] = particleDirection.y();
	aStepRecord.particleDirection[2] = particleDirection.z();
	aStepRecord.energyDeposition = theStep->GetTotalEnergyDeposit()/keV;
	aStepRecord.position[0] = trackPosition.x()/mm;
	aStepRecord.position[1] = trackPosition.y()/mm;
	aStepRecord.position[2] = trackPosition.z()/mm;
        aStepRecord.charge = theTrack->GetDynamicParticle()->GetCharge();
	
	//	Record whether or not the primary particle is a radioactive ion
	if( (aStepRecord.parentID==0) && (aStepRecord.stepNumber==1) &&
			!theTrack->GetDefinition()->GetPDGStable() &&
	                        (aStepRecord.particleID > 1e9) )
		baccManager->GetEvent()->SetRadioactivePrimaryTime(
				baccManager->GetPrimaryParticles()[0].time );
	
	//	Check to see if the particle is an immediate daughter of a primary
	//	radioactive nucleus, and if so, wipe out the global time. This has the
	//	effect of having the primary particle decaying at global time = 0.
	if( !baccManager->GetG4DecayBool() && (aStepRecord.parentID==1) &&
            (aStepRecord.stepNumber==1) &&
            baccManager->GetEvent()->GetRadioactivePrimaryTime() )
	  theTrack->SetGlobalTime(
				  baccManager->GetEvent()->GetRadioactivePrimaryTime() );
	
	aStepRecord.stepTime = theTrack->GetGlobalTime()/ns;


	//      When using the G4Decay generator, global time is reset for all
	//      daughters of a decay. This has the effect of having all isotopes
	//      decay at 0s with a decay time of 1s. Note that hits from each isotope
	//      decay within the chain are grouped into seperate output events.
      if( baccManager->GetG4DecayBool() ){
      
	  radIsoMap = baccManager->GetRadioIsotopeMap();
	  
	  // reset for first step in event
	  if(theTrack->GetTrackID() == 1){	
	    for(itMap=radIsoMap.begin(); itMap!=radIsoMap.end(); ++itMap)
	      itMap->second = false;
	    baccManager->ClearSavePrimaryParticle();
	  }
	  
	  if( aStepRecord.particleID > 1e9 ){	   

	    
	    for(itMap=radIsoMap.begin(); itMap!=radIsoMap.end(); ++itMap){
	      if((itMap->first==aStepRecord.particleID) && (itMap->second == false)){
		
		if(theTrack->GetCurrentStepNumber()==1 && theTrack->GetTrackID() != 1){		  

		  //    Save this isotope as a "primary particle"
		  primaryParticles.id = theTrack->GetDefinition()->GetParticleName();
		  primaryParticles.energy = theStep->GetPreStepPoint()->GetKineticEnergy()/keV;
		  primaryParticles.time = theTrack->GetGlobalTime()/ns;
		  primaryParticles.position = G4ThreeVector(trackPosition.x()/mm,trackPosition.y()/mm,trackPosition.z()/mm);
		  primaryParticles.direction = G4ThreeVector(particleDirection.x(), particleDirection.y(), particleDirection.z());
		  baccManager->SavePrimaryParticle( primaryParticles );		 

		  //    This isotope has been considered, set flag to true
		  itMap->second = true;
		  
		  //    Set a flag to write the output for next track with this parentID
		  baccManager->SetG4DecayOutputBool(true);
		  baccManager->SetIsotopeTrackID(theTrack->GetTrackID());
		}
		
		//    This next line calls stacking manager
		G4EventManager::GetEventManager()->GetStackManager()->ReClassify();
		
		//    This isotope has been considered, set flag to true
		itMap->second = true; 
		
	      }
	    }	      
	  }
	 
	  //    First instance of a daughter from a radioisotope, 
	  //    write all accumulated tracks, start new event
	  if(aStepRecord.parentID==baccManager->GetIsotopeTrackID() && 
             baccManager->GetG4DecayOutputBool() ){
	       baccManager->IterateEventCount();
               baccManager->RecordValues( baccManager->GetEventCount() );
               baccManager->ClearRecords();
               baccManager->SetG4DecayOutputBool(false);
               baccManager->CopyPrimaryParticle();
	  }

	  baccManager->UpdateRadioIsotopeMap(radIsoMap);
      }
    
      //Record step 0, which includes position of particle's creation.
      if (aStepRecord.stepNumber == 1){
        if( (aStepRecord.particleName != "opticalphoton" && aStepRecord.particleName != "thermalelectron") || 
              (aStepRecord.particleName == "opticalphoton" && optPhotRecordLevel) || 
              (aStepRecord.particleName == "thermalelectron" && thermElecRecordLevel) ){
	    //Storage Variables
	    //Note: KE and direction set from pre-step point so no buffer needed
	    double xPositionBuffer = aStepRecord.position[0];
	    double yPositionBuffer = aStepRecord.position[1];
	    double zPositionBuffer = aStepRecord.position[2];
	    double energyDepositonBuffer = aStepRecord.energyDeposition;
	    double timeBuffer = aStepRecord.stepTime;
	    G4String stepProcessBuffer = aStepRecord.stepProcess;
	    aStepRecord.stepTime = theStep->GetPreStepPoint()->GetGlobalTime()/ns;

	    //Thermal electron tracks should be in reverse order to the order their creation positions were saved.
	    //(Used for S2 vertex association - see also G4S1Light and BaccManager)
	    if(aStepRecord.particleName == "thermalelectron" && aStepRecord.creatorProcess == "S1"){ 
	       G4int i = baccManager->GetCurrentS2Index(); //get last thermal electron index
	       if(i==0) i = baccManager->GetS2PositionsSize(); //initialise thermal electron index
	       --i;

	       bool clusterFound = false;
	       if(std::abs(baccManager->GetCreationTime(i)-aStepRecord.stepTime)<1e-10) clusterFound = true;
	       else{
                    i = baccManager->GetS2PositionsSize()-1; //Send back to the start as another set of interactions has been added in G4S1Light
		    if(std::abs(baccManager->GetCreationTime(i)-aStepRecord.stepTime)<1e-10) clusterFound = true;
	       }

	       if(clusterFound){
                  G4ThreeVector clusterPosition = baccManager->GetS2Position(i);
		  aStepRecord.position[0] = clusterPosition[0]/mm;
		  aStepRecord.position[1] = clusterPosition[1]/mm;
		  aStepRecord.position[2] = clusterPosition[2]/mm;
		  baccManager->SetCurrentS2Index(i); //remember current counter position for next thermal electron track
	       }
	       else{
		 G4cout << "::ERROR:: BaccSteppingAction. S2 time not matched, vertex position not found. Index: " << i << G4endl;
	       }
	    }
	    else{
	      trackPosition = theStep->GetPreStepPoint()->GetPosition();
	      aStepRecord.position[0] = trackPosition.x()/mm;
	      aStepRecord.position[1] = trackPosition.y()/mm;
	      aStepRecord.position[2] = trackPosition.z()/mm;
	    }

	    aStepRecord.energyDeposition = 0;
	    aStepRecord.stepProcess = "Creation";
	    aStepRecord.stepNumber = 0;

	    baccManager->AddDeposition(
				       (BaccDetectorComponent*)theTrack->GetVolume(),
				       aStepRecord );
 
	    aStepRecord.position[0] = xPositionBuffer;
	    aStepRecord.position[1] = yPositionBuffer;
	    aStepRecord.position[2] = zPositionBuffer;
	    aStepRecord.energyDeposition = energyDepositonBuffer;
	    aStepRecord.stepTime = timeBuffer;
	    aStepRecord.stepProcess = stepProcessBuffer;
	    aStepRecord.stepNumber = 1;
	  }
	}
	
	//	Handle the case of optical photon record keeping
	if( aStepRecord.particleName == "opticalphoton" ) {
	
		aStepRecord.energyDeposition = 0;
	
		if( optPhotRecordLevel )
			baccManager->AddDeposition(
					(BaccDetectorComponent*)theTrack->GetVolume(),
					aStepRecord );
		
		if( optPhotRecordLevel == 1 || optPhotRecordLevel == 3 )
			theTrack->SetTrackStatus( fStopAndKill );

	} else if ( aStepRecord.particleName == "thermalelectron" ){

                aStepRecord.energyDeposition = 0;

                if( thermElecRecordLevel )
                        baccManager->AddDeposition(
                                        (BaccDetectorComponent*)theTrack->GetVolume(),
                                        aStepRecord );

                if( thermElecRecordLevel == 1 || thermElecRecordLevel == 3 )
                        theTrack->SetTrackStatus( fStopAndKill );

	} else
		baccManager->AddDeposition(
				(BaccDetectorComponent*)theTrack->GetVolume(),
				aStepRecord );
	
	//	Kill the particle if the current volume is made of blackium, or if the
	//	record level is set to 4. The blackium support is kept for historical
	//	reasons. Note that the particle is killed only after the first step is
	//	recorded, so that we at least know what went into the volume and where.
	if( (theTrack->GetMaterial() == blackiumMat) || (recordLevel == 4) )
		theTrack->SetTrackStatus( fStopAndKill );
	
	//	Put debugging code here
	if( DEBUGGING ) {
		G4cout << "\tTracking a " << aStepRecord.particleEnergy << "-keV "
			   << aStepRecord.particleName << " in "
			   << theTrack->GetVolume()->GetName() << " at ( "
			   << aStepRecord.position[0] << ", " << aStepRecord.position[1]
			   << ", " << aStepRecord.position[2] << " )" << G4endl;

        G4cout << "\tTrack " << aStepRecord.trackID << ", Step "
               << aStepRecord.stepNumber << ", process: "
               << aStepRecord.stepProcess << ", created by "
               << aStepRecord.creatorProcess << G4endl;

//		if( aStepRecord.particleName == "gamma" ||
//				aStepRecord.particleName == "alpha" ||
//				aStepRecord.particleName == "e-" ||
//				aStepRecord.particleName == "anti_nu_e" ||
//				aStepRecord.particleName == "neutron"
//				)
//			theTrack->SetTrackStatus( fStopAndKill );
	}
}
