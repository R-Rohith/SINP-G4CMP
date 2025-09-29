// The heat capacities were extrapolated from the data in (https://srd.nist.gov/JPCRD/jpcrd263.pdf)

void extract_plots()
{
	ifstream fin;
	fin.open("Generator-output.txt");

	TH1D *velocity_hist =new TH1D("","Velocity distribution; velocity (km/s)",100,0,800);
	TH1D *theta_hist =new TH1D("","Theta distribution; #theta (rad)",100,0,TMath::Pi());
	TH1D *r_energy_hist =new TH1D("","Recoil energy distribution; Recoil energy (eV)",100,0,50);
	TH1D *phononNo_hist =new TH1D("","Phonon number distribution; No. of phonons",100,0,5000);
	TH2D *xypos_hist=new TH2D("","Primary vertex distribution (X-Y); x (cm); y (cm)",100,-2,2,100,-2,2);
	TH2D *zpos_hist=new TH2D("","Primary vertex distribution (X-Z); x (cm); z (cm)",100,-2,2,100,-2,2);
	TH1D *lfactor_hist = new TH1D("","Lindhard factor ditribution; Lindhard factor",100,0,1);

	auto c1 = new TCanvas("c1","Canvas",200,10,800,800);

	Double_t velocity, theta, r_energy, xpos, ypos, zpos, phononNo, lfactor;
	std::vector<Double_t> r_energy_arr;
	std::vector<std::array<Double_t,3>> pos_arr;
	
	velocity_hist->Sumw2();
	theta_hist->Sumw2();
	r_energy_hist->Sumw2();
	phononNo_hist->Sumw2();
	zpos_hist->Sumw2();
	xypos_hist->Sumw2();
	lfactor_hist->Sumw2();
	while(fin>>velocity>>theta>>r_energy>>xpos>>ypos>>zpos>>phononNo>>lfactor)
	{
		velocity_hist->Fill(velocity);
		theta_hist->Fill(theta);
		r_energy_hist->Fill(r_energy);
		r_energy_arr.push_back(r_energy);
		xypos_hist->Fill(xpos/10,ypos/10);
		zpos_hist->Fill(xpos/10,zpos/10);
		pos_arr.push_back({xpos/10,ypos/10,zpos/10});
		phononNo_hist->Fill(phononNo);
		lfactor_hist->Fill(lfactor);
		//if(!fin.good()) break;
	}
	velocity_hist->Draw();
	c1->SaveAs("velocity-hist.pdf");
	c1->Clear();
	theta_hist->Draw("ep");
	c1->SaveAs("theta-hist.pdf");
	c1->Clear();
	r_energy_hist->Draw("ep");
	c1->SaveAs("recoil-energy-hist.pdf");
	c1->Clear();
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	xypos_hist->Draw("COLZ");
	c1->SaveAs("xy-distribution-hist.pdf");
	c1->Clear();
	zpos_hist->Draw("COLZ");
	c1->SaveAs("xz-distribution-hist.pdf");
	c1->Clear();
	gStyle->SetOptStat(1);
	phononNo_hist->Draw("ep");
	c1->SaveAs("phonon-number-hist.pdf");
	c1->Clear();
	lfactor_hist->Draw("ep");
	c1->SaveAs("Lindhard-factor-hist.pdf");
	c1->Clear();
	
	fin.close();
	fin.open("phonon_hits.txt");
	TString particle_name;
 Double_t tmp, time, Edep,Edep_sum=0, start_time=1e+5;
 std::vector<Double_t> edep_arr;
 Int_t prev_event_no=-1, Event_no, event_count=0;

	while (fin>>tmp>>Event_no>>tmp>>particle_name>>tmp>>tmp>>tmp>>tmp>>tmp>>Edep>>tmp>>tmp>>tmp>>tmp>>time)
	{
		if ((prev_event_no==-1) || (prev_event_no!=Event_no))
		{
   event_count++;
   edep_arr.push_back(Edep_sum);
   Edep_sum=0;
		}
  prev_event_no=Event_no;
  Edep_sum+=Edep;
 }
   edep_arr.push_back(Edep_sum);

  auto edep_vs_Renergy = new TGraph(); 
  TH1D *temperature_hist=new TH1D("","Expected temperature increase per event; T (#mu K)",100,0,100);
  TH2D *temp_vs_x_hist= new TH2D("","Temperature increase vs primary vertex position (X); x (cm); T (#mu K)",100,-2,2,100,0,50);
  TH2D *temp_vs_y_hist= new TH2D("","Temperature increase vs primary vertex position (Y); y (cm); T (#mu K)",100,-2,2,100,0,50);
  TH2D *temp_vs_z_hist= new TH2D("","Temperature increase vs primary vertex position (Z); z (cm); T (#mu K)",100,-2,2,100,0,50);
  Double_t Get_temp(Double_t);
  
	temperature_hist->Sumw2();
	temp_vs_x_hist->Sumw2();
	temp_vs_y_hist->Sumw2();
	temp_vs_z_hist->Sumw2();
  for(int i=0;i<event_count;i++)
  {
	  temperature_hist->Fill(Get_temp(edep_arr[i+1]));
	  edep_vs_Renergy->AddPoint(r_energy_arr[i],edep_arr[i+1]);
	  temp_vs_x_hist->Fill(pos_arr[i][0],Get_temp(edep_arr[i+1]));
	  temp_vs_y_hist->Fill(pos_arr[i][1],Get_temp(edep_arr[i+1]));
	  temp_vs_z_hist->Fill(pos_arr[i][2],Get_temp(edep_arr[i+1]));
  }
  edep_vs_Renergy->SetTitle("Recoil energy vs total energy deposited; Recoil energy (eV); Energy deposited (eV)");
  edep_vs_Renergy->Draw("AP*");
 c1->SaveAs("recoil-vs-deposited.pdf"); 
  c1->Clear();
 temperature_hist->Draw("ep");
  c1->SaveAs("Temperature-hist.pdf"); 
  c1->Clear();
  gStyle->SetOptStat(0);
  temp_vs_x_hist->Draw("COLZ");
  c1->SaveAs("Temp-vs-x.pdf");
  c1->Clear();
  temp_vs_y_hist->Draw("COLZ");
  c1->SaveAs("Temp-vs-y.pdf");
  c1->Clear();
  temp_vs_z_hist->Draw("COLZ");
  c1->SaveAs("Temp-vs-z.pdf");
}

Double_t Get_temp(Double_t edep)
{
	Double_t HC_at_20mK, HC_at_1K=0.00104/184;	// In J/(g . K)
	Double_t volume=TMath::Pi()*2*2*2e-5, density=19;	// In cm^3 and g/cm^3
	
	HC_at_20mK=HC_at_1K*20*20*20e-9;

	return 1e+6*edep*1.6e-19/(HC_at_20mK*density*volume);
}
