#include "PhononActionInitialization.hh"
#include "PhononPrimaryGeneratorAction.hh"
#include "G4CMPStackingAction.hh"
// #include "RunAction.hh"   // <- optional, only if you decide to use it

void PhononActionInitialization::BuildForMaster() const {
  // Master thread gets only run-scoped actions (if any).
  // If you don’t need any, leave this empty.
  // Example (optional):
  // SetUserAction(new RunAction());
}

void PhononActionInitialization::Build() const {
  // Worker threads: primary generator + stacking (and other per-thread actions).
  SetUserAction(new PhononPrimaryGeneratorAction());
  SetUserAction(new G4CMPStackingAction());

  // Optional extras:
  // SetUserAction(new RunAction());
  // SetUserAction(new EventAction());
  // SetUserAction(new SteppingAction());
}
