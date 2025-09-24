#pragma once
#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  ~RunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;
};
