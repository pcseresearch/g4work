#include <iostream>
#include <fstream>

const int bin = 100; 
const int nMod = 9;
const Int_t nLayer = 20;


void sim_plot() {


	//Declaration of leaves types
	//TBranch *hit;   
	Int_t EventIDhit;
	Int_t Module;
	Float_t EDepMod;
	Int_t Layer;
	Float_t EDepLay;
	Float_t EDep;
	
	//TBranch *event;   
	Int_t EventID;
	Float_t EDepTot;
	Float_t PosCal[3];
	
	//TBranch *set;   
	Int_t runID;
	Int_t noCol;
	Int_t noRow;

//	 
//Read files and tree branches ECalFile-2015-03-09-11-16-10.root
//	TFile *froot =  new TFile("./ECalFile_12GeV_offset.root"); //accesses root file
	TFile *froot =  new TFile("./simdata/ECalFile_3gev_52_-52_10000_error.root"); //accesses root file
	TTree *hittree = (TTree*)froot->Get("hittree"); //accessess the individual tree
	TTree *eventtree = (TTree*)froot->Get("eventtree"); //accessess the individual tree
	TTree *settree = (TTree*)froot->Get("settree"); //accessess the individual tree

//Set Leaf Addresses
	TLeaf *hlevent = hittree->GetLeaf("EventID");
	hlevent->SetAddress(&EventIDhit);
	TLeaf *hlmodule = hittree->GetLeaf("Module");
	hlmodule->SetAddress(&Module);
	TLeaf *hledepmod = hittree->GetLeaf("EDepMod");
	hledepmod->SetAddress(&EDepMod);
	TLeaf *hllayer = hittree->GetLeaf("Layer");
	hllayer->SetAddress(&Layer);
	TLeaf *hledeplay = hittree->GetLeaf("EDepLay");
	hledeplay->SetAddress(&EDepLay);
	TLeaf *hledep = hittree->GetLeaf("EDep");
	hledep->SetAddress(&EDep);
	TLeaf *elevent = eventtree->GetLeaf("EventID");
	elevent->SetAddress(&EventID);
	TLeaf *eledeptot = eventtree->GetLeaf("EDepTot");
	eledeptot->SetAddress(&EDepTot);
	TLeaf *elposcal = eventtree->GetLeaf("PosCal");
	elposcal->SetAddress(&PosCal);
	TLeaf *slrun = settree->GetLeaf("runID");
	slrun->SetAddress(&runID);
	TLeaf *slnocol =settree->GetLeaf("noCol");
	slnocol->SetAddress(&noCol);
	TLeaf *slnorow = settree->GetLeaf("noRow");
	slnorow->SetAddress(&noRow);
	

	const int nruns_in_file = (int)settree->GetEntries();
	const int nevents_in_file = (int)eventtree->GetEntries();
	const int nhits_in_file = (int)hittree->GetEntries();
	cout<<"nRuns: "<<nruns_in_file<<", nevents: "<<nevents_in_file<<", nhits: "<<nhits_in_file<<endl;
	
	//default histos
	TH1I *hEventIDhit, *hModule, *hLayer, *hEventID, *hRunID, *hNoCol, *hNoRow;
	TH1F *hEDepMod, *hEDepLay, *hEDep, *hEDepTot, *hPosCal;
	//run histos
	hRunID = new TH1I("hrunID","number of runs",10,0,10);
	hNoCol = new TH1I("hnoCol","number of columns",10,0,10);
	hNoRow = new TH1I("hnoRow","number of rows",10,0,10);
	//event histos
	hEventID = new TH1I("hEventID","number event",nevents_in_file,0,nevents_in_file);
	hEDepTot = new TH1F("hEDepTot","energy deposited total", 900, 1000, 5000); //total energy deposited
	hPosCal = new TH1F("hPosCal","PosCol",1000,0,1000);
	//hit histos
	hEventIDhit = new TH1I("hEventIDhit","number event",nevents_in_file,0,nevents_in_file);
	hModule = new TH1I("hModule","module hit", 10,0,10);
	hLayer = new TH1I("hLayer","layer Hit",50,0,50);
	hEDepMod = new TH1F("hEDepMod","energy deposited in all scint",100,0,1000);
	hEDepLay = new TH1F("hEDepLay","layer energy deposited in",100,0,1000);
	hEDep = new TH1F("hEDep","amount of energy deposited",100,0,1000);
	
	//custom stuff
	float EDepModLayTot[nMod][nLayer];
	TH1F *hEDepModLayTot[nMod], *hEDepModarray[nMod], *hEDepModarrayRootIndex[nMod];
	double maxbin = 5800;
	for(int i=0;i<nMod;i++){
		hEDepModLayTot[i] = new TH1F(Form("hedepmodlaytot_%i",i),"i dunno yet",1000,0,maxbin);
		hEDepModarray[i] = new TH1F(Form("hedepmodarray_%i",i),"Total Energy Deposited in Scint",100,0,maxbin);
		hEDepModarrayRootIndex[i] = new TH1F(Form("hedepmodarrayrootindex_%i",i),"Total Energy Deposited in Scint",580,0,maxbin);
		for(int j=0;j<nLayer;j++){
			EDepModLayTot[i][j] = 0.0;
			
		}
	}

	
	//event loops
	for(int nrun = 0; nrun<nruns_in_file;nrun++){
		settree->GetEntry(nrun);
		hRunID->Fill(runID);
		hNoCol->Fill(noCol);
		hNoRow->Fill(noRow);	
	}	
	for (int nevent = 0; nevent < nevents_in_file; nevent++){ 
			eventtree->GetEntry(nevent);
			hEventID->Fill(EventID);
			hEDepTot->Fill(EDepTot);
			hPosCal->Fill(PosCal[0]);hPosCal->Fill(PosCal[1]);hPosCal->Fill(PosCal[2]);
			
			
	}
	const int nev = nevents_in_file;
	double energyinmod[nev][nMod];
	for(int i=0;i<nevents_in_file;i++){
		for(int j=0;j<nMod;j++){
			energyinmod[i][j]=0.0;
		}
	}

	for (int nhit = 0; nhit < nhits_in_file; nhit++){ 
		hittree->GetEntry(nhit);
		hEventIDhit->Fill(EventIDhit);
		hModule->Fill(Module);
		hLayer->Fill(Layer);
		hEDepMod->Fill(EDepMod);
		hEDepLay->Fill(EDepLay);
		hEDep->Fill(EDep);
		int rootindex = Module;
		if(Module>-1&&Layer>-1){
			EDepModLayTot[Module][Layer] += EDepMod;
			hEDepModarray[Module]->Fill(EDepMod);
			hEDepModLayTot[Module]->Fill(EDepLay);
			
			if(rootindex<=2){
				rootindex+=6;
				hEDepModarrayRootIndex[rootindex]->Fill(EDepMod);
				energyinmod[EventIDhit][rootindex] += EDepMod;
				rootindex-=6;
			}
			if(rootindex>=6){
				rootindex-=6;
				hEDepModarrayRootIndex[rootindex]->Fill(EDepMod);
				energyinmod[EventIDhit][rootindex] += EDepMod;
				rootindex+=6;	
			}
			if(rootindex>2&&rootindex<6){
				hEDepModarrayRootIndex[rootindex]->Fill(EDepMod);
				energyinmod[EventIDhit][rootindex] += EDepMod;
			} 
			
		}
		
		
		
	}
	
	for(int i=0;i<nMod;i++){
		for(int j=0;j<nLayer;j++){
//			cout<<EDepModLayTot[i][j]<<endl;
//			hEDepModLayTot[i]->Fill(EDepModLayTot[i][j]);
		}
	}
	TCanvas *c1 = new TCanvas("c1","Geo Display",65,65,800,800);
	c1->Divide(3,3);
	for(int i=1;i<=nMod;i++){
		c1->cd(i);
		hEDepModarrayRootIndex[i-1]->Draw();
	}
//	c1->Close();


	// 1	2	3 +6
	// 4	5	6
	// 7	8	9 -6
	
	// 	7	8	9
	// 	4	5	6	
	//	1	2	3 


	TCanvas *c3 = new TCanvas("c3","Total Energy",90,90,800,800);
	hEDepTot->Draw();
//	c3->Close();
	

//#########################################################################
//*************************************************************************
//#########################################################################


	double holdesum,holdxpos,holdypos;
 	TH1F *xposition = new TH1F("xposition","X Position",300,-6.0,6.0);
	TH1F *yposition = new TH1F("yposition","Y Position",300,-6.0,6.0);
	TH1F *eupdown = new TH1F("eupdown","E Up Down",300,-0.5,0.5);
	TH1F *eleftright = new TH1F("eleftright","E Left Right",300,-0.5,0.5);
	TH2F *xyposition = new TH2F("xyposition","XY Position",300,-6.0,6.0,300,-6.0,6.0);

	for (int ie = 0; ie < nevents_in_file; ie++) {
		holdesum=0.0;
		holdxpos=0.0;
		holdypos=0.0;
		double energy = 300.0;
		double deltae = 400.0;

		//sigmas are manual correction factors.
		double sigmaxa=1.00; //for left XOR right 
		double sigmaya=1.00;	//up XOR down 
		double sigmaxb=1.0;	//left AND right
		double sigmayb=1.0;	//up AND down

		double block_size=11.2;
		double pedthresh = 20.0;
		double xposcorr = 1.0;
		double xposoffset = 0.0;
		double yposcorr = 1.0;
		double yposoffset = 0.0;
		
		if(true){
		//if (holdesum > energy-deltae && holdesum < energy+deltae){
			double eleft =  energyinmod[ie][3];//+energyinmod[ie][0]+energyinmod[ie][6];
			double emid =   energyinmod[ie][4];
			double eright = energyinmod[ie][5];//+energyinmod[ie][2]+energyinmod[ie][8];
			double eupper = energyinmod[ie][1];//energyinmod[ie][0]+energyinmod[ie][2];
			double elower = energyinmod[ie][7];//+energyinmod[ie][6]+energyinmod[ie][8];
		//	cout << eleft << " " << eright << " " << emid << " " << eupper << " " << elower << endl;
		//  cout << eleft+emid+eright << " " << eleft-eright << endl;
		//  cout << eupper+elower+emid << " " << eupper-elower << endl << endl;
		//  holdxpos=(eright-eleft)/abs(eright-eleft)*pow(abs(eleft-eright)/(eleft+emid+eright),0.5)*xposcorr+xposoffset;
		//  holdypos=(eupper-elower)/abs(eupper-elower)*pow(abs(elower-eupper)/(eupper+emid+elower),0.5)*yposcorr+yposoffset;
		//
		// new position calculation!!	
		
			
			if(eleft < pedthresh && eright > pedthresh && emid > pedthresh) {
					eleftright->Fill((block_size/2.0-sigmaxa*log(0.5*(emid/eright)+1.0)));
				holdxpos=((block_size/2.0-sigmaxa*log(0.5*(emid/eright)+1.0))*xposcorr+xposoffset  );
			}else if (eleft > pedthresh && eright < pedthresh && emid > pedthresh) {
					eleftright->Fill((-1.0*block_size/2.0+sigmaxa*log(0.5*(emid/eleft)+1.0)));
				holdxpos=((-1.0*block_size/2.0+sigmaxa*log(0.5*(emid/eleft)+1.0))*xposcorr+xposoffset  );
			}else if (eleft > pedthresh && eright > pedthresh) {
					eleftright->Fill( (sigmaxb/2.0*log(eleft/eright))  );
				holdxpos=((sigmaxb/2.0*log(eleft/eright))*xposcorr+xposoffset );
			}else{
				eleftright->Fill(0.0);
				holdxpos=0.0;
			}
	
			if(elower < pedthresh && eupper > pedthresh && emid > pedthresh) {
					eupdown->Fill((block_size/2.0-sigmaya*log(0.5*(emid/eupper)+1.0)));
				holdypos=((block_size/2.0-sigmaya*log(0.5*(emid/eupper)+1.0))*yposcorr+yposoffset  );
			}else if (elower > pedthresh && eupper < pedthresh && emid > pedthresh) {
					eupdown->Fill((-1.0*block_size/2.0+sigmaya*log(0.5*(emid/elower)+1.0)));
				holdypos=((-1.0*block_size/2.0+sigmaya*log(0.5*(emid/elower)+1.0))*yposcorr+yposoffset  );
			}else if (elower > pedthresh && eupper > pedthresh) {
					eupdown->Fill( (sigmayb/2.0*log(elower/eupper))  );
				holdypos=((sigmayb/2.0*log(elower/eupper))*yposcorr+yposoffset  );
			}else{
				eupdown->Fill(0.0);
				holdypos=0.0;
			}

			xposition->Fill(holdxpos);
			yposition->Fill(holdypos);
			xyposition->Fill(holdxpos,holdypos);
		}  
	}//closes for(ie) loop
		TMarker *marker = new TMarker(5.0, -5.0 , 3);
		marker->SetMarkerColor(2);//red
		marker->SetMarkerSize(2);
//		marker->Draw();

	TCanvas *c4 = new TCanvas("c4","XYPosition",115,115,800,800);
	xyposition->Draw();
	marker->Draw();
//  	c4->Close();

//*/
}
