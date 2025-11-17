#include "GeneratorPythia.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ios.hh"

// УКАЖИТЕ путь к вашему HepMC3 файлу:
static const char* kHepMCFile = "/home/e/Downloads/100k_27GeV_minimumBias.hepmc3";

// Глобальное состояние для чтения файла (однократно на процесс)
namespace {
  std::unique_ptr<HepMC3::ReaderAscii> g_reader;
  bool g_eof = false;

  // Конвертируем единицы импульса HepMC3 -> Geant4
  inline G4double MomentumUnit(const HepMC3::GenEvent& evt) {
    using U = HepMC3::Units;
    switch (evt.momentum_unit()) {
      case U::MEV: return MeV;
      case U::GEV: return GeV;
      // В вашей сборке могут отсутствовать KEV/TEV — не используем их
      default:     return GeV; // дефолт
    }
  }

  // Конвертируем единицы длины HepMC3 -> Geant4
  inline G4double LengthUnit(const HepMC3::GenEvent& evt) {
    using U = HepMC3::Units;
    switch (evt.length_unit()) {
      case U::MM: return mm;
      case U::CM: return cm;
      // В вашей сборке может не быть Units::M — не используем
      default:    return mm; // дефолт
    }
  }
}

PrimaryGenerator::PrimaryGenerator() {
  if (!g_reader) {
    g_reader = std::make_unique<HepMC3::ReaderAscii>(std::string(kHepMCFile));
    if (g_reader->failed()) {
      G4Exception("PrimaryGenerator", "PG001", FatalException,
                  ("Cannot open HepMC3 file: " + std::string(kHepMCFile)).c_str());
    }
    G4cout << "Opened HepMC3 file: " << kHepMCFile << G4endl;
  }

  q = 0;
}

PrimaryGenerator::~PrimaryGenerator() {
  G4cout << "PrimaryGenerator killed" << G4endl;
}

void PrimaryGenerator::GeneratePrimaries(G4Event* event) {
  if (g_eof) {
    G4RunManager::GetRunManager()->AbortRun(true);
    return;
  }

  HepMC3::GenEvent hepmcEvt;
  g_reader->read_event(hepmcEvt);
  if (g_reader->failed()) {
    g_eof = true;
    G4cout << "End of HepMC3 file reached. Aborting run." << G4endl;
    G4RunManager::GetRunManager()->AbortRun(true);
    return;
  }

  const G4double pUnit = MomentumUnit(hepmcEvt);
  const G4double lUnit = LengthUnit(hepmcEvt);

  G4cout << "HepMC event number: " << hepmcEvt.event_number() << G4endl;

  G4int totalPrimaries = 0;

  for (const auto& vtx : hepmcEvt.vertices()) {
    if (!vtx) continue;

    const auto& p4 = vtx->position();
    const G4double x = p4.x() * lUnit;
    const G4double y = p4.y() * lUnit;
    const G4double z = p4.z() * lUnit;
    // HepMC3 t обычно хранится как c*t в тех же единицах длины; делим на c_light -> время
    const G4double t = (p4.t() * lUnit) / c_light;

    auto g4v = new G4PrimaryVertex(x, y, z, t);

    for (const auto& p : vtx->particles_out()) {
      if (!p) continue;
      if (p->status() != 1) continue;

      const auto& m = p->momentum();
      const G4double px = m.px() * pUnit;
      const G4double py = m.py() * pUnit;
      const G4double pz = m.pz() * pUnit;

      const G4int pdg = p->pid();
      auto g4p = new G4PrimaryParticle(pdg, px, py, pz);

      g4v->SetPrimary(g4p);
      ++totalPrimaries;
    }

    if (g4v->GetNumberOfParticle() > 0) {
      event->AddPrimaryVertex(g4v);
      } else {
      delete g4v;
    }
  }

  if (totalPrimaries == 0) {
    G4cout << "Warning: HepMC event has no status==1 particles. Skipping." << G4endl;
    // Рекурсивно читаем следующее событие
    GeneratePrimaries(event);
  }
}