#include "MyEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ios.hh"
#include "g4root.hh"

#include <iostream>
#include "G4CMPUtils.hh"

MyEventAction::MyEventAction()
 : G4UserEventAction()
{}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
    fEdep = 0.;
    fNSecondaries = 0;
    fNPhononL=0;
    fNPhononTF=0;
    fNPhononTS=0;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
     auto Ana=G4AnalysisManager::Instance();
// Ana->FillNtupleIColumn(0,eventID);
 Ana->FillNtupleDColumn(0,12,fEdep);
 Ana->FillNtupleDColumn(0,13,fNSecondaries);
 Ana->FillNtupleDColumn(0,14,fNPhononL);
 Ana->FillNtupleDColumn(0,15,fNPhononTF);
 Ana->FillNtupleDColumn(0,16,fNPhononTS);
 Ana->AddNtupleRow(0);
/*    std::fstream fout;
fout.open(G4CMP::DebuggingFileThread("Event_output.txt"),std::ios::app);
        if (!fout.good()) {
      G4ExceptionDescription msg;
      msg << "Error opening generator output file ";
      G4Exception("PhononSensitivity::SetOutputFile", "PhonSense003",
                  FatalException, msg);
      fout.close();
    }
	fout<<eventID<<'\t'<<fEdep<<'\t'<<fNSecondaries<<'\t'<<fNPhononL<<'\t'<<fNPhononTF<<'\t'<<fNPhononTS<<std::endl;
	fout.close();*/
// Ana->Write();
}
void MyEventAction::AddSteppingData(G4double E,G4double N,G4double NL,G4double NTF,G4double NTS) 
{
    fEdep+=E;
    fNSecondaries+=N;
    fNPhononL+=NL;
    fNPhononTF+=NTF;
    fNPhononTS+=NTS;
}
