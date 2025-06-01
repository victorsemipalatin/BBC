void WLDistribution() {
    // Открываем файл с исходной гистограммой (если нужно)
    TFile *inputFile = new TFile("output_01.06.2025_22:44:36.root", "READ");
    TH1D *h_source = (TH1D*)inputFile -> Get("PhotonsEnergy"); // Замените на имя вашей гистограммы
    
    TH1D *h_inverse = new TH1D("h_inverse", "Inverse of Source Histogram", 
                               h_source -> GetNbinsX(), 
                               1.976831468e-25 / 1.6 * 1e19 * 1e9 / h_source -> GetXaxis() -> GetXmax(),
                               1.976831468e-25 / 1.6 * 1e19 * 1e9 / h_source -> GetXaxis() -> GetXmin());

    // Перебираем бины и записываем 1 / bin_content
    int nBins = h_source -> GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        double x = h_source -> GetBinCenter(i);
        double content = h_source -> GetBinContent(i);
        double error = h_source -> GetBinError(i);
        
        if (x != 0) {
            double x_inv = 1.976831468e-25 / 1.6 * 1e19 * 1e9 / x;
            // Находим соответствующий бин в новой гистограмме
            int bin_inv = h_inverse -> FindBin(x_inv);
            h_inverse -> SetBinContent(bin_inv, content);
            h_inverse -> SetBinError(bin_inv, error);
        }
    }

    // Можно также нарисовать гистограммы для проверки
	TCanvas* c1 = new TCanvas("c1", "Photons number per event distribution", 2426, 1433);
	h_inverse -> SetLineColor(kRed);
	h_inverse -> SetLineWidth(2);
	h_inverse -> SetFillColor(kYellow);
	h_inverse -> SetFillStyle(3003);
	h_inverse -> SetMarkerStyle(20);
	h_inverse -> SetMarkerColor(kBlue);
	
	h_inverse -> SetTitle("Photon wavelength distribution");
	h_inverse -> GetXaxis() -> SetTitle("#lambda, nm");
	h_inverse -> GetYaxis() -> SetTitle("Count");
	h_inverse -> SetStats(0);
	h_inverse -> Draw();

	c1 -> SetGrid();
	c1 -> SaveAs("photon_wl_1.png");

    // Сохраняем картинку
    //c1->SaveAs("inverse_histogram.png");
}
