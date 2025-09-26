#include "RunAction.hh"
#include "G4Run.hh"

RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
  // no-op (text logging is done by the primary generator)
}

void RunAction::EndOfRunAction(const G4Run*) {
  // no-op
}
