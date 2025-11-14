#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"
#include "SensitiveDetector.hh"
#include "Randomize.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGenerator();
    ~PrimaryGenerator();
    virtual void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *ParticleGun;
    G4int count;
};

#endif
