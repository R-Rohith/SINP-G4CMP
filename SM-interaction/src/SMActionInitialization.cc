/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: 539f524339ae53ad098a07cfa3bebd07784d23dd $

#include "SMActionInitialization.hh"
#include "SMPrimaryGeneratorAction.hh"
#include "G4CMPStackingAction.hh"
#include "SMRunAction.hh"
#include "SMEventAction.hh"
#include "SMSteppingAction.hh"

//void PhononActionInitialization::BuildForMaster() const {
  // master thread: typically only run-level actions
//SetUserAction(new RunAction());
//}

void SMActionInitialization::Build() const {
    SetUserAction(new SMRunAction());
  SetUserAction(new SMPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);
    SMEventAction* eventAction = new SMEventAction();
    SetUserAction(eventAction);
    SMSteppingAction* steppingAction = new SMSteppingAction(eventAction);
    SetUserAction(steppingAction);
} 
