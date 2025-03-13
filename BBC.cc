#include <iostream>
#include "G4RunManager.hh"
#include <G4UImanager.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include "BBCConstruction.hh"
#include "BBCPhysics.hh"
#include "Action.hh"


int main(int argc, char **argv){

    G4RunManager *runManager = new G4RunManager();
    runManager -> SetUserInitialization(new BBCConstruction());
    runManager -> SetUserInitialization(new PhysicsList());
    runManager -> SetUserInitialization(new ActionInitialization());
    runManager -> Initialize();

    G4VisManager *visManager = new G4VisExecutive;
    visManager -> Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    // UImanager -> ApplyCommand("/vis/open OGL");
    // UImanager -> ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    // UImanager -> ApplyCommand("/vis/drawVolume");
    // UImanager -> ApplyCommand("/vis/viewer/set/autoRefresh true");
    // UImanager -> ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/control/execute init_vis.mac");

    G4UIExecutive* ui = nullptr;
    if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }
    if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
      }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    return 0;
}
