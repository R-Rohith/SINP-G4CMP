#ifndef DMRunAction_hh
#define DMRunAction_hh 1

#include "G4UserRunAction.hh"
 
class G4Run;
 
class DMRunAction : public G4UserRunAction {
public:
  DMRunAction();
  ~DMRunAction() override;
 
  void BeginOfRunAction(const G4Run* run) override;
  void EndOfRunAction(const G4Run* run) override;
};

#endif

