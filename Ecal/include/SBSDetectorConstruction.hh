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
// $Id: DetectorConstruction.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SBSDetectorConstruction_h
#define SBSDetectorConstruction_h 1

#include "G4RunManager.hh"
#include "ECalDetectorConstruction.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"



#include <CLHEP/Vector/TwoVector.h>

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

// Mine
class SBSMaterials;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SBSDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
  SBSDetectorConstruction(G4RunManager *);
  ~SBSDetectorConstruction();
 
    G4VPhysicalVolume* Construct();
    void SetMagField(G4double);
    void UpdateGeometry();
     
  public:

     


  private:
  
     SBSMaterials* Materials;

            
     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     G4UniformMagField* magField;      //pointer to the magnetic field
     
//    DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
     //pointers to the different detectors
     ECalDetectorConstruction *Ecal;


  private:
    
     void DefineMaterials();
     void ComputeCalorParameters();
//     G4VPhysicalVolume* Construct();     
};


#endif

