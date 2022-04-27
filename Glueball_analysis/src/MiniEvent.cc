#include "Projects/Glueball_analysis/interface/MiniEvent.h"
#include <iostream>

//
void createMiniEventTree(TTree *t,MiniEvent_t &ev)
{

  //event header
  t->Branch("isData",    &ev.isData,   "isData/O");
  t->Branch("run",       &ev.run,      "run/i");
  t->Branch("lumi",      &ev.lumi,     "lumi/i");
  t->Branch("event",     &ev.event,    "event/l");

  t->Branch("ntrk",      &ev.ntrk,      "ntrk/I");
  t->Branch("trk_p",        ev.trk_p,         "trk_p[ntrk]/F");
  t->Branch("trk_pt",       ev.trk_pt,        "trk_pt[ntrk]/F");
  t->Branch("trk_eta",      ev.trk_eta,       "trk_eta[ntrk]/F");
  t->Branch("trk_phi",      ev.trk_phi,       "trk_phi[ntrk]/F");
  t->Branch("trk_dxy",      ev.trk_dxy,       "trk_dxy[ntrk]/F");
  t->Branch("trk_dz",       ev.trk_dz,        "trk_dz[ntrk]/F");
  t->Branch("trk_dedx",     ev.trk_dedx,      "trk_dedx[ntrk]/F");
  t->Branch("trk_q",        ev.trk_q,         "trk_q[ntrk]/I");

  t->Branch("ngentrk",      &ev.ngentrk,      "ngentrk/I");
  t->Branch("gentrk_id",    ev.gentrk_id,     "gentrk_id[ngentrk]/I");
  t->Branch("gentrk_pz",    ev.gentrk_pz,     "gentrk_pz[ngentrk]/F");
  t->Branch("gentrk_pt",    ev.gentrk_pt,     "gentrk_pt[ngentrk]/F");
  t->Branch("gentrk_eta",   ev.gentrk_eta,    "gentrk_eta[ngentrk]/F");
  t->Branch("gentrk_phi",   ev.gentrk_phi,    "gentrk_phi[ngentrk]/F");
  t->Branch("gentrk_m",     ev.gentrk_m,      "gentrk_m[ngentrk]/F");


  t->Branch("alltrk_pt",    &ev.alltrk_pt,    "alltrk_pt/F");
  t->Branch("alltrk_mass",  &ev.alltrk_mass,  "alltrk_mass/F");

  t->Branch("ThxR",    &ev.ThxR,    "ThxR/F");
  t->Branch("ThyR",  &ev.ThyR,  "ThyR/F");
  t->Branch("ThxL",    &ev.ThxL,    "ThxL/F");
  t->Branch("ThyL",  &ev.ThyL,  "ThyL/F");

  t->Branch("zPV",  &ev.zPV,  "zPV/F");
  
}

//
void attachToMiniEventTree(TTree *t,MiniEvent_t &ev)
{

  //event header 
  t->SetBranchAddress("isData",    &ev.isData);
  t->SetBranchAddress("run",       &ev.run);
  t->SetBranchAddress("event",     &ev.event);
  t->SetBranchAddress("lumi",      &ev.lumi);
  
  t->SetBranchAddress("ntrk",        &ev.ntrk);
  t->SetBranchAddress("trk_p",       ev.trk_p);
  t->SetBranchAddress("trk_pt",      ev.trk_pt);
  t->SetBranchAddress("trk_eta",     ev.trk_eta);
  t->SetBranchAddress("trk_phi",     ev.trk_phi);
  t->SetBranchAddress("trk_dxy",     ev.trk_dxy);
  t->SetBranchAddress("trk_dz",      ev.trk_dz);
  t->SetBranchAddress("trk_dedx",    ev.trk_dedx);
  t->SetBranchAddress("trk_q",       ev.trk_q);

  t->SetBranchAddress("ngentrk",        &ev.ngentrk);
  t->SetBranchAddress("gentrk_id",      ev.gentrk_id);
  t->SetBranchAddress("gentrk_pt",      ev.gentrk_pt);
  t->SetBranchAddress("gentrk_pz",      ev.gentrk_pz);
  t->SetBranchAddress("gentrk_eta",     ev.gentrk_eta);
  t->SetBranchAddress("gentrk_phi",     ev.gentrk_phi);
  t->SetBranchAddress("gentrk_m",       ev.gentrk_m);

  t->SetBranchAddress("alltrk_pt",    &ev.alltrk_pt);
  t->SetBranchAddress("alltrk_mass",  &ev.alltrk_mass);
  
  t->SetBranchAddress("ThxR",    &ev.ThxR);
  t->SetBranchAddress("ThyR",    &ev.ThyR);
  t->SetBranchAddress("ThxL",    &ev.ThxL);
  t->SetBranchAddress("ThyL",    &ev.ThyL);

  t->SetBranchAddress("zPV",    &ev.zPV); 
  
}
