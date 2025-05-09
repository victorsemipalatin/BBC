#include "SensitiveDetector.hh"


SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name){
    fTotalEnergyDeposited = 0.;
    count = 0;
    photonsEnergy = 0.;
    cerenkovCount = 0.;
}


SensitiveDetector::~SensitiveDetector(){
    G4cout << "SensitiveDetector killed" << G4endl;
}


void SensitiveDetector::Initialize(G4HCofThisEvent *){
    fTotalEnergyDeposited = 0.;
    count = 0;
    photonsEnergy = 0.;
    cerenkovCount = 0;
}


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    auto analysisManager = G4AnalysisManager::Instance();
    G4Track* track = aStep -> GetTrack();
    auto creatorProcessPtr = track -> GetCreatorProcess();
    G4String creatorProcess = creatorProcessPtr ? creatorProcessPtr -> GetProcessName() : "primary";
    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();

    // G4int trackID = track -> GetTrackID();
    // if (fGeneratedParticles.find(trackID) == fGeneratedParticles.end()){
    //     fGeneratedParticles.insert(trackID);
    // }

    if(aStep -> GetTrack() -> GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
        count += 1;
        photonsEnergy = aStep -> GetTrack() -> GetKineticEnergy();
        analysisManager -> FillH1(1, photonsEnergy / eV);
    }

    if (creatorProcess == "Cerenkov"){
        cerenkovCount += 1;
        analysisManager -> FillH1(3, aStep -> GetTrack() -> GetKineticEnergy() / eV);
    }

    // G4double energyDeposited = aStep -> GetTotalEnergyDeposit();
    // if (energyDeposited > 0){
    //     fTotalEnergyDeposited += energyDeposited;
    // }

    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *){
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager -> FillH1(0, count);
    analysisManager -> FillH1(2, cerenkovCount);
}
