/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  PhononConfigMessenger.cc
//
// Description:	Macro command defitions to set user configuration in
//		PhononConfigManager.
//
// 20170816  Michael Kelsey

#include "SMConfigMessenger.hh"
#include "SMConfigManager.hh"
#include "G4UIcmdWithAString.hh"


// Constructor and destructor

SMConfigMessenger::SMConfigMessenger(SMConfigManager* mgr)
  : G4UImessenger("/g4cmp/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of phonon hit locations");
//  PrimPartTypeCmd=CreateCommand<G4UIcmdWithAString>("PrimPartType","Set the primary particle type: (mu,n)");
//  PrimPartDistFileCmd=CreateCommand<G4UIcmdWithAString>("PrimPartDistFile","Set the primary particle distribution file. Must be a root file.");
//  PrimPartDistHistCmd=CreateCommand<G4UIcmdWithAString>("PrimPartDistHist","Set the primary particle distribution histogram name. Must be in a root file.");

}


SMConfigMessenger::~SMConfigMessenger() {
  delete hitsCmd; hitsCmd=0;
}


// Parse user input and add to configuration

void SMConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
}
