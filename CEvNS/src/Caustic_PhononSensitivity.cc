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
#include "EventAction.hh"
#include "G4RunManager.hh"

#include<iostream>
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"

using namespace std;

// Caustic_PhononSensitivity::Caustic_PhononSensitivity(G4String name) :
//   G4CMPElectrodeSensitivity(name), fileName("") {
//   SetOutputFile(G4CMP::DebuggingFileThread(Caustic_PhononConfigManager::GetHitOutput()));
// }

//This Part is for TTree
Caustic_PhononSensitivity::Caustic_PhononSensitivity(G4String name)
: G4CMPElectrodeSensitivity(name)
{
    rootFile = new TFile(G4CMP::DebuggingFileThread("phononHits.root"),"RECREATE");

    tree = new TTree("hits","Phonon hits");

    tree->Branch("eventID",&eventID);
    tree->Branch("trackID",&trackID);
    tree->Branch("phononName",&phononName);
    tree->Branch("final_x",&final_x);
    tree->Branch("final_y",&final_y);
    tree->Branch("final_z",&final_z);
    tree->Branch("initial_x",&initial_x);
    tree->Branch("initial_y",&initial_y);
    tree->Branch("initial_z",&initial_z);
    tree->Branch("edep",&edep);
    tree->Branch("weight",&weight);

    // tree->Branch("totalEdep",&totalEdep);
    tree->Branch("recoilEnergy",&recoilEnergy);
}

// Caustic_PhononSensitivity::~Caustic_PhononSensitivity() {
//   if (output.is_open()) output.close();
//   if (!output.good()) {
//     G4cerr << "Error closing output file, " << fileName << ".\n"
//            << "Expect bad things like loss of data." << G4endl;
//   }
// }

Caustic_PhononSensitivity::~Caustic_PhononSensitivity()
{
    rootFile->Write();
    rootFile->Close();
}

void Caustic_PhononSensitivity::EndOfEvent(G4HCofThisEvent* HCE) {
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  auto* hitCol = static_cast<G4CMPElectrodeHitsCollection*>(HCE->GetHC(HCID));
  std::vector<G4CMPElectrodeHit*>* hitVec = hitCol->GetVector();

  G4RunManager* runMan = G4RunManager::GetRunManager();

  totalEdep = 0;                                          //For TTree
  eventID = runMan->GetCurrentEvent()->GetEventID();      //For TTree

//   if (output.good()) {
// // Saving in a txt file the Final Phonon Position.  
//     for (G4CMPElectrodeHit* hit : *hitVec) {
//       output << runMan->GetCurrentEvent()->GetEventID() << '\t'
// 	     << hit->GetTrackID() << '\t'
// 	     << hit->GetParticleName() << '\t'
// 	     << hit->GetFinalPosition().getX()/m << '\t'
// 	     << hit->GetFinalPosition().getY()/m << '\t'
// 	     << hit->GetFinalPosition().getZ()/m << '\t'
// 	     << hit->GetStartPosition().getX()/mm << ' '
//        << hit->GetStartPosition().getY()/mm << ' '
//        << hit->GetStartPosition().getZ()/mm << ' '
// 	     << hit->GetEnergyDeposit()/eV<<'\t'
//        <<hit->GetWeight()<<'\t'
// //	    <<  hit->GetStartPosition().getX()/mm* hit->GetStartPosition().getX()/mm+ hit->GetStartPosition().getY()/mm* hit->GetStartPosition().getY()/mm
// 	     << std::endl;
//     }
//   }
  EventAction* eventAction =
   (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();

     recoilEnergy = eventAction->GetRecoilEnergy()/keV;
    // std::cout<<recoilEnergy/keV<<std::endl;
     for (G4CMPElectrodeHit* hit : *hitVec)
    {
    trackID = hit->GetTrackID();
    phononName = hit->GetParticleName();

    final_x = hit->GetFinalPosition().getX()/m;
    final_y = hit->GetFinalPosition().getY()/m;
    final_z = hit->GetFinalPosition().getZ()/m;

    initial_x = hit->GetStartPosition().getX()/mm;
    initial_y = hit->GetStartPosition().getY()/mm;
    initial_z = hit->GetStartPosition().getZ()/mm;

    edep = hit->GetEnergyDeposit()/eV;
    weight = hit->GetWeight();

    // totalEdep += edep * weight; 

    tree->Fill();
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
