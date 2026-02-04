/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef DMSensitivity_h
#define DMSensitivity_h 1

#include "G4CMPElectrodeSensitivity.hh"


class DMSensitivity final : public G4CMPElectrodeSensitivity {
public:
  DMSensitivity(G4String name);
  virtual ~DMSensitivity();
  // No copies
  DMSensitivity(const DMSensitivity&) = delete;
  DMSensitivity& operator=(const DMSensitivity&) = delete;
  /* Move is disabled for now because old versions of GCC can't move ofstream
  // Move OK
  PhononSensitivity(PhononSensitivity&&);
  PhononSensitivity& operator=(PhononSensitivity&&);
  */
  DMSensitivity(DMSensitivity&&) = delete;
  DMSensitivity& operator=(DMSensitivity&&) = delete;

  virtual void EndOfEvent(G4HCofThisEvent*);

  void SetOutputFile(const G4String& fn);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

private:
  std::ofstream output;
  G4String fileName;

};

#endif
