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
    fEventStartTime = 0.0;
    // Устанавливаем время начала события из первичной вершины
    if (event->GetNumberOfPrimaryVertex() > 0)
    {
        G4PrimaryVertex *primaryVertex = event->GetPrimaryVertex(0);
        fEventStartTime = primaryVertex->GetT0();
    }
}

void EventAction::EndOfEventAction(const G4Event *event)
{
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
    G4double duration = fLastParticleTime - fEventStartTime;
    // G4cout << count / duration * 1000 << "MHz" << G4endl;

    G4AutoLock lock(&writeMutex);
    std::ofstream file(fOutputFileName, std::ios::app);
    if (file.is_open())
    {
        file << count / duration * 1000 << G4endl;
        file.close();
    }

    // **************************************** //
}