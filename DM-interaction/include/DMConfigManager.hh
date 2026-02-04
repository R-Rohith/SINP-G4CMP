/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef DMConfigManager_hh
#define DMConfigManager_hh 1

// $Id$
// File:  PhononConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see PhononConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"

class DMConfigMessenger;


class DMConfigManager {
public:
  ~DMConfigManager();	// Must be public for end-of-job cleanup
  static DMConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }

  static void UpdateGeometry();

private:
  DMConfigManager();		// Singleton: only constructed on request
  DMConfigManager(const DMConfigManager&) = delete;
  DMConfigManager(DMConfigManager&&) = delete;
  DMConfigManager& operator=(const DMConfigManager&) = delete;
  DMConfigManager& operator=(DMConfigManager&&) = delete;

  static DMConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)

  DMConfigMessenger* messenger;
};

#endif	/* PhononConfigManager_hh */
