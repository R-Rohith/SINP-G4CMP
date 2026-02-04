#include "DMRunAction.hh"
#include "G4Run.hh"
#include "G4Threading.hh"   // G4Threading::IsMasterThread()
#include "g4root.hh"

DMRunAction::DMRunAction() = default;
DMRunAction::~DMRunAction() = default;

void DMRunAction::BeginOfRunAction(const G4Run*) {
  // Only the master thread opens/closes the shared file
  G4AnalysisManager *ana = G4AnalysisManager::Instance();

  ana->CreateNtuple("Per_run_data","Per_run_data");
  ana->CreateNtupleIColumn("Run_ID");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("Nucleus_PID");
  ana->CreateNtupleDColumn("DM_mass");
  ana->CreateNtupleDColumn("DM_velocity");
  ana->CreateNtupleDColumn("Initial_posx");
  ana->CreateNtupleDColumn("Initial_posy");
  ana->CreateNtupleDColumn("Initial_posz");
  ana->CreateNtupleDColumn("Recoil_theta");
  ana->CreateNtupleDColumn("Recoil_Energy");
  ana->CreateNtupleDColumn("NSec");
  ana->CreateNtupleDColumn("NPhononL");
  ana->CreateNtupleDColumn("NPhononTF");
  ana->CreateNtupleDColumn("NPhononTS");
  ana->FinishNtuple();

  ana->CreateNtuple("Per_event_data","Per_event_data");
  ana->CreateNtupleIColumn("Run_ID");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("Track_ID");
  ana->CreateNtupleSColumn("Particle_Name");
  ana->CreateNtupleDColumn("Start_energy");
  ana->CreateNtupleDColumn("Start_positionX");
  ana->CreateNtupleDColumn("Start_positionY");
  ana->CreateNtupleDColumn("Start_positionZ");
  ana->CreateNtupleDColumn("Start_time");
  ana->CreateNtupleDColumn("Deposited_energy");
  ana->CreateNtupleIColumn("Track_weight");
  ana->CreateNtupleDColumn("Final_positionX");
  ana->CreateNtupleDColumn("Final_positionY");
  ana->CreateNtupleDColumn("Final_positionZ");
  ana->CreateNtupleDColumn("Final_time");
  ana->FinishNtuple();

  ana->OpenFile("Data_output");
}

void DMRunAction::EndOfRunAction(const G4Run*) {
	auto* ana = G4AnalysisManager::Instance();
	ana->Write();
	ana->CloseFile();
}

