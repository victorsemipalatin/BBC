#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include <G4RotationMatrix.hh>
#include <G4TwoVector.hh>
#include "G4VisAttributes.hh"

#define PI 3.14159265

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume *Construct();

private:
    G4LogicalVolume *logicDetector1;
    G4LogicalVolume *logicDetector2;
    G4LogicalVolume *logicDetector3;
    G4LogicalVolume *logicDetector4;
    G4LogicalVolume *logicDetector5;
    virtual void ConstructSDandField();
};

#endif
