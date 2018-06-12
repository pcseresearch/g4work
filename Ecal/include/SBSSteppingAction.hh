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
// $Id: SteppingAction.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SBSSteppingAction_h
#define SBSSteppingAction_h 1



#include "G4UserSteppingAction.hh"

#include "G4TrackStatus.hh"

class G4Track;
class G4Run;
class SBSDetectorConstruction;
class EventAction;

class ECalAnalysis;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SBSSteppingAction : public G4UserSteppingAction
{
public:
    SBSSteppingAction(ECalAnalysis *ECalHandle);
//   virtual ~SBSSteppingAction();
    ~SBSSteppingAction();
    
    void UserSteppingAction(const G4Step*);
    // void FromRunAction(const G4Run*);
    // void RunStopAction(const G4Run*);
	
private:
    SBSDetectorConstruction* detector;
    EventAction*   eventaction;  
    ECalAnalysis*  Analysis;

    G4Track *actualTrack; // pointer to the actual track


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
