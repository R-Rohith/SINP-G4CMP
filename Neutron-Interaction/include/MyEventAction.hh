// MyEventAction.hh
#include "G4UserEventAction.hh"
#include "globals.hh"

class MyEventAction : public G4UserEventAction {
  public:
    MyEventAction();
    virtual ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void AddSteppingData(G4double,G4double,G4double,G4double,G4double);

  private:
    G4double fEdep;
    G4int fNSecondaries;
    G4int fNPhononL;
    G4int fNPhononTF;
    G4int fNPhononTS;
};

