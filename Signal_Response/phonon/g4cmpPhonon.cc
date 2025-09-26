#include "G4RunManagerFactory.hh"   // MT/SR factory (Geant4 ≥10.6)
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "G4CMPPhysicsList.hh"
#include "G4CMPConfigManager.hh"
#include "PhononActionInitialization.hh"
#include "PhononConfigManager.hh"
#include "PhononDetectorConstruction.hh"

#include <thread>
#include <algorithm>

int main(int argc, char** argv)
{
  // 1) Create a run manager (MT if available)
  auto* runManager = G4RunManagerFactory::CreateRunManager(
#if defined(G4MULTITHREADED)
      G4RunManagerType::MT
#else
      G4RunManagerType::Serial
#endif
  );

#if defined(G4MULTITHREADED)
  // Set a sensible default number of threads (or control via macro: /run/numberOfThreads N)
  unsigned hw = std::max(1u, std::thread::hardware_concurrency());
  runManager->SetNumberOfThreads(std::min<unsigned>(hw, 8)); // cap at 8 by default
#endif

  // 2) Mandatory initializations
  auto* detector = new PhononDetectorConstruction();
  runManager->SetUserInitialization(detector);

  auto* physics = new G4CMPPhysicsList();
  physics->SetCuts();
  runManager->SetUserInitialization(physics);

  // Your actions (PrimaryGenerator + Stacking + RunAction are registered inside)
  runManager->SetUserInitialization(new PhononActionInitialization);

  // Ensure CMP + Phonon macros exist
  G4CMPConfigManager::Instance();
  PhononConfigManager::Instance();

  // 3) Visualization (optional)
  auto* visManager = new G4VisExecutive();
  visManager->Initialize();

  // 4) UI / macro
  auto* UImanager = G4UImanager::GetUIpointer();
  if (argc == 1) {
    auto* ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    delete ui;
  } else {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + G4String(argv[1]));
  }

  delete visManager;
  delete runManager;
  return 0;
}
