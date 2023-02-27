// -*- C++ -*-
//
// Package:    /Projects/Glueball_analysis
// Class:      RecoAnalyzer
//
/**\class RecoAnalyzer RecoAnalyzer.cc /Projects/Glueball_analysis/plugins/RecoAnalyzer.cc

 Description: Processing code used in the glueball analysis
 Implementation: consult with the github repository
*/
//         Developed from PromptAnalyzer.cc by Michael Pitt (michael.pitt@cern.ch)
//         Created:  Thu, 20 Jan 2022 08:37:11 GMT
//
//

// system include files
#include <memory>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#include "UserCode/EnergyLossPID/interface/DataHandler.h"
#include "UserCode/EnergyLossPID/interface/MostProbable.h"
#include "UserCode/EnergyLossPID/interface/ParticleType.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TCanvas.h"
#include "TLatex.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

// PPS
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

// PFCandidates
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "Projects/Glueball_analysis/interface/MiniEvent.h"

//
// class declaration
//


// Matched optics (from 20/01/22)
// https://github.com/FrigyesNemes/Projects/blob/master/Glueball_analysis/plugins/PromptAnalyzer.cc

const double v_x_R_1_F_matched = -2.24546659 ; const double L_x_R_1_F_matched = 194.31957649 ;
const double v_y_R_1_F_matched = 0.01851263  ; const double L_y_R_1_F_matched = 238326.01074279 ;
const double v_x_R_2_F_matched = -1.92329688 ; const double L_x_R_2_F_matched = -2950.79562493 ;
const double v_y_R_2_F_matched = -0.00829497 ; const double L_y_R_2_F_matched = 271314.32190238 ;

const double v_x_L_1_F_matched = -2.20419654 ; const double L_x_L_1_F_matched = 3104.19818816 ;
const double v_y_L_1_F_matched = 0.03239461  ; const double L_y_L_1_F_matched = 238218.46794102 ;
const double v_x_L_2_F_matched = -1.88422776 ; const double L_x_L_2_F_matched = -522.47623562 ;
const double v_y_L_2_F_matched = 0.00750900  ; const double L_y_L_2_F_matched = 271324.15847026 ;

// optics
//const double v_x_R_1_F = -2.24791387053766 ;  const double L_x_R_1_F = 0.125396407127792E3 ;
//const double v_y_R_1_F = +0.025781593410852 ; const double L_y_R_1_F = 238.517247191010E3 ;
//const double v_x_R_2_F = -1.92610996810677 ;  const double L_x_R_2_F = -3.00655323980445E3 ;
//const double v_y_R_2_F = -0.000000021508565 ; const double L_y_R_2_F = 271.511335947517E3 ;

//const double v_x_L_1_F =  -2.1964E+000  ;  const double L_x_L_1_F =   3.1581E+003 ;
//const double v_y_L_1_F =  +2.5777E-002  ;  const double L_y_L_1_F =   2.3834E+005 ;
//const double v_x_L_2_F =  -1.8739E+000  ;  const double L_x_L_2_F =  -4.9261E+002 ;
//const double v_y_L_2_F =  +2.7884E-010  ;  const double L_y_L_2_F =   2.7157E+005 ;

const double v_x_R_1_F = v_x_R_1_F_matched;  const double L_x_R_1_F = L_x_R_1_F_matched;
const double v_y_R_1_F = v_y_R_1_F_matched;  const double L_y_R_1_F = L_y_R_1_F_matched;
const double v_x_R_2_F = v_x_R_2_F_matched;  const double L_x_R_2_F = L_x_R_2_F_matched;
const double v_y_R_2_F = v_y_R_2_F_matched;  const double L_y_R_2_F = L_y_R_2_F_matched;

const double v_x_L_1_F =  v_x_L_1_F_matched;  const double L_x_L_1_F = L_x_L_1_F_matched;
const double v_y_L_1_F =  v_y_L_1_F_matched;  const double L_y_L_1_F = L_y_L_1_F_matched;
const double v_x_L_2_F =  v_x_L_2_F_matched;  const double L_x_L_2_F = L_x_L_2_F_matched;
const double v_y_L_2_F =  v_y_L_2_F_matched;  const double L_y_L_2_F = L_y_L_2_F_matched;


const double m_pi = 0.13957;
const double m_k = 0.493677;
const double m_p = 0.93827;

using namespace reco;
using namespace std;
using reco::TrackCollection;

class RecoAnalyzer : public edm::EDAnalyzer  {
   public:
      explicit RecoAnalyzer(const edm::ParameterSet&);
      ~RecoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
	  void genAnalysis(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() override;

	
      // ----------member data ---------------------------
      edm::EDGetTokenT<TrackCollection> tracksToken_;
      edm::EDGetTokenT<DeDxDataValueMap> DeDxDataToken_;
      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;	
	  edm::EDGetTokenT<vector<CTPPSLocalTrackLite>> rpTrackToken_;
	  edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
	  
	  TTree *tree_;
	  MiniEvent_t ev_;
      TH1F *h_counter;
	  	  
	  edm::Service<TFileService> fs;
	  
	  // apply filter
	  bool applyFilt_;	  
	  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RecoAnalyzer::RecoAnalyzer(const edm::ParameterSet& iConfig)
 :
  tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
  DeDxDataToken_(consumes<DeDxDataValueMap>(iConfig.getUntrackedParameter<edm::InputTag>("DeDxData"))),
  vtxToken_(consumes<VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  rpTrackToken_(consumes<vector<CTPPSLocalTrackLite>>(iConfig.getParameter<edm::InputTag>("rpTrackTag"))),
  genParticlesToken_(consumes<GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticles"))),
  applyFilt_( iConfig.getParameter<bool>("applyFilt") )

{
   //now do what ever initialization is needed
   h_counter = fs->make<TH1F>("counter", ";Counter;Events",4,0.5,4.5);
   h_counter->GetXaxis()->SetBinLabel(1,"Total");
   h_counter->GetXaxis()->SetBinLabel(2,"nPV=1");
   h_counter->GetXaxis()->SetBinLabel(3,"Veto Elastic");
   h_counter->GetXaxis()->SetBinLabel(4,"CMS-TOTEM matching");
   tree_ = fs->make<TTree>("tree","tree with selected events");
   createMiniEventTree(tree_,ev_);
}


RecoAnalyzer::~RecoAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    h_counter->Fill(1,1);
    
	ev_.isData  = iEvent.isRealData();
	ev_.run = iEvent.id().run();
	ev_.lumi = iEvent.luminosityBlock();
	ev_.event   = iEvent.id().event();
	
	// VERTICES
	Handle<VertexCollection> vertices;
	iEvent.getByToken(vtxToken_, vertices);

	ev_.nvtx=vertices->size();
	//if (applyFilt_ && ev_.nvtx!=1) return; // use only events with 1 PV
	if (ev_.nvtx!=1) return; // use only events with 1 PV
    h_counter->Fill(2,1);
	
	const reco::Vertex &primVtx = vertices->front();
	reco::VertexRef primVtxRef(vertices,0);
	ev_.zPV = primVtx.z();
	
	// Get track collection
    Handle<TrackCollection> trackCollectionHandle;
    iEvent.getByToken(tracksToken_, trackCollectionHandle);
    TrackCollection trackCollection = *trackCollectionHandle.product();
	
	// Get dE/dx collection
	Handle<DeDxDataValueMap> dEdxTrackHandle;
    iEvent.getByToken(DeDxDataToken_, dEdxTrackHandle);
	DeDxDataValueMap dEdxTrack = *dEdxTrackHandle.product();
	
    Handle<vector<CTPPSLocalTrackLite>> hRPTracks;
    iEvent.getByToken(rpTrackToken_, hRPTracks);
 
 
    TLorentzVector pi4Rec(0.,0.,0.,0.);
	ev_.ntrk = 0;
    ev_.alltrk_q = 0;

    //cout << "Test event " << ev_.event << ", nvtx = " << ev_.nvtx << " , z = " <<  ev_.zPV << endl;
	for(unsigned int i=0; i<trackCollection.size(); i++){
		
		TrackRef track  = TrackRef( trackCollectionHandle, i );

	    ev_.trk_p[i] = track->p() ;
	    ev_.trk_pt[i] = track->pt() ;
	    ev_.trk_eta[i] = track->eta() ;
	    ev_.trk_phi[i] = track->phi() ;
	    ev_.trk_q[i] = track->charge() ;
	    ev_.trk_dxy[i] = track->dxy(primVtx.position()); // < 0.3
	    ev_.trk_dz[i] = track->dz(primVtx.position()); // < 20.
	    ev_.alltrk_q += ev_.trk_q[i] ;
		
		ev_.trk_dedx[i] = dEdxTrack[track].dEdx();
		ev_.trk_dedxerr[i] = dEdxTrack[track].dEdxError();
		ev_.trk_nSaturMeasure[i] = dEdxTrack[track].numberOfSaturatedMeasurements();
		ev_.trk_nMeasure[i] = int(dEdxTrack[track].numberOfMeasurements());
		
		// number of measurements / layer (from HitPatten)
		const HitPattern &p = track->hitPattern();
		ev_.trk_nMeasureLayer[i] = (p.trackerLayersWithMeasurement()); //all layers
		ev_.trk_nMeasureLayer[i] += 1e2*(p.pixelBarrelLayersWithMeasurement()); //PXB
		ev_.trk_nMeasureLayer[i] += 1e3*(p.pixelEndcapLayersWithMeasurement()); //PXF
		ev_.trk_nMeasureLayer[i] += 1e4*(p.stripTIBLayersWithMeasurement());    //TIB
		ev_.trk_nMeasureLayer[i] += 1e5*(p.stripTIDLayersWithMeasurement());    //TID
		ev_.trk_nMeasureLayer[i] += 1e6*(p.stripTOBLayersWithMeasurement());    //TOB
		ev_.trk_nMeasureLayer[i] += 1e7*(p.stripTECLayersWithMeasurement());    //TEC


	    const double ene_pi = sqrt((track->pt() * track->pt()) + (track->pz() * track->pz()) + (m_pi * m_pi)) ;
	    const TLorentzVector trk_lorentz_pi(track->px(), track->py(), track->pz(), ene_pi) ;

	    pi4Rec += trk_lorentz_pi;
		ev_.ntrk++;
    }
	
	
    if(ev_.MAXTRACKS<ev_.ntrk){
       cout << "ERROR: MAXTRACKS ("<<ev_.MAXTRACKS<<") is smaller than the N of tracks ("<<ev_.ntrk<<")."<<endl;
	   cout <<"\t\t... can cause memory leaks or crush!!!"<<endl;
    }
	ev_.alltrk_mass = pi4Rec.M();
	ev_.alltrk_pt = pi4Rec.Pt();

  //--------------------------------------------------------------------------------
  // process RP data

  // 2018 setup
  //------------------------------------------------
  // -z                    IP               +z
  //         sec45                  sec56
  //top:  24       4           104         124
  //ver:     23  3                 103 123
  //bot:  25       5           105         125
  //

  bool rp_valid_004 = false;
  bool rp_valid_005 = false;
  bool rp_valid_024 = false;
  bool rp_valid_025 = false;

  bool rp_valid_104 = false;
  bool rp_valid_105 = false;
  bool rp_valid_124 = false;
  bool rp_valid_125 = false;

  ev_.xLN = 100; ev_.xLF = 100; ev_.yLN = 100; ev_.yLF = 100;
  ev_.xRN = 100; ev_.xRF = 100; ev_.yRN = 100; ev_.yRF = 100;

  // T, from L to R, as on Jan's slides
  const double mean_x24  = -0.465;
  const double mean_x4   = -0.210;
  const double mean_x104 =  0.167;
  const double mean_x124 = -0.450;

  // B, from L to R
  const double mean_x25  = -0.081;
  const double mean_x5   = -0.112;
  const double mean_x105 =  0.373;
  const double mean_x125 = -0.574;

  // T, from L to R
  const double mean_y24  = -0.689;
  const double mean_y4   = -1.479;
  const double mean_y104 = -0.916;
  const double mean_y124 =  0.044;

  // B, from L to R
  const double mean_y25  = 0.009;
  const double mean_y5   = 0.842;
  const double mean_y105 = 1.312;
  const double mean_y125 = 0.316;

	// process track data
	for (const auto &tr : *hRPTracks)
	{
		CTPPSDetId rpId(tr.getRPId());
		unsigned int rpDecId = (100*rpId.arm()) + (10*rpId.station()) + (1*rpId.rp());

		if(rpDecId == 4) 	{rp_valid_004 = true; ev_.xLN = tr.getX() + mean_x4; ev_.yLN = tr.getY() + mean_y4;}
		if(rpDecId == 5) 	{rp_valid_005 = true; ev_.xLN = tr.getX() + mean_x5; ev_.yLN = tr.getY() + mean_y5;}

		if(rpDecId == 24) 	{rp_valid_024 = true; ev_.xLF = tr.getX() + mean_x24; ev_.yLF = tr.getY() + mean_y24;}
		if(rpDecId == 25) 	{rp_valid_025 = true; ev_.xLF = tr.getX() + mean_x25; ev_.yLF = tr.getY() + mean_y25;}

		if(rpDecId == 104) 	{rp_valid_104 = true; ev_.xRN = tr.getX() + mean_x104; ev_.yRN = tr.getY() + mean_y104;}
		if(rpDecId == 105) 	{rp_valid_105 = true; ev_.xRN = tr.getX() + mean_x105; ev_.yRN = tr.getY() + mean_y105;}

		if(rpDecId == 124) 	{rp_valid_124 = true; ev_.xRF = tr.getX() + mean_x124; ev_.yRF = tr.getY() + mean_y124;}
		if(rpDecId == 125) 	{rp_valid_125 = true; ev_.xRF = tr.getX() + mean_x125; ev_.yRF = tr.getY() + mean_y125;}
	}

	bool diag_top45_bot56 = (rp_valid_024 && rp_valid_004 && rp_valid_105 && rp_valid_125);
	bool diag_bot45_top56 = (rp_valid_025 && rp_valid_005 && rp_valid_104 && rp_valid_124);

	bool top45_top56      = (rp_valid_024 && rp_valid_004 && rp_valid_104 && rp_valid_124);
	bool bot45_bot56      = (rp_valid_025 && rp_valid_005 && rp_valid_105 && rp_valid_125);


	// topology: 1 - TB, 2 - BT, 3 - TT, 4 - BB

	// single-arm kinematics reconstruction
	//double xVtxL, xVtxR;

	double D_x_L = + v_x_L_1_F * L_x_L_2_F - v_x_L_2_F * L_x_L_1_F;
	ev_.ThxL = (v_x_L_1_F * ev_.xLF - v_x_L_2_F * ev_.xLN) / D_x_L;
	ev_.xVtxL = (- ev_.xLN * L_x_L_2_F + ev_.xLF * L_x_L_1_F) / D_x_L;

	double D_x_R = + v_x_R_1_F * L_x_R_2_F - v_x_R_2_F * L_x_R_1_F;
	ev_.ThxR = (v_x_R_1_F * ev_.xRF - v_x_R_2_F * ev_.xRN) / D_x_R;
	ev_.xVtxR = -(+ ev_.xRN * L_x_R_2_F - ev_.xRF * L_x_R_1_F) / D_x_R;

	double th_y_L_1_F = + ev_.yLN / L_y_L_1_F;
	double th_y_L_2_F = + ev_.yLF / L_y_L_2_F;
	ev_.ThyL = (th_y_L_1_F + th_y_L_2_F) / 2.;

	double th_y_R_1_F = + ev_.yRN / L_y_R_1_F;
	double th_y_R_2_F = + ev_.yRF / L_y_R_2_F;
	ev_.ThyR = (th_y_R_1_F + th_y_R_2_F) / 2.;

	double D_y_L = + v_y_L_1_F * L_y_L_2_F - v_y_L_2_F * L_y_L_1_F;
	double D_y_R = + v_y_R_1_F * L_y_R_2_F - v_y_R_2_F * L_y_R_1_F;

	ev_.yVtxL = (- ev_.yLN * L_y_L_2_F + ev_.yLF * L_y_L_1_F) / D_y_L;
	ev_.yVtxR = (+ ev_.yRN * L_y_R_2_F - ev_.yRF * L_y_R_1_F) / D_y_R;
  
	double a_off =  0.000002386 ; // TB
	double b_off = -0.000006593 ; // BT
	double c_off = -0.000007524 ; // TT
	double d_off =  0.000003268 ; // BB


	bool isElastic = false;

	if((TMath::Abs(ev_.ThyL+ev_.ThyR)< 15e-6) && (TMath::Abs(ev_.ThxL+ev_.ThxR)<45e-6)) isElastic = true ;
 
 
    if (applyFilt_ && isElastic) return;
	h_counter->Fill(3,1);
	
	// plots for 4 tracks sample

	const double CMSpx4 = pi4Rec.Px();
	const double CMSpy4 = pi4Rec.Py();

	// CMS-TOTEM matching

	const double beam_mom_GeV = 6500.0 ;

	double TOTEMpy =  beam_mom_GeV * (ev_.ThyL+ev_.ThyR);
	double TOTEMpx = -beam_mom_GeV * (ev_.ThxL+ev_.ThxR);

	bool CTpxcut4 = TMath::Abs(CMSpx4+TOTEMpx) < 0.13 ;
	bool CTpycut4 = TMath::Abs(CMSpy4+TOTEMpy) < 0.06 ;

	bool allCuts4 = (CTpxcut4 && CTpycut4) ;
	
	if (applyFilt_ && !allCuts4) return;
	h_counter->Fill(4,1);
	
	// MC get generator info:
	ev_.ngentrk = 0;
	if(!ev_.isData) {
	  edm::Handle<reco::GenParticleCollection> genParticles;
	  iEvent.getByToken(genParticlesToken_,genParticles);
	  for (size_t i = 0; i < genParticles->size(); ++i){
		const reco::GenParticle & genIt = (*genParticles)[i];
		if (genIt.status()!=1) continue; // save only stable particles
		if (genIt.pdgId()==22) continue; // skip photons
	    ev_.gentrk_pz[ev_.ngentrk] = genIt.pz() ;
	    ev_.gentrk_pt[ev_.ngentrk] = genIt.pt() ;
	    ev_.gentrk_eta[ev_.ngentrk] = genIt.eta() ;
	    ev_.gentrk_phi[ev_.ngentrk] = genIt.phi() ;
	    ev_.gentrk_m[ev_.ngentrk] = genIt.mass() ;
	    ev_.gentrk_id[ev_.ngentrk] = genIt.pdgId(); 
		ev_.ngentrk++;
	  }
	}	
    if(ev_.MAXGENTRACKS<ev_.ngentrk){
       cout << "ERROR: MAXGENTRACKS ("<<ev_.MAXGENTRACKS<<") is smaller than the N of tracks ("<<ev_.ngentrk<<")."<<endl;
	   cout <<"\t\t... can cause memory leaks or crush!!!"<<endl;
    }		
	//genAnalysis(iEvent,iSetup);
	
    tree_->Fill();

}

void RecoAnalyzer::genAnalysis(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	edm::Handle<reco::GenParticleCollection> genParticles;
	iEvent.getByToken(genParticlesToken_,genParticles);
	cout << "N particles = " << genParticles->size()<< endl;
	for (size_t i = 0; i < genParticles->size(); ++i){
		const reco::GenParticle & genIt = (*genParticles)[i];
		cout << i << " : " << genIt.pz() << " , " << genIt.pt() << " , "<< genIt.pdgId() << endl;
        
	    ev_.gentrk_pt[i] = genIt.pt() ;
	    ev_.gentrk_eta[i] = genIt.eta() ;
	    ev_.gentrk_phi[i] = genIt.phi() ;
	    ev_.gentrk_m[i] = genIt.mass() ;
	    ev_.gentrk_id[i] = genIt.pdgId(); 
		ev_.ngentrk++;
	}
	cout << "done"<<endl;
}

// ------------ method called once each job just before starting event loop  ------------
void
RecoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
RecoAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecoAnalyzer);
