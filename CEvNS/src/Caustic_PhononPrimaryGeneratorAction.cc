
/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/


// 20241024 Israel Hernandez -- IIT, QSC and Fermilab

#include "Caustic_PhononPrimaryGeneratorAction.hh"

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
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include<iostream>

#include <cmath>
#define MATH_PI 3.14159265359

using namespace std;

Caustic_PhononPrimaryGeneratorAction::Caustic_PhononPrimaryGeneratorAction() {

// cout<<"Before opening"<<endl;
fParticleGun  = new G4GeneralParticleSource();
cout<<"Before file selection"<<endl;
fInputFile = TFile::Open("/home/rohit-kumar/softwares/SINP-G4CMP/CEvNS/RecoilHistogram.root");
cout<<"After file selection"<<endl;
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
  G4double selector = G4UniformRand();
  if (selector<0.5) {
    fParticleGun->SetParticleDefinition(G4PhononTransFast::Definition());
  

}
  else{
     fParticleGun->SetParticleDefinition(G4PhononTransSlow::Definition());
  // fParticleGun->SetParticleDefinition(G4Electron::Definition());
  // fParticleGun->SetParticleDefinition(G4PhononLong::Definition());
  // If you are interested in Longitudinal phonons.
  // You only need to uncomment and comment on the other fParticleGun.
  }
  // cout<<"Event Generation"<<endl;
  G4double T_keV = fRecoilHist->GetRandom();
  //G4double T_GeV = T_keV * 1e-6;
  fParticleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(T_keV * keV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
