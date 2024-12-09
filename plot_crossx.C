//////////////////////////////////////////////////////////////////////
//------------ PLOT CROSS SECTION (PB) VS. CM ENERGY (GeV) --------///
//////////////////////////////////////////////////////////////////////
//--------------------------- FIT OPTIONS --------------------------//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <TGraphErrors.h>
#include <TCanvas.h>

using namespace std;

void plot_crossx() {
    //////////////////////////////////////
	//------ FIT FUNCTIONS OPTIONS -----//
	//////////////////////////////////////
    int fit_option = 2; // 0 - DEFAULT GAUSSIAN // 1 - INVERSE_SQUARE // 2 - BREIT-WIGNER // 3 -INV_SQR + B-W
	
    // READ FILE
    ifstream infile("crossx_energy.txt");
    
    // OPEN FILE
    if (!infile.is_open()) {
        cout << "ERROR: Unable to open file." << endl;
        return;
    }
    cout << "File opened successfully." << endl;

    // DATA + ERRORS
    double energy, cross_section, error;
    vector<double> energies;
    vector<double> cross_sections;
    vector<double> errors;

    // SKIP FIRST LINE
    string header;
    getline(infile, header);
	
	// GET DATA LINE BY LINE
    string line;
    while (getline(infile, line)) {
        istringstream iss(line);

        // 'SPLIT' LINE AND GET DATA
        if (!(iss >> energy >> cross_section >> error)) {
            cout << "WARNING: Skipping invalid line: " << line << endl;
            continue;
        }

        // PRINT EACH LINE
        cout << "Read: " << energy << " GeV, " << cross_section << " pb, " << error << " pb" << endl;

        // STORE DATA
        energies.push_back(energy);
        cross_sections.push_back(cross_section);
        errors.push_back(error);
    }

    infile.close();

    // CHECK DATA
    if (energies.empty()) {
        cout << "Error: No data read from the file." << endl;
    }
    if(!energies.empty()) {
    	cout << "Data read successfully." << endl;
    }

    // TGRAPH
    int n = energies.size();
    TGraph *graph = new TGraph(n);
	
    for (int i = 0; i < n; ++i) {
        graph->SetPoint(i, energies[i], cross_sections[i]);
    }

    // CHECK TGRAPH
    if (graph->GetN() == 0) {
        cout << "ERROR: No points in the graph." << endl;
    }
    if(graph->GetN() != 0) {
    	cout << "Graph populated successfully with " << graph->GetN() << " points." << endl;
    }

    // CANVAS
    TCanvas *canvas = new TCanvas("c1", "Cross Section vs Energy", 800, 600);
    graph->SetTitle("e- e+ > all all;Energy (GeV);Cross Section (pb)");
    graph->SetMarkerStyle(21);
    graph->SetMarkerSize(0.7);
    graph->Draw("AP");

    // AXES TITLE
    graph->GetXaxis()->SetTitle("Energy (GeV)");
    graph->GetYaxis()->SetTitle("Cross Section (pb)");
    
    //////////////////////////////////////
	//----------- FIT FUNCTIONS --------//
	//////////////////////////////////////
	
	// TPaveText - WRTIE PARAMETERS
	TPaveText* paveText = new TPaveText(0.6, 0.7, 0.9, 0.9, "NDC");
	paveText->SetBorderSize(1);
	paveText->SetFillColor(0);
	paveText->SetTextSize(0.023);
	
    if(fit_option == 0) {
        // GAUSSIAN
        TF1 *fit_gaus = new TF1("gaus", "gaus", 20, 200);
        fit_gaus->SetLineWidth(5);
        graph->Fit("gaus", "R");
        
    }

    if(fit_option == 1) {
    // INVERSE SQAURE
        TF1 *fit_inv_sqr = new TF1("inv_sqr", "[0] / (x**2)", 20, 200);
        fit_inv_sqr->SetLineWidth(5);
        graph->Fit("inv_sqr", "R");
        
        double p0 = fit_inv_sqr->GetParameter(0);
		double p0_err = fit_inv_sqr->GetParError(0);
		
		paveText->AddText("Fit function: y = p0 / x^2");
		paveText->AddText(Form("p0 = %.2e +- %.2e", p0, p0_err));
		
		paveText->Draw();
    }

    if(fit_option == 2) {
    	// BREIT - WIGNER
        TF1 *fit_bw = new TF1("breit_wigner", "[0] / ((x**2 - [1]**2)**2 + [1]**2 * [2]**2)", 20, 200);
        fit_bw->SetLineWidth(5);
        graph->Fit("breit_wigner", "R");
        
        double p0 = fit_bw->GetParameter(0);
		double p0_err = fit_bw->GetParError(0);
		double p1 = fit_bw->GetParameter(1);
		double p1_err = fit_bw->GetParError(1);
		double p2 = fit_bw->GetParameter(2);
		double p2_err = fit_bw->GetParError(2);
		
		paveText->AddText("Fit function: y = p0 / ((x - p1)^2 + p1^2 * p2^2)");
		paveText->AddText(Form("p0 = %.2e +- %.2e", p0, p0_err));
		paveText->AddText(Form("p1 = %.2e +- %.2e", p1, p1_err));
		paveText->AddText(Form("p2 = %.2e +- %.2e", p2, p2_err));
		
		paveText->Draw();
    }
    
    if(fit_option == 3) {
    	// INVERSE SQUARE + BREIT-WIGNER
        TF1 *fit_bw_inv_sqr = new TF1("breit_wigner_inv_sqr", "[0] / ((x**2 - [1]**2)**2 + [1]**2 * [2]**2) + [3]/x**2", 20, 200);
        fit_bw_inv_sqr->SetLineWidth(3);
        fit_bw_inv_sqr->SetLineColor(kBlue);
        fit_bw_inv_sqr->SetParameters(2.76e9, 91.2, 2.43, 3.96e7);
        graph->Fit("breit_wigner_inv_sqr", "R");
        
        double p0 = fit_bw_inv_sqr->GetParameter(0);
		double p0_err = fit_bw_inv_sqr->GetParError(0);
		double p1 = fit_bw_inv_sqr->GetParameter(1);
		double p1_err = fit_bw_inv_sqr->GetParError(1);
		double p2 = fit_bw_inv_sqr->GetParameter(2);
		double p2_err = fit_bw_inv_sqr->GetParError(2);
		double p3 = fit_bw_inv_sqr->GetParameter(3);
		double p3_err = fit_bw_inv_sqr->GetParError(3);
		
		paveText->AddText("Fit function: y = p0 / ((x - p1)^2 + p1^2 * p2^2) + p3 / x^2");
		paveText->AddText(Form("p0 = %.2e +- %.2e", p0, p0_err));
		paveText->AddText(Form("p1 = %.2e +- %.2e", p1, p1_err));
		paveText->AddText(Form("p2 = %.2e +- %.2e", p2, p2_err));
		paveText->AddText(Form("p2 = %.2e +- %.2e", p3, p3_err));
		
		paveText->Draw();
    }

    // Update canvas
    canvas->Update();
}
