////////////////////////////////////////////////////////////////////////////////
/*  BaccBST.cc

This is the code for the Binary Search Tree
********************************************************************************
Change log
  2013/11/20 - Initial submission (Vic)
  2014/09/08 - Changed the print order of PrintNodes method so that the output
               is time-ordered (Brian)

*/
////////////////////////////////////////////////////////////////////////////////
#include "globals.hh"
#include "BaccBST.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <list>
#include <fstream>

BaccBST::BaccBST( G4int numInitLevels, G4double windowStart, G4double 
        windowEnd, G4int numEvts )
{
  //  Record the total number of events so that they can get popped off the
  //  end of the search tree as necessary
  numEvents = numEvts;
  numNonemptyNodes = 0;
  numNodes = 0;

  nodeCount=0;
  warningsWritten=0;
  warningsWritten1e4=0;
  warningsWritten1e5=0;

  //  Create the first level with a node in the middle of the decay time space
  topNode = new decayNode;
  topNode->Z = 0;
  topNode->A = 0;
  topNode->particleName = "";
  topNode->energy = 0;
  topNode->timeOfEvent = (windowEnd + windowStart)*1.e9*ns/2. ;//in nanoseconds
  topNode->sourceByVolumeID=-1;
  topNode->sourcesID=-1;
  topNode->left = 0;
  topNode->right = 0;
  
  //  Now step through each additional initialization level, creating the
  //  other emptry nodes, always chopping the remaining space into halves.
  //  Note that the topmost level is level 1, so we start here with level 2.
  Isotope *tmpIso = 0;
  
  for( G4int i=2; i<=numInitLevels; i++ ) {
    G4double timeStep = (windowEnd + windowStart)/pow(2,i) *1.e9*ns ;//nanoseconds
    G4double runningTime = windowStart*1.e9*ns + timeStep;//ns
    G4int numPointsInLevel = (G4int) pow( 2, i-1 );
    //if(numPointsInLevel<100)std::cout << "BST nodePoint " << numPointsInLevel 
    //           << " times created: " << std::endl;
    for( G4int j=0; j<numPointsInLevel; j++ ) {
      tmpIso = new Isotope( "Empty", 0, 0, 0 );
      Insert( tmpIso, runningTime, G4ThreeVector(0,0,0), -1, -1, topNode );
      //if(numPointsInLevel<100) std::cout << j << ":" << runningTime/ns << " ";
      runningTime += 2*timeStep ;
    }
    //if(numPointsInLevel<100) std::cout << "\n" << std::endl;
  }
  G4cout << "Finished building empty nodes in BST" << G4endl;
  
  hasNodes = true;
}


////////////////////////////////////////////////////////////////////////////////
void BaccBST::Insert( Isotope *iso, G4double theTime, G4ThreeVector Pos, 
                  G4int sourceByVolumeID, G4int sourcesID )
{
    nodeCount=0;
    // Time sent and received in nanoseconds
    Insert( iso, theTime, Pos, sourceByVolumeID, sourcesID, topNode );

    if( nodeCount>1000 && warningsWritten<10) {
        G4cout << "    Filling BST in progress. BST depth reached: " << nodeCount 
               << " (Max 10 warnings)" << G4endl;
        G4cout << "      time of event: " << theTime/ns << " ns" << G4endl;
        warningsWritten++;
    }
    if( nodeCount>10000 && warningsWritten1e4<10 ) {
        G4cout << "    Warning BST depth exceeding 10000. (Max 10 warnings)" << G4endl;
        G4cout << "      time of event: " << theTime/ns << " ns" << G4endl;
        warningsWritten1e4++;
    }
    if( nodeCount>100000 && warningsWritten1e5<10 ) {
        G4cout << "    Warning BST depth exceeding 100000 !!! "
               << " (Max 10 warnings)" << G4endl;
        G4cout << "      time of event: " << theTime/ns << " ns" << G4endl;
        warningsWritten1e5++;
    }

    //  We don't want more events in the window than asked for, so if the tree
    //  gets too big, pop the last one from the BST.
    if( numNonemptyNodes > numEvents+2 )
        PopLast();
}
////////////////////////////////////////////////////////////////////////////////
decayNode *BaccBST::Insert( Isotope *iso, G4double theTime, G4ThreeVector Pos,
    G4int sourceByVolumeID, G4int sourcesID, decayNode *searchNode )
{
    decayNode *newNode = searchNode;

    if( !searchNode ) {
        newNode = new decayNode;

        newNode->Z = iso->GetZ();
        newNode->A = iso->GetA();
        newNode->timeOfEvent = (theTime);//set in nanoseconds
        newNode->pos = Pos;
        newNode->left = 0;
        newNode->right = 0;
        newNode->sourceByVolumeID = sourceByVolumeID;
        newNode->sourcesID = sourcesID;
        // used only for SingleParticle and SingleDecay respectively
        newNode->particleName = iso->GetParticleName();
        newNode->energy = iso->GetEnergy();
        
        numNodes++;
        if( newNode->Z )
            numNonemptyNodes++;
        
    } else if( theTime < searchNode->timeOfEvent ) {
        nodeCount++;
        searchNode->left = Insert( iso, theTime, Pos, sourceByVolumeID,
                sourcesID, searchNode->left );
    }
    else {
        nodeCount++;
        searchNode->right = Insert( iso, theTime, Pos, sourceByVolumeID,
                sourcesID, searchNode->right );
    }

    return newNode;
}

////////////////////////////////////////////////////////////////////////////////
decayNode *BaccBST::GetEarliest()
{
    decayNode *tmpNode = topNode;
    while( tmpNode->left )
        tmpNode = tmpNode->left;
    
    return tmpNode;
}

////////////////////////////////////////////////////////////////////////////////
decayNode *BaccBST::GetLast()
{
    decayNode *tmpNode = topNode;
    while( tmpNode->right )
        tmpNode = tmpNode->right;
    
    return tmpNode;
}

////////////////////////////////////////////////////////////////////////////////
void BaccBST::PopEarliest()
{
    //  Start with the top node, and find the last node that actually branches
    //  to the left.
    decayNode *tmpNode = topNode;
    if( topNode ) {
        if( tmpNode->left ) {
            while( ((decayNode*)(tmpNode->left))->left )
                tmpNode = tmpNode->left;
        
            decayNode *earliestNode = tmpNode->left;
        
            if( earliestNode->right )
                tmpNode->left = earliestNode->right;
            else
                tmpNode->left = 0;
            
            if( earliestNode->Z )
                numNonemptyNodes--;
            delete earliestNode;

        } else if( tmpNode->right ) {
            //  The top node is the earliest node
            topNode = topNode->right;
            if( tmpNode->Z )
                numNonemptyNodes--;
            delete tmpNode;
        } else {
            //  The top node is the only node
            topNode = 0;
            hasNodes = false;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void BaccBST::PopLast()
{
  //  Start with the top node, and find the last node that actually branches
  //  to the right.
  decayNode *tmpNode = topNode;
  if( topNode ) {
    if( tmpNode->right ) {
      while( ((decayNode*)(tmpNode->right))->right )
        tmpNode = tmpNode->right;
    
      decayNode *lastNode = tmpNode->right;
      
      if( lastNode->left )
        tmpNode->right = lastNode->left;
      else
        tmpNode->right = 0;
      
      if( lastNode->Z != 0 )
        numNonemptyNodes--;
      delete lastNode;

    } else if( tmpNode->left ) {
      //  The top node is the last node
      topNode = topNode->left;
      if( tmpNode->Z )
        numNonemptyNodes--;
      delete tmpNode;
    } else {
      //  The top node is the only node
      topNode = 0;
      hasNodes = false;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void BaccBST::PrintNodes()
{
  PrintNodes( topNode );
}

////////////////////////////////////////////////////////////////////////////////
void BaccBST::PrintNodes( decayNode *tmpNode )
{
    if( tmpNode->left )
      PrintNodes( tmpNode->left );
    if( tmpNode->Z ) {
      G4cout << "RecordTreePrint:Z_a_t_(name) <|> volID srcID: " 
      //G4cout << "RecordTreePrint:Z_A_t.pos.1_2: " 
              << tmpNode->Z << " " << tmpNode->A << " " 
              << tmpNode->timeOfEvent << " ";
      if(tmpNode->particleName != "" )  
          G4cout << tmpNode->particleName << "_" << tmpNode->energy;
      G4cout << "\t<|> " 
          //    << tmpNode->pos.getX() << " "
          //    << tmpNode->pos.getY() << " "
          //    << tmpNode->pos.getZ() << " _|\t" 
              << tmpNode->sourceByVolumeID << " "
              << tmpNode->sourcesID << G4endl;
        //<< ", left = " << tmpNode->left << ", right = " << tmpNode->right
    }
    if( tmpNode->right )
      PrintNodes( tmpNode->right );
}

