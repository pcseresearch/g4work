#ifndef SBSConstants_H
#define SBSConstants_H 1

#include "globals.hh"
#include "G4ThreeVector.hh"

class SBSConstants
{
public:
	SBSConstants();
	~SBSConstants();

/*
    G4double           AbsorberThickness;
    G4double           GapThickness;
    G4int              NbOfLayers;
  

    G4int              NbOfCalo;
    G4int              NbOfCaloColumns;
    G4int              NbOfCaloRows;
*/
    G4double           LayerThickness;    
    G4double           CalorThickness;
    G4double           WorldSizeX;
    G4double           WorldSizeYZ;
    
    G4double           ProtonMass;
};

//With this pointer, all constants can be called in the others classes
//with the simple use of Constants-><name of the constant>

extern SBSConstants *Constants;

#endif
