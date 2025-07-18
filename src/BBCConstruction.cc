#include "BBCConstruction.hh"
// #include "CADMesh.hh" // конвертер


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
    G4double reflectivity[2] = {.8, .8};
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

    G4OpticalSurface* dd = new G4OpticalSurface("CoreAir");
    dd -> SetType(dielectric_dielectric);
    dd -> SetFinish(polished);
    dd -> SetModel(unified);

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
	G4Material* PMMA = new G4Material("PMMA", 1.19 * g / cm3, 3);
	PMMA -> AddElement(elC, 5);
	PMMA -> AddElement(elH, 8);
	PMMA -> AddElement(elO, 2);

	// Core
	G4Material* PS = new G4Material("PS", 1.05 * g / cm3, 2);
	PS -> AddElement(elC, 8);
	PS -> AddElement(elH, 8);

    G4double energies[188] = {2.0132323531, 2.0191595709, 2.0249811007, 2.0309070471, 2.036867779,
                              2.0428636035, 2.0487268164, 2.0547927658, 2.0608947425, 2.0672040994,
                              2.0733801236, 2.0795931616, 2.0858435473, 2.0921316182, 2.0989866186,
                              2.1052478588, 2.1106902954, 2.1176676553, 2.1241493647, 2.1306708741,
                              2.1372325511, 2.1381470929, 2.1435588614, 2.1502002827, 2.1567632881,
                              2.1642901376, 2.1699653543, 2.1756185361, 2.1817457184, 2.1860409754,
                              2.1919097387, 2.1983032288, 2.204704942, 2.2092376743, 2.2151139745,
                              2.2186547836, 2.2245813545, 2.2293454571, 2.2353293574, 2.2389351417,
                              2.2453338714, 2.2516475159, 2.2606411804, 2.266970798, 2.2719183924,
                              2.2781333379, 2.2818786533, 2.2877292397, 2.2919266317, 2.2979345973,
                              2.3041866342, 2.3111903315, 2.3161489806, 2.3226086156, 2.3278023241,
                              2.3343272155, 2.3378221367, 2.3438535464, 2.3478483454, 2.3523499932,
                              2.3554549029, 2.358623686, 2.3600533505, 2.3642435545, 2.3668352945,
                              2.3695290335, 2.372132381, 2.3759997242, 2.3787790862, 2.3843928267,
                              2.38855827, 2.3923812167, 2.3954287099, 2.3974975558, 2.3980153257,
                              2.4020418243, 2.4046179521, 2.4117308797, 2.4161039851, 2.4193210218,
                              2.42472144, 2.4258313366, 2.4289647011, 2.4313758147, 2.4410683277,
                              2.4485645472, 2.4579668384, 2.4660061568, 2.4714949973, 2.4744323816,
                              2.4836905469, 2.4917976234, 2.4996849314, 2.5094770645, 2.5132637284,
                              2.5208714431, 2.5258412047, 2.5302325406, 2.5341678292, 2.539706449,
                              2.5431965776, 2.5463069768, 2.5494249936, 2.5525506559, 2.5557711388,
                              2.5564685282, 2.5618161758, 2.5647353345, 2.5698797278, 2.5718638401,
                              2.5762396896, 2.5798754212, 2.5823642625, 2.5902990725, 2.5989523756,
                              2.6077008032, 2.6109710857, 2.6109710857, 2.6117899384, 2.614249581,
                              2.6160562652, 2.6200068028, 2.6216563837, 2.6216563837, 2.6224819534,
                              2.6244654436, 2.6282755319, 2.6291052763, 2.6327623842, 2.6332618692,
                              2.6352061137, 2.6365966185, 2.637849328, 2.6411119508, 2.6412337898,
                              2.6449705613, 2.6452506089, 2.645810882, 2.645810882, 2.6483350511,
                              2.6517081113, 2.6550897747, 2.6561785704, 2.6576316878, 2.6593290023,
                              2.6595716528, 2.6601784728, 2.6618790429, 2.6635817887, 2.6652867143,
                              2.6666278306, 2.6698438712, 2.6732311163, 2.6747029567, 2.6749484211,
                              2.6764221537, 2.6781435621, 2.6803376507, 2.6841859687, 2.6850513962,
                              2.6885186937, 2.6887667005, 2.6893869178, 2.6919949577, 2.6944336962,
                              2.6974009684, 2.7029164588, 2.7077500436, 2.7095119998, 2.7116686182,
                              2.7148116534, 2.7201320794, 2.7259194571, 2.7342428137, 2.7435809336,
                              2.7443343568, 2.7534078187, 2.7634581778, 2.7754308784, 2.7875077739,
                              2.7971043005, 2.8091340551, 2.8205510849, 2.8320612969, 2.8436658364,
                              2.8555057338, 2.8664576619, 2.8790571599};

    // G4double wavelength[188] = {615.8464164319, 614.038606944, 612.2733341501, 610.4867930092, 608.7002518682,
    //                             606.9137107273, 605.1767957292, 603.3902545883, 601.6037134474, 599.7675461637,
    //                             597.9810050228, 596.1944638819, 594.407922741, 592.6213816001, 590.6859620308,
    //                             588.9291965756, 587.4106366058, 585.4752170365, 583.6886758956, 581.9021347547,
    //                             580.1155936138, 579.8674628998, 578.4034916871, 576.6169505462, 574.8623119257,
    //                             572.8630873156, 571.3648504329, 569.8802016607, 568.2797585553, 567.1631703423,
    //                             565.6446103725, 563.9995037386, 562.3618410261, 561.2080332059, 559.7192489218,
    //                             558.8259783514, 557.3371940673, 556.14616664, 554.6573823559, 553.7641117855,
    //                             552.1860004444, 550.6376647889, 548.4470250566, 546.9157040787, 545.7246766514,
    //                             544.2358923673, 543.3426217969, 541.9530897984, 540.9605669423, 539.5462218725,
    //                             538.0822506598, 536.4516773963, 535.3031866628, 533.8144023787, 532.6233749515,
    //                             531.1345906674, 530.3405723825, 528.9758534555, 528.0758156837, 527.0652469575,
    //                             526.3704809583, 525.6633084234, 525.344874007, 524.4137930421, 523.8395476754,
    //                             523.2440339617, 522.669788595, 521.8190547184, 521.2093621068, 519.9822429393,
    //                             519.0754379663, 518.2459724366, 517.5866536822, 517.140018397, 517.0283595757,
    //                             516.1616744389, 515.6086974191, 514.0880106146, 513.157520437, 512.4751609735,
    //                             511.333759689, 511.099807873, 510.4404891186, 509.934302462, 507.9095558356,
    //                             506.3546033612, 504.4176799693, 502.7732500555, 501.6566618425, 501.0611481288,
    //                             499.1934005724, 497.5692722625, 495.999281563, 494.0638619936, 493.3194698516,
    //                             491.8306855675, 490.8629757829, 490.0110603314, 489.2501261418, 488.1831640715,
    //                             487.5132111437, 486.91769743, 486.3221837164, 485.7266700028, 485.1146142415,
    //                             484.9822778607, 483.9699045475, 483.4190543624, 482.4513445778, 482.0791485068,
    //                             481.2603171505, 480.5820931989, 480.118915866, 478.6481774521, 477.054501548,
    //                             475.4540584426, 474.8585447289, 474.8585447289, 474.7096663005, 474.2630310153,
    //                             473.9354984728, 473.2208820164, 472.9231251596, 472.9231251596, 472.7742467312,
    //                             472.416938503, 471.7320977324, 471.5832193039, 470.9281542189, 470.8388271619,
    //                             470.4914441623, 470.2433134483, 470.0199958057, 469.4393699349, 469.4177148907,
    //                             468.7545291642, 468.7049030214, 468.6056507358, 468.6056507358, 468.1590154506,
    //                             467.5635017369, 466.9679880233, 466.776572901, 466.5213527381, 466.2235958812,
    //                             466.1810591874, 466.0747174528, 465.776960596, 465.4792037392, 465.1814468824,
    //                             464.9474950663, 464.3874285975, 463.7990043329, 463.5437841699, 463.5012474761,
    //                             463.2460273131, 462.9482704562, 462.5693071839, 461.9061214574, 461.757243029,
    //                             461.1617293153, 461.1191926215, 461.0128508869, 460.5662156017, 460.1493560022,
    //                             459.6431693456, 458.7052352466, 457.8864038904, 457.5886470335, 457.2247219863,
    //                             456.6953764631, 455.8021058926, 454.834396108, 453.4498267238, 451.9064536826,
    //                             451.7823883256, 450.2936040415, 448.655941329, 446.7205217597, 444.7851021904,
    //                             443.2590982992, 441.360898337, 439.5743571961, 437.7878160552, 436.0012749143,
    //                             434.1934654265, 432.5345343671, 430.6416514916};

    G4double pdf[188] = {0.0080953125, 0.0082792094, 0.0113135089, 0.0141026124, 0.0162480767,
                         0.018393541, 0.0176783862, 0.0202529434, 0.0228275005, 0.0259026659,
                         0.0261172124, 0.0261172124, 0.0267608517, 0.0282626766, 0.0303008677,
                         0.0276619466, 0.0293211057, 0.0334117909, 0.0385609052, 0.0415645551,
                         0.0471427622, 0.0394190909, 0.0431736533, 0.0483227676, 0.05388565,
                         0.0582991765, 0.062733136, 0.072137421, 0.0771256255, 0.0874774906,
                         0.0944717041, 0.1039975655, 0.1083421306, 0.1175139904, 0.1225021948,
                         0.1329613332, 0.1378243855, 0.1484086759, 0.1542014295, 0.1651432973,
                         0.1736393358, 0.18659794, 0.2017387879, 0.207337428, 0.2179217184,
                         0.224519021, 0.2372308968, 0.2419509182, 0.2509082316, 0.2581491735,
                         0.2675534586, 0.2736424905, 0.2830901957, 0.2895802251, 0.3028821036,
                         0.308835767, 0.3243081401, 0.3421440998, 0.3629160948, 0.3864121793,
                         0.4200244529, 0.4032898316, 0.4248875053, 0.4417242916, 0.4573555313,
                         0.472802874, 0.4899052892, 0.5060882197, 0.5237218928, 0.540700967,
                         0.5595332673, 0.5744978806, 0.5901061332, 0.6259890231, 0.6089325822,
                         0.6281957864, 0.6437902467, 0.6712046827, 0.6850429272, 0.7016523965,
                         0.7156694297, 0.729706896, 0.7448477439, 0.7634703737, 0.769520583,
                         0.7684478509, 0.7601000444, 0.7566477974, 0.7407177251, 0.7227689661,
                         0.7144601681, 0.6974914961, 0.6929275084, 0.6900311317, 0.6958238852,
                         0.7039408917, 0.7201212681, 0.7368380106, 0.7560041581, 0.7727387794,
                         0.7885079418, 0.8039552846, 0.8194026273, 0.8348499701, 0.8514058027,
                         0.8705719503, 0.8857618373, 0.9005011769, 0.9134812357, 0.9288213053,
                         0.9454486534, 0.9612535735, 0.9799906282, 0.9882994262, 0.9952770611,
                         0.9922197745, 0.9606814497, 0.9490959426, 0.9787033496, 0.9220630928,
                         0.9400849927, 0.9143394214, 0.8853756537, 0.8976048001, 0.8474009361,
                         0.8649079246, 0.8390336254, 0.8132880542, 0.7943650593, 0.7738115115,
                         0.7194454469, 0.7529146895, 0.7328331439, 0.6890656728, 0.7093110538,
                         0.6713012286, 0.6371454373, 0.6491600373, 0.6066798447, 0.6226420989,
                         0.5959954326, 0.5757851591, 0.558498847, 0.540742576, 0.5214620037,
                         0.4924798464, 0.506915756, 0.4755919776, 0.4599300884, 0.4440536528,
                         0.4262769488, 0.4029680119, 0.3799349205, 0.3286276749, 0.3633841961,
                         0.3452192653, 0.3028821036, 0.3199678009, 0.2928413308, 0.2714725066,
                         0.2204962755, 0.2333690611, 0.2537080625, 0.1973252614, 0.2142658473,
                         0.1879281278, 0.1706142312, 0.155488708, 0.1200885475, 0.1366801379,
                         0.1159406499, 0.0994920905, 0.086458395, 0.0684686771, 0.0483227676,
                         0.0312663266, 0.0245081142, 0.0148535249, 0.0145317053, 0.009382591,
                         0.0027852884, 0.0016589197, 0.0016589197, 0.0016589197, 0.0016589197,
                         0.0031300952, 0.0068080339, 0.0080953125};

    G4double EnergyOpt[10] = {1.9 * eV, 2.2 * eV, 2.3 * eV, 2.4 * eV, 2.56 * eV, 2.66 * eV, 2.68 * eV, 3.69 * eV, 3.7 * eV, 4.0 * eV };
	G4double spIzlOpt[10] = {0.001, 0.05, 0.25, 0.7, 1., 1., 0., 0., 0., 0.};
	G4double absLenOpt[10] = {5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 5.0 * m, 0.1 * mm, 0.1 * mm, 5.0 * m, 5.0 * m };
	G4double rindexOptCore[10] = {1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59, 1.59 };
	G4double rindexOptCladding[10] = {1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49 };

    G4MaterialPropertiesTable* optCladdingProperties = new G4MaterialPropertiesTable();
	optCladdingProperties -> AddProperty("RINDEX", EnergyOpt, rindexOptCladding, 10);
	optCladdingProperties -> AddProperty("WLSABSLENGTH", EnergyOpt, absLenOpt, 10);
	optCladdingProperties -> AddProperty("WLSCOMPONENT", EnergyOpt, spIzlOpt, 10);
	optCladdingProperties -> AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
	PMMA -> SetMaterialPropertiesTable(optCladdingProperties);

    G4MaterialPropertiesTable* optCoreProperties = new G4MaterialPropertiesTable();
	optCoreProperties -> AddProperty("RINDEX", EnergyOpt, rindexOptCore, 10);
	optCoreProperties -> AddProperty("WLSABSLENGTH", EnergyOpt, absLenOpt, 10);
	optCoreProperties -> AddProperty("WLSCOMPONENT", energies, pdf, 188);
	optCoreProperties -> AddConstProperty("WLSTIMECONSTANT", 0.5 * ns);
	PS -> SetMaterialPropertiesTable(optCoreProperties);

    // **************************************** //

    // ********* НАСТРОЙКА ОТРИСОВКИ ********* //    
    auto tileAttr = new G4VisAttributes(G4Color(192, 192, 192, 1.)); // серый цвет
    tileAttr -> SetForceSolid(true);

    auto shifterCore = new G4VisAttributes(G4Color(0, 255, 0, 1));
    shifterCore -> SetForceSolid(true);

    auto shifterCladding = new G4VisAttributes(G4Color(75, 245, 66, .8));
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
    // G4int shift = 1;
    // G4int sectors_num = 1;
    // auto prismSolid = new G4Trd("Tile1", 17.9 / 2 * mm, 39.66 / 2 * mm, 10. / 2 * mm, 10. / 2 * mm, 54.7 / 2 * mm);

    // auto cylinder = new G4Tubs("Сylinder", 0., 1.25 * mm, 11. / 2 * mm, 0. * deg, 360. * deg);

    // auto rotationMatrix = new G4RotationMatrix();
    // rotationMatrix -> rotateX(90. * deg);
    // auto subtraction1_0 = new G4SubtractionSolid("Subtraction1", prismSolid, cylinder, rotationMatrix, G4ThreeVector(5.67 * mm, 0., -23.35 * mm));
    // auto subtraction2_0 = new G4SubtractionSolid("Subtraction2", subtraction1_0, cylinder, rotationMatrix, G4ThreeVector(-5.67 * mm, 0., -23.35 * mm));
    // auto subtraction3_0 = new G4SubtractionSolid("Subtraction3", subtraction2_0, cylinder, rotationMatrix, G4ThreeVector(14.89 * mm, 0., 23.35 * mm));
    // auto subtraction4_0 = new G4SubtractionSolid("Subtraction4", subtraction3_0, cylinder, rotationMatrix, G4ThreeVector(-14.89 * mm, 0., 23.35 * mm));

    // auto tube_0 = new G4Tubs("Tube", 12.75 * mm, 14.35 * mm, 7. / 2 * mm, 0 * deg, 360 * deg);
    // auto subtraction5_0 = new G4SubtractionSolid("Subtraction5", subtraction4_0, tube_0, rotationMatrix, G4ThreeVector(0., 3. * mm, 11. * mm));

    // std::vector<G4TwoVector> polygon_0;
    // G4double delta = 2.7;
    // polygon_0.push_back(G4TwoVector(0., 0.));
    // polygon_0.push_back(G4TwoVector(3.2 * mm, 0.));
    // polygon_0.push_back(G4TwoVector((6.2 + delta * 1.455) * mm, (2.08 + delta) * mm));
    // polygon_0.push_back(G4TwoVector((3.0 + delta * 1.455) * mm, (2.08 + delta) * mm));
    // G4double halfZ =  7 / 2 * mm;
    // auto prism_0 = new G4ExtrudedSolid("Prism", polygon_0, halfZ, G4TwoVector(0, 0), 1.0, G4TwoVector(0, 0), 1.0);

    // auto rotation_0 = new G4RotationMatrix();
    // rotation_0 -> rotateX(90 * deg);
    // rotation_0 -> rotateY(-180 * deg);
    
    // auto solidTile0 = new G4SubtractionSolid("SolidTile0", subtraction5_0, prism_0, rotation_0, G4ThreeVector(2.1 * mm, 3. * mm, 27.5 * mm));

    // auto logicTile0 = new G4LogicalVolume(solidTile0, scintillator, "logicTile1");

    // // logicTile0 -> SetVisAttributes(tileAttr);

    // G4double r = 73.35;
    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateX(90 * deg);
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     rotm -> rotateZ(180. * deg);
    //     auto physTile1 = new G4PVPlacement(rotm, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r +  shift * cos(22.5 * PI / 180 * i) * mm, 0), logicTile0, "physTile1", logicWorld, false, i, checkOverlaps);
    //     new G4LogicalBorderSurface("scintInSurface", physTile1, physWorld, opInSurface);
    //     new G4LogicalBorderSurface("scintOutSurface", physWorld, physTile1, opOutSurface);
    // }

    // G4double l = 97.;
    // auto solidRingCore0 = new G4Torus("SolidRingCore0", 0., 0.48 * mm, 13.48 * mm, 0 * deg, 335 * deg); 
    // auto solidOutletCore0 = new G4Tubs("SolidOutletCore0", 0 * mm, 0.48 * mm, l / 2 * mm, 0 * deg, 360 * deg);
    // auto solidCore0 = new G4UnionSolid("SolidCore0", solidRingCore0, solidOutletCore0, G4Transform3D(*rotationMatrix, G4ThreeVector(13.48 * mm, -(l / 2 - 0.01) * mm, 0. * mm)));

    // auto solidCladdingAll0 = new G4Torus("SolidRingCore0", 0.48 * mm, 0.5 * mm, 13.48 * mm, 0 * deg, 335 * deg); 
    // auto solidOutletCladding0 = new G4Tubs("SolidOutletCladding0", 0.48, .5 * mm, l / 2 * mm, 0 * deg, 360 * deg);
    // auto solidCladding0 = new G4UnionSolid("SolidCladding0", solidCladdingAll0, solidOutletCladding0, G4Transform3D(*rotationMatrix, G4ThreeVector(13.48 * mm, -(l / 2 - 0.0001) * mm, 0. * mm)));

    // auto cyl = new G4Tubs("Cyl", 0., .5 * mm, l / 2 * mm, 0 * deg, 360 * deg);
    // auto ring = new G4Torus("Ring", 0. * mm, 0.5 * mm, 13.48 * mm, 0 * deg, 335 * deg);
    // auto subs = new G4UnionSolid("SolidCladding0", ring, cyl, G4Transform3D(*rotationMatrix, G4ThreeVector(13.48 * mm, -(l / 2 - 0.0001) * mm, 0. * mm)));

    // auto logicCore0 = new G4LogicalVolume(solidCore0, PS, "logicCore0");
    // auto logicCladding0 = new G4LogicalVolume(solidCladding0, PMMA, "logicCladding0");

    // logicCore0 -> SetVisAttributes(shifterCore);
    // logicCladding0 -> SetVisAttributes(shifterCladding);

    // for (G4int i = 0; i < sectors_num; i++){
    //     auto rotm = new G4RotationMatrix();
    //     rotm -> rotateZ((22.5 * i) * deg);
    //     rotm -> rotateY(180. * deg);
    //     delete rotationMatrix;
    //     rotationMatrix = nullptr;
    //     rotationMatrix = new G4RotationMatrix();
    //     rotationMatrix -> rotateY(180. * deg);
    //     rotationMatrix -> rotateZ(237. * deg);
    //     auto physCore1 = new G4PVPlacement(rotationMatrix, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r +  11 + shift * cos(22.5 * PI / 180 * i) * mm, 1.5), logicCore0, "physCore1", logicWorld, false, i, checkOverlaps);
    //     auto physCladding1 = new G4PVPlacement(rotationMatrix, G4ThreeVector(shift * sin(22.5  * PI / 180 * i) * mm, r + 11 + shift * cos(22.5 * PI / 180 * i) * mm, 1.5), logicCladding0, "physCladding1", logicWorld, false, i, checkOverlaps);
    //     new G4LogicalBorderSurface("shifterSurface", physWorld, physCore1, dd);
    // }
    // **************************************** //

    // ********** СВИНЦОВАЯ ПЛАСТИНА ********* //
    // auto solidPlateF = new G4Box("SolidPlateF", .5 * m, 5. / 2 * mm, 0.99 / 2 * m);
    // auto solidPlate = new G4SubtractionSolid("SolidPlate", solidPlateF, subs, G4Transform3D(*rotationMatrix, G4ThreeVector(0, -l / 2, 0)));
    // auto logicPlate = new G4LogicalVolume(solidPlate, lead, "logicPlate");
    // logicPlate -> SetVisAttributes(plateAttr);
    // auto physPlate = new G4PVPlacement(0, G4ThreeVector(0., r + 11 + shift * mm + l / 2, 1.5), logicPlate, "physPlate", logicWorld, false, 0, checkOverlaps);
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
    // auto solidDetector1 = new G4Box("SolidDetector1", 1. / 2 * mm, 1. / 2 * mm, .5 / 2 * mm);
    // auto logicSipm = new G4LogicalVolume(solidDetector1, worldMat, "logicDetector1");
    // logicSipm -> SetVisAttributes(sipmAttr);
    // logicDetector1 = logicSipm;
    // auto detectorMat = new G4RotationMatrix();
    // detectorMat -> rotateY(90 * deg);
    // detectorMat -> rotateX(35. * deg);
    // auto physDetector = new G4PVPlacement(detectorMat, G4ThreeVector(-74.3 * mm, 149.7 * mm, 1.5), logicDetector1, "physDetector", logicWorld, false, 0, checkOverlaps);
    // **************************************** //

    // ******** Тестирвоание волокна ********** //
    
    G4double l = 100.;
    auto solidCoreTest = new G4Tubs("SolidOutletCoreTest", 0 * mm, 0.48 * mm, l / 2 * mm, 0 * deg, 360 * deg);
    auto solidCladdingTest = new G4Tubs("SolidOutletCladdingTest", 0.48, .5 * mm, l / 2 * mm, 0 * deg, 360 * deg);

    auto logicCoreTest = new G4LogicalVolume(solidCoreTest, PS, "logicCoreTest");
    auto logicCladdingTest = new G4LogicalVolume(solidCladdingTest, PMMA, "logicCladdingTest");

    auto solidTester = new G4Box("SolidTester", 1. / 2 * mm, 1. / 2 * mm, 1. / 2 * mm);
    auto logicTester = new G4LogicalVolume(solidTester, worldMat, "logicTester");
    logicDetector1 = logicTester;

    auto solidScint = new G4Box("SolidScint", 1. / 2 * mm, 1. / 2 * mm, 1. / 2 * mm);
    auto logicScint = new G4LogicalVolume(solidTester, scintillator, "logicScint");

    auto solidBox = new G4Box("SolidBox", .5 * m, .5 * m, 1. / 2 * cm);
    auto subs = new G4Tubs("Subs", 0., 0.5 * mm, l / 2 * mm, 0 * deg, 360 * deg);
    auto solidPlate = new G4SubtractionSolid("SolidPlate", solidBox, subs, nullptr, G4ThreeVector());
    auto logicPlate = new G4LogicalVolume(solidPlate, lead, "logicPlate");

    logicCoreTest -> SetVisAttributes(shifterCore);
    logicCladdingTest -> SetVisAttributes(shifterCladding);
    logicTester -> SetVisAttributes(sipmAttr);
    logicScint -> SetVisAttributes(tileAttr);
    logicPlate -> SetVisAttributes(plateAttr);

    auto physCoreTest = new G4PVPlacement(nullptr, G4ThreeVector(), logicCoreTest, "physCoreTest", logicWorld, false, 0, checkOverlaps);
    auto physCladdingTest = new G4PVPlacement(nullptr, G4ThreeVector(), logicCladdingTest, "physCladdingTest", logicWorld, false, 0, checkOverlaps);
    // auto physPlate = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -40), logicPlate, "physPlate", logicWorld, false, 0, checkOverlaps);
    // auto physTester = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., l / 2 * mm + 1. * mm), logicDetector1, "physTester", logicWorld, false, 0, checkOverlaps);
    // auto physScint = new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -l / 2 * mm -1. * mm), logicScint, "physScint", logicWorld, false, 0, checkOverlaps);
    new G4LogicalBorderSurface("shifterSurface", physWorld, physCoreTest, dd);


    // **************************************** //
    
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
