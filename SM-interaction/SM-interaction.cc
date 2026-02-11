/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file phonon/g4cmpPhonon.cc
/// \brief Main program of the G4CMP/phonon example
//
// $Id$
//
// 20140509  Add conditional code for Geant4 10.0 vs. earlier
// 20150112  Remove RM->Initialize() call to allow macro configuration
// 20160111  Remove Geant4 version check since we now hard depend on 10.2+
// 20170816  Add example-specific configuration manager
// 20220718  Remove obsolete pre-processor macros G4VIS_USE and G4UI_USE

//#ifdef G4MULTITHREADED
#include "G4RunManagerFactory.hh"
//#else
#include "G4RunManager.hh"
//#endif
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "SMPhysicsList.hh"
#include "G4CMPConfigManager.hh"
#include "SMActionInitialization.hh"
#include "SMConfigManager.hh"
#include "SMDetectorConstruction.hh"

#include "TROOT.h"
#include "TFile.h"

//#include "CLHEP/Random/RanecuEngine.h"

int main(int argc,char** argv)
{
// To prevent errors from opening same .root file in multiple threads
 ROOT::EnableThreadSafety();
 TFile *fin=TFile::Open("PrimFlux.root");
 fin->Close();

 // Construct the run manager
 //
 
#ifdef G4MULTITHREADED
 G4RunManager * runManager = G4RunManagerFactory::CreateRunManager();
 runManager->SetNumberOfThreads(20); // Set the number of threads for multi-threading mode
#else
  G4RunManager * runManager = new G4RunManager;
#endif

 // Fixed Simulation                                                                           
  G4long seed = 1234567890;
 
 // OR: Random seed based on time (non-reproducible)
//  G4long seed = time(nullptr);


  CLHEP::HepRandom::setTheSeed(seed);

 // Set mandatory initialization classes
 //
 SMDetectorConstruction* detector = new SMDetectorConstruction();
 runManager->SetUserInitialization(detector);

 G4VUserPhysicsList* physics = new SMPhysicsList();
 physics->SetCuts();
 runManager->SetUserInitialization(physics);
 
 // To reduce the RAM usage
 runManager->SetNumberOfEventsToBeStored(0);
 //
 // Set user action classes (different for Geant4 10.0)
 //
 runManager->SetUserInitialization(new SMActionInitialization);

 // Create configuration managers to ensure macro commands exist
 G4CMPConfigManager::Instance();
 SMConfigManager::Instance();

 // Visualization manager
 //
 G4VisManager* visManager = new G4VisExecutive;
 visManager->Initialize();
 
 // Get the pointer to the User Interface manager
 //
 G4UImanager* UImanager = G4UImanager::GetUIpointer();  

 if (argc==1)   // Define UI session for interactive mode
 {
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
      ui->SessionStart();
      delete ui;
 }
 else           // Batch mode
 {
   G4String command = "/control/execute ";
   G4String fileName = argv[1];
   UImanager->ApplyCommand(command+fileName);
 }

 delete visManager;
 delete runManager;

 return 0;
}


