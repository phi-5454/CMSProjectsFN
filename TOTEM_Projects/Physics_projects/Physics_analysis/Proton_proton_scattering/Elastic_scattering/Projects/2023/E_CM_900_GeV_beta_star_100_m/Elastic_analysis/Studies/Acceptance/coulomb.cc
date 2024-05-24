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

#include "TMath.h"

#include "TF1.h"
#include "TH2D.h"
#include "TPave.h"
#include "TBox.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TRandom3.h"
#include "TEllipse.h"
#include "TMultiGraph.h"

#include <fstream>

#include <iomanip>

const double hbarc = 0.19732697 ;                            // [hbarc] = GeV * fm

const double Lambda2_GeV2 = 0.71 ;
const double Fine_structure_constant = 1/137.035 ;

const double beam_energy_GeV = 450.0 ;

const double theta_x_star_resolution_rad = 25.0e-6 ;
const double theta_y_star_resolution_rad =  5.4e-6 ;

double G_proton(double t)
{
  return 1 / pow(1-(-t/Lambda2_GeV2),2) ;
}

double Coulomb_amplitude(double t)
{
  return 2*sqrt(TMath::Pi())*(Fine_structure_constant)*(1/abs(t))*pow(G_proton(t),2) ;
}

double sigma(double *t, double *par)
{
  double ca = Coulomb_amplitude(t[0]) * pow(hbarc, 2) * 2*sqrt(TMath::Pi()); // to have the same dimension, normalization, as in BB
  double result = (10. * (pow(abs(ca),2) / pow(hbarc,2)))  / (4*TMath::Pi()) ;

  return result ;
}

const double epsilon = 1e-7 ;
TF1 *func = new TF1("sigma",  sigma, epsilon, 1, 0) ;

void randomdist()
{
  TCanvas c ;

  TH1F *distribution = new TH1F("distribution", "distribution", 1000, 0.0, 0.1) ;
  TH1F *distribution_pert = new TH1F("distribution_perts", "distribution_pert", 1000, 0.0, 0.1) ;

  TH2F *theta_x_y_star_rad_b1 = new TH2F("theta_x_y_star_rad_b1", "theta_x_y_star_rad_b1", 100, -1e-3, 1e-3, 100, -1e-3, 1e-3) ;
  TH2F *theta_x_y_star_pert_rad_b1 = new TH2F("theta_x_y_star_pert_rad_b1", "theta_x_y_star_pert_rad_b1", 100, -1e-3, 1e-3, 100, -1e-3, 1e-3) ;

  TRandom3 *myrandom = new TRandom3() ;

  c.SetLogy() ;

  for(int i = 0 ; i < 1e8 ; ++i)
  {
    double t_value_GeV2 = func->GetRandom() ;
    double phi_value_rad = myrandom->Uniform() * TMath::Pi() * 2.0 ;

    double theta_star_rad = sqrt(t_value_GeV2) / beam_energy_GeV ;

    double theta_x_star_rad_b1 = theta_star_rad * cos(phi_value_rad) ;
    double theta_y_star_rad_b1 = theta_star_rad * sin(phi_value_rad) ;

    double theta_x_star_rad_b2 = -theta_star_rad * cos(phi_value_rad) ;
    double theta_y_star_rad_b2 = -theta_star_rad * sin(phi_value_rad) ;
    
    double perturb = 1.0 ;

    double pert_x_rad_b1 = myrandom->Gaus() * theta_x_star_resolution_rad * perturb ;
    double pert_y_rad_b1 = myrandom->Gaus() * theta_y_star_resolution_rad * perturb ;

    double pert_x_rad_b2 = myrandom->Gaus() * theta_x_star_resolution_rad * perturb ;
    double pert_y_rad_b2 = myrandom->Gaus() * theta_y_star_resolution_rad * perturb ;

    double theta_x_star_pert_rad_b1 = theta_x_star_rad_b1 + pert_x_rad_b1 ;
    double theta_y_star_pert_rad_b1 = theta_y_star_rad_b1 + pert_y_rad_b1 ;

    double theta_x_star_pert_rad_b2 = theta_x_star_rad_b2 + pert_x_rad_b2 ;
    double theta_y_star_pert_rad_b2 = theta_y_star_rad_b2 + pert_y_rad_b2 ;
    
    if(theta_y_star_rad_b1 < 30e-6) continue ;
    
    theta_x_y_star_rad_b1->Fill(theta_x_star_rad_b1, theta_y_star_rad_b1) ;
    theta_x_y_star_pert_rad_b1->Fill(theta_x_star_pert_rad_b1, theta_y_star_pert_rad_b1) ;
    
    double theta_x_star_pert_rad = (theta_x_star_pert_rad_b1 - theta_x_star_pert_rad_b2) / 2.0 ;
    double theta_y_star_pert_rad = (theta_y_star_pert_rad_b1 - theta_y_star_pert_rad_b2) / 2.0 ;
    
    double theta_star_pert_rad = sqrt((theta_x_star_pert_rad * theta_x_star_pert_rad) + (theta_y_star_pert_rad * theta_y_star_pert_rad)) ;

    double p_GeV = (theta_star_pert_rad * beam_energy_GeV) ;
    double t_value_pert_GeV2 = p_GeV * p_GeV ;

    distribution->Fill(t_value_GeV2) ;
    distribution_pert->Fill(t_value_pert_GeV2) ;
  }

  distribution->SaveAs("plots/coulomb/distribution.root") ;
  distribution->Draw("") ;
  c.SaveAs("plots/coulomb/distribution.pdf") ;

  distribution_pert->SaveAs("plots/coulomb/distribution_pert.root") ;
  distribution_pert->Draw("") ;
  c.SaveAs("plots/coulomb/distribution_pert.pdf") ;

  c.SetLogy(0) ;

  distribution_pert->Divide(distribution) ;
  distribution_pert->SaveAs("plots/coulomb/distribution_pert_div.root") ;
  distribution_pert->Draw("") ;
  c.SaveAs("plots/coulomb/distribution_pert_div.pdf") ;
  
  c.SetLogz() ;  
  
  theta_x_y_star_rad_b1->Draw("colz") ;
  c.SaveAs("plots/coulomb/theta_x_y_star_rad_b1.pdf") ;

  theta_x_y_star_pert_rad_b1->Draw("colz") ;
  c.SaveAs("plots/coulomb/theta_x_y_star_pert_rad_b1.pdf") ;
}

int main(int argc, char *argv[])
{

  if(argc != 2) 
  {
    cout << "Please provide ..." << endl ;
    exit(1) ;
  }

  func->SetNpx(1e4) ;

  const double value_for_normalization  = func->Integral(4e-4, 7e-4) ;
  const double value_for_normalization2 = func->Eval(0.00056250000) ;
  const double value_for_normalization3 = func->Eval(0.0004375) ;

  cout << "value_for_normalization  " << value_for_normalization  << endl ;
  cout << "value_for_normalization2 " << value_for_normalization2 << endl ;
  cout << "value_for_normalization3 " << value_for_normalization3 << endl ;

  TCanvas c ;

  TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0, 1, 100, 0, 5) ;
  hist_2d->SetTitle("") ;
  hist_2d->Draw() ;

  func->Draw("same l") ;
  func->SaveAs("plots/coulomb/func_coulomb_test.root") ;
  c.SaveAs("plots/coulomb/canvas_coulomb_test.root") ;
  c.SaveAs("plots/coulomb/canvas_coulomb_test.pdf") ;

  randomdist() ;
}
