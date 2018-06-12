#ifndef ECalStruct_H
#define ECalStruct_H 1

typedef struct {
    G4int EventID;
    G4int Module; //module or unit of the calorimeter
    G4float EDepMod; //Deposited energy of the Gap (Scintillator) of the module
    G4int Layer; //layer corresponding to the module
    G4float EDepLay; //Deposited energy of the Gap (Scintillator) of the layer for a given module
    G4float EDep;
} UThit;

typedef struct {
    G4int EventID;
    G4float EDepTot;
    G4float PosCal[3];
} UTEvent;

typedef struct {
    G4int runID;
    G4int noCol;
    G4int noRow;
} UTSet;






/*
typedef struct {
  G4int EventID;
  G4int multiX;
  G4int multiTH;
  G4double X;
  G4double TH;
  G4double AngleIn_X;
  G4double AngleOut_X;
  G4double AngleIn_Th;
  G4double AngleOut_Th;
  G4double target[6];
  G4int particle;
} UTevent;

#define NoOfChannels 2304
*/
#endif
