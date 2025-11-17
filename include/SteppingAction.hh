#ifndef STEPPINGACTION_H
#define STEPPINGACTION_H

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Types.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(EventAction *eventAction);
    virtual ~SteppingAction() {}

    virtual void UserSteppingAction(const G4Step *step) override;

private:
    EventAction *fEventAction;
};

#endif
