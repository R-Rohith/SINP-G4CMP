
/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/


// 20241024 Israel Hernandez -- IIT, QSC and Fermilab

#include "Caustic_PhononPrimaryGeneratorAction.hh"

#include "G4CMPEnergyPartition.hh"
#include "G4Event.hh"
#include "G4Geantino.hh"
#include "G4ParticleGun.hh"
#include "G4Electron.hh" // check
#include "G4RandomDirection.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4PhononLong.hh"
#include "G4SystemOfUnits.hh"
#include "G4CMPEqEMField.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include<iostream>
#include <vector>
#include <cmath>

#define MATH_PI 3.14159265359

using namespace std;

Caustic_PhononPrimaryGeneratorAction::Caustic_PhononPrimaryGeneratorAction() {

  // cout<<"Before opening"<<endl;
  fParticleGun  = new G4GeneralParticleSource();
  // cout<<"Before file selection"<<endl;
  fInputFile = TFile::Open("/home/ubuntu/SINP-G4CMP/CEvNS/RecoilHistogram.root");
  // cout<<"After file selection"<<endl;
  fRecoilHist = (TH1D*)fInputFile->Get("h_En");
  // cout<<"After opening"<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


Caustic_PhononPrimaryGeneratorAction::~Caustic_PhononPrimaryGeneratorAction() {
   if(fInputFile) {
        fInputFile->Close();
        delete fInputFile;
   }
   delete fParticleGun;
  // cout<<"After destruction"<<endl;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void Caustic_PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  // Defining 50% of the initial  population for TransFast and TransSlow phonons.
  // This is only to distinguish the Phonons Caustics
  // Note.- This is not the initial density of States of the Substrate
  // G4double selector = G4UniformRand();
  // if (selector<0.5) {
  //   fParticleGun->SetParticleDefinition(G4PhononTransFast::Definition());
  // }
  // else{
  //    fParticleGun->SetParticleDefinition(G4PhononTransSlow::Definition());
  // // fParticleGun->SetParticleDefinition(G4Electron::Definition());
  // // fParticleGun->SetParticleDefinition(G4PhononLong::Definition());
  // // If you are interested in Longitudinal phonons.
  // // You only need to uncomment and comment on the other fParticleGun.
  // }
  // // cout<<"Event Generation"<<endl;
  // G4double T_keV = fRecoilHist->GetRandom();  //This is for phonon generation from random histogram energy
  // //G4double T_GeV = T_keV * 1e-6;
  // //G4cout << "The random energy: " << T_keV << G4endl;
  // fParticleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(T_keV * keV); //This is for phonon generation from random histogram energy
  // fParticleGun->GeneratePrimaryVertex(anEvent);

  //This Part is Added from Signal response code to generate phonons of particular energy by partion 
    const G4double recoilEnergy = fRecoilHist->GetRandom()*keV;
    // cout<<"The the recoil energy is "<<E_R/keV<<"\n";
    const G4ThreeVector pos = SampleEventVertex();
    EventAction* eventAction =
   (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();// This is added for TTree

      eventAction->SetRecoilEnergy(recoilEnergy);// This is added for TTree

  // --- G4CMP partition (tell it “W-184” by AAAZZZ code)
  constexpr int PDG_W184 = 184074;
  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=PDG_W184, /*Etotal=*/recoilEnergy, /*eNIEL=*/0.0); // This is needed to be changed depending upon process.
  //std::cout<<"RECOIL Energy: "<<E_R/eV<<std::endl;
  // grab the generated secondaries as primaries
  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // optional: relabel phonon polarizations to chosen fractions
  const G4double fracTS = 0.50, fracTF = 0.35;
  long int no_of_phonons=0;
  for (auto* p : prims) {
	  no_of_phonons+=p->GetWeight();
    auto* pd = p->GetParticleDefinition();
    if (pd == G4PhononLong::Definition()
     || pd == G4PhononTransSlow::Definition()
     || pd == G4PhononTransFast::Definition()) {
      const G4double u = G4UniformRand();
      if      (u < fracTS)            p->SetParticleDefinition(G4PhononTransSlow::Definition());
      else if (u < fracTS + fracTF)   p->SetParticleDefinition(G4PhononTransFast::Definition());
      else                            p->SetParticleDefinition(G4PhononLong::Definition());
    }
  }
  
  // fout<<v/km*s<<'\t'<<theta<<'\t'<<E_R/eV<<'\t'<<pos[0]<<'\t'<<pos[1]<<'\t'<<pos[2]<<'\t'<<no_of_phonons<<'\t'<<part.LindhardScalingFactor(E_R,8,16)<<std::endl;
  // make vertex and attach primaries
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  anEvent->AddPrimaryVertex(vtx);

}
  else{
     fParticleGun->SetParticleDefinition(G4PhononTransSlow::Definition());
  // fParticleGun->SetParticleDefinition(G4Electron::Definition());
  // fParticleGun->SetParticleDefinition(G4PhononLong::Definition());
  // If you are interested in Longitudinal phonons.
  // You only need to uncomment and comment on the other fParticleGun.
  }
  // cout<<"Event Generation"<<endl;
  // G4double T_keV = fRecoilHist->GetRandom(); // This is to generate primaries from random histogram
  //G4double T_GeV = T_keV * 1e-6;
  fParticleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(T_keV * keV);
  fParticleGun->GeneratePrimaryVertex(anEvent);

G4ThreeVector Caustic_PhononPrimaryGeneratorAction::SampleEventVertex() const {
  const G4double R     = 2.0 * cm;
  const G4double Hhalf = 2.0 * cm;
  const G4double r   = R * std::sqrt(G4UniformRand());
  const G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  const G4double x   = r * std::cos(phi);
  const G4double y   = r * std::sin(phi);
  const G4double z   = (2.0 * G4UniformRand() - 1.0) * Hhalf;
  return {x, y, z};
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
