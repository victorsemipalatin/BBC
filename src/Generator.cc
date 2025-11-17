// #include "Generator.hh"

// PrimaryGenerator::PrimaryGenerator()
// {
//     ParticleGun = new G4ParticleGun(1); // 1 частица в событии
// }

// PrimaryGenerator::~PrimaryGenerator()
// {
//     delete ParticleGun;
//     G4cout << "PrimaryGenerator killed" << G4endl;
// }

// struct SpectrumPoint
// {
//     double E;
//     double counts;
// };

// double fastRand()
// {
//     static unsigned long long seed = 88172645463325252ull;
//     seed ^= seed << 13;
//     seed ^= seed >> 7;
//     seed ^= seed << 17;
//     return (seed % 1000000) / 1000000.0;
// }

// double linearInterp(double x, double x1, double y1, double x2, double y2)
// {
//     if (x2 == x1)
//         return y1;
//     return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
// }

// std::vector<double> buildCDF(const std::vector<SpectrumPoint> &spectrum, int nSteps = 2000, double scale = 0.12)
// {
//     double Emin = spectrum.front().E;
//     double Emax = spectrum.back().E;
//     double step = (Emax - Emin) / nSteps;

//     std::vector<double> cdf(nSteps + 1);
//     double integral = 0.0;

//     for (int i = 0; i <= nSteps; ++i)
//     {
//         double E = Emin + i * step;

//         auto it = std::upper_bound(spectrum.begin(), spectrum.end(), E,
//                                    [](double value, const SpectrumPoint &p)
//                                    { return value < p.E; });

//         double y;
//         if (it == spectrum.begin())
//             y = std::exp(scale * it->counts);
//         else if (it == spectrum.end())
//             y = std::exp(scale * (it - 1)->counts);
//         else
//         {
//             auto p1 = it - 1;
//             auto p2 = it;
//             double y1 = std::exp(scale * p1->counts);
//             double y2 = std::exp(scale * p2->counts);
//             y = linearInterp(E, p1->E, y1, p2->E, y2);
//         }

//         integral += y * step;
//         cdf[i] = integral;
//     }

//     for (auto &v : cdf)
//         v /= integral;

//     return cdf;
// }

// double randomEnergy(const std::vector<SpectrumPoint> &spectrum, const std::vector<double> &cdf, int nSteps = 2000)
// {
//     double r = fastRand();
//     double Emin = spectrum.front().E;
//     double Emax = spectrum.back().E;
//     double step = (Emax - Emin) / nSteps;

//     auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
//     int idx = std::distance(cdf.begin(), it);
//     if (idx <= 0)
//         return Emin;
//     if (idx >= (int)cdf.size())
//         return Emax;

//     double c1 = cdf[idx - 1];
//     double c2 = cdf[idx];
//     double E1 = Emin + (idx - 1) * step;
//     double E2 = Emin + idx * step;

//     double E = E1 + (E2 - E1) * (r - c1) / (c2 - c1);
//     return E;
// }

// void PrimaryGenerator::GeneratePrimaries(G4Event *Event)
// {
//     G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
//     // G4ParticleDefinition *particle = particleTable -> FindParticle("proton");
//     G4ParticleDefinition *particle = particleTable->FindParticle("gamma");
//     // auto particle = G4OpticalPhoton::Definition();

//     // **Тестирование всей поверхности 5 тайла** //
//     auto angle = 0. * degree;
//     angle += 90. * degree;
//     G4int batch_size = 10000;
//     G4long eventID = Event->GetEventID();

//     const G4int NX = 30;
//     const G4int NY = 30;

//     const G4int nPerCell = batch_size;
//     const G4int ix = (eventID / nPerCell) % NX;
//     const G4int iy = (eventID / nPerCell / NX) % NY;

//     const G4double x_min = -65. * mm;
//     const G4double x_max = 0. * mm;
//     const G4double y_min = 0. * mm;
//     const G4double y_max = 58. * mm;

//     const G4double x_step = (x_max - x_min) / NX;
//     const G4double y_step = (y_max - y_min) / NY;

//     const G4double xr = x_min + x_step * ix;
//     const G4double yr = y_min + y_step * iy;

//     G4double radius = 2.5;
//     G4double xi = (2 * G4UniformRand() - 1.0) * radius;
//     G4double yi = (2 * G4UniformRand() - 1.0) * radius;
//     while ((xi * xi + yi * yi) > radius * radius)
//     {
//         xi = (2 * G4UniformRand() - 1.0) * radius;
//         yi = (2 * G4UniformRand() - 1.0) * radius;
//     }

//     auto x = (xr + xi) * mm;
//     auto y = (yr + yi) * mm;
//     G4ThreeVector position(x, y, -30. * mm);
//     G4ThreeVector momentum(0., 0, 1);

//     std::vector<SpectrumPoint> spectrum = {
//         {0, 0}, {2, 0}, {3.6, 62}, {4, 66}, {6, 83.5}, {8, 88.3}, {10, 88.3}, {12, 87.5}, {14, 86.3}, {16, 85}, {18, 84}, {20, 83}, {21, 83}, {21.5, 85}, {22.1, 102}, {22.6, 92}, {23, 82}, {24.5, 82}, {25, 90}, {26, 80}, {28, 79}, {30, 78}, {32, 77}, {34, 76}, {36, 75}, {38, 74}, {40, 73}, {42, 72}, {44, 71}, {46, 69}, {48, 65}, {49, 60}, {50, 44}};
//     double scale = 0.12;
//     int nSteps = 50000;
//     auto cdf = buildCDF(spectrum, nSteps, scale);
//     double energy = randomEnergy(spectrum, cdf, nSteps);
//     // **************************************** //

//     // **************** Tube **************** //
//     // // Свет в торец
//     // G4double l = 900;
//     // G4ThreeVector position(0, 0, 0.);
//     // G4ThreeVector momentum(0., 0., 1.);
//     // Свет в боковую часть
//     // G4ThreeVector position(0., 5. * mm, -900 / 2 * mm - 10. * mm);
//     // G4ThreeVector momentum(0., 0., 1.);
//     // **************************************** //

//     // **************** Ring **************** //
//     // G4ThreeVector position(-13.48 * mm, -1. * mm, -100. * mm);
//     // G4ThreeVector momentum(0, 0., 1.);
//     // **************************************** //

//     //***** Проверка разлёта гамма-квантов *****//
//     // G4int batch_size = 10000;
//     // G4long eventID = Event -> GetEventID();

//     // const G4int NX = 30;
//     // const G4int NY = 30;

//     // const G4int nPerCell = batch_size;
//     // const G4int ix = (eventID / nPerCell) % NX;
//     // const G4int iy = (eventID / nPerCell / NX) % NY;

//     // const G4double x_min = -25. * mm;
//     // const G4double x_max = 25. * mm;
//     // const G4double y_min = 0. * mm;
//     // const G4double y_max = 70. * mm;

//     // const G4double x_step = (x_max - x_min) / NX;
//     // const G4double y_step = (y_max - y_min) / NY;

//     // const G4double xr = x_min + x_step * ix;
//     // const G4double yr = y_min + y_step * iy;

//     // G4double radius = 2.5;
//     // G4double xi =  (2 * G4UniformRand() -1.0) * radius;
//     // G4double yi = (2 * G4UniformRand() -1.0) * radius;
//     // while ((xi * xi + yi * yi) > radius * radius){
//     //     xi = (2 * G4UniformRand() -1.0) * radius;
//     //     yi = (2 * G4UniformRand() - 1.0) * radius;
//     // }

//     // auto x = (xr + xi) * mm;
//     // auto y = (yr + yi + 40) * mm;

//     // G4double x = 0.;
//     // G4double y = 85.;
//     // G4ThreeVector position(x, y, 30. * mm);
//     // G4ThreeVector momentum(0., 0., -1.);
//     // **************************************** //

//     // G4double energy = 22. * keV;
//     // G4double energy = 15. * keV;
//     // G4double energy = 2.7899499 * eV;
//     ParticleGun->SetParticleEnergy(energy);              // задание энергии первичной частицы
//     ParticleGun->SetParticlePosition(position);          // позиция первичной частицы
//     ParticleGun->SetParticleMomentumDirection(momentum); // задание вектора импульса
//     ParticleGun->SetParticleDefinition(particle);
//     ParticleGun->GeneratePrimaryVertex(Event);

//     // для фотонов
//     // const int N = 100;
//     // for(int i = 0; i < N; i++)
//     //     ParticleGun -> GeneratePrimaryVertex(Event);
// }
