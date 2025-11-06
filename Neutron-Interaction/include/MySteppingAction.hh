#include "G4UserSteppingAction.hh"                                                            
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

class MyEventAction;

class MySteppingAction : public G4UserSteppingAction {
public:
MySteppingAction(MyEventAction* eventAction);
virtual ~MySteppingAction();
void UserSteppingAction(const G4Step*) override;
private:
MyEventAction* fEventAction;
};
