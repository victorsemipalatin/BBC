// #ifndef GENERATOR_HH
// #define GENERATOR_HH

// #include "G4VUserPrimaryGeneratorAction.hh"
// #include "G4ParticleGun.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4ParticleTable.hh"
// #include "Randomize.hh"
// #include "G4Event.hh"

// #include <HepMC3/GenEvent.h>
// #include <HepMC3/GenVertex.h>
// #include <HepMC3/GenParticle.h>
// #include <HepMC3/ReaderAscii.h>
// #include <HepMC3/Units.h>

// #include <memory>
// #include <string>

// class PrimaryGenerator: public G4VUserPrimaryGeneratorAction{
//   public:
//     PrimaryGenerator();
//     ~PrimaryGenerator() override;
//     void GeneratePrimaries(G4Event* event) override;
//   private:
//     G4ParticleGun* ParticleGun = nullptr;
//     G4int count = 0;
//     G4int q = 0;
//     G4int n = 1; // сколько частиц будет вылетать из партиклГан
// };
// #endif
