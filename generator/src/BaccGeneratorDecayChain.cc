////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorDecayChain.hh

The code to produce time-ordered decays from a chain (e.g., from 238U or 232Th) 
for Monte Carlo simulation purposes.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#include "BaccGeneratorDecayChain.hh"
#include "BaccBST.hh"

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4GenericIon.hh"
#include "G4SystemOfUnits.hh"
//
//    CLHEP includes
//
#include "Randomize.hh"

//
//  C++ includes
//
#include <iostream>
#include <fstream>
#include <sstream>

//
//  Variable Definitions
// 
#define SECONDS  1.
#define MINUTES  60.
#define HOURS    3600.
#define DAYS     86400.
#define YEARS    31557600.
    
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorDecayChain()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorDecayChain::BaccGeneratorDecayChain()
{
    name = "DecayChain";
    activityMultiplier = 1;
    ion = G4GenericIon::Definition();

    originalRate = 0;
    originalTime_s = 0;

    //   Initialize the Isotope Chains for both Th232 and U238               
    //
    //      The Th232 decay chain
    //
    halflivesTh[0] = 1.405e10*YEARS;  //    Th232
    halflivesTh[1] = 5.75*YEARS;      //    Ra228
    halflivesTh[2] = 6.15*HOURS;      //    Ac228
    halflivesTh[3] = 1.9116*YEARS;    //    Th228
    halflivesTh[4] = 3.66*DAYS;       //    Ra224
    halflivesTh[5] = 55.6*SECONDS;    //    Rn220
    halflivesTh[6] = 0.145*SECONDS;   //    Po216
    halflivesTh[7] = 10.64*HOURS;     //    Pb212
    halflivesTh[8] = 60.55*MINUTES;   //    Bi212
    halflivesTh[9] = 3.053*MINUTES;   //    Tl208 //daughter of Bi212
    halflivesTh[10]= 0.299e-6*SECONDS;//    Po212 //daughter of Bi212

    lambdaTh232[0] = log(2.)/halflivesTh[0];
    lambdaTh232[1] = log(2.)/halflivesTh[1];
    lambdaTh232[2] = log(2.)/halflivesTh[2];
    lambdaTh232[3] = log(2.)/halflivesTh[3];
    lambdaTh232[4] = log(2.)/halflivesTh[4];
    lambdaTh232[5] = log(2.)/halflivesTh[5];
    lambdaTh232[6] = log(2.)/halflivesTh[6],
    lambdaTh232[7] = log(2.)/halflivesTh[7];
    lambdaTh232[8] = log(2.)/halflivesTh[8];
    lambdaTh232[9] = log(2.)/halflivesTh[9];
    lambdaTh232[10]= log(2.)/halflivesTh[10];

    Isotope *Pb208 = new Isotope( "Pb208", 82, 208, 0 );
    
    Isotope *Po212 = new Isotope( "Po212", 84, 212, halflivesTh[10] );
    Po212->AddDaughter( Pb208, 1 );
    
    Isotope *Tl208 = new Isotope( "Tl208", 81, 208, halflivesTh[9] );
    Tl208->AddDaughter( Pb208, 1 );
    
    Isotope *Bi212 = new Isotope( "Bi212", 83, 212, halflivesTh[8] );
    Bi212->AddDaughter( Pb208, 0.00023 );
    Bi212->AddDaughter( Tl208, 0.359285 );
    Bi212->AddDaughter( Po212, 1 );
    
    Isotope *Pb212 = new Isotope( "Pb212", 82, 212, halflivesTh[7] );
    Pb212->AddDaughter( Bi212, 1 );
    
    Isotope *Po216 = new Isotope( "Po216", 84, 216, halflivesTh[6] );
    Po216->AddDaughter( Pb212, 1 );
    
    Isotope *Rn220 = new Isotope( "Rn220", 86, 220, halflivesTh[5] );
    Rn220->AddDaughter( Po216, 1 );
    
    Isotope *Ra224 = new Isotope( "Ra224", 88, 224, halflivesTh[4] );
    Ra224->AddDaughter( Rn220, 1 );
    
    Isotope *Th228 = new Isotope( "Th228", 90, 228, halflivesTh[3] );
    Th228->AddDaughter( Ra224, 1 );
    
    Isotope *Ac228 = new Isotope( "Ac228", 89, 228, halflivesTh[2] );
    Ac228->AddDaughter( Th228, 1 );
    
    Isotope *Ra228 = new Isotope( "Ra228", 88, 228, halflivesTh[1] );
    Ra228->AddDaughter( Ac228, 1 );
    
    Isotope *Th232 = new Isotope( "Th232", 90, 232, halflivesTh[0] );
    Th232->AddDaughter( Ra228, 1 );
    
    isoArrayTh[0] = Th232;
    isoArrayTh[1] = Ra228;
    isoArrayTh[2] = Ac228;
    isoArrayTh[3] = Th228;
    isoArrayTh[4] = Ra224;
    isoArrayTh[5] = Rn220;
    isoArrayTh[6] = Po216;
    isoArrayTh[7] = Pb212;
    isoArrayTh[8] = Bi212;
    isoArrayTh[9] = Tl208;
    isoArrayTh[10]= Po212;

    //
    //      The U238 decay chain
    //
    //    For the halflife definitions, we need six decay chains, and each decay
    //    chain has 14 entries.
    halflivesU238[0] = 4.468e9*YEARS;     //U238
    halflivesU238[1] = 24.10*DAYS;        //Th234
    halflivesU238[2] = 6.7*HOURS;         //Pa234
    halflivesU238[3] = 2.455e5*YEARS;     //U234
    halflivesU238[4] = 7.538e4*YEARS;     //Th230
    halflivesU238[5] = 1600*YEARS;        //Ra225
    halflivesU238[6] = 3.8235*DAYS;       //Rn222
    halflivesU238[7] = 3.1*MINUTES;       //Po218  //Pb or At
    halflivesU238[8] = 26.8*MINUTES;      //Pb214  //toBi214       9a
    halflivesU238[9] = 1.5*SECONDS;       //At218 //Bi214 or 218Rn 9b
    halflivesU238[10]= 19.9*MINUTES;      //Bi214  //Po or Tl     10
    halflivesU238[11]= 35e-3*SECONDS;     //Rn218  //to Po214     10b 
    halflivesU238[12]= 1.3*MINUTES;      //Tl210  //             11a
    halflivesU238[13]= 164.3e-6*SECONDS; //Po214  //             11b
    halflivesU238[14]= 22.3*YEARS;       //Pb210  //Hg or Bi     12
    halflivesU238[15]= 8.15*MINUTES;     //Hg206  //to Tl        13a 
    halflivesU238[16]= 5.013*DAYS;       //Bi210  //to Tl or Po  13b
    halflivesU238[17]= 4.199*MINUTES;    //Tl206  //to stable Pb 14a
    halflivesU238[18]= 138.376*DAYS;     //Po210  //to stable Pb 14b

    lambdaU238[0] = log(2.)/halflivesU238[0];
    lambdaU238[1] = log(2.)/halflivesU238[1];
    lambdaU238[2] = log(2.)/halflivesU238[2];
    lambdaU238[3] = log(2.)/halflivesU238[3];
    lambdaU238[4] = log(2.)/halflivesU238[4];
    lambdaU238[5] = log(2.)/halflivesU238[5];
    lambdaU238[6] = log(2.)/halflivesU238[6],
    lambdaU238[7] = log(2.)/halflivesU238[7];
    lambdaU238[8] = log(2.)/halflivesU238[8];
    lambdaU238[9] = log(2.)/halflivesU238[9];
    lambdaU238[10]= log(2.)/halflivesU238[10];
    lambdaU238[11]= log(2.)/halflivesU238[11];
    lambdaU238[12]= log(2.)/halflivesU238[12];
    lambdaU238[13]= log(2.)/halflivesU238[13];
    lambdaU238[14]= log(2.)/halflivesU238[14];
    lambdaU238[15]= log(2.)/halflivesU238[15];
    lambdaU238[16]= log(2.)/halflivesU238[16];
    lambdaU238[17]= log(2.)/halflivesU238[17];
    lambdaU238[18]= log(2.)/halflivesU238[18];

    Isotope *Pb206 = new Isotope( "Pb206", 82, 206, 0);

    Isotope *Po210 = new Isotope( "Po210", 84, 210, halflivesU238[18] );
    Po210->AddDaughter( Pb206, 1 );

    Isotope *Tl206 = new Isotope( "Tl206", 81, 206, halflivesU238[17] );
    Tl206->AddDaughter( Pb206, 1 );

    Isotope *Bi210 = new Isotope( "Bi210", 83, 210, halflivesU238[16] );
    Bi210->AddDaughter( Tl206, 1.3e-6 );
    Bi210->AddDaughter( Po210, 1 );
    
    Isotope *Hg206 = new Isotope( "Hg206", 80, 206, halflivesU238[15] );
    Hg206->AddDaughter( Tl206, 1 );
    
    Isotope *Pb210 = new Isotope( "Pb210", 82, 210, halflivesU238[14] );
    Pb210->AddDaughter( Hg206, 1.9e-8 );
    Pb210->AddDaughter( Bi210, 1 );
    
    Isotope *Po214 = new Isotope( "Po214", 84, 214, halflivesU238[13] );
    Po214->AddDaughter( Pb210, 1 );

    Isotope *Tl210 = new Isotope( "Tl210", 81, 210, halflivesU238[12] );
    Tl210->AddDaughter( Pb210, 1 );

    Isotope *Rn218 = new Isotope( "Rn218", 86, 218, halflivesU238[11] );//
    Rn218->AddDaughter( Po214, 1 );//
    
    Isotope *Bi214 = new Isotope( "Bi214", 83, 214, halflivesU238[10] );
    Bi214->AddDaughter( Tl210, 2.0e-4 );
    Bi214->AddDaughter( Po214, 1 );

    Isotope *At218 = new Isotope( "At218", 85, 218, halflivesU238[9] );//
    At218->AddDaughter( Rn218, 0.001 );//
    At218->AddDaughter( Bi214, 1 );//

    Isotope *Pb214 = new Isotope( "Pb214", 82, 214, halflivesU238[8] );
    Pb214->AddDaughter( Bi214, 1 );
    
    Isotope *Po218 = new Isotope( "Po218", 84, 218, halflivesU238[7] );
    Po218->AddDaughter( At218, 0.0002 );//
    Po218->AddDaughter( Pb214, 1 );
    
    Isotope *Rn222 = new Isotope( "Rn222", 86, 222, halflivesU238[6] );
    Rn222->AddDaughter( Po218, 1 );
    
    Isotope *Ra226 = new Isotope( "Ra226", 88, 226, halflivesU238[5] );
    Ra226->AddDaughter( Rn222, 1 );
    
    Isotope *Th230 = new Isotope( "Th230", 90, 230, halflivesU238[4] );
    Th230->AddDaughter( Ra226, 1 );
    
    Isotope *U234 = new Isotope( "U234", 92, 234, halflivesU238[3] );
    U234->AddDaughter( Th230, 1 );
    
    Isotope *Pa234 = new Isotope( "Pa234", 91, 234, halflivesU238[2] );
    Pa234->AddDaughter( U234, 1 );
    
    Isotope *Th234 = new Isotope( "Th234", 90, 234, halflivesU238[1] );
    Th234->AddDaughter( Pa234, 1 );
    
    Isotope *U238 = new Isotope( "U238", 92, 238, halflivesU238[0] );
    U238->AddDaughter( Th234, 1 );

    isoArrayU[0] = U238 ;
    isoArrayU[1] = Th234;
    isoArrayU[2] = Pa234;
    isoArrayU[3] = U234 ;
    isoArrayU[4] = Th230 ;
    isoArrayU[5] = Ra226 ;
    isoArrayU[6] = Rn222 ;
    isoArrayU[7] = Po218 ;
    isoArrayU[8] = Pb214 ;//a
    isoArrayU[9] = At218 ;//b
    isoArrayU[10]= Bi214 ;//a
    isoArrayU[11]= Rn218 ;//b
    isoArrayU[12]= Tl210 ;//a
    isoArrayU[13]= Po210 ;//b
    isoArrayU[14]= Pb210 ;
    isoArrayU[15]= Hg206 ;//a
    isoArrayU[16]= Bi210 ;//b
    isoArrayU[17]= Tl206 ;//a
    isoArrayU[18]= Po214 ;//b
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorDecayChain()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorDecayChain::~BaccGeneratorDecayChain() 
{ }

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          CalculatePopulationsInEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorDecayChain::CalculatePopulationsInEventList(G4double
                    sourceAge, G4double initialActivity, G4String iso )
{
    // initialActivity is the individual activity of each source
    //    Determine the original population of the parent nucleus based on its
    //    activity rate. (Work in units of Bq and seconds)
    originalTime_s = 0;
    Isotope *origIso = 0;

    if( iso.find( "Th232")!=G4String::npos  )
        origIso = isoArrayTh[0]; 
    else if( iso.find("U238")!=G4String::npos ) {
        origIso = isoArrayU[0]; 
    } else G4cout << "Parent Isotope not found in DecayChain\n" ;
    
    //Isotope *origIso = isoArrayTh[0]; 
    //G4double Th228pop = populationTh[3];
    G4double No;
    No = initialActivity * origIso->GetHalflife() / log(2.);
    G4int numEvents = baccManager->GetNumEvents();
    while( numEvents > No )
        No *= 2;
    G4double totalRate;

    if( iso.find("Th232")!=G4String::npos  ) {
        this->CalculatePopulationsTh232( No, sourceAge );
        totalRate = this->CalculateRatesTh232( );
    } 
    else if ( iso.find("U238")!=G4String::npos  ) {
        this->CalculatePopulationsU238( No, sourceAge );
        totalRate = this->CalculateRatesU238( );
    } 
    else G4cout << "Parent Isotope not found again \n";

    G4cout << "With a source age of " << sourceAge << 
                " seconds, the populations and rates are" << G4endl;

    if( iso.find("Th232")!=G4String::npos  ) {
      for( G4int i=0; i<11; i++ ) {
        G4cout << "\t" << isoArrayTh[i]->Name() << ": " << populationTh[i];
        G4cout << " \t" << ratesTh[i] << " Bq" << G4endl;
      }
    } 
    else if( iso.find("U238")!=G4String::npos  ) {
      for( G4int i=0; i<19; i++ ) {
        G4cout << "\t" << isoArrayU[i]->Name() << ": " ;
        if( i==3 || i==0 ) G4cout << " " ;
        G4cout << populationU[i];
        G4cout << " \t" << ratesU[i] << " Bq" << G4endl;
      }
    } 
    else G4cout << "Parent Isotope not found once again \n";

    G4cout << G4endl;    
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorDecayChain::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4String iso )
{
    //    The time that has past since the last decay from the starting
    //    population depends on the current rate of the starting population,
    //    which itself falls over time.
    Isotope *currentIso = 0;
    G4double chainTime_ns = 0;
    G4double prob;
    G4double tmpRates[19];//for both U and Th
    G4int index;


    if( iso.find("Th232")!=G4String::npos  ) {
        originalRate = CalculateRatesTh232( );//for each event
        for( G4int i=0; i<11; i++ )
            tmpRates[i] = ratesTh[i]/originalRate;
        for( G4int i=1; i<11; i++ )
            tmpRates[i] += tmpRates[i-1];
    } 
    else if( iso.find("U238")!=G4String::npos  ) {
        originalRate = CalculateRatesU238( );
        for( G4int i=0; i<19; i++ )
            tmpRates[i] = ratesU[i]/originalRate;
        for( G4int i=1; i<19; i++ )
            tmpRates[i] += tmpRates[i-1];
    } 
    else G4cout << "Parent Isotope not found\n";


    //for each parent in population
    originalTime_s += -log(1.-G4UniformRand())/(originalRate) ;//seconds
    
    //    Select an isotope from the surviving original population based
    //    on relative decay rates.
    //for( G4int i=0; i<11; i++ )
    //    tmpRates[i] = ratesTh[i]/originalRate;
    //for( G4int i=1; i<11; i++ )
    //    tmpRates[i] += tmpRates[i-1];

    prob = G4UniformRand();
    index = 0;
    while( prob > tmpRates[index] )
        index++;

    if( iso.find("Th232")!=G4String::npos  ) {        
        currentIso = isoArrayTh[index];
        populationTh[index]--;
        if( populationTh[index] < 0.5 )
            populationTh[index] = 0;
    } 
    else if( iso.find("U238")!=G4String::npos  ) {
        currentIso = isoArrayU[index];
        populationU[index]--;
        if( populationU[index] < 0.5 )
            populationU[index] = 0;
    } 
    else G4cout << "Parent Isotope not found\n";
        
    //    The stocastically-determined next decay of the source must have its
    //    rate adjusted by source's halflife...as time goes on, decays of the
    //    primary become less frequent. This doesn't make a large difference
    //    for sources with long half lives compared to any descendant nucleus,
    //    but it can make a big difference for calibration sources such as
    //    Th228.
    chainTime_ns = originalTime_s *1.e9*ns; //convert s->ns
    baccManager->RecordTreeInsert( currentIso, originalTime_s*1.e9*ns, position, 
                sourceByVolumeID, sourcesID );
        
    while( (currentIso = currentIso->GetNextDaughter()) ) {
        if( currentIso->GetHalflife() ) {
            chainTime_ns += -log(1.-G4UniformRand())
                           *( currentIso->GetHalflife()/log(2.) ) *1.e9*ns ;
            baccManager->RecordTreeInsert( currentIso, chainTime_ns, position,
                            sourceByVolumeID, sourcesID ); 
        }
    }
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                        GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorDecayChain::GenerateFromEventList( G4GeneralParticleSource 
        *particleGun, G4Event *event, decayNode *firstNode ) 
{
     
    G4double time = (firstNode->timeOfEvent)/ns; //time in ns
    G4ThreeVector pos = G4ThreeVector(firstNode->pos);
    G4int zee = firstNode->Z;
    G4int ehh = firstNode->A;

    particleGun->GetCurrentSource()->SetParticleDefinition( ion );
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(
            GetRandomDirection() );
    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy( 0.*keV );
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );

    std::stringstream uistring;
    uistring.str("");
    uistring << "/gps/ion " << zee << " " << ehh << " 0 0" ;
    UI->ApplyCommand(uistring.str());
    uistring.clear();
    uistring.str("");
    uistring << "/grdm/nucleusLimits " << ehh << " " << ehh << " " << 
                                          zee << " " << zee ;
    UI->ApplyCommand(uistring.str());

    particleGun->GeneratePrimaryVertex(event);
    baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
}



//------++++++------++++++------++++++------++++++------++++++------++++++------
//    The equations to determine the populations come from the Bateman equations.
//    These equations were solved using a Mathematica notebook entitled
//    "RateEquations_Th232.nb". Ask Kareem for it if you don't have it. Index
//    0 is the top of the decay chain. 
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                          CalculatePopulationsU238()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4int BaccGeneratorDecayChain::CalculatePopulationsU238( G4double No, G4double t)
{
    populationU[0] = exp(-4.915959875924379e-18*t) * No;
    populationU[1] = ( -1.476772694065758e-11*exp(-3.32885344897776e-7*t) + 
                       1.476772694065758e-11*exp(-4.915959875924379e-18*t) )*No;
    populationU[2] = ( 2.004780283175612e-15*exp(-2.873744529684682e-5*t) - 
                       1.730693904187612e-13*exp(-3.32885344897776e-7*t) +
                       1.710646101355856e-13*exp(-4.915959875924379e-18*t) )*No;
    populationU[3] = (  -2.004780289417107e-15*exp(-2.873744529684682e-5*t)
                        +1.494080034666468e-11*exp(-3.32885344897776e-7*t)
                        -5.494931889085189e-5*exp(-8.94684673141757e-14*t)
                        +5.494930395205634e-5*exp(-4.915959875924379e-18*t) )*No;
    populationU[4] = (  6.241495016753246e-24*exp(-2.873744529684682e-5*t)
                        -4.015591848754958e-18*exp(-3.32885344897776e-7*t)
                        +7.475692676819515e-6*exp(-2.913837718974546e-13*t)
                        -2.434798764397141e-5*exp(-8.94684673141757e-14*t)
                        +1.687229496715592e-5*exp(-4.915959875924379e-18*t) ) *No;

    populationU[5] = (  -6.328576567088756e-32*exp(-2.873744529684682e-5*t)
                        +3.515103839599242e-24*exp(-3.32885344897776e-7*t)
                        -5.09465295551836e-11*exp(-1.372781795351883e-11*t)
                        +1.621185725523342e-7*exp(-2.913837718974546e-13*t)
                        -5.201955328237691e-7*exp(-8.94684673141757e-14*t)
                        +3.581279068009902e-7*exp(-4.915959875924379e-18*t) )*No;

    populationU[6] = (  3.261263785782452e-38*exp(-2.873744529684682e-5*t)
                        -1.84646151880591e-32*exp(-2.098218075594718e-6*t)
                        +2.733462352940275e-29*exp(-3.32885344897776e-7*t)
                        -3.333253426141205e-16*exp(-1.372781795351883e-11*t)
                        +1.060678385070362e-12*exp(-2.913837718974546e-13*t)
                        -3.403435497196595e-12*exp(-8.94684673141757e-14*t)
                        +2.343090437468847e-12*exp(-4.915959875924379e-18*t) )*No;
    populationU[7] = (  -3.56776669205883e-54*exp(-3.726597744945943e-3*t)
                        +1.850487057410076e-41*exp(-2.873744529684682e-5*t)
                        -1.040214640033569e-35*exp(-2.098218075594718e-6*t)
                        +1.539182085545851e-32*exp(-3.32885344897776e-7*t)
                        -1.87675007975207e-19*exp(-1.372781795351883e-11*t)
                        +5.972027872177941e-16*exp(-2.913837718974546e-13*t)
                        -1.916265282238993e-15*exp(-8.94684673141757e-14*t)
                        +1.319250170029175e-15*exp(-4.915959875924379e-18*t) )*No;
    //8 for Pb214 (99.98%) 
    //9 for At218 (0.02%)
    G4double populationU_89 = 
                      (  4.034436596927284e-54*exp(-3.726597744945943e-3*t)
                        -1.555847595337013e-47*exp(-4.310616794527023e-4*t)
                        +1.714045615389058e-40*exp(-2.873744529684682e-5*t)
                        -9.0368105464372e-35*exp(-2.098218075594718e-6*t)
                        +1.331676119061768e-31*exp(-3.32885344897776e-7*t)
                        -1.62248076577862e-18*exp(-1.372781795351883e-11*t)
                        +5.162914422017989e-15*exp(-2.913837718974546e-13*t)
                        -1.656642244344005e-14*exp(-8.94684673141757e-14*t)
                        +1.140513050218783e-14*exp(-4.915959875924379e-18*t) 
                          ) * No ;
    populationU[8] = populationU_89 * 0.9998 ;
    populationU[9] = populationU_89 * 0.0002 ;
    //10 for Bi214(99.98+0.02*99.9 %) = .99999 98 (frac)
    //11 for Rn218 (0.02*0.1 %)       = 2e-7
    G4double populationU_1011 =
                     (  -5.527816135048955e-55*exp(-3.726597744945943e-3*t)
                        +5.736000249127921e-48*exp(-5.805252768508755e-4*t)
                        -4.487154659015442e-47*exp(-4.310616794527023e-4*t)
                        +1.339028045521156e-40*exp(-2.873744529684682e-5*t)
                        -6.734509860743714e-35*exp(-2.098218075594718e-6*t)
                        +9.89386542763562e-32*exp(-3.32885344897776e-7*t)
                        -1.204752537406774e-18*exp(-1.372781795351883e-11*t)
                        +3.833656606333112e-15*exp(-2.913837718974546e-13*t)
                        -1.230118681624122e-14*exp(-8.94684673141757e-14*t)
                        +8.46873496244552e-15*exp(-4.915959875924379e-18*t) 
                          ) * No  ;
    populationU[10] = populationU_1011 * 0.9999998 ;
    populationU[11] = populationU_1011 * 2e-7 ;
    //12 for Tl210 (99.98*0.021 + 0.02*99.9*0.021  % )     = 0.00021
    //13 for Po214 (0.02*(0.1+99.9*99.979)+99.98*99.979  % = 0.99979
    G4double populationU_1213 = ( 
                        (1.843882486597709e-116) * exp(-4.218789899938803e3*t)-
                        (7.606540981996828e-62) * exp(-3.726597744945943e-3*t)+
                        (7.89300645405204e-55) * exp(-5.805252768508755e-4*t)-
                        (6.174535894513268e-54) * exp(-4.310616794527023e-4*t)+
                        (1.842565405173635e-47) * exp(-2.873744529684682e-5*t)-
                        (9.26700142940118e-42) * exp(-2.098218075594718e-6*t)+
                        (1.361442286339027e-38) * exp(-3.32885344897776e-7*t)-
                        (1.657795995778338e-25) * exp(-1.372781795351883e-11*t)+
                        (5.275291293304273e-22) * exp(-2.913837718974546e-13*t)-
                        (1.692700999923311e-21) * exp(-8.94684673141757e-14*t)+
                        (1.165337650192461e-21) * exp(-4.915959875924379e-18*t) 
                                ) * No ;
    populationU[12] = populationU_1213 * 0.00021 ;
    populationU[13] = populationU_1213 * 0.99979 ;

    populationU[14] = (
                        (-1.843882486598139e-116) * exp(-4.218789899938803e3*t)+
                        (8.61118074615658e-56) * exp(-3.726597744945943e-3*t)-
                        (5.736010770503737e-48) * exp(-5.805252768508755e-4*t)+
                        (6.043016679813324e-47) * exp(-4.310616794527023e-4*t)-
                        (2.70506396020934e-39) * exp(-2.873744529684682e-5*t)+
                        (1.864148132097477e-32) * exp(-2.098218075594718e-6*t)-
                        (1.730530951835598e-28) * exp(-3.32885344897776e-7*t)+
                        (1.903048006428163e-18) * exp(-9.84955548234535e-10*t)-
                        (7.201084550093131e-13) * exp(-1.372781795351883e-11*t)+
                        (2.260196565554167e-9) * exp(-2.913837718974546e-13*t)-
                        (7.250884184965159e-9) * exp(-8.94684673141757e-14*t)+
                        (4.991407725962949e-9) * exp(-4.915959875924379e-18*t)
                             ) * No ;
    //15 for Hg206(1.9e-6% i.e. 1.9e-8 fraction)
    //16 for Bi210(almost100%=99.9999981)
    G4double populationU_1516 = (
                        (4.304890604361422e-129) * exp(-4.218789899938803e3*t)-
                        (3.689901687472954e-62) * exp(-3.726597744945943e-3*t)+
                        (9.87507627118107e-58) * exp(-1.427991719324156e-3*t)-
                        (6.666595100394e-54) * exp(-5.805252768508755e-4*t)+
                        (5.970431794415028e-53) * exp(-4.310616794527023e-4*t)-
                        (1.904134084414076e-45) * exp(-2.873744529684682e-5*t)+
                        (1.287685962403713e-38) * exp(-2.098218075594718e-6*t)-
                        (1.193909932705132e-34) * exp(-3.32885344897776e-7*t)+
                        (1.312625948736896e-24) * exp(-9.84955548234535e-10*t)-
                        (4.966939340840388e-19) * exp(-1.372781795351883e-11*t)+
                        (1.558967827104134e-15) * exp(-2.913837718974546e-13*t)-
                        (5.001288530871191e-15) * exp(-8.94684673141757e-14*t)+
                        (3.442817396388514e-15) * exp(-4.915959875924379e-18*t)
                            ) * No ;
    populationU[15] = populationU_1516 * 1.9e-8 ;
      populationU[16] = populationU_1516 * 0.999999981 ;

    //17 for Tl206(1.9e-6 + ~1*0.00013  %) = 0.00000 1319
    //18 for Po210 (~1*99.99987  %)        = 0.99999 8681
    G4double populationU_1718 = (
                        (-1.457136356491624e-135) * exp(-4.218789899938803e3*t)+
                        (5.39864254083581e-62) * exp(-3.726597744945943e-3*t)-
                        (2.200636971858023e-60) * exp(-2.750584049841053e-3*t)+
                        (1.06620360768536e-57) * exp(-1.427991719324156e-3*t)-
                        (4.386905422995525e-54) * exp(-5.805252768508755e-4*t)+
                        (3.675639119525667e-53) * exp(-4.310616794527023e-4*t)-
                        (9.98986386847291e-46) * exp(-2.873744529684682e-5*t)+
                        (6.690246935787638e-39) * exp(-2.098218075594718e-6*t)-
                        (6.199046543569501e-35) * exp(-3.32885344897776e-7*t)+
                        (6.814624176580999e-25) * exp(-9.84955548234535e-10*t)-
                        (2.578633539634732e-19) * exp(-1.372781795351883e-11*t)+
                        (8.09352889311618e-16) * exp(-2.913837718974546e-13*t)-
                        (2.596466233664059e-15) * exp(-8.94684673141757e-14*t)+
                        (1.787371207024943e-15) * exp(-4.915959875924379e-18*t)
                            ) * No ;
      populationU[17] = populationU_1718 * 0.000001319 ;
      populationU[18] = populationU_1718 * 0.999998681 ;

    for( G4int i=0; i<19; i++ ) {
        if( populationU[i] < 1e-3 )
            populationU[i] = 0;
    }
    return 0;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              CalculatePopulationsTh232() 
//------++++++------++++++------++++++------++++++------++++++------++++++------
//  There are 11 entries in this chain because of the branch split at Bi212. 
//  population[9] refers to Tl208 (BR = 35.9285%), and population[10] refers 
//  to Po212 (BR = 64.0485%).
G4int BaccGeneratorDecayChain::CalculatePopulationsTh232( G4double No, G4double t)
{
    populationTh[0] = exp(-1.5633102295822154e-18*t) * No;
    populationTh[1] = (
            -4.092526692066336e-10*exp(-3.819914560979152e-9*t) +
            4.092526692066336e-10*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[2] = (
            6.093349974484541e-18*exp(-3.130746072989816e-5*t) -
            4.994020470155861e-14*exp(-3.819914560979152e-9*t) +
            4.993411135158412e-14*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[3] = (
            -6.095587109366911e-18*exp(-3.130746072989816e-5*t) +
            6.778395547091779e-11*exp(-1.1490117558919298e-8*t) -
            2.0384088895130996e-10*exp(-3.819914560979152e-9*t) +
            1.3605693957597927e-10*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[4] = (
            2.405556715232096e-21*exp(-3.130746072989816e-5*t) -
            7.054965126025266e-18*exp(-2.1919499486438264e-6*t) +
            3.5719328825327396e-13*exp(-1.1490117558919298e-8*t) -
            1.0703914945104614e-12*exp(-3.819914560979152e-9*t) +
            7.132052588167564e-13*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[5] = (
            -1.5678962924313953e-35*exp(-1.246667590935153e-2*t) +
            4.240212053384925e-25*exp(-3.130746072989816e-5*t) -
            1.2406554893120405e-21*exp(-2.1919499486438264e-6*t) +
            6.280347200448607e-17*exp(-1.1490117558919298e-8*t) -
            1.8820135514825727e-16*exp(-3.819914560979152e-9*t) +
            1.2539912337523934e-16*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[6] = (
            4.9322469413906426e-51*exp(-4.780325383172037*t) -
            4.099629652917722e-38*exp(-1.246667590935153e-2*t) +
            1.105817939633977e-27*exp(-3.130746072989816e-5*t) -
            3.235523892780367e-24*exp(-2.191949948643827e-6*t) +
            1.6378603349890488e-19*exp(-1.1490117558919298e-8*t) -
            4.908128870198534e-19*exp(-3.819914560979152e-9*t) +
            3.2703008793902333e-19*exp(-1.5633102295822154e-18*t)
                    ) * No;
    populationTh[7] = (
            -4.932265594977269e-51*exp(-4.780325383172037*t) +
            1.574278913936552e-35*exp(-1.246667590935153e-2*t) -
            3.996044717306284e-22*exp(-3.130746072989816e-5*t) +
            3.800394464773913e-21*exp(-1.807895619613837e-5*t) -
            9.73553906353928e-19*exp(-2.191949948643827e-6*t) +
            4.333482791325874e-14*exp(-1.14901175589193e-8*t) -
            1.298051237710613e-13*exp(-3.819914560979152e-9*t) +
            8.64712660109189e-14*exp(-1.563310229582215e-18*t)
                    ) * No;
    populationTh[8] = (
            1.865433115695027e-56*exp(-4.780325383172037*t) -
            2.318474143005267e-38*exp(-1.246667590935153e-2*t) +
            2.396402722556043e-27*exp(-1.907919572144083e-4*t) -
            4.529864594644605e-23*exp(-3.130746072989816e-5*t) +
            3.978111934342818e-22*exp(-1.807895619613837e-5*t) -
            9.33236359993876e-20*exp(-2.191949948643827e-6*t) +
            4.10654438335809e-15*exp(-1.14901175589193e-8*t) -
            1.230024665004796e-14*exp(-3.819914560979152e-9*t) +
            8.19379523781093e-15*exp(-1.563310229582215e-18*t)
                    ) * No;
    
    populationTh[9] = (
            -7.451199607187105e-61*exp(-4.780325383172037*t) +
            5.094564996414698e-40*exp(-1.246667590935153e-2*t) -
            2.24312350070092e-36*exp(-3.783967575935938e-3*t) +
            1.272452043056786e-28*exp(-1.907919572144083e-4*t) -
            2.303064240820659e-24*exp(-3.130746072989816e-5*t) +
            2.015438688209806e-23*exp(-1.807895619613837e-5*t) -
            4.708211413795812e-21*exp(-2.191949948643827e-6*t) +
            2.070572761732444e-16*exp(-1.14901175589193e-8*t) -
            6.201930789765633e-16*exp(-3.819914560979152e-9*t) +
            4.131404931632829e-16*exp(-1.563310229582215e-18*t)
                    ) * No * 0.359285 ;
    populationTh[10] = (
            -5.005595553391863e-112*exp(-2.318217995183764e6*t) +
            1.535275672293019e-66*exp(-4.780325383172037*t) -
            1.90813039495327e-48*exp(-1.246667590935153e-2*t) +
            1.972266485282762e-37*exp(-1.907919572144083e-4*t) -
            3.72812968295356e-33*exp(-3.130746072989816e-5*t) +
            3.274031016729366e-32*exp(-1.807895619613837e-5*t) -
            7.680640562577292e-30*exp(-2.191949948643827e-6*t) +
            3.379732371659991e-25*exp(-1.14901175589193e-8*t) -
            1.012324180667312e-24*exp(-3.819914560979152e-9*t) +
            6.743585951294984e-25*exp(-1.563310229582215e-18*t)
                    ) * No * 0.640485;
    
    for( G4int i=0; i<11; i++ )
        if( populationTh[i] < 1e-3 )
            populationTh[i] = 0;

  return 0;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//    For this section, the rates couldn't be calculated directly from the
//    Bateman equations because the terms involved ended up involving terms like
//    exp(54000), which is too big a number for computers to deal with. Instead,
//    the rates are calcuated from the populations and half-lives. The return
//    value is the total rate in the entire chain.
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              CalculateRatesTh232()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorDecayChain::CalculateRatesTh232( )
{
    G4double totalRate = 0;
    for( G4int i=0; i<11; i++ ) {
        ratesTh[i] = populationTh[i] * lambdaTh232[i];
            totalRate += ratesTh[i];
    }
    
    return totalRate;
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              CalculateRatesU238()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorDecayChain::CalculateRatesU238( )
{
    G4double totalRate = 0;
    for( G4int i=0; i<19; i++ ) {
        ratesU[i] = populationU[i] * lambdaU238[i];
            totalRate += ratesU[i];
    }
    
    return totalRate;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//    Starting with the initial populations, decrease them by the specified time.
//    If the number falls below 0.5, assume the population is 0. Note that the
//    time is cumulative...if you call ReducePopulationsTh232(10) twice, the final
//    populations are the same as calling ReducePopulationsTh232(20) once.
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              ReducePopulationsTh232()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorDecayChain::ReducePopulationsTh232( G4double t )
{
    for( G4int i=0; i<11; i++ ) {
        populationTh[i] *= exp( -lambdaTh232[i]*t );
        if( populationTh[i] < 0.5 )
            populationTh[i] = 0;
    }
    return CalculateRatesTh232( );
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                              ReducePopulationsU238()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorDecayChain::ReducePopulationsU238( G4double t )
{
    for( G4int i=0; i<19; i++ ) {
        populationU[i] *= exp( -lambdaU238[i]*t );
        if( populationU[i] < 0.5 )
            populationU[i] = 0;
    }    
    return CalculateRatesU238( );
}
