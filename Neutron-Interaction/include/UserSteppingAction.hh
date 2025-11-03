#include "G4UserSteppingAction.hh"                                                            
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
 
class MySteppingAction : public G4UserSteppingAction {
public:
void UserSteppingAction(const G4Step*) override;
};
