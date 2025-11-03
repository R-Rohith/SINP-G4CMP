#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

class MySteppingAction : public G4UserSteppingAction {
public:
    void UserSteppingAction(const G4Step* step) override {
        // Energy deposited in this step
        G4double edep = step->GetTotalEnergyDeposit();

        // Position where it occurred
        G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

        if (edep > 0) {
            G4cout << "Edep = " << edep / keV << " keV"
                   << " at position " << pos << G4endl;
        }
    }
};

