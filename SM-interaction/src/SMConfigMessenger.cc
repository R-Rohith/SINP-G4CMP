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
  : G4UImessenger("/userconfig/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of phonon hit locations");
  PrimPartTypeCmd=CreateCommand<G4UIcmdWithAString>("PrimPartType","Set the primary particle type: (muon, neutron, gamma)");
  PrimPartEnergyCmd=CreateCommand<G4UIcmdWithADoubleAndUnit>("PrimPartEnergy","Set the primary particle Energy");
  PrimPartFluxFilenameCmd=CreateCommand<G4UIcmdWithAString>("PrimPartFluxFilename","Set the primary particle distribution filename. Must be a root file.");
  PrimPartFluxHistnameCmd=CreateCommand<G4UIcmdWithAString>("PrimPartFluxHistname","Set the primary particle distribution histogram name. Must be in a root file.");

}


SMConfigMessenger::~SMConfigMessenger() {
  delete hitsCmd; hitsCmd=0;
  delete PrimPartTypeCmd; PrimPartTypeCmd=0;
  delete PrimPartEnergyCmd; PrimPartEnergyCmd=0;
  delete PrimPartFluxFilenameCmd; PrimPartFluxFilenameCmd=0;
  delete PrimPartFluxHistnameCmd; PrimPartFluxHistnameCmd=0;
}


// Parse user input and add to configuration

void SMConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
  else if (cmd == PrimPartTypeCmd) theManager->SetPrimPartType(value);
  else if (cmd == PrimPartEnergyCmd) theManager->SetPrimPartEnergy(PrimPartEnergyCmd->GetNewDoubleValue(value));
  else if (cmd == PrimPartFluxFilenameCmd) theManager->SetPrimPartFluxFilename(value);
  else if (cmd == PrimPartFluxHistnameCmd) theManager->SetPrimPartFluxHistname(value);
}
