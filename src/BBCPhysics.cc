#include "BBCPhysics.hh"


PhysicsList::PhysicsList(){
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4OpticalPhysics());
}


PhysicsList::~PhysicsList(){
    G4cout << "Physics killed" << G4endl;
}
