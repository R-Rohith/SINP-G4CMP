#include "PhononPrimaryGeneratorAction.hh"

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

PhononPrimaryGeneratorAction::PhononPrimaryGeneratorAction()// = default;
{
  fParticleGun  = new G4GeneralParticleSource();

/*  fout.open(G4CMP::DebuggingFileThread("Generator_output.txt"),std::ios::out);
	if (!fout.good()) {
      G4ExceptionDescription msg;
      msg << "Error opening generator output file ";
      G4Exception("PhononSensitivity::SetOutputFile", "PhonSense003",
                  FatalException, msg);
      fout.close();
    }*/
  //--Analysismanager config
/*  auto* ana = G4AnalysisManager::Instance();
  ana->OpenFile(G4CMP::DebuggingFileThread("Generator_output.root"));
 
  ana->CreateNtuple("Per-event-data","Per-event-data");
  ana->CreateNtupleIColumn("Event_ID");
  ana->CreateNtupleIColumn("PID");
  ana->CreateNtupleDColumn("Initial_energy");
  ana->CreateNtupleDColumn("Initial_posx");
  ana->CreateNtupleDColumn("Initial_posy");
  ana->CreateNtupleDColumn("Initial_posz");
  ana->CreateNtupleDColumn("Initial_theta");
  ana->CreateNtupleDColumn("Initial_phi");
  ana->CreateNtupleDColumn("Initial_dirx");
  ana->CreateNtupleDColumn("Initial_diry");
  ana->CreateNtupleDColumn("Initial_dirz");
  ana->CreateNtupleDColumn("Energy_dep");
  ana->CreateNtupleDColumn("NSecondaries");
  ana->CreateNtupleDColumn("NPhononL");
  ana->CreateNtupleDColumn("NPhononTF");
  ana->CreateNtupleDColumn("NPhononTS");
  */
// Tfin=new TFile("");
// fluxhist=(TH1D*)Tfin->Get("");
}
PhononPrimaryGeneratorAction::~PhononPrimaryGeneratorAction()
{
//	fout.close();
}
void PhononPrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
/*
  // --- masses
  const G4double m_DM = 1.0 * GeV;
  const G4double m_T  = 16.0 * CLHEP::amu_c2;

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
  constexpr int PDG_W184 = 16008;
  G4CMPEnergyPartition part(pos);*/
//  part.DoPartition(/*PDGcode=*/PDG_W184, /*Etotal=*/E_R, /*eNIEL=*/0.0);
  //std::cout<<"RECOIL Energy: "<<E_R/eV<<std::endl;
  // grab the generated secondaries as primaries
/*  std::vector<G4PrimaryParticle*> prims;
  part.GetPrimaries(prims);

  // optional: relabel phonon polarizations to chosen fractions
  const G4double fracTS = 0.50, fracTF = 0.35;
  long int no_of_phonons=0;
  for (auto* p : prims) {
	  no_of_phonons+=p->GetWeight();
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
  
  fout<<v/km*s<<'\t'<<theta<<'\t'<<E_R/eV<<'\t'<<pos[0]<<'\t'<<pos[1]<<'\t'<<pos[2]<<'\t'<<no_of_phonons<<'\t'<<part.LindhardScalingFactor(E_R,8,16)<<std::endl;
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
  //ana->AddNtupleRow();*/

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
  G4double E=10*MeV, theta=CLHEP::pi*G4UniformRand(), phi=2*CLHEP::pi*G4UniformRand();
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
