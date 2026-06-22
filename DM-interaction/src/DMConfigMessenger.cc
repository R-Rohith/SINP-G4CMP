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

#include "DMConfigMessenger.hh"
#include "DMConfigManager.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


// Constructor and destructor

DMConfigMessenger::DMConfigMessenger(DMConfigManager* mgr)
  : G4UImessenger("/userconfig/", "User configuration for G4CMP phonon example"),
    theManager(mgr), hitsCmd(0) {
  hitsCmd = CreateCommand<G4UIcmdWithAString>("HitsFile",
			      "Set filename for output of phonon hit locations");
  CustomEventIDCmd=CreateCommand<G4UIcmdWithAnInteger>("CustomEventID","Set a custom starting point for the event IDs of this simulation");
  CustomEdepFlagCmd = CreateCommand<G4UIcmdWithABool>("CustomEdepFlag","To activate custom energy deposition");
  MinRecoilEnergyCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("MinRecoilEnergy","Set minimum recoil energy");
  MaxRecoilEnergyCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("MaxRecoilEnergy","Set maximum recoil energy");
  VertexVectorCmd = CreateCommand<G4UIcmdWith3VectorAndUnit>("VertexVector","Set vertex position of energy depositions");
  VertexEnergyCmd = CreateCommand<G4UIcmdWithADoubleAndUnit>("VertexEnergy","Set vertex energy deposition");
}


DMConfigMessenger::~DMConfigMessenger() {
  delete hitsCmd; hitsCmd=0; 
  delete CustomEdepFlagCmd; CustomEdepFlagCmd=0;
  delete VertexVectorCmd; VertexVectorCmd=0;
  delete VertexVectorCmd; VertexVectorCmd=0;
}


// Parse user input and add to configuration

void DMConfigMessenger::SetNewValue(G4UIcommand* cmd, G4String value) {
  if (cmd == hitsCmd) theManager->SetHitOutput(value);
  else if (cmd == CustomEventIDCmd) theManager->SetCustomEventID(CustomEventIDCmd->GetNewIntValue(value));
  else if (cmd==CustomEdepFlagCmd) theManager->SetCustomEdepFlag(CustomEdepFlagCmd->GetNewBoolValue(value));
  else if (cmd==MinRecoilEnergyCmd) theManager->SetMinRecoilEnergy(MinRecoilEnergyCmd->GetNewDoubleValue(value));
  else if (cmd==MaxRecoilEnergyCmd) theManager->SetMaxRecoilEnergy(MaxRecoilEnergyCmd->GetNewDoubleValue(value));
  else if (cmd==VertexVectorCmd) theManager->SetVertexVector(VertexVectorCmd->GetNew3VectorValue(value));
  else if (cmd==VertexEnergyCmd) theManager->SetVertexEnergy(VertexEnergyCmd->GetNewDoubleValue(value));
}
