// The heat capacities were extrapolated from the data in (https://srd.nist.gov/JPCRD/jpcrd263.pdf)

#define NoThread 30

#include "arrow/io/file.h"
#include "parquet/stream_reader.h"
void extract_plots()
{
	ROOT::EnableImplicitMT();
//	TH1D *velocity_hist =new TH1D("","Velocity distribution; velocity (km/s); Events",100,0,800);
        TH1D *theta_hist =new TH1D("","Theta distribution; #theta (rad); Events",100,0,TMath::Pi());
        TH1D *phi_hist =new TH1D("","Phi distribution; #phi (rad); Events",100,0,2*TMath::Pi());
//        TH1D *r_energy_hist =new TH1D("","Recoil energy distribution; Recoil energy (eV); Events",100,0,200);
//        TH1D *phononNo_hist =new TH1D("","Primary phonon number distribution; No. of phonons; Events",100,0,20000);
        TH2D *xypos_hist=new TH2D("","Primary vertex distribution (X-Y); x (cm); y (cm);Events",100,-15,15,100,-15,15);
//        TH2D *zpos_hist=new TH2D("","Primary vertex distribution (X-Z); x (cm); z (cm);Events",100,-2,2,100,-2,2);
//        TH1D *lfactor_hist = new TH1D("","Lindhard factor ditribution; Lindhard factor;Events",100,0,1);
        TH2D *TESxypos_hist=new TH2D("","Phonon hits energy distribution on TES; x (cm); y (cm);Energy (eV)",400,-2,2,400,-2,2);
	auto c1 = new TCanvas("c1","Canvas",200,10,800,800);

//	velocity_hist->Sumw2();
	theta_hist->Sumw2();
	phi_hist->Sumw2();
//	r_energy_hist->Sumw2();
//	phononNo_hist->Sumw2();
//	zpos_hist->Sumw2();
	xypos_hist->Sumw2();
	TESxypos_hist->Sumw2();
//	lfactor_hist->Sumw2();

	Double_t velocity, theta, r_energy, xpos, ypos, zpos, phononNo, lfactor,phi;
	std::vector<Double_t> r_energy_arr;
	std::vector<std::array<Double_t,3>> pos_arr;
 	std::vector<Double_t> edep_arr;
        Int_t prev_event_no=-1, Event_no, event_count=0;

	for(int i=0;i<NoThread;i++)
{
	ifstream fin;
	fin.open(string("Generator_output_G4WT")+i+".txt");
	
	while(fin>>velocity>>velocity>>xpos>>ypos>>zpos>>theta>>phi>>velocity>>velocity>>velocity)
	{
//		velocity_hist->Fill(velocity);
		theta_hist->Fill(theta);
		phi_hist->Fill(phi);
//		r_energy_hist->Fill(r_energy);
//		r_energy_arr.push_back(r_energy);
		xypos_hist->Fill(xpos,ypos);
//		zpos_hist->Fill(xpos/10,zpos/10);
		pos_arr.push_back({xpos,ypos,zpos});
//		phononNo_hist->Fill(phononNo);
//		lfactor_hist->Fill(lfactor);
	}
	
	fin.close();

       	Double_t tmp, time, Edep,Edep_sum=0, start_time=1e+5;
	prev_event_no=-1;

   std::shared_ptr<arrow::io::ReadableFile> infile;
   PARQUET_ASSIGN_OR_THROW(infile,arrow::io::ReadableFile::Open((const std::string)(string("phonon_hits_G4WT")+i+".parquet")));
   parquet::StreamReader stream{parquet::ParquetFileReader::Open(infile)};

   int runID, eventID, trackID;
   string particlename;
   double startenergy,startx,starty,startz,startt,edep,TW,finalx,finaly,finalz,finalt;
   while ( !stream.eof() )
   {
      stream >>runID>>eventID>>trackID>>particlename>>startenergy>>startx>>starty>>startz>>startt>>edep>>TW>>finalx>>finaly>>finalz>>finalt>> parquet::EndRow;
      if ((prev_event_no==-1) || (prev_event_no!=eventID))
      {   
	      event_count++;
	      if(prev_event_no!=-1)
		      edep_arr.push_back(Edep_sum);
	      Edep_sum=0;
      }
      prev_event_no=eventID;
//      if(event_count==3) 
//	      cout<<particlename<<'\t'<<eventID<<'\t'<<edep<<'\t'<<TW<<'\t'<<finalx<<'\t'<<finaly<<'\t'<<finalz<<'\t'<<endl;
      Edep_sum+=edep*TW;
      TESxypos_hist->Fill(finalx*100,finaly*100,edep*TW);
 }  
  if(prev_event_no!=-1) 
   edep_arr.push_back(Edep_sum);

}
//	velocity_hist->Draw();
//	c1->SaveAs("velocity-hist.pdf");
//	c1->Clear();
  gStyle->SetOptStat(111110);
//  gStyle->SetOptStat(1110);
	theta_hist->Draw("ep");
	c1->SaveAs("theta-hist.pdf");
	c1->Clear();
	phi_hist->Draw("ep");
	c1->SaveAs("phi-hist.pdf");
	c1->Clear();
//	c1->SetLogy();
//        r_energy_hist->Draw("ep");
//        c1->SaveAs("recoil-energy-hist.pdf");
//        c1->Clear();
//        phononNo_hist->Draw("ep");
//        c1->SaveAs("phonon-number-hist.pdf");
//        c1->Clear();
//        lfactor_hist->Draw("ep");
//        c1->SaveAs("Lindhard-factor-hist.pdf");
//        c1->Clear();
//        c1->SetLogy(0);
        gStyle->SetPalette(1);
        gStyle->SetOptStat(0);
	xypos_hist->GetYaxis()->SetTitleOffset(1.2);
        xypos_hist->Draw("COLZ");
	gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis*)xypos_hist->GetListOfFunctions()->FindObject("palette");
	palette->SetX1NDC(0.91);
	palette->SetX2NDC(0.93);
	palette->SetY1NDC(0.1);
	palette->SetY2NDC(0.9);
       	palette->SetLabelSize(0.03);
       	gStyle->SetTitleFontSize(0.07);
	gPad->Modified();
       	gPad->Update();
        c1->SaveAs("xy-distribution-hist.pdf");
        c1->Clear();
//        zpos_hist->Draw("COLZ");
//       	gPad->Update();
//	palette = (TPaletteAxis*)zpos_hist->GetListOfFunctions()->FindObject("palette");
//	palette->SetX1NDC(0.91);
//       	palette->SetX2NDC(0.93);
//       	palette->SetY1NDC(0.1);
//       	palette->SetY2NDC(0.9);
//       	palette->SetLabelSize(0.04);
//       	gStyle->SetTitleFontSize(0.07);
//       	gPad->Modified();
//       	gPad->Update();
//        c1->SaveAs("xz-distribution-hist.pdf");
//        c1->Clear();
	c1->SetLogz();
	TESxypos_hist->GetZaxis()->SetRangeUser(2e3,4e4);
	TESxypos_hist->GetYaxis()->SetTitleOffset(1.2);
	TESxypos_hist->Draw("COLZ");
        gPad->Update();
        palette = (TPaletteAxis*)TESxypos_hist->GetListOfFunctions()->FindObject("palette");
        palette->SetX1NDC(0.91);
        palette->SetX2NDC(0.93);
        palette->SetY1NDC(0.1);
        palette->SetY2NDC(0.9);
        palette->SetLabelSize(0.03);
        gStyle->SetTitleFontSize(0.07);
        gPad->Modified();
        gPad->Update();
        c1->SaveAs("TES-xy-distribution-hist.pdf");
        c1->Clear();
	c1->SetLogz(0);
        gStyle->SetOptStat(1);

  TH1D *temperature_hist=new TH1D("","Expected temperature increase per event; T (K); Events",100,0,100);
  TH2D *temp_vs_x_hist= new TH2D("","Temperature increase vs primary vertex position (X); x (cm); T (#mu K); Events",100,-15,15,100,0,50);
  TH2D *temp_vs_y_hist= new TH2D("","Temperature increase vs primary vertex position (Y); y (cm); T (#mu K); Events",100,-15,15,100,0,50);
  TH2D *temp_vs_z_hist= new TH2D("","Temperature increase vs primary vertex position (Z); z (cm); T (#mu K); Events",100,-15,15,100,0,50);
  Double_t Get_temp(Double_t);
//  TH2D *edep_vs_Renergy =new TH2D("","Deposited energy vs recoil energy; Recoil energy (eV); Deposited energy (eV); Events",100,0,200,100,0,200);
  
	temperature_hist->Sumw2();
	temp_vs_x_hist->Sumw2();
	temp_vs_y_hist->Sumw2();
	temp_vs_z_hist->Sumw2();
  for(int i=0;i<event_count;i++)
  {						
	  temperature_hist->Fill(Get_temp(edep_arr[i])/1e6);
//	  cout<<Get_temp(edep_arr[i])<<endl;
//	  edep_vs_Renergy->Fill(r_energy_arr[i],edep_arr[i]);
	  temp_vs_x_hist->Fill(pos_arr[i][0],Get_temp(edep_arr[i]));
	  temp_vs_y_hist->Fill(pos_arr[i][1],Get_temp(edep_arr[i]));
	  temp_vs_z_hist->Fill(pos_arr[i][2],Get_temp(edep_arr[i]));
  }
  gStyle->SetOptStat(111110);
//  gStyle->SetOptStat(1110);
   c1->SetLogy();
 temperature_hist->Draw("ep");
  c1->SaveAs("Temperature-hist.pdf"); 
  c1->Clear();
        c1->SetLogy(0);
  gStyle->SetOptStat(0);
//  edep_vs_Renergy->Draw("COLZ");
// gPad->Update();
//palette = (TPaletteAxis*)edep_vs_Renergy->GetListOfFunctions()->FindObject("palette");
//palette->SetX1NDC(0.91);
// palette->SetX2NDC(0.93);
// palette->SetY1NDC(0.1);
// palette->SetY2NDC(0.9);
// palette->SetLabelSize(0.03);
// gStyle->SetTitleFontSize(0.07);
// gPad->Modified();
// gPad->Update();
// c1->SaveAs("recoil-vs-deposited.pdf"); 
//  c1->Clear();
  temp_vs_x_hist->Draw("COLZ");
 gPad->Update();
palette = (TPaletteAxis*)temp_vs_x_hist->GetListOfFunctions()->FindObject("palette");
palette->SetX1NDC(0.91);
 palette->SetX2NDC(0.93);
 palette->SetY1NDC(0.1);
 palette->SetY2NDC(0.9);
 palette->SetLabelSize(0.03);
 gStyle->SetTitleFontSize(0.07);
 gPad->Modified();
 gPad->Update();
  c1->SaveAs("Temp-vs-x.pdf");
  c1->Clear();
  temp_vs_y_hist->Draw("COLZ");
 gPad->Update();
palette = (TPaletteAxis*)temp_vs_y_hist->GetListOfFunctions()->FindObject("palette");
palette->SetX1NDC(0.91);
 palette->SetX2NDC(0.93);
 palette->SetY1NDC(0.1);
 palette->SetY2NDC(0.9);
 palette->SetLabelSize(0.03);
 gStyle->SetTitleFontSize(0.07);
 gPad->Modified();
 gPad->Update();
  c1->SaveAs("Temp-vs-y.pdf");
  c1->Clear();
  temp_vs_z_hist->Draw("COLZ");
 gPad->Update();
 palette = (TPaletteAxis*)temp_vs_z_hist->GetListOfFunctions()->FindObject("palette");
palette->SetX1NDC(0.91);
 palette->SetX2NDC(0.93);
 palette->SetY1NDC(0.1);
 palette->SetY2NDC(0.9);
 palette->SetLabelSize(0.03);
 gStyle->SetTitleFontSize(0.07);
 gPad->Modified();
 gPad->Update();
  c1->SaveAs("Temp-vs-z.pdf");

  std::unique_ptr<TFile> myFile( TFile::Open("Histos.root", "RECREATE") );
//  myFile->WriteObject(velocity_hist, "Velocity histogram");
  myFile->WriteObject(theta_hist, "Theta histogram");
  myFile->WriteObject(phi_hist, "Phi histogram");
//  myFile->WriteObject(r_energy_hist, "Recoil energy histogram");
//  myFile->WriteObject(phononNo_hist, "Primary phonon histogram");
  myFile->WriteObject(xypos_hist, "Primary vertex position (X-Y)");
//  myFile->WriteObject(zpos_hist, "Primary vertex position (X-Z)");
//  myFile->WriteObject(lfactor_hist, "Lindhard factor");
  myFile->WriteObject(TESxypos_hist, "Phonon hit position at TES");
  myFile->WriteObject(temperature_hist, "Temperature rise");
  myFile->WriteObject(temp_vs_x_hist, "Temperature vs primary x position");
  myFile->WriteObject(temp_vs_y_hist, "Temperature vs primary y position");
  myFile->WriteObject(temp_vs_z_hist, "Temperature vs primary z position");
//  myFile->WriteObject(edep_vs_Renergy, "Deposited vs recoil energy");

}

Double_t Get_temp(Double_t edep)
{
	Double_t HC_at_20mK, HC_at_1K=0.00104/184;	// In J/(g . K)
	Double_t volume=TMath::Pi()*2*2*2e-5, density=19;	// In cm^3 and g/cm^3
	
	HC_at_20mK=HC_at_1K*20*20*20e-9;

	return 1e+6*edep*1.6e-19/(HC_at_20mK*density*volume);
}
