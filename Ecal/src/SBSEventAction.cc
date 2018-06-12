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
// $Id: EventAction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "SBSEventAction.hh"

#include "ECalAnalysis.hh"

#include "SBSRunAction.hh"
#include "EventActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSEventAction::SBSEventAction(ECalAnalysis *ECalHandle)
{

    G4cout<<"<SBSEventAction::SBSEventAction>"<<G4endl;
    
//From the original, the fill per event was done in run
// I'll do it in analysis class
    
    runAct = (SBSRunAction*)G4RunManager::GetRunManager()->GetUserRunAction();

    if (ECalHandle!=NULL) Analysis=ECalHandle;
     
    eventMessenger = new EventActionMessenger(this);
    printModulo = 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSEventAction::~SBSEventAction()
{
    G4cout<<"<SBSEventAction::~SBSEventAction>"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSEventAction::BeginOfEventAction(const G4Event* evt)
{  

    G4int evtNb = evt->GetEventID();
    
    if(Variables->Verbose == 1)
    {
	G4cout<<"<SBSEventAction::BeginOfEventAction>"<<G4endl;
	G4cout << "<SBSEventAction::BeginOfEventAction>:printModulo: " << printModulo << G4endl;

// I'm not sure why is set to 100, but since we are using small events is commented
//    if (evtNb%printModulo == 0) 

	G4cout << "\n---> Begin of event: " << evtNb << G4endl;
	CLHEP::HepRandom::showEngineStatus();
    }
    
//Again, somewhere I should take decision in which detector am I
    Analysis->BeginOfEventAction(evt); 
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSEventAction::EndOfEventAction(const G4Event* anEvent)
{
   //In this method, is where the event final data is calculated
   //in KAOS there were two methods with the same name
   //one in KAOSAnalyze and one in KAOSEventAction

  Analysis->EndOfEventAction(anEvent); 
    
}
  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
