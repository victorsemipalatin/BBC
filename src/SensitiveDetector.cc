#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name){
    fTotalEnergyDeposited = 0.;
    count = 0;
    photonsEnergy = 0.;
    cerenkovCount = 0.;
    elDep = 0;
    photDep = 0;
}


SensitiveDetector::~SensitiveDetector(){
    G4cout << "SensitiveDetector killed" << G4endl;
}


void SensitiveDetector::Initialize(G4HCofThisEvent *){
    fTotalEnergyDeposited = 0.;
    count = 0;
    photonsEnergy = 0.;
    cerenkovCount = 0;
    elDep = 0;
    photDep = 0;
}


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    auto analysisManager = G4AnalysisManager::Instance();
    G4Track* track = aStep -> GetTrack();
    auto creatorProcessPtr = track -> GetCreatorProcess();
    G4String creatorProcess = creatorProcessPtr ? creatorProcessPtr -> GetProcessName() : "primary";
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();

    // G4double energyDeposited = aStep -> GetTotalEnergyDeposit();
    // if (energyDeposited > 0) 
    //     fTotalEnergyDeposited += energyDeposited;

    if(aStep -> GetTrack() -> GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
        // if (energyDeposited > 0){
        //     photDep += energyDeposited;
        // }

        count += 1;
        photonsEnergy = aStep -> GetTrack() -> GetKineticEnergy();
        analysisManager -> FillH1(1, photonsEnergy / eV);
        analysisManager -> FillH1(2, 2 * 3.14 * 1.05e-34 * 3e8  / (photonsEnergy / eV) / 1.6 * 1e28);
    }

    // if (creatorProcess == "Cerenkov"){
    //     cerenkovCount += 1;
    //     analysisManager -> FillH1(3, aStep -> GetTrack() -> GetKineticEnergy() / eV);
    // }

    // if(aStep -> GetTrack() -> GetParticleDefinition() == G4Electron::ElectronDefinition()){
    //     if (energyDeposited > 0)
    //         elDep += energyDeposited;
    // }

    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *){
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager -> FillH1(0, count);
    // analysisManager -> FillH1(2, cerenkovCount);
    // analysisManager -> FillH1(4, photDep / eV);
    // analysisManager -> FillH1(5, elDep / keV);
    // analysisManager -> FillH1(6, fTotalEnergyDeposited);
}
