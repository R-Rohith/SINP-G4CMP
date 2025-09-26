#include "PhononPrimaryGeneratorAction.hh"

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

// --- our MT-safe logger
#include "RunAction.hh"

// --- std
#include <cmath>
#include <vector>

PhononPrimaryGeneratorAction::PhononPrimaryGeneratorAction() {
  G4CMPConfigManager::SetVerboseLevel(0);
}
PhononPrimaryGeneratorAction::~PhononPrimaryGeneratorAction() = default;

void PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  // 1) DM kinematics
  const G4double m_DM = 1.0 * GeV;
  const G4double m_T  = 184.0 * CLHEP::amu_c2;

  const G4ThreeVector v_gal = SampleDMVelocity_Galactic();
  const G4double v    = v_gal.mag();
  const G4double beta = v / CLHEP::c_light;

  const G4double theta = SampleThetaIsotropic();
  const G4double cosTh = std::cos(theta);

  const G4double mu  = (m_DM * m_T) / (m_DM + m_T);
  const G4double E_R = 2.0 * (mu * mu / m_T) * (beta * beta) * (1.0 - cosTh);

  // 2) Sample vertex inside cylinder (R=2 cm, H=4 cm, centered)
  const G4ThreeVector pos = SampleEventVertex();

  // 3) Partition with CMP (position-based)
  constexpr int PDG_W184 = 184074; // AAAZZZ for W-184
  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=*/PDG_W184, /*Etotal=*/E_R, /*eNIEL=*/0.0);

  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // Optional: mode mix
  const G4double fracTS = 0.475, fracTF = 0.408;
  for (auto* p : prims) {
    auto* pd = p->GetParticleDefinition();
    if (pd == G4PhononLong::Definition()
     || pd == G4PhononTransSlow::Definition()
     || pd == G4PhononTransFast::Definition()) {
      const G4double u = G4UniformRand();
      if      (u < fracTS)            p->SetParticleDefinition(G4PhononTransSlow::Definition());
      else if (u < fracTS + fracTF)   p->SetParticleDefinition(G4PhononTransFast::Definition());
      else                             p->SetParticleDefinition(G4PhononLong::Definition());
    }
  }

  // 4) Attach primaries
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  event->AddPrimaryVertex(vtx);

  // 5) MT-safe text logging (no g4analysis)
  RunAction::LogRow(event->GetEventID(),
                    E_R / eV, theta, v / (km/s),
                    pos.x()/cm, pos.y()/cm, pos.z()/cm);
}

// --- helpers ---
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
  const G4double u = 2.0 * CLHEP::pi * G4UniformRand();
  return u;
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
