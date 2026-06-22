#include "SMRunAction.hh"
#include "G4Run.hh"
#include "g4root.hh"

#include <iostream>

SMRunAction::SMRunAction() : G4UserRunAction() {}
SMRunAction::~SMRunAction() = default;

void SMRunAction::BeginOfRunAction(const G4Run*) {
   
  G4AnalysisManager *ana = G4AnalysisManager::Instance();
  ana->SetCompressionLevel(6);

  ana->CreateNtuple("Per_run_data","Per_run_data");
  ana->CreateNtupleIColumn("Run_ID");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("PID");
  ana->CreateNtupleDColumn("Initial_energy_MeV");
  ana->CreateNtupleDColumn("Initial_posx_cm");
  ana->CreateNtupleDColumn("Initial_posy_cm");
  ana->CreateNtupleDColumn("Initial_posz_cm");
  ana->CreateNtupleDColumn("Initial_theta");
  ana->CreateNtupleDColumn("Initial_phi");
  ana->CreateNtupleDColumn("Initial_dirx");
  ana->CreateNtupleDColumn("Initial_diry");
  ana->CreateNtupleDColumn("Initial_dirz");
  ana->CreateNtupleDColumn("Energy_dep_eV");
  ana->CreateNtupleDColumn("NSecondaries");
  ana->CreateNtupleDColumn("NPhononL");
  ana->CreateNtupleDColumn("NPhononTF");
  ana->CreateNtupleDColumn("NPhononTS");
  ana->FinishNtuple();
  
  ana->CreateNtuple("Per_event_data","Per_event_data");
  ana->CreateNtupleIColumn("Run_ID");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("Track_ID");
  ana->CreateNtupleSColumn("Particle Name");
  ana->CreateNtupleDColumn("Start_energy_eV");
  ana->CreateNtupleDColumn("Start_positionX_cm");
  ana->CreateNtupleDColumn("Start_positionY_cm");
  ana->CreateNtupleDColumn("Start_positionZ_cm");
  ana->CreateNtupleDColumn("Start_time_ns");
  ana->CreateNtupleDColumn("Deposited_energy_eV");
  ana->CreateNtupleDColumn("Weight");
  ana->CreateNtupleDColumn("Final_positionX_cm");
  ana->CreateNtupleDColumn("Final_positionY_cm");
  ana->CreateNtupleDColumn("Final_positionZ_cm");
  ana->CreateNtupleDColumn("Final_time_ns");
  ana->FinishNtuple();

  ana->OpenFile("Simulation_output");
}

void SMRunAction::EndOfRunAction(const G4Run*) {
  auto* ana = G4AnalysisManager::Instance();
//  if(IsMaster())
//  {
	  ana->Write();
  ana->CloseFile();
//  }
}
