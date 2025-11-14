#include "RunAction.hh"

RunAction::RunAction()
{
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run *run)
{
    auto analysisManager = G4AnalysisManager::Instance();

    // analysisManager -> CreateH1("PhotonsCount", "PhotonsCount", 100, 0, 50.);
    // analysisManager -> CreateH1("PhotonsEnergy", "PhotonsEnergy", 100, 1.8, 2.71);
    // ******* Тестирование оптоволокна ******* //
    // for (G4int i = 0; i < 370; i++){
    //     G4String name = G4String(std::to_string(i));
    //     analysisManager -> CreateH1(name, "WLDistr", 100, 450, 540);
    // }
    // **************************************** //
    // analysisManager -> CreateH1("Finish", "PhotonsWlFinish", 100, 450, 540);
    // analysisManager -> CreateH1("CherenkovCount", "CherenkovCount", 100, 3600., 9400.);
    // analysisManager -> CreateH1("CherenkovEnergy", "CherenkovEnergy", 100, 1.2, 6.4);
    // analysisManager -> CreateH1("Photons Deposited Energy", "photDep", 100, 0, 10000);
    // analysisManager -> CreateH1("Electrons Deposited Energy", "elDep", 100, 0, 10);
    // analysisManager -> CreateH1("Energy", "edep", 100, 0, 100);

    // *********** Энерговыделение *********** //
    // analysisManager -> CreateH1("Electrons Deposited Energy", "elDep", 100, 0.01, 16);
    // **************************************** //

    // ********* Разлёт гамма-квантов ********* //
    // analysisManager -> CreateH1("left", "left", 3, 0, 2);
    // analysisManager -> CreateH1("rigt", "rigt", 3, 0, 2);
    // analysisManager -> CreateH1("back", "back", 3, 0, 2);
    // analysisManager -> CreateH1("front", "front", 3, 0, 2);
    // analysisManager -> CreateH1("bottom", "bottom", 3, 0, 2);
    // analysisManager -> CreateH1("top", "top", 3, 0, 2);
    // **************************************** //

    // time_t now = time(nullptr);
    // tm* ltm = localtime(&now);
    // char buffer[80];
    // strftime(buffer, sizeof(buffer), "%d.%m.%Y_%H:%M:%S", ltm);
    // analysisManager -> OpenFile("/home/e/BBC/data_analyse/data/output_" + std::string(buffer) + ".root");
}

void RunAction::EndOfRunAction(const G4Run *run)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    auto runID = run->GetRunID();
    G4cout << "Finishing run " << runID << G4endl;
}
