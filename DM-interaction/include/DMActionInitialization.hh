#ifndef DMActionInitialization_hh
#define DMActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class DMActionInitialization : public G4VUserActionInitialization {
public:
  DMActionInitialization() = default;
  ~DMActionInitialization() override = default;

  void Build() const override;
//  void BuildForMaster() const override;  // MT-safe: can be empty
};

#endif
