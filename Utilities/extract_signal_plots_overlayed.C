char* Get_energy(const char *title)
{
//	std::cout<<title<<" -> ";
	bool capture_string=false;
	unsigned int i=0;
	static char str[20]="", buff[10]="";
	strcpy(str,"");
	while((title[i]!=')')&&(title[i]!='\0'))
	{
		sprintf(buff,"%c",title[i]);
		if(capture_string)
			strcat(str,buff);
		if(title[i]=='=')
			capture_string=true;
		i++;
	}
//	std::cout<<str<<std::endl;
	return str;
}

void extract_signal_plots_overlayed(char *filename)
{
	auto c1 = new TCanvas("c1","Canvas",200,10,800,600);
	 c1->SetMargin(0.14,0.14,0.12,0.12);
	 c1->SetTickx();
	 c1->SetTicky();
	std::cout<<"Plotting...\n";
	TFile *rootfile=TFile::Open(filename);
	TGraph *single_graph;
	
	TMultiGraph *graph=new TMultiGraph();
	TLegend *legend=new TLegend(0.56,0.58,0.86,0.88);
	char str[50];
	for(int i=1;i<11;i++)
	{
		sprintf(str,"Temperature rise vs time (graph %d)",i);
		single_graph=(TGraph*)rootfile->Get(str);
		single_graph->SetLineColor(2+i);
		single_graph->SetLineWidth(2);
		legend->AddEntry(single_graph,Get_energy(single_graph->GetTitle()));
		graph->Add(single_graph);
	}
	graph->SetTitle("TES Temperature vs time;Time (ms);Temperature (nK)");
//        c1->Clear();
	// c1->SetLogy();
	 graph->Draw("AL");
	 legend->SetNColumns(2);
	 legend->Draw();

	 graph->GetXaxis()->SetLabelSize(0.045);
	 graph->GetXaxis()->SetTitleSize(0.05);
	 graph->GetXaxis()->SetTitleOffset(0.95);
	 graph->GetYaxis()->SetLabelSize(0.045);
	 graph->GetYaxis()->SetTitleSize(0.05);
	 graph->GetYaxis()->SetTitleOffset(1.45);

	 c1->SaveAs("Temp_vs_time.pdf");
	 c1->Clear();
	 delete graph;
	 delete legend;
	
	graph=new TMultiGraph();
	legend=new TLegend(0.56,0.42,0.86,0.12);
	str[50];
	for(int i=1;i<11;i++)
	{
		sprintf(str,"TES current vs time (graph %d)",i);
		single_graph=(TGraph*)rootfile->Get(str);
		single_graph->SetLineColor(2+i);
		single_graph->SetLineWidth(2);
		legend->AddEntry(single_graph,Get_energy(single_graph->GetTitle()));
		graph->Add(single_graph);
	}
	graph->SetTitle("Current through TES vs time;Time (ms);Current (pA)");
//        c1->Clear();
	// c1->SetLogy();
	 graph->Draw("AL");
	 legend->SetNColumns(2);
	 legend->Draw();

	 graph->GetXaxis()->SetLabelSize(0.045);
	 graph->GetXaxis()->SetTitleSize(0.05);
	 graph->GetXaxis()->SetTitleOffset(0.95);
	 graph->GetYaxis()->SetLabelSize(0.045);
	 graph->GetYaxis()->SetTitleSize(0.05);
	 graph->GetYaxis()->SetTitleOffset(1.45);

	 c1->SaveAs("Current_vs_time.pdf");
	 c1->Clear();
	 delete graph;
	 delete legend;
	
	graph=new TMultiGraph();
	legend=new TLegend(0.56,0.58,0.86,0.88);
	str[50];
	for(int i=1;i<11;i++)
	{
		sprintf(str,"SQUID Signal vs time (graph %d)",i);
		single_graph=(TGraph*)rootfile->Get(str);
		single_graph->SetLineColor(2+i);
		single_graph->SetLineWidth(2);
		legend->AddEntry(single_graph,Get_energy(single_graph->GetTitle()));
		graph->Add(single_graph);
	}
	graph->SetTitle("SQUID signal vs time;Time (ms);SQUID Output voltage (#muV)");
//        c1->Clear();
	// c1->SetLogy();
	 graph->Draw("AL");
	 legend->SetNColumns(2);
	 legend->Draw();

	 graph->GetXaxis()->SetLabelSize(0.045);
	 graph->GetXaxis()->SetTitleSize(0.05);
	 graph->GetXaxis()->SetTitleOffset(0.95);
	 graph->GetYaxis()->SetLabelSize(0.045);
	 graph->GetYaxis()->SetTitleSize(0.05);
	 graph->GetYaxis()->SetTitleOffset(1.45);

	 c1->SaveAs("SQUIDvoltage_vs_time.pdf");
	 c1->Clear();
}
