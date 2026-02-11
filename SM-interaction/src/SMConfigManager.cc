/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  PhononConfigManager.cc
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see PhononConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "SMConfigManager.hh"
#include "SMConfigMessenger.hh"
#include "G4RunManager.hh"
#include <stdlib.h>


// Constructor and Singleton Initializer

SMConfigManager* SMConfigManager::theInstance = 0;

SMConfigManager* SMConfigManager::Instance() {
  if (!theInstance) theInstance = new SMConfigManager;
  return theInstance;
}

SMConfigManager::SMConfigManager()
  : Hit_file(getenv("G4CMP_HIT_FILE")?getenv("G4CMP_HIT_FILE"):"phonon_hits.txt"),
    messenger(new SMConfigMessenger(this)) {;}

SMConfigManager::~SMConfigManager() {
  delete messenger; messenger=0;
}


// Trigger rebuild of geometry if parameters change

void SMConfigManager::UpdateGeometry() {
  G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
