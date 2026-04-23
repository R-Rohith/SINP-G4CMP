
#include "Utilities/Thermal_Calc.hh"

#define TIME_MAX 500 // In ms
#define TIME_MIN -10 // in ms
#define TIME_RESOLUTION 1e-1 // In ms

#define TES_OP_TEMP 41e-3 // In Kelvin

#define I_BIAS 26e-6 // In Amps
#define R_SHUNT 2e-3 // In Ohms
#define R_PARASITIC 1e-4 // In Ohms

#define INDUCTOR_WINDINGS 1
#define INDUCTOR_RADIUS 0.1 // In m
#define MU_0 4*TMath::Pi()*1e-7
const double INDUCTANCE=MU_0*INDUCTOR_WINDINGS/(2*INDUCTOR_RADIUS);

#define THERMAL_CONTACT_INEFFICIENCY 1e-5

#define MAGNETIC_FLUX_QUANTUM 2.068e-15 // In Wb

int extract_data()
{
	void Get_signal(double,double,double&,double&);
	long long int Get_BookSignal(double,double,double&);
	double Get_Tungsten_Resistance_Simplified(double);
	 ROOT::EnableImplicitMT();
//	TH1D *velocity_hist =new TH1D("","Velocity distribution; velocity (km/s); Events",100,0,800);
        TH1D *theta_hist =new TH1D("","Theta distribution; #theta (rad); Events",100,0,TMath::Pi());
        TH1D *phi_hist =new TH1D("","Phi distribution; #phi (rad); Events",100,0,2*TMath::Pi());
//        TH1D *r_energy_hist =new TH1D("","Recoil energy distribution; Recoil energy (eV); Events",100,0,200);
//        TH1D *phononNo_hist =new TH1D("","Primary phonon number distribution; No. of phonons; Events",100,0,20000);
//        TH2D *xypos_hist=new TH2D("","Primary vertex distribution (X-Y); x (cm); y (cm);Events",100,-15,15,100,-15,15);
//        TH2D *zpos_hist=new TH2D("","Primary vertex distribution (X-Z); x (cm); z (cm);Events",100,-2,2,100,-2,2);
//        TH1D *lfactor_hist = new TH1D("","Lindhard factor ditribution; Lindhard factor;Events",100,0,1);
        auto vertex_hist=new TGraph2D();
        vertex_hist->SetTitle("Distribution of primary vertex; x (cm);y (cm);z (cm)");
        auto initial_dir_hist=new TGraph2D();
        initial_dir_hist->SetTitle("Momentum directions of Primary particle; x (cm);y (cm);z (cm)");
//        TH2D *TESxypos_hist=new TH2D("","Phonon hits energy distribution on TES; x (cm); y (cm);Energy (eV)",400,-2,2,400,-2,2);
        TH1D *Nphonon_hist =new TH1D("","No. of phonon from primary interaction; No. of phonons; Events",100,0,20000);
        TH1D *NotherSec_hist =new TH1D("","No. of other sceondaries from primary interaction; No. of secondaries; Events",100,0,20000);
        TH1D *initial_energy_hist =new TH1D("","Primary energy distribution; Energy (MeV); Events",100,0,20);
        TH1D *depE_hist =new TH1D("","Interaction energy deposition distribution; Energy (eV); Events",100,0,200);
//	TH1D *hit_hist=new TH1D("","No. of hits per event;No.of hits",1000,0,1e8);
	TH1D *Whit_hist=new TH1D("","No. of hits per event;No.of hits",1000,0,1e8);
	TH1D *E_vs_t=new TH1D("","Energy vs time for one event; Time (#mu s); Energy (eV)",1000,0,10e6);
	TH1D *Max_temp_hist=new TH1D("","Max temperature in a event;Temperature (mK)",100,0,0);
	TGraph *Signal_graph= new TGraph();
	Signal_graph->SetTitle("TES current vs time; Time (ms); Current (#muA)");
	TGraph *Temp_graph= new TGraph();
	Temp_graph->SetTitle("Temperature of TES vs time; Time (ms); Temperature (mK)");
	TH1D *BOOK_Max_temp_hist=new TH1D("","Max temperature in a event;Temperature (mK)",100,0,0);
	TGraph *BOOK_Signal_graph= new TGraph();
	BOOK_Signal_graph->SetTitle("TES Current vs time; Time (ms); Current (#muA)");
	TGraph *BOOK_Temp_graph= new TGraph();
	BOOK_Temp_graph->SetTitle("Temperature of TES vs time; Time (ms); Temperature (mK)");
//  TH2D *edep_vs_Renergy =new TH2D("","Deposited energy vs recoil energy; Recoil energy (eV); Deposited energy (eV); Events",100,0,200,100,0,200);


	int EventCount=0;
	ifstream rootfilelist;
	char rootfilename[50];
	rootfilelist.open("rootfilelist.txt");
	while(rootfilelist>>rootfilename)
	{
		bool overflow=false,underflow=false;
		TFile *fin=new TFile(rootfilename,"read");

		TTree *perevent=(TTree*)fin->Get("Per_event_data");

		int N=perevent->GetEntries();

		int eveID;
		perevent->SetBranchAddress("Event_ID",&eveID);

		double depE,posx,posy,posz,ftime,Esum=0,bigtime=0,smalltime=6e33,count=0,Wcount=0;
		double W;
		perevent->SetBranchAddress("Deposited_energy",&depE);
		perevent->SetBranchAddress("Weight",&W);
//		perevent->SetBranchAddress("Final_positionX",&posx);
//		perevent->SetBranchAddress("Final_positionY",&posy);
//		perevent->SetBranchAddress("Final_positionZ",&posz);
		perevent->SetBranchAddress("Final_time",&ftime);
//		vector<double> DepE_arr;
		long int prevEveID=-1;
		unsigned int EdepArr_size=(TIME_MAX-TIME_MIN)/TIME_RESOLUTION;   //In ms
		double *EdepArr=new double[EdepArr_size];
		double knullD; // To store waste values
//		vector<double> EdepArr,timeArr;
		for(int i=0;i<EdepArr_size;i++)EdepArr[i]=0;
		for(int i=0;i<N;i++)
		{
			perevent->GetEntry(i);
			ftime*=1e-6;	// ns to ms
//			TESxypos_hist->Fill(posx*100,posy*100,W);
			if((prevEveID!=eveID)&&(prevEveID!=-1))
			{
//				time_hist->Fill((bigtime-smalltime)/1e9);
//				temperature_hist->Fill(Get_temp(Esum)/1e6);   // DepE_arr.pushback(Esum);
//				hit_hist->Fill(count);
				Whit_hist->Fill(Wcount);
//				bigtime=0;
//				smalltime=9999999999;
//				count=0;
				Wcount=0;
				double current_temp=TES_OP_TEMP,highest_temp=TES_OP_TEMP; // In K
				double current_I=I_BIAS*R_SHUNT/(R_SHUNT+R_PARASITIC+Get_Tungsten_Resistance_Simplified(current_temp));
				double initial_I=current_I;
				if(current_I>I_BIAS){ cout<<"unnatural current "<<current_I<<" > "<<I_BIAS<<endl;return 1;}
				double BOOK_current_temp=TES_OP_TEMP,BOOK_highest_temp=TES_OP_TEMP; // In K
				bool first_time=true;
/*				for(int j=0,j_offset=0;j<EdepArr_size;j++)
				{
					if(EdepArr[j]<=0)
					{
						if(first_time)
						j_offset++;
						continue;
					}
					first_time=false;
					Temp_graph->AddPoint((float)TIME_MAX/TIME_RESOLUTION*(j-j_offset),Get_temp(EdepArr[j],current_temp,TIME_RESOLUTION));
//					cout<<(float)TIME_MAX/TIME_RESOLUTION*(j-j_offset)<<" "<<Get_temp(EdepArr[j],current_temp,TIME_RESOLUTION)<<endl;
				}
				for(int j=0;j<EdepArr_size;j++)EdepArr[j]=0;
*/
/*				if(TIME_MIN>timeArr.front())underflow=true;
				if(TIME_MAX<timeArr.back())overflow=true;
				for(double lasttime=TIME_MIN;lasttime<TIME_MAX;lasttime+=TIME_RESOLUTION)
				{
					double E=0;
					for(int j=0;(j<timeArr.size())&&(lasttime+TIME_RESOLUTION>timeArr[j]);j++)
						if(lasttime<timeArr[j])
							E+=EdepArr[j];
//					cout<<"Before: "<<lasttime<<" "<<current_temp<<" E: "<<E<<endl;
					Signal_graph->AddPoint(lasttime,Get_signal(E,TIME_RESOLUTION,current_temp));
					Temp_graph->AddPoint(lasttime,current_temp*1e3);
//					cout<<"After: "<<lasttime<<" "<<current_temp<<endl;
				}
*/
				EventCount++;
				for(unsigned long int j=0;j<EdepArr_size;j++)
				{
					Get_signal(EdepArr[j],j*TIME_RESOLUTION+TIME_MIN,current_temp,current_I);
					Signal_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,(current_I-initial_I)*1e6); // In microAmps
					Temp_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,current_temp*1e3); // In mK
					if (j*TIME_RESOLUTION+TIME_MIN>=0)
					{
						BOOK_Signal_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,Get_BookSignal(Esum,j*TIME_RESOLUTION+TIME_MIN,BOOK_current_temp)*1e6);
						BOOK_Temp_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,BOOK_current_temp*1e3);
					}
					else
					{
//						BOOK_Signal_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,1.15e-7/0.001*INDUCTOR_WINDINGS*MU_0/(2*INDUCTOR_RADIUS*MAGNETIC_FLUX_QUANTUM));
						BOOK_Signal_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,initial_I*1e6);
						BOOK_Temp_graph->AddPoint(j*TIME_RESOLUTION+TIME_MIN,BOOK_current_temp*1e3);
					}
//					knullD=Get_signal(EdepArr[j],TIME_RESOLUTION,current_temp);
					EdepArr[j]=0;
					if(highest_temp<current_temp)highest_temp=current_temp;
					if(BOOK_highest_temp<BOOK_current_temp)BOOK_highest_temp=BOOK_current_temp;
				}
				cout<<"Max possible temperature change : "<<Esum/(6.25e18*(1.008*TES_OP_TEMP+0.0346*TES_OP_TEMP*TES_OP_TEMP*TES_OP_TEMP)*1e-3*TMath::Pi()*2*2*2e-5*19/184)<<endl;
				Esum=0;
				Max_temp_hist->Fill(highest_temp*1e3);
				BOOK_Max_temp_hist->Fill(BOOK_highest_temp*1e3);
				if(EventCount>10)break;
			}
//			if(ftime<smalltime)smalltime=ftime;
//			if(ftime>bigtime)bigtime=ftime;
			
//			E_vs_t->Fill(ftime/1e3,depE*W);
//			cout<<ftime/TIME_RESOLUTION<<endl;
	                if(ftime>TIME_MAX)overflow=true;
			else if(ftime<TIME_MIN)underflow=true;
			else
			{
				EdepArr[lround((ftime-TIME_MIN)/TIME_RESOLUTION)]+=depE*W;
				prevEveID=eveID;
				Esum+=depE*W;
//				count++;
				Wcount+=W;
			}

/*			EdepArr.push_back(depE*W);
			timeArr.push_back(ftime);

			for(int j=timeArr.size()-1;j>0;j--)	// Bubble sort
				if(timeArr[last]<timeArr[j-1])
				{
					double tmp=timeArr[j];
					timeArr[j]=timeArr[j-1];
					timeArr[j-1]=tmp;
					tmp=EdepArr[j];
					EdepArr[j]=EdepArr[j-1];
					EdepArr[j-1]=tmp;
				}
				else

					break;
*/
/*			bool did_insertion=false;
			for(int j=0;j<timeArr.size();j++)
				if(timeArr[j]>ftime)
				{
					timeArr.insert(timeArr.begin()+j,ftime);
					EdepArr.insert(EdepArr.begin()+j,depE*W);
					did_insertion=true;
					break;
				}
			if(!did_insertion)
			{
				timeArr.insert(timeArr.end(),ftime);
                                EdepArr.insert(EdepArr.end(),depE*W);
			}
*/
		}
/*		if(prevEveID!=-1)
		{	
//			temperature_hist->Fill(Get_temp(Esum)/1e6);   // DepE_arr.pushback(Esum);
//			time_hist->Fill((bigtime-smalltime)/1e9);
//			hit_hist->Fill(count);
			Whit_hist->Fill(Wcount);
			double current_temp=20; // In mK
                        bool first_time=true;
                        for(int j=0,j_offset=0;j<EdepArr_size;j++)
                        {
                              if(EdepArr[j]<=0)
                              {
                                if(first_time)
                                j_offset++;
                                continue;
                              }
                              first_time=false;
                              Temp_graph->AddPoint((float)TIME_MAX/TIME_RESOLUTION*(j-j_offset),Get_temp(EdepArr[j],current_temp,TIME_RESOLUTION));
                         }
                         for(int j=0;j<EdepArr_size;j++)EdepArr[j]=0;
		}
*/		if(overflow)
			cout<<"Please Increase TIME_MAX. There are hits beyond the current time interval\n";
		if(underflow)
			cout<<"Please Decrease TIME_MIN. There are hits beyond the current time interval\n";

		TTree *perrun=(TTree*)fin->Get("Per_run_data");
		N=perrun->GetEntries();

		perrun->SetBranchAddress("Event_ID",&eveID);
		perrun->SetBranchAddress("Energy_dep",&depE);
		perrun->SetBranchAddress("Initial_posx",&posx);
		perrun->SetBranchAddress("Initial_posy",&posy);
		perrun->SetBranchAddress("Initial_posz",&posz);

		double InitialE, dirx,diry,dirz, theta,phi, Nsec,NPL,NPTF,NPTS;
//		perrun->SetBranchAddress("Recoil_Energy",&InitialE);
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
//			r_energy_hist->Fill(InitialE);
			initial_energy_hist->Fill(InitialE);
//			depE_hist->Fill(depE);
		}

		fin->Close();
	}
	rootfilelist.close();
/*	TH1D *T_vs_t= new TH1D(*E_vs_t);
	T_vs_t->SetTitle("Temperature vs time for one event; Time (#mu s); Temperature (#mu K)");
	Double_t curr_temp=20e3,tmp=0;
	for(int i=0;i<T_vs_t->GetNbinsX();i++){
		tmp+=Get_temp(E_vs_t->GetBinContent(i),curr_temp+tmp,E_vs_t->GetBinWidth(i))*1e+6;
		T_vs_t->SetBinContent(i,tmp);
	}
*/
        TFile *fin=new TFile("Histos.root","RECREATE");

	fin->WriteObject(theta_hist,"Theta distribution");
	fin->WriteObject(phi_hist,"Phi distribution");
	fin->WriteObject(vertex_hist,"Primary vertex distribution");
	fin->WriteObject(initial_dir_hist,"Primary particle direction distribution");
	fin->WriteObject(Nphonon_hist,"Primary phonon distribution");
	fin->WriteObject(NotherSec_hist,"Other secondaries number distribution");
//	fin->WriteObject(TESxypos_hist,"Caustics");
//	fin->WriteObject(temperature_hist,"Temperature rise");
//	fin->WriteObject(r_energy_hist,"Recoil energy");
	fin->WriteObject(initial_energy_hist,"Initial energy");
	fin->WriteObject(depE_hist,"Primary particle deposited energy");
	fin->WriteObject(E_vs_t,"Energy vs Time");
//	fin->WriteObject(hit_hist,"No. of hits per event");
	fin->WriteObject(Whit_hist,"No. of weighted hits per event");
	fin->WriteObject(Signal_graph,"SQUID Signal vs time");
	fin->WriteObject(Temp_graph,"Temperature rise vs time");
	fin->WriteObject(Max_temp_hist,"Max temp in event");
	fin->WriteObject(BOOK_Signal_graph,"SQUID Signal vs time (Acc. to Book");
	fin->WriteObject(BOOK_Temp_graph,"Temperature rise vs time (Acc. to Book");
	fin->WriteObject(BOOK_Max_temp_hist,"Max temp in event (Acc. to Book)");

	fin->Close();
	delete fin;
	fin=NULL;
        return 0;
}

double Get_temp(double edep,double curr_temp,double delta_time)
{
	void Get_Tloss(double&,double);
	Double_t volume=TMath::Pi()*2*2*2e-5, density=19;	// In cm^3 and g/cm^3
//	Double_t HC=6.25e18*5.11e-5*curr_temp*curr_temp*curr_temp/184;	// HC at curr_temp in eV/g/K
	double HC=6.25e18*(1.008*curr_temp+0.0346*curr_temp*curr_temp*curr_temp)*1e-3*volume*density/184;
//	Double_t Rate_of_HeatSink=(300*4e-6*2)*6.25e18; // 300 W/m/K * 4mm^2/ 0.5m ; In eV/s/mK
//	Rate_of_HeatSink=1e8; 
//	Get_Tloss(curr_temp,delta_time);
//	Double_t HC=6.25e18*5.11e-5*curr_temp*curr_temp*curr_temp*1e-9/184;	// HC at curr_temp in eV/g/K
//	curr_temp+=1e3*edep/(HC*density*volume);
//	cout<<"Before: "<<edep<<" "<<curr_temp<<" "<<HC<<" "<<Rate_of_HeatSink<<" "<<(curr_temp-20)*1e-3*delta_time*1e-3<<endl;
//	curr_temp+=1e3*(edep-Rate_of_HeatSink*(curr_temp-20)*1e-3*delta_time*1e-3)/(HC*density*volume);
//	curr_temp-=1e3*Rate_of_HeatSink*delta_time*1e-3/(6.25e18*5.11e-5);
//	curr_temp+=edep/(HC*density*volume);
//	cout<<"After: "<<edep<<" "<<curr_temp<<" "<<HC<<" "<<Rate_of_HeatSink<<" "<<(curr_temp-20)*1e-3*delta_time*1e-3<<endl;
//	if(curr_temp<0)curr_temp=20;
	return edep/HC;//(HC*density*volume);
//	return (edep-std::min(Rate_per_kelvin*(curr_temp-20e3)*1e-6,100e-6)*delta_time)*1.6e-19/(HC*density*volume);
//	return (edep)*1.6e-19/(HC*density*volume);
}

void Get_signal(double dE_dep, double time, double &curr_temp, double &curr_I)
{
	if(curr_temp-TES_OP_TEMP<1e-12)curr_temp=TES_OP_TEMP;
	double Get_Tungsten_Resistance_Simplified(double);
	const double R_TES=Get_Tungsten_Resistance_Simplified(curr_temp); // In Ohms
	const double R_load=R_SHUNT+R_PARASITIC;
	const double V=I_BIAS*R_SHUNT;
	const double Rate_of_heat_loss=(300*1e-6*2)*6.25e18*THERMAL_CONTACT_INEFFICIENCY; // Last term is modification to thermal conductance to match observed signal
	const double dE_joule=curr_I*curr_I*R_TES*TIME_RESOLUTION*1e-3;
	const double dE_sink=Rate_of_heat_loss*(curr_temp-TES_OP_TEMP)*TIME_RESOLUTION*1e-3;
	
	const double TES_HeatCapacity=6.25e18*(1.008*curr_temp+0.0346*curr_temp*curr_temp*curr_temp)*1e-3*TMath::Pi()*2*2*2e-5*19/184;
	curr_temp+=(dE_dep+dE_joule-dE_sink)/TES_HeatCapacity;
	curr_I+=((V-curr_I*R_load-curr_I*R_TES)*TIME_RESOLUTION*1e-3)/INDUCTANCE;
}


long long int Get_BookSignal(double Edep, double time, double &curr_temp)
{
	time*=1e-3;
	double R_L=0,R_0=0.001,I_0=25e-6,L=INDUCTOR_WINDINGS*MU_0/(2*INDUCTOR_RADIUS);
	if(curr_temp-TES_OP_TEMP<1e-12)curr_temp=TES_OP_TEMP;
	double TES_HC=6.25e18*(1.008*curr_temp+0.0346*curr_temp*curr_temp*curr_temp)*1e-3*TMath::Pi()*2*2*2e-5*19/184;
//	double G=2*TMath::Power(curr_temp,1);
//	double G=(300*1e-6*2)*6.25e18*1e-3*1e-4*1.75e-2;
	double G=(300*1e-6*2)*6.25e18*7.0e-5;
	double alpha_I=TES_OP_TEMP/R_0;
	alpha_I*=(curr_temp>TES_OP_TEMP+1e-3)?0.005/6e-3:0.014/1e-3;
	double beta_I=0; // Dependence of R on I
	double El_I=(I_0*R_0)*alpha_I/(G*TES_OP_TEMP);
	double tau=TES_HC/G;
	double tau_el=L/(R_L+R_0*(1+beta_I));
	double tau_I=tau/(1-El_I);
	double tmpvar=TMath::Sqrt((1/tau_el-1/tau_I)*(1/tau_el-1/tau_I)-4*R_0*El_I*(2+beta_I)/(L*tau));
	double tau_plus=1/(0.5/tau_el+0.5/tau_el+0.5*tmpvar);
	double tau_minus=1/(0.5/tau_el+0.5/tau_el-0.5*tmpvar);
	curr_temp=TES_OP_TEMP+((1/tau_I-1/tau_plus)*TMath::Power(TMath::E(),-time/tau_minus)+(1/tau_I-1/tau_minus)*TMath::Power(TMath::E(),-time/tau_plus))*Edep/TES_HC/(1/tau_plus-1/tau_minus);
	double TES_current=I_0+(tau_I/tau_plus-1)*(tau_I/tau_minus-1)*Edep*(TMath::Power(TMath::E(),-time/tau_plus)-TMath::Power(TMath::E(),-time/tau_minus))/((2+beta_I)*I_0*R_0*tau_I*tau_I*(1/tau_plus-1/tau_minus));
	double mag_flux=INDUCTOR_WINDINGS*MU_0/(2*INDUCTOR_RADIUS)*TES_current;
//	std::cout<<time<<" "<<tau<<" "<<tau_I<<" "<<tau_plus<<" "<<tau_minus<<" "<<((1/tau_I-1/tau_plus)*TMath::Power(TMath::E(),-time/tau_minus)+(1/tau_I-1/tau_minus)*TMath::Power(TMath::E(),-time/tau_plus))*Edep/TES_HC/(1/tau_plus-1/tau_minus)<<" "<<curr_temp<<" "<<Edep/TES_HC<<std::endl;
//	return lround(mag_flux/MAGNETIC_FLUX_QUANTUM);
	return TES_current;
}
/*
#define NO_OF_VARIATIONS 10
void Check_BookSignal()
{
	double Edep=10e3, curr_temp=41e-3;
	TGraph *Graph_Arr=new TGraph()[NO_OF_VARIATIONS]; 
	TCanvas *canva=new TCanvas();
	for(int i=1;i<NO_OF_VARIATIONS+1;i++){
	double R_L=0,R_0=0.001,I_0=25e-6,L=TES_INDUCTOR_WINDINGS*i*MU_0/(2*TES_INDUCTOR_RADIUS);
	double G=(300*1e-6*2)*6.25e18*8.25e-4;
	for(double time=-5;time<100;time+=0.1){
	if(time<0){Graph_Arr[i]->AddPoint(time,curr_temp);continue;}
	if(curr_temp-TES_OP_TEMP<1e-12)curr_temp=TES_OP_TEMP;
	double TES_HC=6.25e18*(1.008*curr_temp+0.0346*curr_temp*curr_temp*curr_temp)*1e-3*TMath::Pi()*2*2*2e-5*19/184;
//	double G=2*TMath::Power(curr_temp,1);
//	double G=(300*1e-6*2)*6.25e18*1e-3*1e-4*1.75e-2;
	double alpha_I=TES_OP_TEMP/R_0;
	alpha_I*=(curr_temp>TES_OP_TEMP+1e-3)?0.005/6e-3:0.014/1e-3;
	double beta_I=0; // Dependence of R on I
	double El_I=(I_0*R_0)*alpha_I/(G*TES_OP_TEMP);
	double tau=TES_HC/G;
	double tau_el=L/(R_L+R_0*(1+beta_I));
	double tau_I=tau/(1-El_I);
	double tmpvar=TMath::Sqrt((1/tau_el-1/tau_I)*(1/tau_el-1/tau_I)-4*R_0*El_I*(2+beta_I)/(L*tau));
	double tau_plus=1/(0.5/tau_el+0.5/tau_el+0.5*tmpvar);
	double tau_minus=1/(0.5/tau_el+0.5/tau_el-0.5*tmpvar);
	curr_temp=TES_OP_TEMP+((1/tau_I-1/tau_plus)*TMath::Power(TMath::E(),-time/tau_minus)+(1/tau_I-1/tau_minus)*TMath::Power(TMath::E(),-time/tau_plus))*Edep/TES_HC/(1/tau_plus-1/tau_minus);
	double TES_current=I_0+(tau_I/tau_plus-1)*(tau_I/tau_minus-1)*Edep*(TMath::Power(TMath::E(),-time/tau_plus)-TMath::Power(TMath::E(),-time/tau_minus))/((2+beta_I)*I_0*R_0*tau_I*tau_I*(1/tau_plus-1/tau_minus));
	double mag_flux=TES_INDUCTOR_WINDINGS*MU_0/(2*TES_INDUCTOR_RADIUS)*TES_current;
	Graph_Arr[i]->AddPoint(time,curr_temp);
	}
	Graph_Arr[i]->SetLineColor(i);
	Graph_Arr[i]->Draw("same");
	}
	canva->SaveAs("L_variation_TempSignal.pdf");
	delete Graph_Arr;
	delete canva;
}
*/



/*
void Get_Tloss(double &curr_temp, double delta_time)
{
	// (T2**3)/3+T2*T_sink**2+T3**2*T_sink/2+T_sink**3*log(T2-T_sink)=T1_term-R0_of_heatsink*t/K_HC
	curr_temp*=1e-3;
	double T_sink=20*1e-3, R0_of_heatsink=(300*4e-6*2)*6.25e18;
	T1term=curr_temp*curr_temp*curr_temp/3 + curr_temp*T_sink*T_sink + curr_temp*curr_temp*T_sink/2+TMath::ln(curr_temp-T_sink);
	double diff
	while()
	{
	}
}
*/
