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
// $Id: RunAction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SBSRunAction.hh"

#include "ECalAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSRunAction::SBSRunAction(ECalAnalysis *ECalHandle)
{
    G4cout<<"<SBSRunAction::RunAction()>: begin"<<G4endl;

      if (ECalHandle!=NULL) Analysis=ECalHandle;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSRunAction::~SBSRunAction()
{
    
    G4cout<<"<SBSRunAction::~SBSRunAction()>: closing"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSRunAction::BeginOfRunAction(const G4Run* aRun)
{ 

//From the output, this method ONLY initializes the run, but them gives the control to the next class
    
    G4cout<<"<SBSRunAction::BeginOfSBSRunAction()>"<<G4endl;
    
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    
    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
 
    // Going to ECalAnalysis
    Analysis->BeginOfRunAction(aRun);
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSRunAction::EndOfRunAction(const G4Run* aRun)
{
    G4cout<<"<SBSRunAction::EndOfRunAction()>"<<G4endl;

 //Going to ECalAnalysis
    Analysis->EndOfRunAction(aRun);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*



*/
 //
