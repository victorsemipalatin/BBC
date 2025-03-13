#include "Action.hh"


ActionInitialization::ActionInitialization(){
}


ActionInitialization::~ActionInitialization(){
    std::cout << "ActionInitialization killed" << std::endl;
}


void ActionInitialization::Build() const{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);
}
