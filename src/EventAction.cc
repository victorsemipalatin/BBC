#include "EventAction.hh"

namespace
{
    G4Mutex writeMutex = G4MUTEX_INITIALIZER;
}

EventAction::EventAction()
{
    // fOutputFileName = "/home/e/BBC/data_analyse/data/gamma_test_2.txt";
    // fOutputFileName = "/home/e/BBC/data_analyse/data/tile_5_test_spectrum_1.txt";
    fOutputFileName = "/home/e/BBC/data_analyse/data/loadings.txt";
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *event)
{
    // fEventStartTime = 0.0;
    // // Устанавливаем время начала события из первичной вершины
    // if (event->GetNumberOfPrimaryVertex() > 0)
    // {
    //     G4PrimaryVertex *primaryVertex = event->GetPrimaryVertex(0);
    //     fEventStartTime = primaryVertex->GetT0();
    // }
    // fSensitiveDetector->ClearHitTimes();
}

void EventAction::EndOfEventAction(const G4Event *event)
{
    auto analysisManager = G4AnalysisManager::Instance();
    // G4int eventID = event->GetEventID();
    // if (eventID % 1000000 == 0)
    //     G4cout << eventID << " events are processed" << G4endl;
    // auto count = SensitiveDetector::GetCount();
    // auto vertex = event->GetPrimaryVertex();
    // auto position = vertex->GetPosition();
    // auto x = position.x();
    // auto y = position.y();
    // G4AutoLock lock(&writeMutex);
    // std::ofstream file(fOutputFileName, std::ios::app);
    // if (file.is_open())
    // {
    //     file << std::to_string(x) << '\t' << std::to_string(y) << '\t' << count << G4endl;
    //     file.close();
    // }

    // **************** Загрузки *************** //

    auto count = SensitiveDetector::GetCount();
    auto minTime = SensitiveDetector::GetMinTime();
    auto maxTime = SensitiveDetector::GetMaxTime();
    if (minTime != maxTime)
    {
        auto loading = count / ((maxTime - minTime) / ns) * 1000;
        // G4cout << count / ((maxTime - minTime) / ns) * 1000 << "MHz" << G4endl;
        analysisManager->FillH1(0, loading);
    }

    // **************************************** //
}