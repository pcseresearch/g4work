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

#ifndef ECalDetectorConstruction_h
#define ECalDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4SubtractionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class DetectorMessenger;

// Mine
class SBSMaterials;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ECalDetectorConstruction
{
  
 public:
  
  ECalDetectorConstruction(G4VPhysicalVolume *physiParent, SBSMaterials *Materials);
  ~ECalDetectorConstruction();

  public:
  
     void UpdateGeometry();
     
  public:
  
    G4VPhysicalVolume* GetAbsorber()      {return physiAbsorber;};    
    G4VPhysicalVolume* GetGap()           {return physiGap;}; 
    
  private:
    
    G4Box*             solidCalor;    //pointer to the solid Calor 
    G4LogicalVolume*   logicCalor;    //pointer to the logical Calor
    G4VPhysicalVolume* physiCalor;    //pointer to the physical Calor
    
    G4Box*             solidCaloMod;    //pointer to the solid Calor 
    G4LogicalVolume*   logicCaloMod;    //pointer to the logical Calor
    G4VPhysicalVolume* physiCaloMod;    //pointer to the physical Calor
    
    G4Box*             solidLayer;    //pointer to the solid Layer 
    G4LogicalVolume*   logicLayer;    //pointer to the logical Layer
    G4VPhysicalVolume* physiLayer;    //pointer to the physical Layer
    
    G4Tubs*            solidAbsTub;    //pointer to the cilinder Absorber
    G4LogicalVolume*   logicAbsTub;    //pointer to the logical cilinder Absorber
    G4VPhysicalVolume* physiAbsTub;    //pointer to the physical cilinder Absorber


    G4Tubs*            solidGapTub;    //pointer to the cilinder Absorber
    G4LogicalVolume*   logicGapTub;    //pointer to the logical cilinder Absorber
    G4VPhysicalVolume* physiGapTub;    //pointer to the physical cilinder Absorber

    G4Tubs*            solidGapWLS;    //pointer to the cilinder Absorber WLS
    G4LogicalVolume*   logicGapWLS;    //pointer to the logical cilinder Absorber WLS
    G4VPhysicalVolume* physiGapWLS;    //pointer to the physical cilinder Absorber WLS

    G4Tubs*            solidAbsWLS;    //pointer to the cilinder Absorber WLS
    G4LogicalVolume*   logicAbsWLS;    //pointer to the logical cilinder Absorber WLS
    G4VPhysicalVolume* physiAbsWLS;    //pointer to the physical cilinder Absorber WLS

  
//    G4SubtractionSolid* solidAbsorber; //pointer to the solid Absorber
    G4Box*              solidAbsorber;      //pointer to the solid Gap
    G4LogicalVolume*    logicAbsorber; //pointer to the logical Absorber
    G4VPhysicalVolume*  physiAbsorber; //pointer to the physical Absorber
    
    G4Box*               solidGap_1; //pointer to the box Gap
    G4Tubs*              solidGap_2; //pointer to the cilinder Gap

//    G4SubtractionSolid* solidGap; //pointer to the solid Absorber
    G4Box*             solidGap;      //pointer to the solid Gap
    G4LogicalVolume*   logicGap;      //pointer to the logical Gap
    G4VPhysicalVolume* physiGap;      //pointer to the physical Gap
    
    G4int Columns;
    G4int Rows;

    G4int TubColumns;
    G4int TubRows;

    G4double CalorX;   
    G4double CalorY;

    G4double Rota;    //rotation angle (in deg)
    G4RotationMatrix RSRot;
    G4double ECalDis;
    G4double G4ECalDis;

private:
    
     void DefineMaterials();
     void ComputeCalorParameters();
  //     G4VPhysicalVolume* ConstructCalorimeter();  
       
};

#endif

