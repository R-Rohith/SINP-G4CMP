#include "PhononPrimaryGeneratorAction.hh"

// G4CMP
#include "G4CMPEnergyPartition.hh"
#include "G4CMPConfigManager.hh"
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

// std
#include <fstream>
#include <mutex>
#include <cmath>
#include <vector>

namespace {
  std::ofstream g_txt;
  std::once_flag g_txt_once;
  std::mutex g_txt_mtx;
}

PhononPrimaryGeneratorAction::PhononPrimaryGeneratorAction() {
  // optional: silence G4CMP prints
  G4CMPConfigManager::SetVerboseLevel(0);
}

PhononPrimaryGeneratorAction::~PhononPrimaryGeneratorAction() {
  if (g_txt.is_open()) g_txt.close();
}

void PhononPrimaryGeneratorAction::OpenTxtOnce() {
  std::call_once(g_txt_once, [](){
    g_txt.open("dm_root.txt"); // CSV content, .txt extension
    g_txt << "event_id,ER_eV,theta_rad,v_kms,x_cm,y_cm,z_cm\n";
  });
}

void PhononPrimaryGeneratorAction::WriteTxtRow(int event_id, double ER_eV, double theta_rad,
                                               double v_kms, double x_cm, double y_cm, double z_cm) const {
  if (!g_txt.is_open()) return;
  std::lock_guard<std::mutex> lock(g_txt_mtx);
  g_txt << event_id << ',' << ER_eV << ',' << theta_rad << ','
        << v_kms << ',' << x_cm << ',' << y_cm << ',' << z_cm << '\n';
}

void PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  OpenTxtOnce();

  // --- masses
  const G4double m_DM = 1.0 * GeV;                 // DM mass
  const G4double m_T  = 184.0 * CLHEP::amu_c2;     // W-184 nucleus

  // --- DM halo velocity (truncated Maxwellian)
  const G4ThreeVector v_gal = SampleDMVelocity_Galactic();
  const G4double v    = v_gal.mag();
  const G4double beta = v / CLHEP::c_light;

  // --- scattering angle (isotropic)
  const G4double theta = SampleThetaIsotropic();
  const G4double cosTh = std::cos(theta);

  // --- recoil: ER = 2 μ^2 v^2 / m_T (1 - cosθ)
  const G4double mu   = (m_DM * m_T) / (m_DM + m_T);
  const G4double E_R  = 2.0 * (mu * mu / m_T) * (beta * beta) * (1.0 - cosTh);

  // --- sample start position (uniform in cylinder)
  const G4ThreeVector pos = SampleEventVertex();

  // --- log per-event line (units: eV, rad, km/s, cm)
  WriteTxtRow(event->GetEventID(),
              E_R / eV, theta, v / (km/s),
              pos.x()/cm, pos.y()/cm, pos.z()/cm);

  // --- partition using position-based constructor (no PV used)
  constexpr int PDG_W184 = 184074; // AAAZZZ for W-184
  G4CMPEnergyPartition part(pos);
  part.DoPartition(/*PDGcode=*/PDG_W184, /*Etotal=*/E_R, /*eNIEL=*/0.0);

  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // optional: relabel phonon polarizations
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

  // --- attach primaries to event
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : prims) vtx->SetPrimary(p);
  event->AddPrimaryVertex(vtx);
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
  // Uniform in cylinder R=2 cm, H=4 cm, centered at origin.
  const G4double R     = 2.0 * cm;
  const G4double Hhalf = 2.0 * cm;
  const G4double r   = R * std::sqrt(G4UniformRand());
  const G4double phi = 2.0 * CLHEP::pi * G4UniformRand();
  const G4double x   = r * std::cos(phi);
  const G4double y   = r * std::sin(phi);
  const G4double z   = (2.0 * G4UniformRand() - 1.0) * Hhalf;
  return {x, y, z};
}
