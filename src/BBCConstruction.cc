#include "BBCConstruction.hh"


BBCConstruction::BBCConstruction(){
}


BBCConstruction::~BBCConstruction(){
    std::cout << "BBCConstruction killed" << std::endl;
}


G4VPhysicalVolume *BBCConstruction::Construct(){
    G4NistManager *nist = G4NistManager::Instance();

    G4Material *worldMat = nist -> FindOrBuildMaterial("G4_AIR");
    G4Box *solidWorld = new G4Box("SolidWorld", 0.5 * m, 0.5 * m, 0.5 * m); // кубик объёмом 1 м3, нужно вводить половины длин
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    /*
    1 параметр - вращение: 0 -> отсутствие вращения; 2 параметр - начало координат;
    5 параметр - родительский объём: 0 -> мать; 6 параметр - применение логических операций: 0 -> их отсутствие;
    7 параметр - число копий; 8 параметр - проверка объёмов. - лучше всегда ставить тру
    */
    G4Material* polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE"); // пластиковый сцинтиллятор
    G4Trd *tile_1 = new G4Trd("Tile1", 39.8 / 2 * mm, 61.96 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.71 / 2 * mm);
    G4LogicalVolume *logicTile1 = new G4LogicalVolume(tile_1, polystyrene, "logicTile1");
    G4VPhysicalVolume *physTile1 = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.25 * m), logicTile1, "physTile1", logicWorld, false, 0, true);

    G4Trd *trapezoid_2 = new G4Trd("Trapezoid2", (61.96 + 0.4) / 2 * mm, 84.32 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.21 / 2 * mm);
    // G4LogicalVolume *logicTile21 = new G4LogicalVolume(tile_2_1, polystyrene, "logicTile21");
    // G4VPhysicalVolume *physTile21 = new G4PVPlacement(0, G4ThreeVector(0., 0., (251 + 55.71) * mm), logicTile21, "physTile21", logicWorld, false, 0, true);

    G4Box *subs_box_2 = new G4Box("SubsBox2", 1. / 2 * mm, 12.5 / 2 * m, 55.21 / 2 * m);
    G4VSolid *tile_2 = new G4SubtractionSolid("Tile2", trapezoid_2, subs_box_2, nullptr, G4ThreeVector(0., 0., 0.));
    G4LogicalVolume *logicTile2 = new G4LogicalVolume(tile_2, polystyrene, "logicTile2");
    G4VPhysicalVolume *physTile2 = new G4PVPlacement(0, G4ThreeVector(0., 0., (251 + 55.71) * mm), logicTile2, "physTile2", logicWorld, false, 0, true);

    G4Trd *trapezoid_3 = new G4Trd("Trapezoid3", (84.32 + 0.4) / 2 * mm, 106.28 / 2 * mm, 12.5 / 2 * mm, 12.5 / 2 * mm, 55.21 / 2 * mm);
    G4Box *subs_box_3 = new G4Box("SubsBox3", 1. / 2 * mm, 12.5 / 2 * m, 55.21 / 2 * m);
    G4VSolid *tile_3 = new G4SubtractionSolid("Tile3", trapezoid_3, subs_box_3, nullptr, G4ThreeVector(0., 0., 0.));
    G4LogicalVolume *logicTile3 = new G4LogicalVolume(tile_3, polystyrene, "logicTile3");
    G4VPhysicalVolume *physTile3 = new G4PVPlacement(0, G4ThreeVector(0., 0., (252 + 2 * 55.71) * mm), logicTile3, "physTile3", logicWorld, false, 0, true);

    return physWorld;
}
