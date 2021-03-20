////////////////////////////////////////////////////////////////////////////////
/*	BaccDetectorComponent.cc

This is the code file to define the detector component class. The detector 
components inherit from G4VPhysicalVolumes, but they contain additional 
information and methods related to event generation and output recording.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)

  2014/10/05 - Added Kareem's methods for calculating the volume and mass in a more
               robust way than is available in standard Geant4 code. Also,
               commented out the "CheckOverlaps" call in the Initialize()
               method so the geometry loads more quickly. (Brian)

  2014/10/05 - Small fix to the volume calculation's screen output (Brian)
  2014/10/05 - Added check for volume overlaps, rather than performing overlap
               check by default (Brian) from Bacc
  2015/03/03 - Fixed bug where GetVolume calculation was going negative.
  2015/03/03 - Added component-wise SetMass and SetVolume commands.

*/
////////////////////////////////////////////////////////////////////////////////

//
//	CLHEP includes
//
#include "Randomize.hh"

//
//	GEANT4 includes
//
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4RotationMatrix.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"
#include "G4Material.hh"

//
//	Bacc includes
//
#include "BaccDetectorComponent.hh"
#include "BaccManager.hh"
#include "BaccMaterials.hh"
#include "BaccSource.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					BaccDetectorComponent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccDetectorComponent::BaccDetectorComponent(
		G4RotationMatrix *pRot,
		const G4ThreeVector &tlate,
		G4LogicalVolume *pCurrentLogical,
		const G4String &pName,
		G4LogicalVolume *pMotherLogical,
		G4bool pMany,
		G4int pCopyNo,
		G4bool pSurfChk )
: G4PVPlacement( pRot, tlate, pCurrentLogical, pName, pMotherLogical, pMany,
pCopyNo, pSurfChk )
{
	Initialize();
}

BaccDetectorComponent::BaccDetectorComponent(
		const G4Transform3D &Transform3D,
		G4LogicalVolume *pCurrentLogical,
		const G4String &pName,
		G4LogicalVolume *pMotherLogical,
		G4bool pMany,
		G4int pCopyNo,
		G4bool pSurfChk )
: G4PVPlacement( Transform3D, pCurrentLogical, pName, pMotherLogical, pMany,
pCopyNo, pSurfChk )
{
	Initialize();
}

BaccDetectorComponent::BaccDetectorComponent(
		G4RotationMatrix *pRot,
		const G4ThreeVector &tlate,
		const G4String &pName,
		G4LogicalVolume *pLogical,
		G4VPhysicalVolume *pMother,
		G4bool pMany,
		G4int pCopyNo,
		G4bool pSurfChk )
: G4PVPlacement( pRot, tlate, pName, pLogical, pMother, pMany, pCopyNo,
pSurfChk )
{
	Initialize();
}
		
BaccDetectorComponent::BaccDetectorComponent(
		const G4Transform3D &Transform3D,
		const G4String &pName,
		G4LogicalVolume *pLogical,
		G4VPhysicalVolume *pMother,
		G4bool pMany,
		G4int pCopyNo,
		G4bool pSurfChk )
: G4PVPlacement( Transform3D, pName, pLogical, pMother, pMany, pCopyNo,
pSurfChk )
{
	Initialize();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					~BaccDetectorComponent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccDetectorComponent::~BaccDetectorComponent()
{
	if(BaccManager::GetManager()) BaccManager::GetManager()->Deregister( this );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					Initialize()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::Initialize()
{
	baccManager = BaccManager::GetManager();
    baccMaterials = BaccMaterials::GetMaterials();
	baccManager->Register( this );
	
	navigator = G4TransportationManager::GetTransportationManager()->
			GetNavigatorForTracking();

	eventRecord.clear();
	sources.clear();
	capturePhotons = false;
	
	globalCenter = G4ThreeVector(0,0,0);
    minXYZ = G4ThreeVector(0,0,0);
    maxXYZ = G4ThreeVector(0,0,0);
    maxX = minX = maxY = minY = maxZ = minZ = 0;

        if( baccManager->GetCheckVolumeOverlaps() )
           CheckOverlaps(1000,0.,false);
	
	recordLevel = 0;
	recordLevelOptPhot = 0;
	recordLevelThermElec = 0;

        componentVolume = componentMass = -1;
        volumePrecision = 100000000;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					AddSource()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::AddSource( BaccSource *type, G4double act,
        G4int particleMass, G4int particleNumber, G4String parentIso, G4double sourceAge,
	G4String parName, G4double parEnergy, G4bool pointSource, G4ThreeVector posSource)
//, G4double wimpMass )
{
	source temp;
	temp.type = type;
	temp.activity = act;
	temp.ratio = 0;
    temp.particleMass = particleMass;
    temp.particleNumber = particleNumber;
    temp.parentIsotope = parentIso;
    temp.sourceAge = sourceAge;
    temp.particleName = parName;
    temp.particleEnergy = parEnergy;
    temp.pointSource = pointSource;
    temp.posSource = posSource;
//	temp.wimpMass = wimpMass;
	sources.push_back( temp );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CalculateRatios()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::CalculateRatios()
{
	totalActivity = 0.;
	for( G4int i=0; i<(G4int)sources.size(); i++ ) {
		sources[i].ratio =
				sources[i].activity*sources[i].type->GetActivityMultiplier();
		totalActivity += sources[i].ratio;
	}
	for( G4int i=0; i<(G4int)sources.size(); i++ )
		sources[i].ratio = sources[i].ratio/totalActivity;
	
	if( totalActivity ) {
		G4cout << "For volume " << ((BaccDetectorComponent*)(this))->GetName()
			   << ", sources are as follows:" << G4endl;
		for( G4int i=0; i<(G4int)sources.size(); i++ ) {
			G4cout << "\t" << sources[i].type->GetName() << " ";
			if( sources[i].type->GetName() == "SingleDecay" ||
				sources[i].type->GetName() == "G4Decay"  )
				G4cout << "Z,A = " << sources[i].particleMass << ","<<sources[i].particleNumber;
            else if( sources[i].type->GetName() == "SingleParticle" )
                G4cout << sources[i].particleName;
            else if( sources[i].type->GetName() == "DecayChain" )
                G4cout << sources[i].parentIsotope ;
			G4cout << "\t\t" << sources[i].activity << "\t" << sources[i].ratio
				   << G4endl;

            if( sources[i].type->GetName().find("DecayChain")<G4String::npos )
                G4cout << "\t\tSource age = " << sources[i].sourceAge << " s" 
                       << G4endl;
		}
		G4cout << "\tTotal activity for this source (taking into account "
			   << "activity within" << G4endl
			   << "\tdecay chains) = " << totalActivity << " Bq" << G4endl;
	}
	
	for( G4int i=1; i<(G4int)sources.size(); i++ )
		sources[i].ratio += sources[i-1].ratio;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::GenerateFromEventList( G4GeneralParticleSource
        *particleGun, G4Event *event, decayNode *firstNode )
{
    //time sent and received in *ns
    sources[firstNode->sourcesID].type->GenerateFromEventList( particleGun, 
            event, firstNode);
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//				    	GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::GenerateEventList(G4int sourceByVolumeID)
{
    G4ThreeVector eventPosition;
    // Acitivty units in Bq, time units in seconds
    G4double startParticleTime;
    // Generate numOfEvents for all sources
    G4int numOfEvents = baccManager->GetNumEvents();
    G4double windowEndTime = baccManager->GetWindowEndTime();
    if( windowEndTime > 0 ) windowEndTime*=1.e9*ns;//convert s->ns

    //source activity and activity multiplier
    G4double sourceActivity = 0.;

    for( G4int i=0; i<(G4int)sources.size(); i++ ) {
        sourceActivity = sources[i].activity 
                           * sources[i].type->GetActivityMultiplier();

        // Reset time for each source of each different source
        startParticleTime = ( 0. )*ns ; 
        G4cout << "Adding source " << sources[i].type->GetName() 
		       << " in "<< ((BaccDetectorComponent*)(this))->GetName()
               << " to binary search tree (BST)." << G4endl;
        G4cout << "  Progress: " << i+1 << " of " << sources.size() 
               << " in this volume " << G4endl;

        if( sources[i].type->GetName().find("DecayChain") < G4String::npos ) {
            // Time is determined by DecayChain GenerateEventList method
            sources[i].type->CalculatePopulationsInEventList( 
                            sources[i].sourceAge, sources[i].activity, 
                            sources[i].parentIsotope );
            // Each DecayChain needs to generate events with the specific
            // Population just calculated before moving to the next
            G4cout << "Adding DecayChain to BST" << G4endl;
            for( G4int j=0; j<numOfEvents; j++ ) {
	        if( sources[i].pointSource ) eventPosition = sources[i].posSource;
	        else eventPosition = GetEventLocation();
                sources[i].type->GenerateEventList( eventPosition, 
                              sourceByVolumeID, i, sources[i].parentIsotope );
                if( sources[i].type->GetParentDecayTime() > windowEndTime/s )
                    j = numOfEvents;
            }
        }
        else if( sources[i].type->GetName().find("SingleDecay")<G4String::npos || 
		sources[i].type->GetName().find("G4Decay")<G4String::npos )
        {
            for( G4int j=0; j<numOfEvents; j++ ) {
	        if( sources[i].pointSource ) eventPosition = sources[i].posSource;
                else eventPosition = GetEventLocation();
                startParticleTime += ( -log(G4UniformRand())
                                       / sourceActivity )*s ;
                if( startParticleTime > windowEndTime ) j = numOfEvents;
                sources[i].type->GenerateEventList( eventPosition,
                              sourceByVolumeID, i, sources[i].particleMass,
                              sources[i].particleNumber, startParticleTime/ns );
            }
        }
        else if(sources[i].type->GetName().find("SingleParticle")<G4String::npos)
        {
            for( G4int j=0; j<numOfEvents; j++ ) {
	        if( sources[i].pointSource ) eventPosition = sources[i].posSource;
		else eventPosition = GetEventLocation();
                startParticleTime += ( -log(G4UniformRand())
                                       / sourceActivity )*s ;
                if( startParticleTime > windowEndTime ) j = numOfEvents;
                sources[i].type->GenerateEventList( eventPosition,
                              sourceByVolumeID, i, sources[i].particleName,
                              sources[i].particleEnergy, startParticleTime/ns );
            }
        }
        else if(sources[i].type->GetName().find("Wimp")<G4String::npos)
        {
            for( G4int j=0; j<numOfEvents; j++ ) {
                if( sources[i].pointSource ) eventPosition = sources[i].posSource;  
		else eventPosition = GetEventLocation();
                startParticleTime += ( -log(G4UniformRand())
                                       / sourceActivity )*s ;
                if( startParticleTime > windowEndTime ) j = numOfEvents;
                sources[i].type->GenerateEventList( eventPosition,
                              sourceByVolumeID, i, sources[i].particleEnergy,
                              startParticleTime/ns);
            }
        }
        else if(sources[i].type->GetName().find("MUSUN")<G4String::npos)
        {
            for( G4int j=0; j<numOfEvents; j++ ){
                if( sources[i].pointSource ) eventPosition = sources[i].posSource;
                else eventPosition = GetEventLocation();
                startParticleTime += ( -log(G4UniformRand())
                                      / sourceActivity )*s ;
                if( startParticleTime > windowEndTime ) j = numOfEvents;
                sources[i].type->GenerateEventList( startParticleTime/ns );
            }
        }	    
        else {
            for( G4int j=0; j<numOfEvents; j++ ) {
                if( sources[i].pointSource ) eventPosition = sources[i].posSource;
		else eventPosition = GetEventLocation();
                startParticleTime += ( -log(G4UniformRand()) 
                                      / sourceActivity  )*s ;
                if( startParticleTime > windowEndTime ) j = numOfEvents;
                sources[i].type->GenerateEventList( eventPosition,
                                sourceByVolumeID, i, startParticleTime/ns );
            }
        }
        if( !(sources[i].type->GetName().find("DecayChain") < G4String::npos) ) 
            G4cout << "  Time of primary events range from 0.*ns to " 
                   << startParticleTime/ns << "*ns" << G4endl;
    }
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					DetermineCenterAndExtent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::DetermineCenterAndExtent( G4PVPlacement *ref )
{
	//	This method records the displacement and orientation of this sub-volume
	//	with respect to the physical volume that is passed to it. If you pass
	//	this method the world volume, it will record this object's global
	//	coordinates.
	
	G4ThreeVector translation(0,0,0);
	
	//	First check to make sure the reference object isn't this object itself
	if( ref == this ) {
		G4cout << "\"" << ((BaccDetectorComponent*)(this))->GetName()
			   << "\" passed as reference volume to itself in "
			   << "DetermineCenter()" << G4endl;
		globalCenter = translation;
		return;
	}
	
	//	Create a vector of physical volumes that begins with the reference
	//	volume and ends with the "this" volume.
	std::vector<G4VPhysicalVolume*> thePhysicalStack;
	thePhysicalStack.push_back( ref );

	std::vector<G4int> daughterIndices;
	daughterIndices.push_back(0);

	G4VPhysicalVolume *currentPhysicalVolume = 0;
	G4LogicalVolume *currentLogicalVolume = 0;
	while( thePhysicalStack.back() != this ) {
		currentPhysicalVolume = thePhysicalStack.back();
		currentLogicalVolume = currentPhysicalVolume->GetLogicalVolume();
		
        if( currentLogicalVolume->GetNoDaughters() > daughterIndices.back() ) {
			thePhysicalStack.push_back( currentLogicalVolume->GetDaughter(daughterIndices.back() ));
			daughterIndices.back()++;
			daughterIndices.push_back(0);
		}
        
        else {
			daughterIndices.pop_back();
			thePhysicalStack.pop_back();
		}
	}
	//	Now that we have a vector of physical volumes down to the current
	//	volume, add up the translations and rotations. Because of the
	//	potentially nested rotations, we have to start at the last volume
	//	and work our way back up to the world volume.
	globalOrientation.set( 0, 0, 0 );
	globalOrientation.transform( this->GetObjectRotationValue() );
	for( G4int i=thePhysicalStack.size()-1; i>0; i-- ) {
		translation += thePhysicalStack[i]->GetObjectTranslation();
		translation.transform( thePhysicalStack[i-1]->GetObjectRotationValue());
		globalOrientation.transform(
				thePhysicalStack[i-1]->GetObjectRotationValue() );
	}
    
	globalCenter = translation;
	
	//	Next, determine the X, Y, and Z extent of the shape. Augment the values
	//	by a small buffer to ensure complete coverage of the volume.
	this->GetLogicalVolume()->GetSolid()->CalculateExtent( kXAxis,
			G4VoxelLimits(), G4AffineTransform(), minX, maxX );
	this->GetLogicalVolume()->GetSolid()->CalculateExtent( kYAxis,
			G4VoxelLimits(), G4AffineTransform(), minY, maxY );
	this->GetLogicalVolume()->GetSolid()->CalculateExtent( kZAxis,
			G4VoxelLimits(), G4AffineTransform(), minZ, maxZ );
    minXYZ = G4ThreeVector(minX,minY,minZ);
    maxXYZ = G4ThreeVector(maxX,maxY,maxZ);
    
	minX -= 10.*nm;
	maxX += 10.*nm;
	minY -= 10.*nm;
	maxY += 10.*nm;
	minZ -= 10.*nm;
	maxZ += 10.*nm;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetEventLocation()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4ThreeVector BaccDetectorComponent::GetEventLocation()
{
	//	First determine location of the source. Pick points at random in a
	//	sphere of the appropriate radius, centered on the center of this
	//	geometry object;
	G4bool insideVolume = false;
	G4int counter = 0;
	while( !insideVolume ) {
		counter++;
		if( !(counter%100000) )
			G4cout << "Warning: It has taken at least " << counter
				   << " attempts to find a point inside the "
				   << this->GetName() << " volume" << G4endl;
	
		//	Find a point at random in the box bounded by the dimensions
		//	calculated in DetermineCenterAndExtent.
		xPos = (maxX - minX)*G4UniformRand() + minX;
		yPos = (maxY - minY)*G4UniformRand() + minY;
		zPos = (maxZ - minZ)*G4UniformRand() + minZ;

		G4ThreeVector position( xPos, yPos, zPos );
		position.transform( globalOrientation );
		position += globalCenter;
		
		if( navigator->LocateGlobalPointAndSetup( position ) == this ) {
			insideVolume = true;
			return position;
		}
	}
	
	G4cout << "WARNING! Could not find a location inside volume "
		   << this->GetName();
	return( G4ThreeVector(0,0,0) );
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetVolumePrecision()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::SetVolumePrecision( G4double prec )
{
	if( prec != volumePrecision ) {
        volumePrecision = prec;
        CalculateVolume();
        CalculateMass();
    }
    
    return;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CalculateVolume()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccDetectorComponent::CalculateVolume(
        G4bool takeOutDaughters )
{
    if( !takeOutDaughters && componentVolume >= 0 )
        return componentVolume;
    
    if( takeOutDaughters )
        G4cout << "Calculating the volume of " << GetName() << "... " << std::flush;

    //	Next, determine the X, Y, and Z extent of the shape. Augment the
    //	values by a small buffer to ensure complete coverage of the volume.
    this->GetLogicalVolume()->GetSolid()->CalculateExtent( kXAxis,
            G4VoxelLimits(), G4AffineTransform(), minX, maxX );
    this->GetLogicalVolume()->GetSolid()->CalculateExtent( kYAxis,
            G4VoxelLimits(), G4AffineTransform(), minY, maxY );
    this->GetLogicalVolume()->GetSolid()->CalculateExtent( kZAxis,
            G4VoxelLimits(), G4AffineTransform(), minZ, maxZ );
    
    //  This sets the required number of "inside" hits as a function of the
    //  required precision. The precision is only approximately accurate, but
    //  it should be in the ballpark.
    G4int targetInsideSamples = volumePrecision;
    
    G4int totalSamples = 0;
    G4int totalSamplesInt = 0;
    G4int insideSamples = 0;
    
    G4ThreeVector position;
    EInside in;
    
    while( insideSamples < targetInsideSamples ) {
        totalSamples++;
        totalSamplesInt++;
        if( totalSamplesInt == 2000000000 )
            totalSamplesInt = 0;
        
		xPos = (maxX - minX)*G4UniformRand() + minX;
		yPos = (maxY - minY)*G4UniformRand() + minY;
		zPos = (maxZ - minZ)*G4UniformRand() + minZ;
        
		position = G4ThreeVector( xPos, yPos, zPos );
        in = GetLogicalVolume()->GetSolid()->Inside(position);
		
        if( in != kOutside )
            insideSamples++;
        
        if( !(totalSamplesInt%1000000000) ) {
            G4cout << "Sampled " << totalSamples << " points in "
                   << GetName() << "," << G4endl;
            G4cout << "\t" << insideSamples
                   << " successful hits, need "
                   << targetInsideSamples << G4endl;
        }
    }
    
    G4double outerTestVolume = (maxX-minX) * (maxY-minY) * (maxZ-minZ);
    
    componentVolume = outerTestVolume * insideSamples / totalSamples;
    
    if( takeOutDaughters ) {
        for( G4int i=0; i<GetLogicalVolume()->GetNoDaughters(); i++ ) {
            BaccDetectorComponent *currentComponent =
                    (BaccDetectorComponent*)(
                    GetLogicalVolume()->
                    GetDaughter(i) );
            G4double subVolume =
                    currentComponent->CalculateVolume(false);

            componentVolume -= subVolume;
        }
    }
    
    if( takeOutDaughters )
        G4cout << componentVolume/cm3 << " cm3" << G4endl;
    
    return componentVolume;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					CalculateMass()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccDetectorComponent::CalculateMass()
{
    if( componentMass >= 0 )
        return componentMass;
    
    if( componentVolume < 0 )
        CalculateVolume();
    
    componentMass = componentVolume/cm3 *
            this->GetLogicalVolume()->GetMaterial()->GetDensity() / (g/cm3);
    
    G4cout << GetName() << " has mass " << componentMass << " g" << G4endl;
    
    return componentMass;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetVolume()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccDetectorComponent::GetVolume()
{
    if( componentVolume < 0 )
        CalculateVolume();
    
    return componentVolume;
}
 //------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetVolume()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::SetVolume( G4double v )
{
    componentVolume = v/cm3;
    CalculateMass();
}



//------++++++------++++++------++++++------++++++------++++++------++++++------
//					GetMass()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccDetectorComponent::GetMass()
{
    if( componentMass < 0 )
        CalculateMass();
    
    return componentMass;
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//					SetMass()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccDetectorComponent::SetMass( G4double m )
{
    componentMass = m/g;
    componentVolume = componentMass*g /
             (this->GetLogicalVolume()->GetMaterial()->GetDensity() / (g/cm3));
}

