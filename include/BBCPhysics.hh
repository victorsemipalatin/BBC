#ifndef BBCPHYSICS_HH
#define BBCPHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "FTFP_BERT.hh"


class PhysicsList: public FTFP_BERT{
    public:
        PhysicsList();
        ~PhysicsList();
};

#endif
