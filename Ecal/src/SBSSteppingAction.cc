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
// $Id: SteppingAction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SBSSteppingAction.hh"

#include "SBSDetectorConstruction.hh"

#include "ECalAnalysis.hh"

#include "SBSEventAction.hh"
#include "ECalDetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSSteppingAction::SBSSteppingAction(ECalAnalysis *ECalHandle)					 
{
//It was modified in order to obtain the information directly from ECal
//but should be a way to extract the information from the mother class

   G4cout<<"<SBSSteppingAction::SBSSteppingAction>"<<G4endl;
   
   detector = (SBSDetectorConstruction*)
	G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    
 
//Here is created the object which will send the information to the
//method in the EventAction class

    eventaction = (EventAction*)
	G4RunManager::GetRunManager()->GetUserEventAction();	     

    if (ECalHandle!=NULL) Analysis=ECalHandle;

//    ECalHandle = new ECalAnalysis();
  

 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSSteppingAction::~SBSSteppingAction()
{
    G4cout<<"SBSSteppingAction::~SBSSteppingAction()->finishing"<<G4endl;

    delete Analysis;//was commented 
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSSteppingAction::UserSteppingAction(const G4Step* aStep)
{

//   G4cout<<"<SBSSteppingAction::UserSteppingAction>"<<G4endl;
   
 
//To check only primaries
//note: it still shows the inmediatly daughters
/*
if(aStep -> GetTrack() -> GetTrackID()!=1) 
{
aStep -> GetTrack()->SetTrackStatus(fStopAndKill);
}
*/
    // collect energy and track length step by step
     
    actualTrack = aStep->GetTrack();
    
    if(Variables->Verbose == 1)
    {

    // Control of localization
	G4cout<<actualTrack->GetVolume()->GetName()<<G4endl;
    
    // check if particle goes through ECal (check if there is an easy way)
    // if(actualTrack->GetVolume()->GetName()=="Calorimeter" ||
    //    actualTrack->GetVolume()->GetName()=="CaloModul"||
    //    actualTrack->GetVolume()->GetName()=="Layer"||
    //    actualTrack->GetVolume()->GetName()=="Gap"||
    //    actualTrack->GetVolume()->GetName()=="Absorber"
    // 	)
    	G4cout<<"going to Analysis (from Stepping)"<<G4endl; 
    }


	Analysis->StepAceptance(aStep);
     

//The strategy will be ask the information requiered in every class to another class
//asking if particle is in a given geometry or another. i.e. asking if
//is in ECal, call the method in a class which is related with ECal
//if particle is in Polarimeter (future), ask the method in a class related with the Polarimeter
// The problem should check how to store the data. In individual classes for each detector or a whole class which stores all data??


  //example of saving random number seed of this event, under condition
  //// if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}
/*
void SBSSteppingAction::FromRunAction(const G4Run* aRun)
{

    G4cout<<"<SBSSteppingAction::FromRunAction>"<<G4endl;

}

void 
SBSSteppingAction::RunStopAction(const G4Run* aRun)
{

   G4cout<<"<SBSSteppingAction::RunStopAction>"<<G4endl;

   Analysis->EndOfRunAction(aRun);
}

*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
