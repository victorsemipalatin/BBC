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

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();
    static G4double GetCount() { return count; }

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void Initialize(G4HCofThisEvent *) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;
    G4double fTotalEnergyDeposited, photonsEnergy, elDep, photDep;
    G4int check;
    // static G4ThreadLocal G4double count;
    static G4ThreadLocal G4double count;
    std::unordered_set<G4int> countedTracks;
};

#endif