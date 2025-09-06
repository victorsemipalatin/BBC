void LightLoss(void){
    const int n = 10;
    double length[n] = {5, 6, 7, 8, 9, 10, 20, 30, 40, 90};
    double photons_count[n] = {6829, 6869, 6833, 6730, 6650, 6737, 6587, 6340, 6193, 5529};
    double error_length[n];
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