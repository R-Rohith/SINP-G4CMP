#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "SMEventAction.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

#include <cmath>

#include "SMSteppingAction.hh"
#include "SMConfigManager.hh"

SMSteppingAction::SMSteppingAction(SMEventAction* eventAction)
 : G4UserSteppingAction(),
   fEventAction(eventAction)
{}

SMSteppingAction::~SMSteppingAction(){}

void SMSteppingAction::UserSteppingAction(const G4Step* step) 
{
 if(std::abs(step->GetTrack()->GetDefinition()->GetPDGEncoding())!=SMConfigManager::GetPrimPartPDG())return;
 if(step->GetTrack()->GetGlobalTime()>10*s) step->GetTrack()->SetTrackStatus(fStopAndKill);
 G4double edep = step->GetTotalEnergyDeposit()/eV;
// G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
 const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
 G4double weight=0,NphononL=0,NphononTF=0,NphononTS=0;

 for (auto secTrack : *secondaries) {
  G4ParticleDefinition *particle = secTrack->GetDefinition();
	  if (particle==G4PhononLong::Definition()) NphononL+=secTrack->GetWeight();
	  else if(particle==G4PhononTransFast::Definition()) NphononTF+=secTrack->GetWeight();
	  else if(particle==G4PhononTransSlow::Definition()) NphononTS+=secTrack->GetWeight();
  weight+=secTrack->GetWeight();
//  G4double energy = secTrack->GetKineticEnergy();
 }
 fEventAction->AddSteppingData(edep,weight,NphononL,NphononTF,NphononTS);
}

