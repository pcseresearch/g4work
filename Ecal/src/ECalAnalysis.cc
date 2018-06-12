// ------------------------------------------------------------
//      ECAL Analysis class for SBS project
//      
//      Carlos Ayerbe Gayoso (based in KAOS Simulation)
//      2012
// ------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.h.......oooOO0OOooo....
//#include <string>
//#include <iostream>
//#include <sstream>
#include <time.h>

#include "ECalAnalysis.hh"

#include "SBSEventAction.hh"

//#include "SystemOfUnits.h"
#include "G4ios.hh"

#include "G4UnitsTable.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4TrackStatus.hh"
#include "G4Step.hh"
#include "G4Types.hh"
#include "G4UImanager.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



ECalAnalysis::ECalAnalysis()
{
  G4cout << "<ECalAnalysis::ECalAnalysis> Running " <<G4endl;
  
//  rootfile = NULL;
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

ECalAnalysis::~ECalAnalysis()
{

//  if (rootfile) 
    {
	{
            G4cout << "<KAOSAnalyse::~KAOSAnalyse>: before Close "  << G4endl;
        }
        G4cout << "<KAOSAnalyse::~KAOSAnalyse>: Close: "  << G4endl;
    }    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void ECalAnalysis::BeginOfRunAction(const G4Run *aRun)
{
    time(&start);//time flag for the beggining of run used in counter()

    G4cout<<"<ECalAnalysis::BeginOfRunAction>"<<G4endl;

    time_t START = time(0);
    char bufferST[100];
    strftime((char*)&bufferST,100,"%Y-%m-%d - %H-%M-%S",localtime(&START));
    G4cout<<"Time Stamp <START>: "<<bufferST<<G4endl;

    Run = const_cast <G4Run*>(aRun);//a POINTER to the present run to use in the rest of the code

// This variables are initialized here because is the summary of the run
    sumEAbs = sum2EAbs =sumEGap = sum2EGap = 0.;
    sumLAbs = sum2LAbs =sumLGap = sum2LGap = 0.; 
    
// It was at the beggining of the class, perhaps here we can create a root file every run
    rootfile = CreateSaveFile("ECalFile");

//Here we define some variable will be in the root file, should be adapted according our needs
    hittree = new TTree("hittree","module hits");
    hittree->Branch("hit",&hit.EventID, "EventID/I:Module/I:EDepMod/F:Layer/I:EDepLay/F:EDep/F");

    eventtree = new TTree("eventtree","Calorimeter Summary");
    eventtree->Branch("event",&event.EventID, "EventID/I:EDepTot/F:PosCal[3]/F");

    settree = new TTree("settree","Calorimeter Settings");
    settree->Branch("set",&set.runID, "runID/I:noCol/I:noRow/I");


    deptree = new TTree("deptree","energy deposits");
    deptree->Branch("dep",&depholder,"dep[9][20]/F");

/*TFile * out = new TFile (Form("/home/jlabdaq/analyzer/rootfiles/test%d.root",run),"RECREATE"); // creation of the output file   
int adc[Nchan]; // data array for the adc                                      
    TTree * tdata = new TTree("tdata", Form("Run %d",run)); // creation of the tree
    tdata->Branch("adc",&adc,"adc[16]/I"); // creation of the branch to hold the adc data
*/
G4cout<<"Test Point 1"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ECalAnalysis::EndOfRunAction(const G4Run *aRun)
{
//Code from the original N03

    NbOfEvents = aRun->GetNumberOfEvent();     
    if (NbOfEvents == 0) return;


//compute statistics: mean and rms
//
    sumEAbs /= NbOfEvents; sum2EAbs /= NbOfEvents;
    G4double rmsEAbs = sum2EAbs - sumEAbs*sumEAbs;
    if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;
    
    sumEGap /= NbOfEvents; sum2EGap /= NbOfEvents;
    G4double rmsEGap = sum2EGap - sumEGap*sumEGap;
    if (rmsEGap >0.) rmsEGap = std::sqrt(rmsEGap); else rmsEGap = 0.;
    
    sumLAbs /= NbOfEvents; sum2LAbs /= NbOfEvents;
    G4double rmsLAbs = sum2LAbs - sumLAbs*sumLAbs;
    if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.;
    
    sumLGap /= NbOfEvents; sum2LGap /= NbOfEvents;
    G4double rmsLGap = sum2LGap - sumLGap*sumLGap;
    if (rmsLGap >0.) rmsLGap = std::sqrt(rmsLGap); else rmsLGap = 0.;
    
//print
//
    G4cout
	<< "\n--------------------End of Run------------------------------\n"
	<< "\n mean Energy in Absorber : " << G4BestUnit(sumEAbs,"Energy")
	<< " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
	<< "\n mean Energy in Gap      : " << G4BestUnit(sumEGap,"Energy")
	<< " +- "                          << G4BestUnit(rmsEGap,"Energy")
	<< G4endl;
    
    G4cout
	<< "\n mean trackLength in Absorber : " << G4BestUnit(sumLAbs,"Length")
	<< " +- "                               << G4BestUnit(rmsLAbs,"Length")  
	<< "\n mean trackLength in Gap      : " << G4BestUnit(sumLGap,"Length")
	<< " +- "                               << G4BestUnit(rmsLGap,"Length")
	<< "\n------------------------------------------------------------\n"
	<< G4endl;
  
//we send the variables for this setup, so the root analysis is independent of the set-up
	set.runID = Run->GetRunID();//Number of run
	set.noCol = Variables->NbOfCaloColumns;
	set.noRow = Variables->NbOfCaloRows;
	settree->Fill();
	
// This method is called at the end of every run.
	dosave(); //writes data to trees
	if (rootfile) rootfile->Close();//closes current file
    delete rootfile;//closes the file reader i think --Tony

//timestamp code
    time_t END = time(0);
    char bufferEN[100];
    strftime((char*)&bufferEN,100,"%Y-%m-%d - %H-%M-%S",localtime(&END));
    G4cout<<"Time Stamp <END>: "<<bufferEN<<G4endl;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ECalAnalysis::BeginOfEventAction(const G4Event *anEvent)
{
  // This method is invoqued at the begin of every event. 
 
  if(Variables->Verbose == 1)
    {
	G4cout<<"<ECalAnalysis::BeginOfEventAction>"<<G4endl;
    }

    edep=0.;
    
    // initialisation per event

    EnergyAbs = EnergyGap = 0.;
    TrackLAbs = TrackLGap = 0.;
    
//    G4cout<<"ECalAnalysis::BeginOfEventAction:EnergyAbs: "<<EnergyAbs *MeV<<G4endl;
//    G4cout<<"ECalAnalysis::BeginOfEventAction:EnergyGap: "<<EnergyGap *MeV<<G4endl;
    
    for(G4int i=0;i<NoMaxModules;i++) EnergyAbsMod[i]=0.;
    
    for(G4int i=0;i<NoMaxModules;i++)
      {
	EnergyGapMod[i]=0.;
	for(G4int j=0;j<20;j++)
	  {
	    EnergyGapModLay[i][j]=0.; 
	    //	    G4cout<< EnergyGapModLay[i][j]<<G4endl; 
	  }
      }
    
    hit.Module = -1;
    hit.EDepMod = 0.;
    
    hit.Layer = -1;
    hit.EDepLay = 0.;
    
    hit.EDep = 0;
    
    EnergyTemp = 0;
    
    event.EDepTot = 0;
    
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void ECalAnalysis::EndOfEventAction(const G4Event* anEvent)
{

    Counter();//call the couter for screen output (better at the end of event)
    
//Once an event is finished, HERE is where the data is manipuled to be stored
    
    //accumulates statistic
    //

//In the original, was called through a handle, here is only an internal method
    
//    G4cout<<"ECalAnalysis::EndOfEventAction:EnergyAbs: "<<EnergyAbs<<G4endl;
//    G4cout<<"ECalAnalysis::EndOfEventAction:EnergyGap: "<<EnergyGap<<G4endl;


    fillPerEvent(EnergyAbs, EnergyGap, TrackLAbs, TrackLGap);
    
    
    //print per event (modulo n)
    //
    G4int evtNb = anEvent->GetEventID();

//  if (evtNb%printModulo == 0)//this sentence doesn't work, at least with this modulo
    
// std::setw(7) is a format command for the following sentence
    if(Variables->Verbose == 1)
	
    {
	G4cout<<"***********************"<<G4endl;
	G4cout<<"ECalAnalysis"<<G4endl;
	G4cout << "---> End of event: " << evtNb << G4endl;	
	G4cout<<"***********************"<<G4endl;
	
	G4cout
	    << "   Absorber: total energy: " << std::setw(7)
	    << G4BestUnit(EnergyAbs,"Energy")
	    << "       total track length: " << std::setw(7)
	    << G4BestUnit(TrackLAbs,"Length")
	    << G4endl
	    << "        Gap: total energy: " << std::setw(7)
	    << G4BestUnit(EnergyGap,"Energy")
	    << "       total track length: " << std::setw(7)
	    << G4BestUnit(TrackLGap,"Length")
	    << G4endl;
	
    }    
    
    hit.EventID = Run->GetNumberOfEvent();//Number of event. It gives the index for the storing data   
    hit.EventID = evtNb;//Number of event. It gives the index for the storing data   
    
    for (G4int i=0; i<NoMaxModules;i++)
    {
		if (EnergyGapMod[i]>0.)
		{
			hit.Module = i;
	//	    G4cout<<"i:"<<i<<G4endl;
			hit.EDepMod = EnergyGapMod[i]/MeV;//the energy deposited in the scintillator of the module

			EnergyTemp = EnergyTemp + EnergyGapMod[i]/MeV;//summing all energies in all scint (it is derived to event.EDepTot

			hit.EDep = EnergyGapMod[i]/MeV + EnergyAbsMod[i]/MeV;
	 
			if(Variables->Verbose == 1)
			  {
		   		 G4cout<<"EnergyGapMod["<<i<<"]: "<< EnergyGapMod[i] *MeV<<" MeV"<<G4endl;
			  }	
		}

	
		for (G4int j=0; j<20;j++)
		{
			if (EnergyGapModLay[i][j]>0.)
			{
			hit.Layer = j;
			hit.EDepLay = EnergyGapModLay[i][j]/MeV;
			depholder[i][j]=EnergyGapModLay[i][j]/MeV;
	//		G4cout<<"EnergyGapModLay["<<i<<","<<j<<"]: "<< EnergyGapModLay[i][j] *MeV<<" MeV"<<G4endl;
			}
		} 
	 
//there are events where particles leave 0 energy
//with our conditions, such event is not stored, but the index is increased
//perhaps this condition could be relaxed in other way

		if (EnergyGapMod[i]>0. && (hit.Module>-1))
		{
	//	    G4cout<<"fill i:"<<i<<G4endl;
			hittree->Fill();	
			deptree->Fill();
		}
    }

    event.EventID = evtNb;//Number of event. It gives the index for the storing data   
    event.EDepTot = EnergyTemp;

    event.PosCal[0]=Position.x();
    event.PosCal[1]=Position.y();
    event.PosCal[2]=Position.z();
   
    eventtree->Fill();



    return; //is necesary??
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void 
ECalAnalysis::TrackStartAction()
{
//This method is implemented to create a lisp file to be read by AutoCAD
//I have to check how it works
//I'll do not delete it becaus it could be useful later. I'll keep the KAOS structure
// but is commented, because to avoid the variables names

//**************************************************************************************
/*
  akzeptiert=false; // initializatin of a flag to check if the particle went throug KAOS
  DrawTrackKommando.str("");
  if (Variablen->doDrawTracks)
    {
      // initialization of a string containig a commando to add the track
      // as a line in a script for importing into AutoCAD
      //		G4cout << "<ECalAnalysis::TrackStartAction> --- Info:" << G4endl;
      DrawTrackKommando << "/control/shell echo \"" 
			<< "(command \\\"_.line\\\"" << endl 
			<< "\\\"" << 0 << ",0," << 0 << "\\\"" << endl;
    }  
*/
//***************************************************************************************
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void 
ECalAnalysis::TrackStopAction()
{
//***************************************************************************************
/*
	if (Variablen->doDrawTracks)
	{
//		G4cout << "<ECalAnalysis::TrackStopAction> --- Info:" << G4endl;
		DrawTrackKommando << "\\\"\\\")" << endl << "(princ)" << endl
			<< "\" >> Teilchenspuren.lsp";
		// finalization of the script command for AutoCAD
		if (verbose > 2) G4cout << "Fuehre aus: " << DrawTrackKommando.str() << G4endl;
		G4UImanager * UI = G4UImanager::GetUIpointer();
	
		if (UI) UI->ApplyCommand(DrawTrackKommando.str());
	}
*/
//****************************************************************************************
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void 
ECalAnalysis::StepAceptance(const G4Step *aStep)
{
// This method just check the aceptance of the particle in every detector of KAOS
// I'll keep the method for later, but perhaps, since I'm doing analysis separetly for every detector
// the consideration of acceptance at every detector could do it in SteppingAction class
// Must important, here is where the information from the step of the particles is gotten 
// and stored in the variables to be read for the root variables.    
    
// This method is invoked in SBSSteppingAction:UserSteppingAction

    G4Track *actualTrack = aStep->GetTrack();
    
    // method to check the acceptance of KAOS

    if(Variables->Verbose == 1)
    {
    G4cout << "<ECalAnalysis::StepAceptance>: begin" << G4endl;
    }

// Perhaps not useful in SBS    
    StepStart(aStep); // some things always done by many analysis
// Perhaps not useful in SBS    
 
    edep = aStep->GetTotalEnergyDeposit();

    stepl = 0.;
   
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
	stepl = aStep->GetStepLength();


//*********************************************************************************
//***** This part of the code gets the number of the object (module, and layer)****
//*********************************************************************************

// First define the handle point1
    G4StepPoint* point1 = aStep->GetPreStepPoint();
  
// From it, define the handle of the touchable
    G4TouchableHandle touch1 = point1->GetTouchableHandle();
  
// We can get the volume
    G4VPhysicalVolume* volume = touch1->GetVolume();
  
//from this volume, the name
    G4String name = volume->GetName();
  
// the copy number of the touchable (no from the volume)
//    G4int copyNumber = touch1->GetCopyNumber();
  
//from the touchable, the mother volume
//    G4VPhysicalVolume* mother = touch1->GetVolume(1);
  
//and the Copy Number 1:layer 2:module

    G4int copyNumberLayer = touch1->GetCopyNumber(1);
    G4int copyNumberModule = touch1->GetCopyNumber(2);

//    G4cout<<"Module: "<<copyNumberModule<<G4endl;
//    G4cout<<"Layer: "<<copyNumberLayer<<G4endl;

//********************************************
//*** End of code*****************************
//********************************************

// Get the real position of the particle when reach the calorimeter
    // if((actualTrack->GetVolume()->GetName()=="Calorimeter") &&
    //    (actualTrack->GetNextVolume()->GetName()=="CaloModul"||
    // 	actualTrack->GetNextVolume()->GetName()=="Layer"||
    // 	actualTrack->GetNextVolume()->GetName()=="Gap"||
    // 	actualTrack->GetNextVolume()->GetName()=="Absorber")
    // 	)
  

//this obtains the position of the particle when reaches the calorimeter
    if(Entry("Calorimeter")&&Exit("World"))
    {
	Position = actualTrack->GetPosition();
//	G4cout<<"Position at ECal: "<<Position/cm<<G4endl;
    }
    
    if(actualTrack->GetVolume()->GetName()=="Absorber")
    {
	EnergyAbs+= edep;
	EnergyAbsMod[copyNumberModule] += edep;
	TrackLAbs += stepl;
    }

// Gap=Scintillator, the only energy we can read
    if(actualTrack->GetVolume()->GetName()=="Gap")
    {
	EnergyGap+= edep;
	EnergyGapMod[copyNumberModule] += edep;
	EnergyGapModLay[copyNumberModule][copyNumberLayer] += edep;
	TrackLGap += stepl;
    }

//*****************************************************************
    // if we are not interested in background and leaving the hall,
    // we stop tracking
    
    if (actualTrack->GetNextVolume()->GetName()=="World")
    {
	if(Variables->Verbose == 1)
    	{
	    G4cout << "<ECalAnalysis::StepAceptance> : particle leaves hall...: "
		   << NextVolume
		   << G4endl;
	}
	actualTrack->SetTrackStatus(fStopAndKill);
    } // Particle is killed so can be proceed with other event
    
//    G4cout<<"StepAceptance:EnergyAbs: "<<EnergyAbs *MeV<<G4endl;
//    G4cout<<"StepAceptance:EnergyGap: "<<EnergyGap *MeV<<G4endl;
    

    if(Variables->Verbose == 1)
    {
    G4cout <<"<ECalAnalysis::StepAceptance>: done" << G4endl;
    }

// SHould I return??? Perhaps without returning, the object is not deleted
    //   return;
}


//Save the data
void ECalAnalysis::dosave()
{
    G4cout << "<ECalAnalysis::dosave> " <<G4endl;  
    
    if (hittree)   hittree->Write();
    if (eventtree) eventtree->Write();
    if (settree)   settree->Write();
	if (deptree)   deptree->Write();
}


TFile *
ECalAnalysis::CreateSaveFile(G4String Name)
{

    ostringstream command;
    
// Perhaps later I implement an enviroment variable for this
//  G4String pfad(getenv("KAOSDATADIR"));
    
    G4String pfad("./rootfiles/simdata/");
        
    G4String dataname;
    
    G4UImanager * UI = G4UImanager::GetUIpointer();
    
//  Watch the enviroment variable in the command
//  dataname = pfad + Name +"-" + Variablen->FileNameSuffix + ".root";
    
//  This line creates the name of the file with the time stamp

    char buffer[100];
    time_t Now = time(0);	
    strftime((char*)&buffer,100,"%Y-%m-%d-%H-%M-%S",localtime(&Now));
    FileNameSuffix = buffer;

    dataname = pfad + Name +"-" + FileNameSuffix + ".root";

//  dataname = pfad + Name +"-" + Variables->FileNameSuffix + ".root";

// useless (??)
/*    command.str("");
    command << "/control/shell cp " 
	    << dataname
	    << " " << dataname << ".bak";
    if (UI) UI->ApplyCommand(command.str());
*/

    TFile *rootoutfile = new TFile(dataname, "recreate");

    command.str("");
    command << "/control/shell ln -fs " 
	    << dataname
//	     << " " << pfad << Name << ".root";
	     << " " << Name << ".root";
    //if (verbose >0) G4cout << "Output as: " << command.str() << G4endl;
    if (UI) UI->ApplyCommand(command.str());

    return rootoutfile;
}



void 
ECalAnalysis::StepStart(const G4Step *aStep)
{
// This method was used in KAOS to make things related with the tracks
// perhaps is not useful for us
// For the moment, it makes nothing




// Get the actual track from this step
    actualTrack = aStep->GetTrack();
// Get the name of the volume from this track 
    ActualVolume = actualTrack->GetVolume()->GetName();
    
    // this is more "special" (I have to check its use)
    //from THIS track, we look the NEXT volume and if is true, make a handle and get the name
    if (actualTrack->GetNextVolume())
	{
	    NextVolume = actualTrack->GetNextVolume()->GetName();
	}
    
// I think the AutoCAD aoutput was somewhere else, maybe was only the file
// here is the data
/*
    if (Variablen->doDrawTracks)
    {
	// If we want the output, we add the step point to the line.
	G4ThreeVector Punkt = aStep->GetPostStepPoint()->GetPosition();
	DrawTrackKommando << "\\\"" << Punkt.x()/mm << "," << Punkt.y()/mm << "," 
			  << Punkt.z()/mm << "\\\"" << endl;
	Variablen->doDraw=true;
    }
*/  

}

void 
ECalAnalysis::Counter()
{
//the couter is set to give output AFTER an event. 

    time(&end); //this is the flag end time between events
  
    NbOfEventsC = Run->GetNumberOfEvent()+1;//number of events start at 0 

//Because I don't know how to read a function which needs and object as argument
//when the object is defined somewhere and I don't know how to access it
    TotalNnbEvent =  NumberOfEvents(Run);

    time_elapsed = difftime(end, start);

    Even25 = TotalNnbEvent/10;

    RestEventFactor = ((G4double)TotalNnbEvent-NbOfEventsC)/NbOfEventsC;
    resttime = (time_t)(time_elapsed*RestEventFactor);
    /*    
    G4cout<<"RestEventFactor: "<<RestEventFactor<<G4endl;
    G4cout<<"time_elapsed: "<<time_elapsed<<G4endl;
    G4cout<<"resttime: "<<resttime<<G4endl;
    */
    G4int rtage    = resttime / (86400);
    G4int rhour    = (resttime % (86400)) / 3600;
    G4int rminutes = (resttime % 3600) / 60;
    G4int rseconds = resttime % 60;

    if(NbOfEventsC == Even25 || 
       NbOfEventsC == 2*Even25 || 
       NbOfEventsC == 3*Even25 || 
       NbOfEventsC == 4*Even25 || 
       NbOfEventsC == 5*Even25 || 
       NbOfEventsC == 6*Even25 || 
       NbOfEventsC == 7*Even25 || 
       NbOfEventsC == 8*Even25 || 
       NbOfEventsC == 9*Even25 || 
       NbOfEventsC == TotalNnbEvent-1)   
    {
	G4cout << NbOfEventsC << " Events from " <<TotalNnbEvent  
	       << " processed so far = " 
	       << ((NbOfEventsC*100.0)/TotalNnbEvent)
	       << "%. ";
	G4cout << G4endl;
        
	G4cout<<"ETA: ";
	if (rtage>0)G4cout<<rtage<<" years ";
	if (rhour>0)G4cout<<rhour<<" h: ";
	if (rminutes>0)G4cout<<rminutes<<" m: ";
	G4cout<<rseconds<<" s";
	G4cout<<""<<G4endl;
    }

    //TOTAL TIME EMPLOYED IN THE RUN
    if (NbOfEventsC == TotalNnbEvent)
    {
	resttime = (time_t)(time_elapsed*1.0);
	G4int ry    = resttime / (86400);
	G4int rh    = (resttime % (86400)) / 3600;
	G4int rm = (resttime % 3600) / 60;
	G4int rs = resttime % 60;
	
	G4cout<<"Total time of process: ";
	if (ry>0)G4cout<<ry<<" years ";
	if (rh>0)G4cout<<rh<<" h: ";
	if (rm>0)G4cout<<rm<<" m: ";
	G4cout<<rs<<" s";
	G4cout<<""<<G4endl;
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
// These methods just check if particles enter a volume different to the present (Entry)
// or leaves the volume (Exit)

G4bool 
ECalAnalysis::Entry(G4String Volumen)
{
  if ((ActualVolume != Volumen) 
      && (NextVolume == Volumen))
    return true;
  else 
    return false;
}

G4bool 
ECalAnalysis::Exit(G4String Volumen)
{
  if ((ActualVolume == Volumen) 
      && (NextVolume != Volumen))
    return true;
  else 
    return false; 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....



void 
ECalAnalysis::CheckAkzeptiert()
{
//Useless!!! In Kaos only works to check if enter the dipole and then accept the use of some detectors
}


void
ECalAnalysis::fillPerEvent(G4double EAbs, G4double EGap,
			   G4double LAbs, G4double LGap)
{

    if(Variables->Verbose == 1)
    {
	G4cout<<"<ECalAnalysis::fillPerEvent>:EAbs "<<EAbs *MeV<<G4endl;
    }

  //accumulate statistic
    sumEAbs += EAbs;  
    sum2EAbs += EAbs*EAbs;
    sumEGap += EGap;
    sum2EGap += EGap*EGap;
    
    sumLAbs += LAbs;  sum2LAbs += LAbs*LAbs;
    sumLGap += LGap;  sum2LGap += LGap*LGap;  

    if(Variables->Verbose == 1)
    {
	G4cout<<"<ECalAnalysis::fillPerEvent>:sumEAbs: "<<sumEAbs *MeV<<G4endl;
    }
}


/*
void ECalAnalysis::InitilizationOfEventAction()
{

    G4cout<<" ECalAnalysis::InitilizationOfEventAction()"<<G4endl;
    EnergyAbs = EnergyGap = 0.;
    TrackLAbs = TrackLGap = 0.;

 

}
*/

G4int 
ECalAnalysis::NumberOfEvents(const G4Run* aRun)
{
    nbEventInRun = aRun->GetNumberOfEventToBeProcessed();
    return nbEventInRun;
}

/*
G4int 
ECalAnalysis::GetNumberOfEvents()
{
    return nbEventInRun;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//
//      ----------- ECalAnalysis   ------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
