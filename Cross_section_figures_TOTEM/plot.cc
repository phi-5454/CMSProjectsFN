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
#include "TMultiGraph.h"

#include <iomanip>

double plab_to_sqrts(double plab)
{
        double m = 0.938272088;         // GeV
        double E = sqrt(m*m + plab*plab);
        return sqrt(2*m*(m + E));
}

TGraphAsymmErrors *sigma_total_graph_pp_TOTEM = new TGraphAsymmErrors() ;
TGraphAsymmErrors *sigma_total_graph_pp_STAR  = new TGraphAsymmErrors() ;
TGraphAsymmErrors *sigma_total_graph_pp_OTHER = new TGraphAsymmErrors() ;

TGraphAsymmErrors *sigma_total_graph_ppbar = new TGraphAsymmErrors() ;

ofstream compilation("results/results.txt") ;


TCanvas c ;

int add_TOTEM()
{

  ifstream sigma_total_data("files/sigma_total.txt") ;
  
  double energy, energy_unc_minus, energy_unc_plus, total_cross_section, uncertainty_plus, uncertainty_minus ;
  string type, collaboration, header ;

  getline(sigma_total_data,header) ;
  
  while(sigma_total_data >> energy >> energy_unc_minus >> energy_unc_plus >> total_cross_section >> uncertainty_plus >> uncertainty_minus >>  collaboration)
  {

    if(collaboration.compare("TOTEM") == 0)
    {
      sigma_total_graph_pp_TOTEM->AddPoint(energy, total_cross_section) ;
      int index = sigma_total_graph_pp_TOTEM->GetN() - 1;
      sigma_total_graph_pp_TOTEM->SetPointError(index, energy_unc_minus, energy_unc_plus, uncertainty_minus, uncertainty_plus) ;
      
      compilation << energy << " " << total_cross_section << " " <<  uncertainty_minus << " " <<  uncertainty_plus << " pp" << endl ;
    }
    else if(collaboration.compare("STAR") == 0)
    {
      sigma_total_graph_pp_STAR->AddPoint(energy, total_cross_section) ;
      int index = sigma_total_graph_pp_STAR->GetN() - 1;
      sigma_total_graph_pp_STAR->SetPointError(index, energy_unc_minus, energy_unc_plus, uncertainty_minus, uncertainty_plus) ;

      compilation << energy << " " << total_cross_section << " " <<  uncertainty_minus << " " <<  uncertainty_plus << " pp" << endl ;
    }
    else
    {
      sigma_total_graph_pp_OTHER->AddPoint(energy, total_cross_section) ;
      int index = sigma_total_graph_pp_OTHER->GetN() - 1 ;
      sigma_total_graph_pp_OTHER->SetPointError(index, energy_unc_minus, energy_unc_plus, uncertainty_minus, uncertainty_plus) ;

      compilation << energy << " " << total_cross_section << " " <<  uncertainty_minus << " " <<  uncertainty_plus << " pp" << endl ;
    }
  }
  
  sigma_total_data.cclose() ;

}

const int process_pp    = 1 ;
const int process_ppbar = 2 ;
const int process_pp_selected_by_Ken = 3 ;


int fit_scenario = 0 ;

const int fit_scenario_1_from_5_to_20_GeV  = 1 ;
const int fit_scenario_2_from_5_to_15_GeV  = 2 ;
const int fit_scenario_3_from_15_to_20_GeV = 3 ;
const int fit_scenario_4_from_15_to_25_GeV = 4 ;

double fit_low  = 0 ;
double fit_high = 0 ;

string postfix = "" ;

const string fit_function_name = "pol0" ;
// const string fit_function_name = "pol1" ;

double my_NDF = 0.0 ;
double FCN_value_with_uncertainty_1_on_points = 0.0 ;

bool randomize = false ;

TRandom3 myrand ;

int add_PDG(int process)
{
  bool test_uncertainties = true ;

  string header ;

  string data_file_name = "" ;

  if(process == process_pp) data_file_name = "hepdata/pp_total.dat" ;
  else if(process == process_ppbar) data_file_name = "hepdata/pbarp_total.dat" ;
  else if(process == process_pp_selected_by_Ken) data_file_name = "hepdata/selected_measurments_from_Ken/rpp2022-pp_total_5_15.txt" ;
  else
  {
    cout << "Unknown scenario!" << endl ;
    exit(1) ;
  }
  
  ifstream sigma_total_data(data_file_name) ;

  for(int i = 0 ; i < 11 ; ++i) getline(sigma_total_data, header) ;
  
  double POINT_NUMBER, PLAB, PLAB_MIN, PLAB_MAX, SIG, STA_ERRP, STA_ERRM, SY_ERP, SY_ERM ;
  string REF1, REF2, REF3, REF4, REF5, FLAG ;
  
  bool least_square_fit = true ;
  bool least_square_fit_obtain_FCN_for_unc_1 = false ;
  
  fit_scenario = fit_scenario_1_from_5_to_20_GeV ;
  fit_scenario = fit_scenario_3_from_15_to_20_GeV ;
  fit_scenario = fit_scenario_4_from_15_to_25_GeV ;

  fit_scenario = fit_scenario_2_from_5_to_15_GeV ; 
  
  if(fit_scenario == fit_scenario_1_from_5_to_20_GeV)
  {
    fit_low  =  5.0 ;
    fit_high = 20.0 ;
    
    postfix = "1" ;

    my_NDF = 61.0 ;

    if(fit_function_name.compare("pol0") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 23.0 ;
    }
    else if(fit_function_name.compare("pol1") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 21.0541 ;
    }
  }
  else if(fit_scenario == fit_scenario_2_from_5_to_15_GeV)
  {
    fit_low  =  5.0 ;
    fit_high = 15.0 ;

    postfix = "2" ;
    
    if(process == process_pp)
    {
      my_NDF = 48.0 ;

      if(fit_function_name.compare("pol0") == 0)
      {
        FCN_value_with_uncertainty_1_on_points = 16.4764 ;
      }
      else if(fit_function_name.compare("pol1") == 0)
      {
        FCN_value_with_uncertainty_1_on_points = 11.3775 ;
      }
    }
    else if(process == process_pp_selected_by_Ken)
    {
      my_NDF = 13.0 ;

      if(fit_function_name.compare("pol0") == 0)
      {
        FCN_value_with_uncertainty_1_on_points = 3.141 ;
      }
    }
    else if(process == process_ppbar)
    {
    }
    else
    {
      cout << "Unknown scenario2!" << endl ;
      exit(1) ;
    }
  }
 
  else if(fit_scenario == fit_scenario_3_from_15_to_20_GeV)
  {
    fit_low  = 15.0 ;
    fit_high = 20.0 ;

    postfix = "3" ;

    my_NDF = 12.0 ;

    if(fit_function_name.compare("pol0") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 6.27872 ;
    }
    else if(fit_function_name.compare("pol1") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 4.62413 ;
    }
  }
  else if(fit_scenario == fit_scenario_4_from_15_to_25_GeV)
  {
    fit_low  = 15.0 ;
    fit_high = 25.0 ;

    postfix = "4" ;

    my_NDF = 24.0 ;

    if(fit_function_name.compare("pol0") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 13.6682 ;
    }
    else if(fit_function_name.compare("pol1") == 0)
    {
      FCN_value_with_uncertainty_1_on_points = 7.88967 ;
    }
  }
  else
  {
    cout << "No such scenario!" << endl ;
    exit(1) ;
  }
  
  if(fit_function_name.compare("pol1") == 0) my_NDF = my_NDF - 1 ;
  
  while(sigma_total_data >> POINT_NUMBER >> PLAB >> PLAB_MIN >> PLAB_MAX >> SIG >> STA_ERRP >> STA_ERRM >> SY_ERP >> SY_ERM >> REF1 >> REF2 >> REF3 >> REF4 >> REF5)
  {

    const double percent_to_mb = 1.0E-2 ;

    double syst_minus_mb = SIG * SY_ERM * percent_to_mb ;
    double syst_plus_mb  = SIG * SY_ERP * percent_to_mb ;

    double uncertainty_minus = sqrt((STA_ERRM*STA_ERRM) + (syst_minus_mb*syst_minus_mb)) ;
    double uncertainty_plus = sqrt((STA_ERRP*STA_ERRP) + (syst_plus_mb*syst_plus_mb)) ;
    
  
  	if(least_square_fit)
    {
      // cout << "here" << endl ;
      
      double random_factor = 1.0 ; 
      
      
  		uncertainty_minus = sqrt(FCN_value_with_uncertainty_1_on_points/my_NDF);
	  	uncertainty_plus =  sqrt(FCN_value_with_uncertainty_1_on_points/my_NDF);

      if(randomize)
      {
        random_factor = myrand.Gaus(0.0) * uncertainty_plus ;
        SIG = SIG + random_factor ;
      }
      
      // SIG = SIG ;

      // cout << uncertainty_plus << endl ;
      
      if(least_square_fit_obtain_FCN_for_unc_1)
      {
        uncertainty_minus = 1.0 ;
        uncertainty_plus = 1.0 ;
      }
    }

    if((process == process_ppbar) && (POINT_NUMBER == 442))
    {
      sigma_total_data >> FLAG ;
    }

    double energy = plab_to_sqrts(PLAB) ;
    double energy_unc_minus = energy - plab_to_sqrts(PLAB_MIN) ;
    double energy_unc_plus  = plab_to_sqrts(PLAB_MAX) - energy ;

    if(test_uncertainties)
    {
      if(STA_ERRP != STA_ERRM)
      {
        // cout << "asymstat " << process << std::fixed << std::setprecision(3) << " " << PLAB << " " << energy << " " << (STA_ERRP/STA_ERRM) << " " << REF1 << " " << REF4 << endl ;
      }

      if(SY_ERP != SY_ERM)
      {
        // cout << "asymsyst " << process << std::fixed << std::setprecision(3) << " " << PLAB << " " << energy << " " << (SY_ERP/SY_ERM)     << " " << REF1 << " " << REF4 << endl ;
      }

      // if((energy >= 5) && (energy <= 20)) std::cout << "most: " << std::fixed << std::setprecision(2) << energy << " GeV\t " << REF1 << " " << REF4 << " " << std::setprecision(3) << 1000*(STA_ERRP/SIG) << endl ;
    }

    if((energy_unc_minus < 0) || (energy_unc_plus < 0))
    {
      cout << "Error: wrong process" << endl ;
      exit(1) ;
    }
    
    if((process == process_pp) || (process == process_pp_selected_by_Ken))
    {
      sigma_total_graph_pp_OTHER->AddPoint(energy, SIG) ;
      int index = sigma_total_graph_pp_OTHER->GetN() - 1 ;
      sigma_total_graph_pp_OTHER->SetPointError(index, energy_unc_minus, energy_unc_plus, uncertainty_minus, uncertainty_plus) ;

      compilation << energy << " " << SIG << " " <<  uncertainty_minus << " " <<  uncertainty_plus << " pp" << endl ;
    }
    else if(process == process_ppbar)
    {
      sigma_total_graph_ppbar->AddPoint(energy, SIG) ;
      int index = sigma_total_graph_ppbar->GetN() - 1 ;
      sigma_total_graph_ppbar->SetPointError(index, energy_unc_minus, energy_unc_plus, uncertainty_minus, uncertainty_plus) ;

      compilation << energy << " " << SIG << " " <<  uncertainty_minus << " " <<  uncertainty_plus << " ppbar" << endl ;
    }
    else
    {
      cout << "Error: wrong process" << endl ;
      exit(1) ;
    }
  }

  sigma_total_data.cclose() ;

}

void fit()
{

	// TFitResultPtr ptr = sigma_total_graph_pp_OTHER->Fit(fit_function_name.c_str(), "S", "", 10, 20) ;
  
//  gStyle->SetOptStat(111111) ;    
  gStyle->SetOptStat(0) ;
  gStyle->SetOptFit(111111) ;    
  

	TFitResultPtr ptr = sigma_total_graph_pp_OTHER->Fit(fit_function_name.c_str(), "S", "", fit_low, fit_high) ;
	// TFitResultPtr ptr = sigma_total_graph_pp_OTHER->Fit(fit_function_name.c_str(), "S", "", 5.1, 5.6) ;
  
  cout << "ForParameterError: " << ptr->Parameter(0) << endl ;

	sigma_total_graph_pp_OTHER->GetFunction(fit_function_name.c_str())->SetLineColor(kBlack) ;
	
  cout << endl ;
  cout << "Fit range: " << fit_low << " " << fit_high << endl ;
	cout << "NDF in fit: " << ptr->Ndf() << " my_NDF: " << my_NDF << endl ;
  cout << "Effective uncertainty: " << sqrt(FCN_value_with_uncertainty_1_on_points/my_NDF) << " " << sqrt(FCN_value_with_uncertainty_1_on_points/my_NDF) << endl ;
}

// TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1e0, 1e5, 100, 0, 380) ;
TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1, 25, 100, 30, 80) ;

int post_process()
{
  // pp TOTEM

  sigma_total_graph_pp_TOTEM->Draw("same p") ;
  sigma_total_graph_pp_TOTEM->SetMarkerStyle(20) ;
  sigma_total_graph_pp_TOTEM->SetMarkerSize(0.5) ;

  sigma_total_graph_pp_TOTEM->SetLineColor(kRed) ;
  sigma_total_graph_pp_TOTEM->SetMarkerColor(kRed) ;

  // pp STAR

  sigma_total_graph_pp_STAR->Draw("same p") ;
  sigma_total_graph_pp_STAR->SetMarkerStyle(30) ;
  sigma_total_graph_pp_STAR->SetMarkerSize(0.5) ;

  sigma_total_graph_pp_STAR->SetLineColor(kRed) ;
  sigma_total_graph_pp_STAR->SetMarkerColor(kRed) ;

  // pp OTHER

  sigma_total_graph_pp_OTHER->Draw("same p") ;
  sigma_total_graph_pp_OTHER->SetMarkerStyle(32) ;
  sigma_total_graph_pp_OTHER->SetMarkerSize(0.5) ;

  sigma_total_graph_pp_OTHER->SetLineColor(kRed) ;
  sigma_total_graph_pp_OTHER->SetMarkerColor(kRed) ;

  // ppbar
  
  sigma_total_graph_ppbar->Draw("same p") ;
  sigma_total_graph_ppbar->SetMarkerStyle(26) ;
  sigma_total_graph_ppbar->SetMarkerSize(0.5) ;

  sigma_total_graph_ppbar->SetLineColor(kRed) ;
  sigma_total_graph_ppbar->SetMarkerColor(kRed) ;

  c.SetTitle("") ;
  c.SetGridy() ;
  c.SetLogx() ;
  
 
  c.SaveAs(("fig/sigma_total_graph_" + postfix + "_" + fit_function_name + ".pdf").c_str()) ;
  c.SaveAs(("fig/sigma_total_graph_" + postfix + "_" + fit_function_name + ".root").c_str()) ;
}


int main(int argc, char *argv[])
{
  int myseed = atoi(argv[1]) ;

  myrand.SetSeed(myseed) ;

  gStyle->SetLineScalePS(.3) ;

  hist_2d->GetXaxis()->SetTitle("#sqrt{s}  (GeV)") ;
  hist_2d->GetXaxis()->SetTitleOffset(1.2) ;
  hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

  hist_2d->SetTitle("") ;
  // hist_2d->GetXaxis()->SetRangeUser(5, 25) ;
  hist_2d->Draw() ;

  add_TOTEM() ;
//  add_PDG(process_pp_selected_by_Ken) ;
  add_PDG(process_pp) ;
  add_PDG(process_ppbar) ;

  fit() ;
  
  post_process() ;

  cout << "Completed" << endl ;  
}

  
