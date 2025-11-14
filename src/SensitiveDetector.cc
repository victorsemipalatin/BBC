#include "SensitiveDetector.hh"


G4ThreadLocal G4double SensitiveDetector::count = 0.;


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name){
    fTotalEnergyDeposited = 0.;
    photonsEnergy = 0.;
    // cerenkovCount = 0.;
    elDep = 0;
    photDep = 0;
}


SensitiveDetector::~SensitiveDetector(){
    G4cout << "SensitiveDetector killed" << G4endl;
}


void SensitiveDetector::Initialize(G4HCofThisEvent *){
    fTotalEnergyDeposited = 0.;
    count = 0.;
    photonsEnergy = 0.;
    elDep = 0;
    photDep = 0;
    check = 0;
    countedTracks.clear();
}


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    auto analysisManager = G4AnalysisManager::Instance();
    auto track = aStep -> GetTrack();
    auto creatorProcessPtr = track -> GetCreatorProcess();
    G4String creatorProcess = creatorProcessPtr ? creatorProcessPtr -> GetProcessName() : "primary";
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();
    
    // ******* Тестирование оптоволокна ******* //
    // G4LogicalVolume* currentVolume = aStep -> GetPreStepPoint() -> GetTouchableHandle() -> GetVolume() -> GetLogicalVolume();
    // G4String volumeName = currentVolume -> GetName();
    // G4int number = aStep -> GetPreStepPoint() -> GetTouchableHandle() -> GetCopyNumber();

    // if (volumeName == "logicStartDetector"){
    //     if(aStep -> GetTrack() -> GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
    //         // if (energyDeposited > 0){
    //         //     photDep += energyDeposited;
    //         // }
    //         // count += 1;
    //         photonsEnergy = aStep -> GetTrack() -> GetKineticEnergy();
    //         // analysisManager -> FillH1(1, photonsEnergy / eV);
    //         analysisManager -> FillH1(2, 2 * 3.14 * 1.05e-34 * 3e8  / (photonsEnergy / eV) / 1.6 * 1e28);
    //     }
    // }
    // **************************************** //

    // ********** Тестирование тайла  ********** //
    if(aStep -> GetTrack() -> GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
        if (aStep -> IsFirstStepInVolume()){
            auto trackID = track -> GetTrackID();
            if (countedTracks.find(trackID) == countedTracks.end()){
                countedTracks.insert(trackID);
                count += 1.;
            }
        }
    }
    // **************************************** //

    // *********** Энерговыделение *********** //
    // G4double energyDeposited = aStep -> GetTotalEnergyDeposit();
    // if (energyDeposited > 0) 
    //     fTotalEnergyDeposited += energyDeposited;

    // // if (creatorProcess == "Cerenkov"){
    // //     cerenkovCount += 1;
    // //     analysisManager -> FillH1(3, aStep -> GetTrack() -> GetKineticEnergy() / eV);
    // // }
    // if(aStep -> GetTrack() -> GetParticleDefinition() == G4Electron::ElectronDefinition()){
    //     G4double energyDeposited = aStep -> GetTotalEnergyDeposit();
    //     if (energyDeposited > 0){
    //         elDep += energyDeposited;
    //         // if (track -> GetCreatorProcess() -> GetProcessName() == "phot")
    //         //     G4cout << elDep / keV << " phot" << G4endl;
    //         // else
    //         //     G4cout << elDep / keV << " compt" << G4endl;
    //     }
    // }
    // **************************************** //

    // ********* Разлёт гамма-квантов ********* //
    // auto physVolume = aStep -> GetPreStepPoint() -> GetTouchableHandle() -> GetVolume();
    // G4String volumeName = physVolume -> GetName();
    // G4int number = aStep -> GetPreStepPoint() -> GetTouchableHandle() -> GetCopyNumber();
    // if (creatorProcess == "primary")
    //     if (volumeName != "physTile1"){
    //         // photonsEnergy = aStep -> GetTrack() -> GetKineticEnergy();
    //         // analysisManager -> FillH1(number, 1);
    //         // if (number == 0 || number == 1)
    //             // analysisManager -> FillH1(6, photonsEnergy / keV);
    //         if ((number == 0 || number == 1) && aStep -> IsFirstStepInVolume() && track -> GetParentID() == 0){
    //             if (track -> GetParentID() == 0 && aStep -> IsFirstStepInVolume()){
    //                 auto trackID = track -> GetTrackID();
    //                 if (countedTracks.find(trackID) == countedTracks.end()) {
    //                     countedTracks.insert(trackID);
    //                     count += 1.;
    //                 }
    //             }
    //         }
    //     }
    // **************************************** //


    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *){
    auto analysisManager = G4AnalysisManager::Instance();
    // for(int i = 0; i < 80; i++)
    //     analysisManager -> FillH1(2 + i, count[i]);

    // analysisManager -> FillH1(0, count);
    // analysisManager -> FillH1(2, cerenkovCount);
    // analysisManager -> FillH1(4, photDep / eV);
    // analysisManager -> FillH1(5, elDep / keV);

    // *********** Энерговыделение *********** //
    // analysisManager -> FillH1(0, elDep / keV);
    // **************************************** //

    // ********* Разлёт гамма-квантов ********* //
    // if (fTotalEnergyDeposited == 0 & check == 1)
    //     analysisManager -> FillH1(8, 1);
    // **************************************** //
}
