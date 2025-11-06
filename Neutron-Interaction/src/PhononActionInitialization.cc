/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: 539f524339ae53ad098a07cfa3bebd07784d23dd $

#include "PhononActionInitialization.hh"
#include "PhononPrimaryGeneratorAction.hh"
#include "G4CMPStackingAction.hh"
#include "RunAction.hh"
#include "MyEventAction.hh"
#include "MySteppingAction.hh"

//void PhononActionInitialization::BuildForMaster() const {
  // master thread: typically only run-level actions
//SetUserAction(new RunAction());
//}

void PhononActionInitialization::Build() const {
//    SetUserAction(new RunAction());
  SetUserAction(new PhononPrimaryGeneratorAction);
  SetUserAction(new G4CMPStackingAction);
    MyEventAction* eventAction = new MyEventAction();
    SetUserAction(eventAction);
    MySteppingAction* steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
} 
