#include "SMEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "g4root.hh"

#include <iostream>
#include "G4CMPUtils.hh"

SMEventAction::SMEventAction()
 : G4UserEventAction()
{}

SMEventAction::~SMEventAction() {}

void SMEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    fNSecondaries = 0;
    fNPhononL=0;
    fNPhononTF=0;
    fNPhononTS=0;
}

void SMEventAction::EndOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
     auto Ana=G4AnalysisManager::Instance();
 Ana->FillNtupleDColumn(0,12,fEdep);
 Ana->FillNtupleDColumn(0,13,fNSecondaries);
 Ana->FillNtupleDColumn(0,14,fNPhononL);
 Ana->FillNtupleDColumn(0,15,fNPhononTF);
 Ana->FillNtupleDColumn(0,16,fNPhononTS);
 Ana->AddNtupleRow(0);
}
void SMEventAction::AddSteppingData(G4double E,G4double N,G4double NL,G4double NTF,G4double NTS) 
{
    fEdep+=E;
    fNSecondaries+=N;
    fNPhononL+=NL;
    fNPhononTF+=NTF;
    fNPhononTS+=NTS;
}
