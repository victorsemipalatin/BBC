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

    G4Box *solidWorld = new G4Box("SolidWorld", .5 * m, .5 * m, .5 * m);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // *************** НАСТРОЙКА МАТЕРИАЛОВ *************** //

    // СВИНЕЦ
    auto lead = nist -> FindOrBuildMaterial("G4_Pb");  

    // СЦИНТИЛЛЯТОР
    auto polystyrene = nist -> FindOrBuildMaterial("G4_POLYSTYRENE", 1.045 * g / cm3);

    auto C = nist -> FindOrBuildElement("C");
    auto H = nist -> FindOrBuildElement("H");
    auto O = nist -> FindOrBuildElement("O");

    auto pTerphenyl = new G4Material("pTerphenyl", 1.23 * g / cm3, 2);
    pTerphenyl -> AddElement(C, 18);
    pTerphenyl -> AddElement(H, 14);

    auto POPOP = new G4Material("POPOP", 1.18 * g / cm3, 3);
    POPOP -> AddElement(C, 24);
    POPOP -> AddElement(H, 18);
    POPOP -> AddElement(O, 2);

    auto scintillator = new G4Material("Scintillator", 1.05 * g / cm3, 3);
    scintillator -> AddMaterial(polystyrene, 0.98);
    scintillator -> AddMaterial(pTerphenyl,  0.015);
    scintillator -> AddMaterial(POPOP, 0.005);

    auto mptScintillator = new G4MaterialPropertiesTable();

    const G4int nEntries = 60;
	G4double photonEnergy[nEntries] = {2.3, 2.31525, 2.33051, 2.34576, 2.36102, 2.37627, 2.39153, 2.40678, 2.42203, 2.43729, 2.45254, 2.4678, 2.48305, 2.49831, 2.51356,
		 2.52881, 2.54407, 2.55932, 2.57458, 2.58983, 2.60508, 2.62034, 2.63559, 2.65085, 2.6661, 2.68136, 2.69661, 2.71186, 2.72712, 2.74237,
		 2.75763, 2.77288, 2.78814, 2.80339, 2.81864, 2.8339, 2.84915, 2.86441, 2.87966, 2.89492, 2.91017, 2.92542, 2.94068, 2.95593, 2.97119,
		 2.98644, 3.00169, 3.01695, 3.0322, 3.04746, 3.06271, 3.07797, 3.09322, 3.10847, 3.12373, 3.13898, 3.15424, 3.16949, 3.18475, 3.2};
	G4double refractiveScin[nEntries];
	G4double absLengthScin[nEntries];
	G4double photFraction[nEntries] = {0, 0, 0.04304, 0.09311, 0.14318, 0.19325, 0.24331, 0.29338, 0.34345, 0.39352, 0.44359, 0.49365, 0.54372, 0.59379, 0.65703,
		 0.72516, 0.7829, 0.85487, 0.93619, 1.0156, 1.10002, 1.19322, 1.29936, 1.41172, 1.53233, 1.65876, 1.79893, 1.98186, 2.18771, 2.4366,
		 2.78324, 3.0698, 3.27276, 3.39218, 3.46918, 3.4941, 3.52619, 3.60856, 3.88683, 4.28688, 4.71702, 4.93565, 4.80817, 4.56821, 4.23367,
		 3.56117, 2.30136, 1.47323, 1.10353, 0.84005, 0.61903, 0.46259, 0.35545, 0.2483, 0.14115, 0.034, 0, 0, 0, 0};

	for (int i = 0; i < nEntries; i++){
		refractiveScin[i] = 1.63;
		absLengthScin[i] = 1. * m;
		photonEnergy[i] = photonEnergy[i] * eV;
	}

    mptScintillator -> AddProperty("RINDEX", photonEnergy, refractiveScin, nEntries);
    mptScintillator -> AddProperty("ABSLENGTH", photonEnergy, absLengthScin, nEntries);
	mptScintillator -> AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, photFraction, nEntries);
	mptScintillator -> AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, photFraction, nEntries);
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD", 12000. / MeV); // световыход на единицу энергопотерь
    mptScintillator -> AddConstProperty("RESOLUTIONSCALE", 1.0); // доля энергетического спектра, участвующего в генерации
    mptScintillator -> AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.4 * ns); // Время высвечивания быстрой компоненты сцинтилляционной вспышки
    mptScintillator -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 5. * ns); // Время высвечивания медленной компоненты сцинтилляционной вспышки
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD1", 1.); // доля быстрой компоненты
    mptScintillator -> AddConstProperty("SCINTILLATIONYIELD2", 0.); // доля медленной компоненты
    G4double reflectionIndexPolystyrene[2] = {1., 1.};  // разные коэффициенты для разных энергий (действительно)
    mptScintillator -> AddProperty("REFLECTIVITY", energy, reflectionIndexPolystyrene, 2);
    scintillator -> GetIonisation() -> SetBirksConstant(0.126 * mm / MeV); // Примерное значение для полистирола
    scintillator -> SetMaterialPropertiesTable(mptScintillator);

    G4MaterialPropertiesTable* mirrorSurfaceMPT = new G4MaterialPropertiesTable();
    G4double reflectivity[2] = {.9, .9};
    G4double efficiency[2] = {0.0, 0.0}; // отсутствие собственного излучения
    mirrorSurfaceMPT -> AddProperty("REFLECTIVITY", energy, reflectivity, 2);
    mirrorSurfaceMPT -> AddProperty("EFFICIENCY", energy, efficiency, 2);

    G4OpticalSurface* opOutSurface = new G4OpticalSurface("scintOutSurface");
    opOutSurface -> SetType(dielectric_metal);
    opOutSurface -> SetFinish(polished);
    opOutSurface -> SetModel(unified);
    opOutSurface -> SetMaterialPropertiesTable(mirrorSurfaceMPT);

    G4OpticalSurface* opInSurface = new G4OpticalSurface("scintInSurface");
    opInSurface -> SetType(dielectric_dielectric);
    opInSurface -> SetFinish(polished);
    opInSurface -> SetModel(unified);
    opInSurface -> SetMaterialPropertiesTable(mirrorSurfaceMPT);

    G4MaterialPropertiesTable* blackSurfaceMPT = new G4MaterialPropertiesTable();
    G4double blackReflectivity[2] = {1.0, 1.0};
    blackSurfaceMPT -> AddProperty("REFLECTIVITY", energy, blackReflectivity, 2);

    G4OpticalSurface* blackSurface = new G4OpticalSurface("blackSurface");
    blackSurface -> SetType(dielectric_metal);
    blackSurface -> SetFinish(polished);
    blackSurface -> SetModel(unified);
    blackSurface -> SetMaterialPropertiesTable(blackSurfaceMPT);

    // ОПТОВОЛОКНО

    G4double a = 1.01 * g / mole;
	G4Element* elH  = new G4Element("Hydrogen", "H", 1., a);
	a = 12.01 * g / mole;
	G4Element* elC  = new G4Element("Carbon", "C", 6., a);
	a = 16.00 * g / mole;
	G4Element* elO  = new G4Element("Oxygen", "O", 8., a);

    //Cladding
	G4Material* PMMA = new G4Material("MPMMA", 1.19 * g / cm3, 3);
	PMMA -> AddElement(elC, 5);
	PMMA -> AddElement(elH, 8);
	PMMA -> AddElement(elO, 2);

	// Core
	G4Material* PS = new G4Material("MPS", 1.05 * g / cm3, 2);
	PS -> AddElement(elC, 8);
	PS -> AddElement(elH, 8);

    G4double EnergyOpt[10] = {1.9 * eV, 2.2 * eV, 2.3 * eV, 2.4 * eV, 2.56 * eV, 2.66 * eV, 2.68 * eV, 3.69 * eV, 3.7 * eV, 4.0 * eV };
	G4double SpIzlOpt[10] = {0.001, 0.05, 0.25, 0.7, 1., 1., 0., 0., 0., 0.};
	G4double AbsLenOpt[10] = {5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 0.1 * mm, 0.1 * mm, 5.0 * m, 5.0 * m };
	G4double RindexOptCore[10] = {1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59 };
	G4double RindexOptCov[10] = {1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49 };

    G4MaterialPropertiesTable* optCladdingProperties = new G4MaterialPropertiesTable();
	optCladdingProperties -> AddProperty("RINDEX", EnergyOpt, RindexOptCov, 10);
	optCladdingProperties -> AddProperty("WLSABSLENGTH", EnergyOpt, AbsLenOpt, 10);
	optCladdingProperties -> AddProperty("WLSCOMPONENT", EnergyOpt, SpIzlOpt, 10);
	optCladdingProperties -> AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
	PMMA -> SetMaterialPropertiesTable(optCladdingProperties);

    G4MaterialPropertiesTable* optCoreProperties = new G4MaterialPropertiesTable();
	optCoreProperties -> AddProperty("RINDEX", EnergyOpt, RindexOptCore, 10);
	optCoreProperties -> AddProperty("WLSABSLENGTH", EnergyOpt, AbsLenOpt, 10);
	optCoreProperties -> AddProperty("WLSCOMPONENT", EnergyOpt, SpIzlOpt, 10);
	optCoreProperties -> AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
	PS -> SetMaterialPropertiesTable(optCoreProperties);

    // **************************************** //

    // ********* НАСТРОЙКА ОТРИСОВКИ ********* //    
    auto tileAttr = new G4VisAttributes(G4Color(192, 192, 192, 1.)); // серый цвет
    tileAttr -> SetForceSolid(true);

    auto shifterCore = new G4VisAttributes(G4Color(0, 255, 0, 1.));
    shifterCore -> SetForceSolid(true);

    auto shifterCladding = new G4VisAttributes(G4Color(75, 245, 66, .5));
    shifterCladding -> SetForceSolid(true);

    auto plateAttr = new G4VisAttributes(G4Color(171, 176, 171, 1));
    plateAttr -> SetForceSolid(true);

    auto sipmAttr = new G4VisAttributes(G4Color(255, 0, 0, 1));
    sipmAttr -> SetForceSolid(true);
    // **************************************** //

    // *************** CADMesh *************** //

    // auto meshTile1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_1.stl"); // abs path
    // auto logicTile1 = new G4LogicalVolume(meshTile1 -> GetSolid(), scintillator, "logicTile1");
    // logicTile1 -> SetVisAttributes(tileAttr);

    // logicDetector = new G4LogicalVolume(logicTile1 -> GetSolid(), worldMat, "logicDetector");

    // G4int shift = 1;
    // G4int sectors_num = 1;
    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile1, "physTile1", logicWorld, false, i, checkOverlaps);
    //     new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector, "physDetector", logicWorld, false, i, checkOverlaps);
    //     new G4LogicalBorderSurface("scintInSurface", physTile1, physWorld, opInSurface);
    //     new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile1, opOutSurface);
    // }

    // // auto meshTile2 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_2.stl"); // abs path
    // // auto logicTile2 = new G4LogicalVolume(meshTile2 -> GetSolid(), scintillator, "logicTile2");
    // // for (G4int i = 0; i < 16; i++){
    // //     auto rotm = new G4RotationMatrix();
    // //     rotm -> rotateZ((22.5 * i) * deg);
    // //     auto PhysTile2 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile2, "physTile2", logicWorld, false, i, checkOverlaps);
    // // }

    // auto meshShifter1 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/shifter_1.stl"); // abs path
    // auto logicShifter1 = new G4LogicalVolume(meshShifter1 -> GetSolid(), scintillator, "logicShifter1");
    // // auto logicDetector = new G4LogicalVolume(meshShifter1 -> GetSolid(), scintillator, "logicShifter1");
    // // logicDetector = new G4LogicalVolume(logicShifter1 -> GetSolid(), scintillator, "logicDetector");

    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     // auto physShifter1 = new G4PVPlacement(rotm, G4ThreeVector(0., 84.35002, 3.), logicDetector, "physShifter1", logicWorld, false, i, checkOverlaps);
    //     // auto physShifter1 = new G4PVPlacement(rotm, G4ThreeVector(0., 1., 0.), logicDetector, "physShifter1", logicWorld, false, i, checkOverlaps);
    //     auto physShifter1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector, "physShifter1", logicWorld, false, i, checkOverlaps);
    //     // new G4LogicalBorderSurface("shifterSurface", physWorld, physShifter1, blackSurface);
    // }

    // auto meshTile3 = CADMesh::TessellatedMesh::FromOBJ("/home/e/BBC/models/tile_3.stl"); // abs path
    // auto logicTile3 = new G4LogicalVolume(meshTile3 -> GetSolid(), scintillator, "logicTile3");
    // for (G4int i = 0; i < 16; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto PhysTile3 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile3, "physTile3", logicWorld, false, i, checkOverlaps);
    // }

    // **************************************** //

    // *********** ПРИМИТИВЫ GEANT4 *********** //

    // **************** Тайл 0 **************** //
    G4int shift = 1;
    G4int sectors_num = 1;
    G4Trd *prismSolid = new G4Trd("Tile1", 17.9 / 2 * mm, 39.66 / 2 * mm, 10. / 2 * mm, 10. / 2 * mm, 54.7 / 2 * mm);

    auto cylinder = new G4Tubs("Сylinder", 0., 1.25 * mm, 11. / 2 * mm, 0. * deg, 360. * deg);

    auto rotationMatrix = new G4RotationMatrix();
    rotationMatrix -> rotateX(90. * deg);
    auto subtraction1_0 = new G4SubtractionSolid("Subtraction1", prismSolid, cylinder, rotationMatrix, G4ThreeVector(5.67 * mm, 0., -23.35 * mm));
    auto subtraction2_0 = new G4SubtractionSolid("Subtraction2", subtraction1_0, cylinder, rotationMatrix, G4ThreeVector(-5.67 * mm, 0., -23.35 * mm));
    auto subtraction3_0 = new G4SubtractionSolid("Subtraction3", subtraction2_0, cylinder, rotationMatrix, G4ThreeVector(14.89 * mm, 0., 23.35 * mm));
    auto subtraction4_0 = new G4SubtractionSolid("Subtraction4", subtraction3_0, cylinder, rotationMatrix, G4ThreeVector(-14.89 * mm, 0., 23.35 * mm));

    auto tube_0 = new G4Tubs("Tube", 12.75 * mm, 14.35 * mm, 7. / 2 * mm, 0 * deg, 360 * deg);
    auto subtraction5_0 = new G4SubtractionSolid("Subtraction5", subtraction4_0, tube_0, rotationMatrix, G4ThreeVector(0., 3. * mm, 11. * mm));

    std::vector<G4TwoVector> polygon_0;
    G4double delta = 2.7;
    polygon_0.push_back(G4TwoVector(0., 0.));
    polygon_0.push_back(G4TwoVector(3.2 * mm, 0.));
    polygon_0.push_back(G4TwoVector((6.2 + delta * 1.455) * mm, (2.08 + delta) * mm));
    polygon_0.push_back(G4TwoVector((3.0 + delta * 1.455) * mm, (2.08 + delta) * mm));
    G4double halfZ =  7 / 2 * mm;
    auto prism_0 = new G4ExtrudedSolid("Prism", polygon_0, halfZ, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);

    auto rotation_0 = new G4RotationMatrix();
    rotation_0 -> rotateX(90 * deg);
    rotation_0 -> rotateY(-180 * deg);
    
    auto solidTile0 = new G4SubtractionSolid("SolidTile0", subtraction5_0, prism_0, rotation_0, G4ThreeVector(2.1 * mm, 3. * mm, 27.5 * mm));

    auto logicTile0 = new G4LogicalVolume(solidTile0, scintillator, "logicTile1");

    logicTile0 -> SetVisAttributes(tileAttr);

    G4double r = 73.35;
    for (G4int i = 0; i < sectors_num; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateX(90 * deg);
        rotm -> rotateZ((22.5 * i) * deg);
        auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r +  shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile0, "physTile1", logicWorld, false, i, checkOverlaps);
        new G4LogicalBorderSurface("scintInSurface", physTile1, physWorld, opInSurface);
        new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile1, opOutSurface);
    }

    G4double core_f = .98;
    G4double cladding_f = .02;
    auto solidRingCore0 = new G4Tubs("SolidRingCore0", core_f * 13. * mm, core_f * 14. * mm, core_f * 3. / 2 * mm, 0 * deg, 360 * deg); 
    auto solidOutletCore0 = new G4Tubs("SolidOutletCore0", 0 * mm, core_f * .5 * mm, 100. / 2 * mm, 0 * deg, 360 * deg);

    auto solidRingAll0 = new G4Tubs("SolidRingCore0", core_f * 13 - cladding_f * 1. * mm, core_f * 14. * mm + cladding_f * 1. * mm, 3. / 2 * mm, 0 * deg, 360 * deg); 
    auto solidOutletCladding0 = new G4Tubs("SolidOutletCore0", 0, core_f * .5 * mm + cladding_f * 1. * mm, 100. / 2 * mm, 0 * deg, 360 * deg);

    delete rotationMatrix;
    rotationMatrix = nullptr;
    rotationMatrix = new G4RotationMatrix();
    rotationMatrix -> rotateX(90. * deg);
    rotationMatrix -> rotateZ(-55. * deg);

    auto solidCore0 = new G4UnionSolid("SolidCore0", solidRingCore0, solidOutletCore0, G4Transform3D(*rotationMatrix, G4ThreeVector(35.5 * mm, 40.6 * mm, 0)));
    auto solidCladdingAll0 = new G4UnionSolid("SolidCladdingAll0", solidRingAll0, solidOutletCladding0, G4Transform3D(*rotationMatrix, G4ThreeVector(35.5 * mm, 40.6 * mm, 0)));
    auto solidCladding0 = new G4SubtractionSolid("SolidRingCladding0", solidCladdingAll0, solidCore0, G4Transform3D(G4RotationMatrix(), G4ThreeVector()));

    auto logicCore0 = new G4LogicalVolume(solidCore0, PS, "logicCore0");
    auto logicCladding0 = new G4LogicalVolume(solidCladding0, PMMA, "logicCladding0");

    logicCore0 -> SetVisAttributes(shifterCore);
    logicCladding0 -> SetVisAttributes(shifterCladding);

    for (G4int i = 0; i < sectors_num; i++){
        auto rotm = new G4RotationMatrix();
        rotm -> rotateZ((22.5 * i) * deg);
        auto physCore1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r +  11 + shift * cos(22.5 * PI / 180 * i) * mm, -2.3), logicCore0, "physCore1", logicWorld, false, i, checkOverlaps);
        auto physCludding1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r + 11 + shift * cos(22.5 * PI / 180 * i) * mm, -2.3), logicCladding0, "physCladding1", logicWorld, false, i, checkOverlaps);
        // new G4LogicalBorderSurface("shifterSurface", physWorld, physShifter1, blackSurface);
    }
    // **************************************** //

    // ********** СВИНЦОВАЯ ПЛАСТИНА ********* //
    auto solidPlateF = new G4Box("SolidPlateF", .5 * m, 5. / 2 * mm, 0.99 / 2 * m);
    auto solidPlate = new G4SubtractionSolid("SolidPlate", solidPlateF, solidCladding0, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0, -50, 0)));
    auto logicPlate = new G4LogicalVolume(solidPlate, lead, "logicPlate");
    logicPlate -> SetVisAttributes(plateAttr);
    auto physPlate = new G4PVPlacement(0, G4ThreeVector(0., r + 11 + shift * mm + 50, -2.3), logicPlate, "physPlate", logicWorld, false, 0, checkOverlaps);
    // **************************************** //

    // **************** Тайл 5 **************** //
    // G4int shift = 1;
    // G4int sectors_num = 1;
    // std::vector<G4TwoVector> polygon1;
    // polygon1.push_back(G4TwoVector(0., 0.));
    // polygon1.push_back(G4TwoVector(53.19 * mm, 0.));
    // polygon1.push_back(G4TwoVector(64.26 * mm, 55.61 * mm));
    // polygon1.push_back(G4TwoVector(0., 55.61 * mm));
    // G4double halfZ1 =  10 / 2 * mm;
    // auto prism1 = new G4ExtrudedSolid("Prism1", polygon1, halfZ1, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);
    
    // auto cylinder = new G4Tubs("Сylinder", 0., 1.25 * mm, 11. / 2 * mm, 0. * deg, 360. * deg);
    // auto subtraction1 = new G4SubtractionSolid("Subtraction1", prism1, cylinder, nullptr, G4ThreeVector(4.2 * mm, 4.2 * mm, 0.));
    // auto subtraction2 = new G4SubtractionSolid("Subtraction2", subtraction1, cylinder, nullptr, G4ThreeVector(49.95 * mm, 4.2 * mm, 0.));
    // auto subtraction3 = new G4SubtractionSolid("Subtraction3", subtraction2, cylinder, nullptr, G4ThreeVector(4.2 * mm, 51.41 * mm, 0.));
    // auto subtraction4 = new G4SubtractionSolid("Subtraction4", subtraction3, cylinder, nullptr, G4ThreeVector(59.34 * mm, 51.41 * mm, 0.));

    // auto arc1 = new G4Tubs("Сylinder", 22.51 * mm, 24.11 * mm, 7 / 2 * mm, 90. * deg, 180. * deg);
    // auto subtraction5 = new G4SubtractionSolid("Subtraction5", subtraction4, arc1, nullptr, G4ThreeVector(25.81 * mm, 27.81 * mm, -3. * mm));

    // auto arc2 = new G4Tubs("Сylinder", 22.51 * mm, 24.11 * mm, 7 / 2 * mm, -90. * deg, 180. * deg);
    // auto subtraction6 = new G4SubtractionSolid("Subtraction6", subtraction5, arc2, nullptr, G4ThreeVector(30.07 * mm, 27.81 * mm, -3. * mm));

    // auto box = new G4Box("Box", 4.28 / 2 * mm, 1.6 / 2 * mm, 7 / 2 * mm); // 4.26
    // auto subtraction7 = new G4SubtractionSolid("Subtraction7", subtraction6, box, nullptr, G4ThreeVector(27.94 * mm, 4.5 * mm, -3. * mm));
    // auto subtraction8 = new G4SubtractionSolid("Subtraction8", subtraction7, box, nullptr, G4ThreeVector(27.94 * mm, 51.11 * mm, -3. * mm));

    // auto logicTile5 = new G4LogicalVolume(subtraction8, scintillator, "logicTile1");

    // // TODO: отверстие для вывода оптоволокна из тайла

    // // Shifter
    // auto sum1 = new G4UnionSolid("sum1", arc1, box, nullptr, G4ThreeVector(2.13 * mm, 23.31 * mm, 0.));
    // auto sum2 = new G4UnionSolid("sum2", sum1, box, nullptr, G4ThreeVector(2.13 * mm, -23.31 * mm, 0.));
    // auto sum3 = new G4UnionSolid("sum3", sum2, arc2, nullptr, G4ThreeVector(2 * 2.13 * mm, 0., 0.));

    // auto logicShifter5 = new G4LogicalVolume(sum3, worldMat, "logicShifter5");

    // logicDetector5 = logicTile5;

    // logicTile5 -> SetVisAttributes(tileAttr);
    // // logicDetector5 -> SetVisAttributes(shifterAttr);

    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateY(180. * deg);
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     auto physTile5 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, shift * cos(22.5 * PI / 180 * i) * mm, 0), logicDetector5, "physTile5", logicWorld, false, i, checkOverlaps);
    //     // auto physShifter5 = new G4PVPlacement(rotm, G4ThreeVector(-25.81 + shift * sin(22.5  * PI / 180 * i) * mm, 27.81 + shift * cos(22.5 * PI / 180 * i) * mm, 2. * mm), logicDetector5, "physShifter5", logicWorld, false, i, checkOverlaps);
    //     new G4LogicalBorderSurface("scintInSurface", physTile5, physWorld, opInSurface);
    //     new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile5, opOutSurface);
    // }

    // **************************************** //

    // **************** SiPM **************** //
    auto solidDetector1 = new G4Box("SolidDetector1", 1. / 2 * mm, 1. / 2 * mm, .5 / 2 * mm);
    auto logicSipm = new G4LogicalVolume(solidDetector1, worldMat, "logicDetector1");
    logicSipm -> SetVisAttributes(sipmAttr);
    logicDetector1 = logicSipm;
    auto detectorMat = new G4RotationMatrix();
    detectorMat -> rotateY(90 * deg);
    detectorMat -> rotateX(-35 * deg);
    auto physDetector = new G4PVPlacement(detectorMat, G4ThreeVector(76.7 * mm, 154.8 * mm, -2.3), logicDetector1, "physDetector", logicWorld, false, 0, checkOverlaps);
    // **************************************** //

    delete mptWorld, mptScintillator;
    mptWorld = nullptr;
    mptScintillator = nullptr;
    delete pTerphenyl, POPOP,scintillator;
    pTerphenyl = nullptr;
    POPOP = nullptr;
    scintillator = nullptr;
    delete mirrorSurfaceMPT, opOutSurface, opInSurface, blackSurface, blackSurfaceMPT;
    mirrorSurfaceMPT = nullptr;
    opOutSurface = nullptr;
    opInSurface = nullptr;
    blackSurface = nullptr;
    blackSurfaceMPT = nullptr;
    delete elH, elC, elO, PMMA, PS;
    elH = nullptr;
    elC = nullptr;
    elO = nullptr;
    PMMA = nullptr;
    PS = nullptr;
    delete optCoreProperties, optCladdingProperties;
    optCoreProperties = nullptr;
    optCladdingProperties = nullptr;
    delete tileAttr, shifterCore, shifterCladding, plateAttr, sipmAttr;
    tileAttr = nullptr;
    shifterCore = nullptr;
    shifterCladding = nullptr;
    plateAttr = nullptr;
    sipmAttr = nullptr;
    delete rotationMatrix, rotation_0;
    rotationMatrix = nullptr;
    rotation_0 = nullptr;
    
    return physWorld;
}


void BBCConstruction::ConstructSDandField(){
    auto sensDet1 = new SensitiveDetector("SensitiveDetector1");
    logicDetector1 -> SetSensitiveDetector(sensDet1);
    G4SDManager::GetSDMpointer() -> AddNewDetector(sensDet1);

    // auto sensDet5 = new SensitiveDetector("SensitiveDetector5");
    // logicDetector5 -> SetSensitiveDetector(sensDet5);
    // G4SDManager::GetSDMpointer() -> AddNewDetector(sensDet5);
}
