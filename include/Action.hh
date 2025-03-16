#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "Generator.hh"


class ActionInitialization: public G4VUserActionInitialization{
    public:
        ActionInitialization();
        ~ActionInitialization();
        virtual void Build() const; 
};

#endif
