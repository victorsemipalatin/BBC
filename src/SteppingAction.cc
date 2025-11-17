#include "SteppingAction.hh"


SteppingAction::SteppingAction(EventAction *eventAction)
    : fEventAction(eventAction)
{
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    // Получаем пост-степ точку
    G4StepPoint *postStep = step->GetPostStepPoint();
    G4VPhysicalVolume *volume = postStep->GetPhysicalVolume();

    if (!volume)
        return;

    // Берём глобальное время частицы
    G4double t = postStep->GetGlobalTime();

    // Сохраняем самое большое время (последняя частица)
    fEventAction->SetLastParticleTime(t);
}
