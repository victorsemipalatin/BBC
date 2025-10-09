void WLDistribution() {
    TFile *inputFile = new TFile("output_24.07.2025_00:40:17_90cm.root", "READ");
    TH1D *h = (TH1D*)inputFile -> Get("Finish");

	TCanvas* c1 = new TCanvas("c1", "Photons number per event distribution", 2426, 1433);
	h -> SetLineColor(kRed);
	h -> SetLineWidth(2);
	h -> SetFillColor(kYellow);
	h -> SetFillStyle(3003);
	h -> SetMarkerStyle(20);
	h -> SetMarkerColor(kBlue);
	
	h -> SetTitle("Photon wavelength distribution (after fiber)");
	h -> GetXaxis() -> SetTitle("#lambda, nm");
	h -> GetYaxis() -> SetTitle("Count");
	h -> SetStats(1);
	h -> Draw();

	gStyle -> SetOptStat("nemri");

	c1 -> SetGrid();
	c1 -> SaveAs("photon_wl_tube_after_fiber_90cm.png");
}


void ElDep() {
    TFile *inputFile = new TFile("output_07.10.2025_21:37:49.root", "READ");
    TH1D *h = (TH1D*)inputFile -> Get("Electrons Deposited Energy");

	TCanvas* c1 = new TCanvas("c1", "Electron energy spectrum", 2426, 1433);
	h -> SetLineColor(kRed);
	h -> SetLineWidth(2);
	h -> SetFillColor(kYellow);
	h -> SetFillStyle(3003);
	h -> SetMarkerStyle(20);
	h -> SetMarkerColor(kBlue);
	
	h -> SetTitle("Electron energy spectrum");
	h -> GetXaxis() -> SetTitle("E, keV");
	h -> GetYaxis() -> SetTitle("Count");
	h -> SetStats(0);
	gPad -> SetLogy();
	h -> Draw();

	gStyle -> SetOptStat("nemri");

	c1 -> SetGrid();
	c1 -> SaveAs("eldep_log_22_keV.png");
}


// --- параметры анода Ag ---
const double Emax = 50.0;   // ускоряющее напряжение [keV]
const double Ka = 22.1;     // Kα
const double Kb = 24.9;     // Kβ
const double Ek = 25.5;     // K-край
const double E0 = 7.0;      // экспоненциальное затухание

// --- функция интенсивности ---
double BremsIntensity(double E) {
    if (E <= 0.0 || E >= Emax) return 0.0;
    double I = (Emax - E) * E * std::exp(-E / E0);  // гладкий рост + спад
    if (E > Ek) I *= std::exp(-(E - Ek) / 2.0);     // подавление после K-края
    return I;
}

// --- нормализованный метод с инверсией распределения ---
double SampleBremsstrahlung() {
    // создаём выборку по pdf с плотностью I(E)
    // используем метод принятия-отбраковки, но теперь с pdf, которая реально растёт от 0
    static const double yMax = BremsIntensity(10.0);  // максимум около 10 keV
    while (true) {
        // !!! вместо равномерного по энергии — логарифмически смещённое распределение
        double E = std::pow(gRandom->Uniform(), 2.0) * Emax;  // больше малых энергий
        double y = BremsIntensity(E);
        double yTest = gRandom->Uniform(0, yMax);
        if (yTest < y) return E;
    }
}

// --- генерация полного спектра ---
double SampleXRayAg() {
    double r = gRandom->Rndm();
    if (r < 0.05)
        return gRandom->Gaus(Ka, 0.25);   // Kα
    else if (r < 0.06)
        return gRandom->Gaus(Kb, 0.25);   // Kβ
    else
        return SampleBremsstrahlung();
}


void XRay(){
	TH1D* h = new TH1D("hSpec", "X-ray Spectrum;Energy [keV];Counts", 1000, 0, 60);

    int N = 2000000;
    for (int i = 0; i < N; ++i){
        double E = SampleXRayAg();
        if (E > 0 && E < 55)
			h -> Fill(E);
	}

	TCanvas* c1 = new TCanvas("c1", "Electron energy spectrum", 2426, 1433);
	h -> SetLineColor(kRed);
	h -> SetLineWidth(2);
	h -> SetFillColor(kYellow);
	h -> SetFillStyle(3003);
	h -> SetMarkerStyle(20);
	h -> SetMarkerColor(kBlue);
	
	h -> SetTitle("Electron energy spectrum");
	h -> GetXaxis() -> SetTitle("E, keV");
	h -> GetYaxis() -> SetTitle("Count");
	h -> SetStats(0);
	// gPad -> SetLogy();
	h -> Draw();

	gStyle -> SetOptStat("nemri");

	c1 -> SetGrid();
	c1 -> SaveAs("x_ray.png");

}
