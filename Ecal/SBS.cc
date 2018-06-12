//
// ********************************************************************
//    SBS SIMULATION FOR GEP(5)
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "SBSConstants.hh"
#include "SBSVariables.hh"
#include "SBSDetectorConstruction.hh"
#include "SBSPrimaryGeneratorAction.hh"
#include "SBSEventAction.hh"
#include "SBSRunAction.hh"
#include "SBSSteppingAction.hh"

#include "ECalAnalysis.hh"

#include "PhysicsList.hh"

#include "SteppingVerbose.hh"


#include "G4UIterminal.hh"
#include "G4UItcsh.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
//The first object to load should be the variables, since they are read externally
  Variables = new SBSVariables(); // This object stores all parameters which are read from SBS.ini
  
// It could be implemented an enviroment variable in order to determine the work dir
// could be useful to separate where to store data file

//  G4String pfad(getenv("SBSDATADIR")); // getting our workdir from an enviroment variable

  G4String dataname;
  dataname = "SBS.ini";
  G4cout << "SBS-Geant Simulation. Compiled on: "__DATE__" "__TIME__"." << G4endl;
 
//  In Kaos, was implemented a class called Settings, but for me it's not clear its use
// ReadSetup(dataname);
  
  Variables->LoadFromFile(dataname); // Read the parameters
//  G4cout << "Variables Setting: " << G4endl << Variables << G4endl;
// and display them for check


//The construction of this structure is not robust, since ALL Variables should be loaded
// before anything (e.g Constants)
//Perhaps the ini file should be loaded directly in the Variables class

  Constants = new SBSConstants(); // This object stores all parameters which are fix.

  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  //with the next code, we assure that every time the program runs, the engine has new seeds
  G4int index=time(NULL);
  G4long seeds[2];
  seeds[0] = (long) time(NULL) ;
  seeds[1] = (long) (time(NULL)*G4UniformRand());
  CLHEP::HepRandom::setTheSeeds(seeds,index);
  
  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  //Exported
  G4cout << "runManager Created" << G4endl;

//Could it help? I should study better how the runManager works
  ECalAnalysis *ECalHandle = new ECalAnalysis();// This object does the analysis of the tracking

  // then we create all objects which describe the detector and the world:

  //here is created the detector
  //it is made calling a general class SBSDetectorCosnstruction, whichs call the 
  //rest of the rest of the detectors

  SBSDetectorConstruction *myDetector = new SBSDetectorConstruction(runManager); 

  runManager->SetUserInitialization(myDetector);

  G4cout << "<SBS:main>: SetUserInitializaton" << G4endl;


  // Set mandatory initialization classes
  //
  //Another method is used 
  //runManager->SetUserInitialization(new SBSDetectorConstruction);

  //
  runManager->SetUserInitialization(new PhysicsList);
    
  // Set user action classes
  //
  G4cout << "SBS main: SetUserAction-> SBSPrimaryGeneratorAction" << G4endl;
  runManager->SetUserAction(new SBSPrimaryGeneratorAction);
  //
  G4cout << "SBS main: SetUserAction-> RunAction" << G4endl;
  runManager->SetUserAction(new SBSRunAction(ECalHandle));
  //
  G4cout << "SBS main: SetUserAction-> EventAction" << G4endl;
  runManager->SetUserAction(new SBSEventAction(ECalHandle));
  //
  G4cout << "SBS main: SetUserAction-> SteppingAction" << G4endl;
  runManager->SetUserAction(new SBSSteppingAction(ECalHandle));

  

  // Initialize G4 kernel
  //
  runManager->Initialize();
  G4cout << "SBS main: RunManager Running!" << G4endl;

  

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();



  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else
    { 
      if(Variables->G4GUI)
	{
	  // G4UIterminal is a (dumb) terminal.
	  G4UIsession * session = 0;
#ifdef G4UI_USE
	  session = new G4UIterminal(new G4UItcsh);
#else
	  session = new G4UIterminal();
#endif
	  session->SessionStart();
	  delete session;
	}
      else
	{
	  // interactive mode : define UI session
#ifdef G4UI_USE
	  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
	  UImanager->ApplyCommand("/control/execute SBSvis.mac"); 
#endif
	  if (ui->IsGUI())
	    UImanager->ApplyCommand("/control/execute SBSgui.mac");
	  ui->SessionStart();
	  delete ui;
#endif
	}
    }
  

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

//Always objects should be deleted at the end
  delete Constants;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
