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
    count = 0.;
    photonsEnergy = 0.;
    cerenkovCount = 0.;
}


G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist){
    auto analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *preStepPoint = aStep -> GetPreStepPoint();
    if(aStep -> GetTrack() -> GetParticleDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
        count += 1.;
        photonsEnergy = aStep -> GetTrack() -> GetKineticEnergy();
        // G4cout << photonsEnergy << G4endl;
        analysisManager -> FillH1(1, photonsEnergy / eV);
    }

    const G4Track* track = aStep -> GetTrack();
    auto creatorProcess = track -> GetCreatorProcess() -> GetProcessName();
    // std::ofstream out("hello.txt", std::ios::app);
    // if (out.is_open()){
    //     if (creatorProcess != "Scintillation" && creatorProcess != "Cerenkov")
    //         out << creatorProcess << std::endl;
    // }
    // out.close();   

    if (creatorProcess == "Cerenkov"){
        cerenkovCount += 1.;
    }
    // else if (creatorProcess != "Scintillation"){
    //     // G4cout << "process: "<< creatorProcess << G4endl;
    // }

    G4double energyDeposited = aStep -> GetTotalEnergyDeposit();

    if (energyDeposited > 0){
        fTotalEnergyDeposited += energyDeposited;
    }

    return true;
}


void SensitiveDetector::EndOfEvent(G4HCofThisEvent *){
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager -> FillH1(0, count);
    analysisManager -> FillH1(2, cerenkovCount);
}
