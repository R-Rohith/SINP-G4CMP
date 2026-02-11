/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef SMConfigManager_hh
#define SMConfigManager_hh 1

// $Id$
// File:  SMConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see SMConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"

class SMConfigMessenger;


class SMConfigManager {
public:
  ~SMConfigManager();	// Must be public for end-of-job cleanup
  static SMConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }

  static void UpdateGeometry();

private:
  SMConfigManager();		// Singleton: only constructed on request
  SMConfigManager(const SMConfigManager&) = delete;
  SMConfigManager(SMConfigManager&&) = delete;
  SMConfigManager& operator=(const SMConfigManager&) = delete;
  SMConfigManager& operator=(SMConfigManager&&) = delete;

  static SMConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)

  SMConfigMessenger* messenger;
};

#endif	/* SMConfigManager_hh */
