/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

/// \file exoticphysics/phonon/src/PhononDetectorConstruction.cc \brief
/// Implementation of the PhononDetectorConstruction class
//
// $Id: a2016d29cc7d1e75482bfc623a533d20b60390da $
//
// 20140321  Drop passing placement transform to G4LatticePhysical
// 20211207  Replace G4Logical*Surface with G4CMP-specific versions.
// 20220809  [ For M. Hui ] -- Add frequency dependent surface properties.
// 20221006  Remove unused features; add phonon sensor pad with use of
//		G4CMPPhononElectrode to demonstrate KaplanQP.

#include "PhononDetectorConstruction.hh"
#include "PhononSensitivity.hh"
#include "G4CMPLogicalBorderSurface.hh"
#include "G4CMPPhononElectrode.hh"
#include "G4CMPSurfaceProperty.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4LatticeLogical.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Element.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PhononDetectorConstruction::PhononDetectorConstruction()
  : fLiquidHelium(0), fTungsten(0), fCaWO4(0),
    fWorldPhys(0), topSurfProp(0), wallSurfProp(0),
    electrodeSensitivity(0), fConstructed(false), fpSubstrateLV(0) {;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PhononDetectorConstruction::~PhononDetectorConstruction() {
  delete topSurfProp;
  delete wallSurfProp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* PhononDetectorConstruction::Construct()
{
  if (fConstructed) {
    if (!G4RunManager::IfGeometryHasBeenDestroyed()) {
      // Run manager hasn't cleaned volume stores. This code shouldn't execute
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
    }
    // Have to completely remove all lattices to avoid warning on reconstruction
    G4LatticeManager::GetLatticeManager()->Reset();
    // Clear all LogicalSurfaces
    // NOTE: No need to redefine the G4CMPSurfaceProperties
    G4CMPLogicalBorderSurface::CleanSurfaceTable();
  }

  DefineMaterials();
  SetupGeometry();
  fConstructed = true;

  return fWorldPhys;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
void PhononDetectorConstruction::ConstructSDandField() {
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
 
  PhononSensitivity* electrodeSD
    = new PhononSensitivity("PhononElectrode");
 
  SDman->AddNewDetector(electrodeSD);
  fpSubstrateLV->SetSensitiveDetector(electrodeSD);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhononDetectorConstruction::DefineMaterials()
{ 
  G4NistManager* nistManager = G4NistManager::Instance();

  fLiquidHelium = nistManager->FindOrBuildMaterial("G4_AIR"); // to be corrected
  fTungsten = nistManager->FindOrBuildMaterial("G4_W");
  G4Element *elCa = nistManager->FindOrBuildElement("Ca");
  G4Element *elW = nistManager->FindOrBuildElement("W");
  G4Element *elO = nistManager->FindOrBuildElement("O");
  G4double density = 6.1*g/cm3;
  fCaWO4 = new G4Material("CalciumTungstate", density, 3);
  fCaWO4->AddElement(elCa, 1);
  fCaWO4->AddElement(elW, 1);
  fCaWO4->AddElement(elO, 4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PhononDetectorConstruction::SetupGeometry()
{
  //     
  // World
  //
  G4VSolid* worldSolid = new G4Box("World",5.*cm,5.*cm,5.*cm);
  G4LogicalVolume* worldLogical =
    new G4LogicalVolume(worldSolid,fLiquidHelium,"World");
  worldLogical->SetUserLimits(new G4UserLimits(10*mm, DBL_MAX, DBL_MAX, 0, 0));
  fWorldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"World",0,
                                 false,0);
  
  //                               
  // Calcium Tungstate cylinder - this is the volume in which we will propagate phonons
  //  
  G4VSolid* fCaWO4Solid = new G4Tubs("fCaWO4Solid",0.*cm,2.*cm,
                                         2.*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* fCaWO4Logical =
    new G4LogicalVolume(fCaWO4Solid,fCaWO4,"fCaWO4Logical");
  G4VPhysicalVolume* CaWO4Phys =
    new G4PVPlacement(0,G4ThreeVector(),fCaWO4Logical,"fCaWO4Physical",
                      worldLogical,false,0);

  //
  //CaWO4 lattice information
  //

  // G4LatticeManager gives physics processes access to lattices by volume
  G4LatticeManager* LM = G4LatticeManager::GetLatticeManager();
  G4LatticeLogical* CaWO4Logical = LM->LoadLattice(fCaWO4, "CaWO4");

  // G4LatticePhysical assigns G4LatticeLogical a physical orientation
  G4LatticePhysical* CaWO4Physical = new G4LatticePhysical(CaWO4Logical);
  CaWO4Physical->SetMillerOrientation(1,0,0);
  LM->RegisterLattice(CaWO4Phys, CaWO4Physical);

  // Tungsten - film end caps. This is where phonon hits are registered
  //
  G4VSolid* fTungstenSolid = new G4Tubs("TungstenSolid",0.*cm,2.*cm,100.*nm,
                                        0.*deg, 360.*deg);

  G4LogicalVolume* fTungstenLogical =
    new G4LogicalVolume(fTungstenSolid,fTungsten,"fTungstenLogical");
  G4VPhysicalVolume* TungstenTopPhysical = new G4PVPlacement(0,
    G4ThreeVector(0.,0.,2.*cm+100.*nm), fTungstenLogical, "fTungstenPhysical",
    worldLogical,false,0);

  // detector -- Note : "sensitive detector" is attached to CaWO4 crystal
  fpSubstrateLV=fCaWO4Logical;
  /*
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if (!electrodeSensitivity)
    electrodeSensitivity = new PhononSensitivity("PhononElectrode");
  SDman->AddNewDetector(electrodeSensitivity);
  fCaWO4Logical->SetSensitiveDetector(electrodeSensitivity);

  */
  // surface between W and CaWO4 determines phonon reflection/absorption
  //
  if (!fConstructed) {

    const G4double GHz = 1e9 * hertz; 

    //the following coefficients and cutoff values are not well-motivated
    //the code below is used only to demonstrate how to set these values.
    const std::vector<G4double> anhCoeffs = {0, 0, 0, 0, 0, 1.51e-14};
    const std::vector<G4double> diffCoeffs =
      {5.88e-2, 7.83e-4, -2.47e-6, 1.71e-8, -2.98e-11};
    const std::vector<G4double> specCoeffs =
      {0,928, -2.03e-4, -3.21e-6, 3.1e-9, 2.9e-13};

    const G4double anhCutoff = 520., reflCutoff = 350.;   // Units external

    topSurfProp = new G4CMPSurfaceProperty("TopSurf", 1.0, 0.0, 0.0, 0.0,
					  	        0.5, 1.0, 0.3, 0.0);
    topSurfProp->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
					 diffCoeffs, specCoeffs, GHz, GHz, GHz);
    AttachPhononSensor(topSurfProp);

    wallSurfProp = new G4CMPSurfaceProperty("WallSurf", 0.0, 1.0, 0.0, 0.0,
					    	          0.0, 1.0, 0.3, 0.0);
    wallSurfProp->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
					  diffCoeffs, specCoeffs, GHz, GHz,GHz);

  }


  //
  // Separate surfaces for sensors vs. bare sidewall
  //
  new G4CMPLogicalBorderSurface("detTop", CaWO4Phys, TungstenTopPhysical,
				topSurfProp);
  new G4CMPLogicalBorderSurface("detWall", CaWO4Phys, fWorldPhys,
				wallSurfProp);

  //                                        
  // Visualization attributes
  //
  worldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  fCaWO4Logical->SetVisAttributes(simpleBoxVisAtt);
  fTungstenLogical->SetVisAttributes(simpleBoxVisAtt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// Attach material properties and electrode/sensor handler to surface

void PhononDetectorConstruction::
AttachPhononSensor(G4CMPSurfaceProperty *surfProp) {
  if (!surfProp) return;		// No surface, nothing to do

  // Specify properties of aluminum sensor, same on both detector faces
  // See G4CMPPhononElectrode.hh or README.md for property keys

  // Properties must be added to existing surface-property table
  auto sensorProp = surfProp->GetPhononMaterialPropertiesTablePointer();
  sensorProp->AddConstProperty("filmAbsorption", 1.0);    // True sensor area
  sensorProp->AddConstProperty("filmThickness", 200.*nm);
  sensorProp->AddConstProperty("gapEnergy", 2.3e-6*eV);
  sensorProp->AddConstProperty("lowQPLimit", 3.);
  sensorProp->AddConstProperty("phononLifetime", 250.*ps);
  sensorProp->AddConstProperty("phononLifetimeSlope", 0.25);
  sensorProp->AddConstProperty("vSound", 5.2*km/s);
  sensorProp->AddConstProperty("subgapAbsorption", 0.1);

  // Attach electrode object to handle KaplanQP interface
  surfProp->SetPhononElectrode(new G4CMPPhononElectrode);
}


