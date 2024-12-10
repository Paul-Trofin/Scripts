#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
using namespace Pythia8;

int main() {
    Pythia pythia;

    //----- READ LHE -------///
    pythia.readString("Beams:frameType = 4");            
    pythia.readString("Beams:LHEF = unweighted_events.lhe.gz");

    //----- INITIALIZE -----//
    if (!pythia.init()) {
        std::cout << "Pythia initialization failed." << std::endl;
        return 1;
    }

    // ROOT FILE
    TFile *output = new TFile("pythia8.root", "RECREATE");

    // TREE
    TTree *tree = new TTree("tree", "tree");

    double m_ee, m_mm, m_ll, e_pT, e_eta, e_phi, e_bar_pT, mu_pT, mu_bar_pT;
    double nevents = 0, n = 0, ne = 0, ne_bar = 0, nm = 0, nm_bar = 0;

    tree->Branch("m_ee", &m_ee, "m_ee/D");
    tree->Branch("m_mm", &m_mm, "m_mm/D");
    tree->Branch("m_ll", &m_ll, "m_ll/D");
    tree->Branch("e_pT", &e_pT, "e_pT/D");
    tree->Branch("e_eta", &e_eta, "e_eta/D");
    tree->Branch("e_phi", &e_phi, "e_phi/D");
    tree->Branch("e_bar_pT", &e_bar_pT, "e_bar_pT/D");

    //----- FILL TREE -----//
    // EACH EVENT
    while (pythia.next()) {
        nevents++;

        std::vector<Particle> electrons;
        std::vector<Particle> positrons;
        std::vector<Particle> muons;
        std::vector<Particle> antimuons;

        // EACH PARTICLE IN CURRENT EVENT
        for (int i = 0; i < pythia.event.size(); ++i) {
            if (pythia.event[i].isFinal()) {
                n++;

                if (pythia.event[i].id() == 11) {  // Electron
                    electrons.push_back(pythia.event[i]);
                    ne++;
                }

                if (pythia.event[i].id() == -11) {  // Positron
                    positrons.push_back(pythia.event[i]);
                    ne_bar++;
                }

                if (pythia.event[i].id() == 13) {  // Muon
                    muons.push_back(pythia.event[i]);
                    nm++;
                }

                if (pythia.event[i].id() == -13) {  // Antimuon
                    antimuons.push_back(pythia.event[i]);
                    nm_bar++;
                }
            }
        }

        // e- e+ invariant mass m_ee
        for (int i = 0; i < electrons.size(); ++i) {
            for (int j = 0; j < positrons.size(); ++j) {
            	
            	e_pT = electrons[i].pT();
            	e_eta = electrons[i].eta();
            	e_phi = electrons[i].phi();
            	
                double px_tot = electrons[i].px() + positrons[j].px();
                double py_tot = electrons[i].py() + positrons[j].py();
                double pz_tot = electrons[i].pz() + positrons[j].pz();
                double e_tot = electrons[i].e() + positrons[j].e();
                m_ee = sqrt(e_tot * e_tot - px_tot * px_tot - py_tot * py_tot - pz_tot * pz_tot);
                m_ll = m_ee;
                tree->Fill();

				
            }
        }

        // mu- mu+ invariant mass m_mm
        for (int i = 0; i < muons.size(); ++i) {
            for (int j = 0; j < antimuons.size(); ++j) {
                double px_tot = muons[i].px() + antimuons[j].px();
                double py_tot = muons[i].py() + antimuons[j].py();
                double pz_tot = muons[i].pz() + antimuons[j].pz();
                double e_tot = muons[i].e() + antimuons[j].e();
                m_mm = sqrt(e_tot * e_tot - px_tot * px_tot - py_tot * py_tot - pz_tot * pz_tot);
		m_ll = m_mm;
		tree->Fill();
                
            }
        }
    }
    
    std::cout << "\n//////////////////////////////" << "\n";
    std::cout << "///////// MY OUTPUT //////////" << "\n";
    std::cout << "//////////////////////////////" << "\n\n";
    std::cout << "nevents = " << nevents << "\n";
    std::cout << "n = " << n << "\n";
    std::cout << "ne = " << ne << "\n";
    std::cout << "ne_bar = " << ne_bar << "\n";
    std::cout << "nm = " << nm << "\n";
    std::cout << "nm_bar = " << nm_bar << "\n";

    output->Write();
    output->Close();
    

    std::cout << "\nThe file pythia8.root has been created.\n";
    return 0;
}

