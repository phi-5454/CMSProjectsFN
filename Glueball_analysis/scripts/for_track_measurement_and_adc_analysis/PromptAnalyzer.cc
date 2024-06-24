// Original Author:  Robert Ciesielski
// modified by Jan Kašpar
// and by Frici
// tabulator stops: 3

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
#include "FWCore/Framework/interface/one/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

// dEdx
#include "DataFormats/TrackReco/interface/DeDxHit.h"
#include "DataFormats/TrackReco/interface/DeDxHitInfo.h"
#include "DataFormats/TrackReco/interface/DeDxData.h"

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

/*
// optics
const double v_x_R_1_F = -2.24791387053766 ;  const double L_x_R_1_F = 0.125396407127792E3 ;
const double v_y_R_1_F = +0.025781593410852 ; const double L_y_R_1_F = 238.517247191010E3 ;
const double v_x_R_2_F = -1.92610996810677 ;  const double L_x_R_2_F = -3.00655323980445E3 ;
const double v_y_R_2_F = -0.000000021508565 ; const double L_y_R_2_F = 271.511335947517E3 ;

const double v_x_L_1_F = -2.24791387053766 ;  const double L_x_L_1_F = 0.125396407127792E3 ;
const double v_y_L_1_F = +0.025781593410852 ; const double L_y_L_1_F = 238.517247191010E3 ;
const double v_x_L_2_F = -1.92610996810677 ;  const double L_x_L_2_F = -3.00655323980445E3 ;
const double v_y_L_2_F = -0.000000021508565 ; const double L_y_L_2_F = 271.511335947517E3 ;
*/

// matched optics 2021/12/15

/*
const double v_x_R_1_F  =  -2.23757915 ; const double L_x_R_1_F  =  160.75714027 ;
const double v_y_R_1_F  =   0.03981926 ; const double L_y_R_1_F  =  239432.81562269 ;
const double v_x_R_2_F  =  -1.91153192 ; const double L_x_R_2_F  = -2989.70502218 ;
const double v_y_R_2_F  =   0.01613230 ; const double L_y_R_2_F  =  272722.20799844 ;
           
const double v_x_L_1_F  =  -2.20124313 ; const double L_x_L_1_F  =  3105.89115651 ;
const double v_y_L_1_F  =   0.01292547 ; const double L_y_L_1_F  =  237782.60339454 ;
const double v_x_L_2_F  =  -1.88053761 ; const double L_x_L_2_F  = -528.05554473 ;
const double v_y_L_2_F  =  -0.01472828 ; const double L_y_L_2_F  =  270860.20531844 ;
*/

// matched optics 2022/01/20

const double v_x_R_1_F = -2.24546659 ; const double L_x_R_1_F = 194.31957649 ;
const double v_y_R_1_F = 0.01851263  ; const double L_y_R_1_F = 238326.01074279 ;
const double v_x_R_2_F = -1.92329688 ; const double L_x_R_2_F = -2950.79562493 ;
const double v_y_R_2_F = -0.00829497 ; const double L_y_R_2_F = 271314.32190238 ;
	    			      	 		    
const double v_x_L_1_F = -2.20419654 ; const double L_x_L_1_F = 3104.19818816 ;
const double v_y_L_1_F = 0.03239461  ; const double L_y_L_1_F = 238218.46794102 ;
const double v_x_L_2_F = -1.88422776 ; const double L_x_L_2_F = -522.47623562 ;
const double v_y_L_2_F = 0.00750900  ; const double L_y_L_2_F = 271324.15847026 ;



// optics changes by Ferenc

const double L_x_L_1_F_Ferenc = 2.66E3 ;
const double L_x_L_2_F_Ferenc = -0.83E3 ;

const double L_x_R_1_F_Ferenc = 0.63E3 ;
const double L_x_R_2_F_Ferenc = -2.58E3 ;

// nominal optics Frici

const double v_x_L_1_F_Frici =	-2.1964E+000 ;	const double L_x_L_1_F_Frici =	3.1581E+003 ;
const double v_y_L_1_F_Frici =	2.5777E-002  ;  const double L_y_L_1_F_Frici =	2.3834E+005 ;
const double v_x_L_2_F_Frici =	-1.8739E+000 ;	const double L_x_L_2_F_Frici =	-4.9261E+002 ;
const double v_y_L_2_F_Frici =	2.7884E-010  ;	const double L_y_L_2_F_Frici =	2.7157E+005 ;

const double m_pi = 0.13957;
const double m_k = 0.493677;
const double m_p = 0.93827;

using namespace edm;
using namespace reco;
using namespace std;

// Aux classes

class TVector2D_for_cuts
{
	private:
	
	double x,y ;

	public:

	TVector2D_for_cuts(double, double) ;
	TVector2D_for_cuts(TVector2D_for_cuts &) ;
	
	TVector2D_for_cuts Add(TVector2D_for_cuts &) ;
	TVector2D_for_cuts Multiply(double) ;
	TVector2D_for_cuts Subtract(TVector2D_for_cuts &) ;

	double Scalar(TVector2D_for_cuts &) ;
	double Length() ;

	void Normalize() ;

	double GetX() { return x ; } ;
	double GetY() { return y ; } ;
	
	void Print(string) ;
} ;

TVector2D_for_cuts::TVector2D_for_cuts(double x, double y) : x(x), y(y) 
{

}

TVector2D_for_cuts::TVector2D_for_cuts(TVector2D_for_cuts &a)
{
	x = a.x ;
	y = a.y ;
}

TVector2D_for_cuts TVector2D_for_cuts::Add(TVector2D_for_cuts &b)
{
	x += b.x ;
	y += b.y ;
	
	return (*this) ;
}

TVector2D_for_cuts TVector2D_for_cuts::Subtract(TVector2D_for_cuts &b)
{
	x -= b.x ;
	y -= b.y ;

	return (*this) ;
}

TVector2D_for_cuts TVector2D_for_cuts::Multiply(double a)
{
	x *= a ;
	y *= a ;

	return (*this) ;
}

double TVector2D_for_cuts::Scalar(TVector2D_for_cuts &b)
{
	return ((x * b.x) + (y * b.y)) ;
}

double TVector2D_for_cuts::Length()
{
	return sqrt(this->Scalar(*this)) ;
}

void TVector2D_for_cuts::Normalize()
{
	double length = this->Length() ;
	
	x *= (1.0 / length) ;
	y *= (1.0 / length) ;
}

void TVector2D_for_cuts::Print(string text)
{
	// cout << text << " X: " << x << " Y: " << y << endl ;
}

class TAnalysis_cut 
{
	string name ;

	TVector2D_for_cuts *vector_to_one_point ;
	TVector2D_for_cuts *direction_unit_vector ;
	TVector2D_for_cuts *normal_unit_vector ;
	
	public:

	TAnalysis_cut(string, double, double, double, double) ;
	
	double ProjectionX(double, double) ;
	double ProjectionY(double, double) ;
	
	void Print() ;
} ;

TAnalysis_cut::TAnalysis_cut(string name, double x1, double y1, double x2, double y2): name(name)
{
	vector_to_one_point = new TVector2D_for_cuts(((x1 + x2) / 2.0), ((y1 + y2) / 2.0)) ;

	TVector2D_for_cuts p1(x1, y1) ;
	TVector2D_for_cuts p2(x2, y2) ;
	
	TVector2D_for_cuts diff = p2.Subtract(p1) ;
	diff.Normalize() ;
	
	direction_unit_vector = new TVector2D_for_cuts(diff) ;
	normal_unit_vector = new TVector2D_for_cuts(diff.GetY(), -diff.GetX()) ;
	
	double test1 = direction_unit_vector->Length() ;
	double test2 = normal_unit_vector->Length() ;
	double test3 = direction_unit_vector->Scalar(*normal_unit_vector) ;
	
	const double required_precision = 1e-8 ;
	
	if(TMath::Abs(test1 - 1.0) > required_precision)
	{
		cout << "[Error] TAnalysis_cut::TAnalysis_cut: direction_unit_vector is not a unit vectors!" << endl ;
		exit(1) ;
	}

	if(TMath::Abs(test2 - 1.0) > required_precision)
	{
		cout << "[Error] TAnalysis_cut::TAnalysis_cut: normal_unit_vector is not a unit vectors!" << endl ;
		exit(1) ;
	}

	if(TMath::Abs(test3) > required_precision)
	{
		cout << "[Error] TAnalysis_cut::TAnalysis_cut: vectors are not orthogonal!" << endl ;
		exit(1) ;
	}
}

void TAnalysis_cut::Print()
{
	vector_to_one_point->Print("vector_to_one_point") ;
	direction_unit_vector->Print("direction_unit_vector") ;
	normal_unit_vector->Print("normal_unit_vector") ;
}

double TAnalysis_cut::ProjectionX(double x, double y) 
{
	TVector2D_for_cuts point(x, y) ;
	
	return direction_unit_vector->Scalar(point) ;
}

double TAnalysis_cut::ProjectionY(double x, double y) 
{
	TVector2D_for_cuts point(x, y) ;
	
	return normal_unit_vector->Scalar(point) ;
}


// const bool use_old_dEdx = false ;

//----------------------------------------------------------------------------------------------------

class PromptAnalyzer : public edm::one::EDFilter<>
{
  public:
    explicit PromptAnalyzer(const edm::ParameterSet&);
    ~PromptAnalyzer() {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() override;
    virtual bool filter(edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    void evaluateDEDxCurves(double , double &, double &, double &, double &);
    bool isPionCurve(double, double );
    bool isSafePionCurve(double, double );
    bool isSafePionCurveNew(double, double );
    bool isSuperSafePionCurveNew(double, double );
    bool isKaonCurve(double, double );
    bool isKaonCurveRobert(double, double);

    bool isKaonCurveSpecial(double, double, double);

    bool isProtonCurve(double, double) ;
    double interpolationPionCurve(double, double, double) ;

    bool isElectron_horizontal_curve(double p, double dEdx) { return (p < 2.7) ; } ;
    bool isElectron_horizontal_and_vertical_curve(double p, double dEdx) { return ((p < 2.7) && (dEdx > 0.4)) ; } ;
    
    double factor_for_rho_mass_constraint(TLorentzVector, double, int) ;
    
    TCanvas *plot(TH1F *) ;

    edm::EDGetTokenT<reco::TrackCollection> trkToken_;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData>> dedxToken_ ;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData>> dedxPIXToken_ ;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData>> dedxPixHitsToken_ ;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData>> dedxStrHitsToken_ ;
    edm::EDGetTokenT<edm::ValueMap<reco::DeDxData>> dedxAllHitsToken_ ;
    edm::EDGetTokenT<DeDxDataValueMap> DeDxDataToken_;
   //edm::EDGetTokenT<reco::DeDxHitInfoAss> dedxpixelToken_ ;
    edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
    //edm::EDGetTokenT<edm::TriggerResults>  trigToken_;
    edm::EDGetTokenT<reco::PFCandidateCollection> pfToken_;
    //edm::EDGetTokenT<reco::MuonCollection> muToken_;
    //edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > clusterToken_;

    edm::EDGetTokenT<edm::DetSetVector<TotemRPRecHit>> rpRecHitToken_;
    edm::EDGetTokenT<edm::DetSetVector<TotemRPUVPattern>> rpPatternToken_;
    edm::EDGetTokenT<vector<CTPPSLocalTrackLite>> rpTrackToken_;

    map<string, TH1F*> histosTH1F;
    map<string, TH2F*> histosTH2F;
    map<string, TProfile*> profiles;

    map<string, int> map_of_conditions_if_they_are_applied ;
    std::string outputFileName;
    bool use_old_dEdx ;
    bool use_most_consistent_way_of_dEdx ;
    bool use_Michael_solution ;

    static const int lower_index_for_matrix = -2 ;
    static const int upper_index_for_matrix =  3 ;
    static const int kaon_step_max = 100 ;

    int pass ;

    MostProbable * mostProbable;
    ParticleType * particleType;
    
    TAnalysis_cut *TH2F_theta_x_R_xRF_vertex_term_subtracted_analysis_cut = NULL;
    TAnalysis_cut *TH2F_theta_x_L_xLN_vertex_term_subtracted_analysis_cut = NULL;
    
    TAnalysis_cut *TH2F_theta_y_R_yRF_analysis_cut = NULL;
    TAnalysis_cut *TH2F_theta_y_L_yLF_analysis_cut = NULL;

};

//----------------------------------------------------------------------------------------------------

const int massbins4 = 2000;
const string events_with_reasonable_number_of_dedx_measurments = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/events_with_reasonable_number_of_dedx_measurments.txt" ;

PromptAnalyzer::PromptAnalyzer(const edm::ParameterSet& iConfig) :
  trkToken_(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("tracks"))),
  dedxToken_(consumes<edm::ValueMap<reco::DeDxData>>(iConfig.getParameter<edm::InputTag>("dedxs"))),
  dedxPIXToken_(consumes<edm::ValueMap<reco::DeDxData>>(iConfig.getParameter<edm::InputTag>("dedxPIXs"))),
  dedxPixHitsToken_(consumes<edm::ValueMap<reco::DeDxData>>(iConfig.getParameter<edm::InputTag>("myenergyLossPixHits"))),
  dedxStrHitsToken_(consumes<edm::ValueMap<reco::DeDxData>>(iConfig.getParameter<edm::InputTag>("myenergyLossStrHits"))),
  dedxAllHitsToken_(consumes<edm::ValueMap<reco::DeDxData>>(iConfig.getParameter<edm::InputTag>("myenergyLossAllHits"))),
  DeDxDataToken_(consumes<DeDxDataValueMap>(iConfig.getParameter<edm::InputTag>("myenergyLossAllHits"))),
  //dedxpixelToken_(consumes<reco::DeDxHitInfoAss>(iConfig.getParameter<edm::InputTag>("dedxpixels"))),
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  pfToken_(consumes<reco::PFCandidateCollection>(iConfig.getParameter<edm::InputTag>("pflows"))),
  //muToken_(consumes<reco::MuonCollection>(iConfig.getParameter<edm::InputTag>("muons"))),

  rpRecHitToken_(consumes<edm::DetSetVector<TotemRPRecHit>>(iConfig.getParameter<edm::InputTag>("rpRecHitTag"))),
  rpPatternToken_(consumes<edm::DetSetVector<TotemRPUVPattern>>(iConfig.getParameter<edm::InputTag>("rpPatternTag"))),
  rpTrackToken_(consumes<vector<CTPPSLocalTrackLite>>(iConfig.getParameter<edm::InputTag>("rpTrackTag"))),

  outputFileName(iConfig.getParameter<std::string>("outputFileName"))
{
	use_old_dEdx = false ;
	use_most_consistent_way_of_dEdx = false ;
	use_Michael_solution = false ;

	pass = -99;

	mostProbable = new MostProbable();
	particleType = new ParticleType(mostProbable, pass);
	
}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::beginJob()
{
  int nbins_eta = 80;
  int nbins_pt = 100;
  int nbins_phi = 64;

  histosTH1F["hlooper"] = new TH1F("hlooper","isLooper",5,0,5);
  histosTH1F["hpt"] = new TH1F("hpt","p_{T}",nbins_pt,0,5);
  histosTH1F["heta"] = new TH1F("heta","#eta",nbins_eta,-4,4);
  histosTH1F["hphi"] = new TH1F("hphi","#varphi",nbins_phi,-3.2,3.2);

  histosTH1F["halgo"] = new TH1F("halgo","Algo",15,0,15.);
  histosTH1F["hnhits"] = new TH1F("hnhits","nhits pix+strip",40,0,40.);
  histosTH1F["hchi2"] = new TH1F("hchi2","normalized #chi^{2}",1050,-50,1000.);
  histosTH1F["hdz"] = new TH1F("hdz","dz",1000,-200,200.);
  histosTH1F["hd0"] = new TH1F("hd0","d0",2000,-20,20.);

  histosTH1F["hlooperv"] = new TH1F("hlooperv","isLooper",5,0,5);
  histosTH1F["hptv"] = new TH1F("hptv","p_{T}",nbins_pt,0,5);
  histosTH1F["hetav"] = new TH1F("hetav","#eta",nbins_eta,-4,4);
  histosTH1F["hphiv"] = new TH1F("hphiv","#varphi",nbins_phi,-3.2,3.2);

  histosTH1F["halgov"] = new TH1F("halgov","Algo",15,0,15.);
  histosTH1F["hnhitsv"] = new TH1F("hnhitsv","nhits pixel",40,0,40.);
  histosTH1F["hchi2v"] = new TH1F("hchi2v","normalized #chi^{2} vtx-fitted",550,-50,500.);
  histosTH1F["hdzv"] = new TH1F("hdzv","dz vtx-fitted",500,-100,100.);
  histosTH1F["hd0v"] = new TH1F("hd0v","d0 vtx-fitted",2000,-20,20.);

  histosTH1F["hntrk0"] = new TH1F("hntrk0","Ntrk",150,0,150);
  histosTH1F["hntrk"] = new TH1F("hntrk","Ntrk for nPixelHits>0",150,0,150);
  histosTH1F["hntrkvtx"] = new TH1F("hntrkvtx","Ntrk vtx",150,0,150);

  histosTH1F["hntrk02"] = new TH1F("hntrk02","Ntrk",150,0,150);
  histosTH1F["hntrkvtx2"] = new TH1F("hntrkvtx2","Ntrk vtx",150,0,150);

  histosTH1F["hntrk04"] = new TH1F("hntrk04","Ntrk",150,0,150);
  histosTH1F["hntrkvtx4"] = new TH1F("hntrkvtx4","Ntrk vtx",150,0,150);

  histosTH1F["hvtx"] = new TH1F("hvtx","vtx.isFake()",2,0,2);
  histosTH1F["hvtx2"] = new TH1F("hvtx2","vtx.isFake() 2 tracks",2,0,2);
  histosTH1F["hvtx3"] = new TH1F("hvtx3","vtx.isFake() 2 tracks both |#eta|<2.5 and OS",2,0,2);

  histosTH1F["hnvtx"] = new TH1F("hnvtx","Nvtx",10,0,10);
  histosTH1F["hvtxx"] = new TH1F("hvtxx","X vtx",1000,-1.,1.);
  histosTH1F["hvtxy"] = new TH1F("hvtxy","Y vtx",1000,-1.,1.);
  histosTH1F["hvtxz"] = new TH1F("hvtxz","Z vtx",300,-30.,30.);

  histosTH1F["hvtxx_after_mimic_cut"] = new TH1F("hvtxx_after_mimic_cut","X vtx",1000,-1.,1.);
  histosTH1F["hvtxy_after_mimic_cut"] = new TH1F("hvtxy_after_mimic_cut","X vtx",1000,-1.,1.);
  histosTH1F["hvtxz_after_mimic_cut"] = new TH1F("hvtxz_after_mimic_cut","X vtx",1000,-1.,1.);
  
  histosTH1F["hvtxx2"] = new TH1F("hvtxx2","X vtx",1000,-1.,1.);
  
  // Histograms for 2 tracks

  histosTH1F["2trk_totcharge"] = new TH1F("2trk_totcharge","2 track, total charge",10, -5, 5);

  // Histograms for 6 tracks

  histosTH1F["6trk_totcharge"] = new TH1F("6trk_totcharge","6 track, total charge",10, -5, 5);

  histosTH1F["hvtxchi2"] = new TH1F("hvtxchi2","chi2 vtx",1100,-100.,1000.);
  //histosTH1F["hvtxndof"] = new TH1F("hvtxndof","ndof vtx",1020,-2.,100.);
  //histosTH1F["hvtxchi2fin"] = new TH1F("hvtxchi2fin","chi2 vtx",1100,-100.,1000.);

  //--------------------------------------
  // RPs

  histosTH1F["hnConf"] = new TH1F("", "Number of configurations (TB or BT or TT or BB)" , 5, 0., 5.);
  histosTH1F["hnConfClean"] = new TH1F("", "Number of clean configurations (TB or BT or TT or BB)" , 5, 0., 5.);

  vector<string> labRP = { "TB", "BT", "TT", "BB" };
  histosTH1F["hConf"]  = new TH1F("", "", labRP.size(), 0, labRP.size());
  for (size_t k = 0; k < labRP.size(); ++k)
    histosTH1F["hConf"]->GetXaxis()->SetBinLabel((k+1), labRP[k].c_str());

  histosTH1F["eta_of_all"]  =  new TH1F("eta_of_all","#eta",nbins_eta, -4,4);
  histosTH1F["eta_of_pions"]  =  new TH1F("eta_of_pions","#eta",nbins_eta, -4,4);
  histosTH1F["eta_of_kaons"]  =  new TH1F("eta_of_kaons","#eta",nbins_eta, -4,4);

  histosTH1F["eta_of_all_below_mom_0p7"]  =  new TH1F("eta_of_all_below_mom_0p7","#eta",nbins_eta, -4,4);
  histosTH1F["eta_of_pions_below_mom_0p7"]  =  new TH1F("eta_of_pions_below_mom_0p7","#eta",nbins_eta, -4,4);
  histosTH1F["eta_of_kaons_below_mom_0p7"]  =  new TH1F("eta_of_kaons_below_mom_0p7","#eta",nbins_eta, -4,4);

  // Scattering angles

  histosTH2F["hthx2DIM"] = new TH2F("hthx2DIM"  , "#theta_{X}^{R} vs #theta_{X}^{L}" ,400,-0.0004,0.0004,400,-0.0004,0.0004);
  histosTH2F["hthy2DIM"] = new TH2F("hthy2DIM"  , "#theta_{y}^{R} vs #theta_{y}^{L}" ,400,-0.0004,0.0004,400,-0.0004,0.0004);
  histosTH2F["hthx2DIM_Ferenc"] = new TH2F("hthx2DIM_Ferenc"  , "" ,400,-0.0004,0.0004,400,-0.0004,0.0004);
  histosTH2F["hthx2DIM_full_nom"] = new TH2F("hthx2DIM_full_nom"  , "" ,400,-0.0004,0.0004,400,-0.0004,0.0004);

  histosTH2F["hthx2DIM_Ferenc_rotated"] = new TH2F("hthx2DIM_Ferenc_rotated"  , "" ,400,-0.0004,0.0004,400,-0.0004,0.0004);
  histosTH2F["hthx2DIM_full_nom_rotated"] = new TH2F("hthx2DIM_full_nom_rotated"  , "" ,400,-0.0004,0.0004,400,-0.0004,0.0004);

  // Scattering angles sum

  histosTH1F["hthyEla"] = new TH1F("hthyEla"  ,"#theta_{Y}^{L}+#theta_{Y}^{R}", 2000 , -0.0004 , 0.0004);
  histosTH1F["hthxEla"] = new TH1F("hthxEla"  ,"#theta_{X}^{L}+#theta_{X}^{R}", 2000 , -0.0004 , 0.0004);
  histosTH1F["hthxEla_Ferenc"] = new TH1F("hthxEla_Ferenc"  ,"#theta_{X}^{L}+#theta_{X}^{R}", 2000 , -0.0004 , 0.0004);
  histosTH1F["hthxEla_full_nom"] = new TH1F("hthxEla_full_nom"  ,"#theta_{X}^{L}+#theta_{X}^{R}", 2000 , -0.0004 , 0.0004);

  // Scattering angles profiles

  profiles["TProfile_hthx2DIM"] = new TProfile("TProfile_hthx2DIM"  , "#theta_{X}^{R} vs #theta_{X}^{L}" ,400,-0.0004,0.0004);
  profiles["TProfile_hthx2DIM_Ferenc"] = new TProfile("TProfile_hthx2DIM_Ferenc"  , "" ,400,-0.0004,0.0004);
  profiles["TProfile_hthx2DIM_full_nom"] = new TProfile("TProfile_hthx2DIM_full_nom"  , "" ,400,-0.0004,0.0004);


  // Vertex

  histosTH2F["vtxx2DIM"] = new TH2F("vtxx2DIM"  , "" ,400,-4.0,4.0,400,-4.0,4.0);
  histosTH2F["vtxx2DIM_Ferenc"] = new TH2F("vtxx2DIM_Ferenc"  , "" ,400,-4.0,4.0,400,-4.0,4.0);
  histosTH2F["vtxx2DIM_full_nom"] = new TH2F("vtxx2DIM_full_nom"  , "" ,400,-4.0,4.0,400,-4.0,4.0);

  histosTH2F["vtxy2DIM"] = new TH2F("vtxy2DIM"  , "" ,400,-4.0,4.0,400,-4.0,4.0);

  // Vertex sum

  histosTH1F["vtxx_sum"] = new TH1F("vtxx_sum"  ,"vtxx_sum", 2000 , -1.0e-1 , 1.0e-1) ;
  histosTH1F["vtxx_sum_Ferenc"] = new TH1F("vtxx_sum_Ferenc"  ,"vtxx_sum_Ferenc", 2000 , -1.0e-1 , 1.0e-1) ;
  histosTH1F["vtxx_sum_full_nom"] = new TH1F("vtxx_sum_full_nom"  ,"vtxx_sum_full_nom", 2000 , -1.0e-1 , 1.0e-1) ;

  // Vertex

  profiles["TProfile_vtxx2DIM"] = new TProfile("TProfile_vtxx2DIM"  , "" ,400,-4.0,4.0);
  profiles["TProfile_vtxx2DIM_Ferenc"] = new TProfile("TProfile_vtxx2DIM_Ferenc"  , "" ,400,-4.0,4.0);
  profiles["TProfile_vtxx2DIM_full_nom"] = new TProfile("TProfile_vtxx2DIM_full_nom"  , "" ,400,-4.0,4.0);

  // Scattering angles: 2D plots

  histosTH2F["hthxthyleft2DIM"] =  new TH2F("hthxthyleft2DIM"  , "#theta_{X}^{L} vs #theta_{Y}^{L}" ,400,-0.0004,0.0004,400,-0.0004,0.0004);
  histosTH2F["hthxthyright2DIM"] = new TH2F("hthxthyright2DIM"  , "#theta_{X}^{R} vs #theta_{Y}^{R}" ,400,-0.0004,0.0004,400,-0.0004,0.0004);

  histosTH2F["hthythx2DIM"] = new TH2F("hthythx2DIM"  , "#theta_{Y} vs #theta_{X}" ,800,-0.0004,0.0004,800,-0.0004,0.0004);

  histosTH1F["hthyEla2"] = new TH1F("hthyEla2"  ,"#theta_{Y}^{L}+#theta_{Y}^{R}", 2000 , -0.0004 , 0.0004);
  histosTH1F["hthxEla2"] = new TH1F("hthxEla2"  ,"#theta_{X}^{L}+#theta_{X}^{R}", 2000 , -0.0004 , 0.0004);

  // Correlation plots

  histosTH2F["TH2F_xLN_xRN"] = new TH2F("TH2F_xLN_xRN"  , "" , 400, -10.0, 10.0, 400, -10.0, 10.0);
  histosTH2F["TH2F_xLF_xRF"] = new TH2F("TH2F_xLF_xRF"  , "" , 400, -10.0, 10.0, 400, -10.0, 10.0);
  histosTH2F["TH2F_yLN_yRN"] = new TH2F("TH2F_yLN_yRN"  , "" , 400, -40.0, 40.0, 400, -40.0, 40.0);
  histosTH2F["TH2F_yLF_yRF"] = new TH2F("TH2F_yLF_yRF"  , "" , 400, -40.0, 40.0, 400, -40.0, 40.0);

  histosTH2F["TH2F_xLN_xRN_zoom"] = new TH2F("TH2F_xLN_xRN_zoom"  , "" , 400, -10.0, 10.0, 400, -1.0, 1.0);
  histosTH2F["TH2F_xLF_xRF_zoom"] = new TH2F("TH2F_xLF_xRF_zoom"  , "" , 400, -1.0, 1.0, 400, -1.0, 1.0);
  histosTH2F["TH2F_xRF_xLF_zoom"] = new TH2F("TH2F_xRF_xLF_zoom"  , "" , 400, -1.0, 1.0, 400, -1.0, 1.0);

  histosTH2F["TH2F_xLN_xLF_zoom"] = new TH2F("TH2F_xLN_xLF_zoom"  , "" , 400, -1.0, 1.0, 400, -1.0, 1.0);
  histosTH2F["TH2F_xRN_xRF_zoom"] = new TH2F("TH2F_xRN_xRF_zoom"  , "" , 400, -1.0, 1.0, 400, -1.0, 1.0);

  histosTH2F["TH2F_xRF_xLN"] = new TH2F("TH2F_xRF_xLN"  , "" , 400, -1.0, 1.0, 400, -1.0, 1.0);

  histosTH2F["TH2F_theta_x_L_theta_x_R"] = new TH2F("TH2F_theta_x_L_theta_x_R"  , "" , 400, -1.0e-3, 1.0e-3, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_y_L_theta_y_R"] = new TH2F("TH2F_theta_y_L_theta_y_R"  , "" , 400, -1.0e-3, 1.0e-3, 400, -1.0e-3, 1.0e-3);

  histosTH2F["TH2F_theta_x_L_theta_x_R_vertex_term_subtracted"] = new TH2F("TH2F_theta_x_L_theta_x_R_vertex_term_subtracted"  , "" , 400, -1.0e-3, 1.0e-3, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_x_L_theta_x_R_rotated"] = new TH2F("TH2F_theta_x_L_theta_x_R_rotated"  , "" , 400, -1.0e-3, 1.0e-3, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_x_L_theta_x_R_vertex_term_subtracted_rotated"] = new TH2F("TH2F_theta_x_L_theta_x_R_vertex_term_subtracted_rotated"  , "" , 400, -1.0e-3, 1.0e-3, 400, -1.0e-3, 1.0e-3);
  
  histosTH2F["TH2F_theta_x_L_xLN"] =  new TH2F("TH2F_theta_x_L_xLN"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);
  histosTH2F["TH2F_theta_x_L_xLF"] =  new TH2F("TH2F_theta_x_L_xLF"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);
  histosTH2F["TH2F_theta_x_R_xRN"] =  new TH2F("TH2F_theta_x_R_xRN"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);
  histosTH2F["TH2F_theta_x_R_xRF"] =  new TH2F("TH2F_theta_x_R_xRF"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);
  histosTH2F["TH2F_theta_y_L_yLN"] =  new TH2F("TH2F_theta_y_L_yLN"  , "" , 400, -1.0e-3, 1.0e-3, 400, -40.0, 40.0);
  histosTH2F["TH2F_theta_y_L_yLF"] =  new TH2F("TH2F_theta_y_L_yLF"  , "" , 400, -1.0e-3, 1.0e-3, 400, -40.0, 40.0);
  histosTH2F["TH2F_theta_y_R_yRN"] =  new TH2F("TH2F_theta_y_R_yRN"  , "" , 400, -1.0e-3, 1.0e-3, 400, -40.0, 40.0);
  histosTH2F["TH2F_theta_y_R_yRF"] =  new TH2F("TH2F_theta_y_R_yRF"  , "" , 400, -1.0e-3, 1.0e-3, 400, -40.0, 40.0);

  histosTH2F["TH2F_theta_y_L_yLF_projected_to_localxy"] =  new TH2F("TH2F_theta_y_L_yLF_projected_to_localxy"  , "" , 400, -40.0, 40.0, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_y_R_yRF_projected_to_localxy"] =  new TH2F("TH2F_theta_y_R_yRF_projected_to_localxy"  , "" , 400, -40.0, 40.0, 400, -1.0e-3, 1.0e-3);

  histosTH1F["TH2F_theta_y_L_yLF_projected_to_localxy_only_x"] = new TH1F("TH2F_theta_y_L_yLF_projected_to_localxy_only_x"  ,"", 400 , -1.0e-3 , 1.0e-3) ;
  histosTH1F["TH2F_theta_y_R_yRF_projected_to_localxy_only_x"] = new TH1F("TH2F_theta_y_R_yRF_projected_to_localxy_only_x"  ,"", 400 , -1.0e-3 , 1.0e-3) ;

  histosTH2F["TH2F_theta_x_L_xLN_vertex_term_subtracted"] =  new TH2F("TH2F_theta_x_L_xLN_vertex_term_subtracted"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);
  histosTH2F["TH2F_theta_x_R_xRF_vertex_term_subtracted"] =  new TH2F("TH2F_theta_x_R_xRF_vertex_term_subtracted"  , "" , 400, -1.0e-3, 1.0e-3, 400, -10.0, 10.0);

  histosTH2F["TH2F_xLN_vertex_term_subtracted_xLF_vertex_term_subtracted"] =  new TH2F("TH2F_xLN_vertex_term_subtracted_xLF_vertex_term_subtracted"  , "" , 400, -10, 10, 400, -10.0, 10.0);
  histosTH2F["TH2F_xRN_vertex_term_subtracted_xRF_vertex_term_subtracted"] =  new TH2F("TH2F_xRN_vertex_term_subtracted_xRF_vertex_term_subtracted"  , "" , 400, -10, 10, 400, -10.0, 10.0);

  histosTH2F["TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy"] =  new TH2F("TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy"  , "" , 400, -2.0, 2.0, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy"] =  new TH2F("TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy"  , "" , 400, -2.0, 2.0, 400, -1.0e-3, 1.0e-3);

  histosTH1F["TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy_only_x"] = new TH1F("TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy_only_x"  ,"", 400 , -1.0e-3 , 1.0e-3) ;
  histosTH1F["TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy_only_x"] = new TH1F("TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy_only_x"  ,"", 400 , -1.0e-3 , 1.0e-3) ;
  
  histosTH1F["TH2F_theta_y_L_yLN_cut"] = new TH1F("TH2F_theta_y_L_yLN_cut"  ,"TH2F_theta_y_L_yLN_cut", 2000 , -0.001 , 0.001);


  histosTH2F["TH2F_theta_x_L_xLN_swap"] =  new TH2F("TH2F_theta_x_L_xLN_swap"  , "" , 400, -10.0, 10.0, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_x_R_xRF_swap"] =  new TH2F("TH2F_theta_x_R_xRF_swap"  , "" , 400, -10.0, 10.0, 400, -1.0e-3, 1.0e-3);

  histosTH2F["TH2F_theta_x_L_xLN_swap_rotated"] =  new TH2F("TH2F_theta_x_L_xLN_swap_rotated"  , "" , 400, -10.0, 10.0, 400, -1.0e-3, 1.0e-3);
  histosTH2F["TH2F_theta_x_R_xRF_swap_rotated"] =  new TH2F("TH2F_theta_x_R_xRF_swap_rotated"  , "" , 400, -10.0, 10.0, 400, -1.0e-3, 1.0e-3);
  
  // Profiles

  profiles["TProfile_xLN_xRN"] = new TProfile("TProfile_xLN_xRN"  , "" , 400, -10.0, 10.0);
  profiles["TProfile_xLF_xRF"] = new TProfile("TProfile_xLF_xRF"  , "" , 400, -10.0, 10.0);
  profiles["TProfile_yLN_yRN"] = new TProfile("TProfile_yLN_yRN"  , "" , 400, -40.0, 40.0);
  profiles["TProfile_yLF_yRF"] = new TProfile("TProfile_yLF_yRF"  , "" , 400, -40.0, 40.0);

  profiles["TProfile_xLN_xRN_zoom"] = new TProfile("TProfile_xLN_xRN_zoom"  , "" , 400, -10.0, 10.0);
  profiles["TProfile_xLF_xRF_zoom"] = new TProfile("TProfile_xLF_xRF_zoom"  , "" , 400, -10.0, 10.0);
  
  profiles["TProfile_theta_x_L_theta_x_R"] = new TProfile("TProfile_theta_x_L_theta_x_R"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_y_L_theta_y_R"] = new TProfile("TProfile_theta_y_L_theta_y_R"  , "" , 400, -1.0e-3, 1.0e-3);
  
  profiles["TProfile_theta_x_L_xLN"] = new TProfile("TProfile_theta_x_L_xLN"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_x_L_xLF"] = new TProfile("TProfile_theta_x_L_xLF"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_x_R_xRN"] = new TProfile("TProfile_theta_x_R_xRN"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_x_R_xRF"] = new TProfile("TProfile_theta_x_R_xRF"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_y_L_yLN"] = new TProfile("TProfile_theta_y_L_yLN"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_y_L_yLF"] = new TProfile("TProfile_theta_y_L_yLF"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_y_R_yRN"] = new TProfile("TProfile_theta_y_R_yRN"  , "" , 400, -1.0e-3, 1.0e-3);
  profiles["TProfile_theta_y_R_yRF"] = new TProfile("TProfile_theta_y_R_yRF"  , "" , 400, -1.0e-3, 1.0e-3);

  //--------------------------------------
  // CMS-TOTEM matching

  histosTH1F["hdpy2trk"] = new TH1F("hdpy2trk","2trk, p^{CMS}_{Y}+p^{TOTEM}_{Y}",500,-0.5,0.5);
  histosTH1F["hdpx2trk"] = new TH1F("hdpx2trk","2trk, p^{CMS}_{X}+p^{TOTEM}_{X}",500,-0.5,0.5);

  histosTH1F["hdpy2trkB"] = new TH1F("hdpy2trkB","2trk, p^{CMS}_{Y}+p^{TOTEM}_{Y}",500,-0.5,0.5);
  histosTH1F["hdpx2trkB"] = new TH1F("hdpx2trkB","2trk, p^{CMS}_{X}+p^{TOTEM}_{X}",500,-0.5,0.5);

  histosTH2F["h2DIMdpy2trk"] = new TH2F("h2DIMdpy2trk","2trk, p^{TOTEM}_{Y} vs p^{CMS}_{Y}",200,-2.,2.,200,-2.,2.);
  histosTH2F["h2DIMdpx2trk"] = new TH2F("h2DIMdpx2trk","p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);

  histosTH1F["hdpy4trk"] = new TH1F("hdpy4trk","p^{CMS}_{Y}+p^{TOTEM}_{Y}",500,-0.5,0.5);
  histosTH1F["hdpx4trk"] = new TH1F("hdpx4trk","p^{CMS}_{X}+p^{TOTEM}_{X}",500,-0.5,0.5);

  histosTH1F["hdpy4trk_before_track_constraint"] = new TH1F("hdpy4trk_before_track_constraint","p^{CMS}_{Y}+p^{TOTEM}_{Y}",500,-0.5,0.5);
  histosTH1F["hdpx4trk_before_track_constraint"] = new TH1F("hdpx4trk_before_track_constraint","p^{CMS}_{X}+p^{TOTEM}_{X}",500,-0.5,0.5);

  // New plots per RP to Ken

  histosTH1F["hdpy4trk_1_F"] = new TH1F("hdpy4trk_1_F","p^{CMS}_{Y}+p^{TOTEM}_{Y}, near" ,500,-0.5,0.5);
  histosTH1F["hdpy4trk_2_F"] = new TH1F("hdpy4trk_2_F","p^{CMS}_{Y}+p^{TOTEM}_{Y}, far " ,500,-0.5,0.5);

  histosTH1F["hdpx4trk_1_F"] = new TH1F("hdpx4trk_1_F","p^{CMS}_{X}+p^{TOTEM}_{X}, near" ,500,-0.5,0.5);
  histosTH1F["hdpx4trk_2_F"] = new TH1F("hdpx4trk_2_F","p^{CMS}_{X}+p^{TOTEM}_{X}, far " ,500,-0.5,0.5);

  // New plots per RP to Ken: end

  histosTH1F["hdpy4trkB"] = new TH1F("hdpy4trkB","p^{CMS}_{Y}+p^{TOTEM}_{Y}",500,-0.5,0.5);
  histosTH1F["hdpx4trkB"] = new TH1F("hdpx4trkB","p^{CMS}_{X}+p^{TOTEM}_{X}",500,-0.5,0.5);

  histosTH2F["h2DIMdpy4trk"] = new TH2F("h2DIMdpy4trk","4trk, p^{TOTEM}_{Y} vs p^{CMS}_{Y}",200,-2.,2.,200,-2.,2.);
  histosTH2F["h2DIMdpx4trk"] = new TH2F("h2DIMdpx4trk","4trk, p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);

  histosTH2F["h2DIMdpy4trk_before_track_constraint"] = new TH2F("h2DIMdpy4trk_before_track_constraint","4trk, p^{TOTEM}_{Y} vs p^{CMS}_{Y}",200,-2.,2.,200,-2.,2.);
  histosTH2F["h2DIMdpx4trk_before_track_constraint"] = new TH2F("h2DIMdpx4trk_before_track_constraint","4trk, p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);

  histosTH2F["px_py_plot_1"] = new TH2F("px_py_plot_1","4trk, p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);
  histosTH2F["px_py_plot_2"] = new TH2F("px_py_plot_2","4trk, p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);
  histosTH2F["px_py_plot_3"] = new TH2F("px_py_plot_3","4trk, p^{TOTEM}_{X} vs p^{CMS}_{X}",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x"] = new TH2F("pflow_vs_track_x","4trk, pflow vs. track px",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y"] = new TH2F("pflow_vs_track_y","4trk, pflow vs. track py",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x_hadrons"] = new TH2F("pflow_vs_track_x_hadrons","4trk, pflow vs. track px, hadrons",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y_hadrons"] = new TH2F("pflow_vs_track_y_hadrons","4trk, pflow vs. track py, hadrons",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_z_hadrons"] = new TH2F("pflow_vs_track_z_hadrons","4trk, pflow vs. track pz, hadrons",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x_hadrons_4pfhadron"] = new TH2F("pflow_vs_track_x_hadrons_4pfhadron","4trk, pflow vs. track px, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y_hadrons_4pfhadron"] = new TH2F("pflow_vs_track_y_hadrons_4pfhadron","4trk, pflow vs. track py, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_z_hadrons_4pfhadron"] = new TH2F("pflow_vs_track_z_hadrons_4pfhadron","4trk, pflow vs. track pz, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x_hadrons_allCuts4"] = new TH2F("pflow_vs_track_x_hadrons_allCuts4","4trk, pflow vs. track px, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y_hadrons_allCuts4"] = new TH2F("pflow_vs_track_y_hadrons_allCuts4","4trk, pflow vs. track py, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_z_hadrons_allCuts4"] = new TH2F("pflow_vs_track_z_hadrons_allCuts4","4trk, pflow vs. track pz, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x_hadrons_4pfhadron_allCuts4"] = new TH2F("pflow_vs_track_x_hadrons_4pfhadron_allCuts4","4trk, pflow vs. track px, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y_hadrons_4pfhadron_allCuts4"] = new TH2F("pflow_vs_track_y_hadrons_4pfhadron_allCuts4","4trk, pflow vs. track py, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_z_hadrons_4pfhadron_allCuts4"] = new TH2F("pflow_vs_track_z_hadrons_4pfhadron_allCuts4","4trk, pflow vs. track pz, hadrons, 4 pf hadron",200,-2.,2.,200,-2.,2.);

  histosTH2F["pflow_vs_track_x_mom_matching"] = new TH2F("pflow_vs_track_x_mom_matching","4trk, pflow vs. track px, mom. matching",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_y_mom_matching"] = new TH2F("pflow_vs_track_y_mom_matching","4trk, pflow vs. track py, mom. matching",200,-2.,2.,200,-2.,2.);
  histosTH2F["pflow_vs_track_z_mom_matching"] = new TH2F("pflow_vs_track_z_mom_matching","4trk, pflow vs. track pz, mom. matching",200,-2.,2.,200,-2.,2.);

  histosTH2F["CMS_pt_vs_my_pt"] = new TH2F("CMS_pt_vs_my_pt","CMS pt vs my pt matching",200,-2.,2.,200,-2.,2.);

  histosTH1F["hLS"] = new TH1F("hLS","LS",5000,0.,5000.);
  histosTH1F["hLS2"] = new TH1F("hLS2","2trk rate vs LS",5000,0.,5000.);
  histosTH1F["hLS2ela"] = new TH1F("hLS2ela","2trk, elastic background rate vs LS",5000,0.,5000.);

  // Mass spectra

  int massbins=500;

  histosTH1F["hm2"]    = new TH1F("hm2", "M_{#pi^{+}#pi^{-}} (GeV)", massbins4,0.,10.);
  histosTH1F["hm4"]    = new TH1F("hm4", "M_{#pi^{+}#pi^{+}#pi^{-}#pi^{-}} (GeV)", massbins4,0.,10.);

  histosTH1F["hm4SIG1"] = new TH1F("hm4SIG1", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2"] = new TH1F("hm4SIG2", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4PHI"] = new TH1F("hm4PHI", "M_{4K}", massbins4,0.,10.);
  histosTH1F["hm4KST"] = new TH1F("hm4KST","M_{K^{*}(892)K^{*}(892)} ",massbins4,0.,10.);

  //--------------------------
  histosTH1F["hnOKpidKs"] = new TH1F("hnOKpidKs","Ncomb. K*K* ",5,0,5.);
  histosTH1F["hnOKpidKscurv"] = new TH1F("hnOKpidKscurv","Ncomb. K*K* ",5,0,5.);

  histosTH1F["hmALLkpi0"] = new TH1F("hmALLkpi0","M_{#pi#pi}",massbins,0,5.);
  histosTH1F["hmKSother0"] = new TH1F("hmKSother0","M_{K#pi} if K*(892)",massbins,0,5.);
  histosTH1F["hm4KST0"] = new TH1F("hm4KST0","M_{K^{*}(892)K^{*}(892)} ",massbins4,0.,10.);

  histosTH1F["hmALLkpi0curv"] = new TH1F("hmALLkpi0curv","M_{#pi#pi}",massbins,0,5.);
  histosTH1F["hmKSother0curv"] = new TH1F("hmKSother0curv","M_{K#pi} if K*(892)",massbins,0,5.);
  histosTH1F["hm4KST0curv"] = new TH1F("hm4KST0curv","M_{K^{*}(892)K^{*}(892)} ",massbins4,0.,10.);
  //--------------------------

  histosTH1F["hm4PHIscaled"] = new TH1F("hm4PHIscaled", "M_{4K}", massbins4,0.,10.);
  histosTH1F["hm4PHIscaled2"] = new TH1F("hm4PHIscaled2", "M_{4K}", massbins4,0.,10.);

  histosTH1F["hnKaons"] = new TH1F("hnKaons","N K curves in phiphi mass region",5,0,5.);
  histosTH1F["hnKaons_PhiCutStrict"] = new TH1F("hnKaons_PhiCutStrict","N K curves in phiphi mass region",5,0,5.);

  histosTH1F["hnKaons_FS"] = new TH1F("hnKaons_FS","N K curves in phiphi mass region",5,0,5.);
  histosTH1F["hnKaons_FS_p_cut"] = new TH1F("hnKaons_FS_p_cut","N K curves in phiphi mass region",5,0,5.);

  histosTH1F["hnPionsSIG1"] = new TH1F("hnPionsSIG1","N #pi curves in rhorho mass region",5,0,5.);
  histosTH1F["hnPionsSIG2"] = new TH1F("hnPionsSIG2","N #pi curves in rhorho mass region",5,0,5.);

  histosTH1F["hm4SIG1mass"] = new TH1F("hm4SIG1mass", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG1mass4curves"] = new TH1F("hm4SIG1mass4curves", "M_{#rho#rho} #sigma + 4#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG1mass34curves"] = new TH1F("hm4SIG1mass34curves", "M_{#rho#rho} #sigma + >=3#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG1mass234curves"] = new TH1F("hm4SIG1mass234curves", "M_{#rho#rho} #sigma + >=2#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG1mass1234curves"] = new TH1F("hm4SIG1mass1234curves", "M_{#rho#rho} #sigma + >1=#pi curves", massbins4,0.,10.);

  histosTH1F["hm4SIG1mass_phase_space_cut_3_sigma"] = new TH1F("hm4SIG1mass_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG1mass_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG1mass_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma"] = new TH1F("hm4SIG2mass_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG2mass_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut"] = new TH1F("hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_or"] = new TH1F("hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_or", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_ok"] = new TH1F("hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_ok", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  
  histosTH1F["hm4SIG1mass_track_quality_cut_low_and_high"] = new TH1F("hm4SIG1mass_track_quality_cut_low_and_high", "M_{#rho#rho} #sigma", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_pt_corrected"] = new TH1F("hm4SIG2mass_phase_space_cut_3_sigma_pt_corrected", "M_{#rho#rho} #sigma", massbins4,0.,10.);

  histosTH1F["hm4SIG1mass_Ken_constrained_rho"] = new TH1F("hm4SIG1mass_Ken_constrained_rho", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_Ken_constrained_rho"] = new TH1F("hm4SIG2mass_Ken_constrained_rho", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG4mass_Ken_constrained_rho"] = new TH1F("hm4SIG4mass_Ken_constrained_rho", "M_{#rho#rho} #sigma", massbins4,0.,10.);
  histosTH1F["hm4SIG8mass_Ken_constrained_rho"] = new TH1F("hm4SIG8mass_Ken_constrained_rho", "M_{#rho#rho} #sigma", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass"] = new TH1F("hm4SIG2mass", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG4mass"] = new TH1F("hm4SIG4mass", "M_{#rho#rho} #sigma/4", massbins4,0.,10.);
  histosTH1F["hm4SIG8mass"] = new TH1F("hm4SIG8mass", "M_{#rho#rho} #sigma/8", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_pt_cut_lower_200_MeV"] = new TH1F("hm4SIG2mass_pt_cut_lower_200_MeV", "M_{#rho#rho} #sigma/2, pt < 200 MeV", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_zvtx"] = new TH1F("hm4SIG2mass_zvtx", "M_{#rho#rho} #sigma/2, pt < 200 MeV", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_offset"] = new TH1F("hm4SIG2mass_offset", "M_{#rho#rho} #sigma/2, offset", massbins4,(0. + 0.0025), (10. + 0.0025));

  histosTH1F["hm4SIG2mass_from_plot"] = new TH1F("hm4SIG2mass_from_plot", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_pt"] = new TH1F("hm4SIG2mass_from_plot_pt", "M_{#rho#rho} #sigma/2, pt < 200 MeV", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_offset"] = new TH1F("hm4SIG2mass_from_plot_offset", "M_{#rho#rho} #sigma/2, bin offset", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_0"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_0", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_1"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_1", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_2"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_2", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_3"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_3", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_4"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_4", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_0"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_curve_0", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_1"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_curve_1", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_2"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_curve_2", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_3"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_curve_3", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_4"] = new TH1F("hm4SIG2mass_from_plot_nElectrons_horizontal_curve_4", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);

  // Electrons test in particle flow

  histosTH1F["hm4SIG2mass_from_plot_electrons_0"] = new TH1F("hm4SIG2mass_from_plot_electrons_0", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_electrons_0"] = new TH1F("hm4SIG2mass_from_plot_offset_electrons_0", "M_{#rho#rho} #sigma/2, bin offset", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_from_plot_electrons_1"] = new TH1F("hm4SIG2mass_from_plot_electrons_1", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_electrons_1"] = new TH1F("hm4SIG2mass_from_plot_offset_electrons_1", "M_{#rho#rho} #sigma/2, bin offset", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_from_plot_electrons_2"] = new TH1F("hm4SIG2mass_from_plot_electrons_2", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_electrons_2"] = new TH1F("hm4SIG2mass_from_plot_offset_electrons_2", "M_{#rho#rho} #sigma/2, bin offset", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass_from_plot_electrons_3"] = new TH1F("hm4SIG2mass_from_plot_electrons_3", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_electrons_3"] = new TH1F("hm4SIG2mass_from_plot_offset_electrons_3", "M_{#rho#rho} #sigma/2, bin offset", massbins4,0.,10.);

  
  // With dE/dx cuts as well

  histosTH1F["hm4SIG2mass_from_plot_nSafePionsNew_34"] = new TH1F("hm4SIG2mass_from_plot_nSafePionsNew_34", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_nSafePionsNew_34"] = new TH1F("hm4SIG2mass_from_plot_offset_nSafePionsNew_34", "M_{#rho#rho} #sigma/2, bin offset", massbins4, (0. + 0.0025),  (10. + 0.0025));

  histosTH1F["hm4SIG4mass_from_plot_nSafePionsNew_34"] = new TH1F("hm4SIG4mass_from_plot_nSafePionsNew_34", "M_{#rho#rho} #sigma/4, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG4mass_from_plot_offset_nSafePionsNew_34"] = new TH1F("hm4SIG4mass_from_plot_offset_nSafePionsNew_34", "M_{#rho#rho} #sigma/4, bin offset", massbins4, (0. + 0.0025),  (10. + 0.0025));

  histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_34"] = new TH1F("hm4SIG8mass_from_plot_nSafePionsNew_34", "M_{#rho#rho} #sigma/8, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG8mass_from_plot_offset_nSafePionsNew_34"] = new TH1F("hm4SIG8mass_from_plot_offset_nSafePionsNew_34", "M_{#rho#rho} #sigma/8, bin offset", massbins4, (0. + 0.0025),  (10. + 0.0025));
  
  // With dE/dx cuts as well: pions and kaons

  histosTH1F["hm4SIG2mass_from_plot_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG2mass_from_plot_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG2mass_from_plot_offset_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG2mass_from_plot_offset_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);

  histosTH1F["hm4SIG4mass_from_plot_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG4mass_from_plot_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG4mass_from_plot_offset_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG4mass_from_plot_offset_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);

  histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG8mass_from_plot_offset_nSafePionsNew_234_no_safekaon"] = new TH1F("hm4SIG8mass_from_plot_offset_nSafePionsNew_234_no_safekaon", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);

  histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_original_binning"] = new TH1F("hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_original_binning", "M_{#rho#rho} #sigma/8, bin offset", 1000, 0, 5);
  
  //

  histosTH1F["hm4SIG4mass_from_plot"] = new TH1F("hm4SIG4mass_from_plot", "M_{#rho#rho} #sigma/4", massbins4,0.,10.);
  histosTH1F["hm4SIG4mass_from_plot_pt"] = new TH1F("hm4SIG4mass_from_plot_pt", "M_{#rho#rho} #sigma/4, pt, pt < 200 MeV", massbins4,0.,10.);
  histosTH1F["hm4SIG4mass_from_plot_offset"] = new TH1F("hm4SIG4mass_from_plot_offset", "M_{#rho#rho} #sigma/4, bin offset", massbins4,(0. + 0.0025),  (10. + 0.0025));

  histosTH1F["hm4SIG8mass_from_plot"] = new TH1F("hm4SIG8mass_from_plot", "M_{#rho#rho} #sigma/8", massbins4,0.,10.);
  histosTH1F["hm4SIG8mass_from_plot_offset"] = new TH1F("hm4SIG8mass_from_plot_offset", "M_{#rho#rho} #sigma/8, bin offset", massbins4,(0. + 0.0025),  (10. + 0.0025));
  histosTH1F["hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak"] = new TH1F("hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak", "M_{#rho#rho} #sigma/8, bin offset", massbins4,(0. + 0.0025),  (10. + 0.0025));

  histosTH1F["hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak_original_binning"] = new TH1F("hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak_original_binning", "M_{#rho#rho} #sigma/8, bin offset", 1000, 0, 5);

  histosTH1F["hm4SIG8mass_from_plot_below_th"] = new TH1F("hm4SIG8mass_from_plot_below_th", "M_{#rho#rho} #sigma/8", massbins4,0.,10.);
  histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_34_below_th"] = new TH1F("hm4SIG8mass_from_plot_nSafePionsNew_34_below_th", "M_{#rho#rho} #sigma/8, bin offset", massbins4, 0.,  10.);
  histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_below_th"] = new TH1F("hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_below_th", "M_{#rho#rho} #sigma/2, bin offset", massbins4, 0.,  10.);

  histosTH1F["hm4SIG8mass_from_plot_v2"] = new TH1F("hm4SIG8mass_from_plot_v2", "M_{#rho#rho} #sigma/8", massbins4,0.,10.);
  histosTH1F["hm4SIG8mass_from_plot_offset_v2"] = new TH1F("hm4SIG8mass_from_plot_offset_v2", "M_{#rho#rho} #sigma/8, bin offset", massbins4,(0. + 0.0025),  (10. + 0.0025));

  histosTH1F["hm4SIG2massdiagonal"] = new TH1F("hm4SIG2massdiagonal", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2massparallel"] = new TH1F("hm4SIG2massparallel", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);

  histosTH1F["hm4SIG2massdiagonal_strict"] = new TH1F("hm4SIG2massdiagonal_strict", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);
  histosTH1F["hm4SIG2massparallel_strict"] = new TH1F("hm4SIG2massparallel_strict", "M_{#rho#rho} #sigma/2", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass4curves"] = new TH1F("hm4SIG2mass4curves", "M_{#rho#rho} #sigma/2 + 4#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass34curves"] = new TH1F("hm4SIG2mass34curves", "M_{#rho#rho} #sigma/2 + >=3#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass234curves"] = new TH1F("hm4SIG2mass234curves", "M_{#rho#rho} #sigma/2 + >=2#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass1234curves"] = new TH1F("hm4SIG2mass1234curves", "M_{#rho#rho} #sigma/2 + >=1#pi curves", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass4curves_phase_space_cut_3_sigma"] = new TH1F("hm4SIG2mass4curves_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma/2 + 4#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass34curves_phase_space_cut_3_sigma"] = new TH1F("hm4SIG2mass34curves_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma/2 + >=3#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass234curves_phase_space_cut_3_sigma"] = new TH1F("hm4SIG2mass234curves_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma/2 + >=2#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass1234curves_phase_space_cut_3_sigma"] = new TH1F("hm4SIG2mass1234curves_phase_space_cut_3_sigma", "M_{#rho#rho} #sigma/2 + >=1#pi curves", massbins4,0.,10.);

  histosTH1F["hm4SIG2mass4curves_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG2mass4curves_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma/2 + 4#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass34curves_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG2mass34curves_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma/2 + >=3#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass234curves_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG2mass234curves_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma/2 + >=2#pi curves", massbins4,0.,10.);
  histosTH1F["hm4SIG2mass1234curves_phase_space_cut_1p5_sigma"] = new TH1F("hm4SIG2mass1234curves_phase_space_cut_1p5_sigma", "M_{#rho#rho} #sigma/2 + >=1#pi curves", massbins4,0.,10.);

  histosTH1F["hm4PHImass"] = new TH1F("hm4PHImass", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_with_pion_mass"] = new TH1F("hm4PHImass_with_pion_mass", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_with_pion_mass_peak"] = new TH1F("hm4PHImass_with_pion_mass_peak", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass4curves"] = new TH1F("hm4PHImass4curves", "M_{4K}, #varphi#varphi + 4K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass3curves"] = new TH1F("hm4PHImass3curves", "M_{4K}, #varphi#varphi + 3K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass34curves"] = new TH1F("hm4PHImass34curves", "M_{4K}, #varphi#varphi + >=3K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass234curves"] = new TH1F("hm4PHImass234curves", "M_{4K}, #varphi#varphi + >=2K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass1234curves"] = new TH1F("hm4PHImass1234curves", "M_{4K}, #varphi#varphi + >=1K curves",massbins4,0.,10.);

  histosTH1F["hm4PHImass_reasonable_dedx_measurement"] = new TH1F("hm4PHImass_reasonable_dedx_measurement", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImass_nSuperSafePionsNew_0"] = new TH1F("hm4PHImass_nSuperSafePionsNew_0", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_4"] = new TH1F("hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_4", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_3"] = new TH1F("hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_3", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_2"] = new TH1F("hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_2", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_1"] = new TH1F("hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_1", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_0"] = new TH1F("hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_0", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImass_below_th"] = new TH1F("hm4PHImass_below_th", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass2SS_below_th"] = new TH1F("hm4PHImass2SS_below_th", "hm4PHImass2SS",massbins4,0.,10.);

  histosTH1F["hm4PHImass_bin1"] = new TH1F("hm4PHImass_bin1", "M_{4K}, #varphi#varphi", 2500, 0., 10.);
  histosTH1F["hm4PHImass_below_th_bin1"] = new TH1F("hm4PHImass_below_th_bin1", "M_{4K}, #varphi#varphi", 2500, 0., 10.);

  histosTH1F["hm4PHImass_bin2"] = new TH1F("hm4PHImass_bin2", "M_{4K}, #varphi#varphi", 2700, 0., 10.);
  histosTH1F["hm4PHImass_below_th_bin2"] = new TH1F("hm4PHImass_below_th_bin2", "M_{4K}, #varphi#varphi", 2700, 0., 10.);

  histosTH1F["hm4PHImass_bin3"] = new TH1F("hm4PHImass_bin3", "M_{4K}, #varphi#varphi", 512, 0., 10.);
  histosTH1F["hm4PHImass_below_th_bin3"] = new TH1F("hm4PHImass_below_th_bin3", "M_{4K}, #varphi#varphi", 512,  0., 10.);

  histosTH1F["hm4PHImass_below_th_and_above_dedx_th"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_3", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_2", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_1"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_1", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_4"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_4", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_3"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_3", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_2"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_2", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_1"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_1", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_4"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_4", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_3"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_3", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_2"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_2", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_1"] = new TH1F("hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_1", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  
  histosTH1F["hm4PHImass_4pions"] = new TH1F("hm4PHImass_4pions", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass_4kaons"] = new TH1F("hm4PHImass_4kaons", "M_{4K}, #varphi#varphi",massbins4,0.,10.);

  histosTH1F["hm4PHImassPionProtonVeto"] = new TH1F("", "M_{4K}, #varphi#varphi + Pion Veto",massbins4,0.,10.);

  histosTH1F["hm4PHImassSafePionProtonVeto"] = new TH1F("", "M_{4K}, #varphi#varphi + SafePionProton Veto",massbins4,0.,10.);
  histosTH1F["hm4PHImass1234curvesSafePionProtonVeto"] = new TH1F("", "M_{4K}, #varphi#varphi + >=1K curves + Pion Veto",massbins4,0.,10.);

  histosTH1F["hm4PHImass2SS"] = new TH1F("hm4PHImass2SS", "hm4PHImass2SS",massbins4,0.,10.);
  histosTH1F["hm4PHImass2SS_FS"] = new TH1F("hm4PHImass2SS_FS", "hm4PHImass2SS_FS",massbins4,0.,10.);
  histosTH1F["hm4PHImass2SSSafePionProtonVeto"] = new TH1F("hm4PHImass2SSSafePionProtonVeto", "hm4PHImass2SSSafePionProtonVeto",massbins4,0.,10.);

  histosTH1F["twoPionsOfTheSameSign"] = new TH1F("twoPionsOfTheSameSign", "twoPionsOfTheSameSign",massbins4,0.,10.);

  histosTH1F["hm4PHImass_PhiCutStrict"] = new TH1F("hm4PHImass_PhiCutStrict", "M_{4K}, #varphi#varphi",massbins4,0.,10.);
  histosTH1F["hm4PHImass4curves_PhiCutStrict"] = new TH1F("hm4PHImass4curves_PhiCutStrict", "M_{4K}, #varphi#varphi + 4K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass3curves_PhiCutStrict"] = new TH1F("hm4PHImass3curves_PhiCutStrict", "M_{4K}, #varphi#varphi + 3K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass34curves_PhiCutStrict"] = new TH1F("hm4PHImass34curves_PhiCutStrict", "M_{4K}, #varphi#varphi + >=3K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass234curves_PhiCutStrict"] = new TH1F("hm4PHImass234curves_PhiCutStrict", "M_{4K}, #varphi#varphi + >=2K curves",massbins4,0.,10.);
  histosTH1F["hm4PHImass1234curves_PhiCutStrict"] = new TH1F("hm4PHImass1234curves_PhiCutStrict", "M_{4K}, #varphi#varphi + >=1K curves",massbins4,0.,10.);

  histosTH1F["hm4PHImassPionProtonVeto_PhiCutStrict"] = new TH1F("", "M_{4K}, #varphi#varphi + PionProtonVeto",massbins4,0.,10.);

  histosTH1F["hm4PHImassSafePionProtonVeto_PhiCutStrict"] = new TH1F("", "M_{4K}, #varphi#varphi + SafePionProtonVeto",massbins4,0.,10.);
  histosTH1F["hm4PHImass1234curvesSafePionProtonVeto_PhiCutStrict"] = new TH1F("", "M_{4K}, #varphi#varphi + >=1K curves + SafePionProtonVeto",massbins4,0.,10.);

  histosTH1F["hm4PHImass2SS_PhiCutStrict"] = new TH1F("", "",massbins4,0.,10.);
  histosTH1F["hm4PHImass2SSSafePionProtonVeto_PhiCutStrict"] = new TH1F("", "",massbins4,0.,10.);

  histosTH1F["h_m_4K_PEAK"] = new TH1F("h_m_4K_PEAK", "M_{4K}", massbins4, 0., 10.);
  histosTH1F["h_m_4pi_PEAK"] = new TH1F("h_pi_4pi_PEAK", "M_{4#pi}", massbins4, 0., 10.);

  //-----

  histosTH1F["hm2BKG"] = new TH1F("hm2BKG", "TOTEM2, M_{#pi^{+}#pi^{-}} (GeV)", massbins4,0.,10.);
  histosTH1F["hm4BKG"] = new TH1F("hm4BKG", "M_{#pi^{+}#pi^{+}#pi^{-}#pi^{-}} (GeV)", massbins4,0.,10.);

  histosTH1F["hpt2"] = new TH1F("hpt2","p_{T}",40,0.,2.);
  histosTH1F["heta2"]= new TH1F("heta2","#eta",50,-5.,5.);

  histosTH1F["hpt2BKG"] = new TH1F("hpt2BKG","p_{T}",40,0.,2.);
  histosTH1F["heta2BKG"]= new TH1F("heta2BKG","#eta",50,-5.,5.);

  histosTH1F["hpt4"] = new TH1F("hpt4","p_{T}",40,0.,2.);
  histosTH1F["heta4"]= new TH1F("heta4","#eta",50,-5.,5.);

  histosTH1F["hpt4BKG"] = new TH1F("hpt4BKG","p_{T}",40,0.,2.);
  histosTH1F["heta4BKG"]= new TH1F("heta4BKG","#eta",50,-5.,5.);

  //-----

  histosTH1F["hdphi2"] = new TH1F("hdphi2","#Delta#varphi_{LR}",320,0,TMath::Pi());
  histosTH1F["hdphi4"] = new TH1F("hdphi4","#Delta#varphi_{LR}",320,0,TMath::Pi());

  //---------
  histosTH1F["hmALLpiKen0"] = new TH1F("hmALLpiKen0","M_{#pi#pi}",massbins,0,5.);

  histosTH2F["hmALLpiKen0_2D"] = new TH2F("hmALLpiKen0_2D","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);
//  histosTH2F["hmALLpiKen0_2D_2SS"] = new TH2F("hmALLpiKen0_2D_2SS","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_strict"] = new TH2F("hmALLpiKen0_2D_strict","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_new"] = new TH2F("hmALLpiKen0_2D_new","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 800, 0.4, 2.5, 800, 0.4, 2.5);
  histosTH2F["hmALLpiKen0_2D_PEAK"] = new TH2F("hmALLpiKen0_2D_PEAK","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_4pfhadron_allCuts4"] = new TH2F("hmALLpiKen0_2D_4pfhadron_allCuts4","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi), pflow hadron cuts", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_mom_matching"] = new TH2F("hmALLpiKen0_2D_mom_matching","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_constrained_rho"] = new TH2F("hmALLpiKen0_2D_constrained_rho","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);

  histosTH2F["hmALLpiKen0_2D_pt_corrected"] = new TH2F("hmALLpiKen0_2D_pt_corrected","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);

  histosTH2F["hmALLpiKen0_2D_pt_cut"] = new TH2F("hmALLpiKen0_2D_pt_cut","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_pt_cut_or"] = new TH2F("hmALLpiKen0_2D_pt_cut_or","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_pt_cut_ok"] = new TH2F("hmALLpiKen0_2D_pt_cut_ok","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);

  histosTH2F["hmALLpiKen0_2D_low_p"] = new TH2F("hmALLpiKen0_2D_low_p","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.0, 1.2, 640, 0.0, 1.2);

  histosTH2F["hmALLpiKen0_2D_p_condition"] = new TH2F("hmALLpiKen0_2D_p_condition","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.0, 1.2, 640, 0.0, 1.2);
  histosTH2F["hmALLpiKen0_2D_p_condition_after_phi_cut"] = new TH2F("hmALLpiKen0_2D_p_condition_after_phi_cut","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.0, 1.2, 640, 0.0, 1.2);
  histosTH2F["hmALLpiKen0_2D_p_condition_after_4massk_cut"] = new TH2F("hmALLpiKen0_2D_p_condition_after_4massk_cut","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.0, 1.2, 640, 0.0, 1.2);
  
  histosTH2F["hmALLpiKen0_2D_nSafePionsNew_34"] = new TH2F("hmALLpiKen0_2D_nSafePionsNew_34","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_nSafePionsNew_234_no_safekaon"] = new TH2F("hmALLpiKen0_2D_nSafePionsNew_234_no_safekaon","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_nSafePionsNew_4_no_safekaon"] = new TH2F("hmALLpiKen0_2D_nSafePionsNew_4_no_safekaon","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);


  histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho1"] = new TH2F("hmALLpiKen0_2D_Ken_constrained_rho1","M_{11}(#pi#pi) and. M_{22}(#pi#pi), Ken constrained rho mass, ", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho2"] = new TH2F("hmALLpiKen0_2D_Ken_constrained_rho2","M_{12}(#pi#pi) vs. M_{21) (#pi#pi), Ken constrained rho mass", 80, 0.4, 1.2, 80, 0.4, 1.2);

  histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho1_wrong_assignment"] = new TH2F("hmALLpiKen0_2D_Ken_constrained_rho1_wrong_assignment","M_{12}(#pi#pi) vs. M_{21}(#pi#pi), Ken constrained rho mass, Wrong_assignment!", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho2_wrong_assignment"] = new TH2F("hmALLpiKen0_2D_Ken_constrained_rho2_wrong_assignment","M_{11}(#pi#pi) vs. M_{22}(#pi#pi), Ken constrained rho mass, wrong_assignment!", 80, 0.4, 1.2, 80, 0.4, 1.2);

  histosTH2F["hmALLpiKen0_2D_no_safe_kaon"] = new TH2F("hmALLpiKen0_2D_no_safe_kaon","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["hmALLpiKen0_2D_strict_no_safe_kaon"] = new TH2F("hmALLpiKen0_2D_strict_no_safe_kaon","M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 80, 0.4, 1.2, 80, 0.4, 1.2);

  histosTH2F["m11_vs_m11_constrained_rho"] = new TH2F("m11_vs_m11_constrained_rho","m11 vs constrained", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["m22_vs_m22_constrained_rho"] = new TH2F("m22_vs_m22_constrained_rho","m22 vs constrained", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["m12_vs_m12_constrained_rho"] = new TH2F("m12_vs_m12_constrained_rho","m12 vs constrained", 80, 0.4, 1.2, 80, 0.4, 1.2);
  histosTH2F["m21_vs_m21_constrained_rho"] = new TH2F("m21_vs_m21_constrained_rho","m21 vs constrained", 80, 0.4, 1.2, 80, 0.4, 1.2);

  histosTH1F["hmrhootherKen0"] = new TH1F("hmrhootherKen0","M_{#pi#pi} if #rho",massbins,0,5.);

  histosTH1F["hmALLkKen0"] = new TH1F("hmALLkKen0","M_{KK}",massbins,0,5.);

  histosTH1F["hmphiotherKen0"] = new TH1F("hmphiotherKen0","M_{KK} if #varphi",massbins,0,5.);
  histosTH1F["hmphiotherKen0_PhiCutStrict"] = new TH1F("hmphiotherKen0_PhiCutStrict","M_{KK} if #varphi",massbins,0,5.);

  histosTH2F["hmALLkKen0_2D"] = new TH2F("hmALLkKen0_2D","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.3, 1200, 0.9, 1.3);
  histosTH2F["hmALLkKen0_2D_2SS_v0"] = new TH2F("hmALLkKen0_2D_2SS_v0","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 80, 0.9, 1.3, 80, 0.9, 1.3);
  histosTH2F["hmALLkKen0_2D_new"] = new TH2F("hmALLkKen0_2D_new","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 800, 0.9, 2.5, 800, 0.9, 2.5);

  histosTH2F["hmALLkKen0_2D_2SS_in_dEdx_range"] = new TH2F("hmALLkKen0_2D_2SS_in_dEdx_range","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.3, 1200, 0.9, 1.3);

  histosTH2F["hmALLkKen0_2D_p_condition"] = new TH2F("hmALLkKen0_2D_p_condition","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.3, 1200, 0.9, 1.3);

  histosTH2F["hmALLkKen0_2D_strict_phi_cut_test"] = new TH2F("hmALLkKen0_2D_strict_phi_cut_test","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.3, 1200, 0.9, 1.3);

  histosTH2F["hmALLkKen0_2D_high_p"] = new TH2F("hmALLkKen0_2D_high_p","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.6, 1200, 0.9, 1.6);
  histosTH2F["hmALLkKen0_2D_p_condition_high_p"] = new TH2F("hmALLkKen0_2D_p_condition_high_p","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 1200, 0.9, 1.6, 1200, 0.9, 1.6);


  histosTH2F["hmALLkKen0_2D_2SS_v0_FS"] = new TH2F("hmALLkKen0_2D_2SS_v0_FS","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 80, 0.9, 1.3, 80, 0.9, 1.3);
  histosTH2F["hmALLkKen0_2D_2piSS"] = new TH2F("hmALLkKen0_2D_2piSS","M_{22 or 21}(KK) vs. M_{11 or 12}(KK)", 80, 0.9, 1.3, 80, 0.9, 1.3);
  
  histosTH1F["hmKSother"] = new TH1F("hmKSother","M_{K#pi} if K*(892)",massbins,0,5.);
  
  histosTH1F["hdedx4PHImass_Ferenc_momentum_slice"] = new TH1F("hdedx4PHImass_Ferenc_momentum_slice", "hdedx4PHImass_Ferenc_momentum_slice", 100, 0.,10.);


  //---------DIM

  histosTH1F["heHFhad"] = new TH1F("heHFhad","E HFhad",500,0.,50);
  histosTH1F["heHFemc"] = new TH1F("heHFemc","E HFemc",500,0.,50);

  histosTH1F["hnHF"] = new TH1F("hnHF","n HF",100,0.,100);
  histosTH1F["hnHFhad"] = new TH1F("hnHFhad","n HF had",100,0.,100);
  histosTH1F["hnHFemc"] = new TH1F("hnHFemc","n HF emc",100,0.,100);

  histosTH2F["hHF_Memc_2DIM"] = new TH2F("hHF_Memc_2DIM","N HFhad",600,-5.2,-2.8,160,-3.2,3.2);
  histosTH2F["hHF_Mhad_2DIM"] = new TH2F("hHF_Mhad_2DIM","N HFhad",600,-5.2,-2.8,160,-3.2,3.2);

  histosTH2F["hHF_Pemc_2DIM"] = new TH2F("hHF_Pemc_2DIM","N HFhad",600,2.8,5.2,160,-3.2,3.2);
  histosTH2F["hHF_Phad_2DIM"] = new TH2F("hHF_Phad_2DIM","N HFhad",600,2.8,5.2,160,-3.2,3.2);

  histosTH2F["heHF_Memc_2DIM"] = new TH2F("heHF_Memc_2DIM","E HFhad",60,-5.2,-2.8,100,0.,20.);
  histosTH2F["heHF_Mhad_2DIM"] = new TH2F("heHF_Mhad_2DIM","E HFhad",60,-5.2,-2.8,100,0.,20.);

  histosTH2F["heHF_Pemc_2DIM"] = new TH2F("heHF_Pemc_2DIM","E HFhad",60,2.8,5.2,100,0.,20.);
  histosTH2F["heHF_Phad_2DIM"] = new TH2F("heHF_Phad_2DIM","E HFhad",60,2.8,5.2,100,0.,20.);


  histosTH2F["hpt_rho"] = new TH2F("hpt_rho", "pt corr rho" ,nbins_pt, 0, 5, nbins_pt, 0, 5);
  histosTH2F["hpt_phi"] = new TH2F("hpt_phi", "pt corr phi" ,nbins_pt, 0, 5, nbins_pt, 0, 5);

  histosTH2F["hpt_K0"] = new TH2F("hpt_K0", "pt corr K0" ,nbins_pt, 0, 5, nbins_pt, 0, 5);

  histosTH1F["hpt_rho_projection"] = new TH1F("hpt_rho_projection", "pt corr rho" ,nbins_pt, 0, 5);

  //------------
  // dEdx

  histosTH1F["hdedxDetector"] = new TH1F("hdedxDetector","dedx Detector",4,0,4.);

  histosTH2F["hdedxAllPIX"] = new TH2F("hdedxAllPIX","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllSTR"] = new TH2F("hdedxAllSTR","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxSTRvsPIX"] = new TH2F("hdedxSTRvsPIX","dE/dx STR vs dE/dx PIX", 200, 0.,10.,200, 0.,10.);

  histosTH2F["hdedxPixHits"] = new TH2F("hdedxPixHits","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxStrHits"] = new TH2F("hdedxStrHits","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits"] = new TH2F("hdedxAllHits","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  
  histosTH2F["hdedxAllHits_reasonable_dedx_measurement"] = new TH2F("hdedxAllHits_reasonable_dedx_measurement","dE/dx vs p", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedxAllHits_special_phi_cuts_1"] = new TH2F("hdedxAllHits_special_phi_cuts_1","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_special_phi_cuts_2"] = new TH2F("hdedxAllHits_special_phi_cuts_2","dE/dx vs p", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedxAllHits_special_phi_cuts_control_1"] = new TH2F("hdedxAllHits_special_phi_cuts_control_1","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_special_phi_cuts_control_2"] = new TH2F("hdedxAllHits_special_phi_cuts_control_2","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  
  histosTH2F["hdedxAllHits_special_phi_cuts_control_1_Simone"] = new TH2F("hdedxAllHits_special_phi_cuts_control_1_Simone","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_special_phi_cuts_control_1_Simone_2"] = new TH2F("hdedxAllHits_special_phi_cuts_control_1_Simone_2","dE/dx vs p", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedxAllHits_electrons_0"] = new TH2F("hdedxAllHits_electrons_0","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_electrons_1"] = new TH2F("hdedxAllHits_electrons_1","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_electrons_2"] = new TH2F("hdedxAllHits_electrons_2","dE/dx vs p", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedxAllHits_electrons_3"] = new TH2F("hdedxAllHits_electrons_3","dE/dx vs p", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx2trk"] = new TH2F("hdedx2trk","dE/dx vs p 2trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4trk"] = new TH2F("","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);

  histosTH2F["hdedx4trk_PionCurve"] = new TH2F("","dE/dx vs p 4trk, within pion curves", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_SafePionCurve"] = new TH2F("","dE/dx vs p 4trk, within safe pion curves", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_SafePionNewCurve"] = new TH2F("","dE/dx vs p 4trk, within new safe pion curves", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx4trk_FS"] = new TH2F("hdedx4trk_FS","dE/dx vs p 4trk", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_FS_below_mom_0p7"] = new TH2F("hdedx4trk_FS_below_mom_0p7","dE/dx vs p 4trk", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx4trk_KaonCurve"] = new TH2F("hdedx4trk_KaonCurve","dE/dx vs p 4trk, within kaon curves", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_KaonCurve_FS"] = new TH2F("hdedx4trk_KaonCurve_FS","dE/dx vs p 4trk, within kaon curves", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_KaonCurve_FS_below_mom_0p7"] = new TH2F("hdedx4trk_KaonCurve_FS_below_mom_0p7","dE/dx vs p 4trk, within kaon curves", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx4trk_PionCurve_FS"] = new TH2F("hdedx4trk_PionCurve_FS","dE/dx vs p 4trk, within pion curves", 500, 0.,5.,400, 0.,20.);
  histosTH2F["hdedx4trk_PionCurve_FS_below_mom_0p7"] = new TH2F("hdedx4trk_PionCurve_FS_below_mom_0p7","dE/dx vs p 4trk, within pion curves", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx4trk_KaonCurveRobert"] = new TH2F("hdedx4trk_KaonCurveRobert","dE/dx vs p 4trk, within kaon curves, Robert", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx4trk_ProtonCurve"] = new TH2F("hdedx4trk_ProtonCurve","dE/dx vs p 4trk, within proton curves", 500, 0.,5.,400, 0.,20.);

  histosTH2F["hdedx2trklog"] = new TH2F("hdedx2trklog","log(dE/dx) vs p 2trk", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx4trklog"] = new TH2F("hdedx4trklog","log(dE/dx) vs p 4trk", 500, 0.,5.,300, -1.,2.);

  histosTH2F["hdedx2pionslog"] = new TH2F("hdedx2pionslog","log(dE/dx) vs p 2#pi", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx2kaonslog"] = new TH2F("hdedx2kaonslog","log(dE/dx) vs p 2K", 500, 0.,5.,300, -1.,2.);

  histosTH2F["hdedx2pionslog35"] = new TH2F("hdedx2pionslog35","log(dE/dx) vs p 2#pi", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx2kaonslog35"] = new TH2F("hdedx2kaonslog35","log(dE/dx) vs p 2K", 500, 0.,5.,300, -1.,2.);

  histosTH2F["hdedx2pionslog40"] = new TH2F("hdedx2pionslog40","log(dE/dx) vs p 2#pi", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx2kaonslog40"] = new TH2F("hdedx2kaonslog40","log(dE/dx) vs p 2K", 500, 0.,5.,300, -1.,2.);

  histosTH2F["hdedx2pionslog45"] = new TH2F("hdedx2pionslog45","log(dE/dx) vs p 2#pi", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx2kaonslog45"] = new TH2F("hdedx2kaonslog45","log(dE/dx) vs p 2K", 500, 0.,5.,300, -1.,2.);

  histosTH2F["hdedx2pionslog50"] = new TH2F("hdedx2pionslog50","log(dE/dx) vs p 2#pi", 500, 0.,5.,300, -1.,2.);
  histosTH2F["hdedx2kaonslog50"] = new TH2F("hdedx2kaonslog50","log(dE/dx) vs p 2K", 500, 0.,5.,300, -1.,2.);

//  histosTH2F["hdedx4PHImass"] =  new TH2F("hdedx4PHImass","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4PHImass_Ferenc"] =  new TH2F("hdedx4PHImass_Ferenc","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4PHImass4curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass3curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass34curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass234curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass1234curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass2SS"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass2SS_Ferenc"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass2SS_Ferenc_FS"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImassPionProtonVeto"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImassSafePionProtonVeto"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass1234curvesSafePionProtonVeto"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass2SSSafePionProtonVeto"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hdedx4PHImass_Ferenc_control"] =  new TH2F("hdedx4PHImass_Ferenc_control","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);

  histosTH2F["hdedx4PHImass_strict_Ferenc"] =  new TH2F("hdedx4PHImass_strict_Ferenc","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);

  histosTH2F["hdedx4PHImass_Ferenc_false_1"] =  new TH2F("hdedx4PHImass_Ferenc_false_1","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4PHImass_Ferenc_false_2"] =  new TH2F("hdedx4PHImass_Ferenc_false_2","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4PHImass_Ferenc_false_3"] =  new TH2F("hdedx4PHImass_Ferenc_false_3","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
  histosTH2F["hdedx4PHImass_Ferenc_false_4"] =  new TH2F("hdedx4PHImass_Ferenc_false_4","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);

  histosTH2F["hdedx4_twoPionsOfTheSameSign"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4_twoPionsOfTheSameSign_v0"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hdedx4PHImass_Ferenc_peak"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_Ferenc_peak_high"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_Ferenc_peak_low"] =  new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH1F["p_peak_low"] = new TH1F("p_peak_low","p_peak_low",500, 0., 5.);
  histosTH1F["p_peak_high"] = new TH1F("p_peak_high","p_peak_high",500, 0., 5.);

  histosTH2F["hdedx4PHImass_PEAK"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hdedx4SIG1mass"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4SIG1mass4curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4SIG1mass34curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4SIG1mass234curves"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4SIG1mass1234curves"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hm4SIG8mass_from_plot_dEdx_info"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_0"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_1"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_2"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_3"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_v2"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hdedx4_nSafePionsNew_234_no_safekaon"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4_nSafePionsNew_234_no_safekaon_Ferenc"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4_nSafePionsNew_4_no_safekaon_Ferenc"] = new TH2F(* histosTH2F["hdedx4trk"]);

  histosTH2F["hdedx4PHImass_2SS_v0"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_2SS_v0_Ferenc"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_2SS_v0_Ferenc_FS"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_test_of_3_pions_and_1_kaon"] = new TH2F(* histosTH2F["hdedx4trk"]);
  histosTH2F["hdedx4PHImass_test_of_3_pions_and_1_kaon_old_dedx"] = new TH2F(* histosTH2F["hdedx4trk"]);

  
  // Particle flow
  
  histosTH1F["npflow"] = new TH1F("npflow","npflow",100,0.,100);
  histosTH1F["npflow_hadrons"] = new TH1F("npflow_hadrons","npflow_hadrons",100,0.,100);
  
  // 4-vector matching

  histosTH1F["pi4m11_Px"] = new TH1F("pi4m11_Px","pi4m11_Px",100,-5., 5);
  histosTH1F["pi4m22_Px"] = new TH1F("pi4m22_Px","pi4m22_Px",100,-5., 5);

  histosTH1F["pi4m11_Py"] = new TH1F("pi4m11_Py","pi4m11_Py",100,-5., 5);
  histosTH1F["pi4m22_Py"] = new TH1F("pi4m22_Py","pi4m22_Py",100,-5., 5);

  histosTH1F["pi4m11_Pz"] = new TH1F("pi4m11_Pz","pi4m11_Pz",100,-5., 5);
  histosTH1F["pi4m22_Pz"] = new TH1F("pi4m22_Pz","pi4m22_Pz",100,-5., 5);

  histosTH1F["pi4m11_E"]  = new TH1F("pi4m11_E","pi4m11_E",  100,-5., 5);
  histosTH1F["pi4m22_E"]  = new TH1F("pi4m22_E","pi4m22_E",  100,-5., 5);

  histosTH1F["pi4m11_M"]  = new TH1F("pi4m11_M","pi4m11_M",  100,-5., 5);
  histosTH1F["pi4m22_M"]  = new TH1F("pi4m22_M","pi4m22_M",  100,-5., 5);

  histosTH1F["pi4m12_Px"] = new TH1F("pi4m12_Px","pi4m12_Px",100,-5., 5);
  histosTH1F["pi4m21_Px"] = new TH1F("pi4m21_Px","pi4m21_Px",100,-5., 5);

  histosTH1F["pi4m12_Py"] = new TH1F("pi4m12_Py","pi4m12_Py",100,-5., 5);
  histosTH1F["pi4m21_Py"] = new TH1F("pi4m21_Py","pi4m21_Py",100,-5., 5);

  histosTH1F["pi4m12_Pz"] = new TH1F("pi4m12_Pz","pi4m12_Pz",100,-5., 5);
  histosTH1F["pi4m21_Pz"] = new TH1F("pi4m21_Pz","pi4m21_Pz",100,-5., 5);

  histosTH1F["pi4m12_E"]  = new TH1F("pi4m12_E","pi4m12_E",  100,-5., 5);
  histosTH1F["pi4m21_E"]  = new TH1F("pi4m21_E","pi4m21_E",  100,-5., 5);

  histosTH1F["pi4m12_M"]  = new TH1F("pi4m12_M","pi4m12_M",  100,-5., 5);
  histosTH1F["pi4m21_M"]  = new TH1F("pi4m21_M","pi4m21_M",  100,-5., 5);


  histosTH1F["pi4m11_Px_minus_pi4m22_Px"]  = new TH1F("pi4m11_Px_minus_pi4m22_Px","pi4m11_Px_minus_pi4m22_Px",  100, -5., 5);
  histosTH1F["pi4m11_Py_minus_pi4m22_Py"]  = new TH1F("pi4m11_Py_minus_pi4m22_Py","pi4m11_Py_minus_pi4m22_Py",  100, -5., 5);
  histosTH1F["pi4m11_Pz_minus_pi4m22_Pz"]  = new TH1F("pi4m11_Pz_minus_pi4m22_Pz","pi4m11_Pz_minus_pi4m22_Pz",  100, -5., 5);

  histosTH1F["pi4m12_Px_minus_pi4m21_Px"]  = new TH1F("pi4m12_Px_minus_pi4m21_Px","pi4m12_Px_minus_pi4m21_Px",  100, -5., 5);
  histosTH1F["pi4m12_Py_minus_pi4m21_Py"]  = new TH1F("pi4m12_Py_minus_pi4m21_Py","pi4m12_Py_minus_pi4m21_Py",  100, -5., 5);
  histosTH1F["pi4m12_Pz_minus_pi4m21_Pz"]  = new TH1F("pi4m12_Pz_minus_pi4m21_Pz","pi4m12_Pz_minus_pi4m21_Pz",  100, -5., 5);
  
// Alignment test

  histosTH1F["hpt2_run_319174"] = new TH1F("hpt2_run_319174","p_{T}",40,0.,2.);
  histosTH1F["hpx_run_319174"] = new TH1F("hpx_run_319174","p_{x}",40, -2.,2.);
  histosTH1F["hpy_run_319174"] = new TH1F("hpy_run_319174","p_{y}",40, -2.,2.);
  histosTH1F["hpz_run_319174"] = new TH1F("hpz_run_319174","p_{z}",40, -2.,2.);

  histosTH1F["hpt2_run_319174_test"] = new TH1F("hpt2_run_319174_test","p_{T}",40,0.,2.);

  histosTH1F["rho_mass"] = new TH1F("rho_mass","p_{T}", 1000, 700.,800.);


// General status bits
 
  histosTH1F["general_status_bits"]  = new TH1F("general_status_bits","general_status_bits", 100, 0, 100);

// Mass scanning 2D
 
     for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
      {
        stringstream suffix_1, suffix_2 ;
        suffix_1 << (ni - lower_index_for_matrix) ;
        suffix_2 << (nj - lower_index_for_matrix) ;

        string name_for_histo = ("hm4SIG2mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        histosTH1F[name_for_histo.c_str()] = new TH1F((name_for_histo).c_str(), (name_for_histo).c_str(), massbins4, 0.,10.);

        name_for_histo = ("hm4SIG4mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        histosTH1F[name_for_histo.c_str()] = new TH1F((name_for_histo).c_str(), (name_for_histo).c_str(), massbins4, 0.,10.);

        name_for_histo = ("hm4SIG8mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        histosTH1F[name_for_histo.c_str()] = new TH1F((name_for_histo).c_str(), (name_for_histo).c_str(), massbins4, 0.,10.);

      }

// Mass scanning dE/dx

      for(int ki = 0 ; ki < kaon_step_max ; ++ki)
      {
        stringstream ss ;

        ss << ki ;

	string histogram_name = "hm4PHImass2SS_" + ss.str() ;
        histosTH1F[histogram_name] = new TH1F((histogram_name).c_str(), (histogram_name).c_str(), massbins4, 0.,10.);
      }

	vector<int> run_number_vector{319104, 319124, 319125, 319159, 319160, 319174, 319175, 319176, 319177, 319190, 319222, 319223, 319254, 319255, 319256, 319260, 319262, 319263, 319264, 319265, 319266, 319267, 319268, 319270, 319300, 319311} ;
	
	for(unsigned int i = 0 ; i < run_number_vector.size() ; ++i)
	{
		stringstream ss ;
		
		ss << run_number_vector[i] ;
		
		string histogram_name = "hmALLpiKen0_2D_" + ss.str() ;
		histosTH2F[histogram_name.c_str()] = new TH2F(histogram_name.c_str(), "M_{22 or 21}(#pi#pi) vs. M_{11 or 12}(#pi#pi)", 640, 0.4, 1.2, 640, 0.4, 1.2);
	}

	TH2F_theta_x_R_xRF_vertex_term_subtracted_analysis_cut = new TAnalysis_cut("TH2F_theta_x_R_xRF_vertex_term_subtracted_analysis_cut", -5e-5, -0.32471182, 5e-5,  0.32503068) ;
	TH2F_theta_x_L_xLN_vertex_term_subtracted_analysis_cut = new TAnalysis_cut("TH2F_theta_x_L_xLN_vertex_term_subtracted_analysis_cut", -5e-5,  0.29720915, 5e-5, -0.29534867) ;
	
	TH2F_theta_y_R_yRF_analysis_cut = new TAnalysis_cut("TH2F_theta_y_R_yRF_analysis_cut", -4e-4, -23.083271, -2e-4, -11.552197) ;
	TH2F_theta_y_L_yLF_analysis_cut = new TAnalysis_cut("TH2F_theta_y_L_yLF_analysis_cut", -4e-4, -22.761178, -2e-4, -11.301531) ;
  
}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::endJob()
{
  if (outputFileName == "")
    return;

  TFile * f_out = TFile::Open(outputFileName.c_str(), "RECREATE");

  for (const auto &p : histosTH1F)
    p.second->Write(p.first.c_str());

  for (const auto &p : histosTH2F)
    p.second->Write(p.first.c_str());

  plot(histosTH1F["hm4SIG2massdiagonal"])->Write("hm4SIG2massdiagonal_canvas") ;

  delete f_out;
}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::evaluateDEDxCurves(double p, double &piu3, double &kd3, double &ku3, double &pid3)
{

  if(use_old_dEdx)
  {
  // 3sigma curves based on 2018 data with pbins=2
  	ku3 = 4.53163e+00-5.89828e-01*p + exp(3.16431e+00-5.38011e+00*p);
  	kd3 = 1.46882e+00-9.31222e-04*p + exp(2.12237e+00-4.34300e+00*p);
  	piu3 = 3.49965e+00-2.88850e-01*atan(p-1.51050e+00) + exp(3.03012e+00-1.53867e+01*p);
  }
  else
  {
	// Before 2020.05.29
	//
  	// ku3 = 3.56575- (-0.0159641* p) + exp(3.81291 - (6.37289*p));
  	// kd3 = 2.53307- (-0.00992539* p) + exp(3.29768 - (6.10077*p));
	// piu3 = 616507.088- (392613.677* atan(p + 1843.17019)) + exp(3.42193876 - (14.7244767 * p));
	//

	/* 2020

	ku3 =  3.56575287- (-0.015964108* p) + exp(3.8129127 - (6.37289422*p)) ;
	kd3 =  2.09219508- (-0.0255047903* p) + exp(3.19090268 - (5.40106319*p)) ;

	piu3 = 44752.1802- (28507.4293* atan(p - -924.395828)) + exp(3.01618089 - (11.2923282 * p)) ;
	pid3 = 36.9120386- (-23.3399876* atan(p - 15.9443554)) + exp(2.64833668 - (16.6946182 * p)) ;

	double elu3 = -5887.94566- (-3765.47451* atan(p - -153.022563)) + exp(0.28993272 - (0.958793402 * p)) ;
	double eld3 = -33178.2546- (-21145.3256* atan(p - -607.523744)) + exp(5.11130563 - (88.0686011 * p)) ;

	*/

	double factor = 1.0 ;
	
	if(p < 0.7) 
	{
		factor = 1.0 + fabs(p - 0.7) ;
	}

	ku3 =  3.56575287- (-0.015964108* p) + exp(3.8129127 - (6.37289422*p)) * factor ;
	kd3 =  2.09219508- (-0.0255047903* p) + exp(3.19090268 - (5.40106319*p)) ;

	piu3 = (44752.1802- (28507.4293* atan(p - -924.395828)) + exp(3.01618089 - (11.2923282 * p))) - 0.5 ;
	pid3 = 36.9120386- (-23.3399876* atan(p - 15.9443554)) + exp(2.64833668 - (16.6946182 * p)) ;

	double elu3 = -5887.94566- (-3765.47451* atan(p - -153.022563)) + exp(0.28993272 - (0.958793402 * p)) ;
	double eld3 = -33178.2546- (-21145.3256* atan(p - -607.523744)) + exp(5.11130563 - (88.0686011 * p)) ;

  }
}

//----------------------------------------------------------------------------------------------------

bool PromptAnalyzer::isPionCurve(double p, double dEdx)
{
  double piu3, kd3, ku3, pid3 ;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  return (dEdx < piu3);
}

//----------------------------------------------------------------------------------------------------

bool PromptAnalyzer::isSafePionCurve(double p, double dEdx)
{
  const double max = 0.9 / p;
  return (dEdx < max);
}

bool PromptAnalyzer::isSafePionCurveNew(double p, double dEdx)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  const double lb = 0;
  const double ub = min(kd3, piu3);

  return ((dEdx > lb) && (dEdx < ub));
}

bool PromptAnalyzer::isSuperSafePionCurveNew(double p, double dEdx)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);
  
  piu3 = 0.9 * piu3 ;

  const double lb = 0;
  const double ub = min(kd3, piu3);

  return ((dEdx > lb) && (dEdx < ub));
}

//----------------------------------------------------------------------------------------------------

bool PromptAnalyzer::isKaonCurve(double p, double dEdx)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  const double lb = max(kd3, piu3);
  const double ub = ku3;

  return (dEdx > lb && dEdx < ub);
}

bool PromptAnalyzer::isKaonCurveSpecial(double p, double dEdx, double step)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  double delta = ((double)step) * (fabs(ku3-kd3) / 25.) ;

  if(p < 0.4) delta = 0.0 ;

  const double lb = max(kd3, piu3) - delta ;
  const double ub = (ku3 + delta);

  return (dEdx > lb && dEdx < ub);
}


bool PromptAnalyzer::isKaonCurveRobert(double p, double dEdx)
{
	bool iskappa = false;
   
	double piu3, kd3, ku3, pid3;
	evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

	if(dEdx < max(ku3,piu3) && dEdx > kd3) iskappa = true;

  return iskappa;
}


//----------------------------------------------------------------------------------------------------

bool PromptAnalyzer::isProtonCurve(double p, double dEdx)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  return (dEdx > ku3);
}

//----------------------------------------------------------------------------------------------------

double  PromptAnalyzer::interpolationPionCurve(double p, double dEdx, double interpolation)
{
  double piu3, kd3, ku3, pid3;
  evaluateDEDxCurves(p, piu3, kd3, ku3, pid3);

  return (pid3 + ((piu3 - pid3) * interpolation)) ;
}

//----------------------------------------------------------------------------------------------------

void PromptAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//----------------------------------------------------------------------------------------------------

TCanvas *PromptAnalyzer::plot(TH1F *plot_for_text)
{
        map<string, int>::iterator it ;
	
	TCanvas *canvas = new TCanvas() ;
	canvas->cd() ;
	
	string name(plot_for_text->GetName()) ;
	canvas->SetName((name + "_canvas").c_str()) ;
	
	plot_for_text->Draw("ap") ;

	int i = 0 ;

	for (it = map_of_conditions_if_they_are_applied.begin(); it != map_of_conditions_if_they_are_applied.end(); ++it)
	{
		if(it->second == 1)
		{
			TLatex *new_text = new TLatex(0, i*0.05 , it->first.c_str()) ;
			new_text->Draw("same") ;
			++i ;
		}
	}	

	return canvas ;
}

const int mode_pz = 1 ; 	// pz
const int mode_pz_and_pt = 2 ; 	// pt and pz

double PromptAnalyzer::factor_for_rho_mass_constraint(TLorentzVector lv, double rhoMass, int mode)
{
	double result = 0 ;

	if(mode == mode_pz)
	{
		result = (sqrt(((lv.E()*lv.E()) - (lv.Px() * lv.Px()) - (lv.Py() * lv.Py()) - (rhoMass*rhoMass)) / (lv.Pz() * lv.Pz()))) ;
	}
	else if(mode == mode_pz_and_pt)
	{
		double numerator   = ((lv.E()*lv.E())  - (rhoMass*rhoMass)) ;
		double denominator = ((lv.Px() * lv.Px()) + (lv.Py() * lv.Py()) + (lv.Pz() * lv.Pz())) ;
		
		result = sqrt(numerator / denominator) ;
	}
	else
	{
		exit(1) ;
	}

	return result ;
}

// TODO: Implement new hit reconstruction?

bool PromptAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // defaults
  bool returnStatus = false;

  // get input
  edm::Handle<TrackCollection> tracks;
  iEvent.getByToken(trkToken_, tracks);

  edm::Handle<edm::ValueMap<reco::DeDxData>> dedxs;
  iEvent.getByToken(dedxToken_, dedxs);

  edm::Handle<edm::ValueMap<reco::DeDxData>> dedxPIXs;
  iEvent.getByToken(dedxPIXToken_, dedxPIXs);

  edm::Handle<edm::ValueMap<reco::DeDxData>> myenergyLossPixHits;
  iEvent.getByToken(dedxPixHitsToken_, myenergyLossPixHits);

  edm::Handle<edm::ValueMap<reco::DeDxData>> myenergyLossStrHits;
  iEvent.getByToken(dedxStrHitsToken_, myenergyLossStrHits);

  edm::Handle<edm::ValueMap<reco::DeDxData>> myenergyLossAllHits;
  iEvent.getByToken(dedxAllHitsToken_, myenergyLossAllHits);
  
  Handle<DeDxDataValueMap> dEdxTrackHandleTest;
  iEvent.getByToken(DeDxDataToken_, dEdxTrackHandleTest);;

  //edm::Handle<reco::DeDxHitInfoAss> dedxpixels;
  //iEvent.getByToken(dedxpixelToken_, dedxpixels);

  edm::Handle<VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);

  edm::Handle<PFCandidateCollection> pfs;
  iEvent.getByToken(pfToken_, pfs);

  //edm::Handle<MuonCollection> muons;
  //iEvent.getByToken(muToken_, muons);

  //edm::Handle<reco::DeDxHitInfoAss> dedxCollH;
  //iEvent.getByLabel("dedxHitInfo", dedxCollH);

  edm::Handle<edm::DetSetVector<TotemRPRecHit>> hRPRecHits;
  iEvent.getByToken(rpRecHitToken_, hRPRecHits);

  edm::Handle<edm::DetSetVector<TotemRPUVPattern>> hRPPatterns;
  iEvent.getByToken(rpPatternToken_, hRPPatterns);

  edm::Handle<vector<CTPPSLocalTrackLite>> hRPTracks;
  iEvent.getByToken(rpTrackToken_, hRPTracks);

  
  RunNumber_t 	my_run  = iEvent.run() ;
  // EventNumber_t my_event = iEvent.id() ;
  
  // cout << "eventprintout " << iEvent.run() << endl ;
  // cout << endl ;
  // cout << "eventprintout " << iEvent.id() << endl ;

  //if (!dedxpixels.isValid())
  //.  printf("Invalid dedxCollH\n");

  int LS = iEvent.luminosityBlock();
  histosTH1F["hLS"]->Fill(LS);

  int ntrk0 = 0;
  int ntrk  = 0;

  bool fiducialRegion = false;

  double etaCut = 2.5;

  int totcharge = 0;

  // tracks in 2-track-events (npixelhits>0)
  TLorentzVector pi1(0.,0.,0.,0.);
  TLorentzVector pi2(0.,0.,0.,0.);
  TLorentzVector pipiRec(0.,0.,0.,0.);

  TLorentzVector k1(0.,0.,0.,0.);
  TLorentzVector k2(0.,0.,0.,0.);
  TLorentzVector kkRec(0.,0.,0.,0.);

  // tracks in 4-track-events (npixelhits>0)
  TLorentzVector pi4pos1(0.,0.,0.,0.);
  TLorentzVector pi4pos2(0.,0.,0.,0.);
  TLorentzVector pi4neg1(0.,0.,0.,0.);
  TLorentzVector pi4neg2(0.,0.,0.,0.);
  TLorentzVector pi4Rec(0.,0.,0.,0.);

  TLorentzVector k4pos1(0.,0.,0.,0.);
  TLorentzVector k4pos2(0.,0.,0.,0.);
  TLorentzVector k4neg1(0.,0.,0.,0.);
  TLorentzVector k4neg2(0.,0.,0.,0.);
  TLorentzVector k4Rec(0.,0.,0.,0.);

  bool iskaon4pos[2] = {false,false};
  bool iskaon4neg[2] = {false,false};

  bool ispion4pos[2] = {false,false};
  bool ispion4neg[2] = {false,false};

  int ntrk4pos=0;
  int ntrk4neg=0;

  /*
  bool is2PIsample = false;
  bool is2Ksample = false;

  bool is2PIsample2 = false;
  bool is2Ksample2 = false;

  bool is2PIsample3 = false;
  bool is2Ksample3 = false;

  bool is2PIsample4 = false;
  bool is2Ksample4 = false;

  bool is2PIsample5 = false;
  bool is2Ksample5 = false;
  */

  //----------------------------------------------------------------------
  // process tracks

  double sum_of_px = 0 ;
  double sum_of_py = 0 ;
  double sum_of_pz = 0 ;

  bool test_event_for_deposit = false ;
  EventNumber_t my_event = iEvent.id().event() ;
  
  if(test_event_for_deposit && (my_event != 1309147036)) return false ;
  
  // ifstream events_with_reasonable_number_of_dedx_measurments_file(events_with_reasonable_number_of_dedx_measurments) ;
  
  // if(!events_with_reasonable_number_of_dedx_measurments_file.is_open()) exit(1) ;
  
  EventNumber_t anevent ;
  
  bool is_event_with_reasonable_number_of_dedx_measurments = false ;
  
  //while(events_with_reasonable_number_of_dedx_measurments_file >> anevent)
  //{
    //if(anevent == my_event) is_event_with_reasonable_number_of_dedx_measurments = true ;
  //}
  
  // events_with_reasonable_number_of_dedx_measurments_file.close() ;

  int itref=0;

  // cout << "ok" << endl ;

  // returnStatus = true ;
  
  vector<double > CHIN_4TRK ;

  // vector<double > PTERR_4TRK ;
  
  const bool mass_scanning =        true ;
  const bool plots_per_run_number = true ;
  const bool invert_all_switch =    false ;
  const bool use_PDG_rho_mass =     true ;

  const bool mimic_Ferenc_cut_for_2023_01_26_check_of_phi_phi_2SSK =    true ;

  bool use_vertex_cuts =            true  ;
  bool use_nConfClean =             true ;
  bool use_pT_cut =                 true ;
  bool use_isElastic =              true ;
  bool skip_check_npixelhits =      false  ;
  bool skip_check_npixelhits_2 =    false  ;
  bool skip_allCuts4 =              false  ;
  bool skip_track_and_charge_and_vertex_cut = false ;
  bool use_etaCutSatisfied = 	      true ;
  bool use_allTracksHavePixelHit =  true ;
  
  bool Ken_test_for_non_exclusive_events = false ;

  if(invert_all_switch)
  {
  	use_vertex_cuts =	                false  ;
  	use_nConfClean =	                false ;
  	use_pT_cut =		                  false ;
  	use_isElastic = 	                false ;

  	skip_check_npixelhits =           true  ;
  	skip_check_npixelhits_2 =         true  ;
  	skip_allCuts4 = 	                true  ;
  	skip_track_and_charge_and_vertex_cut = true ;

  	use_etaCutSatisfied =	            false ;
  	use_allTracksHavePixelHit =       false ;
  }
  
  if(mimic_Ferenc_cut_for_2023_01_26_check_of_phi_phi_2SSK)
  {
    use_vertex_cuts = 			                false  ;
    use_nConfClean =  			                false ;
    use_pT_cut =					                  false ;
    use_isElastic =				                  false ;
    skip_check_npixelhits = 	              true  ;
    skip_check_npixelhits_2 =               true  ;
    skip_allCuts4 =				                  false  ;
    skip_track_and_charge_and_vertex_cut =  false ;
    use_etaCutSatisfied =		                false ;
    use_allTracksHavePixelHit =             false ;
  }

  bool skimming = false ;

  if(skimming) 					            histosTH1F["general_status_bits"]->Fill(0) ;
  if(use_vertex_cuts) 				      histosTH1F["general_status_bits"]->Fill(1) ;
  if(use_nConfClean) 				        histosTH1F["general_status_bits"]->Fill(2) ;
  if(use_pT_cut) 				            histosTH1F["general_status_bits"]->Fill(3) ;
  if(use_isElastic) 				        histosTH1F["general_status_bits"]->Fill(4) ;
  if(skip_check_npixelhits) 	      histosTH1F["general_status_bits"]->Fill(5) ;
  if(skip_check_npixelhits_2) 			histosTH1F["general_status_bits"]->Fill(6) ;
  if(skip_allCuts4) 				        histosTH1F["general_status_bits"]->Fill(7) ;
  if(skip_track_and_charge_and_vertex_cut) 	histosTH1F["general_status_bits"]->Fill(8) ;
  if(use_etaCutSatisfied) 			    histosTH1F["general_status_bits"]->Fill(9) ;
  if(use_allTracksHavePixelHit) 		histosTH1F["general_status_bits"]->Fill(10) ;

  if(mimic_Ferenc_cut_for_2023_01_26_check_of_phi_phi_2SSK) 		histosTH1F["general_status_bits"]->Fill(11) ;

  for (TrackCollection::const_iterator itTrack = tracks->begin(); itTrack != tracks->end(); ++itTrack)
  {
	int looper = itTrack->isLooper();

	//double p = itTrack->p();

	double 	pt = itTrack->pt();
	double 	pz = itTrack->pz();
	double 	eta = itTrack->eta();
	double 	phi = itTrack->phi();
	int 	charge = itTrack->charge();
	int 	npixelhits = itTrack->hitPattern().numberOfValidPixelHits();
	//int 	nstriphits = itTrack->hitPattern().numberOfValidStripHits();
	int 	algo = itTrack->algo();
	double 	chi2 = itTrack->normalizedChi2();

	//double ept=itTrack->ept;

	// vector<TPixelHit>::const_reverse_iterator hit = itTrack->pixelHits.rbegin();

    
	if(my_run == 319174)
	{

		histosTH1F["hpt2_run_319174"]->Fill(pt); 
		histosTH1F["hpx_run_319174"]->Fill(itTrack->px()); 
		histosTH1F["hpy_run_319174"]->Fill(itTrack->py()); 
		histosTH1F["hpz_run_319174"]->Fill(pz); 
		
		// cout << "eventprintout " << iEvent.run() << " " << iEvent.id() << endl ;
	}

    CHIN_4TRK.push_back(chi2) ;
    //PTERR_4TRK.push_back(ept) ;

    double 	d0 = itTrack->d0();
    double 	dz = itTrack->dz();

    double 	px = itTrack->px();
    double 	py = itTrack->py();

    sum_of_px += px ;
    sum_of_py += py ;
    sum_of_pz += pz ;

    // dEdx
    reco::TrackRef trackRef = reco::TrackRef(tracks, itref);

    double thisdedx = (*dedxs)[trackRef].dEdx();
    double thisdedxPIX = (*dedxPIXs)[trackRef].dEdx();


    double thisdedxPixHits = 0 ;
    double thisdedxStrHits = 0 ;
    double thisdedxAllHits = 0 ;


    if(myenergyLossPixHits.isValid())
    {
       if(myenergyLossPixHits->size() > trackRef.key()) 
       {
          thisdedxPixHits = myenergyLossPixHits->get(trackRef.key()).dEdx();
       }
    }

    if(myenergyLossStrHits.isValid())
    {
       if(myenergyLossStrHits->size() > trackRef.key())
       {
          thisdedxStrHits = myenergyLossStrHits->get(trackRef.key()).dEdx();
       }
    }

	 // cout << "myenergyLossAllHits->size(): " << myenergyLossAllHits->size() << " " << endl ;
    if(myenergyLossAllHits.isValid())
    {
       if(myenergyLossAllHits->size() > trackRef.key()) 
       {
	        
          thisdedxAllHits = myenergyLossAllHits->get(trackRef.key()).dEdx();
			 // cout << "thisdedxAllHits" << thisdedxAllHits << endl ;
	  if(!use_old_dEdx) thisdedxPIX = thisdedxAllHits ;
       }
       
       if(use_most_consistent_way_of_dEdx)
	{
	       thisdedxAllHits = (*myenergyLossAllHits)[trackRef].dEdx();
	       thisdedxPIX = (*myenergyLossAllHits)[trackRef].dEdx();
	}
	
       if(use_Michael_solution)
       {
       		if(dEdxTrackHandleTest.isValid() && (dEdxTrackHandleTest->size() > trackRef.key()))
		{
			DeDxDataValueMap dEdxTracktest = *dEdxTrackHandleTest.product();
			double test_dedx = dEdxTracktest[trackRef].dEdx();
			// cout << "blabla" << endl ;
		}
       }
	

    }
	 else
	 {
	 	// cout << "Warning: myenergyLossAllHits not valid." << endl ;
	 }

//    cout << "mytest31: " << iEvent.run() << " " <<  iEvent.id() << " " << trackRef.key() << " " << itTrack->p() << " " << thisdedxAllHits << endl ;


    itref++;


    if(skimming || skip_check_npixelhits || (npixelhits > 0))
    {

	map_of_conditions_if_they_are_applied["if (npixelhits > 0)"] = 1 ;

      int hdedxDetector = 0;

      if (thisdedxPIX > 0)
      {
        hdedxDetector = 2;
        histosTH2F["hdedxAllPIX"]->Fill(itTrack->p(),thisdedxPIX);
      } else if(thisdedx > 0)
      {
        hdedxDetector = 1;
        histosTH2F["hdedxAllSTR"]->Fill(itTrack->p(),thisdedx);
      }

      histosTH1F["hdedxDetector"]->Fill(hdedxDetector);
      histosTH2F["hdedxSTRvsPIX"]->Fill(thisdedxPIX,thisdedx);

      if(thisdedxPixHits) histosTH2F["hdedxPixHits"]->Fill(itTrack->p(),thisdedxPixHits);
      if(thisdedxStrHits) histosTH2F["hdedxStrHits"]->Fill(itTrack->p(),thisdedxStrHits);
      if(thisdedxAllHits)
      {
	histosTH2F["hdedxAllHits"]->Fill(itTrack->p(),thisdedxAllHits);

      }


      /*
      if ( isPionCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.6 ) is2PIsample=true;
      if ( isKaonCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.5 ) is2Ksample=true;

      if ( isPionCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.35 ) is2PIsample2=true;
      if ( isKaonCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.35 ) is2Ksample2=true;

      if ( isPionCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.40 ) is2PIsample3=true;
      if ( isKaonCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.40 ) is2Ksample3=true;

      if ( isPionCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.45 ) is2PIsample4=true;
      if ( isKaonCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.45 ) is2Ksample4=true;

      if ( isPionCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.50 ) is2PIsample5=true;
      if ( isKaonCurve(itTrack->p(),thisdedxPIX) && itTrack->p()<0.50 ) is2Ksample5=true;
      */

      histosTH1F["hpt"]->Fill(pt);
      histosTH1F["heta"]->Fill(eta);
      histosTH1F["hphi"]->Fill(phi);
      histosTH1F["halgo"]->Fill(algo);
      //histosTH1F["hnhits"]->Fill(npixelhits+nstriphits);
      histosTH1F["hnhits"]->Fill(npixelhits);

      histosTH1F["hlooper"]->Fill(looper);
      histosTH1F["hchi2"]->Fill(chi2);
      histosTH1F["hd0"]->Fill(d0);
      histosTH1F["hdz"]->Fill(dz);

      totcharge += charge;

      // four-momenta with different mass hypotheses
      const double ene_pi = sqrt((pt*pt) + (pz*pz) + (m_pi*m_pi));
      const TLorentzVector trk_lorentz_pi(itTrack->px(), itTrack->py(), itTrack->pz(), ene_pi);

      const double ene_K = sqrt((pt*pt) + (pz*pz) + (m_k*m_k));
      const TLorentzVector trk_lorentz_K(itTrack->px(), itTrack->py(), itTrack->pz(), ene_K);

      //--------------------------------------
      // 2 trk

      if (ntrk == 0) pi1 = trk_lorentz_pi;
      if (ntrk == 1) pi2 = trk_lorentz_pi;

      if (ntrk == 0) k1 = trk_lorentz_K;
      if (ntrk == 1) k2 = trk_lorentz_K;

      pipiRec += trk_lorentz_pi;
      kkRec   += trk_lorentz_K;

      //--------------------------------------
      // 4trk

      pi4Rec += trk_lorentz_pi;
      k4Rec  += trk_lorentz_K;

      if (charge > 0)
      {
        if (ntrk4pos == 0) pi4pos1 = trk_lorentz_pi;
        if (ntrk4pos == 1) pi4pos2 = trk_lorentz_pi;

        if (ntrk4pos == 0) k4pos1 = trk_lorentz_K;
        if (ntrk4pos == 1) k4pos2 = trk_lorentz_K;

        if (ntrk4pos < 2)
        {
          if( isPionCurve(itTrack->p(),thisdedxPIX) ) ispion4pos[ntrk4pos] = true;
          if( isKaonCurve(itTrack->p(),thisdedxPIX) ) iskaon4pos[ntrk4pos] = true;
        }

        ntrk4pos++;
      }

      if (charge < 0)
      {
        if (ntrk4neg == 0) pi4neg1 = trk_lorentz_pi;
        if (ntrk4neg == 1) pi4neg2 = trk_lorentz_pi;

        if (ntrk4neg == 0) k4neg1 = trk_lorentz_K;
        if (ntrk4neg == 1) k4neg2 = trk_lorentz_K;

        if (ntrk4neg < 2)
        {
          if ( isPionCurve(itTrack->p(),thisdedxPIX) ) ispion4neg[ntrk4pos] = true;
          if ( isKaonCurve(itTrack->p(),thisdedxPIX) ) iskaon4neg[ntrk4pos] = true;
        }

        ntrk4neg++;
      }

      ntrk++; // pixel tracks
      
      map_of_conditions_if_they_are_applied["if (npixelhits > 0)"] = 0 ;
    }
    
    double my_pt = sqrt((sum_of_px * sum_of_px) + (sum_of_py * sum_of_py)) ;
    
    histosTH2F["CMS_pt_vs_my_pt"]->Fill(pt, my_pt);

    ntrk0++; // all tracks
  }

  double sum_of_pt = sqrt((sum_of_px * sum_of_px) + (sum_of_py * sum_of_py)) ;

  // if(sum_of_pt > 0.8) return returnStatus ;

  histosTH1F["hntrk0"]->Fill(ntrk0);
  histosTH1F["hntrk"]->Fill(ntrk);

  //----------------------------------------------------------------------
  // process vertex and vertex tracks

  int nvtx=0;
  int ntrkvtx=0;
  
  vector<double> vector_vertex_z_to_test_Ferenc ;

  for (VertexCollection::const_iterator itVtx = vertices->begin(); itVtx != vertices->end(); ++itVtx)
  {
    const int vtxisfake = itVtx->isFake();
    if (vtxisfake == 0)
      nvtx++;
    else
      continue;

    if (nvtx == 1) // executed for the first vertex found
    {
      ntrkvtx = 0; // count the number of tracks associted with the vertex

      for (auto tv=itVtx->tracks_begin(); tv!=itVtx->tracks_end(); tv++)
      {
        const reco::TrackRef trackRef = tv->castTo<reco::TrackRef>();

        int looper = trackRef->isLooper();
        double pt = trackRef->pt();
        //double pz = trackRef->pz();
        double eta = trackRef->eta();
        double phi = trackRef->phi();
        //int charge = trackRef->charge();
        int npixelhits = trackRef->hitPattern().numberOfValidPixelHits();
        //int nstriphits = trackRef->hitPattern().numberOfValidStripHits();
        int algo = trackRef->algo();
        double chi2 = trackRef->normalizedChi2();
        double d0 = trackRef->d0();
        double dz = trackRef->dz();

        histosTH1F["hptv"]->Fill(pt);
        histosTH1F["hetav"]->Fill(eta);
        histosTH1F["hphiv"]->Fill(phi);
        histosTH1F["halgov"]->Fill(algo);
        //histosTH1F["hnhitsv"]->Fill(npixelhits+nstriphits);
        histosTH1F["hnhitsv"]->Fill(npixelhits);

        histosTH1F["hlooperv"]->Fill(looper);

        histosTH1F["hchi2v"]->Fill(chi2);
        histosTH1F["hd0v"]->Fill(d0);
        histosTH1F["hdzv"]->Fill(dz);

        ntrkvtx++;
      }
    }
  }

  histosTH1F["hnvtx"]->Fill(nvtx);
  histosTH1F["hntrkvtx"]->Fill(ntrkvtx);

  // not yet vertex cut, because checking vertex efficiency

  int isfake = vertices->begin()->isFake();
  double xvtx = vertices->begin()->x();
  double yvtx = vertices->begin()->y();
  double zvtx = vertices->begin()->z();
  double chi2vtx = vertices->begin()->normalizedChi2();

  fiducialRegion = ((ntrk == 2) && (TMath::Abs(pi1.Eta()) < etaCut) && (TMath::Abs(pi2.Eta()) < etaCut)) ;

  histosTH1F["hvtx"]->Fill( isfake );
  if(ntrk == 2)
  {
    histosTH1F["hvtx2"]->Fill( isfake );
    if(fiducialRegion && totcharge==0) histosTH1F["hvtx3"]->Fill(isfake);
  }

  //  fiducialRegion = fiducialRegion && nvtx==1;
  //

  if((!skimming) && (use_vertex_cuts) && (nvtx != 1)) { return returnStatus ; }

  histosTH1F["hvtxx"]->Fill(xvtx);
  histosTH1F["hvtxy"]->Fill(yvtx);
  histosTH1F["hvtxz"]->Fill(zvtx);
  histosTH1F["hvtxchi2"]->Fill(chi2vtx);

  // cut on vertex z position
  if((!skimming) && (use_vertex_cuts) && (fabs(zvtx) > 24)) return returnStatus;
  
  if(mimic_Ferenc_cut_for_2023_01_26_check_of_phi_phi_2SSK)
  {
    
    // if((fabs(xvtx) > 1) || (fabs(yvtx) > 1))) return returnStatus;
  }

  histosTH1F["hvtxx_after_mimic_cut"]->Fill(xvtx);
  histosTH1F["hvtxy_after_mimic_cut"]->Fill(yvtx);
  histosTH1F["hvtxz_after_mimic_cut"]->Fill(zvtx);

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

  double xLN = 100, xLF = 100, yLN = 100, yLF = 100;
  double xRN = 100, xRF = 100, yRN = 100, yRF = 100;

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

  // buffer for RP data
  struct StripInfo
  {
      set<unsigned int> u_planes;
      set<unsigned int> v_planes;

      map<unsigned int, unsigned int> clustersPerPlane;

      bool u_tooFull = false;
      bool v_tooFull = false;

      unsigned int u_patterns = 0;
      unsigned int v_patterns = 0;

      bool hasActiv = false;
      bool hasTrack = false;
  };

  map<unsigned int, StripInfo> rpInfo;

  // process rec hits
  for (const auto &dsRecHits : *hRPRecHits)
  {
      TotemRPDetId planeId(dsRecHits.detId());
      unsigned int rpDecId  = (planeId.arm()*100) + (planeId.station()*10) + planeId.rp();
      unsigned int planeIdx = planeId.plane();

      rpInfo[rpDecId].clustersPerPlane[planeIdx] = dsRecHits.size();

      if (dsRecHits.size() == 0) continue;

      if ((planeIdx % 2) == 0) rpInfo[rpDecId].v_planes.insert(planeIdx);
      else
		rpInfo[rpDecId].u_planes.insert(planeIdx);
  }

  // process patterns
  for (const auto &dsPatterns : *hRPPatterns)
  {
      TotemRPDetId rpId(dsPatterns.detId());
      unsigned int rpDecId = (rpId.arm()*100) + (rpId.station()*10) + rpId.rp();

      for (const auto &pat : dsPatterns)
      {
          if (! pat.getFittable()) continue;

          if (pat.getProjection() == TotemRPUVPattern::projU) rpInfo[rpDecId].u_patterns++;
          if (pat.getProjection() == TotemRPUVPattern::projV) rpInfo[rpDecId].v_patterns++;
      }
  }

  // process track data
  for (const auto &tr : *hRPTracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    unsigned int rpDecId = (100*rpId.arm()) + (10*rpId.station()) + (1*rpId.rp());

    rpInfo[rpDecId].hasTrack = true;

    if(rpDecId == 4) 	{rp_valid_004 = true; xLN = tr.getX() + mean_x4; yLN = tr.getY() + mean_y4;}
    if(rpDecId == 5) 	{rp_valid_005 = true; xLN = tr.getX() + mean_x5; yLN = tr.getY() + mean_y5;}

    if(rpDecId == 24) 	{rp_valid_024 = true; xLF = tr.getX() + mean_x24; yLF = tr.getY() + mean_y24;}
    if(rpDecId == 25) 	{rp_valid_025 = true; xLF = tr.getX() + mean_x25; yLF = tr.getY() + mean_y25;}

    if(rpDecId == 104) 	{rp_valid_104 = true; xRN = tr.getX() + mean_x104; yRN = tr.getY() + mean_y104;}
    if(rpDecId == 105) 	{rp_valid_105 = true; xRN = tr.getX() + mean_x105; yRN = tr.getY() + mean_y105;}

    if(rpDecId == 124) 	{rp_valid_124 = true; xRF = tr.getX() + mean_x124; yRF = tr.getY() + mean_y124;}
    if(rpDecId == 125) 	{rp_valid_125 = true; xRF = tr.getX() + mean_x125; yRF = tr.getY() + mean_y125;}
  }

  // process all RP inputs
  for (auto &p : rpInfo)
  {
      auto &info = p.second;

      unsigned int n_too_full_u = 0, n_too_full_v = 0;

      for (const auto &clP : info.clustersPerPlane)
      {
          if (clP.second <= 5) continue;

          if ((clP.first % 2) == 1) n_too_full_u++;
          else n_too_full_v++;
      }

      info.u_tooFull = (n_too_full_u >= 3);
      info.v_tooFull = (n_too_full_v >= 3);

      info.hasActiv = (info.hasTrack || info.u_patterns > 0 || info.v_patterns > 0 || info.u_tooFull || info.v_tooFull) ;
  }

  bool diag_top45_bot56 = (rp_valid_024 && rp_valid_004 && rp_valid_105 && rp_valid_125);
  bool diag_bot45_top56 = (rp_valid_025 && rp_valid_005 && rp_valid_104 && rp_valid_124);

  bool top45_top56      = (rp_valid_024 && rp_valid_004 && rp_valid_104 && rp_valid_124);
  bool bot45_bot56      = (rp_valid_025 && rp_valid_005 && rp_valid_105 && rp_valid_125);

  bool clean_bot45_top56 = ((!rpInfo[24].hasActiv && rpInfo[25].hasTrack) && (!rpInfo[4].hasActiv && rpInfo[5].hasTrack) && (rpInfo[104].hasTrack && !rpInfo[105].hasActiv) && (rpInfo[124].hasTrack && !rpInfo[125].hasActiv));
  bool clean_top45_bot56 = ((rpInfo[24].hasTrack && !rpInfo[25].hasActiv) && (rpInfo[4].hasTrack && !rpInfo[5].hasActiv) && (!rpInfo[104].hasActiv && rpInfo[105].hasTrack) && (!rpInfo[124].hasActiv && rpInfo[125].hasTrack));
  bool clean_top45_top56 = ((rpInfo[24].hasTrack && !rpInfo[25].hasActiv) && (rpInfo[4].hasTrack && !rpInfo[5].hasActiv) && (rpInfo[104].hasTrack && !rpInfo[105].hasActiv) && (rpInfo[124].hasTrack && !rpInfo[125].hasActiv));
  bool clean_bot45_bot56 = ((!rpInfo[24].hasActiv && rpInfo[25].hasTrack) && (!rpInfo[4].hasActiv && rpInfo[5].hasTrack) && (!rpInfo[104].hasActiv && rpInfo[105].hasTrack) && (!rpInfo[124].hasActiv && rpInfo[125].hasTrack));

  int nconf=0;
  if (diag_top45_bot56) nconf++;
  if (diag_bot45_top56) nconf++;
  if (top45_top56) nconf++;
  if (bot45_bot56) nconf++;

  unsigned int nConfClean = 0;
  if (clean_top45_bot56) nConfClean++;
  if (clean_bot45_top56) nConfClean++;
  if (clean_top45_top56) nConfClean++;
  if (clean_bot45_bot56) nConfClean++;

  histosTH1F["hnConf"]->Fill(nconf);
  histosTH1F["hnConfClean"]->Fill(nConfClean);

  if((!skimming) && (use_nConfClean) && (nConfClean != 1)) return returnStatus;
  
  bool test_dedx = false ;  
  if(test_dedx) cout << "Event_nConfClean_ok" << endl ;

  // topology: 1 - TB, 2 - BT, 3 - TT, 4 - BB
  int tb = -1;
  if (clean_top45_bot56) tb = 0;
  if (clean_bot45_top56) tb = 1;
  if (clean_top45_top56) tb = 2;
  if (clean_bot45_bot56) tb = 3;

  histosTH1F["hConf"]->Fill(tb);

  // single-arm kinematics reconstruction
  double ThxR, ThyR, ThxL, ThyL, xVtxL, xVtxR;

  double D_x_L = + v_x_L_1_F * L_x_L_2_F - v_x_L_2_F * L_x_L_1_F;
  ThxL = (v_x_L_1_F * xLF - v_x_L_2_F * xLN) / D_x_L;
  xVtxL = (- xLN * L_x_L_2_F + xLF * L_x_L_1_F) / D_x_L;

  double D_x_R = + v_x_R_1_F * L_x_R_2_F - v_x_R_2_F * L_x_R_1_F;
  ThxR = (v_x_R_1_F * xRF - v_x_R_2_F * xRN) / D_x_R;
  xVtxR = -(+ xRN * L_x_R_2_F - xRF * L_x_R_1_F) / D_x_R;

  double th_y_L_1_F = + yLN / L_y_L_1_F;
  double th_y_L_2_F = + yLF / L_y_L_2_F;
  ThyL = (th_y_L_1_F + th_y_L_2_F) / 2.;

  double th_y_R_1_F = + yRN / L_y_R_1_F;
  double th_y_R_2_F = + yRF / L_y_R_2_F;
  ThyR = (th_y_R_1_F + th_y_R_2_F) / 2.;

  double D_y_L = + v_y_L_1_F * L_y_L_2_F - v_y_L_2_F * L_y_L_1_F;
  double D_y_R = + v_y_R_1_F * L_y_R_2_F - v_y_R_2_F * L_y_R_1_F;

  double yVtxL = (- yLN * L_y_L_2_F + yLF * L_y_L_1_F) / D_y_L;
  double yVtxR = (+ yRN * L_y_R_2_F - yRF * L_y_R_1_F) / D_y_R;
  
  // Version by Ferenc

  double D_x_L_Ferenc = + v_x_L_1_F * L_x_L_2_F_Ferenc - v_x_L_2_F * L_x_L_1_F_Ferenc;
  double ThxL_Ferenc = (v_x_L_1_F * xLF - v_x_L_2_F * xLN) / D_x_L_Ferenc;
  double xVtxL_Ferenc = (- xLN * L_x_L_2_F_Ferenc + xLF * L_x_L_1_F_Ferenc) / D_x_L_Ferenc;

  double D_x_R_Ferenc = + v_x_R_1_F * L_x_R_2_F_Ferenc - v_x_R_2_F * L_x_R_1_F_Ferenc;
  double ThxR_Ferenc = (v_x_R_1_F * xRF - v_x_R_2_F * xRN) / D_x_R_Ferenc;
  double xVtxR_Ferenc = -(+ xRN * L_x_R_2_F_Ferenc - xRF * L_x_R_1_F_Ferenc) / D_x_R_Ferenc;

  // Version Frici
  
  double D_x_L_full_nom = + v_x_L_1_F_Frici * L_x_L_2_F_Frici - v_x_L_2_F_Frici * L_x_L_1_F_Frici;
  double ThxL_full_nom = (v_x_L_1_F_Frici * xLF - v_x_L_2_F_Frici * xLN) / D_x_L_full_nom;
  double xVtxL_full_nom = (- xLN * L_x_L_2_F_Frici + xLF * L_x_L_1_F_Frici) / D_x_L_full_nom;
  
  const double meter_to_mm_conversion_factor = 1000.0 ;
  const double GeV_to_MeV_conversion_factor = 1000.0 ;

  const double distance_of_RPs_meter = 7.0 ;
  const double distance_of_RPs_mm = distance_of_RPs_meter * meter_to_mm_conversion_factor ;
  
  const double theta_x_L = ((xLF - xLN) / distance_of_RPs_mm) ;
  const double theta_x_R = ((xRF - xRN) / distance_of_RPs_mm) ;

  const double theta_y_L = ((yLF - yLN) / distance_of_RPs_mm) ;
  const double theta_y_R = ((yRF - yRN) / distance_of_RPs_mm) ;
  
  const double dv_x_ds_L = (v_x_L_2_F - v_x_L_1_F) / distance_of_RPs_mm ;
  const double dv_x_ds_R = (v_x_R_2_F - v_x_R_1_F) / distance_of_RPs_mm ;

  // Correct residual shifts in thx (only, not needed thy)
  // 2015
  //  if(specialreco) ThxL=rec_proton_left->thx-5.04e-5;
  // 2018
  // was on during the  for Express
  // Gauss fit: shift xL+xR = -1.80371e-04
  // ThxR += 1.815e-04;
  //
  // my calculations from shift in dpx/6500

  double a_off =  0.000002386 ; // TB
  double b_off = -0.000006593 ; // BT
  double c_off = -0.000007524 ; // TT
  double d_off =  0.000003268 ; // BB

  if (tb==0) {ThxL += 0. ;           ThxR += a_off ;} // TB
  if (tb==1) {ThxL += (b_off-c_off); ThxR += c_off ;} // BT
  if (tb==2) {ThxL += 0. ;           ThxR += c_off ;} // TT
  if (tb==3) {ThxL += (d_off-a_off); ThxR += a_off ;} // BB

  bool use_isElasticLike_cut = true ;

  bool isElasticLike = false;
  // bool isElasticLike_small_vertex = false;

  // 5 sigma
  
  const double n_x_sigma = 3 ;
  const double n_y_sigma = 3 ;
  //  const double n_x_star_sigma = 3 ;
  const double n_x_star_sigma = 0.5 ;
  
  double vertex_x_mm = (xVtxL_full_nom - xVtxR) ;
  
  double xVtx_full_nom = (xVtxL_full_nom + xVtxR) / 2.0 ;
  double xVtx = (xVtxL + xVtxR) / 2.0 ;

  
  double x1 = 1.98318E-005 ;
  double y1 = 1.00000E+000 ;

  double x1o = -1.00000E+000 ;
  double y1o = 1.98318E-005 ;
  
  bool spectrometer_cut_y = false ;

  double projection_on_normal = ((x1o*theta_y_L) + (y1o * yLN)) ;
  if((TMath::Abs(projection_on_normal - 3.13881e-06) <  2.53867e-06)) spectrometer_cut_y = true ;
  if(spectrometer_cut_y && (TMath::Abs(vertex_x_mm)< (n_x_star_sigma * 2e-2)) && (TMath::Abs(ThyL+ThyR)< (n_y_sigma * 3e-6)) && (TMath::Abs(ThxL+ThxR)< (n_x_sigma * 9e-6))) isElasticLike = true ;

  histosTH1F["TH2F_theta_y_L_yLN_cut"]->Fill(projection_on_normal); 

  //  if(isElasticLike && (TMath::Abs(xVtx_full_nom) < (0.1 / 3.0))) isElasticLike_small_vertex = true ;
  // if(isElasticLike && (TMath::Abs(xVtx_full_nom) < (0.1))) isElasticLike_small_vertex = true ;


  // if(use_isElasticLike_cut && isElasticLike && (TMath::Abs(xvtx - 0.09757) < (0.049 / 2.0)))  // CMS vertex does not work 2021.12.17

  if(use_isElasticLike_cut && isElasticLike)
 {
	  histosTH1F["hvtxx2"]->Fill(xvtx);

	  // Vertex plots

	  histosTH2F["vtxx2DIM"]->Fill(xVtxL, xVtxR);
	  histosTH2F["vtxx2DIM_Ferenc"]->Fill(xVtxL_Ferenc, xVtxR_Ferenc);
	  histosTH2F["vtxx2DIM_full_nom"]->Fill(xVtxL_full_nom, xVtxR);

	  histosTH2F["vtxy2DIM"]->Fill(yVtxL,yVtxR);

	  histosTH1F["vtxx_sum"]->Fill(xVtxL - xVtxR);
	  histosTH1F["vtxx_sum_Ferenc"]->Fill(xVtxL_Ferenc - xVtxR_Ferenc);
	  histosTH1F["vtxx_sum_full_nom"]->Fill(xVtxL_full_nom - xVtxR);

	  // Angles for rotated versions
	  
	  double hthx2DIM_Ferenc_angle = (TMath::Pi() / 4.0) ;

	  double hthx2DIM_full_nom_angle = (TMath::Pi() / 4.0) + 0.00430394 ;
	  
	  double ThxL_Ferenc_rotated = (ThxL_Ferenc * cos(hthx2DIM_Ferenc_angle)) - (ThxR_Ferenc * sin(hthx2DIM_Ferenc_angle)); 
	  double ThxR_Ferenc_rotated = (ThxL_Ferenc * sin(hthx2DIM_Ferenc_angle)) + (ThxR_Ferenc * cos(hthx2DIM_Ferenc_angle)); 

	  double ThxL_full_nom_rotated = (ThxL_full_nom * cos(hthx2DIM_full_nom_angle)) - (ThxR * sin(hthx2DIM_full_nom_angle)); 
	  double ThxR_rotated = (ThxL_full_nom * sin(hthx2DIM_full_nom_angle)) + (ThxR * cos(hthx2DIM_full_nom_angle)); 

	  // Scattering angle plots

	  histosTH2F["hthx2DIM"]->Fill(ThxL, ThxR);
	  histosTH2F["hthx2DIM_Ferenc"]->Fill(ThxL_Ferenc, ThxR_Ferenc);
	  histosTH2F["hthx2DIM_full_nom"]->Fill(ThxL_full_nom, ThxR);
	  histosTH2F["hthy2DIM"]->Fill(ThyL, ThyR);

  	  histosTH2F["hthx2DIM_Ferenc_rotated"]->Fill(ThxL_Ferenc_rotated, ThxR_Ferenc_rotated);
  	  histosTH2F["hthx2DIM_full_nom_rotated"]->Fill(ThxL_full_nom_rotated, ThxR_rotated);

	  histosTH2F["hthxthyleft2DIM"]->Fill(ThxL, ThyL);
	  histosTH2F["hthxthyright2DIM"]->Fill(ThxR, ThyR);

	  histosTH2F["hthythx2DIM"]->Fill(ThxL+ThxR, ThyL+ThyR);

	  histosTH1F["hthxEla"]->Fill(ThxL+ThxR);
	  histosTH1F["hthxEla_Ferenc"]->Fill(ThxL_Ferenc+ThxR_Ferenc);
	  histosTH1F["hthxEla_full_nom"]->Fill(ThxL_full_nom+ThxR);
	  histosTH1F["hthyEla"]->Fill(ThyL+ThyR);

          // For optics
	  
	  // Correlation plots: corrdinates left-right

	  histosTH2F["TH2F_xLN_xRN"]->Fill(xLN,xRN); 
	  histosTH2F["TH2F_xLF_xRF"]->Fill(xLF,xRF); 
	  histosTH2F["TH2F_yLN_yRN"]->Fill(yLN,yRN); 
	  histosTH2F["TH2F_yLF_yRF"]->Fill(yLF,yRF); 

	  histosTH2F["TH2F_xLN_xRN_zoom"]->Fill(xLN,xRN); 
	  histosTH2F["TH2F_xLF_xRF_zoom"]->Fill(xLF,xRF); 
	  histosTH2F["TH2F_xRF_xLF_zoom"]->Fill(xRF,xLF); 

	  histosTH2F["TH2F_xLN_xLF_zoom"]->Fill(xLN,xLF); 
	  histosTH2F["TH2F_xRN_xRF_zoom"]->Fill(xRN,xRF); 

	  histosTH2F["TH2F_xRF_xLN"]->Fill(xRF,xLN); 

	  // profiles["TProfile_xLN_xRN"]->Fill(xLN,xRN); 
	  // profiles["TProfile_xLF_xRF"]->Fill(xLF,xRF); 
	  // profiles["TProfile_yLN_yRN"]->Fill(yLN,yRN); 
	  // profiles["TProfile_yLF_yRF"]->Fill(yLF,yRF); 

	  // profiles["TProfile_xLN_xRN_zoom"]->Fill(xLN,xRN); 
	  // profiles["TProfile_xLF_xRF_zoom"]->Fill(xLF,xRF); 

	  
	  // Rotatations and other transformations
	  
	  const double theta_x_L_theta_x_R_angle = (TMath::Pi() / 4.0) - 0.0674138 - 0.00354727;
	  const double theta_x_L_theta_x_R_angle_for_subtracted = (TMath::Pi() / 4.0) - 0.0674138 - 0.00354727;
	  const double theta_x_L_xLN_swap_angle =  0.000127911 + 3.74254E-005;
	  const double theta_x_R_xRF_swap_angle = -0.000105768;
	  
	  double xLN_rotated = (xLN * cos(theta_x_L_xLN_swap_angle)) - (theta_x_L * sin(theta_x_L_xLN_swap_angle)); 
	  double theta_x_L_rotated_2 = (xLN * sin(theta_x_L_xLN_swap_angle)) + (theta_x_L * cos(theta_x_L_xLN_swap_angle)); 

	  double xRF_rotated = (xRF * cos(theta_x_R_xRF_swap_angle)) - (theta_x_R * sin(theta_x_R_xRF_swap_angle)); 
	  double theta_x_R_rotated_2 = (xRF * sin(theta_x_R_xRF_swap_angle)) + (theta_x_R * cos(theta_x_R_xRF_swap_angle)); 

	  // Vertex term subtraction

          double theta_x_L_vertex_term_subtracted = theta_x_L + (dv_x_ds_L * xVtx) ;
          double xLN_vertex_term_subtracted = xLN + (v_x_L_1_F * xVtx) ;
          double xLF_vertex_term_subtracted = xLF + (v_x_L_2_F * xVtx) ;

          double theta_x_R_vertex_term_subtracted = theta_x_R + (dv_x_ds_R * xVtx) ;
          double xRN_vertex_term_subtracted = xRN + (v_x_R_1_F * xVtx) ;
          double xRF_vertex_term_subtracted = xRF + (v_x_R_2_F * xVtx) ;

	  double theta_x_L_rotated = (theta_x_L * cos(theta_x_L_theta_x_R_angle)) - (theta_x_R * sin(theta_x_L_theta_x_R_angle)); 
	  double theta_x_R_rotated = (theta_x_L * sin(theta_x_L_theta_x_R_angle)) + (theta_x_R * cos(theta_x_L_theta_x_R_angle)); 

	  double theta_x_L_vertex_term_subtracted_rotated = (theta_x_L_vertex_term_subtracted * cos(theta_x_L_theta_x_R_angle_for_subtracted)) - (theta_x_R_vertex_term_subtracted * sin(theta_x_L_theta_x_R_angle_for_subtracted)); 
	  double theta_x_R_vertex_term_subtracted_rotated = (theta_x_L_vertex_term_subtracted * sin(theta_x_L_theta_x_R_angle_for_subtracted)) + (theta_x_R_vertex_term_subtracted * cos(theta_x_L_theta_x_R_angle_for_subtracted)); 

	  // Correlation plots: angles left-right

	  histosTH2F["TH2F_theta_x_L_theta_x_R"]->Fill(theta_x_L,theta_x_R); 
	  histosTH2F["TH2F_theta_y_L_theta_y_R"]->Fill(theta_y_L,theta_y_R); 

	  histosTH2F["TH2F_theta_x_L_theta_x_R_vertex_term_subtracted"]->Fill(theta_x_L_vertex_term_subtracted,theta_x_R_vertex_term_subtracted); 
	  histosTH2F["TH2F_theta_x_L_theta_x_R_rotated"]->Fill(theta_x_L_rotated, theta_x_R_rotated); 
	  histosTH2F["TH2F_theta_x_L_theta_x_R_vertex_term_subtracted_rotated"]->Fill(theta_x_L_vertex_term_subtracted_rotated, theta_x_R_vertex_term_subtracted_rotated); 
	  
	  // Correlation plots: angle - position 

	  histosTH2F["TH2F_theta_x_L_xLN"]->Fill(theta_x_L, xLN); 
	  histosTH2F["TH2F_theta_x_L_xLN_swap"]->Fill(xLN, theta_x_L); 
	  histosTH2F["TH2F_theta_x_L_xLN_swap_rotated"]->Fill(xLN_rotated , theta_x_L_rotated_2); 
	  histosTH2F["TH2F_theta_x_L_xLF"]->Fill(theta_x_L, xLF); 

	  histosTH2F["TH2F_theta_x_L_xLN_vertex_term_subtracted"]->Fill(theta_x_L_vertex_term_subtracted, xLN_vertex_term_subtracted); 
	  histosTH2F["TH2F_xLN_vertex_term_subtracted_xLF_vertex_term_subtracted"]->Fill(xLN_vertex_term_subtracted, xLF_vertex_term_subtracted); 

	  double theta_x_L_vertex_term_subtracted_projected_to_localxy = TH2F_theta_x_L_xLN_vertex_term_subtracted_analysis_cut->ProjectionX(theta_x_L_vertex_term_subtracted, xLN_vertex_term_subtracted) ;
	  double xLN_vertex_term_subtracted_projected_to_localxy       = TH2F_theta_x_L_xLN_vertex_term_subtracted_analysis_cut->ProjectionY(theta_x_L_vertex_term_subtracted, xLN_vertex_term_subtracted) ;

	  histosTH2F["TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy"]->Fill(theta_x_L_vertex_term_subtracted_projected_to_localxy, xLN_vertex_term_subtracted_projected_to_localxy); 
	  histosTH1F["TH2F_theta_x_L_xLN_vertex_term_subtracted_projected_to_localxy_only_x"]->Fill(theta_x_L_vertex_term_subtracted_projected_to_localxy); 

	  histosTH2F["TH2F_theta_x_R_xRN"]->Fill(theta_x_R, xRN); 
	  histosTH2F["TH2F_theta_x_R_xRF"]->Fill(theta_x_R, xRF); 
	  histosTH2F["TH2F_theta_x_R_xRF_swap"]->Fill(xRF, theta_x_R); 
	  histosTH2F["TH2F_theta_x_R_xRF_swap_rotated"]->Fill(xRF_rotated, theta_x_R_rotated_2); 

	  histosTH2F["TH2F_theta_x_R_xRF_vertex_term_subtracted"]->Fill(theta_x_R_vertex_term_subtracted, xRF_vertex_term_subtracted); 
	  histosTH2F["TH2F_xRN_vertex_term_subtracted_xRF_vertex_term_subtracted"]->Fill(xRN_vertex_term_subtracted, xRF_vertex_term_subtracted); 

	  double theta_x_R_vertex_term_subtracted_projected_to_localxy = TH2F_theta_x_R_xRF_vertex_term_subtracted_analysis_cut->ProjectionX(theta_x_R_vertex_term_subtracted, xRF_vertex_term_subtracted) ;
	  double xRF_vertex_term_subtracted_projected_to_localxy       = TH2F_theta_x_R_xRF_vertex_term_subtracted_analysis_cut->ProjectionY(theta_x_R_vertex_term_subtracted, xRF_vertex_term_subtracted) ;

	  histosTH2F["TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy"]->Fill(theta_x_R_vertex_term_subtracted_projected_to_localxy, xRF_vertex_term_subtracted_projected_to_localxy); 
	  histosTH1F["TH2F_theta_x_R_xRF_vertex_term_subtracted_projected_to_localxy_only_x"]->Fill(theta_x_R_vertex_term_subtracted_projected_to_localxy); 
	  
	  histosTH2F["TH2F_theta_y_L_yLN"]->Fill(theta_y_L, yLN); 
	  histosTH2F["TH2F_theta_y_L_yLF"]->Fill(theta_y_L, yLF); 

	  histosTH2F["TH2F_theta_y_R_yRN"]->Fill(theta_y_R, yRN); 
	  histosTH2F["TH2F_theta_y_R_yRF"]->Fill(theta_y_R, yRF); 
	  
	  double theta_y_L_projected_to_localxy = TH2F_theta_y_L_yLF_analysis_cut->ProjectionX(theta_y_L, yLF) ;
	  double yLF_projected_to_localxy       = TH2F_theta_y_L_yLF_analysis_cut->ProjectionY(theta_y_L, yLF) ;

	  histosTH2F["TH2F_theta_y_L_yLF_projected_to_localxy"]->Fill(theta_y_L_projected_to_localxy, yLF_projected_to_localxy); 
	  histosTH1F["TH2F_theta_y_L_yLF_projected_to_localxy_only_x"]->Fill(theta_y_L_projected_to_localxy); 

	  double theta_y_R_projected_to_localxy = TH2F_theta_y_R_yRF_analysis_cut->ProjectionX(theta_y_R, yRF) ;
	  double yRF_projected_to_localxy       = TH2F_theta_y_R_yRF_analysis_cut->ProjectionY(theta_y_R, yRF) ;

	  histosTH2F["TH2F_theta_y_R_yRF_projected_to_localxy"]->Fill(theta_y_R_projected_to_localxy, yRF_projected_to_localxy); 
	  histosTH1F["TH2F_theta_y_R_yRF_projected_to_localxy_only_x"]->Fill(theta_y_R_projected_to_localxy); 
	  
	  

	  // profiles["TProfile_theta_x_L_xLN"]->Fill(theta_x_L, xLN); 
	  // profiles["TProfile_theta_x_L_xLF"]->Fill(theta_x_L, xLF); 

	  // profiles["TProfile_theta_x_R_xRN"]->Fill(theta_x_R, xRN); 
	  // profiles["TProfile_theta_x_R_xRF"]->Fill(theta_x_R, xRF); 

	  // profiles["TProfile_theta_y_L_yLN"]->Fill(theta_y_L, yLN); 
	  // profiles["TProfile_theta_y_L_yLF"]->Fill(theta_y_L, yLF); 

	  // profiles["TProfile_theta_y_R_yRN"]->Fill(theta_y_R, yRN); 
	  // profiles["TProfile_theta_y_R_yRF"]->Fill(theta_y_R, yRF); 
	  
  }

  // elastic cut
  bool isElastic = false;
  map_of_conditions_if_they_are_applied["isElastic"] = 1 ;        

  // 5 sigma
  if ((TMath::Abs(ThyL+ThyR)< 15e-6) && (TMath::Abs(ThxL+ThxR)<45e-6)) isElastic = true ;

  if ((!skimming) && (use_isElastic) && isElastic) return returnStatus;

  histosTH1F["hthxEla2"]->Fill(ThxL+ThxR);
  histosTH1F["hthyEla2"]->Fill(ThyL+ThyR);

  //--------------------------------------------------------------------------------
  // process HF data

  /*
  int nHFhad=0;
  int nHFemc=0;

  //double thresHFemc=2.;
  //double thresHFhad=5.5;

  double thresHFemc=5.;
  double thresHFhad=6.5;

  int nmuPF=0;

  for (PFCandidateCollection::const_iterator iPFCand = pfs->begin() ; iPFCand != pfs->end() ; ++iPFCand)
  {
    int PFtype = iPFCand->particleId();
    double PFene = iPFCand->energy();
    double PFeta = iPFCand->eta();
    double PFphi = iPFCand->phi();
    double PFpt  = iPFCand->pt();
    double PFpx  = iPFCand->px();

    // id=3
    if (PFtype == reco::PFCandidate::mu) nmuPF++;

    if (PFtype == reco::PFCandidate::h_HF) // pfOut=6
    {
      histosTH1F["heHFhad"]->Fill(PFene);
      if(PFene > thresHFhad) nHFhad++;

      if(PFene > thresHFhad && PFeta<0) histosTH2F["hHF_Mhad_2DIM"]->Fill(PFeta,PFphi);
      if(PFene > thresHFhad && PFeta>0) histosTH2F["hHF_Phad_2DIM"]->Fill(PFeta,PFphi);

      if(PFene > thresHFhad && PFeta<0) histosTH2F["heHF_Mhad_2DIM"]->Fill(PFeta,PFene);
      if(PFene > thresHFhad && PFeta>0) histosTH2F["heHF_Phad_2DIM"]->Fill(PFeta,PFene);

    } else if(PFtype == reco::PFCandidate::egamma_HF) // pfOut=7
    {
      histosTH1F["heHFemc"]->Fill(PFene);
      if(PFene > thresHFemc) nHFemc++;

      if(PFene > thresHFemc && PFeta<0) histosTH2F["hHF_Memc_2DIM"]->Fill(PFeta,PFphi);
      if(PFene > thresHFemc && PFeta>0) histosTH2F["hHF_Pemc_2DIM"]->Fill(PFeta,PFphi);

      if(PFene > thresHFemc && PFeta<0) histosTH2F["heHF_Memc_2DIM"]->Fill(PFeta,PFene);
      if(PFene > thresHFemc && PFeta>0) histosTH2F["heHF_Pemc_2DIM"]->Fill(PFeta,PFene);

    }
  }

  histosTH1F["hnHF"]->Fill(nHFhad+nHFemc);
  histosTH1F["hnHFhad"]->Fill(nHFhad);
  histosTH1F["hnHFemc"]->Fill(nHFemc);

  //if(nHFhad+nHFemc > 1 ) return;
  */

  double sum_of_PFpx = 0 ;
  double sum_of_PFpy = 0 ;

  double sum_of_PFpx_hadrons = 0 ;
  double sum_of_PFpy_hadrons = 0 ;
  double sum_of_PFpz_hadrons = 0 ;

  int number_of_particle_flow_entries = 0 ;
  int number_of_particle_flow_entries_hadrons = 0 ;
  int number_of_particle_flow_entries_electrons = 0 ;
  int pf_vertex_type = 0 ;
  

  for (PFCandidateCollection::const_iterator iPFCand = pfs->begin() ; iPFCand != pfs->end() ; ++iPFCand)
  {
    int PFtype = iPFCand->particleId();
    double PFene = iPFCand->energy();
    double PFeta = iPFCand->eta();
    double PFphi = iPFCand->phi();
    double PFpt  = iPFCand->pt();
    double PFpx  = iPFCand->px();
    double PFpy	  = iPFCand->py();
    double PFpz	  = iPFCand->pz();
    
    ++number_of_particle_flow_entries ;
    // pf_vertex_type = iPFCand->PFVertexType();
    
    sum_of_PFpx += PFpx ;
    sum_of_PFpy += PFpy ;

    if (PFtype == reco::PFCandidate::h)
    {
    	++number_of_particle_flow_entries_hadrons ; 

	sum_of_PFpx_hadrons += PFpx ;
	sum_of_PFpy_hadrons += PFpy ;
	sum_of_PFpz_hadrons += PFpz ;
    }


    if (PFtype == reco::PFCandidate::e)
    {
        ++number_of_particle_flow_entries_electrons ;
    }

    
  }

  //--------------------------------------------------------------------------------
  // CMS-TOTEM matching

  double TOTEMpy =  6500.*(ThyL+ThyR);
  double TOTEMpx = -6500.*(ThxL+ThxR);
  double TOTEMphiL = TMath::ATan2(ThyL, ThxL);
  double TOTEMphiR = TMath::ATan2(ThyR, ThxR);

  double TOTEMdphi = TOTEMphiL - TOTEMphiR;
  if (TOTEMdphi < 0) TOTEMdphi = TOTEMdphi + 2.*TMath::Pi(); // from (-2pi,2pi) to (0,2pi)
  if (TOTEMdphi > TMath::Pi()) TOTEMdphi = 2.*TMath::Pi() - TOTEMdphi; // from (0,2pi) to (0,pi)

  //double CMSpx=pipiRec.Px();
  //double CMSpy=pipiRec.Py();

  //--------------------------------------------------------------------------------
  // plots for 2 tracks sample

  /*
  if (ntrk==2 && totcharge==0 && ntrkvtx==2)
  {
    bool CTpxcut = TMath::Abs(CMSpx+TOTEMpx)<0.15;
    bool CTpycut = TMath::Abs(CMSpy+TOTEMpy)<0.06;

    bool CTpycutBKG = TMath::Abs(CMSpy+TOTEMpy)>0.06 && TMath::Abs(CMSpy+TOTEMpy)<0.3;

    bool allCuts = CTpxcut && CTpycut;

    histosTH1F["hdpy2trk"]->Fill(CMSpy+TOTEMpy);
    histosTH1F["hdpx2trk"]->Fill(CMSpx+TOTEMpx);
    if(CTpxcut) histosTH1F["hdpy2trkB"]->Fill(CMSpy+TOTEMpy);
    if(CTpycut) histosTH1F["hdpx2trkB"]->Fill(CMSpx+TOTEMpx);

    histosTH2F["h2DIMdpy2trk"]->Fill(CMSpy,TOTEMpy);
    histosTH2F["h2DIMdpx2trk"]->Fill(CMSpx,TOTEMpx);

    histosTH1F["hLS2"]->Fill(LS);
    if(TMath::Abs(TOTEMpy)<0.1 && TMath::Abs(CMSpy)>0.1) histosTH1F["hLS2ela"]->Fill(LS);

    // Mass 2 tracks
    double mrec = pipiRec.M();

    if (allCuts)
    {
      int nKaons=0;
      int nPions=0;

      int itref2=0;
      for (TrackCollection::const_iterator itTrack = tracks->begin();itTrack != tracks->end();++itTrack)
      {
        int npixelhits = itTrack->hitPattern().numberOfValidPixelHits();
        reco::TrackRef trackRef = reco::TrackRef(tracks, itref2);
        double thisdedxPIX = (*dedxPIXs)[trackRef].dEdx(); //dedxs.get(trackRef.key()).dEdx();

        itref2++;

        if(npixelhits>0)
        {
          histosTH2F["hdedx2trk"]->Fill(itTrack->p(),thisdedxPIX);
          histosTH2F["hdedx2trklog"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

          if( isKaonCurve(itTrack->p(),thisdedxPIX) ) nKaons++;
          if( isPionCurve(itTrack->p(),thisdedxPIX) ) nPions++;

          if(is2PIsample) histosTH2F["hdedx2pionslog"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));
          if(is2Ksample)  histosTH2F["hdedx2kaonslog"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

          if(is2PIsample2) histosTH2F["hdedx2pionslog35"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));
          if(is2Ksample2)  histosTH2F["hdedx2kaonslog35"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

          if(is2PIsample3) histosTH2F["hdedx2pionslog40"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));
          if(is2Ksample3)  histosTH2F["hdedx2kaonslog40"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

          if(is2PIsample4) histosTH2F["hdedx2pionslog45"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));
          if(is2Ksample4)  histosTH2F["hdedx2kaonslog45"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

          if(is2PIsample5) histosTH2F["hdedx2pionslog50"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));
          if(is2Ksample5)  histosTH2F["hdedx2kaonslog50"]->Fill(itTrack->p(),TMath::Log10(thisdedxPIX));

        }
      }

      histosTH1F["hntrk02"]->Fill(ntrk0);
      histosTH1F["hntrkvtx2"]->Fill(ntrkvtx);

      histosTH1F["hm2"]->Fill(mrec);

      histosTH1F["hdphi2"]->Fill(TOTEMdphi);

      histosTH1F["hpt2"]->Fill(pi4pos1.Pt());
      histosTH1F["hpt2"]->Fill(pi4neg1.Pt());

      histosTH1F["heta2"]->Fill(pi4pos1.Eta());
      histosTH1F["heta2"]->Fill(pi4neg1.Eta());
    }

    if(CTpycutBKG)
    {
      histosTH1F["hm2BKG"]->Fill(mrec);

      histosTH1F["hpt2BKG"]->Fill(pi1.Pt());
      histosTH1F["hpt2BKG"]->Fill(pi2.Pt());

      histosTH1F["heta2BKG"]->Fill(pi1.Eta());
      histosTH1F["heta2BKG"]->Fill(pi2.Eta());
    }
  }
  */

  //--------------------------------------------------------------------------------
  // plots for 4 tracks sample

  const double CMSpx4 = pi4Rec.Px();
  const double CMSpy4 = pi4Rec.Py();

  histosTH2F["h2DIMdpy4trk_before_track_constraint"]->Fill(CMSpy4, TOTEMpy);
  histosTH2F["h2DIMdpx4trk_before_track_constraint"]->Fill(CMSpx4, TOTEMpx);
  
  histosTH2F["px_py_plot_1"]->Fill(TOTEMpx, TOTEMpy);
  
  histosTH1F["hdpy4trk_before_track_constraint"]->Fill(CMSpy4+TOTEMpy);
  histosTH1F["hdpx4trk_before_track_constraint"]->Fill(CMSpx4+TOTEMpx);

  double CMS_pt = sqrt((CMSpx4*CMSpx4) + (CMSpy4*CMSpy4)) ;

  if((!skimming) && (use_pT_cut) && (CMS_pt > 0.8)) return returnStatus ;
  
  if (ntrk == 2)
  {
	histosTH1F["2trk_totcharge"]->Fill(totcharge);
  }

  if (ntrk == 6)
  {
	histosTH1F["6trk_totcharge"]->Fill(totcharge);
  }
  
  // For skimming 
    
  if(skimming)
  {
  	if((ntrk == 4) && (totcharge == 0))
	{
		returnStatus = true ;
	}
	else
	{
		returnStatus = false ;
	}
	
	return returnStatus;
  }


  // Original: if (skip_track_and_charge_and_vertex_cut || ((ntrk == 4) && (totcharge == 0) && (ntrkvtx == 4)))
  // if (skip_track_and_charge_and_vertex_cut || ((ntrk == 4) && (totcharge == 0) ))
  if(skip_track_and_charge_and_vertex_cut || ((ntrk == 4) && (totcharge == 0)))
  {
  
  
    histosTH1F["npflow"]->Fill(number_of_particle_flow_entries);
    histosTH1F["npflow_hadrons"]->Fill(number_of_particle_flow_entries_hadrons);

    histosTH2F["pflow_vs_track_x"]->Fill(sum_of_px, sum_of_PFpx);
    histosTH2F["pflow_vs_track_y"]->Fill(sum_of_py, sum_of_PFpy);

    histosTH2F["pflow_vs_track_x_hadrons"]->Fill(sum_of_px, sum_of_PFpx_hadrons);
    histosTH2F["pflow_vs_track_y_hadrons"]->Fill(sum_of_py, sum_of_PFpy_hadrons);
    histosTH2F["pflow_vs_track_z_hadrons"]->Fill(sum_of_pz, sum_of_PFpz_hadrons);
    
    if(number_of_particle_flow_entries_hadrons == 4)
    {
	    histosTH2F["pflow_vs_track_x_hadrons_4pfhadron"]->Fill(sum_of_px, sum_of_PFpx_hadrons);
	    histosTH2F["pflow_vs_track_y_hadrons_4pfhadron"]->Fill(sum_of_py, sum_of_PFpy_hadrons);
	    histosTH2F["pflow_vs_track_z_hadrons_4pfhadron"]->Fill(sum_of_pz, sum_of_PFpz_hadrons);
    }

    map_of_conditions_if_they_are_applied["if (ntrk == 4 && totcharge == 0 && ntrkvtx == 4)"] = 1 ;

    bool CTpxcut4 = TMath::Abs(CMSpx4+TOTEMpx) < 0.13 ;
    bool CTpycut4 = TMath::Abs(CMSpy4+TOTEMpy) < 0.06 ;

    bool CTpxcut4_strict = TMath::Abs(CMSpx4+TOTEMpx) < (0.13 / 2.0) ;
    bool CTpycut4_strict = TMath::Abs(CMSpy4+TOTEMpy) < (0.06 / 2.0) ;

    bool CTpycut4BKG = (fabs(CMSpy4+TOTEMpy) > 0.06) && (fabs(CMSpy4+TOTEMpy) < 0.3) ;

    bool allCuts4 = (CTpxcut4 && CTpycut4) ;
    
    // if(Ken_test_for_non_exclusive_events) allCuts4 = !allCuts4 ;

    histosTH1F["hdpy4trk"]->Fill(CMSpy4+TOTEMpy);
    histosTH1F["hdpx4trk"]->Fill(CMSpx4+TOTEMpx);
    if(CTpxcut4) histosTH1F["hdpy4trkB"]->Fill(CMSpy4+TOTEMpy);
    if(CTpycut4) histosTH1F["hdpx4trkB"]->Fill(CMSpx4+TOTEMpx);

    histosTH2F["h2DIMdpy4trk"]->Fill(CMSpy4, TOTEMpy);
    histosTH2F["h2DIMdpx4trk"]->Fill(CMSpx4, TOTEMpx);
    
    histosTH2F["px_py_plot_2"]->Fill(TOTEMpx, TOTEMpy);

    
    // New plots to Ken

    // L_1_F

    double TOTEMpy_1_F =  6500.* (th_y_L_1_F + th_y_R_1_F);
    double TOTEMpy_2_F =  6500.* (th_y_L_2_F + th_y_R_2_F) ;

    double ThxLN = (xLN / L_x_L_1_F) ;
    double ThxLF = (xLF / L_x_L_2_F) ;
    double ThxRN = (xRN / L_x_R_1_F) ;
    double ThxRF = (xRF / L_x_R_2_F) ;

    double TOTEMpx_1_F = -6500.* (ThxLN + ThxRN);
    double TOTEMpx_2_F = -6500.* (ThxLF + ThxRF);

    histosTH1F["hdpy4trk_1_F"]->Fill(CMSpy4 + TOTEMpy_1_F);
    histosTH1F["hdpy4trk_2_F"]->Fill(CMSpy4 + TOTEMpy_2_F);

    histosTH1F["hdpx4trk_1_F"]->Fill(CMSpx4 + TOTEMpx_1_F);
    histosTH1F["hdpx4trk_2_F"]->Fill(CMSpx4 + TOTEMpx_2_F);

    // TODO
    const double mrec4 = pi4Rec.M();

    if (skip_allCuts4 || allCuts4)
    {

     histosTH2F["px_py_plot_3"]->Fill(TOTEMpx, TOTEMpy);

     histosTH2F["pflow_vs_track_x_hadrons_allCuts4"]->Fill(sum_of_px, sum_of_PFpx_hadrons);
     histosTH2F["pflow_vs_track_y_hadrons_allCuts4"]->Fill(sum_of_py, sum_of_PFpy_hadrons);
     histosTH2F["pflow_vs_track_z_hadrons_allCuts4"]->Fill(sum_of_pz, sum_of_PFpz_hadrons);

      map_of_conditions_if_they_are_applied["if (allCuts4)"] = 1 ;

      returnStatus = true;

      int nPions=0, nSafePions=0, nSafePionsNew=0, nSuperSafePionsNew=0;
      int nKaons=0, nKaonsPos=0, nKaonsNeg=0;
      int nKaons_FS=0, nKaonsPos_FS=0, nKaonsNeg_FS=0;
      int nPionsPos=0, nPionsNeg=0;
      int nProtons=0;
      int nElectrons_horizontal_curve = 0 ;
      int nElectrons_horizontal_and_vertical_curve = 0 ;

      struct DEDxInfo
      {
        int nPixelHits;
        double p;
        double dEdx;
      };

      vector<DEDxInfo> dEdxInfo;
      vector<DEDxInfo> dEdxInfo_Ferenc;

      int itref4=0;

      bool etaCutSatisfied = true;
      bool allTracksHavePixelHit = true;

      int nKaonsPosArray[kaon_step_max] ;
      int nKaonsNegArray[kaon_step_max] ;

      for(int kaon_step_index = 0 ; kaon_step_index < kaon_step_max ; ++kaon_step_index)
      {
        nKaonsPosArray[kaon_step_index] = 0 ;
        nKaonsNegArray[kaon_step_index] = 0 ;
      }

   int tracks_below_p_limit_to_separate_K_pi_Simone = 0 ;
	int tracks_below_p_limit_to_separate_K_pi = 0 ;
	int tracks_above_p_limit_to_separate_K_pi = 0 ;
	int tracks_above_dedx_limit_to_separate_K_pi = 0 ;
	int tracks_above_dedx_limit_of_Simone_and_below_p_limit = 0 ;
	
	int positive_and_in_dEdx_range = 0 ;
	int negative_and_in_dEdx_range = 0 ;

      for (TrackCollection::const_iterator itTrack = tracks->begin(); itTrack != tracks->end(); ++itTrack)
      {
        int npixelhits = itTrack->hitPattern().numberOfValidPixelHits();
        reco::TrackRef trackRef = reco::TrackRef(tracks, itref4);
        double thisdedxPIX = (*dedxPIXs)[trackRef].dEdx();

        itref4++;

        dEdxInfo.push_back({npixelhits, itTrack->p(), thisdedxPIX});

    	  double thisdedxPIX_Ferenc = 0 ;
		  double thisdedxAllHits = 0 ;

    		if(myenergyLossAllHits.isValid())
    		{
       			if(myenergyLossAllHits->size() > trackRef.key())
       			{
          			thisdedxAllHits = myenergyLossAllHits->get(trackRef.key()).dEdx();
	        			dEdxInfo_Ferenc.push_back({npixelhits, itTrack->p(), thisdedxAllHits});

		      	  if(!use_old_dEdx) thisdedxPIX = thisdedxAllHits ;
			}
	}
	
	// cout << "mytest2: " << iEvent.run() << " " <<  iEvent.id() << " " << (itref4-1) << " "<< trackRef.key() << " " << itTrack->p() << " " << thisdedxAllHits << endl ;
	// cout << "mytest2: " <<  iEvent.id().event() << " " << itTrack->p() << " " << itTrack->eta() << " " << itTrack->phi() << endl ;

	int pid = particleType->guess(itTrack->p(), thisdedxPIX);
	
	// if(itTrack->p() < 0.6) ++tracks_below_p_limit_to_separate_K_pi ;
	// if(itTrack->p() < 0.7) ++tracks_below_p_limit_to_separate_K_pi ;
	if(itTrack->p() < 0.5) ++tracks_below_p_limit_to_separate_K_pi ;

	if(itTrack->p() < 0.85) ++tracks_below_p_limit_to_separate_K_pi_Simone ;

	if(itTrack->p() > 0.2) ++tracks_above_p_limit_to_separate_K_pi ;
	if(thisdedxAllHits > 3.5) ++tracks_above_dedx_limit_to_separate_K_pi ;
	// if(thisdedxAllHits > 3) ++tracks_above_dedx_limit_to_separate_K_pi ;
	
	if((itTrack->p() < 0.7) && (itTrack->charge() == +1)) ++positive_and_in_dEdx_range ;
	if((itTrack->p() < 0.7) && (itTrack->charge() == -1)) ++negative_and_in_dEdx_range ;

	if((thisdedxAllHits > 3.5) && (itTrack->p() < 0.6)) ++tracks_above_dedx_limit_of_Simone_and_below_p_limit ;
	
        if(number_of_particle_flow_entries_electrons == 0) histosTH2F["hdedxAllHits_electrons_0"]->Fill(itTrack->p(), thisdedxAllHits);
        if(number_of_particle_flow_entries_electrons <= 1) histosTH2F["hdedxAllHits_electrons_1"]->Fill(itTrack->p(), thisdedxAllHits);
        if(number_of_particle_flow_entries_electrons <= 2) histosTH2F["hdedxAllHits_electrons_2"]->Fill(itTrack->p(), thisdedxAllHits);
        if(number_of_particle_flow_entries_electrons <= 3) histosTH2F["hdedxAllHits_electrons_3"]->Fill(itTrack->p(), thisdedxAllHits);


        if (fabs(itTrack->eta()) > 2.5) etaCutSatisfied = false ;

        if (npixelhits == 0) allTracksHavePixelHit = false ;


        if((skip_check_npixelhits_2) || (npixelhits > 0))
        {
          histosTH2F["hdedx4trk"]->Fill(itTrack->p(), thisdedxPIX);
          histosTH2F["hdedx4trklog"]->Fill(itTrack->p(), TMath::Log10(thisdedxPIX));
	  
          if (isPionCurve(itTrack->p(), thisdedxPIX))
          {
            nPions++;

            histosTH2F["hdedx4trk_PionCurve"]->Fill(itTrack->p(), thisdedxPIX);
          }

          if (isSafePionCurve(itTrack->p(), thisdedxPIX))
          {
            nSafePions++;

            histosTH2F["hdedx4trk_SafePionCurve"]->Fill(itTrack->p(), thisdedxPIX);
          }

          if (isSafePionCurveNew(itTrack->p(), thisdedxPIX))
          {
            nSafePionsNew++;

            histosTH2F["hdedx4trk_SafePionNewCurve"]->Fill(itTrack->p(), thisdedxPIX);
	    
            if (itTrack->charge() > 0)
              nPionsPos++;
            if (itTrack->charge() < 0)
              nPionsNeg++;
	    
          }

          if (isSuperSafePionCurveNew(itTrack->p(), thisdedxPIX))
          {
            nSuperSafePionsNew++;
   
          }

	  if(isElectron_horizontal_curve(itTrack->p(), thisdedxPIX))
	  {
		nElectrons_horizontal_curve++ ;
          }

	  if(isElectron_horizontal_and_vertical_curve(itTrack->p(), thisdedxPIX))
	  {
      nElectrons_horizontal_and_vertical_curve++ ;
	  }

          if(isKaonCurve(itTrack->p(), thisdedxPIX))
          {
            nKaons++;

            histosTH2F["hdedx4trk_KaonCurve"]->Fill(itTrack->p(), thisdedxPIX);

            if (itTrack->charge() > 0)
              nKaonsPos++;
            if (itTrack->charge() < 0)
              nKaonsNeg++;
          }

	  for(int kaon_step_index = 0 ; kaon_step_index < kaon_step_max ; ++kaon_step_index)
	  {
	        if(isKaonCurveSpecial(itTrack->p(), thisdedxPIX, kaon_step_index))
	        {
        		if (itTrack->charge() > 0) nKaonsPosArray[kaon_step_index]++;
	            	if (itTrack->charge() < 0) nKaonsNegArray[kaon_step_index]++;
          	}

	  }

          histosTH2F["hdedx4trk_FS"]->Fill(itTrack->p(), thisdedxPIX);
          histosTH1F["eta_of_all"]->Fill(itTrack->eta());

			if(itTrack->p() < 0.7)
			{
	          histosTH2F["hdedx4trk_FS_below_mom_0p7"]->Fill(itTrack->p(), thisdedxPIX);
   	       histosTH1F["eta_of_all_below_mom_0p7"]->Fill(itTrack->eta());
			}

          if(test_dedx)
          {
            cout << "eventprintout reference1 " << iEvent.id() << " " << (itref4-1) << " " << itTrack->p() << endl ;

    				if(itTrack->p() < 0.7)
            {
              cout << "eventprintout reference2 " << iEvent.id() << " " << (itref4-1) << " " << itTrack->p() << endl ;
            }
          }


          if(pid == kaon)
          {
            nKaons_FS++;
            // cout << "mypid: " << itref4 << " kaon" << endl ;
				
            histosTH2F["hdedx4trk_KaonCurve_FS"]->Fill(itTrack->p(), thisdedxPIX);
            histosTH1F["eta_of_kaons"]->Fill(itTrack->eta());

				if(itTrack->p() < 0.7)
				{
      	      histosTH2F["hdedx4trk_KaonCurve_FS_below_mom_0p7"]->Fill(itTrack->p(), thisdedxPIX);
         	   histosTH1F["eta_of_kaons_below_mom_0p7"]->Fill(itTrack->eta());
				}
				
				if(test_dedx) cout << "eventprintout pidkaon " << iEvent.id() << " " << (itref4-1) << " " << itTrack->p() << endl ;

            if (itTrack->charge() > 0)
              nKaonsPos_FS++;
            if (itTrack->charge() < 0)
              nKaonsNeg_FS++;
          }
          else if(pid == pion)
			 {
            histosTH2F["hdedx4trk_PionCurve_FS"]->Fill(itTrack->p(), thisdedxPIX);
            histosTH1F["eta_of_pions"]->Fill(itTrack->eta());

				if(itTrack->p() < 0.7)
				{
      	      histosTH2F["hdedx4trk_PionCurve_FS_below_mom_0p7"]->Fill(itTrack->p(), thisdedxPIX);
         	   histosTH1F["eta_of_pions_below_mom_0p7"]->Fill(itTrack->eta());
				}

			 }
          else
          {
            // cout << "mypid: " << itref4 << " notkaon" << endl ;
          }

          if(isKaonCurveRobert(itTrack->p(), thisdedxPIX))
          {
            histosTH2F["hdedx4trk_KaonCurveRobert"]->Fill(itTrack->p(), thisdedxPIX);
          }

          if(isProtonCurve(itTrack->p(), thisdedxPIX))
          {
            nProtons++;

            histosTH2F["hdedx4trk_ProtonCurve"]->Fill(itTrack->p(), thisdedxPIX);
          }
        }
      }
		
      histosTH1F["hntrk04"]->Fill(ntrk0);
      histosTH1F["hntrkvtx4"]->Fill(ntrkvtx);

      histosTH1F["hm4"]->Fill(mrec4);

      histosTH1F["hdphi4"]->Fill(TOTEMdphi);

      histosTH1F["hpt4"]->Fill(pi4pos1.Pt());
      histosTH1F["hpt4"]->Fill(pi4neg1.Pt());
      histosTH1F["hpt4"]->Fill(pi4pos2.Pt());
      histosTH1F["hpt4"]->Fill(pi4neg2.Pt());

      histosTH1F["heta4"]->Fill(pi4pos1.Eta());
      histosTH1F["heta4"]->Fill(pi4neg1.Eta());
      histosTH1F["heta4"]->Fill(pi4pos2.Eta());
      histosTH1F["heta4"]->Fill(pi4neg2.Eta());

      int number_of_tracks_with_too_large_chi2 = 0 ;
      int number_of_tracks_with_too_small_chi2_1 = 0 ;
      int number_of_tracks_with_too_large_pt = 0 ;

      for(unsigned int chi_index = 0 ; chi_index < CHIN_4TRK.size() ; ++chi_index)
      {
                if(CHIN_4TRK[chi_index] > 2.0)  ++number_of_tracks_with_too_large_chi2 ; 
	        if(CHIN_4TRK[chi_index] < 0.1) ++number_of_tracks_with_too_small_chi2_1 ;
                // if(PTERR_4TRK[chi_index] > 0.01)        ++number_of_tracks_with_too_large_pt ;
      }


      map_of_conditions_if_they_are_applied["if (!etaCutSatisfied)"] = 1 ;      
      if (use_etaCutSatisfied && (!etaCutSatisfied)) return returnStatus;

      map_of_conditions_if_they_are_applied["if (!allTracksHavePixelHit)"] = 1 ;      
      if (use_allTracksHavePixelHit && (!allTracksHavePixelHit)) return returnStatus;


      //------------------------
      // rho-rho
      
      double pt_correction_factor = 1.04 ;

   	  const TLorentzVector pi4pos1_pt_corrected(pt_correction_factor * pi4pos1.Px(), pt_correction_factor * pi4pos1.Py(), pi4pos1.Pz(), pi4pos1.E());      
   	  const TLorentzVector pi4pos2_pt_corrected(pt_correction_factor * pi4pos2.Px(), pt_correction_factor * pi4pos2.Py(), pi4pos2.Pz(), pi4pos2.E());      
   	  const TLorentzVector pi4neg1_pt_corrected(pt_correction_factor * pi4neg1.Px(), pt_correction_factor * pi4neg1.Py(), pi4neg1.Pz(), pi4neg1.E());      
   	  const TLorentzVector pi4neg2_pt_corrected(pt_correction_factor * pi4neg2.Px(), pt_correction_factor * pi4neg2.Py(), pi4neg2.Pz(), pi4neg2.E());      

      const TLorentzVector pi4m11_pt_corrected = pi4pos1_pt_corrected + pi4neg1_pt_corrected;
      const TLorentzVector pi4m22_pt_corrected = pi4pos2_pt_corrected + pi4neg2_pt_corrected;

      const TLorentzVector pi4m12_pt_corrected = pi4pos1_pt_corrected + pi4neg2_pt_corrected;
      const TLorentzVector pi4m21_pt_corrected = pi4pos2_pt_corrected + pi4neg1_pt_corrected;

      const double m11_pt_corrected = pi4m11_pt_corrected.M();
      const double m22_pt_corrected = pi4m22_pt_corrected.M();

      const double m12_pt_corrected = pi4m12_pt_corrected.M();
      const double m21_pt_corrected = pi4m21_pt_corrected.M();
      
      TLorentzVector pi4Rec_pt_corrected = pi4pos1_pt_corrected + pi4pos2_pt_corrected + pi4neg1_pt_corrected + pi4neg2_pt_corrected ;
      const double mrec4_pt_corrected = pi4Rec_pt_corrected.M();

      const TLorentzVector pi4m11 = pi4pos1 + pi4neg1;
      const TLorentzVector pi4m22 = pi4pos2 + pi4neg2;

      const TLorentzVector pi4m12 = pi4pos1 + pi4neg2;
      const TLorentzVector pi4m21 = pi4pos2 + pi4neg1;

      const double m11 = pi4m11.M();
      const double m22 = pi4m22.M();

      const double m12 = pi4m12.M();
      const double m21 = pi4m21.M();
      

      const double rhoM = 0.77;
      const double rhoCut = 0.124;
      const double rhoCut2 = rhoCut/2.;
      const double rhoCut4 = rhoCut/4.;
      const double rhoCut8 = rhoCut/8.;
      const double rhoCut16 = rhoCut/16.;

      // Rho mass constraints

      TLorentzVector pi4m11_constrained_rho = pi4m11 ;
      TLorentzVector pi4m22_constrained_rho = pi4m22 ; 

      TLorentzVector pi4m12_constrained_rho = pi4m12 ;
      TLorentzVector pi4m21_constrained_rho = pi4m21 ;
      
      pi4m11_constrained_rho.SetPz(sqrt(pow(pi4m11_constrained_rho.Energy(),2) - (pow(pi4m11_constrained_rho.Pt(),2) + (rhoM * rhoM) ))) ;
      pi4m22_constrained_rho.SetPz(sqrt(pow(pi4m22_constrained_rho.Energy(),2) - (pow(pi4m22_constrained_rho.Pt(),2) + (rhoM * rhoM) ))) ;

      pi4m12_constrained_rho.SetPz(sqrt(pow(pi4m12_constrained_rho.Energy(),2) - (pow(pi4m12_constrained_rho.Pt(),2) + (rhoM * rhoM) ))) ;
      pi4m21_constrained_rho.SetPz(sqrt(pow(pi4m21_constrained_rho.Energy(),2) - (pow(pi4m21_constrained_rho.Pt(),2) + (rhoM * rhoM) ))) ;


      const double m11_constrained_rho = pi4m11_constrained_rho.M();
      const double m22_constrained_rho = pi4m22_constrained_rho.M();

      const double m12_constrained_rho = pi4m12_constrained_rho.M();
      const double m21_constrained_rho = pi4m21_constrained_rho.M();

      histosTH2F["hmALLpiKen0_2D_constrained_rho"]->Fill(m11_constrained_rho, m22_constrained_rho);
      histosTH2F["hmALLpiKen0_2D_constrained_rho"]->Fill(m12_constrained_rho, m21_constrained_rho);

      histosTH2F["m11_vs_m11_constrained_rho"]->Fill(m11, m11_constrained_rho);
      histosTH2F["m22_vs_m22_constrained_rho"]->Fill(m22, m22_constrained_rho);
      histosTH2F["m12_vs_m12_constrained_rho"]->Fill(m12, m12_constrained_rho);
      histosTH2F["m21_vs_m21_constrained_rho"]->Fill(m21, m21_constrained_rho);


      //

      // Rho mass constraints by Ken: pz
      
      int mode = mode_pz ;
      
      // Rho mass constraints by Ken: pz and pz

      mode = mode_pz_and_pt ;

      // Scenario 1: 11, 22
      
      double factor11 = factor_for_rho_mass_constraint(pi4m11, rhoM, mode) ;
      double factor22 = factor_for_rho_mass_constraint(pi4m22, rhoM, mode) ;

      // Scenario 2: 12, 21

      double factor12 = factor_for_rho_mass_constraint(pi4m12, rhoM, mode) ;
      double factor21 = factor_for_rho_mass_constraint(pi4m21, rhoM, mode) ;

      TLorentzVector pi4pos1_Ken_constrained_rho1 = pi4pos1 ;
      TLorentzVector pi4pos2_Ken_constrained_rho1 = pi4pos2 ;
      TLorentzVector pi4neg1_Ken_constrained_rho1 = pi4neg1 ;
      TLorentzVector pi4neg2_Ken_constrained_rho1 = pi4neg2 ;

      TLorentzVector pi4pos1_Ken_constrained_rho2 = pi4pos1 ;
      TLorentzVector pi4pos2_Ken_constrained_rho2 = pi4pos2 ;
      TLorentzVector pi4neg1_Ken_constrained_rho2 = pi4neg1 ;
      TLorentzVector pi4neg2_Ken_constrained_rho2 = pi4neg2 ;

      
      // Factor's application

      // Scenario 1
      
      pi4pos1_Ken_constrained_rho1.SetPz(pi4pos1.Pz() * factor11) ;
      pi4neg1_Ken_constrained_rho1.SetPz(pi4neg1.Pz() * factor11) ;

      pi4pos2_Ken_constrained_rho1.SetPz(pi4pos2.Pz() * factor22) ;
      pi4neg2_Ken_constrained_rho1.SetPz(pi4neg2.Pz() * factor22) ;
      
      if(mode == mode_pz_and_pt)
      {
	      pi4pos1_Ken_constrained_rho1.SetPx(pi4pos1.Px() * factor11) ;
	      pi4neg1_Ken_constrained_rho1.SetPx(pi4neg1.Px() * factor11) ;

	      pi4pos2_Ken_constrained_rho1.SetPx(pi4pos2.Px() * factor22) ;
	      pi4neg2_Ken_constrained_rho1.SetPx(pi4neg2.Px() * factor22) ;

	      pi4pos1_Ken_constrained_rho1.SetPy(pi4pos1.Py() * factor11) ;
	      pi4neg1_Ken_constrained_rho1.SetPy(pi4neg1.Py() * factor11) ;

	      pi4pos2_Ken_constrained_rho1.SetPy(pi4pos2.Py() * factor22) ;
	      pi4neg2_Ken_constrained_rho1.SetPy(pi4neg2.Py() * factor22) ;
      }

      // Scenario 2

      pi4pos1_Ken_constrained_rho2.SetPz(pi4pos1.Pz() * factor12) ;
      pi4neg2_Ken_constrained_rho2.SetPz(pi4neg2.Pz() * factor12) ;

      pi4pos2_Ken_constrained_rho2.SetPz(pi4pos2.Pz() * factor21) ;
      pi4neg1_Ken_constrained_rho2.SetPz(pi4neg1.Pz() * factor21) ;

      if(mode == mode_pz_and_pt)
      {

	      pi4pos1_Ken_constrained_rho2.SetPx(pi4pos1.Px() * factor12) ;
	      pi4neg2_Ken_constrained_rho2.SetPx(pi4neg2.Px() * factor12) ;

	      pi4pos2_Ken_constrained_rho2.SetPx(pi4pos2.Px() * factor21) ;
	      pi4neg1_Ken_constrained_rho2.SetPx(pi4neg1.Px() * factor21) ;

	      pi4pos1_Ken_constrained_rho2.SetPy(pi4pos1.Py() * factor12) ;
	      pi4neg2_Ken_constrained_rho2.SetPy(pi4neg2.Py() * factor12) ;

	      pi4pos2_Ken_constrained_rho2.SetPy(pi4pos2.Py() * factor21) ;
	      pi4neg1_Ken_constrained_rho2.SetPy(pi4neg1.Py() * factor21) ;

      }
      // Massses

      // Scenario 1
      
      const TLorentzVector pi4m11_Ken_constrained_rho1 = pi4pos1_Ken_constrained_rho1 + pi4neg1_Ken_constrained_rho1 ;
      const TLorentzVector pi4m22_Ken_constrained_rho1 = pi4pos2_Ken_constrained_rho1 + pi4neg2_Ken_constrained_rho1 ;

      const TLorentzVector pi4m12_Ken_constrained_rho1_wrong_assignment = pi4pos1_Ken_constrained_rho1 + pi4neg2_Ken_constrained_rho1 ;
      const TLorentzVector pi4m21_Ken_constrained_rho1_wrong_assignment = pi4pos2_Ken_constrained_rho1 + pi4neg1_Ken_constrained_rho1 ;

      const double m11_constrained_rho1 = pi4m11_Ken_constrained_rho1.M();
      const double m22_constrained_rho1 = pi4m22_Ken_constrained_rho1.M();

      const double m12_constrained_rho1_wrong_assignment = pi4m12_Ken_constrained_rho1_wrong_assignment.M();
      const double m21_constrained_rho1_wrong_assignment = pi4m21_Ken_constrained_rho1_wrong_assignment.M();

      // Scenario 2

      const TLorentzVector pi4m11_Ken_constrained_rho2_wrong_assignment = pi4pos1_Ken_constrained_rho2 + pi4neg1_Ken_constrained_rho2 ;
      const TLorentzVector pi4m22_Ken_constrained_rho2_wrong_assignment = pi4pos2_Ken_constrained_rho2 + pi4neg2_Ken_constrained_rho2 ;

      const TLorentzVector pi4m12_Ken_constrained_rho2 = pi4pos1_Ken_constrained_rho2 + pi4neg2_Ken_constrained_rho2 ;
      const TLorentzVector pi4m21_Ken_constrained_rho2 = pi4pos2_Ken_constrained_rho2 + pi4neg1_Ken_constrained_rho2 ;

      const double m11_constrained_rho2_wrong_assignment = pi4m11_Ken_constrained_rho2_wrong_assignment.M();
      const double m22_constrained_rho2_wrong_assignment = pi4m22_Ken_constrained_rho2_wrong_assignment.M();

      const double m12_constrained_rho2 = pi4m12_Ken_constrained_rho2.M();
      const double m21_constrained_rho2 = pi4m21_Ken_constrained_rho2.M();

      TLorentzVector the_4_body_system_Ken_constrained_rho1 = pi4pos1_Ken_constrained_rho1 + pi4pos2_Ken_constrained_rho1 + pi4neg1_Ken_constrained_rho1 + pi4neg2_Ken_constrained_rho1 ;
      TLorentzVector the_4_body_system_Ken_constrained_rho2 = pi4pos1_Ken_constrained_rho2 + pi4pos2_Ken_constrained_rho2 + pi4neg1_Ken_constrained_rho2 + pi4neg2_Ken_constrained_rho2 ;
      
      const double mrec4_Ken_constrained_rho1 = the_4_body_system_Ken_constrained_rho1.M() ;
      const double mrec4_Ken_constrained_rho2 = the_4_body_system_Ken_constrained_rho2.M() ;

      // 

      double rhoM_from_plot = 0.75;
      
      if(use_PDG_rho_mass) rhoM_from_plot = 0.775 ;
    
      
      histosTH1F["rho_mass"]->Fill(rhoM_from_plot * GeV_to_MeV_conversion_factor) ;

      const double rhoM_from_plot_v2 = 0.74245;   // 2019.05.21 value from hmALLpiKen0_2D

      const double rhoM_from_plot_v3_diagonal_x = 0.753319 ;
      const double rhoM_from_plot_v3_diagonal_y = 0.765377 ;

      histosTH1F["hmALLpiKen0"]->Fill(m11);
      histosTH1F["hmALLpiKen0"]->Fill(m12);

      histosTH2F["hmALLpiKen0_2D"]->Fill(m11, m22);
      histosTH2F["hmALLpiKen0_2D"]->Fill(m12, m21);

      histosTH2F["hmALLpiKen0_2D_pt_corrected"]->Fill(m11_pt_corrected, m22_pt_corrected);
      histosTH2F["hmALLpiKen0_2D_pt_corrected"]->Fill(m12_pt_corrected, m21_pt_corrected);

      if((pi4pos1.Pt() > 0.4) && (pi4pos2.Pt() > 0.4) && (pi4neg1.Pt() > 0.4) && (pi4neg2.Pt() > 0.4))
      {
        histosTH2F["hmALLpiKen0_2D_pt_cut"]->Fill(m11, m22);
        histosTH2F["hmALLpiKen0_2D_pt_cut"]->Fill(m12, m21);
      }
      
      int ptok = 0 ;

      if(pi4pos1.Pt() > 0.4) ++ptok ;
      if(pi4pos2.Pt() > 0.4) ++ptok ;
      if(pi4neg1.Pt() > 0.4) ++ptok ;
      if(pi4neg2.Pt() > 0.4) ++ptok ;

      if(ptok >= 2)
      {
        histosTH2F["hmALLpiKen0_2D_pt_cut_ok"]->Fill(m11, m22);
        histosTH2F["hmALLpiKen0_2D_pt_cut_ok"]->Fill(m12, m21);
      }

      if((pi4pos1.Pt() > 0.4) || (pi4pos2.Pt() > 0.4) || (pi4neg1.Pt() > 0.4) || (pi4neg2.Pt() > 0.4))
      {
        histosTH2F["hmALLpiKen0_2D_pt_cut_or"]->Fill(m11, m22);
        histosTH2F["hmALLpiKen0_2D_pt_cut_or"]->Fill(m12, m21);
      }

      histosTH2F["hmALLpiKen0_2D_low_p"]->Fill(m11, m22);
      histosTH2F["hmALLpiKen0_2D_low_p"]->Fill(m12, m21);

      if(tracks_below_p_limit_to_separate_K_pi == 4)
      {
         histosTH2F["hmALLpiKen0_2D_p_condition"]->Fill(m11, m22);
         histosTH2F["hmALLpiKen0_2D_p_condition"]->Fill(m12, m21);
      }
      
      if(plots_per_run_number)
      {
	      stringstream ss_my_run ;
	      ss_my_run << my_run ;
      
	      histosTH2F[("hmALLpiKen0_2D_" + ss_my_run.str()).c_str()]->Fill(m11, m22);
	      histosTH2F[("hmALLpiKen0_2D_" + ss_my_run.str()).c_str()]->Fill(m12, m21);
     }

      if(nKaons == 0)
      {
	      histosTH2F["hmALLpiKen0_2D_no_safe_kaon"]->Fill(m11, m22);
	      histosTH2F["hmALLpiKen0_2D_no_safe_kaon"]->Fill(m12, m21);
      }
      
    	if (nSafePionsNew >= 3)
    	{
        histosTH2F["hmALLpiKen0_2D_nSafePionsNew_34"]->Fill(m11, m22);
        histosTH2F["hmALLpiKen0_2D_nSafePionsNew_34"]->Fill(m12, m21);
    	}

    	if((nSafePionsNew >= 2) && (nKaons == 0))
    	{
	      histosTH2F["hmALLpiKen0_2D_nSafePionsNew_234_no_safekaon"]->Fill(m11, m22);
	      histosTH2F["hmALLpiKen0_2D_nSafePionsNew_234_no_safekaon"]->Fill(m12, m21);
	      
	      for (const auto &in : dEdxInfo)
       		histosTH2F["hdedx4_nSafePionsNew_234_no_safekaon"]->Fill(in.p, in.dEdx);

	      for (const auto &in : dEdxInfo_Ferenc)
      		histosTH2F["hdedx4_nSafePionsNew_234_no_safekaon_Ferenc"]->Fill(in.p, in.dEdx);
	      
	}

	if((nSafePionsNew == 4) && (nKaons == 0))
	{
	      histosTH2F["hmALLpiKen0_2D_nSafePionsNew_4_no_safekaon"]->Fill(m11, m22);
	      histosTH2F["hmALLpiKen0_2D_nSafePionsNew_4_no_safekaon"]->Fill(m12, m21);
	      
	      for (const auto &in : dEdxInfo_Ferenc)
		histosTH2F["hdedx4_nSafePionsNew_4_no_safekaon_Ferenc"]->Fill(in.p, in.dEdx);
	      
	}


      if(CTpxcut4_strict && CTpycut4_strict)
      {
	      histosTH2F["hmALLpiKen0_2D_strict"]->Fill(m11, m22);
	      histosTH2F["hmALLpiKen0_2D_strict"]->Fill(m12, m21);

	      if(nKaons == 0)
	      {
		      histosTH2F["hmALLpiKen0_2D_strict_no_safe_kaon"]->Fill(m11, m22);
		      histosTH2F["hmALLpiKen0_2D_strict_no_safe_kaon"]->Fill(m12, m21);
	      }

      }

      histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho1"]->Fill(m11_constrained_rho1, m22_constrained_rho1);
      histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho1_wrong_assignment"]->Fill(m12_constrained_rho1_wrong_assignment, m21_constrained_rho1_wrong_assignment);

      histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho2"]->Fill(m12_constrained_rho2, m21_constrained_rho2);
      histosTH2F["hmALLpiKen0_2D_Ken_constrained_rho2_wrong_assignment"]->Fill(m11_constrained_rho2_wrong_assignment, m22_constrained_rho2_wrong_assignment);


     if(number_of_particle_flow_entries_hadrons == 4)
     {
	     histosTH2F["pflow_vs_track_x_hadrons_4pfhadron_allCuts4"]->Fill(sum_of_px, sum_of_PFpx_hadrons);
	     histosTH2F["pflow_vs_track_y_hadrons_4pfhadron_allCuts4"]->Fill(sum_of_py, sum_of_PFpy_hadrons);
	     histosTH2F["pflow_vs_track_z_hadrons_4pfhadron_allCuts4"]->Fill(sum_of_pz, sum_of_PFpz_hadrons);

             histosTH2F["hmALLpiKen0_2D_4pfhadron_allCuts4"]->Fill(m11, m22);
             histosTH2F["hmALLpiKen0_2D_4pfhadron_allCuts4"]->Fill(m12, m21);

     }

     double dpx = fabs(sum_of_px - sum_of_PFpx_hadrons) ;
     double dpy = fabs(sum_of_py - sum_of_PFpy_hadrons) ;
     double dpz = fabs(sum_of_pz - sum_of_PFpz_hadrons) ;

     if((dpx < 0.005) && (dpy < 0.005) && (dpz < 0.005))
     {
	     histosTH2F["pflow_vs_track_x_mom_matching"]->Fill(sum_of_px, sum_of_PFpx_hadrons);
	     histosTH2F["pflow_vs_track_y_mom_matching"]->Fill(sum_of_py, sum_of_PFpy_hadrons);
	     histosTH2F["pflow_vs_track_z_mom_matching"]->Fill(sum_of_pz, sum_of_PFpz_hadrons);

             histosTH2F["hmALLpiKen0_2D_mom_matching"]->Fill(m11, m22);
             histosTH2F["hmALLpiKen0_2D_mom_matching"]->Fill(m12, m21);
     }

      histosTH2F["hmALLpiKen0_2D_new"]->Fill(m11, m22);
      histosTH2F["hmALLpiKen0_2D_new"]->Fill(m12, m21);

      if (fabs(m11-rhoM) < rhoCut) histosTH1F["hmrhootherKen0"]->Fill(m22);
      if (fabs(m12-rhoM) < rhoCut) histosTH1F["hmrhootherKen0"]->Fill(m21);
      
      bool scenario_1_nOK2mass = false ;
      bool scenario_2_nOK2mass = false ;

      bool scenario_1_nOK4mass = false ;
      bool scenario_2_nOK4mass = false ;

      bool scenario_1_nOK8mass = false ;
      bool scenario_2_nOK8mass = false ;

      bool scenario_1_nOK16mass = false ;
      bool scenario_2_nOK16mass = false ;

      int nOK2mass = 0;
      if ((fabs(m11-rhoM) < rhoCut2) && (fabs(m22-rhoM) < rhoCut2))
      {
      	nOK2mass++;
	scenario_1_nOK2mass = true ;
	
	histosTH1F["pi4m11_Px"]->Fill(pi4m11.Px()) ;
	histosTH1F["pi4m22_Px"]->Fill(pi4m22.Px()) ;

	histosTH1F["pi4m11_Py"]->Fill(pi4m11.Py()) ;
	histosTH1F["pi4m22_Py"]->Fill(pi4m22.Py()) ;

	histosTH1F["pi4m11_Pz"]->Fill(pi4m11.Pz()) ;
	histosTH1F["pi4m22_Pz"]->Fill(pi4m22.Pz()) ;

	histosTH1F["pi4m11_E"]->Fill(pi4m11.E()) ;
	histosTH1F["pi4m22_E"]->Fill(pi4m22.E()) ;

	histosTH1F["pi4m11_M"]->Fill(pi4m11.M()) ;
	histosTH1F["pi4m22_M"]->Fill(pi4m22.M()) ;


	histosTH1F["pi4m11_Px_minus_pi4m22_Px"]->Fill(pi4m11.Px() - pi4m22.Px()) ;
	histosTH1F["pi4m11_Py_minus_pi4m22_Py"]->Fill(pi4m11.Py() - pi4m22.Py()) ;
	histosTH1F["pi4m11_Pz_minus_pi4m22_Pz"]->Fill(pi4m11.Pz() - pi4m22.Pz()) ;

      }

      if ((fabs(m12-rhoM) < rhoCut2) && (fabs(m21-rhoM) < rhoCut2))
      {
      	nOK2mass++;
	scenario_2_nOK2mass = true ;

	histosTH1F["pi4m12_Px"]->Fill(pi4m12.Px()) ;
	histosTH1F["pi4m21_Px"]->Fill(pi4m21.Px()) ;

	histosTH1F["pi4m12_Py"]->Fill(pi4m12.Py()) ;
	histosTH1F["pi4m21_Py"]->Fill(pi4m21.Py()) ;

	histosTH1F["pi4m12_Pz"]->Fill(pi4m12.Pz()) ;
	histosTH1F["pi4m21_Pz"]->Fill(pi4m21.Pz()) ;

	histosTH1F["pi4m12_E"]->Fill(pi4m12.E()) ;
	histosTH1F["pi4m21_E"]->Fill(pi4m21.E()) ;

	histosTH1F["pi4m12_M"]->Fill(pi4m12.M()) ;
	histosTH1F["pi4m21_M"]->Fill(pi4m21.M()) ;


	histosTH1F["pi4m12_Px_minus_pi4m21_Px"]->Fill(pi4m12.Px() - pi4m21.Px()) ;
	histosTH1F["pi4m12_Py_minus_pi4m21_Py"]->Fill(pi4m12.Py() - pi4m21.Py()) ;
	histosTH1F["pi4m12_Pz_minus_pi4m21_Pz"]->Fill(pi4m12.Pz() - pi4m21.Pz()) ;
      }

      // Mass constraints 2 sigma
      // ========================================

      int nOK4mass=0;
      
      bool phase_space_cut_3_sigma = false ;
      bool phase_space_cut_1p5_sigma = false ;

      const double my_projection = (pi4pos2.Pt() + pi4neg2.Pt()) / (sqrt(2)) ;
      histosTH1F["hpt_rho_projection"]->Fill(my_projection) ;

      // const double reference_projection = 0.7 / sqrt(2.0) ;
      const double reference_projection = 0.5276 ;

      if(fabs(my_projection - reference_projection) < 0.18) phase_space_cut_3_sigma = true ;
      if(fabs(my_projection - reference_projection) < 0.09) phase_space_cut_1p5_sigma = true ;

      if((fabs(m11-rhoM) < rhoCut4) && (fabs(m22-rhoM) < rhoCut4))
      {
      	nOK4mass++;
      	scenario_1_nOK4mass = true ;
	
      	histosTH2F["hpt_rho"]->Fill(pi4pos1.Pt(), pi4neg1.Pt()) ;
      	histosTH2F["hpt_rho"]->Fill(pi4pos2.Pt(), pi4neg2.Pt()) ;

      }

      if((fabs(m12-rhoM) < rhoCut4) && (fabs(m21-rhoM) < rhoCut4))
      {
      	nOK4mass++;
      	scenario_2_nOK4mass = true ;

      	histosTH2F["hpt_rho"]->Fill(pi4pos1.Pt(), pi4neg2.Pt()) ;
        histosTH2F["hpt_rho"]->Fill(pi4pos2.Pt(), pi4neg1.Pt()) ;

      }

      // Scanning with 2 sigma (some other plots as well)

      map< int, map<int, double >> nOK4mass_matrix ;
      map< int, map<int, double >> nOK8mass_matrix ;
      map< int, map<int, double >> nOK16mass_matrix ;

      if(mass_scanning)
      {
	      for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
	      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
	      {
        	nOK4mass_matrix[ni][nj] = 0  ;
        	nOK8mass_matrix[ni][nj] = 0  ;
        	nOK16mass_matrix[ni][nj] = 0 ;
	      }

	      for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
	      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
	      {
		if((fabs((m11 - rhoM) + (ni * rhoCut4)) < rhoCut4) && (fabs((m22 - rhoM) + (nj * rhoCut4)) < rhoCut4)) nOK4mass_matrix[ni][nj]++ ;
		if((fabs((m12 - rhoM) + (ni * rhoCut4)) < rhoCut4) && (fabs((m21 - rhoM) + (nj * rhoCut4)) < rhoCut4)) nOK4mass_matrix[ni][nj]++ ;
	      }
      }

      // Mass constraints 4 sigma
      // ========================================

      int nOK8mass=0;
      if((fabs(m11-rhoM) < rhoCut8) && (fabs(m22-rhoM) < rhoCut8))
      {
	nOK8mass++;
	scenario_1_nOK8mass = true ;
      }

      if((fabs(m12-rhoM) < rhoCut8) && (fabs(m21-rhoM) < rhoCut8))
      {
	nOK8mass++;
	scenario_2_nOK8mass = true ;
      }

      // Scanning with 4 sigma
      
      if(mass_scanning)
      {
      
	      for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
	      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
	      {	
        	if((fabs((m11 - rhoM) + (ni * rhoCut8)) < rhoCut8) && (fabs((m22 - rhoM) + (nj * rhoCut8)) < rhoCut8)) nOK8mass_matrix[ni][nj]++ ;
	        if((fabs((m12 - rhoM) + (ni * rhoCut8)) < rhoCut8) && (fabs((m21 - rhoM) + (nj * rhoCut8)) < rhoCut8)) nOK8mass_matrix[ni][nj]++ ;
	      }
      }

      // Mass constraints 8 sigma
      // ========================================
       
      int nOK16mass=0;
      if((fabs(m11-rhoM) < rhoCut16) && (fabs(m22-rhoM) < rhoCut16))
      {
	nOK16mass++;
	scenario_1_nOK16mass = true ;
      }
      
      if((fabs(m12-rhoM) < rhoCut16) && (fabs(m21-rhoM) < rhoCut16))
      {
	nOK16mass++;
	scenario_2_nOK16mass = true ;
      }

      // Scanning with 8 sigma
      
      if(mass_scanning)
      {
      
	      for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
	      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
	      {
        	if((fabs((m11 - rhoM) + (ni * rhoCut16)) < rhoCut16) && (fabs((m22 - rhoM) + (nj * rhoCut16)) < rhoCut16)) nOK16mass_matrix[ni][nj]++ ;
	        if((fabs((m12 - rhoM) + (ni * rhoCut16)) < rhoCut16) && (fabs((m21 - rhoM) + (nj * rhoCut16)) < rhoCut16)) nOK16mass_matrix[ni][nj]++ ;
	      }
      }

      // Mass cuts with rho mass from 2D mass plot itself
      
      double rhoM_from_plot_x = rhoM_from_plot ;
      double rhoM_from_plot_y = rhoM_from_plot ;

      const int topology_parallel = 1 ;
      const int topology_diagonal = 2 ;

      int topology = topology_parallel ;

      if(topology == topology_diagonal)
      {
      	rhoM_from_plot_x = rhoM_from_plot_v3_diagonal_x ;
      	rhoM_from_plot_y = rhoM_from_plot_v3_diagonal_y ;
      }

      int nOK4mass_from_plot = 0;
      int nOK8mass_from_plot = 0;
      int nOK16mass_from_plot = 0;

      int nOK16mass_from_plot_v2 = 0;

      if((fabs(m11 - rhoM_from_plot_x) < rhoCut4) && (fabs(m22 -   rhoM_from_plot_y) < rhoCut4)) nOK4mass_from_plot++;
      if((fabs(m12 - rhoM_from_plot_x) < rhoCut4) && (fabs(m21 -   rhoM_from_plot_y) < rhoCut4)) nOK4mass_from_plot++;

      if((fabs(m11 - rhoM_from_plot_x) < rhoCut8) && (fabs(m22 -   rhoM_from_plot_y) < rhoCut8)) nOK8mass_from_plot++;
      if((fabs(m12 - rhoM_from_plot_x) < rhoCut8) && (fabs(m21 -   rhoM_from_plot_y) < rhoCut8)) nOK8mass_from_plot++;

      if((fabs(m11 - rhoM_from_plot_x) < rhoCut16) && (fabs(m22 -  rhoM_from_plot_y) < rhoCut16)) nOK16mass_from_plot++;
      if((fabs(m12 - rhoM_from_plot_x) < rhoCut16) && (fabs(m21 -  rhoM_from_plot_y) < rhoCut16)) nOK16mass_from_plot++;

      if((fabs(m11 - rhoM_from_plot_v2) < rhoCut16) && (fabs(m22 - rhoM_from_plot_v2) < rhoCut16)) nOK16mass_from_plot_v2++;
      if((fabs(m12 - rhoM_from_plot_v2) < rhoCut16) && (fabs(m21 - rhoM_from_plot_v2) < rhoCut16)) nOK16mass_from_plot_v2++;

      // ###################
      // Old dEdx plots
      // ###################

      if(nOK2mass  > 0)
      {
        histosTH1F["hnPionsSIG1"]->Fill(nPions);

        {
          histosTH1F["hm4SIG1mass"]->Fill(mrec4);
	  
	  if(phase_space_cut_3_sigma) histosTH1F["hm4SIG1mass_phase_space_cut_3_sigma"]->Fill(mrec4);
	  if(phase_space_cut_1p5_sigma) histosTH1F["hm4SIG1mass_phase_space_cut_1p5_sigma"]->Fill(mrec4);
	  
	  if(scenario_1_nOK2mass) histosTH1F["hm4SIG1mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho1);
	  if(scenario_2_nOK2mass) histosTH1F["hm4SIG1mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho2);
	  
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4SIG1mass"]->Fill(in.p, in.dEdx);
        }

        if (nPions == 4)
        {
          histosTH1F["hm4SIG1mass4curves"]->Fill(mrec4);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4SIG1mass4curves"]->Fill(in.p, in.dEdx);
        }

        if (nPions >= 3)
        {
          histosTH1F["hm4SIG1mass34curves"]->Fill(mrec4);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4SIG1mass34curves"]->Fill(in.p, in.dEdx);
        }

        if (nPions >= 2)
        {
          histosTH1F["hm4SIG1mass234curves"]->Fill(mrec4);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4SIG1mass234curves"]->Fill(in.p, in.dEdx);
        }

        if (nPions >= 1)
        {
          histosTH1F["hm4SIG1mass1234curves"]->Fill(mrec4);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4SIG1mass1234curves"]->Fill(in.p, in.dEdx);
        }


        if((number_of_tracks_with_too_large_chi2 == 0) && (number_of_tracks_with_too_small_chi2_1 == 0))
	{
		histosTH1F["hm4SIG1mass_track_quality_cut_low_and_high"]->Fill(mrec4);
	}

      }

      if (nOK4mass>0)
      {
        map_of_conditions_if_they_are_applied["if (nOK4mass>0)"] = 1 ;      

        histosTH1F["hm4SIG2mass"]->Fill(mrec4);
        histosTH1F["hm4SIG2mass_offset"]->Fill(mrec4);

	if(phase_space_cut_3_sigma)
	{
		histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma"]->Fill(mrec4);
		histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_pt_corrected"]->Fill(mrec4_pt_corrected);
    
    if((pi4pos1.Pt() > 0.4) && (pi4pos2.Pt() > 0.4) && (pi4neg1.Pt() > 0.4) && (pi4neg2.Pt() > 0.4))
    {
      histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut"]->Fill(mrec4);
    }

    if((pi4pos1.Pt() > 0.4) || (pi4pos2.Pt() > 0.4) || (pi4neg1.Pt() > 0.4) || (pi4neg2.Pt() > 0.4))
    {
      histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_or"]->Fill(mrec4);
    }
    
    if(ptok >= 2)
    {
      histosTH1F["hm4SIG2mass_phase_space_cut_3_sigma_and_pt_cut_ok"]->Fill(mrec4);
    }
    

        	if (nPions == 4) histosTH1F["hm4SIG2mass4curves_phase_space_cut_3_sigma"]->Fill(mrec4);
        	if (nPions >= 3) histosTH1F["hm4SIG2mass34curves_phase_space_cut_3_sigma"]->Fill(mrec4);
        	if (nPions >= 2) histosTH1F["hm4SIG2mass234curves_phase_space_cut_3_sigma"]->Fill(mrec4);
        	if (nPions >= 1) histosTH1F["hm4SIG2mass1234curves_phase_space_cut_3_sigma"]->Fill(mrec4);
	}
	
	if(phase_space_cut_1p5_sigma)
	{
		histosTH1F["hm4SIG2mass_phase_space_cut_1p5_sigma"]->Fill(mrec4);

        	if (nPions == 4) histosTH1F["hm4SIG2mass4curves_phase_space_cut_1p5_sigma"]->Fill(mrec4);
        	if (nPions >= 3) histosTH1F["hm4SIG2mass34curves_phase_space_cut_1p5_sigma"]->Fill(mrec4);
        	if (nPions >= 2) histosTH1F["hm4SIG2mass234curves_phase_space_cut_1p5_sigma"]->Fill(mrec4);
        	if (nPions >= 1) histosTH1F["hm4SIG2mass1234curves_phase_space_cut_1p5_sigma"]->Fill(mrec4);
	}

        if(scenario_1_nOK4mass) histosTH1F["hm4SIG2mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho1);
        if(scenario_2_nOK4mass) histosTH1F["hm4SIG2mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho2);

	
	if(CMS_pt < 0.2) histosTH1F["hm4SIG2mass_pt_cut_lower_200_MeV"]->Fill(mrec4);
	if(fabs(zvtx) < 5.0) histosTH1F["hm4SIG2mass_zvtx"]->Fill(mrec4);

        histosTH1F["hnPionsSIG2"]->Fill(nPions);
        if (nPions == 4) histosTH1F["hm4SIG2mass4curves"]->Fill(mrec4);
        if (nPions >= 3) histosTH1F["hm4SIG2mass34curves"]->Fill(mrec4);
        if (nPions >= 2) histosTH1F["hm4SIG2mass234curves"]->Fill(mrec4);
        if (nPions >= 1) histosTH1F["hm4SIG2mass1234curves"]->Fill(mrec4);

	bool diagonal = (diag_top45_bot56 || diag_bot45_top56);
	bool parallel = (top45_top56 || bot45_bot56);

	bool diagonal_strict = ((tb == 0) || (tb == 1));
	bool parallel_strict = ((tb == 2) || (tb == 3));

	if(diagonal)
	{
		histosTH1F["hm4SIG2massdiagonal"]->Fill(mrec4);
	}

	if(parallel)
	{
		histosTH1F["hm4SIG2massparallel"]->Fill(mrec4);
	}

	if(diagonal_strict)
	{
		histosTH1F["hm4SIG2massdiagonal_strict"]->Fill(mrec4);
	}

	if(parallel_strict)
	{
		histosTH1F["hm4SIG2massparallel_strict"]->Fill(mrec4);
	}
        map_of_conditions_if_they_are_applied["if (nOK4mass>0)"] = 0 ;      
      }

      if (nOK8mass > 0)
      {
        map_of_conditions_if_they_are_applied["if (nOK8mass>0)"] = 1 ;      

        histosTH1F["hm4SIG4mass"]->Fill(mrec4);

        if(scenario_1_nOK8mass) histosTH1F["hm4SIG4mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho1);
        if(scenario_2_nOK8mass) histosTH1F["hm4SIG4mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho2);

        map_of_conditions_if_they_are_applied["if (nOK8mass>0)"] = 0 ;      
      }

      if (nOK16mass > 0)
      {
        map_of_conditions_if_they_are_applied["if (nOK16mass>0)"] = 1 ;      

        histosTH1F["hm4SIG8mass"]->Fill(mrec4);

        if(scenario_1_nOK16mass) histosTH1F["hm4SIG8mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho1);
        if(scenario_2_nOK16mass) histosTH1F["hm4SIG8mass_Ken_constrained_rho"]->Fill(mrec4_Ken_constrained_rho2);

        map_of_conditions_if_they_are_applied["if (nOK16mass>0)"] = 0 ;      
      }


	// Mass scanning plots
	
      if(mass_scanning)
      {
	      for(int ni = lower_index_for_matrix ; ni < upper_index_for_matrix ; ++ni)
	      for(int nj = lower_index_for_matrix ; nj < upper_index_for_matrix ; ++nj)
	      {
        	stringstream suffix_1, suffix_2 ;
        	suffix_1 << (ni - lower_index_for_matrix) ;
        	suffix_2 << (nj - lower_index_for_matrix) ;

        	string name_for_histo = ("hm4SIG2mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        	if(nOK4mass_matrix[ni][nj] > 0) histosTH1F[name_for_histo.c_str()]->Fill(mrec4) ;

        	name_for_histo = ("hm4SIG4mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        	if(nOK8mass_matrix[ni][nj] > 0) histosTH1F[name_for_histo.c_str()]->Fill(mrec4) ;

        	name_for_histo = ("hm4SIG8mass_" + suffix_1.str() + "_" + suffix_2.str()) ;
        	if(nOK16mass_matrix[ni][nj] > 0) histosTH1F[name_for_histo.c_str()]->Fill(mrec4) ;

	      }
      }
	
	

      if(nOK4mass_from_plot > 0)
      {
        	histosTH1F["hm4SIG2mass_from_plot"]->Fill(mrec4);
	        histosTH1F["hm4SIG2mass_from_plot_offset"]->Fill(mrec4);

		// Very simple electron cuts on dE/dx
		
		if(nElectrons_horizontal_and_vertical_curve ==  0) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_0"]->Fill(mrec4);
		if(nElectrons_horizontal_and_vertical_curve <=  1) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_1"]->Fill(mrec4);
		if(nElectrons_horizontal_and_vertical_curve <=  2) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_2"]->Fill(mrec4);
		if(nElectrons_horizontal_and_vertical_curve <=  3) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_3"]->Fill(mrec4);
		if(nElectrons_horizontal_and_vertical_curve <=  4) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_and_vertical_curve_4"]->Fill(mrec4);

                if(nElectrons_horizontal_curve ==  0) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_0"]->Fill(mrec4);
                if(nElectrons_horizontal_curve <=  1) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_1"]->Fill(mrec4);
                if(nElectrons_horizontal_curve <=  2) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_2"]->Fill(mrec4);
                if(nElectrons_horizontal_curve <=  3) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_3"]->Fill(mrec4);
                if(nElectrons_horizontal_curve <=  4) histosTH1F["hm4SIG2mass_from_plot_nElectrons_horizontal_curve_4"]->Fill(mrec4);

		// Cut on electrons with Particle Flow, doesn't really work

                if(number_of_particle_flow_entries_electrons ==  0) histosTH1F["hm4SIG2mass_from_plot_electrons_0"]->Fill(mrec4);
                if(number_of_particle_flow_entries_electrons ==  0) histosTH1F["hm4SIG2mass_from_plot_offset_electrons_0"]->Fill(mrec4);

                if(number_of_particle_flow_entries_electrons <=  1) histosTH1F["hm4SIG2mass_from_plot_electrons_1"]->Fill(mrec4);
                if(number_of_particle_flow_entries_electrons <=  1) histosTH1F["hm4SIG2mass_from_plot_offset_electrons_1"]->Fill(mrec4);

                if(number_of_particle_flow_entries_electrons <=  2) histosTH1F["hm4SIG2mass_from_plot_electrons_2"]->Fill(mrec4);
                if(number_of_particle_flow_entries_electrons <=  2) histosTH1F["hm4SIG2mass_from_plot_offset_electrons_2"]->Fill(mrec4);

                if(number_of_particle_flow_entries_electrons <=  3) histosTH1F["hm4SIG2mass_from_plot_electrons_3"]->Fill(mrec4);
                if(number_of_particle_flow_entries_electrons <=  3) histosTH1F["hm4SIG2mass_from_plot_offset_electrons_3"]->Fill(mrec4);

	
		if (nSafePionsNew >= 3)
		{
			histosTH1F["hm4SIG2mass_from_plot_nSafePionsNew_34"]->Fill(mrec4);
			histosTH1F["hm4SIG2mass_from_plot_offset_nSafePionsNew_34"]->Fill(mrec4);
		}

		if ((nSafePionsNew >= 2) && (nKaons == 0))
		{
			histosTH1F["hm4SIG2mass_from_plot_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);
			histosTH1F["hm4SIG2mass_from_plot_offset_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);
		}

		if(CMS_pt < 0.2) histosTH1F["hm4SIG2mass_from_plot_pt"]->Fill(mrec4);
      }

      if(nOK8mass_from_plot > 0)
      {
		histosTH1F["hm4SIG4mass_from_plot"]->Fill(mrec4);
		histosTH1F["hm4SIG4mass_from_plot_offset"]->Fill(mrec4);

		if (nSafePionsNew >= 3)
		{
			histosTH1F["hm4SIG4mass_from_plot_nSafePionsNew_34"]->Fill(mrec4);
			histosTH1F["hm4SIG4mass_from_plot_offset_nSafePionsNew_34"]->Fill(mrec4);
		}

		if ((nSafePionsNew >= 2) && (nKaons == 0))
		{
			histosTH1F["hm4SIG4mass_from_plot_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);
			histosTH1F["hm4SIG4mass_from_plot_offset_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);
		}

		if(CMS_pt < 0.2) histosTH1F["hm4SIG4mass_from_plot_pt"]->Fill(mrec4);
      }

      if(nOK16mass_from_plot > 0)
      {
		histosTH1F["hm4SIG8mass_from_plot"]->Fill(mrec4);
		histosTH1F["hm4SIG8mass_from_plot_offset"]->Fill(mrec4);
		histosTH1F["hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak"]->Fill(mrec4 + 0.1);
		histosTH1F["hm4SIG8mass_from_plot_offset_match_2015_Mirkos_peak_original_binning"]->Fill(mrec4 + 0.1);

		if (nSafePionsNew >= 3)
		{
			histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_34"]->Fill(mrec4);
			histosTH1F["hm4SIG8mass_from_plot_offset_nSafePionsNew_34"]->Fill(mrec4);
		}

		if ((nSafePionsNew >= 2) && (nKaons == 0))
		{
			histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);
			histosTH1F["hm4SIG8mass_from_plot_offset_nSafePionsNew_234_no_safekaon"]->Fill(mrec4);


			histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_original_binning"]->Fill(mrec4 + 0.1);
		}
		
		if(tracks_below_p_limit_to_separate_K_pi == 4)
		{
			histosTH1F["hm4SIG8mass_from_plot_below_th"]->Fill(mrec4);

			if (nSafePionsNew >= 3)
			{
				histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_34_below_th"]->Fill(mrec4);
			}

			if ((nSafePionsNew >= 2) && (nKaons == 0))
			{
				histosTH1F["hm4SIG8mass_from_plot_nSafePionsNew_234_no_safekaon_below_th"]->Fill(mrec4);
			}
		}

	for (const auto &in : dEdxInfo)
	{
		histosTH2F["hm4SIG8mass_from_plot_dEdx_info"]->Fill(in.p, in.dEdx);

		if(number_of_particle_flow_entries_electrons ==  0) histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_0"]->Fill(in.p, in.dEdx);
		if(number_of_particle_flow_entries_electrons <=  1) histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_1"]->Fill(in.p, in.dEdx);
		if(number_of_particle_flow_entries_electrons <=  2) histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_2"]->Fill(in.p, in.dEdx);
		if(number_of_particle_flow_entries_electrons <=  3) histosTH2F["hm4SIG8mass_from_plot_dEdx_info_electrons_3"]->Fill(in.p, in.dEdx);
	}

      }

      if(nOK16mass_from_plot_v2 > 0)
      {
    		histosTH1F["hm4SIG8mass_from_plot_v2"]->Fill(mrec4);
    	  histosTH1F["hm4SIG8mass_from_plot_offset_v2"]->Fill(mrec4);

        for (const auto &in : dEdxInfo)
   		  histosTH2F["hm4SIG8mass_from_plot_dEdx_info_v2"]->Fill(in.p, in.dEdx);
      }
      
      //------------------------
      // K0 - K0
      
      const double GeV_to_MeV = 1000.0 ;
      const double MeV_to_GeV = 1.0 / GeV_to_MeV ;
      
      const double m_K0_MeV = 497.6 ;
      const double sigma_K0_observed_MeV = 15.8 ;

      const double m_K0_GeV = m_K0_MeV * MeV_to_GeV ;
      const double sigma_K0_observed_GeV = sigma_K0_observed_MeV * MeV_to_GeV ;
      
      if((fabs(m11 - m_K0_GeV) < (2.0 * sigma_K0_observed_GeV)) && (fabs(m22 - m_K0_GeV) < (2.0 * sigma_K0_observed_GeV)))
      {
      	histosTH2F["hpt_K0"]->Fill(pi4pos1.Pt(), pi4neg1.Pt()) ;
      	histosTH2F["hpt_K0"]->Fill(pi4pos2.Pt(), pi4neg2.Pt()) ;
      }
      


      //------------------------
      // phi-phi

      const double m11k = (k4pos1 + k4neg1).M();
      const double m22k = (k4pos2 + k4neg2).M();

      const double m12k = (k4pos1 + k4neg2).M();
      const double m21k = (k4pos2 + k4neg1).M();

      const double m4k = (k4pos1 + k4pos2 + k4neg1 + k4neg2).M();

      const double phiCen = 1.02;
      const double phiCut = 0.02;
      const double phiCutStrict = 0.01;

      const double phiCen_control = 1.09;
      const double phiCut_control = 0.04;

      histosTH1F["hmALLkKen0"]->Fill(m11k);
      histosTH1F["hmALLkKen0"]->Fill(m12k);

      histosTH2F["hmALLkKen0_2D"]->Fill(m11k, m22k);
      histosTH2F["hmALLkKen0_2D"]->Fill(m12k, m21k);
		
		if((negative_and_in_dEdx_range == 2) || (positive_and_in_dEdx_range == 2))
		{
	      histosTH2F["hmALLkKen0_2D_2SS_in_dEdx_range"]->Fill(m11k, m22k);
   	   histosTH2F["hmALLkKen0_2D_2SS_in_dEdx_range"]->Fill(m12k, m21k);
		}

      histosTH2F["hmALLkKen0_2D_high_p"]->Fill(m11k, m22k);
      histosTH2F["hmALLkKen0_2D_high_p"]->Fill(m12k, m21k);
      
      if(tracks_below_p_limit_to_separate_K_pi == 4)
      {
	      histosTH2F["hmALLkKen0_2D_p_condition_high_p"]->Fill(m11k, m22k);
	      histosTH2F["hmALLkKen0_2D_p_condition_high_p"]->Fill(m12k, m21k);
      }      

      histosTH2F["hmALLkKen0_2D_new"]->Fill(m11k, m22k);
      histosTH2F["hmALLkKen0_2D_new"]->Fill(m12k, m21k);

      if (fabs(m11k - phiCen) < phiCut) histosTH1F["hmphiotherKen0"]->Fill(m22k);
      if (fabs(m12k - phiCen) < phiCut) histosTH1F["hmphiotherKen0"]->Fill(m21k);

      if (fabs(m11k - phiCen) < phiCutStrict) histosTH1F["hmphiotherKen0_PhiCutStrict"]->Fill(m22k);
      if (fabs(m12k - phiCen) < phiCutStrict) histosTH1F["hmphiotherKen0_PhiCutStrict"]->Fill(m21k);

      unsigned int nOKPhiCut = 0;

      if ((fabs(m11k - phiCen) < phiCut) && (fabs(m22k - phiCen) < phiCut))
      {
        nOKPhiCut++;

      	histosTH2F["hpt_phi"]->Fill(k4pos1.Pt(), k4neg1.Pt()) ;
      	histosTH2F["hpt_phi"]->Fill(k4pos2.Pt(), k4neg2.Pt()) ;
      }

      if ((fabs(m12k - phiCen) < phiCut) && (fabs(m21k - phiCen) < phiCut))
      {
      	nOKPhiCut++;

      	histosTH2F["hpt_phi"]->Fill(k4pos1.Pt(), k4neg2.Pt()) ;
      	histosTH2F["hpt_phi"]->Fill(k4pos2.Pt(), k4neg1.Pt()) ;
      }

      unsigned int nOKPhiCut_control = 0;

      if ((fabs(m11k - phiCen_control) < phiCut_control) && (fabs(m22k - phiCen_control) < phiCut_control))
      {
        nOKPhiCut_control++;
      }

      if ((fabs(m12k - phiCen_control) < phiCut_control) && (fabs(m21k - phiCen_control) < phiCut_control))
      {
      	nOKPhiCut_control++;
      }

      unsigned int nOKPhiCutStrict = 0;
      if ((fabs(m11k - phiCen) < phiCutStrict) && (fabs(m22k - phiCen) < phiCutStrict)) nOKPhiCutStrict++;
      if ((fabs(m12k - phiCen) < phiCutStrict) && (fabs(m21k - phiCen) < phiCutStrict)) nOKPhiCutStrict++;

      bool twoKaonsOfTheSameSign = ((nKaonsPos == 2) || (nKaonsNeg == 2));
      bool twoKaonsOfTheSameSign_FS = ((nKaonsPos_FS == 2) || (nKaonsNeg_FS == 2));
      bool twoPionsOfTheSameSign = ((nPionsPos == 2) || (nPionsNeg == 2));

	
      bool twoKaonsOfTheSameSignArray[kaon_step_max] ;

      for(int ki = 0 ; ki < kaon_step_max ; ++ki)
      {
      	twoKaonsOfTheSameSignArray[ki] = ((nKaonsPosArray[ki] == 2) || (nKaonsNegArray[ki] == 2));
      }
      
      if(twoKaonsOfTheSameSign)
      {
	      histosTH2F["hmALLkKen0_2D_2SS_v0"]->Fill(m11k, m22k);
	      histosTH2F["hmALLkKen0_2D_2SS_v0"]->Fill(m12k, m21k);

              for (const auto &in : dEdxInfo)
                 histosTH2F["hdedx4PHImass_2SS_v0"]->Fill(in.p, in.dEdx);
		 
              for (const auto &in : dEdxInfo_Ferenc)
                 histosTH2F["hdedx4PHImass_2SS_v0_Ferenc"]->Fill(in.p, in.dEdx);
      }

      if(twoKaonsOfTheSameSign_FS)
      {
	      histosTH2F["hmALLkKen0_2D_2SS_v0_FS"]->Fill(m11k, m22k);
	      histosTH2F["hmALLkKen0_2D_2SS_v0_FS"]->Fill(m12k, m21k);

              for (const auto &in : dEdxInfo_Ferenc)
                 histosTH2F["hdedx4PHImass_2SS_v0_Ferenc_FS"]->Fill(in.p, in.dEdx);
      }

		if (twoPionsOfTheSameSign)
		{

	      histosTH2F["hmALLkKen0_2D_2piSS"]->Fill(m11k, m22k);
	      histosTH2F["hmALLkKen0_2D_2piSS"]->Fill(m12k, m21k);

		 for (const auto &in : dEdxInfo)
   		histosTH2F["hdedx4_twoPionsOfTheSameSign_v0"]->Fill(in.p, in.dEdx);
		}

      if(nOKPhiCut_control > 0)
      {
				 for (const auto &in : dEdxInfo_Ferenc)
				 {
			         histosTH2F["hdedx4PHImass_Ferenc_control"]->Fill(in.p, in.dEdx);
				 }
		}

      if(nOKPhiCut > 0)
      {
        if(twoKaonsOfTheSameSign_FS) cout << "twoKaonsOfTheSameSign_FS" << endl ;

        if(test_dedx) cout << "eventprintout nOKPhiCut " << iEvent.id() << endl ;
      
        if(tracks_below_p_limit_to_separate_K_pi == 4)
        {
          histosTH2F["hmALLpiKen0_2D_p_condition_after_phi_cut"]->Fill(m11, m22);
          histosTH2F["hmALLpiKen0_2D_p_condition_after_phi_cut"]->Fill(m12, m21);
        }

        if((m4k > 2.210) && (m4k < 2.230))
        {
           histosTH2F["hmALLpiKen0_2D_p_condition_after_4massk_cut"]->Fill(m11, m22);
           histosTH2F["hmALLpiKen0_2D_p_condition_after_4massk_cut"]->Fill(m12, m21);
        }
      
        histosTH1F["hnKaons"]->Fill(nKaons);
        histosTH1F["hnKaons_FS"]->Fill(nKaons_FS);
		  
        if((nSafePionsNew == 3) && (nKaons_FS == 1))
        {
          bool plot_suspicious_events = false ;

          for (const auto &in : dEdxInfo_Ferenc)
          {
            if(plot_suspicious_events) cout << "plot_suspicious_events: " << iEvent.id() << " p: " << in.p << "  dE/dx: " <<  in.dEdx << endl ;

            histosTH2F["hdedx4PHImass_test_of_3_pions_and_1_kaon"]->Fill(in.p, in.dEdx);
          }
        }

        if((nSafePions == 3) && (nKaons == 1))
        {
          bool plot_suspicious_events = false ;

          for (const auto &in : dEdxInfo)
          {
            if(plot_suspicious_events) cout << "plot_suspicious_old_dedx_events: " << iEvent.id() << " p: " << in.p << "  dE/dx: " <<  in.dEdx << endl ;

            histosTH2F["hdedx4PHImass_test_of_3_pions_and_1_kaon_old_dedx"]->Fill(in.p, in.dEdx);
          }
  		  }


        if(nSuperSafePionsNew == 0)
        {
          histosTH1F["hm4PHImass_nSuperSafePionsNew_0"]->Fill(m4k);
        }

        if(tracks_below_p_limit_to_separate_K_pi_Simone == 4)
        {
          histosTH1F["hnKaons_FS_p_cut"]->Fill(nKaons_FS);
        }

          histosTH1F["hm4PHImass"]->Fill(m4k);
          histosTH1F["hm4PHImass_bin1"]->Fill(m4k);
          histosTH1F["hm4PHImass_bin2"]->Fill(m4k);
          histosTH1F["hm4PHImass_bin3"]->Fill(m4k);
          
          if(is_event_with_reasonable_number_of_dedx_measurments)
          {
            histosTH1F["hm4PHImass_reasonable_dedx_measurement"]->Fill(m4k);

    				for (const auto &in : dEdxInfo_Ferenc)
   	  				histosTH2F["hdedxAllHits_reasonable_dedx_measurement"]->Fill(in.p, in.dEdx);
          }
	  
	  if(tracks_above_dedx_limit_of_Simone_and_below_p_limit == 4) histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_4"]->Fill(m4k);
	  if(tracks_above_dedx_limit_of_Simone_and_below_p_limit == 3) histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_3"]->Fill(m4k);
	  if(tracks_above_dedx_limit_of_Simone_and_below_p_limit == 2) histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_2"]->Fill(m4k);
	  if(tracks_above_dedx_limit_of_Simone_and_below_p_limit == 1) histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_1"]->Fill(m4k);
	  if(tracks_above_dedx_limit_of_Simone_and_below_p_limit == 0) histosTH1F["hm4PHImass_tracks_above_dedx_limit_of_Simone_and_below_p_limit_0"]->Fill(m4k);

          histosTH1F["hm4PHImass_with_pion_mass"]->Fill(mrec4);

          if(tracks_below_p_limit_to_separate_K_pi == 4)
			 {
			 	histosTH1F["hm4PHImass_below_th"]->Fill(m4k);

				for (const auto &in : dEdxInfo_Ferenc)
					histosTH2F["hdedxAllHits_special_phi_cuts_control_1_Simone"]->Fill(in.p, in.dEdx);

               // cout << "hello1" << endl ;

				for (const auto &in : dEdxInfo)
					histosTH2F["hdedxAllHits_special_phi_cuts_control_1_Simone_2"]->Fill(in.p, in.dEdx);

               // cout << "hello2" << endl ;
				

				if(tracks_above_dedx_limit_to_separate_K_pi == 4)
				{
				 	histosTH1F["hm4PHImass_below_th_and_above_dedx_th"]->Fill(m4k);

				         for (const auto &in : dEdxInfo_Ferenc)
						histosTH2F["hdedxAllHits_special_phi_cuts_control_1"]->Fill(in.p, in.dEdx);

					if(tracks_above_p_limit_to_separate_K_pi == 4)
					{
			        	 	for (const auto &in : dEdxInfo_Ferenc) histosTH2F["hdedxAllHits_special_phi_cuts_control_2"]->Fill(in.p, in.dEdx);
					}

				}
				else if(tracks_above_dedx_limit_to_separate_K_pi == 3)
				{
				 	histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3"]->Fill(m4k);
					
					if(tracks_above_p_limit_to_separate_K_pi == 4) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_4"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 3) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_3"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 2) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_2"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 1) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_3_above_p_limit_1"]->Fill(m4k) ;
				}
				else if(tracks_above_dedx_limit_to_separate_K_pi == 2)
				{
				 	histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2"]->Fill(m4k);

					for (const auto &in : dEdxInfo_Ferenc)
						histosTH2F["hdedxAllHits_special_phi_cuts_1"]->Fill(in.p, in.dEdx);
					
	
					if(tracks_above_p_limit_to_separate_K_pi == 4) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_4"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 3) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_3"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 2) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_2"]->Fill(m4k) ;
					if(tracks_above_p_limit_to_separate_K_pi == 1) histosTH1F["hm4PHImass_below_th_and_above_dedx_th_2_above_p_limit_1"]->Fill(m4k) ;
					
					if(tracks_above_p_limit_to_separate_K_pi == 4) for (const auto &in : dEdxInfo_Ferenc) histosTH2F["hdedxAllHits_special_phi_cuts_2"]->Fill(in.p, in.dEdx);
				}
				else if(tracks_above_dedx_limit_to_separate_K_pi == 1)
				{
				 	histosTH1F["hm4PHImass_below_th_and_above_dedx_th_1"]->Fill(m4k);
				}

			  }

//          for (const auto &in : dEdxInfo)
//            histosTH2F["hdedx4PHImass"]->Fill(in.p, in.dEdx);

          
			 vector<double> temporary_dEdx_values_to_test_false_association ;
			 const double selected_momentum = 0.494758 ;

				 for (const auto &in : dEdxInfo_Ferenc)
				 {
			         histosTH2F["hdedx4PHImass_Ferenc"]->Fill(in.p, in.dEdx);
						temporary_dEdx_values_to_test_false_association.push_back(in.dEdx) ;
						
						if(fabs(in.p - selected_momentum) < 0.05)
						{
							histosTH1F["hdedx4PHImass_Ferenc_momentum_slice"]->Fill(in.dEdx);
						}
				 }

			          if(temporary_dEdx_values_to_test_false_association.size() == 4)
			 {
			    int ifalse = 0 ;

	          for (const auto &in : dEdxInfo_Ferenc)
				 {
                histosTH2F["hdedx4PHImass_Ferenc_false_1"]->Fill(in.p, temporary_dEdx_values_to_test_false_association[(ifalse + 1) % 4]);
                histosTH2F["hdedx4PHImass_Ferenc_false_2"]->Fill(in.p, temporary_dEdx_values_to_test_false_association[(ifalse + 2) % 4]);
                histosTH2F["hdedx4PHImass_Ferenc_false_3"]->Fill(in.p, temporary_dEdx_values_to_test_false_association[(ifalse + 3) % 4]);
                histosTH2F["hdedx4PHImass_Ferenc_false_4"]->Fill(in.p, temporary_dEdx_values_to_test_false_association[(ifalse + 4) % 4]);

					 ++ifalse ;
				 }
			 }
			 
			 temporary_dEdx_values_to_test_false_association.clear() ;

          if ((nOKPhiCut > 0) && (m4k > 2.200) && (m4k < 2.240))
          {
	          histosTH1F["hm4PHImass_with_pion_mass_peak"]->Fill(mrec4);

	          for (const auto &in : dEdxInfo_Ferenc)
			histosTH2F["hdedx4PHImass_Ferenc_peak"]->Fill(in.p, in.dEdx);
			
		if(nPions == 4)
		{
			histosTH1F["hm4PHImass_4pions"]->Fill(m4k);
		}

		if(nKaons == 4) 
		{
			histosTH1F["hm4PHImass_4kaons"]->Fill(m4k);
		}
          }
   
          if ((nOKPhiCut > 0) && (m4k > 2.220) && (m4k < 2.240))
          {
                  for (const auto &in : dEdxInfo_Ferenc)
		  {
                        histosTH2F["hdedx4PHImass_Ferenc_peak_high"]->Fill(in.p, in.dEdx);
			histosTH1F["p_peak_high"]->Fill(in.p) ;
		  }

          }

          if ((nOKPhiCut > 0) && (m4k > 2.200) && (m4k < 2.220))
          {
                for (const auto &in : dEdxInfo_Ferenc)
		{
                	histosTH2F["hdedx4PHImass_Ferenc_peak_low"]->Fill(in.p, in.dEdx);
			histosTH1F["p_peak_low"]->Fill(in.p) ;
		}

          }


        if (nKaons == 4)
        {
          histosTH1F["hm4PHImass4curves"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass4curves"]->Fill(in.p, in.dEdx);
        }

        if (nKaons == 3)
        {
          histosTH1F["hm4PHImass3curves"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass3curves"]->Fill(in.p, in.dEdx);
        }

        if (nKaons >= 3)
        {
          histosTH1F["hm4PHImass34curves"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass34curves"]->Fill(in.p, in.dEdx);
        }

        if (nKaons >= 2)
        {
          histosTH1F["hm4PHImass234curves"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass234curves"]->Fill(in.p, in.dEdx);
        }

        if (nKaons >= 1)
        {
          histosTH1F["hm4PHImass1234curves"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass1234curves"]->Fill(in.p, in.dEdx);
        }

        if (twoKaonsOfTheSameSign)
        {
          histosTH1F["hm4PHImass2SS"]->Fill(m4k);
          histosTH1F["hm4PHImass2SS_below_th"]->Fill(m4k);

          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass2SS"]->Fill(in.p, in.dEdx);

          for (const auto &in : dEdxInfo_Ferenc)
            histosTH2F["hdedx4PHImass2SS_Ferenc"]->Fill(in.p, in.dEdx);
        }

	for(int ki = 0 ; ki < kaon_step_max ; ++ki)
	{
		stringstream ss ;

		ss << ki ;

		if(twoKaonsOfTheSameSignArray[ki]) 
		{
			string histogram_name = "hm4PHImass2SS_" + ss.str() ;
			histosTH1F[histogram_name]->Fill(m4k);
		}
	}

        if (twoKaonsOfTheSameSign_FS)
        {
          histosTH1F["hm4PHImass2SS_FS"]->Fill(m4k);

          for (const auto &in : dEdxInfo_Ferenc)
            histosTH2F["hdedx4PHImass2SS_Ferenc_FS"]->Fill(in.p, in.dEdx);
        }

        if (twoPionsOfTheSameSign)
        {
          histosTH1F["twoPionsOfTheSameSign"]->Fill(m4k);

          for (const auto &in : dEdxInfo_Ferenc)
            histosTH2F["hdedx4_twoPionsOfTheSameSign"]->Fill(in.p, in.dEdx);
        }

        if ((nPions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImassPionProtonVeto"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImassPionProtonVeto"]->Fill(in.p, in.dEdx);
        }

        if ((nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImassSafePionProtonVeto"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImassSafePionProtonVeto"]->Fill(in.p, in.dEdx);
        }

        if ((nKaons >= 1) && (nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImass1234curvesSafePionProtonVeto"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass1234curvesSafePionProtonVeto"]->Fill(in.p, in.dEdx);
        }

        if (twoKaonsOfTheSameSign && (nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImass2SSSafePionProtonVeto"]->Fill(m4k);
          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass2SSSafePionProtonVeto"]->Fill(in.p, in.dEdx);
        }
      }

      if (nOKPhiCutStrict > 0)
      {
			histosTH2F["hmALLkKen0_2D_strict_phi_cut_test"]->Fill(m11k, m22k);
			histosTH2F["hmALLkKen0_2D_strict_phi_cut_test"]->Fill(m12k, m21k);
		
		
        histosTH1F["hnKaons_PhiCutStrict"]->Fill(nKaons);
	
	for (const auto &in : dEdxInfo_Ferenc)
	{
		histosTH2F["hdedx4PHImass_strict_Ferenc"]->Fill(in.p, in.dEdx);
	}


        histosTH1F["hm4PHImass_PhiCutStrict"]->Fill(m4k);

        if (nKaons == 4) histosTH1F["hm4PHImass4curves_PhiCutStrict"]->Fill(m4k);
        if (nKaons == 3) histosTH1F["hm4PHImass3curves_PhiCutStrict"]->Fill(m4k);

        if (nKaons >= 3) histosTH1F["hm4PHImass34curves_PhiCutStrict"]->Fill(m4k);
        if (nKaons >= 2) histosTH1F["hm4PHImass234curves_PhiCutStrict"]->Fill(m4k);
        if (nKaons >= 1) histosTH1F["hm4PHImass1234curves_PhiCutStrict"]->Fill(m4k);

        if (twoKaonsOfTheSameSign)
        {
          histosTH1F["hm4PHImass2SS_PhiCutStrict"]->Fill(m4k);
        }

        if ((nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImassSafePionProtonVeto_PhiCutStrict"]->Fill(m4k);
        }

        if ((nKaons >= 1) && (nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImass1234curvesSafePionProtonVeto_PhiCutStrict"]->Fill(m4k);
        }

        if (twoKaonsOfTheSameSign && (nSafePions == 0) && (nProtons == 0))
        {
          histosTH1F["hm4PHImass2SSSafePionProtonVeto_PhiCutStrict"]->Fill(m4k);
        }
      }

      // begin: test code
      if ((nOKPhiCut > 0) && (m4k > 2.210) && (m4k < 2.230))
      {
          histosTH1F["h_m_4K_PEAK"]->Fill(m4k);
          histosTH1F["h_m_4pi_PEAK"]->Fill(mrec4);

          histosTH2F["hmALLpiKen0_2D_PEAK"]->Fill(m11, m22);
          histosTH2F["hmALLpiKen0_2D_PEAK"]->Fill(m12, m21);

          for (const auto &in : dEdxInfo)
            histosTH2F["hdedx4PHImass_PEAK"]->Fill(in.p, in.dEdx);
      }


      // end: test code

      //------------------------------------------
      // K*-K*

      double kstarCut = 0.042;

      TLorentzVector KpPI2Rec0(0.,0.,0.,0.);
      TLorentzVector KmPI2Rec0(0.,0.,0.,0.);
      TLorentzVector KPI4Rec0(0.,0.,0.,0.);

      double mKpPI0=0;
      double mKmPI0=0;
      double mKPI40=0;

      int nOKpidKs=0;

      bool passcurves = false;

      // 2combinations for phi phi, but 4 for KsKs
      //    pos1pos2  +K +pi
      //    neg1neg2  -K -pi

      KpPI2Rec0 =  k4pos1 + pi4neg2;
      KmPI2Rec0 = pi4pos2 +  k4neg1;
      mKpPI0 = KpPI2Rec0.M();
      mKmPI0 = KmPI2Rec0.M();

      KPI4Rec0 = KpPI2Rec0 + KmPI2Rec0;
      mKPI40 = KPI4Rec0.M();

      if(iskaon4pos[0] && ispion4neg[1] && ispion4pos[1] && iskaon4neg[0]) passcurves = true;

      if(fabs(mKpPI0-0.892)<kstarCut && fabs(mKmPI0-0.892)<kstarCut)  nOKpidKs++;

      KpPI2Rec0 =  k4pos1 + pi4neg1;
      KmPI2Rec0 = pi4pos2 +  k4neg2;
      mKpPI0 = KpPI2Rec0.M();
      mKmPI0 = KmPI2Rec0.M();

      KPI4Rec0 = KpPI2Rec0 + KmPI2Rec0;
      mKPI40 = KPI4Rec0.M();

      if(iskaon4pos[0] && ispion4neg[0] && ispion4pos[1] && iskaon4neg[1]) passcurves = true;

      if(fabs(mKpPI0-0.892)<kstarCut && fabs(mKmPI0-0.892)<kstarCut)  nOKpidKs++;

      KpPI2Rec0 =  k4pos2 + pi4neg1;
      KmPI2Rec0 = pi4pos1 +  k4neg2;
      mKpPI0 = KpPI2Rec0.M();
      mKmPI0 = KmPI2Rec0.M();

      KPI4Rec0 = KpPI2Rec0 + KmPI2Rec0;
      mKPI40 = KPI4Rec0.M();

      if(iskaon4pos[1] && ispion4neg[0] && ispion4pos[0] && iskaon4neg[1]) passcurves = true;

      if(fabs(mKpPI0-0.892)<kstarCut && fabs(mKmPI0-0.892)<kstarCut)  nOKpidKs++;

      KpPI2Rec0 =  k4pos2 + pi4neg2;
      KmPI2Rec0 = pi4pos1 +  k4neg1;
      mKpPI0 = KpPI2Rec0.M();
      mKmPI0 = KmPI2Rec0.M();

      KPI4Rec0=KpPI2Rec0+KmPI2Rec0;
      mKPI40=KPI4Rec0.M();

      if(iskaon4pos[1] && ispion4neg[1] && ispion4pos[0] && iskaon4neg[0]) passcurves = true;

      if(fabs(mKpPI0-0.892)<kstarCut && fabs(mKmPI0-0.892)<kstarCut)  nOKpidKs++;

      histosTH1F["hmALLkpi0"]->Fill(mKpPI0);
      if(fabs(mKpPI0-0.892)<kstarCut) histosTH1F["hmKSother0"]->Fill(mKmPI0);

      if (passcurves)
      {
        histosTH1F["hmALLkpi0curv"]->Fill(mKpPI0);
        if(fabs(mKpPI0-0.892)<kstarCut) histosTH1F["hmKSother0curv"]->Fill(mKmPI0);
      }

      if(nOKpidKs>0)
      {
        histosTH1F["hnOKpidKs"]->Fill(nOKpidKs);
        histosTH1F["hm4KST0"]->Fill(mKPI40);

        if(passcurves)
        {
          histosTH1F["hnOKpidKscurv"]->Fill(nOKpidKs);
          histosTH1F["hm4KST0curv"]->Fill(mKPI40);
        }
      }
      
      map_of_conditions_if_they_are_applied["if (allCuts4)"] = 0 ;

      map_of_conditions_if_they_are_applied["if (!etaCutSatisfied)"] = 0 ;      
      map_of_conditions_if_they_are_applied["if (!allTracksHavePixelHit)"] = 0 ;      

    }

    if (CTpycut4BKG)
    {
      histosTH1F["hm4BKG"]->Fill(mrec4);

      histosTH1F["hpt4BKG"]->Fill(pi4pos1.Pt());
      histosTH1F["hpt4BKG"]->Fill(pi4neg1.Pt());
      histosTH1F["hpt4BKG"]->Fill(pi4pos2.Pt());
      histosTH1F["hpt4BKG"]->Fill(pi4neg2.Pt());

      histosTH1F["heta4BKG"]->Fill(pi4pos1.Eta());
      histosTH1F["heta4BKG"]->Fill(pi4neg1.Eta());
      histosTH1F["heta4BKG"]->Fill(pi4pos2.Eta());
      histosTH1F["heta4BKG"]->Fill(pi4neg2.Eta());
    }
    
    map_of_conditions_if_they_are_applied["if (ntrk == 4 && totcharge == 0 && ntrkvtx == 4)"] = 0 ;
  }

  map_of_conditions_if_they_are_applied["isElastic"] = 0 ;        

  return returnStatus;
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(PromptAnalyzer);
