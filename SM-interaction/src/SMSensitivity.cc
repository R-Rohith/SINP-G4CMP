/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#include "SMSensitivity.hh"
#include "G4CMPElectrodeHit.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "SMConfigManager.hh"
#include <fstream>

#include"G4CMPUtils.hh"
#include "g4analysis.hh"

SMSensitivity::SMSensitivity(G4String name) :
  G4CMPElectrodeSensitivity(name), fileName("") {
}

/* Move is disabled for now because old versions of GCC can't move ofstream
PhononSensitivity::PhononSensitivity(PhononSensitivity&& in) :
  G4CMPElectrodeSensitivity(std::move(in)),
  output(std::move(in.output)),
  fileName(std::move(in.fileName)) {
}

PhononSensitivity& PhononSensitivity::operator=(PhononSensitivity&& in) {
  // Move all base mebers
  G4CMPElectrodeSensitivity::operator=(std::move(in));

  // Our members
  output.close();
  output = std::move(in.output);
  fileName = in.fileName;

  return *this;
}
*/

SMSensitivity::~SMSensitivity() {
}

void SMSensitivity::EndOfEvent(G4HCofThisEvent* HCE) {
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol = static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  std::vector<G4CMPElectrodeHit*>* hitVec = hitCol->GetVector();

  G4RunManager* runMan = G4RunManager::GetRunManager();

//	double energy=0;

    auto Ana=G4AnalysisManager::Instance();
    for (G4CMPElectrodeHit* hit : *hitVec) {
	  //  energy+=hit->GetEnergyDeposit()/eV;
	Ana->FillNtupleIColumn(1,0,runMan->GetCurrentRun()->GetRunID());
	Ana->FillNtupleIColumn(1,1,runMan->GetCurrentEvent()->GetEventID());
	Ana->FillNtupleIColumn(1,2,hit->GetTrackID());
	Ana->FillNtupleSColumn(1,3,hit->GetParticleName());
	Ana->FillNtupleDColumn(1,4,hit->GetStartEnergy()/eV);
	Ana->FillNtupleDColumn(1,5,hit->GetStartPosition().getX()/m);
	Ana->FillNtupleDColumn(1,6,hit->GetStartPosition().getY()/m);
	Ana->FillNtupleDColumn(1,7,hit->GetStartPosition().getZ()/m);
	Ana->FillNtupleDColumn(1,8,hit->GetStartTime()/ns);
	Ana->FillNtupleDColumn(1,9,hit->GetEnergyDeposit()/eV);
	Ana->FillNtupleDColumn(1,10,hit->GetWeight());
	Ana->FillNtupleDColumn(1,11,hit->GetFinalPosition().getX()/m);
	Ana->FillNtupleDColumn(1,12,hit->GetFinalPosition().getY()/m);
	Ana->FillNtupleDColumn(1,13,hit->GetFinalPosition().getZ()/m);
	Ana->FillNtupleDColumn(1,14,hit->GetFinalTime()/ns);
	Ana->AddNtupleRow(1);
    }
}


G4bool SMSensitivity::IsHit(const G4Step* step,
                                const G4TouchableHistory*) const {
  /* Phonons tracks are sometimes killed at the boundary in order to spawn new
   * phonon tracks. These tracks that are killed deposit no energy and should
   * not be picked up as hits.
   */
  const G4Track* track = step->GetTrack();
  const G4StepPoint* postStepPoint = step->GetPostStepPoint();
  const G4ParticleDefinition* particle = track->GetDefinition();
  const G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
  const G4VPhysicalVolume* volume = touch1->GetVolume();
  if(volume==nullptr)return false;

  G4bool correctParticle = particle == G4PhononLong::Definition() ||
                           particle == G4PhononTransFast::Definition() ||
                           particle == G4PhononTransSlow::Definition();

  G4bool correctStatus = step->GetTrack()->GetTrackStatus() == fStopAndKill &&
                         postStepPoint->GetStepStatus() == fGeomBoundary &&
                         step->GetNonIonizingEnergyDeposit() > 0.;

  return correctParticle && correctStatus;
}
