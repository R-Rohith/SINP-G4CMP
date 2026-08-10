
#include "Utilities/Thermal_Calc.hh"

#include <iostream>
#include <cstdlib>
#include <string>

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"

#define MU_0 4*TMath::Pi()*1e-7
#define MAGNETIC_FLUX_QUANTUM 2.068e-15 // In Wb

void Get_signal(double,double,double&,double&,double&);
double Get_Tungsten_Resistance_Simplified(double);  // Returns resistance in ohms
double Get_Tungsten_HC(double);  // Returns Heat Capacity in ev/mol
void Update_Environment_Variables();
long int Get_Event_Index(int, unsigned int*);

//---Set the environment variables-----------------------------------

unsigned long int NO_OF_EVENTS_PER_RUN=1000;
unsigned long int NO_OF_SIGNAL_PLOTS=10;
double BEST_SIGNAL_PLOT_CUTOFF=140;  // In eV
double WORST_SIGNAL_PLOT_CUTOFF=50;  // In eV 
unsigned long int NO_OF_SIGNALS_SAVED=100;
double TIME_MAX=500;  // In ms
double TIME_MIN=-10;  // In ms
double TIME_RESOLUTION=1e-1;  // In ms
double TES_OP_TEMP=41e-3;  // In kelvin
double I_BIAS=26e-6;  // In amps
double R_SHUNT=2e-3;  // In ohms
double R_PARASITIC=1e-4;  // In ohms
double TES_NO_OF_MOLES=TMath::Pi()*2*2*2e-5*19/184;
unsigned int TES_INDUCTOR_WINDINGS=1;
double TES_INDUCTOR_RADIUS=0.1;  //In m
double THERMAL_CONTACT_INEFFICIENCY=1e-5;
double SQUID_FB_RESISTANCE=1e6;  // In ohms
double SQUID_FB_VOLTAGE=10;  // In volts
unsigned int SQUID_FB_INDUCTOR_WINDINGS=1;
double SQUID_FB_INDUCTOR_RADIUS=0.1;  // In m
double SQUID_SLEW_RATE=2e6;  // In magnetic quantum per second
char OUTPUT_FILENAME[50]="Histos.root";
double TES_INDUCTANCE;
double SQUID_FB_INDUCTANCE;

void Update_Environment_Variables()
{
	char *stopstr, *envVar=getenv("NO_OF_EVENTS_PER_RUN");  // No of events in one file
	if(envVar==NULL) cout<<"\"NO_OF_EVENTS_PER_RUN\" is not set, using default value\n";
	else NO_OF_EVENTS_PER_RUN=atol(envVar);

	envVar=getenv("NO_OF_SIGNAL_PLOTS");
	if(envVar==NULL) cout<<"\"NO_OF_SIGNAL_PLOTS\" is not set, using default value\n";
	else NO_OF_SIGNAL_PLOTS=atol(envVar);

        envVar=getenv("BEST_SIGNAL_PLOT_CUTOFF");
        if(envVar==NULL) cout<<"\"BEST_SIGNAL_PLOT_CUTOFF\" is not set, using default value\n";
        else BEST_SIGNAL_PLOT_CUTOFF=strtod(envVar,&stopstr);

        envVar=getenv("WORST_SIGNAL_PLOT_CUTOFF");
        if(envVar==NULL) cout<<"\"WORST_SIGNAL_PLOT_CUTOFF\" is not set, using default value\n";
        else WORST_SIGNAL_PLOT_CUTOFF=strtod(envVar,&stopstr);

	envVar=getenv("NO_OF_SIGNALS_SAVED");
	if(envVar==NULL) cout<<"\"NO_OF_SIGNALS_SAVED\" is not set, using default value\n";
	else NO_OF_SIGNALS_SAVED=atol(envVar);

	envVar=getenv("TIME_MAX");
	if(envVar==NULL) cout<<"\"TIME_MAX\" is not set, using default value.\n";
	else TIME_MAX=strtod(envVar,&stopstr);  // In ms

	envVar=getenv("TIME_MIN");
	if(envVar==NULL) cout<<"\"TIME_MIN\" is not set, using default value.\n";
	else TIME_MIN=strtod(envVar,&stopstr);  // In ms

	envVar=getenv("TIME_RESOLUTION");
	if(envVar==NULL) cout<<"\"TIME_RESOLUTION\" is not set, using default value.\n";
	else TIME_RESOLUTION=strtod(envVar,&stopstr);  // In ms

	envVar=getenv("TES_OP_TEMP");
	if(envVar==NULL) cout<<"\"TES_OP_TEMP\" is not set, using default value.\n";
	else TES_OP_TEMP=strtod(envVar,&stopstr);  // In kelvin

	envVar=getenv("I_BIAS");
	if(envVar==NULL) cout<<"\"I_BIAS\" is not set, using default value.\n";
	else I_BIAS=strtod(envVar,&stopstr);  // In amps

	envVar=getenv("R_SHUNT");
	if(envVar==NULL) cout<<"\"R_SHUNT\" is not set, using default value.\n";
	else R_SHUNT=strtod(envVar,&stopstr);  // In ohms

	envVar=getenv("R_PARASITIC");
	if(envVar==NULL) cout<<"\"R_PARASITIC\" is not set, using default value.\n";
	else R_PARASITIC=strtod(envVar,&stopstr);  // In ohms

	envVar=getenv("TES_NO_OF_MOLES");
	if(envVar==NULL) cout<<"\"TES_NO_OF_MOLES\" is not set, using default value.\n";
	else TES_NO_OF_MOLES=atoi(envVar);

	envVar=getenv("TES_INDUCTOR_WINDINGS");
	if(envVar==NULL) cout<<"\"TES_INDUCTOR WINDINGS\" is not set, using default value.\n";
	else TES_INDUCTOR_WINDINGS=atoi(envVar);

	envVar=getenv("TES_INDUCTOR_RADIUS");
	if(envVar==NULL) cout<<"\"TES_INDUCTOR_RADIUS\" is not set, using default value.\n";
	else TES_INDUCTOR_RADIUS=strtod(envVar,&stopstr);  //In m

	envVar=getenv("THERMAL_CONTACT_INEFFICIENCY");
	if(envVar==NULL) cout<<"\"THERMAL_CONTACT_INEFFICIENCY\" is not set, using default value.\n";
	else THERMAL_CONTACT_INEFFICIENCY=strtod(envVar,&stopstr);

	envVar=getenv("SQUID_FB_RESISTANCE");
	if(envVar==NULL) cout<<"\"SQUID_FB_RESISTANCE\" is not set, using default value.\n";
	else SQUID_FB_RESISTANCE=strtod(envVar,&stopstr);  // In ohms

	envVar=getenv("SQUID_FB_VOLTAGE");
	if(envVar==NULL) cout<<"\"SQUID_FB_VOLTAGE\" is not set, using default value.\n";
	else SQUID_FB_VOLTAGE=strtod(envVar,&stopstr);  // In volts

	envVar=getenv("SQUID_FB_INDUCTOR_WINDINGS");
	if(envVar==NULL) cout<<"\"SQUID_FB_INDUCTOR WINDINGS\" is not set, using default value.\n";
	else SQUID_FB_INDUCTOR_WINDINGS=atoi(envVar);

	envVar=getenv("SQUID_FB_INDUCTOR_RADIUS");
	if(envVar==NULL) cout<<"\"SQUID_FB_INDUCTOR_RADIUS\" is not set, using default value.\n";
	else SQUID_FB_INDUCTOR_RADIUS=strtod(envVar,&stopstr);  // In m

	envVar=getenv("SQUID_SLEW_RATE");
	if(envVar==NULL) cout<<"\"SQUID_SLEW_RATE\" is not set, using default value.\n";
	else SQUID_SLEW_RATE=strtod(envVar,&stopstr);  // In magnetic quantum per second

	envVar=getenv("OUTPUT_FILENAME");
	if(envVar==NULL) cout<<"\"OUTPUT_FILENAME\" is not set, using default value.\n";
	else strcpy(OUTPUT_FILENAME,envVar);

	TES_INDUCTANCE=MU_0*TES_INDUCTOR_WINDINGS/(2*TES_INDUCTOR_RADIUS);

	SQUID_FB_INDUCTANCE=MU_0*SQUID_FB_INDUCTOR_WINDINGS/(2*SQUID_FB_INDUCTOR_RADIUS);
}
//-------------------------------------------------------------------

unsigned int EventsReadPerRun;
long int Get_Event_Index(int EventID, unsigned int *EventIDArr)
{

	unsigned int EveIndex=0;
	for(;EveIndex<EventsReadPerRun;EveIndex++)
		if(EventIDArr[EveIndex]==EventID)
			return EveIndex;
	if(EventsReadPerRun>=NO_OF_EVENTS_PER_RUN)return -1;  // If we have enough events
	EventIDArr[EveIndex]=EventID;
	EventsReadPerRun++;
	return EveIndex;
}



int extract_data()
{
//	ROOT::EnableImplicitMT();
	Update_Environment_Variables();
//---Histogram declarations------------------------------------------

	TH1D *theta_hist =new TH1D("","Theta distribution; #theta (rad); Events",100,0,TMath::Pi());
        TH1D *phi_hist =new TH1D("","Phi distribution; #phi (rad); Events",100,0,2*TMath::Pi());
        auto vertex_hist=new TGraph2D();
        vertex_hist->SetTitle("Distribution of primary vertex; x (cm);y (cm);z (cm)");
        auto initial_dir_hist=new TGraph2D();
        initial_dir_hist->SetTitle("Momentum directions of Primary particle; x (cm);y (cm);z (cm)");
        TH2D *TESxypos_hist=new TH2D("","Phonon hits energy distribution on TES; x (cm); y (cm);Energy (eV)",400,-2,2,400,-2,2);
        TH1D *Nphonon_hist =new TH1D("","No. of phonon from primary interaction; No. of phonons; Events",100,0,0);
        TH1D *NotherSec_hist =new TH1D("","No. of other sceondaries from primary interaction; No. of secondaries; Events",100,0,0);
        TH1D *initial_energy_hist =new TH1D("","Primary energy distribution; Energy (MeV); Events",100,0,0);
        
	TH1D *depE_hist =new TH1D("","Interaction energy deposition distribution; Energy (eV); Events",100,0,0);
	TH1D *TESdepE_hist =new TH1D("","Energy deposition distribution at TES; Energy (eV); Events",100,1,2e6);
/*
	double *VarBins= new double[TESdepE_hist->GetNbinsX()+1];
	VarBins[0]=TESdepE_hist->GetBinLowEdge(1);
	for(int i=1;i<=TESdepE_hist->GetNbinsX();i++) VarBins[i]=VarBins[i-1]*TMath::Power(TESdepE_hist->GetBinLowEdge(TESdepE_hist->GetNbinsX()+1),1.0/TESdepE_hist->GetNbinsX());
	TESdepE_hist->SetBins(TESdepE_hist->GetNbinsX(),VarBins);
	delete VarBins; VarBins=NULL;
*/
	TH1D *hit_hist=new TH1D("","No. of hits per event;No.of hits",1000,0,0);
	TH2D *E_vs_t=new TH2D("","Energy deposited at TES vs time; Time (ms); Energy (eV)",100,0,10,100,0,0.012);

	TH1D *Max_temp_hist=new TH1D("","Max temperature in a event;Temperature (mK)",100,0,0);
	TGraph *Temp_graph[2*NO_OF_SIGNAL_PLOTS], *Current_graph[2*NO_OF_SIGNAL_PLOTS], *SQUID_out_graph[2*NO_OF_SIGNAL_PLOTS];
	for (int i=0;i<2*NO_OF_SIGNAL_PLOTS;i++)
	{
	Current_graph[i]= new TGraph();
	Temp_graph[i]= new TGraph();
	SQUID_out_graph[i]= new TGraph();
	}
	
	TH2D *signal_shape_hist_PN=new TH2D("","Peak normalised signal histogram; Time (ms); Normalized amplitude",(int)(TIME_MAX-TIME_MIN)/TIME_RESOLUTION,TIME_MIN,TIME_MAX,300,0,1);
	TH2D *signal_shape_hist_AN=new TH2D("","Area normalised signal histogram; Time (ms); Normalized amplitude",(int)(TIME_MAX-TIME_MIN)/TIME_RESOLUTION,TIME_MIN,TIME_MAX,300,0,0.007);
//---Open output filename and open a new TTree for storing TES temp, 
//---current, and output voltage-------------------------------------

	unsigned int EventCount=0, EventsWithEdep=0, GoodEveCount=0, Best_signalplot_count=0, Worst_signalplot_count=0;
	bool overflow=false,underflow=false;  // To indicate any data out of the selected time range 
	long int overflow_counter=0, underflow_counter=0;

	TFile *fout=new TFile(OUTPUT_FILENAME,"RECREATE");
	double current_temp, current_I, current_Vout, time;
	int prevEveID=-1;
	int EventID;
	TTree *OutTree=new TTree("Signal data","Signal data");
	OutTree->Branch("Event_ID",&EventID);
	OutTree->Branch("TES_temperature",&current_temp);
	OutTree->Branch("TES_current",&current_I);
	OutTree->Branch("SQUID_output",&current_Vout);
	OutTree->Branch("Time_per_event",&time);

	ifstream rootfilelist;
	char rootfilename[50];
	rootfilelist.open("rootfilelist.txt");  // Contains list of .root data files 
	while(rootfilelist>>rootfilename)
	{
		std::cout<<"Processing: "<<rootfilename<<std::endl;
		TFile *fin=new TFile(rootfilename,"read");
		TTree *perevent=(TTree*)fin->Get("Per_event_data");
		int N=perevent->GetEntries();

		double depE,posx,posy,posz,ftime,Esum=0;
		double W;

		perevent->SetBranchAddress("Event_ID",&EventID);
		perevent->SetBranchAddress("Deposited_energy",&depE);
		perevent->SetBranchAddress("Weight",&W);
		perevent->SetBranchAddress("Final_positionX",&posx);
		perevent->SetBranchAddress("Final_positionY",&posy);
//		perevent->SetBranchAddress("Final_positionZ",&posz);
		perevent->SetBranchAddress("Final_time",&ftime);

		unsigned int EdepArr_sizeY=(TIME_MAX-TIME_MIN)/TIME_RESOLUTION;   //In ms
		double *EdepArr[NO_OF_EVENTS_PER_RUN];
		for(int i=0;i<NO_OF_EVENTS_PER_RUN;i++)
			EdepArr[i]=new double[EdepArr_sizeY];
		for(int i=0;i<NO_OF_EVENTS_PER_RUN;i++)
			for(int j=0;j<EdepArr_sizeY;j++)
				EdepArr[i][j]=0;
		double *EsumArr=new double[NO_OF_EVENTS_PER_RUN];
		for(int i=0;i<NO_OF_EVENTS_PER_RUN;i++)EsumArr[i]=0;
		unsigned int *HitCountArr=new unsigned int[NO_OF_EVENTS_PER_RUN];
		for(int i=0;i<NO_OF_EVENTS_PER_RUN;i++)HitCountArr[i]=0;
		unsigned int *EventIDArr=new unsigned int[NO_OF_EVENTS_PER_RUN];
		double knullD; // To store waste values
		
		EventsReadPerRun=0;

		for(int i=0;i<N;i++)
		{
			perevent->GetEntry(i);
			ftime*=1e-6;	// ns to ms
			if(ftime>TIME_MAX){overflow=true; overflow_counter++;}
			else if(ftime<TIME_MIN){underflow=true; underflow_counter++;}
			else
			{
				E_vs_t->Fill(ftime,depE,W);
				long int EveIndex=Get_Event_Index(EventID,EventIDArr);
				if(EveIndex<0) continue;
				EdepArr[EveIndex][lround((ftime-TIME_MIN)/TIME_RESOLUTION)]+=depE*W;
				TESxypos_hist->Fill(posx,posy,W);
				HitCountArr[EveIndex]+=W;
				EsumArr[EveIndex]+=depE*W;
			}
		}
		for(unsigned int EveIndex=0;EveIndex<EventsReadPerRun;EveIndex++)  // Iterate through all the events that were read
		{
			TESdepE_hist->Fill(EsumArr[EveIndex]);
			if(EsumArr[EveIndex]<=0)continue;
                        
			bool Plot_signal=false;
                        unsigned int Index_signalplot;
                        if(Best_signalplot_count<NO_OF_SIGNAL_PLOTS)
                                if(EsumArr[EveIndex]>BEST_SIGNAL_PLOT_CUTOFF)
                                {
                                        Plot_signal=true;
                                        Index_signalplot=Best_signalplot_count;
                                        Best_signalplot_count++;
                                }
                        if(Worst_signalplot_count<NO_OF_SIGNAL_PLOTS)
                                if(EsumArr[EveIndex]<WORST_SIGNAL_PLOT_CUTOFF)
                                {
                                        Plot_signal=true;
                                        Index_signalplot=NO_OF_SIGNAL_PLOTS+Worst_signalplot_count;
                                        Worst_signalplot_count++;
                                }
			if(Plot_signal)
			{
				char str[100];
				sprintf(str,"TES current vs time (Edep=%.2lf KeV); Time (ms); Current (#muA)",EsumArr[EveIndex]*1e-3);  // eV to KeV
				Current_graph[Index_signalplot]->SetTitle(str);
				sprintf(str,"Temperature of TES vs time (Edep=%.2lf KeV); Time (ms); Temperature (mK)",EsumArr[EveIndex]*1e-3); // ev to KeV
				Temp_graph[Index_signalplot]->SetTitle(str);
				sprintf(str,"Output of SQUID vs time (Edep=%.2lf KeV); Time (ms); Voltage (V)",EsumArr[EveIndex]*1e-3);  // eV to KeV
				SQUID_out_graph[Index_signalplot]->SetTitle(str);
			}
			current_temp=TES_OP_TEMP;
			double highest_temp=TES_OP_TEMP;
			current_I=I_BIAS*R_SHUNT/(R_SHUNT+R_PARASITIC+Get_Tungsten_Resistance_Simplified(current_temp));
			const double initial_I=current_I;
			current_Vout=SQUID_FB_VOLTAGE;
			EventID=EventIDArr[EveIndex];
			double signal_amplitude[EdepArr_sizeY];
			double signal_amplitude_normFactor_PeakNormalisation=0, signal_amplitude_normFactor_AreaNormalisation=0;

			for(unsigned long int j=0;j<EdepArr_sizeY;j++)
			{
				time=j*TIME_RESOLUTION+TIME_MIN;
				Get_signal(EdepArr[EveIndex][j],time,current_temp,current_I,current_Vout);
				signal_amplitude[j]=current_Vout-SQUID_FB_VOLTAGE;
				if(signal_amplitude_normFactor_PeakNormalisation<signal_amplitude[j]) signal_amplitude_normFactor_PeakNormalisation=signal_amplitude[j];
				signal_amplitude_normFactor_AreaNormalisation+=signal_amplitude[j]*TIME_RESOLUTION;
				if(Plot_signal)
				{
					Current_graph[Index_signalplot]->AddPoint(time,(current_I)*1e6); // In microAmps
					Temp_graph[Index_signalplot]->AddPoint(time,current_temp*1e3); // In mK
					SQUID_out_graph[Index_signalplot]->AddPoint(time,current_Vout);
				}
				if(highest_temp<current_temp)highest_temp=current_temp;
				if(GoodEveCount<NO_OF_SIGNALS_SAVED)OutTree->Fill();
			}
//		        cout<<"Max possible temperature change for energy "<<EsumArr[EveIndex]<<" eV : "<<EsumArr[EveIndex]/(6.25e18*(1.008*TES_OP_TEMP+0.0346*TES_OP_TEMP*TES_OP_TEMP*TES_OP_TEMP)*1e-3*TMath::Pi()*2*2*2e-5*19/184)<<" K"<<endl;
			Max_temp_hist->Fill(highest_temp*1e3);
			hit_hist->Fill(HitCountArr[EveIndex]);
			EventsWithEdep++;
			GoodEveCount++;
			for(unsigned long int j=0;j<EdepArr_sizeY;j++)
			{
				time=j*TIME_RESOLUTION+TIME_MIN;
				signal_shape_hist_PN->Fill(time,signal_amplitude[j]/signal_amplitude_normFactor_PeakNormalisation);
				signal_shape_hist_AN->Fill(time,signal_amplitude[j]/signal_amplitude_normFactor_AreaNormalisation);
			}
		}
		EventCount+=EventsReadPerRun;

		TTree *perrun=(TTree*)fin->Get("Per_run_data");
		N=perrun->GetEntries();

		perrun->SetBranchAddress("Event_ID",&EventID);
		perrun->SetBranchAddress("Energy_dep",&depE);
		perrun->SetBranchAddress("Initial_posx",&posx);
		perrun->SetBranchAddress("Initial_posy",&posy);
		perrun->SetBranchAddress("Initial_posz",&posz);

		double InitialE, dirx,diry,dirz, theta,phi, Nsec,NPL,NPTF,NPTS;
		perrun->SetBranchAddress("Initial_energy",&InitialE); 
		perrun->SetBranchAddress("Initial_dirx",&dirx);
		perrun->SetBranchAddress("Initial_diry",&diry);
		perrun->SetBranchAddress("Initial_dirz",&dirz);
		perrun->SetBranchAddress("Initial_theta",&theta);
		perrun->SetBranchAddress("Initial_phi",&phi);
		perrun->SetBranchAddress("NSecondaries",&Nsec);
		perrun->SetBranchAddress("NPhononL",&NPL);
		perrun->SetBranchAddress("NPhononTF",&NPTF);
		perrun->SetBranchAddress("NPhononTS",&NPTS);

		for(int i=0;i<N;i++)
		{
			perrun->GetEntry(i);
			theta_hist->Fill(theta);
			phi_hist->Fill(phi);
			vertex_hist->AddPoint(posx,posy,posz);
			initial_dir_hist->AddPoint(dirx,diry,dirz);
			Nphonon_hist->Fill(NPL+NPTF+NPTS);
			NotherSec_hist->Fill(Nsec-(NPL+NPTF+NPTS));
			initial_energy_hist->Fill(InitialE);
			depE_hist->Fill(depE);
		}
		for(int i=0;i<NO_OF_EVENTS_PER_RUN;i++)
                        delete[] EdepArr[i];
                delete[] EsumArr;
                delete[] HitCountArr;
                delete[] EventIDArr;

		fin->Close();

	}
	rootfilelist.close();
        
	cout<<"\nOut of "<<EventCount<<" events, "<<EventsWithEdep<<" events has energy depositions\n";
	if(overflow)
		cout<<"Please Increase TIME_MAX. There are "<<overflow_counter<<" hits beyond the current time interval\n";
	if(underflow)
		cout<<"Please Decrease TIME_MIN. There are "<<underflow_counter<<" hits beyond the current time interval\n";

	fout->WriteObject(theta_hist,"Theta distribution");
	fout->WriteObject(phi_hist,"Phi distribution");
	fout->WriteObject(vertex_hist,"Primary vertex distribution");
	fout->WriteObject(initial_dir_hist,"Primary particle direction distribution");
	fout->WriteObject(Nphonon_hist,"Primary phonon distribution");
	fout->WriteObject(NotherSec_hist,"Other secondaries number distribution");
	fout->WriteObject(TESxypos_hist,"Caustics");
	fout->WriteObject(initial_energy_hist,"Initial energy");
	fout->WriteObject(depE_hist,"Primary particle deposited energy");
	fout->WriteObject(TESdepE_hist,"Deposited energy at TES");
	fout->WriteObject(E_vs_t,"Energy vs Time");
	fout->WriteObject(hit_hist,"No. of hits per event");
	for(int i=0;i<2*NO_OF_SIGNAL_PLOTS;i++)
	{
		char str[50];
		sprintf(str,"TES current vs time (graph %d)",i+1);
	fout->WriteObject(Current_graph[i],str);
	sprintf(str,"Temperature rise vs time (graph %d)",i+1);
	fout->WriteObject(Temp_graph[i],str);
	sprintf(str,"SQUID Signal vs time (graph %d)",i+1);
	fout->WriteObject(SQUID_out_graph[i],str);
	}
	fout->WriteObject(Max_temp_hist,"Max temp in event");
	fout->WriteObject(signal_shape_hist_PN,"Peak normalised signal shape histogram");
	fout->WriteObject(signal_shape_hist_AN,"Area normalised signal shape histogram");
	fout->WriteObject(OutTree,"Signal data");

	fout->Close();
        return 0;
}


void Get_signal(double dE_dep, double time, double &curr_temp, double &curr_I, double &curr_Vout)
{
	if(curr_temp-TES_OP_TEMP<1e-15)curr_temp=TES_OP_TEMP;
	const double dt=TIME_RESOLUTION*1e-3;
	const double R_TES=Get_Tungsten_Resistance_Simplified(curr_temp); // In Ohms
	const double R_load=R_SHUNT+R_PARASITIC;
	const double V=I_BIAS*R_SHUNT;
	const double G=(300*1e-6*2)*6.25e18*THERMAL_CONTACT_INEFFICIENCY; // Last term is modification to thermal conductance to match observed signal
/*	const double dE_joule=curr_I*curr_I*R_TES*TIME_RESOLUTION*1e-3;
	const double dE_sink=G*(curr_temp-TES_OP_TEMP)*TIME_RESOLUTION*1e-3;
		
	curr_temp+=(dE_dep+dE_joule-dE_sink)/(Get_Tungsten_HC(curr_temp)*TES_NO_OF_MOLES);
	const double dB_TES=(V-curr_I*R_load-curr_I*R_TES)*TIME_RESOLUTION*1e-3;
	curr_I+=dB_TES/TES_INDUCTANCE;
	curr_Vout+=SQUID_FB_RESISTANCE/SQUID_FB_INDUCTANCE*((dB_TES<SQUID_SLEW_RATE*TIME_RESOLUTION*1e-3)?-dB_TES:(dB_TES<0)?SQUID_SLEW_RATE*TIME_RESOLUTION*1e-3:-SQUID_SLEW_RATE*TIME_RESOLUTION*1e-3); 
*/
	const double P_dep=dE_dep/dt;
	const double P_joule=curr_I*curr_I*R_TES;
	double dB_TES=-curr_I*TES_INDUCTANCE;
//	cout<<"Energy: "<<dE_dep<<" T: "<<curr_temp<<" -> ";
	curr_temp=(P_dep+P_joule-TMath::Exp(-G*dt/(Get_Tungsten_HC(curr_temp)*TES_NO_OF_MOLES))*(P_dep+P_joule-G*(curr_temp-TES_OP_TEMP)))/G+TES_OP_TEMP;
//	cout<<curr_temp<<" I: "<<curr_I<<" -> ";
	curr_I=(V-TMath::Exp(-(R_load+R_TES)*dt/TES_INDUCTANCE)*(V-(R_load+R_TES)*curr_I))/(R_load+R_TES);
//	cout<<curr_I<<endl;
	dB_TES+=curr_I*TES_INDUCTANCE;
	curr_Vout+=SQUID_FB_RESISTANCE/SQUID_FB_INDUCTANCE*((TMath::Abs(dB_TES)<SQUID_SLEW_RATE*dt)?-dB_TES:(dB_TES<0)?SQUID_SLEW_RATE*dt:-SQUID_SLEW_RATE*dt); 
	if(TMath::Abs(dB_TES)>SQUID_SLEW_RATE*dt)cout<<"Magnetic quantum loss !!\n";
}

