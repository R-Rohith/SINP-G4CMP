
	auto c1 = new TCanvas("c1","Canvas",200,10,800,600);

int extract_plots(char *filename,char *objectname,char *Objtype,char *stat)
{
  void OneD_hist(char*,char*,char*);
  void TwoD_GraphE(char*,char*,char*);
  void TwoD_Graph(char*,char*,char*);
//  if(*argc!=5) return 1;

	ROOT::EnableImplicitMT();
  if(std::strcmp(Objtype,"TH1D")==0)
    OneD_hist(filename,objectname,stat);
  else if(std::strcmp(Objtype,"TGraphErrors")==0)
    TwoD_GraphE(filename,objectname,stat);
  else if(std::strcmp(Objtype,"TGraph")==0)
    TwoD_Graph(filename,objectname,stat);
  else
    std::cout<<"\nImproper usage of macro!!\n Usage: extract_plots(<file name>,<object name>,<object type>,<statistics>)\n";
return 0;
}


void OneD_hist(char *filename,char *objectname,char *stat)
{
 std::cout<<"Plotting...";
 TFile *rootfile=TFile::Open(filename);
 TH1D *hist=(TH1D*)rootfile->Get(objectname);
 if(std::strcmp(stat,"FullStat")==0)
  gStyle->SetOptStat(111110);
 else if(std::strcmp(stat,"MinStat")==0)
  gStyle->SetOptStat(1110);
 else if(std::strcmp(stat,"NoStat")==0)
  gStyle->SetOptStat(0);
 
//  gStyle->SetOptStat(1110);

 c1->SetLogy();
 c1->SetMargin(0.14,0.14,0.12,0.12); 
 hist->GetXaxis()->SetLabelSize(0.045);
 hist->GetXaxis()->SetTitleSize(0.05);
 hist->GetXaxis()->SetTitleOffset(0.95);
 
 hist->GetYaxis()->SetLabelSize(0.045);
 hist->GetYaxis()->SetTitleSize(0.05);
 hist->GetYaxis()->SetTitleOffset(1.45);
 hist->GetYaxis()->SetTitle("Counts");
 hist->SetMarkerStyle(20);
 hist->Draw("e");
 c1->Update();
 char str[100];
 strcpy(str,objectname);
 std::strcat(str,".pdf");
 c1->SaveAs(str);
 c1->Clear();
}

void TwoD_GraphE(char *filename,char *objectname,char *stat)
{
 std::cout<<"Plotting...";
 TFile *rootfile=TFile::Open(filename);
 TGraphErrors *graph=(TGraphErrors*)rootfile->Get(objectname);
 if(std::strcmp(stat,"FullStat")==0)
  gStyle->SetOptStat(111110);
 else if(std::strcmp(stat,"MinStat")==0)
  gStyle->SetOptStat(1110);
 else if(std::strcmp(stat,"NoStat")==0)
  gStyle->SetOptStat(0);
 
 c1->SetLogy();
 c1->SetMargin(0.14,0.14,0.12,0.12); 
 
 graph->GetXaxis()->SetLabelSize(0.045);
 graph->GetXaxis()->SetTitleSize(0.05);
 graph->GetXaxis()->SetTitleOffset(0.95);

 graph->GetXaxis()->SetRangeUser(20,80);
 
 graph->GetYaxis()->SetLabelSize(0.045);
 graph->GetYaxis()->SetTitleSize(0.05);
 graph->GetYaxis()->SetTitleOffset(0.9);
 
 graph->Draw("ALP");
 c1->Update();
 char str[100];
 strcpy(str,objectname);
 std::strcat(str,".pdf");
 c1->SaveAs(str);
 c1->Clear();
}

void TwoD_Graph(char *filename,char *objectname,char *stat)
{
 std::cout<<"Plotting...";
 TFile *rootfile=TFile::Open(filename);
 TGraph *graph=(TGraph*)rootfile->Get(objectname);
 if(std::strcmp(stat,"FullStat")==0)
  gStyle->SetOptStat(111110);
 else if(std::strcmp(stat,"MinStat")==0)
  gStyle->SetOptStat(1110);
 else if(std::strcmp(stat,"NoStat")==0)
  gStyle->SetOptStat(0);
 
// c1->SetLogy();
 c1->SetMargin(0.14,0.14,0.12,0.12); 

 graph->GetXaxis()->SetLabelSize(0.045);
 graph->GetXaxis()->SetTitleSize(0.05);
 graph->GetXaxis()->SetTitleOffset(0.95);

// graph->GetXaxis()->SetRangeUser(-100,2000);
 
 graph->GetYaxis()->SetLabelSize(0.045);
 graph->GetYaxis()->SetTitleSize(0.05);
 graph->GetYaxis()->SetTitleOffset(1.45);   //It was 0.95

 graph->Draw("AP.");
 c1->Update();
 char str[100];
 strcpy(str,objectname);
 std::strcat(str,".pdf");
 c1->SaveAs(str);
 c1->Clear();
}
