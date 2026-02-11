/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef SMConfigMessenger_hh
#define SMConfigMessenger_hh 1

// $Id$
// File:  SMConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		SMConfigManager.
//
// 20170816  Michael Kelsey

#include "G4UImessenger.hh"

class SMConfigManager;
class G4UIcmdWithAString;
class G4UIcommand;


class SMConfigMessenger : public G4UImessenger {
public:
  SMConfigMessenger(SMConfigManager* theData);
  virtual ~SMConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

private:
  SMConfigManager* theManager;
  G4UIcmdWithAString* hitsCmd;

private:
  SMConfigMessenger(const SMConfigMessenger&);	// Copying is forbidden
  SMConfigMessenger& operator=(const SMConfigMessenger&);
};

#endif /* SMConfigMessenger_hh */
