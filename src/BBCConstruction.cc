#include "BBCConstruction.hh"
#include "CADMesh.hh" // конвертер



BBCConstruction::BBCConstruction(){
}


BBCConstruction::~BBCConstruction(){
    G4cout << "BBCConstruction killed" << std::endl;
}


G4VPhysicalVolume *BBCConstruction::Construct(){
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *worldMat = nist -> FindOrBuildMaterial("G4_AIR");
    G4Box *solidWorld = new G4Box("SolidWorld", 2. * m, 2. * m, 2. * m); // кубик объёмом 4 ^ 3 м3, нужно вводить половины длин
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    /*
    1 параметр - вращение: 0 -> отсутствие вращения; 2 параметр - начало координат;
    5 параметр - родительский объём: 0 -> мать; 6 параметр - применение логических операций: 0 -> их отсутствие;
    7 параметр - число копий; 8 параметр - проверка объёмов. - лучше всегда ставить тру
    */
    // G4Material* polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE"); // пластиковый сцинтиллятор
    // G4Trd *tile_1 = new G4Trd("Tile1", 39.8 / 2 * mm, 61.96 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.71 / 2 * mm);
    // G4LogicalVolume *logicTile1 = new G4LogicalVolume(tile_1, polystyrene, "logicTile1");
    // G4VPhysicalVolume *physTile1 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25 * m), logicTile1, "physTile1", logicWorld, false, 0, true);

    // G4Trd *trapezoid_2 = new G4Trd("Trapezoid2", (61.96 + 0.4) / 2 * mm, 84.32 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.21 / 2 * mm);
    // G4LogicalVolume *logicTile21 = new G4LogicalVolume(tile_2_1, polystyrene, "logicTile21");
    // G4VPhysicalVolume *physTile21 = new G4PVPlacement(0, G4ThreeVector(0., 0., (251 + 55.71) * mm), logicTile21, "physTile21", logicWorld, false, 0, true);

    // G4Box *subs_box_2 = new G4Box("SubsBox2", 1. / 2 * mm, 12.5 / 2 * m, 55.21 / 2 * m);
    // G4VSolid *tile_2 = new G4SubtractionSolid("Tile2", trapezoid_2, subs_box_2, nullptr, G4ThreeVector(0., 0., 0.));
    // G4LogicalVolume *logicTile2 = new G4LogicalVolume(tile_2, polystyrene, "logicTile2");
    // G4VPhysicalVolume *physTile2 = new G4PVPlacement(0, G4ThreeVector(0., 0., (251 + 55.71) * mm), logicTile2, "physTile2", logicWorld, false, 0, true);

    // G4Trd *trapezoid_3 = new G4Trd("Trapezoid3", (84.32 + 0.4) / 2 * mm, 106.28 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.21 / 2 * mm);
    // G4Box *subs_box_3 = new G4Box("SubsBox3", 1. / 2 * mm, 12.5 / 2 * m, 55.21 / 2 * m);
    // G4VSolid *tile_3 = new G4SubtractionSolid("Tile3", trapezoid_3, subs_box_3, nullptr, G4ThreeVector(0., 0., 0.));
    // G4LogicalVolume *logicTile3 = new G4LogicalVolume(tile_3, polystyrene, "logicTile3");
    // G4VPhysicalVolume *physTile3 = new G4PVPlacement(0, G4ThreeVector(0., 0., (252 + 2 * 55.71) * mm), logicTile3, "physTile3", logicWorld, false, 0, true);

    G4Material* polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE", 1.045 * g / cm3); // пластиковый сцинтиллятор
    auto mptPolystyrene = new G4MaterialPropertiesTable();
    G4double energy[2] = {1.239841939 * eV / 0.9, 1.239841939 * eV / 0.2};
    G4double reflectionIndexPolystyrene[2] = {1.63, 1.63};
    G4double fraction[2] = {1.0, 1.0}; // нужно поменять 
    mptPolystyrene -> AddProperty("RINDEX", energy, reflectionIndexPolystyrene, 2);
    mptPolystyrene -> AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD", 1200. / MeV); // световыход на единицу энергопотерь, уменьшенный в 10 раз
    mptPolystyrene -> AddConstProperty("RESOLUTIONSCALE", 1.0); // доля энергетического спектра, участвующего в генерации
    mptPolystyrene -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.4 * ns); // Время высвечивания быстрой компоненты сцинтилляционной вспышки
    mptPolystyrene -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5. * ns); // Время высвечивания медленной компоненты сцинтилляционной вспышки
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD1", .65); // доля быстрой компоненты
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD2", 0.); // доля медленной компоненты
    polystyrene -> GetIonisation() -> SetBirksConstant(0.126 * mm / MeV); // постоянная Биркса
    polystyrene -> SetMaterialPropertiesTable(mptPolystyrene);

    // auto mptWorld = new G4MaterialPropertiesTable();
    // G4double reflectionIndexworld[2] = {1., 1.};
    // mptWorld -> AddProperty("RINDEX", energy, reflectionIndexworld, 2);
    // worldMat -> SetMaterialPropertiesTable(mptWorld);

    auto meshTile1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_1.stl"); // abs path
    auto logicTile1 = new G4LogicalVolume(meshTile1 -> GetSolid(), polystyrene, "logicTile1");
    G4int shift = 1;
    for (G4int i = 0; i < 16; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto PhysTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile1, "physTile1", logicWorld, false, i, true);
    }

    auto meshTile2 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_2.stl"); // abs path
    auto logicTile2 = new G4LogicalVolume(meshTile2 -> GetSolid(), polystyrene, "logicTile2");
    for (G4int i = 0; i < 16; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto PhysTile2 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile2, "physTile2", logicWorld, false, i, true);
    }

    // auto meshTile3 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_3.stl"); // abs path
    // auto logicTile3 = new G4LogicalVolume(meshTile3 -> GetSolid(), polystyrene, "logicTile3");
    // for (G4int i = 0; i < 16; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile3 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile3, "physTile3", logicWorld, false, i, true);
    // }
                                
    return physWorld;
}
