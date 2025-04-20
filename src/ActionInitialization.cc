#include "ActionInitialization.hh"


ActionInitialization::ActionInitialization(){
}


ActionInitialization::~ActionInitialization(){
    G4cout << "ActionInitialization killed" << G4endl;
}


void ActionInitialization::BuildForMaster() const{
    RunAction *runAction = new RunAction();
    SetUserAction(runAction);
}


void ActionInitialization::Build() const{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);
    auto runAction = new RunAction();
    SetUserAction(runAction);
}
