/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file exoticphysics/phonon/src/G4CMPPhysicsList.cc
/// \brief Implementation of the G4CMPPhysicsList class
//
// $Id: e2b3e3fff1097479691a61b4932663d966357dd4 $
//
// 20140328  Change to ModularPhysicsList, use new G4CMPPhysics for processes

#include "My_G4CMPPhysicsList.hh"
#include "G4CMPPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"

#include "G4ParticleDefinition.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

#include "G4ProcessManager.hh"

// Constructor and destructor

My_G4CMPPhysicsList::My_G4CMPPhysicsList(G4int verbose) : G4VModularPhysicsList() {
  SetVerboseLevel(verbose);
  if (verbose) G4cout << "G4CMPPhysicsList::constructor" << G4endl;

  defaultCutValue = DBL_MIN;	// 100*mm;

  RegisterPhysics(new G4CMPPhysics);		// Phonons and charge-carriers
  RegisterPhysics(new G4EmStandardPhysics);

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics);

  // Decays
  RegisterPhysics( new G4DecayPhysics);

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics);

   // Hadron Physics
  RegisterPhysics(  new G4HadronPhysicsFTFP_BERT);

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics);

  // Ion Physics
  RegisterPhysics( new G4IonPhysics);

  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut);
}

My_G4CMPPhysicsList::~My_G4CMPPhysicsList() {;}

// These values are used as the default production thresholds
// for the world volume.
/*
void My_G4CMPPhysicsList::ConstructParticle()
{
 G4MuonPlus::MuonPlusDefinition();
 G4MuonMinus::MuonMinusDefinition();

 G4Electron::ElectronDefinition();
 G4Positron::PositronDefinition();
 G4Gamma::GammaDefinition();
 G4NeutrinoE::NeutrinoEDefinition();
 G4AntiNeutrinoE::AntiNeutrinoEDefinition();
 G4NeutrinoMu::NeutrinoMuDefinition();
 G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

 G4VModularPhysicsList::ConstructParticle();
}

void My_G4CMPPhysicsList::ConstructProcess()
{
 G4ProcessManager *MuPlus=G4MuonPlus::MuonPlusDefinition()->GetProcessManager();
 G4ProcessManager *MuMinus=G4MuonMinus::MuonMinusDefinition()->GetProcessManager();

 MuPlus->AddProcess(new G4MuMultipleScattering(),-1,-1,1);
 MuPlus->AddProcess(new G4MuIonisation(),-1,2,2);
 MuPlus->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);
 MuMinus->AddProcess(new G4MuMultipleScattering(),-1,-1,1);
 MuMinus->AddProcess(new G4MuIonisation(),-1,2,2);
 MuMinus->AddProcess(new G4MuBremsstrahlung(),-1,-1,3);

 G4VModularPhysicsList::ConstructProcess();
// Verbose Level hasn't been set
}
*/
void My_G4CMPPhysicsList::SetCuts() {
  SetCutsWithDefault();
}


