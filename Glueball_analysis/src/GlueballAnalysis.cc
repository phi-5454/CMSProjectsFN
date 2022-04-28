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

  const double m_pi = 0.13957;
  const double m_k = 0.493677;
  const double m_p = 0.93827;
    
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
  int trk_isK[ev.MAXTRACKS], trk_isPi[ev.MAXTRACKS], trk_isP[ev.MAXTRACKS];
  outT->Branch("ntrk",&ev.ntrk,"ntrk/I");
  outT->Branch("trk_p",    ev.trk_p,    "trk_p[ntrk]/F");
  outT->Branch("trk_pt",   ev.trk_pt,   "trk_pt[ntrk]/F");
  outT->Branch("trk_eta",  ev.trk_eta,  "trk_eta[ntrk]/F");
  outT->Branch("trk_phi",  ev.trk_phi,  "trk_phi[ntrk]/F");
  outT->Branch("trk_dedx", ev.trk_dedx, "trk_dedx[ntrk]/F");
  outT->Branch("trk_q",    ev.trk_q,    "trk_q[ntrk]/I");
  outT->Branch("trk_id",   ev.trk_id,   "trk_id[ntrk]/I");
  outT->Branch("alltrk_mass", &ev.alltrk_mass,  "alltrk_mass/F");
  outT->Branch("alltrk_pt",   &ev.alltrk_pt,    "alltrk_pt/F");
  outT->Branch("trk_isPi",    trk_isPi,    "trk_isPi[ntrk]/I");
  outT->Branch("trk_isK",     trk_isK,     "trk_isK[ntrk]/I");
  outT->Branch("trk_isP",     trk_isP,     "trk_isP[ntrk]/I");

    
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
		
	  // Keep only events with 2 or >=4 tracks
	  if(skimtree && !(ev.ntrk==2 || ev.ntrk>=4)) continue;
	  evt_count->Fill(5,1);
	  
	  // compute track ID (https://indico.cern.ch/event/1154003/contributions/4845647/attachments/2433551/4167637/update.pdf):
	  float K[3]={0.0588246, 0.6906, 2.26059}, dK[3]={0.0606806,0.194597,0.347767}, C=2.75, dC=0.15, a[3]={-0.171908, -0.50415, -0.714069};
	  for(int i_trk = 0; i_trk<ev.ntrk; i_trk++){
		  ev.trk_p[i_trk] = ev.trk_pt[i_trk]*cosh(ev.trk_eta[i_trk]); // should be removed in a new production
		  float dedx=ev.trk_dedx[i_trk], p=ev.trk_p[i_trk];
		  trk_isPi[i_trk]=0; trk_isK[i_trk]=0; trk_isP[i_trk]=0;

		  // pion ID:
		  if( (dedx - (C - 2.*dC)) * p > a[0] &&  (((dedx - (C + 2.*dC)) * p - a[0]) * p ) < ( K[0] + 2.*dK[0] ) ){
			  if( (dedx - (C - dC)) * p > a[0] &&  (((dedx - (C + dC)) * p - a[0]) * p ) < ( K[0] + dK[0] ) ) trk_isPi[i_trk]=2;
			  else trk_isPi[i_trk]=1;
		  }

		  // Kaon ID:
		  if( (((dedx - (C - 2.*dC)) * p - a[1]) * p ) > ( K[1] - 2.*dK[1] ) &&  (((dedx - (C + 2.*dC)) * p - a[1]) * p ) < ( K[1] + 2.*dK[1] ) ){
			  if( (((dedx - (C - dC)) * p - a[1]) * p ) > ( K[1] - dK[1] ) &&  (((dedx - (C + dC)) * p - a[1]) * p ) < ( K[1] + dK[1] ) ) trk_isK[i_trk]=2;
			  else trk_isK[i_trk]=1;
		  }


		  // proton ID:
		  if( (((dedx - (C - 2.*dC)) * p - a[2]) * p ) > ( K[2] - 2.*dK[2] ) &&  (((dedx - (C + 2.*dC)) * p - a[2]) * p ) < ( K[2] + 2.*dK[2] ) ){
			  if( (((dedx - (C - dC)) * p - a[2]) * p ) > ( K[2] - dK[2] ) &&  (((dedx - (C + dC)) * p - a[2]) * p ) < ( K[2] + dK[2] ) ) trk_isP[i_trk]=2;
			  else trk_isP[i_trk]=1;
		  }
	  }
	  
	  // Compute invariant mass of all tracks:
	  TLorentzVector pi4Rec(0.,0.,0.,0.);
	  for(int i_trk = 0; i_trk<ev.ntrk; i_trk++){
		  float m = m_pi;
		  if (trk_isP[i_trk]==2) m = m_p;
		  if (trk_isK[i_trk]==2) m = m_k;
		  if (trk_isPi[i_trk]==2) m = m_pi;
          
		  const TLorentzVector trk_lorentz_pi( ev.trk_pt[i_trk]*cos(ev.trk_phi[i_trk]),
                                    		   ev.trk_pt[i_trk]*sin(ev.trk_phi[i_trk]),
											   ev.trk_pt[i_trk]*sinh(ev.trk_eta[i_trk]),
											   sqrt(ev.trk_p[i_trk]*ev.trk_p[i_trk] + m*m)
											  );
	      pi4Rec += trk_lorentz_pi;		  
	  }
	  ev.alltrk_mass = pi4Rec.M();
	  
	  
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
