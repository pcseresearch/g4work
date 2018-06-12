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
// $Id: DetectorConstruction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4SubtractionSolid.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "SBSVariables.hh"
#include "SBSConstants.hh"
#include "ECalDetectorConstruction.hh"
#include "SBSMaterials.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Perhaps we should include the run Manager as parameter to the class, since it is derived from SBSDetectorConstructio, but for the moment without parameters

ECalDetectorConstruction::ECalDetectorConstruction(G4VPhysicalVolume *physiParent, SBSMaterials *Materials)

{
    G4cout<<"<ECalDetectorConstruction::ECalDetectorConstruction>: Creating Calorimeter"<<G4endl;
    
//number of holes in the calorimeter tiles (should go to .ini for flexibility)
    TubColumns=12;
    TubRows=12;

//The Calorimeter as a block with the World as parent
//All distances are given in mm
    { 
	Columns=Variables->NbOfCaloColumns;
	Rows=Variables->NbOfCaloRows;
	
	CalorY = ((Variables->CalModSizeXY)*(Rows));
	CalorX = ((Variables->CalModSizeXY)*(Columns));
	
	//check size
	solidCalor = new G4Box("SolidCalorimeter",		//its name
			       CalorX/2+1*mm,
			       CalorY/2+1*mm,
			       Constants->CalorThickness/2+1*mm);  

//size of the calorimeter a little bit greater than the size of modules. If I restrict to the size of the modules, the identification of the volume becomes more difficult. Geant4 directly identify the lower volume, in this case the gap, since there are not space where the event makes a step
	
	
	logicCalor = new G4LogicalVolume(solidCalor,	//its solid
					 Materials->defaultMaterial,	//its material
					 "LogicCalorimeter");	//its name
	

//The spacial position of the calorimeter
	Rota = Variables->EleAng;//angle of the position of ECal (check the units, should be deg)
	RSRot.rotateX(-Rota);//Rotation matrix of the calorimeter in order to face the scattered particles perpendicularly  
	G4ECalDis = Variables->ECalDis + Constants->CalorThickness/2; //the distance from target to the front face of the calorimeter corrected with the position in Geant4 of the calorimeter (in the center of mass of the detector)


	physiCalor = new G4PVPlacement(G4Transform3D(RSRot,G4ThreeVector(0,G4ECalDis * sin(Rota),G4ECalDis *cos(Rota))),
				       "Calorimeter",
				       logicCalor,  
				       physiParent,       //its mother  volume(passed)
				       false,		//no boolean operation
				       0);	        //copy numbe

    }
    


//The modules of the calorimeter with the Calorimeter as parent 
// because the world can not be used as slice 

    if (Constants->CalorThickness > 0.)  
    { 
	solidCaloMod = new G4Box("SolidCaloModul",		//its name
				 Variables->CalModSizeXY/2,
				 Variables->CalModSizeXY/2,//size
				 Constants->CalorThickness/2);
	  
	logicCaloMod = new G4LogicalVolume(solidCaloMod,	//its solid
					   Materials->defaultMaterial,	//its material
					   "LogicCaloModul");	//its name
	  
	
	
//The offset is applied to set the Center of the calorimeter at (0,0,0)
	G4ThreeVector offset(-(CalorX-(Variables->CalModSizeXY))/2,-(CalorY-(Variables->CalModSizeXY))/2,0);   
	
	for (G4int j=0;j<Rows;j++)
	{
	    for (G4int i=0;i<Columns;i++)	      
	    {
		physiCaloMod = new G4PVPlacement
		    (0,		//no rotation
//		     offset + G4ThreeVector((Variables->CalModSizeXY)*j,(Variables->CalModSizeXY)*i,0),	//at (0,0,0)
		     offset + G4ThreeVector((Variables->CalModSizeXY)*i,(Variables->CalModSizeXY)*j,0),	//at (0,0,0)
		     "CaloModul",	//its name
		     logicCaloMod,	//its logical volume
		     physiCalor,       //its mother  volume(passed)
		     false,		//no boolean operation
		     (j*Columns)+i);	        //copy number
		
		
//with this numeration units make columns, and tenths make rows
//e.g. with 15 columns 
//0-1-2-3-4-5-6-7-8-9-10-11-12-13-14... first row (i=0->14;j=0*columns)
//15-16-17-18-19-20-21-22-23-24-25-26-27-28-29... second row (i=1->14;j=1*colums)
		
		
	    }
	}
    }
    
    
    //***************************************************************                                 
    // Layer
    //The calorimeter is formed by layers
    //each layer consists in absorber and gap
    //generally absorber=Pb and gap=scintillator
    //***************************************************************                                 
    
    solidLayer = new G4Box("SolidLayer",			//its name
			   Variables->CalModSizeXY/2,
			   Variables->CalModSizeXY/2, //size
			   Constants->LayerThickness/2);
      
    logicLayer = new G4LogicalVolume(solidLayer,	//its solid
				     Materials->defaultMaterial,	//its material
				     "LogicLayer");	//its name
      
    physiLayer = new G4PVReplica("Layer",		//its name
				 logicLayer,	//its logical volume
				 logicCaloMod,	//its mother
				 kZAxis,		//axis of replication
				 Variables->NbOfLayers,	//number of replica
				 Constants->LayerThickness);	//witdth of replica
 

   //***************************************************************
   //                              Absorber
   //***************************************************************                                 
      solidAbsorber=0; logicAbsorber=0; physiAbsorber=0;  

      if (Variables->LeadThickness > 0.) 
      { 
	  solidAbsorber = new G4Box("SolidAbsorber",
			       Variables->CalModSizeXY/2,
			       Variables->CalModSizeXY/2,
			       Variables->LeadThickness/2);

	  logicAbsorber = new G4LogicalVolume(solidAbsorber,    //its solid
						Materials->AbsorberMaterial, //its material
						"LogicAbsorber"); //name


	  physiAbsorber = new G4PVPlacement(0,		   //no rotation
					    G4ThreeVector(0.,0.,Variables->ScintThickness/2),  //its position
					    logicAbsorber,     //its logical volume		    
					    "Absorber", //its name
					    logicLayer,        //its mother
					    false,             //no boulean operat
					    0);                //copy number
	  

	  if(Variables->Fibers)
	  { 
	      //***************************************************************
	      //                       Absorber Holes
	      //***************************************************************
	      
	      solidAbsTub = new G4Tubs("AbsorberTub",		//its name
				       0,
				       0.75*mm,//Hera-B nominal value
				       Variables->LeadThickness/2,
				       0*deg,				    
				       360.*deg);      
	      
	      logicAbsTub = new G4LogicalVolume(solidAbsTub,    //its solid
						Materials->defaultMaterial, //its material(vacuum, for the moment)
						"LogicAbsTub"); //name
	      
	      
	      
	      for (G4int j=0;j<TubRows;j++)
	      {
		  for (G4int i=0;i<TubColumns;i++)	      
		  {
		      physiAbsTub = new G4PVPlacement
			  (0,		//no rotation
//		     offset + G4ThreeVector((Variables->CalModSizeXY)*j,(Variables->CalModSizeXY)*i,0),	//at (0,0,0)
			   G4ThreeVector(9.25*mm*i-50.825*mm,9.25*mm*j-50.825*mm,0),	//at (0,0,0)
			   "AbsTubs",	//its name
			   logicAbsTub,	//its logical volume
			   physiAbsorber,      //its mother  volume(passed)
			   false,		//no boolean operation
			   (j*Columns)+i);	        //copy number
		      
//same as the numeration of the modules		
//with this numeration units make columns, and tenths make rows
//e.g. with 15 columns 
//0-1-2-3-4-5-6-7-8-9-10-11-12-13-14... first row (i=0->14;j=0*columns)
//15-16-17-18-19-20-21-22-23-24-25-26-27-28-29... second row (i=1->14;j=1*colums)
		      
		      
		  }
	      }
	      
	      //***************************************************************
	      //                       Absorber WLS
	      //***************************************************************
	      
	      solidAbsWLS = new G4Tubs("AbsWLS",		//its name
				       0,
				       0.6*mm,
				       Variables->LeadThickness/2,
				       0*deg,				    
				       360.*deg);      
	      
	      logicAbsWLS = new G4LogicalVolume(solidAbsWLS,    //its solid
						Materials->defaultMaterial, //its material(vacuum, for the moment)
						"LogicAbsWLS"); //name
	      
	      
	      physiAbsWLS = new G4PVPlacement(0,		//no rotation
					      G4ThreeVector(0,0.,0.),	//at (0,0,0)
					      "WLSAbs",	//its name
					      logicAbsWLS,	//its logical volume
					      physiAbsTub,      //its mother  volume(passed)
					      false,		//no boolean operation
					      0);	        //copy number
	      
	  }//END -->if(Variables->Fibers)<-- (Absorber Part)
      
	  
      }//END Absorber creation

      //***************************************************************
      //                        Scintillator (Gap)
      //***************************************************************
      solidGap=0; logicGap=0; physiGap=0; 
      
      if (Variables->ScintThickness > 0.)
      { 
	  solidGap = new G4Box("SolidGap",
			       Variables->CalModSizeXY/2,
			       Variables->CalModSizeXY/2,
			       Variables->ScintThickness/2);
	  
	  
	  logicGap = new G4LogicalVolume(solidGap,
					 Materials->SciMaterial,
					 "LogicGap");
      
	  physiGap = new G4PVPlacement(0,                      //no rotation
				       G4ThreeVector(0.,0.,-Variables->LeadThickness/2),   //its position
				       logicGap,               //its logical volume	       
				       "Gap", //its name
				       logicLayer,             //its mother
				       false,                  //no boulean operat
				       0);                     //copy number



	  if(Variables->Fibers)
	  {
	      //***************************************************************
	      //                       Scintillator (gap) Holes
	      //***************************************************************
	      
	      solidGapTub = new G4Tubs("GapTub",		//its name
				       0,
				       0.75*mm,//Hera-B nominal value
		
				       Variables->ScintThickness/2,
				       0*deg,				    
				       360.*deg);      
	      
	      logicGapTub = new G4LogicalVolume(solidGapTub,    //its solid
						Materials->defaultMaterial, //its material(vacuum, for the moment)
						"LogicGapTub"); //name
	  
	      

	      for (G4int j=0;j<TubRows;j++)
	      {
		  for (G4int i=0;i<TubColumns;i++)	      
		  {
		      physiGapTub = new G4PVPlacement
			  (0,		//no rotation
//		     offset + G4ThreeVector((Variables->CalModSizeXY)*j,(Variables->CalModSizeXY)*i,0),	//at (0,0,0)
			   G4ThreeVector(9.25*mm*i-50.825*mm,9.25*mm*j-50.825*mm,0),	//at (0,0,0)
			   "GapTubs",	//its name
			   logicGapTub,	//its logical volume
			   physiGap,      //its mother  volume(passed)
			   false,		//no boolean operation
			   (j*Columns)+i);	        //copy number
		      
//same as the numeration of the modules		
//with this numeration units make columns, and tenths make rows
//e.g. with 15 columns 
//0-1-2-3-4-5-6-7-8-9-10-11-12-13-14... first row (i=0->14;j=0*columns)
//15-16-17-18-19-20-21-22-23-24-25-26-27-28-29... second row (i=1->14;j=1*colums)
		      
		      
		  }
	      }
	      
	      //***************************************************************
	      //                       Scintillator (gap) WLS
	      //***************************************************************
	      
	      solidGapWLS = new G4Tubs("GapWLS",		//its name
				       0,
				       0.6*mm,
				       Variables->ScintThickness/2,
				       0*deg,				    
				       360.*deg);      
	      
	      logicGapWLS = new G4LogicalVolume(solidGapWLS,    //its solid
						Materials->defaultMaterial, //its material(vacuum, for the moment)
						"LogicGapWLS"); //name
	      
	      
	      physiGapWLS = new G4PVPlacement(0,		//no rotation
					      G4ThreeVector(0,0.,0.),	//at (0,0,0)
					      "WLSGap",	//its name
					      logicGapWLS,	//its logical volume
					      physiGapTub,      //its mother  volume(passed)
					      false,		//no boolean operation
					      0);	        //copy number
	      
	  }//END -->if(Variables->Fibers)<-- (Scintillator part)
	  
      }//End Scintillator creation
  

      
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//                        Summary of the calorimeter
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << Variables->NbOfLayers << " layers of: [ "
            << Variables->LeadThickness/mm << "mm of " << Materials->AbsorberMaterial->GetName() 
         << " + "
         << Variables->ScintThickness/mm << "mm of " << Materials->SciMaterial->GetName() << " ] " 
         << "With a cross section per module of: " << Variables->CalModSizeXY /mm <<"mm"

	 <<"\n------------------------------------------------------------\n";
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //                          Visualization attributes
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  // logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);

  G4VisAttributes* TubVisAtt = new G4VisAttributes(G4Colour(0.5,0.5,.5));//Gray

  //in case we want to debug the fibers
  //  G4VisAttributes* WLSAbsVisAtt = new G4VisAttributes(G4Colour(0.6,0.,0.6,0.5));//abs wls

  G4VisAttributes* WLSVisAtt = new G4VisAttributes(G4Colour(0.,1.,0.));//Green
  WLSVisAtt->SetForceSolid(true);

  G4VisAttributes* GapVisAtt = new G4VisAttributes(G4Colour(0.1,0.5,.9));//kind a blue
  G4VisAttributes* AbsVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));//yellow
  G4VisAttributes* LayVisAtt = new G4VisAttributes(G4Colour(1.,0.,1.));//magenta

  //Visualitation of the layers (switch in SBS.ini file)
  if(Variables->ECalDetail)
    { 
      //Layers visible (modules and calorimeter box invisible)
      logicGap->SetVisAttributes(GapVisAtt);
      logicAbsorber->SetVisAttributes(AbsVisAtt);
      
      if(Variables->Fibers)
      {
	  //  logicGapTub ->SetVisAttributes(G4VisAttributes::Invisible); 
	  //  logicAbsTub ->SetVisAttributes(G4VisAttributes::Invisible); 
	  
	  logicGapTub ->SetVisAttributes(TubVisAtt);
	  logicAbsTub ->SetVisAttributes(TubVisAtt);
	  
	  logicAbsWLS ->SetVisAttributes(WLSVisAtt);
	  logicGapWLS ->SetVisAttributes(WLSVisAtt);
      }

      logicCaloMod->SetVisAttributes(G4VisAttributes::Invisible);
      logicCalor  ->SetVisAttributes(G4VisAttributes::Invisible); 
    }
  else
    {
      //Layers invisible (only the calorimeter box and modules)
      logicCalor   ->SetVisAttributes(simpleBoxVisAtt);  
      logicCaloMod ->SetVisAttributes(LayVisAtt);

      if(Variables->Fibers)
      {
	  logicGapTub  ->SetVisAttributes(G4VisAttributes::Invisible); 
	  logicAbsTub  ->SetVisAttributes(G4VisAttributes::Invisible); 
      
	  logicAbsWLS  ->SetVisAttributes(G4VisAttributes::Invisible); 
	  logicGapWLS  ->SetVisAttributes(G4VisAttributes::Invisible); 
      }      

      logicGap     ->SetVisAttributes(G4VisAttributes::Invisible); 
      logicAbsorber->SetVisAttributes(G4VisAttributes::Invisible); 
      logicLayer   ->SetVisAttributes(G4VisAttributes::Invisible);
      
    }
  
  

 /*
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
  // Should be implemented with an external option so it is not necessary compile
  //every time

 {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  simpleBoxVisAtt->SetVisibility(true);
  delete logicCalor->GetVisAttributes();
  logicCalor->SetVisAttributes(simpleBoxVisAtt);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicLayer->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  logicAbsorber->SetVisAttributes(atb);}
  
 {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
  atb->SetForceSolid(true);
  logicGap->SetVisAttributes(atb);}
 */
  
  
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ECalDetectorConstruction::~ECalDetectorConstruction()
{ 
    G4cout<<"<ECalDetectorConstruction::~ECalDetectorConstruction()>: Finished"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

