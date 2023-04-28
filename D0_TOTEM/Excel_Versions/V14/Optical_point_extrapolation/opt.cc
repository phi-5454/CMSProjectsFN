
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
#include "TGraph.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TRandom3.h"
#include "TMultiGraph.h"

#include <iomanip>

const int unit_scenario_in_GeV = 1 ;
const int unit_scenario_in_TeV = 2 ;
const int unit_scenario_in_10GeV = 3 ;

const int standard_uncertainties = 1 ;
const int normalized_to_chi2_1 = 2 ;
const int normalized_to_p_value_0p5 = 3 ;
const int normalized_to_chi2_1_in_GeV = 4 ;

const int fit_scenario_standard = 1 ;
const int fit_scenario_test_concavity = 2 ;
const int fit_scenario_constraint_from_low_energy = 3 ;

int scenario = standard_uncertainties ;
int unit_scenario = unit_scenario_in_10GeV ;

// int fit_scenario = fit_scenario_test_concavity ;
int fit_scenario = fit_scenario_constraint_from_low_energy ;

Double_t special_fit_function(Double_t *x, Double_t *par)
{
  double a = par[0] ;
  double b = par[1] ;
  double c = par[2] ;

  double sig_tot = x[0] ;

  double log_sig_tot = log(sig_tot) ;
  double final = (a * log_sig_tot * log_sig_tot) + (b * log_sig_tot) + c ;

  return final ;
}

TGraphErrors *graph = new TGraphErrors() ;
TGraphErrors *graph2 = NULL ;

double save_chi2 = 0 ;

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{

  double chi2 = 0 ;

  int number_of_points_in_graph = graph2->GetN() ;

  for(int i = 0 ; i < number_of_points_in_graph ; ++i)
  {
    double sqrts_value = 0 ;
    double dsdt = 0 ;
    double uncertainty_y = 0 ;

    graph2->GetPoint(i, sqrts_value, dsdt) ;
    uncertainty_y = graph2->GetErrorY(i) ;

    double value = special_fit_function(&sqrts_value, par) ;

    double delta = ((value - dsdt) / uncertainty_y) ;
    chi2 += delta*delta ;
  }

  // cout << chi2 << endl ;
  
  save_chi2 = chi2 ;

  f = chi2 ; 
}

TF1 *func = NULL ;

TH1D *hista = new TH1D("hista", "hista", 100, 70, 68) ;
TH1D *histb = new TH1D("histb", "histb", 100, 70, 68) ;
TH1D *histc = new TH1D("histc", "histc", 100, 70, 68) ;

TH1D *chi2_hist = new TH1D("chi2_hist", "chi2_hist", 100, 0, 10) ;
TH1D *p_value_hist = new TH1D("p_value_hist", "p_value_hist", 100, 0, 2) ;

TH1D *hist_der = new TH1D("hist_der", "hist_der", 100, 70, 68) ;

TVector par_vec(7) ;

double energy_factor = 1.0 ;
int number_of_free_parameters = 3 ;

Int_t MinuitFit()
{

  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetPrintLevel(1) ;  
//   gMinuit2->SetPrintLevel(-1) ;  
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);
  
  double c_start = 80 ;
  
  if(fit_scenario == fit_scenario_constraint_from_low_energy)
  {
    c_start = 38 ;
  }

  gMinuit2->mnparm(0, "a",    3, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(1, "b",    10, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(2, "c",    c_start, 0.1, 0, 0, ierflg);

  arglist[0] = 1000000 ;
  arglist[1] = 0.01 ;
  arglist[2] = 1 ;

  if(fit_scenario == fit_scenario_constraint_from_low_energy)
  {
    gMinuit2->FixParameter(2) ;
    number_of_free_parameters = 2 ;
  }

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
  
  double par[7] ;
  double pare[7] ;

  gMinuit2->GetParameter(0, par[0], pare[0]) ;
  gMinuit2->GetParameter(1, par[1], pare[1]) ;
  gMinuit2->GetParameter(2, par[2], pare[2]) ;


  par_vec(0) = par[0] ;
  par_vec(1) = par[1] ;
  par_vec(2) = par[2] ;

  func = new TF1("func", special_fit_function, 0.0001, 14.0 * energy_factor, 3) ;  
  func->SetNpx(1000) ;

  func->SetParameters(par_vec(0), par_vec(1), par_vec(2))  ;

  hista->Fill(par_vec(0)) ;
  histb->Fill(par_vec(1)) ;
  histc->Fill(par_vec(2)) ;
  
  return ierflg ;
}

int main(int argc, char *argv[])
{

  bool perturb = true ;

  bool test = true ;
  
  if(!perturb) test = true ;
  
  string postfix = "TeV" ;

  if(unit_scenario == unit_scenario_in_TeV)
  {
  }
  else if(unit_scenario == unit_scenario_in_GeV)
  {
    energy_factor = 1000.0 ;
    postfix = "GeV" ;    
  }
  else if(unit_scenario == unit_scenario_in_10GeV)
  {
    energy_factor = 100.0 ;
    postfix = "10GeV" ;    
  }
  else
  {
    cout << "Unknown scenario" << endl ;
    exit(1) ;
  }

  // gStyle->SetOptStat(0) ;  
  gStyle->SetOptFit(1111) ;

  int number_of_point = 0   ;

    // graph->SetPoint(number_of_point++, 0.0625, 43.32) ;

/*
  graph->SetPoint(number_of_point++, 2.76,   84.7) ;
  graph->SetPoint(number_of_point++, 7.00,   98.3) ;
  graph->SetPoint(number_of_point++, 8.00,  101.5) ;
  graph->SetPoint(number_of_point++, 13.00, 110.3) ;
  */

  graph->SetPoint(number_of_point++, 10.2463e-3 * energy_factor, 38.43) ;
  graph->SetPoint(number_of_point++, 16.83e-3 * energy_factor,   38.62) ;

  graph->SetPoint(number_of_point++, 2.76 * energy_factor,   84.7) ;
  graph->SetPoint(number_of_point++, 7.00 * energy_factor,   98.3) ;
  graph->SetPoint(number_of_point++, 8.00 * energy_factor,  101.5) ;
  graph->SetPoint(number_of_point++, 13.0 * energy_factor,  110.3) ;
  
  number_of_point = 0 ;  
  
  if(scenario == standard_uncertainties)
  {
    graph->SetPointError(number_of_point++, 0, 0.2) ;
    graph->SetPointError(number_of_point++, 0, 0.2) ;

    graph->SetPointError(number_of_point++, 0, 3.3) ;
    graph->SetPointError(number_of_point++, 0, 2.8) ;
    graph->SetPointError(number_of_point++, 0, 2.1) ;
    graph->SetPointError(number_of_point++, 0, 3.5) ;
  }
  else if(scenario == normalized_to_chi2_1)
  {
    graph->SetPointError(number_of_point++, 0, 0.8048) ;
    graph->SetPointError(number_of_point++, 0, 0.6829) ;
    graph->SetPointError(number_of_point++, 0, 0.5121) ;
    graph->SetPointError(number_of_point++, 0, 0.8536) ;
  }
  else if(scenario == normalized_to_p_value_0p5)
  {
    graph->SetPointError(number_of_point++, 0, 1.20) ;
    graph->SetPointError(number_of_point++, 0, 1.018) ;
    graph->SetPointError(number_of_point++, 0, 0.7636) ;
    graph->SetPointError(number_of_point++, 0, 1.27) ;
  }
  else
  {
    cout << "Unknown scenario" << endl ;
    exit(1) ;
  }
  
  TCanvas c ;
  
  const int number_of_experiments_test = 1000 ;
  const int number_of_experiments_full = 1000e2 ;

  int number_of_experiments = number_of_experiments_full ;
  
  if(test) number_of_experiments = number_of_experiments_test ;
  
  if(!perturb) number_of_experiments = 1 ;

  TH1D *hist1 = new TH1D("hist1", "hist1", 100, 70, 90) ;
  TH1D *hist1_p_constraint = new TH1D("hist1_p_constraint", "hist1_p_constraint", 100, 70, 90) ;
  TH1D *hist1_concavity = new TH1D("hist1_concavity", "hist1_concavity", 100, 70, 90) ;
  
  double lower_boundary = 0.01 ;
  double upper_boundary = 14.0 * energy_factor ;
  upper_boundary = 10.0 ;

  TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, lower_boundary, upper_boundary, 100, 0, 140) ;

//  TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0, 1.4e-1, 100, 0, 140) ;

  hist_2d->SetTitle("") ;
  hist_2d->Draw("") ;
  hist_2d->GetXaxis()->SetTitle(("#sqrt{s} (" + postfix + ")").c_str()) ;
  hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

   graph->Draw("same p") ;

  graph->GetYaxis()->SetRangeUser(0, 120) ;
  
  for(int j = 0 ; j < number_of_experiments ; ++j)
  {
    graph2 = new TGraphErrors() ;

    for(int i = 0 ; i < graph->GetN() ; ++i)
    {
      double sqrts_value = 0 ;
      double dsdt = 0 ;
      double uncertainty_y = 0 ;

      graph->GetPoint(i, sqrts_value, dsdt) ;
      uncertainty_y = graph->GetErrorY(i) ;

      double mygaus = 0.0 ;

      if(perturb) mygaus = gRandom->Gaus() ;

      double dsdt_pert = dsdt + (mygaus * uncertainty_y) ;

      graph2->SetPoint(i, sqrts_value,   dsdt_pert) ; 
      graph2->SetPointError(i, 0, uncertainty_y) ;

    }

    int status = MinuitFit() ;
    
//    cout << endl << endl ;
//    cout << endl << endl ;
//    cout << endl << endl ;

//    double result = func->Eval(62.5e-3) ;

    double result = func->Eval(1.96 * energy_factor) ;

    if(test) cout << endl << "unit_scenario: " << unit_scenario << " energyfactor: " << energy_factor << " result:" << result << endl << endl ;
    
    if(!perturb)
    {
      stringstream ss ;
      
      ss << result ;
      
      TLatex *latex = new TLatex() ;

        latex->SetNDC() ;
        latex->SetTextSize(0.04) ;
        latex->SetTextFont(132) ;

        latex->DrawLatex(0.45, 0.85, (("#sigma_{tot}(1.96 " + postfix + ") = " + ss.str() + " mb").c_str())) ;
      
    }
    
    if(fit_scenario == fit_scenario_standard)
    {
      if(!perturb)
      {
         func->SaveAs("func.root") ;
         hist_2d->SaveAs("hist_2d.root") ;
      }

      if(test)
      {
        c.cd() ;
        func->Draw("same") ;
      }
      hist1->Fill(result) ;
    }
    else if(fit_scenario == fit_scenario_test_concavity)
    {
      // Parameter values

      if((par_vec(0) > 0.0) && (par_vec(2) < 80))
      {
        if(test)
        {
          c.cd() ;
          func->Draw("same") ;
        }

        if(status == 0) hist1->Fill(result) ;
      }        

    }
    else if(fit_scenario == fit_scenario_constraint_from_low_energy)
    {
      if(!perturb)
      {
         func->SaveAs("func.root") ;
         hist_2d->SaveAs("hist_2d.root") ;
      }

      // bool concavitiy_condition = ((par_vec(0) < 0.7) && (par_vec(0) > 0.61)) ;
      bool concavitiy_condition = (par_vec(0) < 0.7) ;
      //bool concavitiy_condition = true ;

      if(test)
      {
        c.cd() ;
        // func->Draw("same") ;

        // if(concavitiy_condition)
        {
          func->Draw("same") ;
        }
      }
      
      if(status == 0)
      {
        hist1->Fill(result) ;

        double p_value = TMath::Prob(save_chi2, number_of_free_parameters) ;
        chi2_hist->Fill(save_chi2) ;
        p_value_hist->Fill(p_value) ;

        if(p_value < 0.5) 
        {
          hist1_p_constraint->Fill(result) ;
        }
        
        // if(concavitiy_condition)
        {
          hist1_concavity->Fill(result) ;
        }
      }

    }
    else
    {
      cout << "Unknown fit scenario!" << endl ;
      exit(1) ;
    }
    
    if(number_of_experiments != 1) func->SetLineColor(j%10) ;

    // graph2->Draw("same p") ;
    
    // graph2->Delete() ;
  }
  
  gStyle->SetLineScalePS(.3) ;

  c.SaveAs("plots2/hist_2d.pdf") ;
  c.SaveAs("plots2/hist_2d.root") ;
  
  hist1->SetTitle("#sqrt{s}=1.96 TeV") ;
  hist1->Draw("") ;
  hist1->SaveAs("plots2/hist1.root") ;
  hist1->GetXaxis()->SetTitle("#sigma_{tot} (mb)") ;
  c.SaveAs("plots2/hist1.pdf") ;

  hist1_p_constraint->SetTitle("#sqrt{s}=1.96 TeV") ;
  hist1_p_constraint->Draw("") ;
  hist1_p_constraint->SaveAs("plots2/hist1.root") ;
  hist1_p_constraint->GetXaxis()->SetTitle("#sigma_{tot} (mb)") ;
  c.SaveAs("plots2/hist1_p_constraint.pdf") ;

  hist1_concavity->SetTitle("#sqrt{s}=1.96 TeV") ;
  hist1_concavity->Draw("") ;
  hist1_concavity->SaveAs("plots2/hist1.root") ;
  hist1_concavity->GetXaxis()->SetTitle("#sigma_{tot} (mb)") ;
  c.SaveAs("plots2/hist1_concavity.pdf") ;
  
  chi2_hist->Draw("") ;
  c.SaveAs("plots2/chi2_hist.pdf") ;

  p_value_hist->Draw("") ;
  c.SaveAs("plots2/p_value_hist.pdf") ;
}

  
