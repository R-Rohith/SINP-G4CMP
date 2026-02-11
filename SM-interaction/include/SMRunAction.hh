#pragma once
#include "G4UserRunAction.hh"

class G4Run;

class SMRunAction : public G4UserRunAction {
public:
  SMRunAction();
  ~SMRunAction() override;

  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;
};
