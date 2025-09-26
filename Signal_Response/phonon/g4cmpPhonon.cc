// main.cc  — MT-ready, no g4analysis required

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

#include "PhononDetectorConstruction.hh"
#include "PhononActionInitialization.hh"

#include "G4CMPPhysicsList.hh"
#include "G4CMPConfigManager.hh"
#include "PhononConfigManager.hh"

#include <algorithm>
#include <thread>

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

int main(int argc, char** argv) {
  // 1) Run manager (MT when available)
#if USE_FACTORY
  auto* runManager =
  G4RunManagerFactory::CreateRunManager(
  #ifdef G4MULTITHREADED
      G4RunManagerType::MT
  #else
      G4RunManagerType::Serial
  #endif
  );
  #ifdef G4MULTITHREADED
    // Default thread count (you can also set via macro: /run/numberOfThreads N)
    unsigned hw = std::max(1u, std::thread::hardware_concurrency());
    runManager->SetNumberOfThreads(std::min<unsigned>(hw, 16)); // cap at 16
  #endif
#else
  #ifdef G4MULTITHREADED
    auto* runManager = new G4MTRunManager();
    unsigned hw = std::max(1u, std::thread::hardware_concurrency());
    runManager->SetNumberOfThreads(std::min<unsigned>(hw, 8));
  #else
    auto* runManager = new G4RunManager();
  #endif
#endif

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
