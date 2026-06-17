#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH1D.h>

void PlotRecoilHistogram() {
    // 1. Define the histogram
    // Arguments: Name, Title, Number of Bins, X-min, X-max
    // Adjust 100, 0, 100 to fit your expected energy range
    TH1D *h1 = new TH1D("h1", "Recoil Energy Distribution;Energy (keV);Counts", 100, 0, 10);

    // 2. Open the text file
    std::ifstream file("recoil_data.txt");
    double value;

    if (!file.is_open()) {
        std::cout << "Error: Could not open file!" << std::endl;
        return;
    }

    // 3. Read values and fill the histogram
    while (file >> value) {
        h1->Fill(value);
    }
    file.close();

    // 4. Create Canvas and Draw
    TCanvas *c1 = new TCanvas("c1", "Histogram Canvas", 800, 600);
    c1->SetGrid();

    // Set aesthetics
    h1->SetFillColor(kBlue-7);
    h1->SetLineColor(kBlue+2);
    h1->SetLineWidth(2);

    h1->Draw();

    // Optional: Log scale if the distribution has a long tail
    // c1->SetLogy();

    c1->Update();
}