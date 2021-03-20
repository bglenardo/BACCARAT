////////////////////////////////////////////////////////////////////////////////
/*    BaccIsotope.cc

This is the code for the Isotope information stored in the Binary Search Tree.
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
*/
////////////////////////////////////////////////////////////////////////////////

#include "BaccIsotope.hh"
#include "globals.hh"

////////////////////////////////////////////////////////////////////////////////
Isotope::Isotope( G4String na, G4int zee, G4int eh,
        G4double hl ) :
    name(na),
    halflife(hl),
    Z(zee),
    A(eh),
    particleName(""),
    energy(0.)
{}

////////////////////////////////////////////////////////////////////////////////
Isotope::Isotope( G4String na, G4int zee, G4int eh,
        G4String pn, G4double en) :
    name(na),
    halflife(0),
    Z(zee),
    A(eh),
    particleName(pn),
    energy(en)
{}

////////////////////////////////////////////////////////////////////////////////
void Isotope::AddDaughter( Isotope *daughter, G4double ratio )
{
    daughters.push_back( daughter );
    branchRatio.push_back( ratio );
}

////////////////////////////////////////////////////////////////////////////////
Isotope *Isotope::GetNextDaughter()
{
    if( !daughters.size() )
        return 0;

    G4double prob = G4UniformRand();
    G4int index=0;
    while( index<(G4int)daughters.size() ) {
        if( prob < branchRatio[index] )
            return daughters[index];
        index++;
    }
    
    G4cout << "Couldn't find a daughter! Probability is " << prob
              << " and branch ratios are" << G4endl;
    for( G4int i=0; i<(G4int)daughters.size(); i++ )
        G4cout << branchRatio[i] << G4endl;
    exit(0);
}

