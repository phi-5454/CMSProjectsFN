// Original Author:  Robert Ciesielski
// modified by Jan Kašpar
// and by Frici Nemes

// system include files
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "UserCode/EnergyLossPID/interface/DataHandler.h"
#include "UserCode/EnergyLossPID/interface/MostProbable.h"
#include "UserCode/EnergyLossPID/interface/ParticleType.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TCanvas.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TMath.h"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

// PPS
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"

// PFCandidates
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

// optics

const double v_x_R_1_F = -2.24791387053766;
const double L_x_R_1_F = 0.125396407127792E3;
const double v_y_R_1_F = +0.025781593410852;
const double L_y_R_1_F = 238.517247191010E3;
const double v_x_R_2_F = -1.92610996810677;
const double L_x_R_2_F = -3.00655323980445E3;
const double v_y_R_2_F = -0.000000021508565;
const double L_y_R_2_F = 271.511335947517E3;

const double v_x_L_1_F = -2.1964E+000;
const double L_x_L_1_F = 3.1581E+003;
const double v_y_L_1_F = +2.5777E-002;
const double L_y_L_1_F = 2.3834E+005;
const double v_x_L_2_F = -1.8739E+000;
const double L_x_L_2_F = -4.9261E+002;
const double v_y_L_2_F = +2.7884E-010;
const double L_y_L_2_F = 2.7157E+005;

// Matched optics

const double v_x_R_1_F_matched = -2.24546659;
const double L_x_R_1_F_matched = 194.31957649;
const double v_y_R_1_F_matched = 0.01851263;
const double L_y_R_1_F_matched = 238326.01074279;
const double v_x_R_2_F_matched = -1.92329688;
const double L_x_R_2_F_matched = -2950.79562493;
const double v_y_R_2_F_matched = -0.00829497;
const double L_y_R_2_F_matched = 271314.32190238;

const double v_x_L_1_F_matched = -2.20419654;
const double L_x_L_1_F_matched = 3104.19818816;
const double v_y_L_1_F_matched = 0.03239461;
const double L_y_L_1_F_matched = 238218.46794102;
const double v_x_L_2_F_matched = -1.88422776;
const double L_x_L_2_F_matched = -522.47623562;
const double v_y_L_2_F_matched = 0.00750900;
const double L_y_L_2_F_matched = 271324.15847026;

const double m_pi = 0.13957;
const double m_k = 0.493677;
const double m_p = 0.93827;

using namespace edm;
using namespace reco;
using namespace std;

//---------------------------------------------------------------------------------------------------

class PromptAnalyzer : public edm::one::EDFilter<> {
public:
  explicit PromptAnalyzer(const edm::ParameterSet &);
  ~PromptAnalyzer() {}

  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  virtual void beginJob() override;
  virtual bool filter(edm::Event &, const edm::EventSetup &) override;
  virtual void endJob() override;

  edm::EDGetTokenT<reco::TrackCollection> trkToken_;
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<reco::PFCandidateCollection> pfToken_;

  edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> rpRecHitToken_;
  edm::EDGetTokenT<edm::DetSetVector<TotemRPUVPattern>> rpPatternToken_;
  edm::EDGetTokenT<vector<CTPPSLocalTrackLite>> rpTrackToken_;

  std::string outputFileName;
};

//----------------------------------------------------------------------------------------------------

const int massbins4 = 2000;

PromptAnalyzer::PromptAnalyzer(const edm::ParameterSet &iConfig)
    : trkToken_(consumes<reco::TrackCollection>(
          iConfig.getParameter<edm::InputTag>("tracks"))),
      rpTrackToken_(consumes<vector<CTPPSLocalTrackLite>>(
          iConfig.getParameter<edm::InputTag>("rpTrackTag"))),

      outputFileName(iConfig.getParameter<std::string>("outputFileName")) {}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::beginJob() {}

void PromptAnalyzer::endJob() {
  if (outputFileName == "")
    return;
}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::fillDescriptions(
    edm::ConfigurationDescriptions &descriptions) {
  // Please change this to state exactly what you do use, even if it is no
  // parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool PromptAnalyzer::filter(edm::Event &iEvent, const edm::EventSetup &iSetup) {
  // defaults
  bool returnStatus = false;

  // get input
  edm::Handle<TrackCollection> tracks;
  iEvent.getByToken(trkToken_, tracks);

  edm::Handle<vector<CTPPSLocalTrackLite>> hRPTracks;
  iEvent.getByToken(rpTrackToken_, hRPTracks);

  TLorentzVector pi4Rec(0., 0., 0., 0.);
  int totcharge = 0;

  //----------------------------------------------------------------------
  // process tracks

  for (TrackCollection::const_iterator itTrack = tracks->begin();
       itTrack != tracks->end(); ++itTrack) {
    double pt = itTrack->pt();
    double pz = itTrack->pz();
    int charge = itTrack->charge();
    totcharge += charge;

    const double ene_pi = sqrt((pt * pt) + (pz * pz) + (m_pi * m_pi));
    const TLorentzVector trk_lorentz_pi(itTrack->px(), itTrack->py(),
                                        itTrack->pz(), ene_pi);

    pi4Rec += trk_lorentz_pi;
  }

  //--------------------------------------------------------------------------------
  // process RP data

  // 2018 setup
  //------------------------------------------------
  // -z                    IP               +z
  //         sec45                  sec56
  // top:  24       4           104         124
  // ver:     23  3                 103 123
  // bot:  25       5           105         125
  //

  bool rp_valid_004 = false;
  bool rp_valid_005 = false;
  bool rp_valid_024 = false;
  bool rp_valid_025 = false;

  bool rp_valid_104 = false;
  bool rp_valid_105 = false;
  bool rp_valid_124 = false;
  bool rp_valid_125 = false;

  double xLN = 100, xLF = 100, yLN = 100, yLF = 100;
  double xRN = 100, xRF = 100, yRN = 100, yRF = 100;

  // T, from L to R, as on Jan's slides
  const double mean_x24 = -0.465;
  const double mean_x4 = -0.210;
  const double mean_x104 = 0.167;
  const double mean_x124 = -0.450;

  // B, from L to R
  const double mean_x25 = -0.081;
  const double mean_x5 = -0.112;
  const double mean_x105 = 0.373;
  const double mean_x125 = -0.574;

  // T, from L to R
  const double mean_y24 = -0.689;
  const double mean_y4 = -1.479;
  const double mean_y104 = -0.916;
  const double mean_y124 = 0.044;

  // B, from L to R
  const double mean_y25 = 0.009;
  const double mean_y5 = 0.842;
  const double mean_y105 = 1.312;
  const double mean_y125 = 0.316;

  // process track data
  for (const auto &tr : *hRPTracks) {
    CTPPSDetId rpId(tr.getRPId());
    unsigned int rpDecId =
        (100 * rpId.arm()) + (10 * rpId.station()) + (1 * rpId.rp());

    if (rpDecId == 4) {
      rp_valid_004 = true;
      xLN = tr.getX() + mean_x4;
      yLN = tr.getY() + mean_y4;
    }
    if (rpDecId == 5) {
      rp_valid_005 = true;
      xLN = tr.getX() + mean_x5;
      yLN = tr.getY() + mean_y5;
    }

    if (rpDecId == 24) {
      rp_valid_024 = true;
      xLF = tr.getX() + mean_x24;
      yLF = tr.getY() + mean_y24;
    }
    if (rpDecId == 25) {
      rp_valid_025 = true;
      xLF = tr.getX() + mean_x25;
      yLF = tr.getY() + mean_y25;
    }

    if (rpDecId == 104) {
      rp_valid_104 = true;
      xRN = tr.getX() + mean_x104;
      yRN = tr.getY() + mean_y104;
    }
    if (rpDecId == 105) {
      rp_valid_105 = true;
      xRN = tr.getX() + mean_x105;
      yRN = tr.getY() + mean_y105;
    }

    if (rpDecId == 124) {
      rp_valid_124 = true;
      xRF = tr.getX() + mean_x124;
      yRF = tr.getY() + mean_y124;
    }
    if (rpDecId == 125) {
      rp_valid_125 = true;
      xRF = tr.getX() + mean_x125;
      yRF = tr.getY() + mean_y125;
    }
  }

  bool diag_top45_bot56 =
      (rp_valid_024 && rp_valid_004 && rp_valid_105 && rp_valid_125);
  bool diag_bot45_top56 =
      (rp_valid_025 && rp_valid_005 && rp_valid_104 && rp_valid_124);

  bool top45_top56 =
      (rp_valid_024 && rp_valid_004 && rp_valid_104 && rp_valid_124);
  bool bot45_bot56 =
      (rp_valid_025 && rp_valid_005 && rp_valid_105 && rp_valid_125);

  // topology: 1 - TB, 2 - BT, 3 - TT, 4 - BB

  // single-arm kinematics reconstruction
  double ThxR, ThyR, ThxL, ThyL, xVtxL, xVtxR;

  double D_x_L = +v_x_L_1_F * L_x_L_2_F - v_x_L_2_F * L_x_L_1_F;
  ThxL = (v_x_L_1_F * xLF - v_x_L_2_F * xLN) / D_x_L;
  // xVtxL = (- xLN * L_x_L_2_F + xLF * L_x_L_1_F) / D_x_L;

  double D_x_R = +v_x_R_1_F * L_x_R_2_F - v_x_R_2_F * L_x_R_1_F;
  ThxR = (v_x_R_1_F * xRF - v_x_R_2_F * xRN) / D_x_R;
  // xVtxR = -(+ xRN * L_x_R_2_F - xRF * L_x_R_1_F) / D_x_R;

  double th_y_L_1_F = +yLN / L_y_L_1_F;
  double th_y_L_2_F = +yLF / L_y_L_2_F;
  ThyL = (th_y_L_1_F + th_y_L_2_F) / 2.;

  double th_y_R_1_F = +yRN / L_y_R_1_F;
  double th_y_R_2_F = +yRF / L_y_R_2_F;
  ThyR = (th_y_R_1_F + th_y_R_2_F) / 2.;

  double D_y_L = +v_y_L_1_F * L_y_L_2_F - v_y_L_2_F * L_y_L_1_F;
  double D_y_R = +v_y_R_1_F * L_y_R_2_F - v_y_R_2_F * L_y_R_1_F;

  double yVtxL = (-yLN * L_y_L_2_F + yLF * L_y_L_1_F) / D_y_L;
  double yVtxR = (+yRN * L_y_R_2_F - yRF * L_y_R_1_F) / D_y_R;

  double a_off = 0.000002386;  // TB
  double b_off = -0.000006593; // BT
  double c_off = -0.000007524; // TT
  double d_off = 0.000003268;  // BB

  bool isElastic = false;

  // Remove premature cut from elastic scattering
  // if((TMath::Abs(ThyL+ThyR)< 15e-6) && (TMath::Abs(ThxL+ThxR)<45e-6))
  // isElastic = true ;

  if (isElastic)
    return returnStatus;

  // plots for 4 tracks sample

  const double CMSpx4 = pi4Rec.Px();
  const double CMSpy4 = pi4Rec.Py();

  // CMS-TOTEM matching

  const double beam_mom_GeV = 6500.0;

  double TOTEMpy = beam_mom_GeV * (ThyL + ThyR);
  double TOTEMpx = -beam_mom_GeV * (ThxL + ThxR);

  bool CTpxcut4 = TMath::Abs(CMSpx4 + TOTEMpx) < 0.13;
  bool CTpycut4 = TMath::Abs(CMSpy4 + TOTEMpy) < 0.06;

  bool allCuts4 = (CTpxcut4 && CTpycut4);

  if (allCuts4) {
    // Your code
  }

  return returnStatus;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PromptAnalyzer);
