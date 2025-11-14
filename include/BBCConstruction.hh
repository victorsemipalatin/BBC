#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"
#include "G4SubtractionSolid.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4ExtrudedSolid.hh"
#include "G4UnionSolid.hh"
#include <vector>
#include <cmath>

#define PI 3.14159265

class BBCConstruction : public G4VUserDetectorConstruction
{
public:
    BBCConstruction();
    virtual ~BBCConstruction();
    virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume *logicDetector1;
    G4LogicalVolume *logicDetector2;
    G4LogicalVolume *logicDetector5;
    virtual void ConstructSDandField();
};

#endif
