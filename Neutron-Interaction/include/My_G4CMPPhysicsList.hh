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

#ifndef My_G4CMPPhysicsList_h
#define My_G4CMPPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class My_G4CMPPhysicsList: public G4VModularPhysicsList {
public:
  My_G4CMPPhysicsList(G4int verbose=0);
  virtual ~My_G4CMPPhysicsList();

  // Use default copy/move semantics
  My_G4CMPPhysicsList(const My_G4CMPPhysicsList&) = default;
  My_G4CMPPhysicsList(My_G4CMPPhysicsList&&) = default;
  My_G4CMPPhysicsList& operator=(const My_G4CMPPhysicsList&) = default;
  My_G4CMPPhysicsList& operator=(My_G4CMPPhysicsList&&) = default;

public:
  void SetCuts();
//  void ConstructParticle();
//  void ConstructProcess();
};

#endif
