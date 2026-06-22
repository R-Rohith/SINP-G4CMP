/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef DMConfigManager_hh
#define DMConfigManager_hh 1

// $Id$
// File:  PhononConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see PhononConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"
#include "G4ThreeVector.hh"

class DMConfigMessenger;


class DMConfigManager {
public:
  ~DMConfigManager();	// Must be public for end-of-job cleanup
  static DMConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }
  static const G4int GetCustomEventID()
  {
          return Instance()->CustomEventID;
  } 
  static const G4bool GetCustomEdepFlag()
  {
	  return Instance()->CustomEdepFlag;
  }
  static const G4double GetMinRecoilEnergy()
  {
	  return Instance()->MinRecoilEnergy;
  }
  static const G4double GetMaxRecoilEnergy()
  {
	  return Instance()->MaxRecoilEnergy;
  }
  static const G4ThreeVector GetVertexVector()
  {
    return Instance()->VertexVector;
  }
  static const G4double GetVertexEnergy()
  {
    return Instance()->VertexEnergy;
  }
  static void SetCustomEventID(const G4int& value)
  {
          Instance()->CustomEventID=value;
  }
  static void SetCustomEdepFlag(G4bool flag)
  {
	  Instance()->CustomEdepFlag=flag;
  }
  static void SetMinRecoilEnergy(G4double Emin)
  {
	  Instance()->MinRecoilEnergy=Emin;
  }
  static void SetMaxRecoilEnergy(G4double Emax)
  {
	  Instance()->MaxRecoilEnergy=Emax;
  }
  static void SetVertexVector(G4ThreeVector pos)
  {
    Instance()->VertexVector=pos;
  }
  static void SetVertexEnergy(G4double energy)
  {
    Instance()->VertexEnergy=energy;
  }


  static void UpdateGeometry();

private:
  DMConfigManager();		// Singleton: only constructed on request
  DMConfigManager(const DMConfigManager&) = delete;
  DMConfigManager(DMConfigManager&&) = delete;
  DMConfigManager& operator=(const DMConfigManager&) = delete;
  DMConfigManager& operator=(DMConfigManager&&) = delete;

  static DMConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)
  G4int CustomEventID;  // Starting point of event ID
  G4bool CustomEdepFlag;  // Flag to toggle manual Edep simulations
  G4double MinRecoilEnergy, MaxRecoilEnergy;
  G4ThreeVector VertexVector;
  G4double VertexEnergy;

  DMConfigMessenger* messenger;
};

#endif	/* PhononConfigManager_hh */
