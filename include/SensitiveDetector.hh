#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4OpticalPhoton.hh"
#include <G4Electron.hh>
#include <unordered_set>


class SensitiveDetector : public G4VSensitiveDetector{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void Initialize(G4HCofThisEvent*) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;
    G4double fTotalEnergyDeposited, photonsEnergy, elDep, photDep;
    // G4int count, cerenkovCount;
    // G4int count[80];
};

#endif