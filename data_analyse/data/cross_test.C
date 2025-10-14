void draw_colormap_from_file() {
    ifstream infile("file_3.txt");
    if (!infile.is_open()) {
        printf("error\n");
        return;
    }

    int nBinsX = 65;
    int nBinsY = 58;
    TH2F *h2 = new TH2F("h2", "Number of photons depending on hit position", nBinsX, -59, 0, nBinsY, 0, 66);

    double x, y, z;
    while (infile >> x >> y >> z) {
        // int binX = h2 -> GetXaxis() -> FindBin(x);
        // int binY = h2 -> GetYaxis() -> FindBin(y);
        // h2 -> SetBinContent(binX, binY, z);
        h2 -> Fill(-y, x, z);
    }
    infile.close();

    TCanvas *c1 = new TCanvas("c1", "Colormap", 2426, 1433);
    c1 -> SetRightMargin(0.18);
    gStyle -> SetOptStat(0);
    h2 -> Draw("COLZ");

    // TPolyLine *poly = new TPolyLine(5);
    // poly -> SetPoint(0, 0, 0);
    // poly -> SetPoint(1, -55.61, 0);
    // poly -> SetPoint(2, -55.61, 64.26);
    // poly -> SetPoint(3, 0, 53.19);
    // poly -> SetPoint(4, 0, 0);
    // poly -> SetLineColor(kRed);
    // poly -> SetLineStyle(2);
    // poly -> SetLineWidth(3);
    // poly -> Draw("SAME");

    h2 -> GetXaxis() -> SetTitle("X, mm");
    h2 -> GetYaxis() -> SetTitle("Y, mm");
    h2 -> GetZaxis() -> SetTitle("Number of photons");
    c1 -> SaveAs("res_4.png");
}

