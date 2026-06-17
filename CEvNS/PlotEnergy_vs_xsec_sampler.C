#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>

void PlotEnergy_vs_xsec_sampler() {
// 1. Create a canvas for the plot
    TCanvas *c1 = new TCanvas("c1", "Recoil Energy vs Differential Cross Section", 800, 600);
    c1->SetGrid();
    // --- ACTIVATE LOG SCALE HERE ---
    c1->SetLogy(1); 
    // -------------------------------

    // 2. TMultiGraph allows you to manage multiple graphs and auto-scale axes
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle("Recoil Energy vs Differential Cross Section (Sampler);Energy (keV);Differential Cross Section (fm^{2})");

    // Define file names and visual preferences
    const char* files[] = {"accepted_points_Bi.txt", "accepted_points_Ge.txt", "accepted_points_BGO_O.txt"};
    const char* labels[] = {" Bismuth", " Germanium", " Oxygen"};
    int colors[] = {kRed, kBlue, kGreen+2};
    // int markers[] = {20, 21, 22}; // Circle, Square, Triangle
    float markerSize [] = {0.1, 0.3, 0.5};

    // 3. Create a Legend
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);

    // 4. Loop through files and load data
    for (int i = 0; i < 3; ++i) {
        TGraph *gr = new TGraph(files[i]);
        
        if (gr->GetN() == 0) {
            std::cout << "Warning: File " << files[i] << " not found or empty!" << std::endl;
            continue;
        }

        // Set aesthetics
        gr->SetLineColor(colors[i]);
        gr->SetMarkerColor(colors[i]);
        // gr->SetMarkerStyle(markers[i]);
        gr->SetMarkerSize(markerSize[i]);
        gr->SetLineWidth(2);
        gr->SetTitle(labels[i]);

        mg->Add(gr, "P"); // "LP" means draw Line and Points
        leg->AddEntry(gr, labels[i], "l");
    }

    // 5. Draw everything
    mg->GetXaxis()->SetRangeUser(0, 90); // Adjust range as needed
    mg->GetYaxis()->SetRangeUser(1e-11, 1e-4); // Adjust range as needed
    mg->Draw("AP"); // 'A' is required for the first draw to create axes
    leg->Draw();

    // Optional: Set Log scale if your data spans many orders of magnitude
    // c1->SetLogy();
    // c1->SetLogx();

    c1->Update();
}