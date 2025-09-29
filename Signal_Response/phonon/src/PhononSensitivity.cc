/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#include "PhononSensitivity.hh"
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
#include "PhononConfigManager.hh"
#include <fstream>

#include"G4CMPUtils.hh"

PhononSensitivity::PhononSensitivity(G4String name) :
  G4CMPElectrodeSensitivity(name), fileName("") {
 // SetOutputFile(PhononConfigManager::GetHitOutput());
  SetOutputFile(G4CMP::DebuggingFileThread(PhononConfigManager::GetHitOutput()));
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

PhononSensitivity::~PhononSensitivity() {
  if (output.is_open()) output.close();
  if (!output.good()) {
    G4cerr << "Error closing output file, " << fileName << ".\n"
           << "Expect bad things like loss of data.";
  }
}

void PhononSensitivity::EndOfEvent(G4HCofThisEvent* HCE) {
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol = static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  std::vector<G4CMPElectrodeHit*>* hitVec = hitCol->GetVector();

  G4RunManager* runMan = G4RunManager::GetRunManager();

  if (output.good()) {
    for (G4CMPElectrodeHit* hit : *hitVec) {
      output << runMan->GetCurrentRun()->GetRunID() << '\t'
             << runMan->GetCurrentEvent()->GetEventID() << '\t'
             << hit->GetTrackID() << '\t'
             << hit->GetParticleName() << '\t'
             << hit->GetStartEnergy()/eV << '\t'
             << hit->GetStartPosition().getX()/m << '\t'
             << hit->GetStartPosition().getY()/m << '\t'
             << hit->GetStartPosition().getZ()/m << '\t'
             << hit->GetStartTime()/ns << '\t'
             << hit->GetEnergyDeposit()/eV << '\t'
             << hit->GetWeight() << '\t'
             << hit->GetFinalPosition().getX()/m << '\t'
             << hit->GetFinalPosition().getY()/m << '\t'
             << hit->GetFinalPosition().getZ()/m << '\t'
             << hit->GetFinalTime()/ns << '\n';
    }
  }
}

void PhononSensitivity::SetOutputFile(const G4String &fn) {
  if (fileName != fn) {
    if (output.is_open()) output.close();
    fileName = fn;
    std::cout<<"Opening file "<<fileName<<std::endl;
    output.open(fileName, std::ios_base::out);
    if (!output.good()) {
      G4ExceptionDescription msg;
      msg << "Error opening output file " << fileName;
      G4Exception("PhononSensitivity::SetOutputFile", "PhonSense003",
                  FatalException, msg);
      output.close();
    } else {/*
      output << "Run ID,Event ID,Track ID,Particle Name,Start Energy [eV],"
             << "Start X [m],Start Y [m],Start Z [m],Start Time [ns],"
             << "Energy Deposited [eV],Track Weight,End X [m],End Y [m],End Z [m],"
             << "Final Time [ns]\n";*/
    }
  }
}

G4bool PhononSensitivity::IsHit(const G4Step* step,
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
