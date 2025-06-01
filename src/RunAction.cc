#include "RunAction.hh"


RunAction::RunAction(){

}


RunAction::~RunAction(){
}


void RunAction::BeginOfRunAction(const G4Run *run){
    auto analysisManager = G4AnalysisManager::Instance();

    // analysisManager -> CreateH1("PhotonsCount", "PhotonsCount", 100, 0, 210000.);
    analysisManager -> CreateH1("PhotonsEnergy", "PhotonsEnergy", 100, 1.8, 2.71);
    // analysisManager -> CreateH1("CherenkovCount", "CherenkovCount", 100, 3600., 9400.);
    // analysisManager -> CreateH1("CherenkovEnergy", "CherenkovEnergy", 100, 1.2, 6.4);
    // analysisManager -> CreateH1("Photons Deposited Energy", "photDep", 100, 0, 10000);
    // analysisManager -> CreateH1("Electrons Deposited Energy", "elDep", 100, 0, 10);
    // analysisManager -> CreateH1("Energy", "edep", 100, 0, 100);

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y_%H:%M:%S", ltm);
    analysisManager -> OpenFile("/home/e/BBC/data_analyse/data/output_" + std::string(buffer) + ".root");
}


void RunAction::EndOfRunAction(const G4Run *run){
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager -> Write();
    analysisManager -> CloseFile();
    auto runID = run -> GetRunID();
    G4cout << "Finishing run " << runID << G4endl;
}
