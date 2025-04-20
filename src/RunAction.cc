#include "RunAction.hh"


RunAction::RunAction(){
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager -> CreateH1("PhotonsCount", "PhotonsCount", 100, 5000., 13000.);


}


RunAction::~RunAction(){
}


void RunAction::BeginOfRunAction(const G4Run *run){
    auto analysisManager = G4AnalysisManager::Instance();
    auto runID = run -> GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
    analysisManager -> OpenFile("output" + strRunID.str() + ".root");


}


void RunAction::EndOfRunAction(const G4Run *run){
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager -> Write();
    analysisManager -> CloseFile();
    auto runID = run -> GetRunID();
    G4cout << "Finishing run " << runID << G4endl;

}