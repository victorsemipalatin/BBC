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

    G4Material* polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE", 1.045 * g / cm3);

    G4Element* C = nist -> FindOrBuildElement("C");
    G4Element* H = nist -> FindOrBuildElement("H");
    G4Element* O = nist -> FindOrBuildElement("O");

    G4Material* pTerphenyl = new G4Material("pTerphenyl", 1.23 * g / cm3, 2);
    pTerphenyl -> AddElement(C, 18);
    pTerphenyl -> AddElement(H, 14);

    G4Material* POPOP = new G4Material("POPOP", 1.18 * g/cm3, 3);
    POPOP -> AddElement(C, 24);
    POPOP -> AddElement(H, 18);
    POPOP -> AddElement(O, 2);

    G4Material* scintillator = new G4Material("Scintillator", 1.05 * g/cm3, 3);
    scintillator -> AddMaterial(polystyrene, 0.98);
    scintillator -> AddMaterial(pTerphenyl,  0.015);
    scintillator -> AddMaterial(POPOP, 0.005);

    auto mptScintillator = new G4MaterialPropertiesTable();
    G4double refractiveIndexScintillator[2] = {1.63, 1.63};
    G4double fraction[2] = {1.0, 1.0}; // нужно поменять 
    mptScintillator -> AddProperty("RINDEX", energy, refractiveIndexScintillator, 2);
    mptScintillator -> AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV); // световыход на единицу энергопотерь
    mptScintillator -> AddConstProperty("RESOLUTIONSCALE", 1.0); // доля энергетического спектра, участвующего в генерации
    mptScintillator -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.4 * ns); // Время высвечивания быстрой компоненты сцинтилляционной вспышки
    mptScintillator -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5. * ns); // Время высвечивания медленной компоненты сцинтилляционной вспышки
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD1", 1.); // доля быстрой компоненты
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD2", 0.); // доля медленной компоненты
    G4double reflectionIndexPolystyrene[2] = {1., 1.};  // разные коэффициенты для разных энергий
    mptScintillator -> AddProperty("REFLECTIVITY", energy, reflectionIndexPolystyrene, 2);

    // G4double WLS_Energy[5] = {2.70 * eV, 2.82 * eV, 2.95 * eV, 3.10 * eV, 3.26 * eV};
    // G4double WLS_AbsLength[5] = {200. * mm, 100. * mm, 20.0 * mm, 3.0 * mm, 0.5 * mm};
    // G4double WLS_Emission[5] = {0.10, 0.30, 0.80, 1.00, 0.10};
    // mptScintillator -> AddProperty("WLSABSLENGTH",   WLS_Energy, WLS_AbsLength, 5);
    // mptScintillator -> AddProperty("WLSCOMPONENT",   WLS_Energy, WLS_Emission,  5);
    // mptScintillator -> AddConstProperty("WLSTIMECONSTANT", 1.5*ns);

    scintillator -> GetIonisation() -> SetBirksConstant(0.126 * mm/MeV); // Примерное значение для полистирола
    scintillator -> SetMaterialPropertiesTable(mptScintillator);

    G4MaterialPropertiesTable* mirrorSurfaceMPT = new G4MaterialPropertiesTable();
    G4double reflectivity[2] = {.9, .9};
    G4double efficiency[2] = {0.0, 0.0}; // отсутствие собственного излучения
    mirrorSurfaceMPT -> AddProperty("REFLECTIVITY", energy, reflectivity, 2);
    mirrorSurfaceMPT -> AddProperty("EFFICIENCY", energy, efficiency, 2);

    G4OpticalSurface* opOutSurface = new G4OpticalSurface("scintSurface");
    opOutSurface -> SetType(dielectric_metal);
    opOutSurface -> SetFinish(polished);
    opOutSurface -> SetModel(unified);
    opOutSurface -> SetMaterialPropertiesTable(mirrorSurfaceMPT);

    G4OpticalSurface* opInSurface = new G4OpticalSurface("scintSurface");
    opInSurface -> SetType(dielectric_dielectric);
    opInSurface -> SetFinish(polished);
    opInSurface -> SetModel(unified);
    opInSurface -> SetMaterialPropertiesTable(mirrorSurfaceMPT);

    auto meshTile1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_1.stl"); // abs path
    auto logicTile1 = new G4LogicalVolume(meshTile1 -> GetSolid(), scintillator, "logicTile1");

    auto tile1Attr = new G4VisAttributes(G4Color(192, 192, 192, 0.5)); // серый цвет
    tile1Attr -> SetForceSolid(true);
    logicTile1 -> SetVisAttributes(tile1Attr);

    G4int shift = 1;
    G4int sectors_num = 1;
    for (G4int i = 0; i < sectors_num; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile1, "physTile1", logicWorld, false, i, checkOverlaps);
        auto logicalBorderInSurface = new G4LogicalBorderSurface("scintInSurface", physTile1, physWorld, opInSurface);
        new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile1, opOutSurface);
        // logicDetector = new G4LogicalVolume(meshTile1 -> GetSolid(), scintillator, "logicDetector");
        // auto physDetector = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector, "physDetector", logicWorld, false, i, checkOverlaps);
    }

    // auto meshTile2 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_2.stl"); // abs path
    // auto logicTile2 = new G4LogicalVolume(meshTile2 -> GetSolid(), scintillator, "logicTile2");
    // for (G4int i = 0; i < 16; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile2 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile2, "physTile2", logicWorld, false, i, checkOverlaps);
    // }

    G4MaterialPropertiesTable* blackSurfaceMPT = new G4MaterialPropertiesTable();
    G4double blackReflectivity[2] = {1.0, 1.0};
    blackSurfaceMPT -> AddProperty("REFLECTIVITY", energy, blackReflectivity, 2);

    G4OpticalSurface* blackSurface = new G4OpticalSurface("scintSurface");
    blackSurface -> SetType(dielectric_metal);
    blackSurface -> SetFinish(polished);
    blackSurface -> SetModel(unified);
    blackSurface -> SetMaterialPropertiesTable(blackSurfaceMPT);

    auto meshShifter1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/shifter_1.stl"); // abs path
    auto logicShifter1 = new G4LogicalVolume(meshShifter1 -> GetSolid(), scintillator, "logicShifter1");
    logicDetector = new G4LogicalVolume(logicShifter1 -> GetSolid(), scintillator, "logicDetector");

    auto shifter1Attr = new G4VisAttributes(G4Color(0, 255, 0, 0.5));
    shifter1Attr -> SetForceSolid(true);
    logicDetector -> SetVisAttributes(shifter1Attr);
    for (G4int i = 0; i < sectors_num; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto physShifter1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector, "physShifter1", logicWorld, false, i, checkOverlaps);
        new G4LogicalBorderSurface("shifterSurface", physWorld, physShifter1, blackSurface);
        // auto SD = new SensitiveDetector("shifterAbsorber");
        // logicShifter1 -> SetSensitiveDetector(SD);
    }

    // auto meshTile3 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_3.stl"); // abs path
    // auto logicTile3 = new G4LogicalVolume(meshTile3 -> GetSolid(), scintillator, "logicTile3");
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
