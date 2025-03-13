#include "Generator.hh"


PrimaryGenerator::PrimaryGenerator(){
    ParticleGun = new G4ParticleGun(1); // 1 частица в событие
}


PrimaryGenerator::~PrimaryGenerator(){
    delete ParticleGun;
    std::cout << "PrimaryGenerator killed" << std::endl;
}


void PrimaryGenerator::GeneratePrimaries(G4Event *Event){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String proton = "proton";
    G4ParticleDefinition *particle = particleTable -> FindParticle("proton");

    G4ThreeVector position(0., 0., 0.);
    G4ThreeVector momentum(0., 0., 1.);

    ParticleGun -> SetParticlePosition(position);
    ParticleGun -> SetParticleMomentumDirection(momentum);
    ParticleGun -> SetParticleMomentum(10. * GeV);
    ParticleGun -> SetParticleDefinition(particle);

    ParticleGun -> GeneratePrimaryVertex(Event);
}
