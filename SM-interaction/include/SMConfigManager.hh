/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef SMConfigManager_hh
#define SMConfigManager_hh 1

// $Id$
// File:  SMConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see SMConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include<iostream>
#include <cstdlib>

class SMConfigMessenger;


class SMConfigManager {
public:
  ~SMConfigManager();	// Must be public for end-of-job cleanup
  static SMConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }
  static const G4int GetCustomEventID()
  {
	  return Instance()->CustomEventID;
  }
  static const G4String& GetPrimPartType()
  {
    return Instance()->PrimPartType;
  }
  static const G4int GetPrimPartPDG()
  {
	  if(Instance()->PrimPartType=="neutron")
		  return 2112;
	  else if(Instance()->PrimPartType=="muon")
		  return 13;
	  else if(Instance()->PrimPartType=="gamma")
		  return 22;

  }

  static const G4double& GetPrimPartEnergy()
  {
	  return Instance()->PrimPartEnergy;
  }

  static const G4String& GetPrimPartFluxFilename()
  {
	  return Instance()->PrimPartFluxFilename;
  }

  static const G4String& GetPrimPartFluxHistname()
  {
	  return Instance()->PrimPartFluxHistname;
  }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; /*UpdateGeometry();*/ }
  static void SetCustomEventID(const G4int& value)
  {
	  Instance()->CustomEventID=value;
  }
  static void SetPrimPartType(const G4String& value)
  {
    Instance()->PrimPartType=value;// UpdateGeometry();
  }

  static void SetPrimPartEnergy(const G4double& value)
  {
	  Instance()->PrimPartEnergy=value;// UpdateGeometry();
  }

  static void SetPrimPartFluxFilename(const G4String& value)
  {
	  Instance()->PrimPartFluxFilename=value;// UpdateGeometry();
  }

  static void SetPrimPartFluxHistname(const G4String& value)
  {
	  Instance()->PrimPartFluxHistname=value;// UpdateGeometry();
  }

  static void UpdateGeometry();

private:
  SMConfigManager();		// Singleton: only constructed on request
  SMConfigManager(const SMConfigManager&) = delete;
  SMConfigManager(SMConfigManager&&) = delete;
  SMConfigManager& operator=(const SMConfigManager&) = delete;
  SMConfigManager& operator=(SMConfigManager&&) = delete;

  static SMConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)
  G4int CustomEventID;  // Starting point of event ID
  G4String PrimPartType;  // Primary particle type
  G4double PrimPartEnergy;  // Primary particle energy
  G4String PrimPartFluxFilename;  // Name of file containing primary particle flux distribution
  G4String PrimPartFluxHistname;  // Name of histogram with primary particle flux distribution

  SMConfigMessenger* messenger;
};

#endif	/* SMConfigManager_hh */
