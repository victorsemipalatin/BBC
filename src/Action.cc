#include "Action.hh"


ActionInitialization::ActionInitialization(){
}


ActionInitialization::~ActionInitialization(){
    G4cout << "ActionInitialization killed" << G4endl;
}


void ActionInitialization::Build() const{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);
}
