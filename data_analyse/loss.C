void LightLoss(void){

    TFile *inputFile = new TFile("output_08.09.2025_01:20:39.root", "READ");
    const int n = 79;
    double length[n];
    double photons_count[n];
    for(int i = 1; i < n + 1; i++){
        auto h = (TH1D*)inputFile -> Get(std::to_string(i).c_str());
        int count = h -> GetEntries();
        length[i - 1] = 1 * i;
        photons_count[i - 1] = count;
        std::cout << count << std::endl;
    }
    double error_length[n] = {0};
	double error_count[n];
	for(int i = 0; i < n; i++)
        error_count[i] = sqrt(photons_count[i]);

	auto c1 = new TCanvas("c1", "Number of photons depending on fiber length", 2426, 1433);
    auto graph = new TGraphErrors(n, length, photons_count, error_length, error_count);
    graph -> SetTitle("Number of photons depending on fiber length");
    graph -> SetMarkerStyle(20);
    graph -> SetMarkerSize(1);
	graph -> SetLineWidth(2);
    graph -> SetMarkerColor(kBlue);
    graph -> SetLineColor(kRed);
	graph -> GetXaxis() -> SetTitle("Length, cm");
	graph -> GetYaxis() -> SetTitle("Number of photons");
    graph -> Draw("AP");
    
	c1 -> SetGrid();
    c1 -> SaveAs("number_of_photons_depending_on_legth.png");
}