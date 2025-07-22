/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/


// 20241024 Israel Hernandez -- IIT, QSC and Fermilab

#include "Caustic_PhononPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4Geantino.hh"
#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"
#include "G4PhononLong.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#define MATH_PI 3.14159265359

using namespace std;

Caustic_PhononPrimaryGeneratorAction::Caustic_PhononPrimaryGeneratorAction() {
//----When using gps as source-------
//fParticleGun  = new G4GeneralParticleSource();

//-----When using particle gun-------------
 G4int n_particle=1;
 fParticleGun=new G4ParticleGun(n_particle);
 fParticleGun->SetParticleDefinition(G4Geantino::Definition());
 fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
 fParticleGun->SetParticleEnergy(0.0075*eV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


Caustic_PhononPrimaryGeneratorAction::~Caustic_PhononPrimaryGeneratorAction() {
  delete fParticleGun;
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
  // fParticleGun->SetParticleDefinition(G4PhononLong::Definition());
  // If you are interested in Longitudinal phonons.
  // You only need to uncomment and comment on the other fParticleGun.
  }

  G4double randX,randY;
  do{
   randX=(G4UniformRand()*4-2);
   randY=(G4UniformRand()*4-2);
  }while((randX*randX+randY*randY)>=4);
  fParticleGun->SetParticlePosition(G4ThreeVector(randX*mm,randY*mm,0.0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
