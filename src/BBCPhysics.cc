#include "BBCPhysics.hh"


PhysicsList::PhysicsList(){
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}


PhysicsList::~PhysicsList(){
    std::cout << "Physics killed" << std::endl;
}
