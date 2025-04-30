#include "RunAction.hh"


RunAction::RunAction(){
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager -> CreateH1("PhotonsCount", "PhotonsCount", 100, 38000., 100000.);
    analysisManager -> CreateH1("PhotonsEnergy", "PhotonsEnergy", 100, 1.2, 6.4);
    analysisManager -> CreateH1("CherenkovCount", "CherenkovCount", 100, 1500., 5000.);
}


RunAction::~RunAction(){
}


void RunAction::BeginOfRunAction(const G4Run *run){
    auto analysisManager = G4AnalysisManager::Instance();
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
