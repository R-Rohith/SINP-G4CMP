/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// 20241024 Israel Hernandez -- IIT, QSC and Fermilab
// 20250101 M. Kelsey -- G4CMP-434: Make output file thread-safe

#include "Caustic_PhononSensitivity.hh"
#include "G4CMPElectrodeHit.hh"
#include "G4CMPUtils.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "Caustic_PhononConfigManager.hh"

#include<iostream>
#include "G4ThreeVector.hh"
using namespace std;

Caustic_PhononSensitivity::Caustic_PhononSensitivity(G4String name) :
  G4CMPElectrodeSensitivity(name), fileName("") {
  SetOutputFile(G4CMP::DebuggingFileThread(Caustic_PhononConfigManager::GetHitOutput()));
}



Caustic_PhononSensitivity::~Caustic_PhononSensitivity() {
  if (output.is_open()) output.close();
  if (!output.good()) {
    G4cerr << "Error closing output file, " << fileName << ".\n"
           << "Expect bad things like loss of data." << G4endl;
  }
}

void Caustic_PhononSensitivity::EndOfEvent(G4HCofThisEvent* HCE) {
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol = static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  std::vector<G4CMPElectrodeHit*>* hitVec = hitCol->GetVector();

  G4RunManager* runMan = G4RunManager::GetRunManager();

  if (output.good()) {
    // Saving in a txt file the Final Phonon Position.
    for (G4CMPElectrodeHit* hit : *hitVec) {
      output << runMan->GetCurrentEvent()->GetEventID() << '\t'
	     << hit->GetTrackID() << '\t'
	     << hit->GetParticleName() << '\t'
	     << hit->GetFinalPosition().getX()/m << '\t'
	     << hit->GetFinalPosition().getY()/m << '\t'
	     << hit->GetFinalPosition().getZ()/m << '\t'
	     << hit->GetStartPosition().getX()/mm << ' '
             << hit->GetStartPosition().getY()/mm << ' '
             << hit->GetStartPosition().getZ()/mm << ' '
//	    << hit->GetEnergyDeposit()/eV<<'\t'
//	    <<  hit->GetStartPosition().getX()/mm* hit->GetStartPosition().getX()/mm+ hit->GetStartPosition().getY()/mm* hit->GetStartPosition().getY()/mm
	     << std::endl;
    }
  }
}

void Caustic_PhononSensitivity::SetOutputFile(const G4String &fn) {
  if (fileName != fn) {
    if (output.is_open()) output.close();
    fileName = fn;
    output.open(fileName, std::ios_base::out);
    if (!output.good()) {
      G4ExceptionDescription msg;
      msg << "Error opening output file " << fileName;
      G4Exception("PhononSensitivity::SetOutputFile", "PhonSense003",
                  FatalException, msg);
      output.close();
    } else {

    }
  }
}

G4bool Caustic_PhononSensitivity::IsHit(const G4Step* step,
                                const G4TouchableHistory*) const {
  /* Phonons tracks are sometimes killed at the boundary in order to spawn new
   * phonon tracks. These tracks that are killed deposit no energy and should
   * not be picked up as hits.
   */
//  try{
  const G4Track* track = step->GetTrack();
  const G4StepPoint* postStepPoint = step->GetPostStepPoint();
  const G4ThreeVector finalposition= postStepPoint->GetPosition();
  const G4ParticleDefinition* particle = track->GetDefinition();
// Including this to save only the data on the Aluminum Detector
  const G4TouchableHandle touch1 = postStepPoint->GetTouchableHandle();
  const G4VPhysicalVolume* volume = touch1->GetVolume();
// When the particle reaches the end of the world
  if(volume==nullptr)return false;
//-----------------------------------------------
  const G4String name = volume->GetName();
  G4bool correctParticle = particle == G4PhononLong::Definition() ||
                           particle == G4PhononTransFast::Definition() ||
                           particle == G4PhononTransSlow::Definition();
// Saving only the phonon that impacts into the Aluminum Sensor
  G4bool correctStatus = step->GetTrack()->GetTrackStatus() == fStopAndKill &&
                         postStepPoint->GetStepStatus() == fGeomBoundary &&
                         step->GetNonIonizingEnergyDeposit() > 0. && name=="BolometerPhysical";
  return correctParticle && correctStatus;
}
