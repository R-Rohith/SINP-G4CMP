#include "RunAction.hh"
#include "G4Run.hh"
#include "g4analysis.hh"

RunAction::RunAction() = default;
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
  auto* ana = G4AnalysisManager::Instance();
  ana->SetVerboseLevel(0);
  ana->SetNtupleMerging(true);      // MT merging if MT build

  // Pick the output format via the filename extension:
  // - If CSV plugin is available, this writes ASCII CSV.
  // - If not, Geant4 may still create the default (e.g., ROOT) file.
  ana->OpenFile("dm_root.csv");

  ana->CreateNtuple("dm", "per-event");
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
