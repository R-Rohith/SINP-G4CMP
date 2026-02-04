#include "DMPrimaryGeneratorAction.hh"

// --- G4CMP
#include "G4CMPEnergyPartition.hh"
#include "G4CMPConfigManager.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

// --- Geant4
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4CMPUtils.hh"
#include "g4root.hh"

// std
#include <cmath>
#include <vector>
#include <iostream>

DMPrimaryGeneratorAction::DMPrimaryGeneratorAction()// = default;
{
}
DMPrimaryGeneratorAction::~DMPrimaryGeneratorAction(){fout.close();}
void DMPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  // 1) DM kinematics
  const G4double m_DM = 1.0 * GeV;
  G4double rand=G4UniformRand();
  G4double m_T;
  int PDG_A = 16008;
  if(rand<(4/6)) {m_T  = 16.0 * CLHEP::amu_c2;PDG_A=16008;}		// recoil with oxygen
  else if(rand<(5/6)) {m_T  = 32.0 * CLHEP::amu_c2;PDG_A=32040;}	// recoil with calcium 
  else {m_T  = 184.0 * CLHEP::amu_c2;PDG_A=184074;}			// recoil with tungsten
  // --- G4CMP partition (tell it “W-184” by AAAZZZ code)

  const G4ThreeVector v_gal = SampleDMVelocity_Galactic();
  const G4double v    = v_gal.mag();
  const G4double beta = v / CLHEP::c_light;

  const G4double cosTh = SampleThetaIsotropic();
  const G4double theta = std::acos(cosTh);

  const G4double mu  = (m_DM * m_T) / (m_DM + m_T);
  const G4double E_R = 2.0 * (mu * mu / m_T) * (beta * beta) * (1.0 - cosTh);

  // 2) Sample vertex inside cylinder (R=2 cm, H=4 cm, centered)
  const G4ThreeVector pos = SampleEventVertex();

  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=*/PDG_A, /*Etotal=*/E_R, /*eNIEL=*/0.0);
  //std::cout<<"RECOIL Energy: "<<E_R/eV<<std::endl;
  // grab the generated secondaries as primaries
  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // optional: relabel phonon polarizations to chosen fractions
  const G4double fracTS = 0.50, fracTF = 0.35;
  long int no_of_sec=0,nL=0,nTF=0,nTS=0;
  for (auto* p : prims) {
	  no_of_sec+=p->GetWeight();
    auto* pd = p->GetParticleDefinition();
    if (pd == G4PhononLong::Definition()
     || pd == G4PhononTransSlow::Definition()
     || pd == G4PhononTransFast::Definition()) {
      const G4double u = G4UniformRand();
      if      (u < fracTS)            {p->SetParticleDefinition(G4PhononTransSlow::Definition());nTS++;}
      else if (u < fracTS + fracTF)   {p->SetParticleDefinition(G4PhononTransFast::Definition());nTF++;}
      else                            {p->SetParticleDefinition(G4PhononLong::Definition());nL++;}
    }
  }

  G4AnalysisManager *ana = G4AnalysisManager::Instance();
  ana->FillNtupleIColumn(0,0,G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID());
  ana->FillNtupleIColumn(0,1,event->GetEventID());
  ana->FillNtupleIColumn(0,2,PDG_A);
  ana->FillNtupleDColumn(0,3,m_DM/GeV);
  ana->FillNtupleDColumn(0,4,v*s/km);
  ana->FillNtupleDColumn(0,5,pos[0]/cm);
  ana->FillNtupleDColumn(0,6,pos[1]/cm);
  ana->FillNtupleDColumn(0,7,pos[2]/cm);
  ana->FillNtupleDColumn(0,8,theta);
  ana->FillNtupleDColumn(0,9,E_R/eV);
  ana->FillNtupleDColumn(0,10,no_of_sec);
  ana->FillNtupleDColumn(0,11,nL);
  ana->FillNtupleDColumn(0,12,nTF);
  ana->FillNtupleDColumn(0,13,nTS);
  ana->AddNtupleRow(0);
  // make vertex and attach primaries
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  event->AddPrimaryVertex(vtx);
}

// --- helpers ---
G4ThreeVector DMPrimaryGeneratorAction::SampleDMVelocity_Galactic() const {
  const G4double v0    = 220.0 * km / s;
  const G4double vesc  = 533.0 * km / s;
  const G4double sigma = v0 / std::sqrt(2.0);

  G4ThreeVector v;
  do {
    const G4double vx = G4RandGauss::shoot(0., sigma);
    const G4double vy = G4RandGauss::shoot(0., sigma);
    const G4double vz = G4RandGauss::shoot(0., sigma);
    v.set(vx, vy, vz);
  } while (v.mag() > vesc);
  return v;
}

G4double DMPrimaryGeneratorAction::SampleThetaIsotropic() const {
//  const G4double u = 2.0 * CLHEP::pi * G4UniformRand();
  G4double rand=G4UniformRand();
  return 2.0*rand*rand-1;
}

G4ThreeVector DMPrimaryGeneratorAction::SampleEventVertex() const {
  const G4double R     = 2.0 * cm;
  const G4double Hhalf = 2.0 * cm;
  const G4double r   = R * std::sqrt(G4UniformRand());
  const G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  const G4double x   = r * std::cos(phi);
  const G4double y   = r * std::sin(phi);
  const G4double z   = (2.0 * G4UniformRand() - 1.0) * Hhalf;
  return {x, y, z};
}
