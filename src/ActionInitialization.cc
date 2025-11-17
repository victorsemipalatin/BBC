#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
    G4cout << "ActionInitialization killed" << G4endl;
}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGenerator());
    SetUserAction(new RunAction());
    auto eventAction = new EventAction();
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction(eventAction));

    
}
