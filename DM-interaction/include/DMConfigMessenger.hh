/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef DMConfigMessenger_hh
#define DMononConfigMessenger_hh 1

// $Id$
// File:  PhononConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		PhononConfigManager.
//
// 20170816  Michael Kelsey

#include "G4UImessenger.hh"

class DMConfigManager;
class G4UIcmdWithAString;
class G4UIcommand;


class DMConfigMessenger : public G4UImessenger {
public:
  DMConfigMessenger(DMConfigManager* theData);
  virtual ~DMConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

private:
  DMConfigManager* theManager;
  G4UIcmdWithAString* hitsCmd;

private:
  DMConfigMessenger(const DMConfigMessenger&);	// Copying is forbidden
  DMConfigMessenger& operator=(const DMConfigMessenger&);
};

#endif /* PhononConfigMessenger_hh */
