#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4int checkOverlaps = 1;
  G4NistManager *nist = G4NistManager::Instance();

  // Определяем материал(в данном случае для примера пластик: Полиэтилен)
  G4Material *plastic = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4double halfX = 2.5 * cm;
  G4double halfY = 2.5 * cm;
  G4double halfZ = 0.5 * cm;
  // создаем мир
  G4Material *worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
  G4double worldSize = 5 * m; // размер мира, который больше размера параллепипеда
  G4Box *solidWorld = new G4Box("SolidWorld", worldSize / 2, worldSize / 2, worldSize / 2);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "LogicWorld");
  G4VPhysicalVolume *physWorld = new G4PVPlacement{0, G4ThreeVector(0, 0, 0), logicWorld, "PhysWorld", nullptr, false, 0, true};

  auto tileAttr = new G4VisAttributes(G4Color(192, 192, 192, 1.)); // серый цвет
  tileAttr->SetForceSolid(true);

  // **************** Тайл 1 **************** //
  G4Trap *solidTrap1 = new G4Trap("SolidBox",
                                  0.5 * cm,      // половина длинны по оси Z
                                  0,             // угол тетта оси Z относительно глобальной оси координат
                                  0,             // угол phi оси Z относителтно глобальной оси координат
                                  5.47 / 2 * cm, // половина длинны по оси Y на нижней грани
                                  0.895 * cm,    // половина длин трапеции по оси X1 на нижней грани
                                  1.983 * cm,    // половина длинны трапеции по оси Х2 на нижней грани
                                  0,
                                  5.47 / 2 * cm,
                                  0.895 * cm,
                                  1.983 * cm,
                                  0);

  // определяем геометрию отверстия под крепление тайла
  G4Tubs *tubseForKrep = new G4Tubs("tubseForKrep", // "название",
                                    0.0 * cm,       // начальный радиус,
                                    0.125 * cm,     // конечный радиус,
                                    0.5 * 2 * cm,   // Высота/2,
                                    0 * deg,        // начальный угол,
                                    360 * deg       // угол охвата
  );
  // определяем геометрию паза для шифтера
  G4Tubs *tubseForShift = new G4Tubs("tubseForShift", // "название",
                                     (1.2745) * cm,   // начальный радиус,
                                     1.4345 * cm,     // конечный радиус,
                                     0.35 * cm,       // Высота/2,
                                     0 * deg,         // начальный угол,
                                     360 * deg        // угол охвата
  );

  // определим вектор смещения, относительно центра трапеции
  G4ThreeVector translation11(-(0.442 + 0.125) * cm, -(2.735 - 0.275 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation12((0.442 + 0.125) * cm, -(2.735 - 0.275 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation13(-(1.3675 + 0.125) * cm, (2.735 - 0.275 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation14((1.3675 + 0.125) * cm, (2.735 - 0.275 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation15(0 * cm, (1.1005) * cm, -0.3 * cm);

  // вычитаем цилиндр из трапеции
  G4SubtractionSolid *s11 = new G4SubtractionSolid("1.1", solidTrap1, tubseForKrep, nullptr, translation11);
  G4SubtractionSolid *s12 = new G4SubtractionSolid("1.2", s11, tubseForKrep, nullptr, translation12);
  G4SubtractionSolid *s13 = new G4SubtractionSolid("1.3", s12, tubseForKrep, nullptr, translation13);
  G4SubtractionSolid *s14 = new G4SubtractionSolid("1.4", s13, tubseForKrep, nullptr, translation14);
  G4SubtractionSolid *solidTile1 = new G4SubtractionSolid("solidTile1", s14, tubseForShift, nullptr, translation15);

  auto logicTile1 = new G4LogicalVolume(solidTile1, plastic, "logicTile1");
  logicTile1->SetVisAttributes(tileAttr);
  // **************************************** //

  // **************** Тайл 2 **************** //
  G4Trap *solidTrap2 = new G4Trap("SolidBox2",
                                  0.5 * cm,       // половина длинны по оси Z
                                  0,              // угол тетта оси Z относительно глобальной оси координат
                                  0,              // угол phi оси Z относителтно глобальной оси координат
                                  5.56 / 2 * cm,  // половина длинны по оси Y на нижней грани
                                  3.990 / 2 * cm, // половина длин трапеции по оси X1 на нижней грани
                                  6.202 / 2 * cm, // половина длинны трапеции по оси Х2 на нижней грани
                                  0,
                                  5.56 / 2 * cm,
                                  3.990 / 2 * cm,
                                  6.202 / 2 * cm,
                                  0);

  G4ThreeVector translation21(-(1.536 + 0.125) * cm, -(2.805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation22((1.536 + 0.125) * cm, -(2.805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation23(-(2.485 + 0.125) * cm, (2.805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation24((2.485 + 0.125) * cm, (2.805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation25(0 * cm, (0.2595) * cm, -0.3 * cm);

  G4Tubs *tubseForShift2 = new G4Tubs("tubseForShift2", // "название",
                                      (3.98 / 2) * cm,  // начальный радиус,
                                      (4.3 / 2) * cm,   // конечный радиус,
                                      0.35 * cm,        // Высота/2,
                                      0 * deg,          // начальный угол,
                                      360 * deg         // угол охвата
  );

  G4SubtractionSolid *s21 = new G4SubtractionSolid("2.1", solidTrap2, tubseForKrep, nullptr, translation21);
  G4SubtractionSolid *s22 = new G4SubtractionSolid("2.2", s21, tubseForKrep, nullptr, translation22);
  G4SubtractionSolid *s23 = new G4SubtractionSolid("2.3", s22, tubseForKrep, nullptr, translation23);
  G4SubtractionSolid *s24 = new G4SubtractionSolid("2.4", s23, tubseForKrep, nullptr, translation24);
  G4SubtractionSolid *solidTile2 = new G4SubtractionSolid("solidTile2", s24, tubseForShift2, nullptr, translation25);

  auto logicTile2 = new G4LogicalVolume(solidTile2, plastic, "logicTile2");
  logicTile2->SetVisAttributes(tileAttr);
  // **************************************** //

  // **************** Тайл 3 **************** //
  G4Trap *solidTrap3 = new G4Trap("SolidBox3",
                                  0.5 * cm,       // половина длинны по оси Z
                                  0,              // угол тетта оси Z относительно глобальной оси координат
                                  0,              // угол phi оси Z относителтно глобальной оси координат
                                  5.56 / 2 * cm,  // половина длинны по оси Y на нижней грани
                                  6.226 / 2 * cm, // половина длин трапеции по оси X1 на нижней грани
                                  8.439 / 2 * cm, // половина длинны трапеции по оси Х2 на нижней грани
                                  0,
                                  5.56 / 2 * cm,
                                  6.226 / 2 * cm,
                                  8.439 / 2 * cm,
                                  0);

  ///////////разделяем на два тайла////////////////////////////
  G4ThreeVector translation31(0 * cm, 0 * cm, 0.0 * cm);
  ///////////отверстия для крепления//////////////////////////
  G4ThreeVector translation32((0.295 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation33(-(0.295 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation34((3.573 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation35(-(3.573 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation36((0.295 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation37(-(0.295 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation38((2.634 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation39(-(2.634 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  //////////отверстия для шифтера///////////////////////////
  G4ThreeVector translation310((1.4345 + 0.33 + 0.03) * cm, 0.816 * cm, -0.3 * cm);
  G4ThreeVector translation311(-(1.4345 + 0.33 + 0.03) * cm, 0.816 * cm, -0.3 * cm);

  G4Box *solidBox2 = new G4Box("SolidBox2", 0.03 * cm, 5.561 * cm, 1 * cm);
  G4Tubs *tubseForShift3 = new G4Tubs("tubseForShift3",              // "название",
                                      (2.869 / 2) * cm,              // начальный радиус,
                                      ((2.869 + 0.16 * 2) / 2) * cm, // конечный радиус,
                                      0.35 * cm,                     // Высота/2,
                                      0 * deg,                       // начальный угол,
                                      360 * deg                      // угол охвата
  );

  G4SubtractionSolid *s31 = new G4SubtractionSolid("3.1", solidTrap3, solidBox2, nullptr, translation31);
  G4SubtractionSolid *s32 = new G4SubtractionSolid("3.2", s31, tubseForKrep, nullptr, translation32);
  G4SubtractionSolid *s33 = new G4SubtractionSolid("3.3", s32, tubseForKrep, nullptr, translation33);
  G4SubtractionSolid *s34 = new G4SubtractionSolid("3.4", s33, tubseForKrep, nullptr, translation34);
  G4SubtractionSolid *s35 = new G4SubtractionSolid("3.5", s34, tubseForKrep, nullptr, translation35);
  G4SubtractionSolid *s36 = new G4SubtractionSolid("3.6", s35, tubseForKrep, nullptr, translation36);
  G4SubtractionSolid *s37 = new G4SubtractionSolid("3.7", s36, tubseForKrep, nullptr, translation37);
  G4SubtractionSolid *s38 = new G4SubtractionSolid("3.8", s37, tubseForKrep, nullptr, translation38);
  G4SubtractionSolid *s39 = new G4SubtractionSolid("3.9", s38, tubseForKrep, nullptr, translation39);
  G4SubtractionSolid *s310 = new G4SubtractionSolid("3.10", s39, tubseForShift3, nullptr, translation310);
  G4SubtractionSolid *solidTile3 = new G4SubtractionSolid("solidTile3", s310, tubseForShift3, nullptr, translation311);

  auto logicTile3 = new G4LogicalVolume(solidTile3, plastic, "logicTile3");
  logicTile3->SetVisAttributes(tileAttr);
  // **************************************** //

  // **************** Тайл 4 **************** //
  G4Trap *solidTrap4 = new G4Trap("SolidBox4",
                                  0.5 * cm,        // половина длинны по оси Z
                                  0,               // угол тетта оси Z относительно глобальной оси координат
                                  0,               // угол phi оси Z относителтно глобальной оси координат
                                  5.56 / 2 * cm,   // половина длинны по оси Y на нижней грани
                                  8.463 / 2 * cm,  // половина длин трапеции по оси X1 на нижней грани
                                  10.675 / 2 * cm, // половина длинны трапеции по оси Х2 на нижней грани
                                  0,
                                  5.56 / 2 * cm,
                                  8.463 / 2 * cm,
                                  10.675 / 2 * cm,
                                  0);
  ///////////разделяем на два тайла////////////////////////////////////////////////////////
  G4ThreeVector translation41(0 * cm, 0 * cm, 0.0 * cm);
  ///////////отверстия для крепления///////////////////////////////////////////////////////
  G4ThreeVector translation42((0.295 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation43(-(0.295 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation44((4.691 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation45(-(4.691 + 0.03 + 0.125) * cm, (2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation46((0.295 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation47(-(0.295 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation48((3.752 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation49(-(3.752 + 0.03 + 0.125) * cm, -(2.7805 - 0.295 - 0.125) * cm, 0.0 * cm);
  //////////отверстия для шифтера////////////////////////////////////////////////////////////
  G4ThreeVector translation410((2.299) * cm, 0.312 * cm, -0.3 * cm);
  G4ThreeVector translation411((2.231) * cm, -0.379 * cm, -0.3 * cm);
  G4ThreeVector translation412(-(2.299) * cm, 0.312 * cm, -0.3 * cm);
  G4ThreeVector translation413(-(2.231) * cm, -0.379 * cm, -0.3 * cm);
  //////////выемка кастомной формы, для соедениния полуокружностей////////////////////////////
  G4ThreeVector translation414((0.068 + 0.16 - 0.097) * cm, (0.312 - 0.379 - 0.31) * cm, -0.3 * cm);
  G4ThreeVector translation415(-(0.366 + 0.16 - 0.097) * cm, (0.312 - 0.379 - 0.31) * cm, -0.3 * cm);
  G4ThreeVector translation416((0.366 + 0.16 - 0.097 + 3.74) * cm, (0.312 - 0.379 - 0.31) * cm, -0.3 * cm);
  G4ThreeVector translation417(-(0.366 + 0.16 - 0.097 + 3.9) * cm, (0.312 - 0.379 - 0.31) * cm, -0.3 * cm);

  G4Box *solidBox3 = new G4Box("SolidBox3", 0.03 * cm, 5.561 * cm, 1 * cm);

  G4Tubs *TubseForShift41 = new G4Tubs("TubseForShift41",            // "название",
                                       (3.88 / 2) * cm,              // начальный радиус,
                                       ((3.88 + 0.16 * 2) / 2) * cm, // конечный радиус,
                                       0.35 * cm,                    // Высота/2,
                                       0 * deg,                      // начальный угол,
                                       180 * deg                     // угол охвата
  );
  G4Tubs *TubseForShift42 = new G4Tubs("TubseForShift42",            // "название",
                                       (3.74 / 2) * cm,              // начальный радиус,
                                       ((3.74 + 0.16 * 2) / 2) * cm, // конечный радиус,
                                       0.35 * cm,                    // Высота/2,
                                       180 * deg,                    // начальный угол,
                                       180 * deg                     // угол охвата
  );
  ////////////////////////////////////создадим кастомные фигуры для соеденения двух полу кружностей/////////////////////////////////////////////////////////////
  std::vector<G4TwoVector> polygon_0;
  polygon_0.push_back(G4TwoVector(0.68 * mm, -0.05 * mm));
  polygon_0.push_back(G4TwoVector((1.6 + 0.68) * mm, -0.05 * mm));
  polygon_0.push_back(G4TwoVector((0.68 + 1.6) * mm, (6.91 + 0.05) * mm));
  polygon_0.push_back(G4TwoVector(0.68 * mm, (6.91 + 0.05) * mm));

  auto prism_0 = new G4ExtrudedSolid("prism1",               // название
                                     polygon_0,              // контур полигона
                                     0.35 * cm,              // половина высоты
                                     G4TwoVector(0, 0), 1.0, // параметры для нижнего основания (0.0)-  смещение, 1.0 - масштаб
                                     G4TwoVector(0, 0), 1.0  //  параметры для верхнего основания (0.0)-  смещение, 1.0 - масштаб
  );

  std::vector<G4TwoVector> polygon_1;
  polygon_1.push_back(G4TwoVector((0 - 0.675) * mm, -0.05 * mm));
  polygon_1.push_back(G4TwoVector((1.6 - 0.675) * mm, -0.05 * mm));
  polygon_1.push_back(G4TwoVector((0.68 + 1.6) * mm, (6.91 + 0.05) * mm));
  polygon_1.push_back(G4TwoVector((0.68) * mm, (6.91 + 0.05) * mm));

  auto prism_1 = new G4ExtrudedSolid("prism2",               // название
                                     polygon_1,              // контур полигона
                                     0.35 * cm,              // половина высоты
                                     G4TwoVector(0, 0), 1.0, // параметры для нижнего основания (0.0)-  смещение, 1.0 - масштаб
                                     G4TwoVector(0, 0), 1.0  //  параметры для верхнего основания (0.0)-  смещение, 1.0 - масштаб
  );

  std::vector<G4TwoVector> polygon_2;
  polygon_2.push_back(G4TwoVector((0 + 0.68) * mm, -0.05 * mm));
  polygon_2.push_back(G4TwoVector((1.6 + 0.68) * mm, -0.05 * mm));
  polygon_2.push_back(G4TwoVector((0.68 + 1.6 - 1.36) * mm, (6.91 + 0.05) * mm));
  polygon_2.push_back(G4TwoVector(-(0.68) * mm, (6.91 + 0.05) * mm));

  auto prism_2 = new G4ExtrudedSolid("prism2",               // название
                                     polygon_2,              // контур полигона
                                     0.35 * cm,              // половина высоты
                                     G4TwoVector(0, 0), 1.0, // параметры для нижнего основания (0.0)-  смещение, 1.0 - масштаб
                                     G4TwoVector(0, 0), 1.0  //  параметры для верхнего основания (0.0)-  смещение, 1.0 - масштаб
  );

  G4SubtractionSolid *s41 = new G4SubtractionSolid("4.1", solidTrap4, solidBox2, nullptr, translation41);
  G4SubtractionSolid *s42 = new G4SubtractionSolid("4.2", s41, tubseForKrep, nullptr, translation42);
  G4SubtractionSolid *s43 = new G4SubtractionSolid("43", s42, tubseForKrep, nullptr, translation43);
  G4SubtractionSolid *s44 = new G4SubtractionSolid("44", s43, tubseForKrep, nullptr, translation44);
  G4SubtractionSolid *s45 = new G4SubtractionSolid("45", s44, tubseForKrep, nullptr, translation45);
  G4SubtractionSolid *s46 = new G4SubtractionSolid("46", s45, tubseForKrep, nullptr, translation46);
  G4SubtractionSolid *s47 = new G4SubtractionSolid("47", s46, tubseForKrep, nullptr, translation47);
  G4SubtractionSolid *s48 = new G4SubtractionSolid("48", s47, tubseForKrep, nullptr, translation48);
  G4SubtractionSolid *s49 = new G4SubtractionSolid("49", s48, tubseForKrep, nullptr, translation49);
  G4SubtractionSolid *s410 = new G4SubtractionSolid("410", s49, TubseForShift41, nullptr, translation410);
  G4SubtractionSolid *s411 = new G4SubtractionSolid("411", s410, TubseForShift42, nullptr, translation411);
  G4SubtractionSolid *s412 = new G4SubtractionSolid("410", s411, TubseForShift41, nullptr, translation412);
  G4SubtractionSolid *s413 = new G4SubtractionSolid("410", s412, TubseForShift42, nullptr, translation413);
  G4SubtractionSolid *s414 = new G4SubtractionSolid("410", s413, prism_0, nullptr, translation414);
  G4SubtractionSolid *s415 = new G4SubtractionSolid("410", s414, prism_0, nullptr, translation415);
  G4SubtractionSolid *s416 = new G4SubtractionSolid("410", s415, prism_1, nullptr, translation416);
  G4SubtractionSolid *solidTile4 = new G4SubtractionSolid("solidTile4", s416, prism_2, nullptr, translation417);

  auto logicTile4 = new G4LogicalVolume(solidTile4, plastic, "logicTile4");
  logicTile4->SetVisAttributes(tileAttr);
  // **************************************** //

  // **************** Тайл 5 **************** //
  G4Trap *solidTrap5 = new G4Trap("SolidBox5",
                                  0.5 * cm,        // половина длинны по оси Z
                                  0,               // угол тетта оси Z относительно глобальной оси координат
                                  0,               // угол phi оси Z относителтно глобальной оси координат
                                  5.56 / 2 * cm,   // половина длинны по оси Y на нижней грани
                                  10.699 / 2 * cm, // половина длин трапеции по оси X1 на нижней грани
                                  12.911 / 2 * cm, // половина длинны трапеции по оси Х2 на нижней грани
                                  0,
                                  5.56 / 2 * cm,
                                  10.699 / 2 * cm,
                                  12.911 / 2 * cm,
                                  0);

  G4Tubs *TubseForShift51 = new G4Tubs("TubseForShift51",    // "название",
                                       (2.25) * cm,          // начальный радиус,
                                       ((2.25 + 0.16)) * cm, // конечный радиус,
                                       0.35 * cm,            // Высота/2,
                                       -90 * deg,            // начальный угол,
                                       180 * deg             // угол охвата
  );
  G4Tubs *TubseForShift52 = new G4Tubs("TubseForShift52",    // "название",
                                       (2.25) * cm,          // начальный радиус,
                                       ((2.25 + 0.16)) * cm, // конечный радиус,
                                       0.35 * cm,            // Высота/2,
                                       90 * deg,             // начальный угол,
                                       180 * deg             // угол охвата
  );
  std::vector<G4TwoVector> polygon_5;
  polygon_5.push_back(G4TwoVector((0.0 - 0.05) * cm, 0.0 * cm));
  polygon_5.push_back(G4TwoVector((0.43 + 0.05) * cm, (0.) * cm));
  polygon_5.push_back(G4TwoVector((0.43 + 0.05) * cm, (0.16) * cm));
  polygon_5.push_back(G4TwoVector((0.0 - 0.05) * cm, 0.16 * cm));

  auto prism_5 = new G4ExtrudedSolid("prism5",               // название
                                     polygon_5,              // контур полигона
                                     0.35 * cm,              // половина высоты
                                     G4TwoVector(0, 0), 1.0, // параметры для нижнего основания (0.0)-  смещение, 1.0 - масштаб
                                     G4TwoVector(0, 0), 1.0  //  параметры для верхнего основания (0.0)-  смещение, 1.0 - масштаб
  );
  ///////////////////////////////////для вырезания середины/////////////////////////////////////////////
  G4ThreeVector translation51(0, 0, 0);
  ///////////////////////////////////для отверстий под крепление///////////////////////////////////////
  G4ThreeVector translation52((0.295 + 0.03 + 0.125) * cm, (5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation53(-(0.295 + 0.03 + 0.125) * cm, (5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation54((5.809 + 0.03 + 0.125) * cm, (5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation55(-(5.809 + 0.03 + 0.125) * cm, (5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation56((0.295 + 0.03 + 0.125) * cm, -(5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation57(-(0.295 + 0.03 + 0.125) * cm, -(5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation58((4.87 + 0.03 + 0.125) * cm, -(5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  G4ThreeVector translation59(-(4.87 + 0.03 + 0.125) * cm, -(5.56 / 2 - 0.295 - 0.125) * cm, 0.0 * cm);
  //////////////////////////////////для паз для шифтера/////////////////////////////////////////////////
  G4ThreeVector translation510(2.61 * cm, 0.053 * cm, -0.3 * cm);
  G4ThreeVector translation511(-2.61 * cm, 0.053 * cm, -0.3 * cm);
  G4ThreeVector translation512(3.037 * cm, 0.053 * cm, -0.3 * cm);
  G4ThreeVector translation513(-3.037 * cm, 0.053 * cm, -0.3 * cm);
  /////////////////////////////////для кастомной фигуры/////////////////////////////////////////////////
  G4ThreeVector translation514((2.825 - 0.215) * cm, (2.463 - 0.16) * cm, -0.3 * cm);
  G4ThreeVector translation515((2.825 - 0.215) * cm, -(2.357) * cm, -0.3 * cm);
  G4ThreeVector translation516(-(2.825 + 0.215) * cm, (2.463 - 0.16) * cm, -0.3 * cm);
  G4ThreeVector translation517(-(2.825 + 0.215) * cm, -(2.357) * cm, -0.3 * cm);
  ////////////////////////////////////////////////////////////////////////////////
  G4SubtractionSolid *s51 = new G4SubtractionSolid("5.1", solidTrap5, solidBox2, nullptr, translation51);
  G4SubtractionSolid *s52 = new G4SubtractionSolid("5.2", s51, tubseForKrep, nullptr, translation52);
  G4SubtractionSolid *s53 = new G4SubtractionSolid("5.3", s52, tubseForKrep, nullptr, translation53);
  G4SubtractionSolid *s54 = new G4SubtractionSolid("5.4", s53, tubseForKrep, nullptr, translation54);
  G4SubtractionSolid *s55 = new G4SubtractionSolid("5.5", s54, tubseForKrep, nullptr, translation55);
  G4SubtractionSolid *s56 = new G4SubtractionSolid("5.6", s55, tubseForKrep, nullptr, translation56);
  G4SubtractionSolid *s57 = new G4SubtractionSolid("5.7", s56, tubseForKrep, nullptr, translation57);
  G4SubtractionSolid *s58 = new G4SubtractionSolid("5.8", s57, tubseForKrep, nullptr, translation58);
  G4SubtractionSolid *s59 = new G4SubtractionSolid("5.9", s58, tubseForKrep, nullptr, translation59);
  G4SubtractionSolid *s510 = new G4SubtractionSolid("5.10", s59, TubseForShift52, nullptr, translation510);
  G4SubtractionSolid *s511 = new G4SubtractionSolid("5.11", s510, TubseForShift51, nullptr, translation511);
  G4SubtractionSolid *s512 = new G4SubtractionSolid("5.12", s511, TubseForShift51, nullptr, translation512);
  G4SubtractionSolid *s513 = new G4SubtractionSolid("5.13", s512, TubseForShift52, nullptr, translation513);
  G4SubtractionSolid *s514 = new G4SubtractionSolid("5.13", s513, prism_5, nullptr, translation514);
  G4SubtractionSolid *s515 = new G4SubtractionSolid("5.13", s514, prism_5, nullptr, translation515);
  G4SubtractionSolid *s516 = new G4SubtractionSolid("5.13", s515, prism_5, nullptr, translation516);
  G4SubtractionSolid *solidTile5 = new G4SubtractionSolid("solidTile5", s516, prism_5, nullptr, translation517);

  auto logicTile5 = new G4LogicalVolume(solidTile5, plastic, "logicTile5");
  logicTile5->SetVisAttributes(tileAttr);
  // **************************************** //

  logicDetector1 = logicTile1;
  logicDetector2 = logicTile2;
  logicDetector3 = logicTile3;
  logicDetector4 = logicTile4;
  logicDetector5 = logicTile5;

  G4int sectors_num = 16;
  G4double r1 = 73.35 * mm;
  G4double r2 = r1 + 54.7 * mm + 1 * mm;
  G4double r3 = r2 + 55.6 * mm + 1 * mm;
  G4double r4 = r3 + 55.6 * mm + 1 * mm;
  G4double r5 = r4 + 55.6 * mm + 1 * mm;
  G4double z = 1716. * mm;
  for (G4int i = 0; i < sectors_num; i++)
  {
    auto rotm = new G4RotationMatrix();
    rotm->rotateZ((22.5 * i) * deg);
    auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(r1 * sin(22.5 * PI / 180 * i) * mm, r1 * cos(22.5 * PI / 180 * i) * mm, z), logicTile1, "physTile1", logicWorld, false, i, checkOverlaps);
    auto physTile2 = new G4PVPlacement(rotm, G4ThreeVector(r2 * sin(22.5 * PI / 180 * i) * mm, r2 * cos(22.5 * PI / 180 * i) * mm, z), logicTile2, "physTile2", logicWorld, false, i, checkOverlaps);
    auto physTile3 = new G4PVPlacement(rotm, G4ThreeVector(r3 * sin(22.5 * PI / 180 * i) * mm, r3 * cos(22.5 * PI / 180 * i) * mm, z), logicTile3, "physTile3", logicWorld, false, i, checkOverlaps);
    auto physTile4 = new G4PVPlacement(rotm, G4ThreeVector(r4 * sin(22.5 * PI / 180 * i) * mm, r4 * cos(22.5 * PI / 180 * i) * mm, z), logicTile4, "physTile4", logicWorld, false, i, checkOverlaps);
    auto physTile5 = new G4PVPlacement(rotm, G4ThreeVector(r5 * sin(22.5 * PI / 180 * i) * mm, r5 * cos(22.5 * PI / 180 * i) * mm, z), logicTile5, "physTile5", logicWorld, false, i, checkOverlaps);
  }

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  auto sensDet1 = new SensitiveDetector("SensitiveDetector1");
  logicDetector1->SetSensitiveDetector(sensDet1);
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet1);

  auto sensDet2 = new SensitiveDetector("SensitiveDetector2");
  logicDetector2->SetSensitiveDetector(sensDet2);
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet2);

  auto sensDet3 = new SensitiveDetector("SensitiveDetector3");
  logicDetector3->SetSensitiveDetector(sensDet3);
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet3);

  auto sensDet4 = new SensitiveDetector("SensitiveDetector4");
  logicDetector4->SetSensitiveDetector(sensDet4);
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet4);

  auto sensDet5 = new SensitiveDetector("SensitiveDetector5");
  logicDetector5->SetSensitiveDetector(sensDet5);
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet5);
}
