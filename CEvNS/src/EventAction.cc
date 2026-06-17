#include "EventAction.hh"
#include "G4Event.hh"
#include <iostream>

EventAction::EventAction()
{
    recoilEnergy = 0;
}

EventAction::~EventAction() {}

void EventAction::EndOfEventAction(const G4Event*)
{
    recoilEnergy = 0;   // reset every event
}

void EventAction::SetRecoilEnergy(double E)
{
    recoilEnergy = E;
}

double EventAction::GetRecoilEnergy() const
{
    return recoilEnergy;
}