#pragma once
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

#include "G4GeneralParticleSource.hh"

#include<fstream>

// ROOT
#include "TFile.h"
#include "TH1.h"

class G4Event;

class SMPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  SMPrimaryGeneratorAction();
  ~SMPrimaryGeneratorAction();	// override = default;
  void GeneratePrimaries(G4Event* event) override;

private:
  std::ofstream fout;
  G4GeneralParticleSource *fParticleGun;
  TFile *PrimFluxFile;
  TH1D *PrimFlux;
};
