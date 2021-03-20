////////////////////////////////////////////////////////////////////////////////
/*    BaccGeneratorMASN.cc

This is the code file for the cosmic Muon And Spallation Neutron generator.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

//
//    General notes on this generator
//
/*
This generator is based on Mei and Hime's paper on muons and 
muon-induced neutrons.  It has the option of generating either
just muons, or just the muon-induced neutons described by equations
in section C of the paper.  See Physical Review D 73 053004 (2006) for 
further details.
*/

//
//    GEANT4 includes
//
#include "globals.hh"
#include "G4Geantino.hh"
#include "G4Neutron.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4GenericIon.hh"

//
//    Bacc includes
//
#include "BaccGeneratorMASN.hh"

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    BaccGeneratorMASN()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorMASN::BaccGeneratorMASN()
{
    name = "MASN";
    activityMultiplier = 1;

    //    Get the Muon angle:
    G4double h_0 = 1.95;                        //km.w.e
    G4double I_1 = 8.6e-6;                      //s^-1 cm^-2 sr^-1
    G4double I_2 = 0.44e-6;                     //s^-1 cm^-2 sr^-1
    G4double lambda_1 = 0.45;                   //km.w.e
    G4double lambda_2 = 0.87;                   //km.w.e

    G4double muonAnglePDF[1001];
    G4double muonAngleArea = 0.;
    for(int i=0; i<1001; i++){
        G4double sectheta = 1./cos(pi/2*i/1000);
        muonAngle[i] = pi/2*i/1000;
        muonAnglePDF[i] = (I_1*exp(-h_0*sectheta/lambda_1)+
                I_2*exp(-h_0*sectheta/lambda_2))*sectheta;
        muonAngleArea+=muonAnglePDF[i];
    }
    for(int i=0; i<1001; i++) {
        muonAnglePDF[i]/=muonAngleArea;
        muonAngleCDF[i]=0.;
        for(int j=0; j<i; j++) muonAngleCDF[i]+=muonAnglePDF[j];
    }

    //    Get the muon energy:
    G4double h = h_0*1./cos(GetMuonAngle());    //km.w.e
    G4double b = 0.4;                           //km.w.e^{-1}
    G4double gamma_mu = 3.77;
    G4double epsilon_mu = 693.;                 //GeV

    G4double muonEnergyPDF[1001];
    G4double muonEnergyArea=0.;
    for(int i=0; i<1001; i++){
        muonEnergy[i] = 4.*i;
        muonEnergyPDF[i] = exp(-b*h*(gamma_mu-1))*
                pow(muonEnergy[i] + epsilon_mu*(1-exp(-b*h)),-gamma_mu);
        muonEnergyArea+=muonEnergyPDF[i];
    }
    for(int i=0; i<1001; i++){
        muonEnergyPDF[i]/=muonEnergyArea;
        muonEnergyCDF[i]=0.;
        for(int j=0; j<i; j++) muonEnergyCDF[i]+=muonEnergyPDF[j];
    }
    muonEnergyCDF[1000] = 1.;                   //force the CDF to go 
                                                //to one rather than 0.9999
    
    //    Get the neutron energy:
    G4double a_0 = 7.333;
    G4double a_1 = 2.105;
    G4double a_2 = -5.35e-15;
    G4double a_3 = 2.893;
    G4double tempMuonEnergy = GetMuonEnergy();
    G4double B = 0.324 - 0.641*exp(-0.014*tempMuonEnergy);
    
    G4double neutronEnergyPDF[1001];
    G4double neutronEnergyArea=0.;
    for(int i=0; i<1001; i++){
        neutronEnergy[i] = (i+2.5)/250;
        neutronEnergyPDF[i] = exp(-a_0*neutronEnergy[i])/neutronEnergy[i] + 
                B*exp(-a_1*neutronEnergy[i]) + a_2*pow(neutronEnergy[i],-a_3);
        neutronEnergyArea+=neutronEnergyPDF[i];
    }
    for(int i=0; i<1001; i++){ 
        neutronEnergyPDF[i]/=neutronEnergyArea;
        neutronEnergyCDF[i]=0.;
        for(int j=0; j<i; j++) neutronEnergyCDF[i]+=neutronEnergyPDF[j];
    }

    //    Get the neutron multiplicity:
    //    generic parameters:
    tempMuonEnergy = GetMuonEnergy();
    G4double B_M = 0.321*pow(tempMuonEnergy,-0.247);
    G4double C_M = 318.1*exp(-0.01421*tempMuonEnergy);
    G4double D_M = 2.02*exp(-0.006959*tempMuonEnergy);

    G4double neutronMultPDF[1001];
    G4double neutronMultArea=0.;
    for(int i=0; i<1001; i++){
        neutronMult[i] = (i+5.)/5.;
        neutronMultPDF[i] = exp(-B_M*neutronMult[i]) + 
                C_M*exp(-D_M*neutronMult[i]);
        neutronMultArea+=neutronMultPDF[i];
    }
    for(int i=0; i<1001; i++){
        neutronMultPDF[i]/=neutronMultArea;
        neutronMultCDF[i]=0.;
        for(int j=0; j<i; j++) neutronMultCDF[i]+=neutronMultPDF[j];
    }

    //    Get the neutron angle:
    //    generic parameters:
    tempMuonEnergy = GetMuonEnergy();
    G4double B_theta = 0.482*pow(tempMuonEnergy,0.045);
    G4double C_theta = 0.832*pow(tempMuonEnergy,-0.152);

    G4double neutronAnglePDF[1001];
    G4double neutronAngleArea=0.;
    for(int i=0; i<1001; i++){
        neutronAngle[i] = pi*i/1000.;
        neutronAnglePDF[i] = 1./(pow(1.-cos(neutronAngle[i]),B_theta)+C_theta);
        neutronAngleArea+=neutronAnglePDF[i];
    }
    for(int i=0; i<1001; i++){
        neutronAnglePDF[i]/=neutronAngleArea;
        neutronAngleCDF[i]=0.;
        for(int j=0; j<i; j++) neutronAngleCDF[i]+=neutronAnglePDF[j];
    }
    neutronAngleCDF[1000]=1.;

    //neutronDef = G4Geantino::Definition();
    neutronDef = G4Neutron::Definition();
    //muonDef = G4Geantino::Definition();

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    ~BaccGeneratorMASN()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorMASN::~BaccGeneratorMASN() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorMASN::GenerateEvent( G4GeneralParticleSource *particleGun,
//        G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorMASN::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int a=-1; G4int z=-1;
    G4double hl=-1;
    Isotope *currentIso = new Isotope(name, z, a, hl);
    baccManager->RecordTreeInsert( currentIso, time, position, 
                  sourceByVolumeID, sourcesID );    
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorMASN::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
    G4double time = (firstNode->timeOfEvent)/ns;

    G4ThreeVector pos = G4ThreeVector(firstNode->pos);

    //    The Muon:
    if(G4UniformRand() < 0.65)
        muonDef = G4MuonPlus::MuonPlusDefinition();
    else
        muonDef = G4MuonMinus::MuonMinusDefinition();
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->SetParticleTime( time*ns );
    G4ThreeVector muonDir = GetRandomDirection();
    G4double muonAngleTemp = GetMuonAngle();
    muonDir.setTheta( muonAngleTemp );
    //    angle treated correctly?
  /*
      particleGun->SetParticleDefinition( muonDef );
      particleGun->GetCurrentSource()->GetAngDist()->
              SetParticleMomentumDirection( muonDir );
      particleGun->GetCurrentSource()->GetEneDist()->
              SetMonoEnergy(GetMuonEnergy()*GeV);
      particleGun->GeneratePrimaryVertex( event );
      baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
  */
      //    The Neutrons:
      G4double neutronAngleTemp = GetNeutronAngle();
      for(int n=0; n<200; n++){
          G4ThreeVector neutronDir = GetRandomDirection();
  //        neutronDir.rotate(neutronAngleTemp, muonDir);
          neutronDir.setTheta(neutronAngleTemp+muonAngleTemp);
          particleGun->SetParticleDefinition( neutronDef );
          particleGun->GetCurrentSource()->GetAngDist()->
                  SetParticleMomentumDirection( neutronDir );
          particleGun->GetCurrentSource()->GetEneDist()->
                  SetMonoEnergy(GetNeutronEnergy()*GeV);
          particleGun->GeneratePrimaryVertex( event );
          baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );
      }
    
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetMuonAngle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorMASN::GetMuonAngle()
{
    G4double prob = G4UniformRand();
    G4int indexLo = 0, indexHi = 1000;

    while(!(muonAngleCDF[indexLo+1] > prob && 
            muonAngleCDF[indexHi-1] < prob) ) {
        if(muonAngleCDF[(indexLo+indexHi)/2] < prob )
            indexLo = (indexLo + indexHi)/2;
        else
            indexHi = (indexLo + indexHi)/2;
    }
    G4double split = (prob - muonAngleCDF[indexLo]) /
            (muonAngleCDF[indexHi] - muonAngleCDF[indexLo]);
    G4double angle = muonAngle[indexLo] + 
            split*(muonAngle[indexHi] - muonAngle[indexLo]);

    return (angle);
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetMuonEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorMASN::GetMuonEnergy()
{
    G4double prob = G4UniformRand();
    G4int indexLo = 0, indexHi = 1000;

    while(!(muonEnergyCDF[indexLo+1] > prob && 
            muonEnergyCDF[indexHi-1] < prob) ) {
        if(muonEnergyCDF[(indexLo+indexHi)/2] < prob )
            indexLo = (indexLo + indexHi)/2;
        else
            indexHi = (indexLo + indexHi)/2;
    }
    G4double split = (prob - muonEnergyCDF[indexLo]) /
            (muonEnergyCDF[indexHi] - muonEnergyCDF[indexLo]);
    G4double energy = muonEnergy[indexLo] + 
            split*(muonEnergy[indexHi] - muonEnergy[indexLo]);


    return (energy);
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetNeutonAngle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorMASN::GetNeutronAngle()
{
    G4double prob = G4UniformRand();
    G4int indexLo = 0, indexHi = 1000;

    while(!(neutronAngleCDF[indexLo+1] > prob && 
            neutronAngleCDF[indexHi-1] < prob) ) {
        if(neutronAngleCDF[(indexLo+indexHi)/2] < prob )
            indexLo = (indexLo + indexHi)/2;
        else
            indexHi = (indexLo + indexHi)/2;
    }
    G4double split = (prob - neutronAngleCDF[indexLo]) /
            (neutronAngleCDF[indexHi] - neutronAngleCDF[indexLo]);
    G4double angle = neutronAngle[indexLo] + 
            split*(neutronAngle[indexHi] - neutronAngle[indexLo]);

    return (angle);
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetNeutronEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorMASN::GetNeutronEnergy()
{
    G4double prob = G4UniformRand();
    G4int indexLo = 0, indexHi = 1000;

    while(!(neutronEnergyCDF[indexLo+1] > prob && 
            neutronEnergyCDF[indexHi-1] < prob) ) {
        if(neutronEnergyCDF[(indexLo+indexHi)/2] < prob )
            indexLo = (indexLo + indexHi)/2;
        else
            indexHi = (indexLo + indexHi)/2;
    }
    G4double split = (prob - neutronEnergyCDF[indexLo]) /
            (neutronEnergyCDF[indexHi] - neutronEnergyCDF[indexLo]);
    G4double energy = neutronEnergy[indexLo] + 
            split*(neutronEnergy[indexHi] - neutronEnergy[indexLo]);

    return (energy);
}
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                  GetNeutronMultiplicity()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorMASN::GetNeutronMultiplicity()
{
    G4double prob = G4UniformRand();
    G4int indexLo = 0, indexHi = 1000;

    while(!(neutronMultCDF[indexLo+1] > prob && 
            neutronMultCDF[indexHi-1] < prob) ) {
        if(neutronMultCDF[(indexLo+indexHi)/2] < prob )
            indexLo = (indexLo + indexHi)/2;
        else
            indexHi = (indexLo + indexHi)/2;
    }
    G4double split = (prob - neutronMultCDF[indexLo]) /
            (neutronMultCDF[indexHi] - neutronMultCDF[indexLo]);
    G4double energy = neutronMult[indexLo] + 
            split*(neutronMult[indexHi] - neutronMult[indexLo]);

    return (energy);
}
