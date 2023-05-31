#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include "math.h"

using namespace std;

#include "stdlib.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMinuit.h"
#include "TStyle.h"
#include "TFitResult.h"

#include <iomanip>

const double Ly_near = 19.1 ;
const double Ly_far  = 17.4 ; 

const double Lx_near = 8.8228247 ;
const double Lx_far  = 5.8873862 ;

const double vy_near = 2.47 ;
const double vy_far  = 3.13 ;

const double vx_near = -3.3797 ;
const double vx_far  = -3.0494 ;

const double beam_size_XRPV_D6R5_B1_m = 9.336356565323156e-04 ;
const double beam_size_XRPV_B6R5_B1_m = 9.944974273364598e-04 ;

Double_t my_exponential_distribution(Double_t *x, Double_t *par)
{
      return par[0] * exp(par[1] * (x[0])) ;
}

Double_t theta_star_rad_from_t_GeV2(double t_GeV2, double beam_momentum_GeV)
{
        return asin(sqrt(-t_GeV2) / beam_momentum_GeV) ;
}

const double PI_TIMES_2 = (2.0 * TMath::Pi()) ;
const double beam_momentum_GeV = 450 ;

const double Constant = 2.363e7 ;
const double Slope = -14.0 ;
//const double Slope = -1.69642e+01 ;
// const double Slope = -1.69642 ;

// const string aperture_filename="/afs/cern.ch/eng/lhc/optics/V6.503/aperture/aperture.b1.madx" ;
const string aperture_filename="data/lhc/apertures_after_IP5_B1.txt" ;
const string optics_file_name = "/afs/cern.ch/work/f/fnemes/main_workspace_git/projects/LHCRealOpticsTools/OpticsProject/Project_files/2018/E_CM_900_GeV/Valentina/twiss_beam1_900_beta11_thetay0.tfs" ;

class TAperture
{ 
  public:

  string name ;
  TH2D *histogram ;
  TH2D *histogram_vertex ;
  TH1D *histogram_t ;
  TH1D *histogram_phi ;

  double rectangle_half_width = 0 ;
  double rectangle_half_height = 0 ;
  double ellipse_semi_axis_hor = 0 ;
  double ellipse_semi_axis_ver = 0 ;
  
  double Lx, Ly, vx, vy ;
  
  TAperture(string name, double Lx, double Ly, double vx, double vy, double rectangle_half_width, double rectangle_half_height, double ellipse_semi_axis_hor, double ellipse_semi_axis_ver):
  name(name), Lx(Lx), Ly(Ly), vx(vx), vy(vy), rectangle_half_width(rectangle_half_width), rectangle_half_height(rectangle_half_height), ellipse_semi_axis_hor(ellipse_semi_axis_hor), ellipse_semi_axis_ver(ellipse_semi_axis_ver)
  {
    histogram = new TH2D(name.c_str(), name.c_str(), 100, -1000e-6, 1000e-6, 100, -1000e-6, 1000e-6) ;
    histogram_vertex = new TH2D((name + "_vertex").c_str(), (name + "_vertex").c_str(), 100, -1000e-6, 1000e-6, 100, -1000e-6, 1000e-6) ;
    histogram_t = new TH1D((name + "_t").c_str(), (name + "_t").c_str(), 1000, 0, 10) ;
    histogram_phi = new TH1D((name + "_phi").c_str(), (name + "_phi").c_str(), 100, 0, 10) ;
  }
  
  
} ;

TF1 *t_GeV2_distribution = NULL ;
TRandom3 myrand ;

class TProton
{
  public:

  double theta_x_star ;
  double theta_y_star ;
  double x_star ;
  double y_star ;

  double theta_x_star_pert ;
  double theta_y_star_pert ;

  double theta_x_star_reco ;
  double theta_y_star_reco ;
  double x_star_reco ;
  double y_star_reco ;

  TProton() {} ;
  void Reconstruct() ;
} ;


void TProton::Reconstruct()
{
  double x_near = (Lx_near * theta_x_star_pert) + (vx_near * x_star) ;
  double x_far  = (Lx_far *  theta_x_star_pert) + (vx_far *  x_star) ;
  double y_near = (Ly_near * theta_y_star_pert) + (vy_near * y_star) ;
  double y_far  = (Ly_far *  theta_y_star_pert) + (vy_far *  y_star) ;

  double determinant_x = ((Lx_near * vx_far) - (Lx_far * vx_near)) ;
  double determinant_y = ((Ly_near * vy_far) - (Ly_far * vy_near)) ;

  theta_x_star_reco = ((x_near * vx_far) - (x_far * vx_near)) / determinant_x ;
  x_star_reco =      -((x_near * Lx_far) - (x_far * Lx_near)) / determinant_x ;

  theta_y_star_reco = ((y_near * vy_far) - (y_far * vy_near)) / determinant_y ;
  y_star_reco =      -((y_near * Ly_far) - (y_far * Ly_near)) / determinant_y ;

  double theta_star_reco = sqrt((theta_x_star_reco*theta_x_star_reco) + (theta_y_star_reco*theta_y_star_reco)) ;
  double minus_t_GeV2_reco = - beam_momentum_GeV * beam_momentum_GeV * theta_star_reco * theta_star_reco ;
}

class TProtonPair
{
  public:

  double minus_t_GeV2 ;
  Double_t phi_IP5_rad ;    

  double theta_star_rad ;

  TProton p_b1, p_b2 ;

  double beam_divergence_x_b1 ;
  double beam_divergence_y_b1 ;

  double beam_divergence_x_b2 ;
  double beam_divergence_y_b2 ;
  
  bool reconstructed ;

  double theta_x_star_reco ;
  double theta_y_star_reco ;
  double x_star_reco ;
  double y_star_reco ;

  TProtonPair() ;
  void GenerateParticles() ;
  bool TestApertures(vector<TAperture *> &) ;
  bool TestAperturesOneProton(TProton, vector<TAperture *> &, bool) ;
} ;

const double vertex_size_m = 200e-6 ;

const double beam_divergence_x_rad = 50e-6 ;
const double beam_divergence_y_rad = 27e-6 ;

TProtonPair::TProtonPair()
{
  GenerateParticles() ;
}

void TProtonPair::GenerateParticles()
{
  reconstructed = false ;

  minus_t_GeV2 = t_GeV2_distribution->GetRandom() ;
  phi_IP5_rad = gRandom->Uniform(0, PI_TIMES_2) ;    

  theta_star_rad = theta_star_rad_from_t_GeV2(-minus_t_GeV2, beam_momentum_GeV) ;

  p_b1.theta_x_star =  (theta_star_rad * cos(phi_IP5_rad)) ;
  p_b1.theta_y_star =  (theta_star_rad * sin(phi_IP5_rad)) ;

  p_b2.theta_x_star = -(theta_star_rad * cos(phi_IP5_rad)) ;
  p_b2.theta_y_star = -(theta_star_rad * sin(phi_IP5_rad)) ;

  p_b1.x_star = 1.0 * vertex_size_m * myrand.Gaus() ;
  p_b1.y_star = 1.0 * vertex_size_m * myrand.Gaus() ;

  p_b2.x_star = p_b1.x_star ;
  p_b2.y_star = p_b1.y_star ;
  
  double factor = 1.0 ;

  beam_divergence_x_b1 = factor * beam_divergence_x_rad * myrand.Gaus() ;
  beam_divergence_y_b1 = factor * beam_divergence_y_rad * myrand.Gaus() ;

  beam_divergence_x_b2 = factor * beam_divergence_x_rad * myrand.Gaus() ;
  beam_divergence_y_b2 = factor * beam_divergence_y_rad * myrand.Gaus() ;

  p_b1.theta_x_star_pert = p_b1.theta_x_star + beam_divergence_x_b1 ;
  p_b1.theta_y_star_pert = p_b1.theta_y_star + beam_divergence_y_b1 ;

  p_b2.theta_x_star_pert = p_b2.theta_x_star + beam_divergence_x_b2 ;
  p_b2.theta_y_star_pert = p_b2.theta_y_star + beam_divergence_y_b2 ;
  
  theta_x_star_reco = 0 ;
  theta_y_star_reco = 0 ;
  x_star_reco = 0 ;
  y_star_reco = 0 ;
  
}

bool TProtonPair::TestApertures(vector<TAperture *> &vector_apertures) 
{
  bool test_p_b1 = TestAperturesOneProton(p_b1, vector_apertures, true) ;
  bool test_p_b2 = TestAperturesOneProton(p_b2, vector_apertures, false) ;
  
  if(test_p_b1 && test_p_b2)
  {
    p_b1.Reconstruct() ;
    p_b2.Reconstruct() ;

    theta_x_star_reco = (p_b1.theta_x_star_reco - p_b2.theta_x_star_reco) / 2.0 ;
    theta_y_star_reco = (p_b1.theta_y_star_reco - p_b2.theta_y_star_reco) / 2.0 ;

    x_star_reco = (p_b1.x_star_reco + p_b2.x_star_reco) / 2.0 ;
    y_star_reco = (p_b1.y_star_reco + p_b2.y_star_reco) / 2.0 ;
    
    reconstructed = true ;
  }

}

bool TProtonPair::TestAperturesOneProton(TProton proton, vector<TAperture *> &vector_apertures, bool save_result) 
{
  bool test = true ;

  for(int j = 0 ; j < vector_apertures.size() ; ++j)
  {
    double x_pos_meter = ((vector_apertures[j]->Lx * proton.theta_x_star_pert) + (vector_apertures[j]->vx * proton.x_star)) ;
    double y_pos_meter = ((vector_apertures[j]->Ly * proton.theta_y_star_pert) + (vector_apertures[j]->vy * proton.y_star)) ;

    if((fabs(x_pos_meter) < vector_apertures[j]->rectangle_half_width) &&
       (fabs(y_pos_meter) < vector_apertures[j]->rectangle_half_height)  )
    {
      // ((x/A)^2) + ((y/B)^2) == 1

      double x_scaled =  (x_pos_meter / vector_apertures[j]->ellipse_semi_axis_hor) ;

      double y_scaled = sqrt(1 - (x_scaled*x_scaled)) * vector_apertures[j]->ellipse_semi_axis_ver ;

      if(fabs(y_pos_meter) < y_scaled)
      {
        if(save_result)
        {
          vector_apertures[j]->histogram->Fill(proton.theta_x_star_pert, proton.theta_y_star_pert) ;
          vector_apertures[j]->histogram_vertex->Fill(proton.x_star, proton.y_star) ;
          vector_apertures[j]->histogram_t->Fill(minus_t_GeV2) ;
          vector_apertures[j]->histogram_phi->Fill(phi_IP5_rad) ;
        }
      }
      else
      {
        test = false ;
      }
    }
    else
    {
      test = false ;
    }
  }
  
  return test ;
}

void test_aperture(vector<TAperture *> &vector_apertures)
{


  t_GeV2_distribution->SetParameters(Constant, Slope) ;
  t_GeV2_distribution->SetNpx(100000) ;
  
  TH1D *histogram_t = new TH1D("histogram_t", "histogram_t", 1000, 0, 10) ;
  TH1D *histogram_phi = new TH1D("histogram_phi", "histogram_phi", 100, 0, 10) ;

  TH2D *histogram_theta_x_star_rad = new TH2D("histogram_theta_x_star_rad", "histogram_theta_x_star_rad", 100, -1400e-6, 1400e-6,  100, -1400e-6, 1400e-6) ;
  TH2D *histogram_theta_x_y_star_rad = new TH2D("histogram_theta_x_y_star_rad", "histogram_theta_x_y_star_rad", 100, -1400e-6, 1400e-6,  100, -1400e-6, 1400e-6) ;


	for(int i = 0 ; i < 1e6 ; ++i)
	{
    TProtonPair pp ;
   
    histogram_t->Fill(pp.minus_t_GeV2) ;
    histogram_phi->Fill(pp.phi_IP5_rad) ;
  
    pp.TestApertures(vector_apertures) ;
    
    if(pp.reconstructed)
    {
      histogram_theta_x_star_rad->Fill(pp.p_b1.theta_x_star, pp.theta_x_star_reco) ;
      histogram_theta_x_y_star_rad->Fill(pp.p_b1.theta_x_star_reco, pp.theta_y_star_reco) ;
    }
    
  }

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);

  TCanvas c ;
  
  for(int j = 0 ; j < vector_apertures.size() ; ++j)
  {
    vector_apertures[j]->histogram->GetXaxis()->SetTitle("#theta_{x}* (rad)") ;
    vector_apertures[j]->histogram->GetYaxis()->SetTitle("#theta_{y}* (rad)") ;

    vector_apertures[j]->histogram->Draw("colz") ;
    c.SaveAs(("plots/apeture_test/aperture_test_" + vector_apertures[j]->name + ".root").c_str()) ;

    vector_apertures[j]->histogram_vertex->Draw("colz") ;
    c.SaveAs(("plots/apeture_test/aperture_test_" + vector_apertures[j]->name + "_vertex.root").c_str()) ;

    vector_apertures[j]->histogram_t->Draw("") ;
    c.SaveAs(("plots/apeture_test/aperture_test_" + vector_apertures[j]->name + "_t.root").c_str()) ;

    vector_apertures[j]->histogram_phi->Draw("") ;
    c.SaveAs(("plots/apeture_test/aperture_test_" + vector_apertures[j]->name + "_phi.root").c_str()) ;
    
    cout << "statistics " << vector_apertures[j]->name << " " << vector_apertures[j]->histogram->GetEntries() << endl ;
  }
  
  histogram_t->SaveAs("histogram_t.root") ;
  histogram_phi->SaveAs("histogram_phi.root") ;
  histogram_theta_x_star_rad->SaveAs("histogram_theta_x_star_rad.root") ;
  histogram_theta_x_y_star_rad->SaveAs("histogram_theta_x_y_star_rad.root") ;
  
  c.cd() ;
  
  histogram_t->Draw("") ;

  for(int j = 0 ; j < vector_apertures.size() ; ++j)
  {
    vector_apertures[j]->histogram_t->Draw("same") ;
  }
  
  c.SaveAs("histogram_t_combined.root") ;
}

void read_apertures()
{

  ifstream apertures(aperture_filename.c_str()) ;
  vector<TAperture *> vector_apertures ;
  
  string name ;
  double rectangle_half_width = 0 ;
  double rectangle_half_height = 0 ;
  double ellipse_semi_axis_hor = 0 ;
  double ellipse_semi_axis_ver = 0 ;
  
  const int RE12_position = 13 ;
  const int RE34_position = 23 ;

  const double LARGE_VALUE = 1e99 ;

  double theta_x_star_limit_due_rectangle_half_width_min = LARGE_VALUE ;
  double theta_x_star_limit_due_ellipse_semi_axis_hor_min = LARGE_VALUE ;

  double theta_y_star_limit_due_rectangle_half_height_min = LARGE_VALUE ;
  double theta_y_star_limit_due_ellipse_semi_axis_ver_min = LARGE_VALUE ;

  string theta_x_star_limit_due_rectangle_half_width_min_name = "" ;
  string theta_x_star_limit_due_ellipse_semi_axis_hor_min_name = "" ;

  string theta_y_star_limit_due_rectangle_half_height_min_name = "" ;
  string theta_y_star_limit_due_ellipse_semi_axis_ver_min_name = "" ;
  
  while(apertures >> name >> rectangle_half_width >> rectangle_half_height >>  ellipse_semi_axis_hor >> ellipse_semi_axis_ver)
  {
    // cout << name << endl ;
    
    ifstream optics_file(optics_file_name.c_str()) ;
    
    const string magnet_name = "\"" + name + "\"" ;
    string word ;
    
    bool found = false ;
    
    while(optics_file >> word)
    {
      if(word.compare(magnet_name) == 0)
      {
        found = true ;
        
        double Lx = 0 ;
        double Ly = 0 ;

        double vx = 0 ;
        double vy = 0 ;

        double value = 0 ;
        
        for(int i = 0 ; i < 24 ; ++i)
        {
          optics_file >> value ;
          // cout << value << " " ;

          if(i == (RE12_position - 1)) Lx = value ;
          if(i == (RE34_position - 1)) Ly = value ;

          if(i == (RE12_position - 2)) vx = value ;
          if(i == (RE34_position - 2)) vy = value ;
        }
        
        // cout << name << "     " << Lx  << " " << Ly << endl ;
        
        TAperture *aperture = new TAperture(name, Lx, Ly, vx, vy, rectangle_half_width, rectangle_half_height,  ellipse_semi_axis_hor, ellipse_semi_axis_ver) ;
        vector_apertures.push_back(aperture) ;
        
        double theta_x_star_limit_due_rectangle_half_width = (rectangle_half_width / Lx) ;
        double theta_x_star_limit_due_ellipse_semi_axis_hor = (ellipse_semi_axis_hor / Lx) ;

        double theta_y_star_limit_due_rectangle_half_height = (rectangle_half_height / Ly) ;
        double theta_y_star_limit_due_ellipse_semi_axis_ver = (ellipse_semi_axis_ver / Ly) ;

        cout << name << " theta_x_star_limit_due_rectangle_half_width_min_name:  " << theta_x_star_limit_due_rectangle_half_width_min << endl ;
        cout << name << " theta_x_star_limit_due_ellipse_semi_axis_hor_min_name: " << theta_x_star_limit_due_ellipse_semi_axis_hor_min << endl ;

        cout << name << " theta_y_star_limit_due_rectangle_half_height_min_name: " << theta_y_star_limit_due_rectangle_half_height_min << endl ;
        cout << name << " theta_y_star_limit_due_ellipse_semi_axis_ver_min_name: " << theta_y_star_limit_due_ellipse_semi_axis_ver_min << endl << endl ;
        
        if(theta_x_star_limit_due_rectangle_half_width_min > theta_x_star_limit_due_rectangle_half_width)   { theta_x_star_limit_due_rectangle_half_width_min_name = name ; theta_x_star_limit_due_rectangle_half_width_min = theta_x_star_limit_due_rectangle_half_width ; } ;
        if(theta_x_star_limit_due_ellipse_semi_axis_hor_min > theta_x_star_limit_due_ellipse_semi_axis_hor) { theta_x_star_limit_due_ellipse_semi_axis_hor_min_name = name ;theta_x_star_limit_due_ellipse_semi_axis_hor_min = theta_x_star_limit_due_ellipse_semi_axis_hor ; } ;

        if(theta_y_star_limit_due_rectangle_half_height_min > theta_y_star_limit_due_rectangle_half_height) { theta_y_star_limit_due_rectangle_half_height_min_name = name ; theta_y_star_limit_due_rectangle_half_height_min = theta_y_star_limit_due_rectangle_half_height ; } ;
        if(theta_y_star_limit_due_ellipse_semi_axis_ver_min > theta_y_star_limit_due_ellipse_semi_axis_ver) { theta_y_star_limit_due_ellipse_semi_axis_ver_min_name = name ; theta_y_star_limit_due_ellipse_semi_axis_ver_min = theta_y_star_limit_due_ellipse_semi_axis_ver ; } ;
        
        // test_aperture(name, Lx, Ly, rectangle_half_width, rectangle_half_height,  ellipse_semi_axis_hor, ellipse_semi_axis_ver) ;
      }
    }
    
    optics_file.close() ;
    
    if(!found)
    {
      cout << name << " not found" << endl ;
    }
  }
  
  cout << endl ;
  cout << "#################" << endl ;  
  cout << endl ;

  cout << "theta_x_star_limit_due_rectangle_half_width_min_name:  " <<  theta_x_star_limit_due_rectangle_half_width_min_name  << theta_x_star_limit_due_rectangle_half_width_min << endl ;
  cout << "theta_x_star_limit_due_ellipse_semi_axis_hor_min_name: " <<  theta_x_star_limit_due_ellipse_semi_axis_hor_min_name << theta_x_star_limit_due_ellipse_semi_axis_hor_min << endl ;
  
  cout << "theta_y_star_limit_due_rectangle_half_height_min_name: " <<  theta_y_star_limit_due_rectangle_half_height_min_name << theta_y_star_limit_due_rectangle_half_height_min << endl ;
  cout << "theta_y_star_limit_due_ellipse_semi_axis_ver_min_name: " <<  theta_y_star_limit_due_ellipse_semi_axis_ver_min_name << theta_y_star_limit_due_ellipse_semi_axis_ver_min << endl ;

  test_aperture(vector_apertures) ;
}

int main()
{

  t_GeV2_distribution = new TF1("t_GeV2_distribution", my_exponential_distribution, 0.0, 7.0, 2) ;

  // gStyle->SetOptStat(0) ;
  // gStyle->SetOptFit(1111) ;

	TF1 *t_GeV2_distribution = new TF1("t_GeV2_distribution", my_exponential_distribution, 0.0, 7.0, 2) ;
  t_GeV2_distribution->SetParameters(Constant, Slope) ;
  t_GeV2_distribution->SetNpx(100000) ;
  
//   TH1D *hist_minus_t_GeV2 = new TH1D("hist_minus_t_GeV2", "hist_minus_t_GeV2", 100, 0, 2) ;
//   TH1D *hist_minus_t_GeV2_reco = new TH1D("hist_minus_t_GeV2_reco", "hist_minus_t_GeV2_reco", 100, 0, 2) ;
//   TH1D *hist_minus_t_GeV2_reco_diff_rel_percent = new TH1D("hist_minus_t_GeV2_reco_diff_rel_percent", "hist_minus_t_GeV2_reco_diff_rel_percent", 100, 1e-2, -1e-2) ;

  TH1D *hist_minus_t_GeV2 = new TH1D("hist_minus_t_GeV2", "hist_minus_t_GeV2", 100, 0, 2) ;
  TH1D *hist_minus_t_GeV2_reco = new TH1D("hist_minus_t_GeV2_reco", "hist_minus_t_GeV2_reco", 100, 0, 2) ;

  TH1D *hist_minus_t_GeV2_reco_diff_rel_percent = new TH1D("hist_minus_t_GeV2_reco_diff_rel_percent", "hist_minus_t_GeV2_reco_diff_rel_percent", 100, 1e-2, -1e-2) ;

	TH2D *hist = new TH2D("hist", "hist", 100, -5e+2, 5e+2, 100, -1000, 1000) ;

	TH2D *hist_compare_theta_y_star = new TH2D("hist_compare_theta_y_star", "hist_compare_theta_y_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;
	TH2D *hist_compare_y_star = new TH2D("hist_compare_y_star", "hist_compare_y_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;

	TH2D *hist_compare_theta_x_star = new TH2D("hist_compare_theta_x_star", "hist_compare_theta_x_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;
	TH2D *hist_compare_x_star = new TH2D("hist_compare_x_star", "hist_compare_x_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;

	TH2D *hist_theta_x_y_star_reco = new TH2D("hist_theta_x_y_star_reco", "hist_theta_x_y_star_reco", 1000, -16.0e-4, 16.0e-4,  1000, -16.0e-4, 16.0e-4) ;

	// Best effect
	const double delta_n = -2.0 * 0.0 ;
	const double delta_f = -3.0 * 0.0 ;

	// 

  const double Lx_near_reco = Lx_near + delta_n ;
  const double Lx_far_reco = Lx_far   + delta_f ;

  const double Ly_near_reco = Ly_near + delta_n ;
  const double Ly_far_reco = Ly_far   + delta_f ;

  read_apertures() ;
  
  exit(1) ;

	for(int i = 0 ; i < 1e6 ; ++i)
	{

    const double minus_t_GeV2 = t_GeV2_distribution->GetRandom() ;
    const Double_t phi_IP5_rad = gRandom->Uniform(0, PI_TIMES_2) ;    
  
    double theta_star_rad = theta_star_rad_from_t_GeV2(-minus_t_GeV2, beam_momentum_GeV) ;

    double theta_x_star = (theta_star_rad * cos(phi_IP5_rad)) ;
    double theta_y_star = (theta_star_rad * sin(phi_IP5_rad)) ;
  
	  double y_star = 1.0 * 200e-6 * myrand.Gaus() ;
	  double x_star = 1.0 * 200e-6 * myrand.Gaus() ;

		double beam_divergence_x = 0.0 * 20e-6 * myrand.Gaus() ;
		double beam_divergence_y = 0.0 * 20e-6 * myrand.Gaus() ;
		
		double theta_x_star_pert = theta_x_star + beam_divergence_x ;
		double theta_y_star_pert = theta_y_star + beam_divergence_y ;

		double x_near = (Lx_near * theta_x_star_pert) + (vx_near * x_star) ;
		double x_far =  (Lx_far *  theta_x_star_pert) + (vx_far *  x_star) ;
		double y_near = (Ly_near * theta_y_star_pert) + (vy_near * y_star) ;
		double y_far =  (Ly_far *  theta_y_star_pert) + (vy_far *  y_star) ;

		// cout << y_far << endl ;
//		if(fabs(x_far)< 3e-3)
//		if(fabs(x_near)< 3e-3)

		// if((y_far < -1.e-3) && (y_far > -10.0e-3) )
  	//	if(fabs(x_near) < 3e-3)
  	//	if(fabs(x_far) < 3e-3)
  	//	if((y_far < -4.0e-3) && (y_far > -10.0e-3) && (y_near < (-4.0e-3*1.4*(Ly_far/Ly_near))) && (y_near > (-10.0e-3*(Ly_far/Ly_near))))
		{

      double determinant_x = ((Lx_near_reco * vx_far) - (Lx_far_reco * vx_near)) ;
      double determinant_y = ((Ly_near_reco * vy_far) - (Ly_far_reco * vy_near)) ;

			double theta_y_star_reco = ((y_near * vy_far) - (y_far * vy_near)) / determinant_y ;
			// double theta_y_star_reco = ((y_near / Ly_near_reco) + (y_far / Ly_far_reco)) / 2.0 ;
			double y_star_reco =      -((y_near * Ly_far_reco) - (y_far * Ly_near_reco)) / determinant_y ;

			double theta_x_star_reco = ((x_near * vx_far) - (x_far * vx_near)) / determinant_x ;
			double x_star_reco =      -((x_near * Lx_far_reco) - (x_far * Lx_near_reco)) / determinant_x ;
      
      double theta_star_reco = sqrt((theta_x_star_reco*theta_x_star_reco) + (theta_y_star_reco*theta_y_star_reco)) ;
      double minus_t_GeV2_reco = - beam_momentum_GeV * beam_momentum_GeV * theta_star_reco * theta_star_reco ;
      
      hist_minus_t_GeV2->Fill(minus_t_GeV2) ;
      hist_minus_t_GeV2_reco->Fill(-minus_t_GeV2_reco) ;
      
      hist_minus_t_GeV2_reco_diff_rel_percent->Fill(((minus_t_GeV2 + minus_t_GeV2_reco) / minus_t_GeV2) * 100.0) ;

			hist_compare_theta_x_star->Fill(theta_x_star_pert, theta_x_star_reco) ;
			hist_compare_x_star->Fill(x_star, x_star_reco) ;

			hist_compare_theta_y_star->Fill(theta_y_star_pert, theta_y_star_reco) ;
			hist_compare_y_star->Fill(y_star, y_star_reco) ;
      
      hist_theta_x_y_star_reco->Fill(theta_x_star_reco, theta_y_star_reco) ;

			hist->Fill(theta_y_star_reco * 1e6, y_star_reco * 1e6) ;
		}

	}
	
	gStyle->SetPalette(1) ;

	TCanvas c ;
	
  gStyle->SetTitleFillColor(kWhite) ;
  gStyle->SetStatColor(kWhite) ;
  gStyle->SetFillColor(kWhite) ;

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	
	hist->SetFillColor(kWhite) ;
	hist->SetTitle("") ;

	hist->Draw("colz") ;
	
	hist->GetXaxis()->SetTitle("#theta_{x,reco}* [#murad]") ;
	hist->GetYaxis()->SetTitle("x_{reco}* [#mum]") ;
	hist->GetYaxis()->SetTitleOffset(1.2) ;

	TLatex *latex = new TLatex() ;
	latex->SetTextFont(132) ;
	latex->SetNDC();
	latex->DrawText(0.24, 0.92, "simulation preliminary") ;

	latex->SetTextFont(22) ;
	latex->DrawText(0.12, 0.92, "TOTEM") ;
	latex->Draw("same") ;
	
	
	c.SetFillColor(kWhite) ;
	
//	gStyle->SetOptStat(0);

  c.SetGridx() ;
  c.SetGridy() ;	
	
	hist->GetXaxis()->SetAxisColor(17);
	hist->GetYaxis()->SetAxisColor(17);	

	c.SetLogz() ;
	c.SaveAs("plots/hist.pdf") ;
	
	hist_compare_theta_x_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_theta_x_star.pdf") ;

	hist_compare_x_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_x_star.pdf") ;

	hist_compare_theta_y_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_theta_y_star.pdf") ;

	hist_compare_y_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_y_star.pdf") ;

  hist_theta_x_y_star_reco->Draw("colz") ;
	c.SaveAs("plots/hist_theta_x_y_star_reco.pdf") ;
	c.SaveAs("plots/hist_theta_x_y_star_reco.root") ;

  // cout >> hist_minus_t_GeV2->Integral() ;
  hist_minus_t_GeV2->Draw("colz") ;
  hist_minus_t_GeV2->GetYaxis()->SetRangeUser(0.1, 1e7) ;
  c.SetLogy() ;
	c.SaveAs("plots/hist_minus_t_GeV2.pdf") ;

  hist_minus_t_GeV2_reco->Draw("colz") ;
  hist_minus_t_GeV2_reco->GetYaxis()->SetRangeUser(0.1, 1e7) ;
  c.SetLogy() ;
	c.SaveAs("plots/hist_minus_t_GeV2_reco.pdf") ;
  
  hist_minus_t_GeV2_reco_diff_rel_percent->Draw("colz") ;
  c.SetLogy(0) ;
	c.SaveAs("plots/hist_minus_t_GeV2_reco_diff_rel_percent.pdf") ;
  
}	
