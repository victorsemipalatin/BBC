#include "Generator.hh"


PrimaryGenerator::PrimaryGenerator(){
    ParticleGun = new G4ParticleGun(1); // 1 частица в событии
}


PrimaryGenerator::~PrimaryGenerator(){
    delete ParticleGun;
    G4cout << "PrimaryGenerator killed" << G4endl;
}


void PrimaryGenerator::GeneratePrimaries(G4Event *Event){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    // G4ParticleDefinition *particle = particleTable -> FindParticle("proton");
    G4ParticleDefinition *particle = particleTable -> FindParticle("gamma");
    // auto particle = G4OpticalPhoton::Definition();

    // **Тестирование всей поверхности 5 тайла** //
    // auto angle = 0. * degree;
    // angle += 90. * degree;
    // G4int batch_size = 1000;
    // G4long eventID = Event -> GetEventID();

    // const G4int NX = 65;
    // const G4int NY = 58;

    // const G4int nPerCell = batch_size;
    // const G4int ix = (eventID / nPerCell) % NX;
    // const G4int iy = (eventID / nPerCell / NX) % NY;

    // const G4double x_min = -65. * mm;
    // const G4double x_max = 0. * mm;
    // const G4double y_min = 0. * mm;
    // const G4double y_max = 58. * mm;

    // const G4double x_step = (x_max - x_min) / NX;
    // const G4double y_step = (y_max - y_min) / NY;

    // const G4double xr = x_min + x_step * ix;
    // const G4double yr = y_min + y_step * iy;

    // G4double radius = 2.5;
    // G4double xi =  (2 * G4UniformRand() -1.0) * radius;
    // G4double yi = (2 * G4UniformRand() -1.0) * radius;
    // while ((xi * xi + yi * yi) > radius * radius){
    //     xi = (2 * G4UniformRand() -1.0) * radius;
    //     yi = (2 * G4UniformRand() - 1.0) * radius;
    // }

    // auto x = (xr + xi) * mm;
    // auto y = (yr + yi) * mm;
    // G4ThreeVector position(x, y, 30. * mm);
    // G4ThreeVector momentum(0., cos(angle), -sin(angle));
    // **************************************** //

    // **************** Tube **************** //
    // // Свет в торец
    // G4double l = 900;
    // G4ThreeVector position(0, 0, 0.);
    // G4ThreeVector momentum(0., 0., 1.);
    // Свет в боковую часть
    // G4ThreeVector position(0., 5. * mm, -900 / 2 * mm - 10. * mm);
    // G4ThreeVector momentum(0., 0., 1.);
    // **************************************** //

    // **************** Ring **************** //
    // G4ThreeVector position(-13.48 * mm, -1. * mm, -100. * mm);
    // G4ThreeVector momentum(0, 0., 1.);
    // **************************************** //

    //***** Проверка разлёта гамма-квантов *****//
    G4int batch_size = 10000;
    G4long eventID = Event -> GetEventID();

    const G4int NX = 30;
    const G4int NY = 30;

    const G4int nPerCell = batch_size;
    const G4int ix = (eventID / nPerCell) % NX;
    const G4int iy = (eventID / nPerCell / NX) % NY;

    const G4double x_min = -25. * mm;
    const G4double x_max = 25. * mm;
    const G4double y_min = 0. * mm;
    const G4double y_max = 70. * mm;

    const G4double x_step = (x_max - x_min) / NX;
    const G4double y_step = (y_max - y_min) / NY;

    const G4double xr = x_min + x_step * ix;
    const G4double yr = y_min + y_step * iy;

    G4double radius = 2.5;
    G4double xi =  (2 * G4UniformRand() -1.0) * radius;
    G4double yi = (2 * G4UniformRand() -1.0) * radius;
    while ((xi * xi + yi * yi) > radius * radius){
        xi = (2 * G4UniformRand() -1.0) * radius;
        yi = (2 * G4UniformRand() - 1.0) * radius;
    }

    auto x = (xr + xi) * mm;
    auto y = (yr + yi + 40) * mm;

    G4ThreeVector position(x, y, 30. * mm);
    G4ThreeVector momentum(0., 0., -1.);
    // **************************************** //


    G4double energy = 22. * keV;
    // G4double energy = 2.7899499 * eV;
    ParticleGun -> SetParticleEnergy(energy); // задание энергии первичной частицы
    ParticleGun -> SetParticlePosition(position); // позиция первичной частицы
    ParticleGun -> SetParticleMomentumDirection(momentum); // задание вектора импульса
    ParticleGun -> SetParticleDefinition(particle);
    ParticleGun -> GeneratePrimaryVertex(Event);

    // для фотонов
    // const int N = 100;
    // for(int i = 0; i < N; i++)
    //     ParticleGun -> GeneratePrimaryVertex(Event);
}
