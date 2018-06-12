//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: DetectorConstruction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//Mine
#include "SBSMaterials.hh"
#include "SBSDetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SBSConstants.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSDetectorConstruction::SBSDetectorConstruction(G4RunManager *runManager)
:solidWorld(0),logicWorld(0),physiWorld(0),
 magField(0)
{
//Definition of the pointer which extract the materials
//For its use e.g.  Materials->SciMaterial

   G4cout<<"<SBSDetectorConstruction::SBSDetectorConstruction>: Running"<<G4endl;

   Materials = new SBSMaterials();
  
  // create commands for interactive definition of the calorimeter
//I will not use it for the moment, anyhow it looks that here is where runmanager should be uses
//  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSDetectorConstruction::~SBSDetectorConstruction()
{

    delete Ecal;//not sure 22/07/2013
    delete Materials;//warning in cppcheck

    G4cout<<"<SBSDetectorConstruction::SBSDetectorConstruction>: Finished"<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Materials are defined in a separeted function

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* SBSDetectorConstruction::Construct()
{
 
   G4GeometryManager::GetInstance()->OpenGeometry();
   G4PhysicalVolumeStore::GetInstance()->Clean();
   G4LogicalVolumeStore::GetInstance()->Clean();
   G4SolidStore::GetInstance()->Clean();
   
   //     
  // World
  //

//For the moment the size of the world (and shape) is a box
//perhaps a realistic could be an oval 
//with the shape of Hall A
//anyhow, it should be independent of the calorimeter
//as is seen here

  solidWorld = new G4Box("World",				//its name
			 Constants->WorldSizeX/2,
			 Constants->WorldSizeYZ/2,
			 Constants->WorldSizeYZ/2);	//its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   Materials->defaultMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume
				 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number


   G4cout<<"<SBSDetectorConstruction::Construct>: World Constructed"<<G4endl;
  
   //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

 /*
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
  */



  //The Detectors IN THE WORLD (probably temporaly)
  //is sent with the pointer Materials and the parent volume

  Ecal = new ECalDetectorConstruction(physiWorld, Materials);

 //it was defined before (commented, it doesn't work)
 //return Construct();

  //
  //always return the physical World
  //
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//In case we want to apply a magnetic field for test purposes
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void SBSDetectorConstruction::SetMagField(G4double fieldValue)
{
    //apply a global uniform magnetic field along Z axis
    G4FieldManager* fieldMgr
	= G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
    if(magField) delete magField;		//delete the existing magn field
    
    if(fieldValue!=0.)			// create a new one if non nul
    { magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
	fieldMgr->SetDetectorField(magField);
	fieldMgr->CreateChordFinder(magField);
    } else {
	magField = 0;
	fieldMgr->SetDetectorField(magField);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void SBSDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
