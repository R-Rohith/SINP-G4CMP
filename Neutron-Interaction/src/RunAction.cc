#include "RunAction.hh"
#include "G4Run.hh"
#include "g4analysis.hh"

RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
  auto* ana = G4AnalysisManager::Instance();
  ana->OpenFile("dm_events.root");

  ana->CreateNtuple("dm","per-event");
  ana->CreateNtupleIColumn("event_id");
  ana->CreateNtupleDColumn("ER_eV");
  ana->CreateNtupleDColumn("theta_rad");
  ana->CreateNtupleDColumn("v_kms");
  ana->CreateNtupleDColumn("x_cm");
  ana->CreateNtupleDColumn("y_cm");
  ana->CreateNtupleDColumn("z_cm");
  ana->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*) {
  auto* ana = G4AnalysisManager::Instance();
  ana->Write();
  ana->CloseFile();
}
