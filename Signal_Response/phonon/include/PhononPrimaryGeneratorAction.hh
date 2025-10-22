#pragma once
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

#include<fstream>

class G4Event;

class PhononPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PhononPrimaryGeneratorAction();
  ~PhononPrimaryGeneratorAction();	// override = default;
  void GeneratePrimaries(G4Event* event) override;

private:
  // DM sampling helpers
  G4ThreeVector SampleDMVelocity_Galactic() const;
  G4double      SampleThetaIsotropic() const;

  // Position sampling (uniform in cylinder R=2 cm, H=4 cm, centered)
  G4ThreeVector SampleEventVertex() const;
  std::ofstream fout;
};
