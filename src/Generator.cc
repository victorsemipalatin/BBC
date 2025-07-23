#include "Generator.hh"
#include "G4OpticalPhoton.hh"


PrimaryGenerator::PrimaryGenerator(){
    ParticleGun = new G4ParticleGun(1); // 1 частица в событии
    count = 0;
}


PrimaryGenerator::~PrimaryGenerator(){
    delete ParticleGun;
    G4cout << "PrimaryGenerator killed" << G4endl;
}


void PrimaryGenerator::GeneratePrimaries(G4Event *Event){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable -> FindParticle("proton");
    // G4ParticleDefinition *particle = particleTable -> FindParticle("gamma");
    // auto particle = G4OpticalPhoton::Definition();
    auto angle = 0. * degree;
    angle += 90. * degree;
    //71.
    G4int i = 0;
    G4int j = 3;
    // G4ThreeVector position(i * 6.63 * 1 * mm, (46. + j * 9.2116) * mm, 100. * mm);
    // G4ThreeVector position(0. * mm, 80. * mm, 100. * mm);
    // G4ThreeVector position(-30. * mm, 30. * mm, 100. * mm);
    // G4ThreeVector momentum(0., cos(angle), -sin(angle));

    // **************** Tube **************** //
    G4ThreeVector position(-100 * mm, 0, -900 / 2 * mm - 1.1 * mm);
    G4ThreeVector momentum(1., 0., 0.);
    // **************************************** //

    // **************** Ring **************** //
    // G4ThreeVector position(-13.48 * mm, -1. * mm, -100. * mm);
    // G4ThreeVector momentum(0, 0., 1.);
    // **************************************** //


    G4double energy = 4. * GeV;
    ParticleGun -> SetParticleEnergy(energy); // задание энергии первичной частицы
    ParticleGun -> SetParticlePosition(position); // позиция первичной частицы
    ParticleGun -> SetParticleMomentumDirection(momentum); // задание вектора импульса
    ParticleGun -> SetParticleDefinition(particle);

    ParticleGun -> GeneratePrimaryVertex(Event);
}
