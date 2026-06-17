/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// 20241024 Israel Hernandez -- IIT, QSC and Fermilab

#ifndef Caustic_PhononSensitivity_h
#define Caustic_PhononSensitivity_h 1

#include "G4CMPElectrodeSensitivity.hh"
#include "TFile.h"
#include "TTree.h"

class Caustic_PhononSensitivity final : public G4CMPElectrodeSensitivity {
public:
  Caustic_PhononSensitivity(G4String name);
  virtual ~Caustic_PhononSensitivity();
  // No copies
  Caustic_PhononSensitivity(const Caustic_PhononSensitivity&) = delete;
  Caustic_PhononSensitivity& operator=(const Caustic_PhononSensitivity&) = delete;

  Caustic_PhononSensitivity(Caustic_PhononSensitivity&&) = delete;
  Caustic_PhononSensitivity& operator=(Caustic_PhononSensitivity&&) = delete;

  virtual void EndOfEvent(G4HCofThisEvent*);

  void SetOutputFile(const G4String& fn);

protected:
  virtual G4bool IsHit(const G4Step*, const G4TouchableHistory*) const;

private:
  std::ofstream output;
  G4String fileName;
  TFile* rootFile; //Added for TTree
  TTree* tree;    //Added for TTree

  int eventID;    //Added for TTree
  int trackID;    //Added for TTree
  double final_x, final_y, final_z;  //Added for TTree
  double initial_x, initial_y, initial_z; //Added for TTree
  double edep;          //Added for TTree
  double weight;        //Added for TTree
  double totalEdep;     //Added for TTree
  double recoilEnergy; //Added for TTree
  std::string phononName;//Added for TTree
};

#endif
