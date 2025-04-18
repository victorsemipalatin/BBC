#include "BBCConstruction.hh"
#include "CADMesh.hh" // конвертер



BBCConstruction::BBCConstruction(){
}


BBCConstruction::~BBCConstruction(){
    G4cout << "BBCConstruction killed" << G4endl;
    
}


G4VPhysicalVolume *BBCConstruction::Construct(){

    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();

    G4double energy[2] = {1.239841939 * eV / 0.9, 1.239841939 * eV / 0.2};
    G4Material *worldMat = nist -> FindOrBuildMaterial("G4_Galactic");
    auto mptWorld = new G4MaterialPropertiesTable();
    G4double reflectionIndexworld[2] = {1., 1.};
    mptWorld -> AddProperty("RINDEX", energy, reflectionIndexworld, 2);
    worldMat -> SetMaterialPropertiesTable(mptWorld);

    G4Box *solidWorld = new G4Box("SolidWorld", .5 * m, .5 * m, .5 * m); // кубик объёмом 4 ^ 3 м3, нужно вводить половины длин
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    G4Material* polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE", 1.045 * g / cm3); // пластиковый сцинтиллятор
    auto mptPolystyrene = new G4MaterialPropertiesTable();
    G4double refractiveIndexPolystyrene[2] = {1.63, 1.63};
    G4double fraction[2] = {1.0, 1.0}; // нужно поменять 
    mptPolystyrene -> AddProperty("RINDEX", energy, refractiveIndexPolystyrene, 2);
    mptPolystyrene -> AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD", 1200. / MeV); // световыход на единицу энергопотерь, уменьшенный в 10 раз
    mptPolystyrene -> AddConstProperty("RESOLUTIONSCALE", 1.0); // доля энергетического спектра, участвующего в генерации
    mptPolystyrene -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.4 * ns); // Время высвечивания быстрой компоненты сцинтилляционной вспышки
    mptPolystyrene -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5. * ns); // Время высвечивания медленной компоненты сцинтилляционной вспышки
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD1", .65); // доля быстрой компоненты
    mptPolystyrene -> AddConstProperty("SCINTILLATIONYIELD2", 0.); // доля медленной компоненты
    G4double reflectionIndexPolystyrene[2] = {1., 1.};  // разные коэффициенты для разных энергий
    mptPolystyrene -> AddProperty("REFLECTIVITY", energy, reflectionIndexPolystyrene, 2);
    polystyrene -> SetMaterialPropertiesTable(mptPolystyrene);

    G4MaterialPropertiesTable* surfaceMPT = new G4MaterialPropertiesTable();
    G4double reflectivity[2] = {.9, .9};
    G4double efficiency[2] = {0.0, 0.0}; // отсутствие собственного излучения
    surfaceMPT -> AddProperty("REFLECTIVITY", energy, reflectivity, 2);
    surfaceMPT -> AddProperty("EFFICIENCY", energy, efficiency, 2);

    G4OpticalSurface* opOutSurface = new G4OpticalSurface("scintSurface");
    opOutSurface -> SetType(dielectric_metal);
    opOutSurface -> SetFinish(polished);
    opOutSurface -> SetModel(unified);
    opOutSurface -> SetMaterialPropertiesTable(surfaceMPT);

    G4OpticalSurface* opInSurface = new G4OpticalSurface("scintSurface");
    opInSurface -> SetType(dielectric_dielectric);
    opInSurface -> SetFinish(polished);
    opInSurface -> SetModel(unified);
    opInSurface -> SetMaterialPropertiesTable(surfaceMPT);

    auto meshTile1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_1.stl"); // abs path
    auto logicTile1 = new G4LogicalVolume(meshTile1 -> GetSolid(), polystyrene, "logicTile1");

    auto tile1Attr = new G4VisAttributes(G4Color(192, 192, 192, 0.5)); // серый цвет
    tile1Attr -> SetForceSolid(true);
    logicTile1 -> SetVisAttributes(tile1Attr);

    G4int shift = 1;
    G4int sectors_num = 2;

    for (G4int i = 0; i < sectors_num; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile1, "physTile1", logicWorld, false, i, checkOverlaps);
        auto logicalBorderInSurface = new G4LogicalBorderSurface("scintInSurface", physTile1, physWorld, opInSurface);
        auto logicalBorderOutSurface = new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile1, opOutSurface);
        logicDetector = new G4LogicalVolume(meshTile1 -> GetSolid(), polystyrene, "logicDetector");
        auto physDetector = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector, "physDetector", logicWorld, false, i, checkOverlaps);
    }

    // auto meshTile2 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_2.stl"); // abs path
    // auto logicTile2 = new G4LogicalVolume(meshTile2 -> GetSolid(), polystyrene, "logicTile2");
    // for (G4int i = 0; i < 16; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile2 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile2, "physTile2", logicWorld, false, i, checkOverlaps);
    // }

    // auto meshShifter1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/shifter_1.stl"); // abs path
    // auto logicShifter1 = new G4LogicalVolume(meshShifter1 -> GetSolid(), polystyrene, "logicShifter1");
    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile2 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicShifter1, "physShifter1", logicWorld, false, i, checkOverlaps);
    // }

    // auto meshTile3 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_3.stl"); // abs path
    // auto logicTile3 = new G4LogicalVolume(meshTile3 -> GetSolid(), polystyrene, "logicTile3");
    // for (G4int i = 0; i < 16; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile3 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile3, "physTile3", logicWorld, false, i, checkOverlaps);
    // }
                                
    return physWorld;
}



void BBCConstruction::ConstructSDandField(){
    auto sensDet = new SensitiveDetector("SensitiveDetector");
    logicDetector -> SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer() -> AddNewDetector(sensDet);
}