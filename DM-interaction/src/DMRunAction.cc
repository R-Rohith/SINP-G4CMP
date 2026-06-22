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
  ana->CreateNtupleDColumn("Initial_posx_cm");
  ana->CreateNtupleDColumn("Initial_posy_cm");
  ana->CreateNtupleDColumn("Initial_posz_cm");
  ana->CreateNtupleDColumn("Recoil_theta");
  ana->CreateNtupleDColumn("Recoil_Energy_eV");
  ana->CreateNtupleDColumn("NPhononL");
  ana->CreateNtupleDColumn("NPhononTF");
  ana->CreateNtupleDColumn("NPhononTS");
  ana->FinishNtuple();

  ana->CreateNtuple("Per_event_data","Per_event_data");
  ana->CreateNtupleIColumn("Run_ID");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("Track_ID");
  ana->CreateNtupleSColumn("Particle_Name");
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

void DMRunAction::EndOfRunAction(const G4Run*) {
	auto* ana = G4AnalysisManager::Instance();
	ana->Write();
	ana->CloseFile();
}

