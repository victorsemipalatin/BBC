#ifndef CONSTRUCTION_HH
#define CINSTRUCTION_HH

#include <iostream>
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"


class BBCConstruction: public G4VUserDetectorConstruction{
    public:
        BBCConstruction();
        ~BBCConstruction();
        virtual G4VPhysicalVolume *Construct();
};

#endif
