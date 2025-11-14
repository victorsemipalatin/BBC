#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "SensitiveDetector.hh"
#include "RunAction.hh"
// #include "BBCConstruction.hh"
#include "DetectorConstruction.hh"

#include <fstream>
#include <string>

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event *) override;
    void EndOfEventAction(const G4Event *) override;

private:
    G4String fOutputFileName;
};

#endif