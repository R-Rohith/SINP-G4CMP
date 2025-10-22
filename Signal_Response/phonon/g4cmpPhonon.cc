// main.cc  — MT-ready, no g4analysis required

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

#include "PhononDetectorConstruction.hh"
#include "PhononActionInitialization.hh"

#include "G4CMPPhysicsList.hh"
#include "G4CMPConfigManager.hh"
#include "PhononConfigManager.hh"

//#include "CLHEP/Random/RanecuEngine.h"

int main(int argc,char** argv)
{
 // Construct the run manager
 //
 
#ifdef G4MULTITHREADED
 G4RunManager * runManager = G4RunManagerFactory::CreateRunManager();
 runManager->SetNumberOfThreads(20); // Set the number of threads for multi-threading mode
#else
  G4RunManager * runManager = new G4RunManager;
#endif

 // Fixed Simulation                                                                           
 // G4long seed = 2486945;
 // CLHEP::HepRandom::setTheSeed(seed);
 
 // OR: Random seed based on time (non-reproducible)
  G4long seed = time(nullptr);
  CLHEP::HepRandom::setTheSeed(seed);

// Prefer the factory API when available (Geant4 ≥ 10.6)
#if __has_include("G4RunManagerFactory.hh")
  #include "G4RunManagerFactory.hh"
  #define USE_FACTORY 1
#else
  #include "G4RunManager.hh"
  #ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
  #endif
  #define USE_FACTORY 0
#endif

 G4VUserPhysicsList* physics = new G4CMPPhysicsList();
 physics->SetCuts();
 runManager->SetUserInitialization(physics);
 
 // To reduce the RAM usage
 runManager->SetNumberOfEventsToBeStored(0);
 //
 // Set user action classes (different for Geant4 10.0)
 //
 runManager->SetUserInitialization(new PhononActionInitialization);

  // 2) Mandatory initializations
  runManager->SetUserInitialization(new PhononDetectorConstruction());

  auto* physics = new G4CMPPhysicsList();
  physics->SetCuts();
  runManager->SetUserInitialization(physics);

  // Your actions (PrimaryGenerator + Stacking + RunAction registered inside)
  runManager->SetUserInitialization(new PhononActionInitialization());

  // Ensure CMP & phonon config commands exist (for your macros)
  G4CMPConfigManager::Instance();
  PhononConfigManager::Instance();

  // 3) Visualization (optional but handy)
  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  // 4) UI or batch
  auto* UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    // Interactive session
    auto* ui = new G4UIExecutive(argc, argv);
    // You can also execute a default vis macro here if you like:
    // UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    // Batch mode: pass a macro file as arg1
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + G4String(argv[1]));
  }

  delete visManager;
  delete runManager;
  return 0;
}
