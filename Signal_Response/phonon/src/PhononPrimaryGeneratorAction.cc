// PhononPrimaryGeneratorAction.cc
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

// (for ntuple filling → RunAction opens dm_root.txt via G4AnalysisManager)
#include "g4analysis.hh"

// --- std
#include <cmath>
#include <vector>

PhononPrimaryGeneratorAction::PhononPrimaryGeneratorAction() {
  // Keep G4CMP messages quiet unless you want verbosity
  static bool once = false;
  if (!once) { G4CMPConfigManager::SetVerboseLevel(0); once = true; }
}

PhononPrimaryGeneratorAction::~PhononPrimaryGeneratorAction() = default;

void PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  // -----------------------------
  // 1) Dark-matter kinematics
  // -----------------------------
  const G4double m_DM = 1.0 * GeV;                 // dark matter mass
  const G4double m_T  = 184.0 * CLHEP::amu_c2;     // W-184 target nucleus mass

  const G4ThreeVector v_gal = SampleDMVelocity_Galactic(); // (m/s)
  const G4double v    = v_gal.mag();
  const G4double beta = v / CLHEP::c_light;                 // dimensionless

  const G4double theta = SampleThetaIsotropic();            // [rad]
  const G4double cosTh = std::cos(theta);

  // Elastic recoil (non-relativistic): ER = 2 μ^2 v^2 / m_T * (1 - cosθ)
  const G4double mu  = (m_DM * m_T) / (m_DM + m_T);
  const G4double E_R = (mu * mu / m_T) * (beta * beta) * (1.0 - cosTh); // [energy units]

  // -----------------------------
  // 2) Sample event vertex
  // -----------------------------
  const G4ThreeVector pos = SampleEventVertex(); // uniform in cylinder R=2 cm, H=4 cm

  // -----------------------------
  // 3) Partition with G4CMP
  // -----------------------------
  // Use position-based ctor (no PV dependencies)
  constexpr int PDG_W184 = 184074;         // AAAZZZ pseudo-PDG for W-184
  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=*/PDG_W184, /*Etotal=*/E_R, /*eNIEL=*/0.0);

  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // Optional: impose a starting mode mix (TS/TF/L)
  const G4double fracTS = 0.50;
  const G4double fracTF = 0.35;
  for (auto* p : prims) {
    auto* pd = p->GetParticleDefinition();
    if (pd == G4PhononLong::Definition()
     || pd == G4PhononTransSlow::Definition()
     || pd == G4PhononTransFast::Definition()) {
      const G4double u = G4UniformRand();
      if      (u < fracTS)              p->SetParticleDefinition(G4PhononTransSlow::Definition());
      else if (u < fracTS + fracTF)     p->SetParticleDefinition(G4PhononTransFast::Definition());
      else                               p->SetParticleDefinition(G4PhononLong::Definition());
    }
  }

  // -----------------------------
  // 4) Create vertex & attach primaries
  // -----------------------------
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  event->AddPrimaryVertex(vtx);

  // -----------------------------
  // 5) Fill per-event ntuple (thread-safe; merged by RunAction)
  // -----------------------------
  auto* ana = G4AnalysisManager::Instance();
  // column indices must match RunAction::BeginOfRunAction
  ana->FillNtupleIColumn(0, event->GetEventID());
  ana->FillNtupleDColumn(1, E_R / eV);
  ana->FillNtupleDColumn(2, theta);
  ana->FillNtupleDColumn(3, v / (km/s));
  ana->FillNtupleDColumn(4, pos.x()/cm);
  ana->FillNtupleDColumn(5, pos.y()/cm);
  ana->FillNtupleDColumn(6, pos.z()/cm);
  ana->AddNtupleRow();
}

// =============================
// Helpers
// =============================

// Galactic truncated Maxwell-Boltzmann (no Earth boost)
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

// θ isotropic in 3D: cosθ ~ U[-1,1] ⇒ θ = arccos(1 - 2u)
G4double PhononPrimaryGeneratorAction::SampleThetaIsotropic() const {
  const G4double u = 2.0 * CLHEP::pi * G4UniformRand();
  return u;
}

// Uniform inside cylinder: R=2 cm, H=4 cm, centered at origin (axis = ẑ)
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
