/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef SMSensitivity_h
#define SMSensitivity_h 1

#include "G4CMPElectrodeSensitivity.hh"

class SMSensitivity final : public G4CMPElectrodeSensitivity {
public:
  SMSensitivity(G4String name);
  virtual ~SMSensitivity();
  // No copies
  SMSensitivity(const SMSensitivity&) = delete;
  SMSensitivity& operator=(const SMSensitivity&) = delete;
  /* Move is disabled for now because old versions of GCC can't move ofstream
  // Move OK
  SMSensitivity(SMSensitivity&&);
  SMSensitivity& operator=(SMSensitivity&&);
  */
  SMSensitivity(SMSensitivity&&) = delete;
  SMSensitivity& operator=(SMSensitivity&&) = delete;

  virtual void EndOfEvent(G4HCofThisEvent*);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

private:
  std::ofstream output;
  G4String fileName;

  // Paraquet file writing-------------
};

#endif
