#include "PhononActionInitialization.hh"
#include "PhononPrimaryGeneratorAction.hh"
#include "G4CMPStackingAction.hh"
#include "RunAction.hh"

//void PhononActionInitialization::BuildForMaster() const {
  // master thread: typically only run-level actions
//SetUserAction(new RunAction());
//}

void PhononActionInitialization::Build() const {
  // Worker threads: generator + stacking + (can also have a RunAction; harmless)
  SetUserAction(new PhononPrimaryGeneratorAction());
  SetUserAction(new G4CMPStackingAction());
  SetUserAction(new RunAction());   // optional; safe either way
}
