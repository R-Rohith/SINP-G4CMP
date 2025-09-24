#include "MyEnergyPartition.hh"
#include "Randomize.hh"

G4int MyEnergyPartition::ChoosePhononPolarization() {
  const G4double u = G4UniformRand();
  if (u < fTS)               return G4PhononPolarization::TransSlow; // TS
  if (u < fTS + fTF)         return G4PhononPolarization::TransFast; // TF
                             return G4PhononPolarization::Long;      // L
}
