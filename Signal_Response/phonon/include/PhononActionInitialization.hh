#ifndef PhononActionInitialization_hh
#define PhononActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class PhononActionInitialization : public G4VUserActionInitialization {
public:
  PhononActionInitialization() = default;
  ~PhononActionInitialization() override = default;

  void Build() const override;
  void BuildForMaster() const override;  // MT-safe: can be empty
};

#endif
