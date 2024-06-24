#ifndef _minievent_h_
#define _minievent_h_

#include "TTree.h"

struct MiniEvent_t
{
  MiniEvent_t()
  {
    ntrk=0;
  }

  static const int MAXTRACKS     =  400;
  static const int MAXGENTRACKS  =  1000;  
  static const int MAXPROTONS    =  4;

  Bool_t isData;
  UInt_t run,lumi;
  ULong64_t event;
  
  // Vertex info
  Int_t nvtx;
  Float_t zPV, xPV, yPV;

   
  //track info
  Int_t ntrk, trk_nMeasure[MAXTRACKS], trk_nSaturMeasure[MAXTRACKS], trk_nMeasureLayer[MAXTRACKS];
  Int_t trk_q[MAXTRACKS], trk_isPi[MAXTRACKS], trk_isK[MAXTRACKS], trk_isP[MAXTRACKS];
  Float_t trk_p[MAXTRACKS], trk_perr[MAXTRACKS], trk_pt[MAXTRACKS], trk_pterr[MAXTRACKS], trk_eta[MAXTRACKS], trk_phi[MAXTRACKS], trk_dedx[MAXTRACKS], trk_dedxerr[MAXTRACKS];
  Float_t trk_dxy[MAXTRACKS], trk_dz[MAXTRACKS], trk_dxyerr[MAXTRACKS], trk_dzerr[MAXTRACKS];
  
  // generator level particles
  Int_t ngentrk;
  Int_t gentrk_id[MAXGENTRACKS];
  Float_t gentrk_pt[MAXGENTRACKS], gentrk_pz[MAXGENTRACKS], gentrk_eta[MAXGENTRACKS], gentrk_phi[MAXGENTRACKS], gentrk_m[MAXGENTRACKS];  
  
  Int_t alltrk_q;
  Float_t alltrk_pt, alltrk_mass;

  //TOTEM protons
  Float_t ThxR, ThyR, ThxL, ThyL, xLN, xLF, xRN, xRF, yLN, yLF, yRN, yRF, xVtxR, xVtxL, yVtxL, yVtxR;
};

void createMiniEventTree(TTree *t,MiniEvent_t &ev);
void attachToMiniEventTree(TTree *t, MiniEvent_t &ev);

#endif
