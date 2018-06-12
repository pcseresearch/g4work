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
// $Id: PrimaryGeneratorAction.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SBSPrimaryGeneratorAction.hh"

#include "SBSDetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "SBSVariables.hh"
#include "SBSConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSPrimaryGeneratorAction::SBSPrimaryGeneratorAction()
{
 
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  Detector = (SBSDetectorConstruction*)
             G4RunManager::GetRunManager()->GetUserDetectorConstruction();  
  
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(Variables->EnergyBeam*MeV);

  G4double position = -0.5*(Constants->WorldSizeX);

  G4ThreeVector PartPosDef(0.*cm,0.*cm,position);
  particleGun->SetParticlePosition(PartPosDef);


  rndmFlag = "off";

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SBSPrimaryGeneratorAction::~SBSPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
  G4cout<<"SBSPrimaryGeneratorAction::~PrimaryGeneratorAction()->Finished"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SBSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //this function is called at the begining of event
    //THIS IS THE PLACE WHERE GENERATOR HAPPEN
    //Previous is default
    // 

    switch (Variables->ReactionCase)
	{
	case 1:
	{
	    //  Simple Gun, position of target in SBS.ini
	     G4ThreeVector PartPosRnd = (Variables->Target)*mm;//position
	     if(Variables->Verbose == 1)
	     {
		 G4cout<<"PartPosRnd: "<<PartPosRnd<<G4endl;
	     }
	    particleGun->SetParticlePosition(PartPosRnd);
	    particleGun->GeneratePrimaryVertex(anEvent);
	   
	    
	    
	
	 break;
	}

	case 2:
	{
	    
	    EnergyBeam=Variables-> EnergyBeam;
	    mProton = Constants->ProtonMass;//Ok, it can be calculated here, but is more for estetic
	    
	    
            // We are considering that the scatered electron as an "input", i.e. from its random generation, we determine the proton generation. Perhaps in the other way (proton->electron) the formula are simpler...	    
	    
// the angle input for the formula is only theta, scattering angle. The 3D distribution comes from the emission vector with a random phi. 
	    
	    //...ooo000ooo...
	    //elastic electron angle parameters as input for elastic formula
	    //...ooo000ooo...
	    
	    theta = G4UniformRand()* 90.*deg;//scattering angle between 0-90 deg
	    phi   = G4UniformRand()* 180.*deg -90.*deg;//phi=[-90,90]deg  choosing only one plane for a particle, the other will be phi+180 (the other plane)
	
	    //...ooo000ooo...
	    //this formula is in case we use cartesian or square angles, i.e. theta in plane xy and phi in plane xy.

	    // spherical emission angles calculated from its cartesian angles

//	    Theta_sca = acos (1/(sqrt(sqr(tan(phi)) +sqr(tan(theta)) +1)));//scattering angle in spherical
    

	    //...ooo000ooo...
	    //elastic scattering formula
	    //...ooo000ooo...
	    
//	    EleMomSca = (mp*EnergyBeam)/(EnergyBeam+mp-(EnergyBeam*cos(Theta_sca)));
	    EleMomSca = (mProton*EnergyBeam)/(EnergyBeam+mProton-(EnergyBeam*cos(theta)));
	    ProEneSca = EnergyBeam+mProton-EleMomSca;
	    ProMomSca = sqrt(sqr(ProEneSca)-sqr(mProton));
	    ProMomAng = acos((EnergyBeam*(EnergyBeam+mProton-EleMomSca)-EleMomSca*mProton)/(EnergyBeam*ProMomSca));
	    
	    //...ooo000ooo...
	    
	    //...ooo000ooo...
	    //proton elastic angles in cartesian
	    //...ooo000ooo...
	    
//	    phi_pro = -asin((EleMomSca/ProMomSca)*sin(phi));//momentum conservation (CHECK SIGN!!)
	    
//	    theta_pro = -asin((EleMomSca/ProMomSca)*sin(theta));//alternative way using momentum conservation
	    

	    
	    //...ooo000ooo...
	    
	    
	    
	    //RTA: ReaktionTeilchenAngle
	    //RTM: ReaktionTeilchenMomentum
	    //proton:0, electron:1
	    
	    Ang_Para[0] = ProMomAng;
	    Mom_Para[0] = ProMomSca;
      
	    Ang_Para[1] = theta;
	    Mom_Para[1] = EleMomSca;
	    
	    Phi_Para[0] = phi+180*deg;// the complementary plane to the scattering electron plane
	    Phi_Para[1] = phi;

      
	    for (G4int i=0;i<2;i++)//only two particles for the moment, should be generalized
		{
		    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		    G4ParticleDefinition* particle = particleTable->FindParticle(Variables->ReactionParticle[i]);
		    //particleGun->SetParticleDefinition(particleTable->FindParticle("e-"));
		    
		    particleGun->SetParticleDefinition(particle);
		    particleGun->SetParticleMomentum(Mom_Para[i]*MeV);
		    
		    //...ooo000ooo...
		    //distribution along the plane
		    //...ooo000ooo...
		    
		    //particleGun->SetParticleMomentumDirection(G4ThreeVector(sin(EmmAng[i]),0,cos(EmmAng[i])));
		    //...ooo000ooo...
		    
		    //...ooo000ooo...
		    //cartesian vector for emission in 3D
		    //...ooo000ooo...
//		    G4ThreeVector RanDis(sin(ThetaAng[i]),sin(PhiAng[i]),cos(RTA[i]));

		    G4ThreeVector RanDis(sin(Ang_Para[i])*cos(Phi_Para[i]),sin(Ang_Para[i])*sin(Phi_Para[i]),cos(Ang_Para[i]));
		    
		    //ATTENTION!! This vector is not normalized and can give some problem later if it is not normalized
		    
//		    G4ThreeVector RanDisUni = RanDis/RanDis.mag();
		    
//		    G4ThreeVector RanDisUniRot = RanDisUni;
//		    RanDisUniRot.rotateY(-SpecKAng);//correction by Lars, since rotateY() returns void value
		    //this rotation should be done because in Geant, Kaos is at 0deg
		    
		    //G4cout << "PGA: Direction Vector " << RanDisUni<< G4endl;
		    
		    particleGun->SetParticleMomentumDirection(RanDis);
		    particleGun->GeneratePrimaryVertex(anEvent);
		    //...ooo000ooo...

		 }
	    break;
	}	    



	case 3://angular randon scan (I have to check, if I wanted more complicated, to make functions)
		{
		    G4ThreeVector PartPosRnd = (Variables->Target);
		    Dist = abs(PartPosRnd.z());
		    RndScaAng = (atan((((Variables->CalModSizeXY)*
					//	(Variables->NbOfCaloRows)
					1)/2)
				      /Dist));
//we assure that the aperture of the cone is restricted to the size of the calorimeter (in height),
//with the target centered at 0,0
		    
		    theta = G4UniformRand()* RndScaAng;//scattering angle 
		    phi = G4UniformRand()* 360.*deg;//phi 	    

		    G4ThreeVector RndDir(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
		    
		    if(abs((Dist*tan(theta))*cos(phi))/cm > 6.||abs((Dist*tan(theta))*sin(phi))/cm > 6.)
		    {
			cout<<"X: "<<Dist*(tan(theta))*cos(phi)/cm<<" Y: "<<Dist*(tan(theta))*sin(phi)/cm<<endl;
			cout<<"Event: "<<anEvent->GetEventID();
		    }
		    
		    particleGun->SetParticlePosition(PartPosRnd);
		    particleGun->SetParticleMomentumDirection(RndDir);
		    particleGun->GeneratePrimaryVertex(anEvent);
		    
		    break;
		}   
		

	case 4://randon scan 
		{
		    G4ThreeVector PartPosRnd = (Variables->Target);
		    
		    G4cout<<"Dist: "<<Dist<<" CalModSizeXY/2: "<<(Variables->CalModSizeXY)/2<<G4endl;
		   
		    Columns=Variables->NbOfCaloColumns;
		    Rows=Variables->NbOfCaloRows;
		    
		    MaxAperY = ((Variables->CalModSizeXY)*(Rows));
		    MaxAperX = ((Variables->CalModSizeXY)*(Columns));
		    
		    Xrnd = G4UniformRand()* MaxAperX - MaxAperX/2;
		    Yrnd = G4UniformRand()* MaxAperY - MaxAperY/2; 	    

		    G4ThreeVector RndDir(Xrnd,Yrnd,0);
		    G4cout<<"RndScaAng: "<<RndScaAng<<"theta: "<<theta<<" RndDir: "<<RndDir<<G4endl;
		    
		    particleGun->SetParticlePosition(PartPosRnd+RndDir);

		    particleGun->GeneratePrimaryVertex(anEvent);
		    break;
		    
		}   
	
	case 5://rectangular randon scan 
		{
		    G4ThreeVector PartPosRnd = (Variables->Target);
		    Dist = abs(PartPosRnd.z());
		    ModHalfSide=(Variables->CalModSizeXY)/2;
		    
		    RndAptX = (G4UniformRand()*ModHalfSide*2)-ModHalfSide;
		    RndAptY = (G4UniformRand()*ModHalfSide*2)-ModHalfSide;

//we assure that the aperture of the cone is restricted to the size of the calorimeter (in height),
//with the target centered at 0,0
		       
		    G4ThreeVector RndDir(tan(RndAptX/Dist),tan(RndAptY/Dist),1 );
		    
		    particleGun->SetParticlePosition(PartPosRnd);
		    particleGun->SetParticleMomentumDirection(RndDir);
		    particleGun->GeneratePrimaryVertex(anEvent);
		    
		    break;
		}
	case 6://multi e-
		{
		//Simple Gun, position of target in SBS.ini

		double max_elec;
		int number_e;

		max_elec=4;
		double erand = G4UniformRand();
		if(max_elec==4){
//	    		cout<<expdecay<<" = "<<number_e<<endl;
			if(erand>=0.0 && erand<0.614791){
				number_e=1;
			}
			if(erand>=0.614791 && erand<0.905301){
				number_e=2;
			}
			if(erand>=0.905301 && erand<0.982764){
				number_e=3;
			}
			if(erand>=0.982764 && erand<1.0){
				number_e=4;
			}
		}//close if max_e=4
		
		if(max_elec==6){
//	    		cout<<expdecay<<" = "<<number_e<<endl;
			if(erand>=0.0 && erand<0.476281){
				number_e=1;
			}
			if(erand>=0.476281 && erand<0.796537){
				number_e=2;
			}
			if(erand>=0.796537 && erand<0.936849){
				number_e=3;
			}
			if(erand>=0.936849 && erand<0.985388){
				number_e=4;
			}
			if(erand>=0.985388 && erand<0.996898){
				number_e=5;
			}
			if(erand>=0.996898 && erand<1.0){
				number_e=6;
			}
		}//close if max_e=6
		
	    G4ThreeVector PartPosRnd = (Variables->Target)*mm;//position
		if(Variables->Verbose == 1){
			G4cout<<"PartPosRnd: "<<PartPosRnd<<G4endl;
		}
		particleGun->SetNumberOfParticles(number_e);
		particleGun->SetParticlePosition(PartPosRnd);
		particleGun->GeneratePrimaryVertex(anEvent);

    
		   
		
		}//close case 6 
	
	}//close switch
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

