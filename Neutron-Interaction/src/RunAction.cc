#include "RunAction.hh"
#include "G4Run.hh"
#include "g4root.hh"

#include <iostream>

RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
/*  auto* ana = G4AnalysisManager::Instance();
  ana->OpenFile("dm_events.root");

  ana->CreateNtuple("dm","per-event");
  ana->CreateNtupleIColumn("event_id");
  ana->CreateNtupleDColumn("ER_eV");
  ana->CreateNtupleDColumn("theta_rad");
  ana->CreateNtupleDColumn("v_kms");
  ana->CreateNtupleDColumn("x_cm");
  ana->CreateNtupleDColumn("y_cm");
  ana->CreateNtupleDColumn("z_cm");
  ana->FinishNtuple();*/
   
  G4AnalysisManager *ana = G4AnalysisManager::Instance();
//  if(IsMaster())
//  {
  ana->OpenFile("Generator_output");
//  }
   
  ana->CreateNtuple("Per_event_data","Per_event_data");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("PID");
  ana->CreateNtupleDColumn("Initial_energy");
  ana->CreateNtupleDColumn("Initial_posx");
  ana->CreateNtupleDColumn("Initial_posy");
  ana->CreateNtupleDColumn("Initial_posz");
  ana->CreateNtupleDColumn("Initial_theta");
  ana->CreateNtupleDColumn("Initial_phi");
  ana->CreateNtupleDColumn("Initial_dirx");
  ana->CreateNtupleDColumn("Initial_diry");
  ana->CreateNtupleDColumn("Initial_dirz");
  ana->CreateNtupleDColumn("Energy_dep");
  ana->CreateNtupleDColumn("NSecondaries");
  ana->CreateNtupleDColumn("NPhononL");
  ana->CreateNtupleDColumn("NPhononTF");
  ana->CreateNtupleDColumn("NPhononTS");
  std::cout<<"Created Tuples\n";
}

void RunAction::EndOfRunAction(const G4Run*) {
  auto* ana = G4AnalysisManager::Instance();
//  if(IsMaster())
//  {
	  ana->Write();
  ana->CloseFile();
//  }
}
