/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file library/include/G4CMPPhysicsList.cc
/// \brief Definition of the G4CMPPhysicsList class
//
// $Id: a718cfd2de0c790f3f9bbb8b084415dd41fc76e6 $
//
// 20140328  Change to ModularPhysicsList, use new G4CMPPhysics for processes
// 20170525  Destructor should be virtual, add default copy/move semantics

#ifndef SMPhysicsList_h
#define SMPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class SMPhysicsList: public G4VModularPhysicsList {
public:
  SMPhysicsList(G4int verbose=0);
  virtual ~SMPhysicsList();

  // Use default copy/move semantics
  SMPhysicsList(const SMPhysicsList&) = default;
  SMPhysicsList(SMPhysicsList&&) = default;
  SMPhysicsList& operator=(const SMPhysicsList&) = default;
  SMPhysicsList& operator=(SMPhysicsList&&) = default;

public:
  void SetCuts();
  void ConstructParticle();
//  void ConstructProcess();
};

#endif
