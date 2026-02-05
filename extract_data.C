void extract_data()
{
  Double_t Get_temp(Double_t,Double_t,Double_t);
	 ROOT::EnableImplicitMT();
	TH1D *velocity_hist =new TH1D("","Velocity distribution; velocity (km/s); Events",100,0,800);
        TH1D *theta_hist =new TH1D("","Theta distribution; #theta (rad); Events",100,0,TMath::Pi());
//        TH1D *phi_hist =new TH1D("","Phi distribution; #phi (rad); Events",100,0,2*TMath::Pi());
        TH1D *r_energy_hist =new TH1D("","Recoil energy distribution; Recoil energy (eV); Events",100,0,200);
//        TH1D *phononNo_hist =new TH1D("","Primary phonon number distribution; No. of phonons; Events",100,0,20000);
//        TH2D *xypos_hist=new TH2D("","Primary vertex distribution (X-Y); x (cm); y (cm);Events",100,-15,15,100,-15,15);
//        TH2D *zpos_hist=new TH2D("","Primary vertex distribution (X-Z); x (cm); z (cm);Events",100,-2,2,100,-2,2);
//        TH1D *lfactor_hist = new TH1D("","Lindhard factor ditribution; Lindhard factor;Events",100,0,1);
        auto vertex_hist=new TGraph2D();
        vertex_hist->SetTitle("Distribution of primary vertex of phonons; x (cm);y (cm);z (cm)");
//        auto initial_dir_hist=new TGraph2D();
//        initial_dir_hist->SetTitle("Momentum directions of neutrons; x (cm);y (cm);z (cm)");
        TH2D *TESxypos_hist=new TH2D("","Phonon hits energy distribution on TES; x (cm); y (cm);Energy (eV)",400,-2,2,400,-2,2);
//        auto c1 = new TCanvas("c1","Canvas",200,10,800,800);
        TH1D *Nphonon_hist =new TH1D("","No. of phonon from primary interaction; No. of phonons; Events",100,0,20000);
        TH1D *NotherSec_hist =new TH1D("","No. of other sceondaries from primary interaction; No. of secondaries; Events",100,0,20000);
//        TH1D *energy_hist =new TH1D("","Primary energy distribution; Energy (eV); Events",100,0,200);
        TH1D *depE_hist =new TH1D("","Interaction energy deposition distribution; Energy (eV); Events",100,0,200);
	TH1D *time_hist=new TH1D("","Length of signal from event;Time (s)",100,0,1e32);
//	TH1D *hit_hist=new TH1D("","No. of hits per event;No.of hits",1000,0,1e8);
//	TH1D *Whit_hist=new TH1D("","No. of hits per event;No.of hits",1000,0,1e8);
	TH1D *E_vs_t=new TH1D("","Energy vs time for one event; Time (#mu s); Energy (eV)",1000,0,10e6);
/*      velocity_hist->Sumw2();
        theta_hist->Sumw2();
        phi_hist->Sumw2();
	Nphonon_hist->Sumw2();
	NotherSec_hist->Sumw2();
	energy_hist->Sumw2();
	depE_hist->Sumw2();
	time_hist->Sumw2();
	hit_hist->Sumw2();
	Whit_hist->Sumw2();
//      r_energy_hist->Sumw2();
//      phononNo_hist->Sumw2();
//      zpos_hist->Sumw2();
//      xypos_hist->Sumw2();
        TESxypos_hist->Sumw2();
//      lfactor_hist->Sumw2();
*/
	TH1D *temperature_hist=new TH1D("","Expected temperature increase per event; T (K); Events",100,0,300);
//  TH2D *temp_vs_x_hist= new TH2D("","Temperature increase vs primary vertex position (X); x (cm); T (#mu K); Events",100,-15,15,100,0,50);
//  TH2D *temp_vs_y_hist= new TH2D("","Temperature increase vs primary vertex position (Y); y (cm); T (#mu K); Events",100,-15,15,100,0,50);
//  TH2D *temp_vs_z_hist= new TH2D("","Temperature increase vs primary vertex position (Z); z (cm); T (#mu K); Events",100,-15,15,100,0,50);
//  TH2D *edep_vs_Renergy =new TH2D("","Deposited energy vs recoil energy; Recoil energy (eV); Deposited energy (eV); Events",100,0,200,100,0,200);

//        temperature_hist->Sumw2();
//        temp_vs_x_hist->Sumw2();
//        temp_vs_y_hist->Sumw2();
//        temp_vs_z_hist->Sumw2();

	ifstream rootfilelist;
	char rootfilename[50];
	rootfilelist.open("rootfilelist.txt");
	while(rootfilelist>>rootfilename)
	{
		TFile *fin=new TFile(rootfilename,"read");

		TTree *perevent=(TTree*)fin->Get("Per_event_data");

		int N=perevent->GetEntries();

		int eveID;
		perevent->SetBranchAddress("Event_ID",&eveID);

		double depE,posx,posy,posz,ftime,Esum=0,bigtime=0,smalltime=6e33,count=0,Wcount=0;
		int W;
		perevent->SetBranchAddress("Deposited_energy",&depE);
		perevent->SetBranchAddress("Track_weight",&W);
		perevent->SetBranchAddress("Final_positionX",&posx);
		perevent->SetBranchAddress("Final_positionY",&posy);
		perevent->SetBranchAddress("Final_positionZ",&posz);
		perevent->SetBranchAddress("Final_time",&ftime);
//		vector<double> DepE_arr;
		long int prevEveID=-1;
		for(int i=0;i<N;i++)
		{
			perevent->GetEntry(i);
			TESxypos_hist->Fill(posx*100,posy*100,W);
			if((prevEveID!=eveID)&&(prevEveID!=-1))
			{
//				time_hist->Fill((bigtime-smalltime)/1e9);
//				temperature_hist->Fill(Get_temp(Esum)/1e6);   // DepE_arr.pushback(Esum);
//				hit_hist->Fill(count);
//				Whit_hist->Fill(Wcount);
				Esum=0;
				bigtime=0;
				smalltime=9999999999;
				count=0,Wcount=0;
			}
			if(ftime<smalltime)smalltime=ftime;
			if(ftime>bigtime)bigtime=ftime;
			prevEveID=eveID;
			Esum+=depE*W;
			count++;
			Wcount+=W;
			E_vs_t->Fill(ftime/1e3,depE*W);
		}
		if(prevEveID!=-1)
		{	
//			temperature_hist->Fill(Get_temp(Esum)/1e6);   // DepE_arr.pushback(Esum);
//			time_hist->Fill((bigtime-smalltime)/1e9);
//			hit_hist->Fill(count);
//			Whit_hist->Fill(Wcount);
		}
		TTree *perrun=(TTree*)fin->Get("Per_run_data");
		N=perrun->GetEntries();

		perrun->SetBranchAddress("Event_ID",&eveID);
//		perrun->SetBranchAddress("Energy_dep",&depE);
		perrun->SetBranchAddress("Initial_posx",&posx);
		perrun->SetBranchAddress("Initial_posy",&posy);
		perrun->SetBranchAddress("Initial_posz",&posz);

		double InitialE, dirx,diry,dirz, theta,phi, Nsec,NPL,NPTF,NPTS;
		perrun->SetBranchAddress("Recoil_Energy",&InitialE);
//		perrun->SetBranchAddress("Initial_dirx",&dirx);
//		perrun->SetBranchAddress("Initial_diry",&diry);
//		perrun->SetBranchAddress("Initial_dirz",&dirz);
		perrun->SetBranchAddress("Recoil_theta",&theta);
//		perrun->SetBranchAddress("Initial_phi",&phi);
		perrun->SetBranchAddress("NSec",&Nsec);
		perrun->SetBranchAddress("NPhononL",&NPL);
		perrun->SetBranchAddress("NPhononTF",&NPTF);
		perrun->SetBranchAddress("NPhononTS",&NPTS);

		for(int i=0;i<N;i++)
		{
			perrun->GetEntry(i);
			theta_hist->Fill(theta);
//			phi_hist->Fill(phi);
			vertex_hist->AddPoint(posx,posy,posz);
//			initial_dir_hist->AddPoint(dirx,diry,dirz);
			Nphonon_hist->Fill(NPL+NPTF+NPTS);
			NotherSec_hist->Fill(Nsec-(NPL+NPTF+NPTS));
			r_energy_hist->Fill(InitialE);
			depE_hist->Fill(depE);
		}

		fin->Close();
	}
	rootfilelist.close();
	TH1D *T_vs_t= new TH1D(*E_vs_t);
	T_vs_t->SetTitle("Temperature vs time for one event; Time (#mu s); Temperature (#mu K)");
	Double_t curr_temp=20e3,tmp=0;
	for(int i=0;i<T_vs_t->GetNbinsX();i++){
		tmp+=Get_temp(E_vs_t->GetBinContent(i),curr_temp+tmp,E_vs_t->GetBinWidth(i))*1e+6;
		T_vs_t->SetBinContent(i,tmp);
	}

        TFile *fin=new TFile("Histos.root","RECREATE");

	fin->WriteObject(theta_hist,"Theta distribution");
//	fin->WriteObject(phi_hist,"Phi distribution");
	fin->WriteObject(vertex_hist,"Primary vertex distribution");
//	fin->WriteObject(initial_dir_hist,"Primary particle direction distribution");
	fin->WriteObject(Nphonon_hist,"Primary phonon distribution");
	fin->WriteObject(NotherSec_hist,"Other secondaries number distribution");
	fin->WriteObject(TESxypos_hist,"Caustics");
	fin->WriteObject(temperature_hist,"Temperature rise");
	fin->WriteObject(r_energy_hist,"Recoil energy");
	fin->WriteObject(depE_hist,"Primary particle deposited energy");
	fin->WriteObject(time_hist,"Duration of event signal");
	fin->WriteObject(E_vs_t,"Energy vs Time");
	fin->WriteObject(T_vs_t,"Temperature vs Time (heat sink considered)");
//	fin->WriteObject(hit_hist,"No. of hits per event");
//	fin->WriteObject(Whit_hist,"No. of weighted hits per event");

	fin->Close();
	delete fin;
	fin=NULL;

}

Double_t Get_temp(Double_t edep,Double_t curr_temp,Double_t delta_time)
{
	Double_t volume=TMath::Pi()*2*2*2e-5, density=19;	// In cm^3 and Kg/cm^3
	Double_t HC=5.11e-5*curr_temp*curr_temp*curr_temp*1e-18/184;	// HC at curr_temp
	Double_t Rate_per_kelvin=1*25e-6/1;
	return (edep-std::min(Rate_per_kelvin*(curr_temp-20e3)*1e-6,100e-6)*delta_time)*1.6e-19/(HC*density*volume);
//	return (edep)*1.6e-19/(HC*density*volume);
}
