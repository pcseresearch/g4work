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
// $Id: PrimaryGeneratorAction.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SBSPrimaryGeneratorAction_h
#define SBSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class SBSDetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SBSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    SBSPrimaryGeneratorAction();    
    virtual ~SBSPrimaryGeneratorAction();
    
    void GeneratePrimaries(G4Event*);
    void SetRndmFlag(G4String val) { rndmFlag = val;}
    
private:
    G4ParticleGun*              particleGun;	 //pointer a to G4  class
    SBSDetectorConstruction*    Detector;     //pointer to the geometry
    
    PrimaryGeneratorMessenger*  gunMessenger;   //messenger of this class
    G4String                    rndmFlag;	     //flag for a rndm impact point

//elastic reaction parameters
    G4double                    mProton;
    G4double                    EnergyBeam;

    G4double                    theta, phi; //random angles

    G4double RndAptX, RndAptY;//Random half aperture (half size module)
    G4double ModHalfSide;


    G4double EleMomSca;
    G4double ProEneSca, ProMomSca, ProMomAng;

    G4double Mom_Para[2]; //momentum parameters 
    G4double Ang_Para[2]; //angle parameters
    G4double Phi_Para[2]; //Phi angle parameters

    G4double Dist; //distance target calorimeter
    G4double RndScaAng; //angle of aperture for the randon scan

    G4int Columns, Rows;
    G4double MaxAperX,  MaxAperY; //maximum aperture or displacement of the random position, it is the size of the calorimeter
    G4double Xrnd,Yrnd; //the random values of X and Y (target)

 };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


