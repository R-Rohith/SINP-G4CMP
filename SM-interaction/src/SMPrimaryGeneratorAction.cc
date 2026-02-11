#include "SMPrimaryGeneratorAction.hh"

// G4CMP
#include "G4CMPEnergyPartition.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

// Geant4
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "g4root.hh"

#include "G4CMPUtils.hh"

//#include "G4MuonMinus.hh"
//#include "G4MuonPlus.hh"
#include "G4ParticleTypes.hh"

// std
#include <cmath>
#include <vector>
#include <iostream>


SMPrimaryGeneratorAction::SMPrimaryGeneratorAction()// = default;
{
  fParticleGun  = new G4GeneralParticleSource();
  
  PrimFluxFile=TFile::Open("PrimFlux.root"); 
  PrimFlux=(TH1D*)PrimFluxFile->Get("histNeutronEnergy");

}
SMPrimaryGeneratorAction::~SMPrimaryGeneratorAction()
{
	PrimFluxFile->Close();
}
void SMPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {

	/*
	 //--Muon as primary-------

  G4double rand=G4UniformRand();
  G4ParticleDefinition *partType;
  if(rand<0.585)
	  partType=G4MuonMinus::Definition();
  else
	  partType=G4MuonPlus::Definition();
  G4ThreeVector pos, dir;
  G4double r=(11+2+1)*std::sqrt(G4UniformRand())*cm,phiPos=2*CLHEP::pi*G4UniformRand();	//--R_max=(radius of cone with h=4cm & theta=70deg + radius of crystal + lateral displacement of muons)
  pos[0]=r*std::cos(phiPos);
  pos[1]=r*std::sin(phiPos);
  pos[2]=4*cm;	//--The elevation of the muon source surface
  G4double E=200e3*MeV, theta=CLHEP::pi-70*CLHEP::pi/180*G4UniformRand(), phi=2*CLHEP::pi*G4UniformRand();
  dir[0]=std::sin(theta)*std::cos(phi);
  dir[1]=std::sin(theta)*std::sin(phi);
  dir[2]=std::cos(theta);

  do
  {
	  E=1e4*G4UniformRand();
	  G4double prob=G4UniformRand(), thresh=fluxhist->GetBinContent(fluxhist->GetBin(E));
  }while(prob<=thresh);
  E*=MeV;*/

//--Neutron as primary-------

  G4ParticleDefinition *partType;
  if(G4UniformRand()<0.5)
   partType=G4AntiNeutron::Definition();
  else
   partType=G4Neutron::Definition();
  

  G4ThreeVector pos, dir;
  G4double r=4*cm,phiPos=2*CLHEP::pi*G4UniformRand(),thetaPos=CLHEP::pi*G4UniformRand();	
  pos[0]=r*std::sin(thetaPos)*std::cos(phiPos);
  pos[1]=r*std::sin(thetaPos)*std::sin(phiPos);
  pos[2]=r*std::cos(thetaPos);
  G4double E=PrimFlux->GetRandom(), theta=CLHEP::pi*G4UniformRand(), phi=2*CLHEP::pi*G4UniformRand();
  dir[0]=r*std::sin(theta)*std::cos(phi)-pos[0];
  dir[1]=r*std::sin(theta)*std::sin(phi)-pos[1];
  dir[2]=r*std::cos(theta)-pos[2];
  G4double dirmag=std::sqrt(dir[0]*dir[0]+dir[1]*dir[1]+dir[2]*dir[2]);
  if(dirmag==0) std::cout<<"\n\n--Uh Oh... divide by zero------------\n\n";
  dir[0]/=dirmag;
  dir[1]/=dirmag;
  dir[2]/=dirmag;

//  fout<<event->GetEventID()<<'\t'<<partType->GetPDGEncoding()<<'\t'<<E/MeV<<'\t'<<pos[0]/cm<<'\t'<<pos[1]/cm<<'\t'<<pos[2]/cm<<'\t'<<theta<<'\t'<<phi<<'\t'<<dir[0]<<'\t'<<dir[1]<<'\t'<<dir[2]<<std::endl;
 
  G4AnalysisManager *ana = G4AnalysisManager::Instance();
  ana->FillNtupleIColumn(0,0,G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID());
  ana->FillNtupleIColumn(0,1,event->GetEventID());
  ana->FillNtupleIColumn(0,2,partType->GetPDGEncoding());
  ana->FillNtupleDColumn(0,3,E/MeV);
  ana->FillNtupleDColumn(0,4,pos[0]/cm);
  ana->FillNtupleDColumn(0,5,pos[1]/cm);
  ana->FillNtupleDColumn(0,6,pos[2]/cm);
  ana->FillNtupleDColumn(0,7,theta);
  ana->FillNtupleDColumn(0,8,phi);
  ana->FillNtupleDColumn(0,9,dir[0]);
  ana->FillNtupleDColumn(0,10,dir[1]);
  ana->FillNtupleDColumn(0,11,dir[2]);
  
  fParticleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
  fParticleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(dir);
  fParticleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(E);
  fParticleGun->SetParticleDefinition(partType);
  fParticleGun->GeneratePrimaryVertex(event);

  std::cout<<event->GetEventID()<<std::endl;
//  G4ThreeVector pos=fParticleGun->GetParticlePosition(), mom=fParticleGun->GetParticleMomentumDirection();
//  G4double E=fParticleGun->GetParticleEnergy(), theta=std::acos(mom[2]/mom.mag()), phi=(mom[1]<0)?2*CLHEP::pi-std::acos(mom[0]/std::sqrt(mom[0]*mom[0]+mom[1]*mom[1])):std::acos(mom[0]/std::sqrt(mom[0]*mom[0]+mom[1]*mom[1]));
}

