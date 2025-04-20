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
    auto angle = 0. * degree;
    angle += 90. * degree;
    //71.
    G4ThreeVector position(0. * mm, 80. * mm, 100. * mm);
    G4ThreeVector momentum(0., cos(angle), -sin(angle));

    ParticleGun -> SetParticlePosition(position);
    ParticleGun -> SetParticleMomentumDirection(momentum);
    ParticleGun -> SetParticleMomentum(4. * GeV);
    ParticleGun -> SetParticleDefinition(particle);

    ParticleGun -> GeneratePrimaryVertex(Event);
}
