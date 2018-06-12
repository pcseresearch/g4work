#include "SBSVariables.hh"
#include "SBSConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

SBSConstants::SBSConstants()
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Perhaps, these parameters should not be calculated here
//but since some of then are used in different classes
//meanwhile are here

//Last change. Derived values are calculated here from direct values in the ini file

 
  LayerThickness = Variables->LeadThickness + Variables->ScintThickness;
  CalorThickness = (Variables->NbOfLayers)*LayerThickness;

  G4cout<<"LT "<<LayerThickness<<G4endl;
  G4cout<<"CT "<<CalorThickness<<G4endl;

//old values from example
/*
  WorldSizeX = 1.2*CalorThickness; 
  WorldSizeYZ = 1.2*CalorSizeYZ;
*/

//temptative values for world
  WorldSizeX = 20.*m; 
  WorldSizeYZ = 20.*m;

//I'm not sure if a definition is better
  ProtonMass = 938.27 *MeV;




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


}


SBSConstants::~SBSConstants()
{
	G4cout << "<SBSConstans::~SBSConstants>: Finished" << G4endl;
}


//With this pointer, all constants can be called in the others classes
//with the simple use of Constants-><name of the constant>

SBSConstants *Constants=NULL;

  
