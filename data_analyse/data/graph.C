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


struct SpectrumPoint {
    double E;
    double counts; // логарифмическое значение
};

double fastRand() {
    static unsigned long long seed = 88172645463325252ull;
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return (seed % 1000000) / 1000000.0;
}

// --- Линейная интерполяция ---
double linearInterp(double x, double x1, double y1, double x2, double y2) {
    if (x2 == x1) return y1;
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}

// --- Построение интегральной функции распределения ---
std::vector<double> buildCDF(const std::vector<SpectrumPoint>& spectrum, int nSteps = 2000, double scale = 0.12) {
    double Emin = spectrum.front().E;
    double Emax = spectrum.back().E;
    double step = (Emax - Emin) / nSteps;

    std::vector<double> cdf(nSteps + 1);
    double integral = 0.0;

    for (int i = 0; i <= nSteps; ++i) {
        double E = Emin + i * step;

        auto it = std::upper_bound(spectrum.begin(), spectrum.end(), E,
            [](double value, const SpectrumPoint& p) { return value < p.E; });

        double y;
        if (it == spectrum.begin())
            y = std::exp(scale * it->counts);
        else if (it == spectrum.end())
            y = std::exp(scale * (it - 1)->counts);
        else {
            auto p1 = it - 1;
            auto p2 = it;
            double y1 = std::exp(scale * p1->counts);
            double y2 = std::exp(scale * p2->counts);
            y = linearInterp(E, p1->E, y1, p2->E, y2);
        }

        integral += y * step;
        cdf[i] = integral;
    }

    // Нормировка
    for (auto& v : cdf)
        v /= integral;

    return cdf;
}

double randomEnergy(const std::vector<SpectrumPoint>& spectrum, const std::vector<double>& cdf, int nSteps = 2000) {
    double r = fastRand();
    double Emin = spectrum.front().E;
    double Emax = spectrum.back().E;
    double step = (Emax - Emin) / nSteps;

    auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
    int idx = std::distance(cdf.begin(), it);
    if (idx <= 0) return Emin;
    if (idx >= (int)cdf.size()) return Emax;

    double c1 = cdf[idx - 1];
    double c2 = cdf[idx];
    double E1 = Emin + (idx - 1) * step;
    double E2 = Emin + idx * step;

    double E = E1 + (E2 - E1) * (r - c1) / (c2 - c1);
    return E;
}


void XRay(){
	TH1D* h = new TH1D("hSpec", "X-ray Spectrum;Energy [keV];Counts", 1000, 0, 60);

    std::vector<SpectrumPoint> spectrum = {
        {0,0},{2,0},{3.6,62},{4,66},{6,83.5},{8,88.3},{10,88.3},{12,87.5},
        {14,86.3},{16,85},{18,84},{20,83},{21,83},{21.5,85},{22.1,102},
        {22.6,92},{23,82},{24.5,82},{25,90},{26,80},{28,79},{30,78},{32,77},
        {34,76},{36,75},{38,74},{40,73},{42,72},{44,71},{46,69},{48,65},{49,60},{50,44}
    };

    double scale = 0.12; // ← ключевой параметр: "насколько логарифмический спектр распрямляем"
    int nSteps = 200000;

    auto cdf = buildCDF(spectrum, nSteps, scale);

    for (int i = 0; i < nSteps; ++i){
        double E = randomEnergy(spectrum, cdf, nSteps);
		h -> Fill(E);
	}

	TCanvas* c1 = new TCanvas("c1", "Electron energy spectrum", 2426, 1433);
	h -> SetLineColor(kRed);
	h -> SetLineWidth(2);
	h -> SetFillColor(kYellow);
	h -> SetFillStyle(3003);
	h -> SetMarkerStyle(20);
	h -> SetMarkerColor(kBlue);
	
	h -> SetTitle("Mini-X Silver (Ag) X-Ray Tube Output Spectrum");
	h -> GetXaxis() -> SetTitle("E, keV");
	h -> GetYaxis() -> SetTitle("Count");
	h -> SetStats(0);
	// gPad -> SetLogy();
	h -> Draw();

	gStyle -> SetOptStat("nemri");

	c1 -> SetGrid();
	c1 -> SaveAs("x_ray.png");

}


void gammaFlight() {
    ifstream infile("gamma_test_2.txt");
    if (!infile.is_open()) {
        printf("error\n");
        return;
    }

    int nBinsX = 122 * 2;
    int nBinsY = 138 * 2;
    TH2F *h2 = new TH2F("h2", "Registered gamma quanta", nBinsX, -30, 30, nBinsY, 40, 109);

    double x, y, z;
    while (infile >> x >> y >> z) {
        // int binX = h2 -> GetXaxis() -> FindBin(x);
        // int binY = h2 -> GetYaxis() -> FindBin(y);
        // h2 -> SetBinContent(binX, binY, z);
        h2 -> Fill(x, y, z);
    }
    infile.close();

    TCanvas *c1 = new TCanvas("c1", "Colormap", 2426, 1433);
    c1 -> SetRightMargin(0.18);
    gStyle -> SetOptStat(0);
    h2 -> Draw("COLZ");

    TPolyLine *poly = new TPolyLine(5);
    auto delta = 47.;
    poly -> SetPoint(0, -17.9 / 2, delta);
    poly -> SetPoint(1, -39.66 / 2, delta + 54.7);
    poly -> SetPoint(2, 39.66 / 2,  delta + 54.7);
    poly -> SetPoint(3, 17.9 / 2, delta);
    poly -> SetPoint(4, -17.9 / 2, delta);
    poly -> SetLineColor(kRed);
    poly -> SetLineStyle(2);
    poly -> SetLineWidth(3);
    poly -> Draw("SAME");

    h2 -> GetXaxis() -> SetTitle("X, mm");
    h2 -> GetYaxis() -> SetTitle("Y, mm");
    h2 -> GetZaxis() -> SetTitle("Number of #gamma-quanta");
    c1 -> SaveAs("gamma_flight_3.png");
}

