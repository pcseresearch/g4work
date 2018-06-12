#include "SBSMaterials.hh"
//#include "SBSVariablen.hh"
//#include "SBSSettings.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



SBSMaterials::SBSMaterials()
{ 
  G4cout<<"<SBSMaterials>:Loading..."<<G4endl;

  //This function illustrates the possible ways to define materials
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4int iz, n;                 //iz=number of protons  in an isotope; 
  // n=number of nucleons in an isotope;
  
  G4int ncomponents, natoms;
  G4double abundance, fractionmass;
  
//
// define Elements
//
  
  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  G4Element* Si = new G4Element("Silicon",symbol="Si" , z= 14., a= 28.09*g/mole);
  
  //
  // define an Element from isotopes, by relative abundance 
  //
  
  G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
  G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);
  
  G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
  U->AddIsotope(U5, abundance= 90.*perCent);
  U->AddIsotope(U8, abundance= 10.*perCent);
  
  //
  // define simple materials
//
  
  new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);


  //
  // Blei
  //
  new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);

  G4Material* Pb = new G4Material("Lead", z=82., a=207.19*g/mole, density=11.35*g/cm3);

  
  
  //
  // define a material from elements.   case 1: chemical molecule
  //
  
  G4Material* H2O = 
    new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);

  /*
  // Alternative Scintillator
  G4Material* Sci = 
    new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
  Sci->AddElement(C, natoms=9);
  Sci->AddElement(H, natoms=10);
  */  

  G4Material* Myl = 
    new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
  Myl->AddElement(C, natoms=10);
  Myl->AddElement(H, natoms= 8);
  Myl->AddElement(O, natoms= 4);
  
G4Material* SiO2 = 
  new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
 SiO2->AddElement(Si, natoms=1);
 SiO2->AddElement(O , natoms=2);
 
 //
 // define a material from elements.   case 2: mixture by fractional mass
 //

 G4Material* Air = 
   new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
 Air->AddElement(N, fractionmass=0.7);
 Air->AddElement(O, fractionmass=0.3);
 
 //
 // define a material from elements and/or others materials (mixture of mixtures)
 //
 
 G4Material* Aerog = 
   new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
 Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
 Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
 Aerog->AddElement (C   , fractionmass= 0.1*perCent);
 
 //
 // examples of gas in non STP conditions
 //
 
 G4Material* CO2 = 
   new G4Material("CarbonicGas", density= 1.842*mg/cm3, ncomponents=2,
		  kStateGas, 325.*kelvin, 50.*atmosphere);
 CO2->AddElement(C, natoms=1);
 CO2->AddElement(O, natoms=2);
 
 G4Material* steam = 
   new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
		  kStateGas, 500.*kelvin, 2.*atmosphere);
 steam->AddMaterial(H2O, fractionmass=1.);
 
 //
 // examples of vacuum
 //
 
 G4Material* Vacuum =
   new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
		  kStateGas, 2.73*kelvin, 3.e-18*pascal);
 
 G4Material* beam = 
   new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
		  kStateGas, STP_Temperature, 2.e-2*bar);
 beam->AddMaterial(Air, fractionmass=1.);
 
//default materials of the World
 defaultMaterial  = Vacuum;
 
 //materials of the calorimeter
 AbsorberMaterial   = Pb;


 //
 // or use G4-NIST materials data base
 //
 G4NistManager* man = G4NistManager::Instance();
 
 //Scintillator material
 G4Material* Sci = man->FindOrBuildMaterial("G4_POLYSTYRENE");
  
 SciMaterial = Sci;



// print table (if we want all the list)
//
// G4cout << *(G4Material::GetMaterialTable()) << G4endl;

 G4cout<<"<SBSMaterials>:Loaded"<<G4endl;
}

SBSMaterials::~SBSMaterials()
{
  //  if (verbose > 1)      
    G4cout << "<SBSMaterials~> END>" << G4endl; 
}
