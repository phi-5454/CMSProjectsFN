#include <TFile.h>
#include <TROOT.h>
#include <TH1.h>
#include <TSystem.h>
#include <TGraph.h>
#include <TLorentzVector.h>
#include <TGraphAsymmErrors.h>

#include "Projects/Glueball_analysis/interface/GlueballAnalysis.h"
#include "Projects/Glueball_analysis/interface/MiniEvent.h"

#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include "TMath.h"

using namespace std;

#define ADDVAR(x,name,t,tree) tree->Branch(name,x,TString(name)+TString(t))

void RunGlueballAnalysis(const TString in_fname,
                      TString outname,
                      bool skimtree, 
                      bool debug) 
{

  /////////////////////
  // INITIALIZATION //
  ///////////////////

  //const double m_pi = 0.13957;
  //const double m_k = 0.493677;
  //const double m_p = 0.93827;
    
  //const char* CMSSW_BASE = getenv("CMSSW_BASE");
  MiniEvent_t ev;  

  //READ TREE FROM FILE
  TFile *f = TFile::Open(in_fname);
  if(f==NULL || f->IsZombie()) {
    cout << "Corrupted or missing file " << in_fname << endl;
    return;
  }
  TH1 *counter=(TH1 *)f->Get("analysis/counter");
  if(!counter) {cout << "Corrupted or missing counter: \"analysis/counter\" " << endl;return;}
  TTree *t = (TTree*)f->Get("analysis/tree");
  if(!t) {cout << "Corrupted or missing tree: \"analysis/tree\" " << endl;return;}
  attachToMiniEventTree(t,ev);
  Int_t nentries(t->GetEntriesFast());
  if (debug) nentries = min(1000,nentries); //restrict number of entries for testing
  std::cout << "...producing " << outname << " from " << nentries << " events" << std::endl;  

  //PREPARE OUTPUT (BOOK SOME HISTOGRAMS)
  TString baseName=gSystem->BaseName(outname); 
  TString dirName=gSystem->DirName(outname);
  TFile *fOut=TFile::Open(dirName+"/"+baseName,"RECREATE");
  fOut->cd();

  // BOOK OUTPUT TREE
  TTree *outT=new TTree("tree","tree");
  
  // Event info
  outT->Branch("Run",&ev.run,"Run/i");
  outT->Branch("EventNum",&ev.event,"EventNum/l");
  outT->Branch("LumiSection",&ev.lumi,"LumiSection/i");

  // Tracks
  outT->Branch("ntrk",&ev.ntrk,"ntrk/I");
  outT->Branch("trk_pt",   ev.trk_pt,   "trk_pt[ntrk]/F");
  outT->Branch("trk_eta",  ev.trk_eta,  "trk_eta[ntrk]/F");
  outT->Branch("trk_phi",  ev.trk_phi,  "trk_phi[ntrk]/F");
  outT->Branch("trk_dedx", ev.trk_dedx, "trk_dedx[ntrk]/F");
  outT->Branch("trk_q",    ev.trk_q,    "trk_q[ntrk]/I");
  outT->Branch("alltrk_mass",&ev.alltrk_mass,"alltrk_mass/F");

    
  //BOOK HISTOGRAMS  
  TH1F * evt_count = new TH1F("evt_count",   ";Selection Stage;Events",5,0,5);
  evt_count->GetXaxis()->SetBinLabel(1,"Total");
  evt_count->GetXaxis()->SetBinLabel(2,"nPV=1");
  evt_count->GetXaxis()->SetBinLabel(3,"Veto Elastic");
  evt_count->GetXaxis()->SetBinLabel(4,"CMS-TOTEM matching");
  evt_count->GetXaxis()->SetBinLabel(5,"nTracks=2 OR 4");
  evt_count->SetBinContent(1,counter->GetBinContent(1));
  evt_count->SetBinContent(2,counter->GetBinContent(2));
  evt_count->SetBinContent(3,counter->GetBinContent(3));
  evt_count->SetBinContent(4,counter->GetBinContent(4));

   
  std::cout << "initialization done" << std::endl;

	
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////  LOOP OVER EVENTS  /////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
  for (Int_t iev=0;iev<nentries;iev++)
    {
      t->GetEntry(iev);
      if(iev%10==0) printf ("\r [%3.0f%%] done", 100.*(float)iev/(float)nentries);
		
	  // Keep only events with 2 or 4 tracks
	  if(skimtree && !(ev.ntrk==2 || ev.ntrk==4)) continue;
	  evt_count->Fill(5,1);
	  
	  // Save output tree
	  outT->Fill();
 
    } // END EVENT LOOP
  
  //close input file
  f->Close();
  
  //save histos to file
  cout << endl << "Writes " << fOut->GetName() << " with " << outT->GetEntries() <<  " events." << endl;  
  fOut->cd();
  evt_count->Write(); 
  outT->Write();
  
  fOut->Close();
}
