#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    // void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

    void SetRecoilEnergy(double E);
    double GetRecoilEnergy() const;

private:
    double recoilEnergy;
};