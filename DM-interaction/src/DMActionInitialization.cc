#include "DMActionInitialization.hh"
#include "DMPrimaryGeneratorAction.hh"
#include "DMRunAction.hh"
#include "G4CMPStackingAction.hh"


//void PhononActionInitialization::BuildForMaster() const {
  // master thread: typically only run-level actions
//SetUserAction(new RunAction());
//}

void DMActionInitialization::Build() const {
  // Worker threads: generator + stacking + (can also have a RunAction; harmless)
  SetUserAction(new DMPrimaryGeneratorAction());
  SetUserAction(new G4CMPStackingAction());
  SetUserAction(new DMRunAction());   // optional; safe either way
}
