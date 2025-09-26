#include "PhononPrimaryGeneratorAction.hh"

// G4CMP
#include "G4CMPEnergyPartition.hh"
#include "G4PhononLong.hh"
#include "G4PhononTransFast.hh"
#include "G4PhononTransSlow.hh"

// Geant4
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "g4analysis.hh"

// std
#include <cmath>
#include <vector>
#include <iostream>

PhononPrimaryGeneratorAction::PhononPrimaryGeneratorAction() = default;

void PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  // --- masses
  const G4double m_DM = 1.0 * GeV;
  const G4double m_T  = 184.0 * CLHEP::amu_c2;

  // --- DM halo speed (truncated Maxwellian)
  const G4ThreeVector v_gal = SampleDMVelocity_Galactic();
  const G4double v    = v_gal.mag();
  const G4double beta = v / CLHEP::c_light;

  // --- isotropic CM angle
  const G4double theta = SampleThetaIsotropic();
  const G4double cosTh = std::cos(theta);

  // --- recoil energy: ER = 2 μ^2 v^2 / m_T * (1 - cosθ)
  const G4double mu = (m_DM * m_T) / (m_DM + m_T);
  const G4double E_R = (mu * mu / m_T) * (beta * beta) * (1.0 - cosTh);

  // --- sample a start position in a cylinder R=2 cm, H=4 cm (centered)
  const G4ThreeVector pos = SampleEventVertex();

  // --- G4CMP partition (tell it “W-184” by AAAZZZ code)
  constexpr int PDG_W184 = 184074;
  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=*/PDG_W184, /*Etotal=*/E_R, /*eNIEL=*/0.0);
  //std::cout<<"RECOIL Energy: "<<E_R/eV<<std::endl;
  // grab the generated secondaries as primaries
  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // optional: relabel phonon polarizations to chosen fractions
  const G4double fracTS = 0.50, fracTF = 0.35;
  for (auto* p : prims) {
    auto* pd = p->GetParticleDefinition();
    if (pd == G4PhononLong::Definition() ||
        pd == G4PhononTransSlow::Definition() ||
        pd == G4PhononTransFast::Definition()) {
      const G4double u = G4UniformRand();
      if      (u < fracTS)            p->SetParticleDefinition(G4PhononTransSlow::Definition());
      else if (u < fracTS + fracTF)   p->SetParticleDefinition(G4PhononTransFast::Definition());
      else                             p->SetParticleDefinition(G4PhononLong::Definition());
    }
  }

  std::cout<<v/km*s<<'\t'<<theta<<'\t'<<E_R/eV<<'\t'<<pos[0]<<'\t'<<pos[1]<<'\t'<<pos[2]<<'\t'<<prims.size()<<std::endl;
  // make vertex and attach primaries
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  event->AddPrimaryVertex(vtx);

  // --- (nice to have) per-event logging via g4analysis
  //auto* ana = G4AnalysisManager::Instance();
  //ana->FillNtupleIColumn(0, event->GetEventID());
  //ana->FillNtupleDColumn(1, E_R / eV);
  //ana->FillNtupleDColumn(2, theta);
  //ana->FillNtupleDColumn(3, v / (km/s));
  //ana->FillNtupleDColumn(4, pos.x()/cm);
  //ana->FillNtupleDColumn(5, pos.y()/cm);
  //ana->FillNtupleDColumn(6, pos.z()/cm);
  //ana->AddNtupleRow();
}

// ---------- helpers ----------
G4ThreeVector PhononPrimaryGeneratorAction::SampleDMVelocity_Galactic() const {
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

G4double PhononPrimaryGeneratorAction::SampleThetaIsotropic() const {
  const G4double u = G4UniformRand();
  return std::acos(1.0 - 2.0*u);
}

G4ThreeVector PhononPrimaryGeneratorAction::SampleEventVertex() const {
  const G4double R     = 2.0 * cm;
  const G4double Hhalf = 2.0 * cm;
  const G4double r   = R * std::sqrt(G4UniformRand());
  const G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  const G4double x   = r * std::cos(phi);
  const G4double y   = r * std::sin(phi);
  const G4double z   = (2.0 * G4UniformRand() - 1.0) * Hhalf;
  return {x, y, z};
}
