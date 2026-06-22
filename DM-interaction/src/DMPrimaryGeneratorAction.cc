#include "DMPrimaryGeneratorAction.hh"
#include "DMConfigManager.hh"

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
	CustomEdepFlag=DMConfigManager::GetCustomEdepFlag();
	Emin=DMConfigManager::GetMinRecoilEnergy();
	Emax=DMConfigManager::GetMaxRecoilEnergy();
}
DMPrimaryGeneratorAction::~DMPrimaryGeneratorAction(){;}
void DMPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
  event->SetEventID(DMConfigManager::GetCustomEventID()+event->GetEventID());
  int PDG_A;
  G4double m_DM=0,v=0,theta=0,E_R=0;
  G4ThreeVector pos;

  if(!CustomEdepFlag)
  {
  G4double rand=G4UniformRand();
  if(rand<(4/6)) PDG_A=16008;		// recoil with oxygen
  else if(rand<(5/6)) PDG_A=32040;	// recoil with calcium 
  else PDG_A=184074;			// recoil with tungsten
  E_R=Emin+G4UniformRand()*(Emax-Emin);
  pos = SampleEventVertex();
  }
  else
  {
  G4double rand=G4UniformRand();
  if(rand<(4/6)) PDG_A=16008;		// recoil with oxygen
  else if(rand<(5/6)) PDG_A=32040;	// recoil with calcium 
  else PDG_A=184074;			// recoil with tungsten
  E_R=DMConfigManager::GetVertexEnergy();
  pos=DMConfigManager::GetVertexVector();
  }
  
  G4CMPEnergyPartition part(pos);  
  part.DoPartition(/*PDGcode=*/PDG_A, /*Etotal=*/E_R, /*eNIEL=*/0.0);
  
  std::vector<G4PrimaryParticle*> primaries;
  part.GetPrimaries(primaries);

/*  // optional: relabel phonon polarizations to chosen fractions
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
*/
  long int no_of_sec=0,nL=0,nTF=0,nTS=0;
  for (int i=0; i<primaries.size(); i++)
    if(primaries[i]->GetParticleDefinition()==G4PhononLong::Definition()) nL++;
    else if (primaries[i]->GetParticleDefinition()==G4PhononTransFast::Definition()) nTF++;
    else if (primaries[i]->GetParticleDefinition()==G4PhononTransSlow::Definition()) nTS++;

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
  ana->FillNtupleDColumn(0,10,nL);
  ana->FillNtupleDColumn(0,11,nTF);
  ana->FillNtupleDColumn(0,12,nTS);
  ana->AddNtupleRow(0);
  // make vertex and attach primaries
  auto* vtx = new G4PrimaryVertex(pos, 0.*ns);
  for (auto* p : primaries) vtx->SetPrimary(p);
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
