#include "BBCPhysics.hh"


PhysicsList::PhysicsList(){
    RegisterPhysics(new G4EmStandardPhysics());

    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    auto opticalParams = G4OpticalParameters::Instance();    
    opticalParams -> SetProcessActivation("Cerenkov", true);
    opticalParams -> SetProcessActivation("Scintillation", true);
    opticalParams -> SetProcessActivation("OpWLS", true);
    opticalParams -> SetProcessActivation("OpAbsorption", true);
    
    RegisterPhysics(opticalPhysics);
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}


PhysicsList::~PhysicsList(){
    G4cout << "Physics killed" << G4endl;
}
