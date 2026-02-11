#include "G4UserSteppingAction.hh"                                                            
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

class SMEventAction;

class SMSteppingAction : public G4UserSteppingAction {
public:
SMSteppingAction(SMEventAction* eventAction);
virtual ~SMSteppingAction();
void UserSteppingAction(const G4Step*) override;
private:
SMEventAction* fEventAction;
};
