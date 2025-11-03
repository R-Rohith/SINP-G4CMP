#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "UserSteppingAction.hh"

void UserSteppingAction(const G4Step* step) override {
 auto Ana=G4AnalysisManager::Instance();
 
 G4double edep = step->GetTotalEnergyDeposit();
 G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
 std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

 for (auto secTrack : *secondaries) {
  G4String name = secTrack->GetDefinition()->GetParticleName();
  G4double weight = secTrack->GetWeight();
  G4double energy = secTrack->GetKineticEnergy();
 }
}

