void draw_tree() {
    // Open the ROOT file
    TFile *file = TFile::Open("pythia8.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open file 'pythia8.root'" << std::endl;
        return;
    }

    // Get TREE selectively 
    TTree *tree = (TTree*)file->Get("tree");
    if (!tree) {
        std::cerr << "Error: Could not find tree in 'pythia8.root'" << std::endl;
        file->Close();
        return;
    }
    
    TTree *sel_tree = tree->CloneTree(0);
    double m_ee;
    tree->SetBranchAddress("m_ee", &m_ee);
    
    Long64_t entries = tree->GetEntries();
    for(Long64_t i = 0; i < entries; i++) {
    	tree->GetEntry(i);
    	if(m_ee >= 70 && m_ee <= 110) {
    		sel_tree->Fill();
    	}
    }

    // CANVAS
    TCanvas *c1 = new TCanvas("c1", "m_ee Plot", 1980, 1080);

    // DRAW
    sel_tree->Draw("m_ee");

    // SAVE
    c1->SaveAs("m_ee_plot.png");

    // Clean up
    file->Close();
}

