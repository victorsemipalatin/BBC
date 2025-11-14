#include "BBCPhysics.hh"

PhysicsList::PhysicsList()
{
    G4OpticalPhysics *opticalPhysics = new G4OpticalPhysics();
    auto opticalParams = G4OpticalParameters::Instance();       // оптические параметры
    opticalParams->SetProcessActivation("Cerenkov", true);      // включение процесса рождения черенковских фотонов
    opticalParams->SetProcessActivation("Scintillation", true); // включение процесса сцинтилляции
    opticalParams->SetProcessActivation("OpWLS", true);         // включение спектросмещения сцинтиллятора
    opticalParams->SetProcessActivation("OpAbsorption", true);  // включение поглощения
    RegisterPhysics(opticalPhysics);
}

PhysicsList::~PhysicsList()
{
    G4cout << "Physics killed" << G4endl;
}
