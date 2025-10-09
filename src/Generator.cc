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


// G4double SampleXRayEnergy() {
//     G4double r = G4UniformRand();
//     if (r < 0.3)
//         return 22.1 * keV;  // K_alpha
//     else if (r < 0.35)
//         return 24.9 * keV;  // K_beta
//     else{
//         G4double E_min = 1 * keV;
//         G4double E_max = 20 * keV;
//         return E_min + (E_max - E_min) * G4UniformRand();
//     }
// }


void PrimaryGenerator::GeneratePrimaries(G4Event *Event){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    // G4ParticleDefinition *particle = particleTable -> FindParticle("proton");
    G4ParticleDefinition *particle = particleTable -> FindParticle("gamma");
    // auto particle = G4OpticalPhoton::Definition();
    auto angle = 0. * degree;
    angle += 90. * degree;
    //71.
    G4int i = 0;
    G4int j = 3;
    // G4ThreeVector position(i * 6.63 * 1 * mm, (46. + j * 9.2116) * mm, 100. * mm);
    G4ThreeVector position(0. * mm, 80. * mm, 30. * mm);
    // G4ThreeVector position(-30. * mm, 30. * mm, 100. * mm);
    G4ThreeVector momentum(0., cos(angle), -sin(angle));

    // **************** Tube **************** //
    // Свет в торец
    // G4double l = 900;
    // G4ThreeVector position(-100 * mm, 0, -l / 2 * mm - 1.1 * mm);
    // G4ThreeVector momentum(1., 0., 0.);
    // Свет в боковую часть
    // G4ThreeVector position(0., 5. * mm, -900 / 2 * mm - 10. * mm);
    // G4ThreeVector momentum(0., 0., 1.);
    // **************************************** //

    // **************** Ring **************** //
    // G4ThreeVector position(-13.48 * mm, -1. * mm, -100. * mm);
    // G4ThreeVector momentum(0, 0., 1.);
    // **************************************** //


    G4double energy = 22. * keV;
    ParticleGun -> SetParticleEnergy(energy); // задание энергии первичной частицы
    ParticleGun -> SetParticlePosition(position); // позиция первичной частицы
    ParticleGun -> SetParticleMomentumDirection(momentum); // задание вектора импульса
    ParticleGun -> SetParticleDefinition(particle);

    ParticleGun -> GeneratePrimaryVertex(Event);
}
