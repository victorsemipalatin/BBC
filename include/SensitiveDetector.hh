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
#include <vector>

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    SensitiveDetector(G4String);
    ~SensitiveDetector();
    static G4double GetCount() { return fParticlesCount; }
    static G4double GetMinTime() { return fMinTime; }
    static G4double GetMaxTime() { return fMaxTime; }

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    virtual void Initialize(G4HCofThisEvent *) override;
    virtual void EndOfEvent(G4HCofThisEvent *) override;
    G4double fTotalEnergyDeposited, fPhotonsEnergy, fElDep, photDep;
    G4int check;
    static G4double fParticlesCount;
    static G4double fMinTime, fMaxTime;
    std::unordered_set<G4int> fCountedTracks;
};

#endif