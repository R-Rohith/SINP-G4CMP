/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id: e58a61fedbb99b167e16dafebc9c8664ae0c7b94 $

#ifndef PhononActionInitialization_hh
#define PhononActionInitialization_hh 1

#include "G4VUserActionInitialization.hh"

class PhononActionInitialization : public G4VUserActionInitialization {
public:
  PhononActionInitialization() = default;
  ~PhononActionInitialization() override = default;
  void Build() const override;
  //void BuildForMaster() const override;
};

#endif	/* PhononActionInitialization_hh */
