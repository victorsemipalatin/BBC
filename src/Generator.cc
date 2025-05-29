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
    G4ParticleDefinition *particle = particleTable -> FindParticle("proton");
    // G4ParticleDefinition *particle = particleTable -> FindParticle("gamma");
    auto angle = 0. * degree;
    angle += 90. * degree;
    //71.
    G4int i = 0;
    G4int j = 3;
    // G4ThreeVector position(i * 6.63 * 1 * mm, (46. + j * 9.2116) * mm, 100. * mm);
    G4ThreeVector position(0. * mm, 80. * mm, 100. * mm);
    // G4ThreeVector position(-30. * mm, 30. * mm, 100. * mm);
    G4ThreeVector momentum(0., cos(angle), -sin(angle));

    ParticleGun -> SetParticlePosition(position);
    ParticleGun -> SetParticleMomentumDirection(momentum);
    ParticleGun -> SetParticleMomentum(4. * GeV);
    // ParticleGun -> SetParticleMomentum(50. * keV);
    ParticleGun -> SetParticleDefinition(particle);

    ParticleGun -> GeneratePrimaryVertex(Event);
}
