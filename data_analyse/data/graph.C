void WLDistribution() {
    TFile *inputFile = new TFile("output_21.07.2025_23:45:19_tube.root", "READ");
    TH1D *h = (TH1D*)inputFile -> Get("WL");

	TCanvas* c1 = new TCanvas("c1", "Photons number per event distribution", 2426, 1433);
	h -> SetLineColor(kRed);
	h -> SetLineWidth(2);
	h -> SetFillColor(kYellow);
	h -> SetFillStyle(3003);
	h -> SetMarkerStyle(20);
	h -> SetMarkerColor(kBlue);
	
	h -> SetTitle("Photon wavelength distribution (tube configuration)");
	h -> GetXaxis() -> SetTitle("#lambda, nm");
	h -> GetYaxis() -> SetTitle("Count");
	h -> SetStats(0);
	h -> Draw();

	c1 -> SetGrid();
	c1 -> SaveAs("photon_wl_tube.png");
}
