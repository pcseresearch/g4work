#ifndef SBSVariables_H
#define SBSVariables_H 1

#include <iostream>
using namespace std;

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

// Since the vectors can not be dimensioned dinamically
// we give a big number (40x80=3200 modules for example)
#define NoMaxModules 3200

class SBSVariables
{
public:
    SBSVariables();
    ~SBSVariables();
    
    G4int LoadFromFile(G4String FileName);
    //   G4int SaveToFile(G4String FileName);
    
//ECalDetectorConstruction

    G4double LeadThickness;
    G4double ScintThickness;
    G4int NbOfLayers;  
    G4double CalModSizeXY;   
    G4int NbOfCaloColumns;
    G4int NbOfCaloRows;

    G4bool ECalDetail;
    G4bool Fibers;
    G4bool G4GUI;
    G4int Verbose;  


    G4double EleAng;   
    G4double ECalDis;

//ECalAnalysis
    G4String      FileNameSuffix;	
  
//SBSPrimaryGeneratorAction
    G4ThreeVector Target;

    G4int         ReactionCase;	
    G4double      EnergyBeam;
    G4int         nK;
    G4String      ReactionParticle[4];

/*

  G4String Elektrondetektor;
 
  G4double SpecBImpuls;
  //	G4double SpecBImpulsbreite;
  
  G4String Target;


  G4bool Simple;

  G4bool ScanAngles;
  G4double KAOS_ang;
  G4double C_ang;
 
  G4double maxAng;
  G4double angStep;
  
  G4bool ScanEnergy;
  G4double minEne;
  G4double maxEne;
  G4double eneStep;

 
  G4ThreeVector TARGETPosition;


  G4bool UseJoch;
  G4bool UseDipol;
  G4bool UseKolli;
  G4bool UseFibers;
  G4bool UseFieldMap;
	
	G4bool Wedeln;
	G4bool LongTarget;
	G4double MaxStepLength;
//	G4bool LimitStep;
	G4bool doDecay;
	G4bool doAllBackround;
	G4bool doSecondarys;
	G4bool doEMPhysics;
	G4bool checkTargethit;
	G4bool coincOnly;
	G4bool FastMode;
	G4bool doDrawTracks;
		

	G4double FEEWinkel;
	G4RotationMatrix FEERotation;

	G4double FEELaenge;

	G4double Detektorabstand;
	G4double Detektorwinkel;
	G4double JochVerschiebung;
	G4double Detektorhub;
	G4double Feldstaerke;
	G4double DetektorTarget;
	
	G4double SciFiWinkel;
	G4double FEE_OOP;




	G4double xwWeite;
	G4double ywWeite;
			
// Variablen, die nicht eingelesen werden:
	unsigned int nK;
	G4bool isInKField;
	G4bool doDraw;
	Gun_modes GunMode;



	G4double TargetX;
	G4double TargetY;
*/
private:
    G4bool ReadBoolean(G4String Value);
	
};

ostream & operator << (ostream& s, const SBSVariables v);

extern SBSVariables *Variables;

#endif
