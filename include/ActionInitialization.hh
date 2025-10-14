#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "Generator.hh"
#include "RunAction.hh"
#include "EventAction.hh"


class ActionInitialization: public G4VUserActionInitialization{
    public:
        ActionInitialization();
        ~ActionInitialization();
        virtual void BuildForMaster() const;
        virtual void Build() const; 
};

#endif
