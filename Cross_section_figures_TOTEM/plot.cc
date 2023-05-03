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
  
  sigma_total_data.close() ;

}

const int process_pp    = 1 ;
const int process_ppbar = 2 ;

int add_PDG(int process)
{
  bool test_uncertainties = true ;

  string header ;

  string data_file_name = "" ;

  if(process == process_pp) data_file_name = "hepdata/pp_total.dat" ;
  if(process == process_ppbar) data_file_name = "hepdata/pbarp_total.dat" ;
  
  ifstream sigma_total_data(data_file_name) ;

  for(int i = 0 ; i < 11 ; ++i) getline(sigma_total_data, header) ;
  
  double POINT_NUMBER, PLAB, PLAB_MIN, PLAB_MAX, SIG, STA_ERRP, STA_ERRM, SY_ERP, SY_ERM ;
  string REF1, REF2, REF3, REF4, REF5, FLAG ;

  while(sigma_total_data >> POINT_NUMBER >> PLAB >> PLAB_MIN >> PLAB_MAX >> SIG >> STA_ERRP >> STA_ERRM >> SY_ERP >> SY_ERM >> REF1 >> REF2 >> REF3 >> REF4 >> REF5)
  {

    const double percent_to_mb = 1.0E-2 ;

    double syst_minus_mb = SIG * SY_ERM * percent_to_mb ;
    double syst_plus_mb  = SIG * SY_ERP * percent_to_mb ;

    double uncertainty_minus = sqrt((STA_ERRM*STA_ERRM) + (syst_minus_mb*syst_minus_mb)) ;
    double uncertainty_plus = sqrt((STA_ERRP*STA_ERRP) + (syst_plus_mb*syst_plus_mb)) ;

    if((process == process_ppbar) && (POINT_NUMBER == 442))
    {
      sigma_total_data >> FLAG ;
    }

    double energy = plab_to_sqrts(PLAB) ;
    double energy_unc_minus = energy - plab_to_sqrts(PLAB_MIN) ;
    double energy_unc_plus  = plab_to_sqrts(PLAB_MAX) - energy ;

    if(test_uncertainties)
    {
      if(STA_ERRP != STA_ERRM) cout << "asymstat " << process << std::fixed << std::setprecision(3) << " " << PLAB << " " << energy << " " << (STA_ERRP/STA_ERRM) << " " << REF1 << " " << REF4 << endl ;
      if(SY_ERP != SY_ERM)     cout << "asymsyst " << process << std::fixed << std::setprecision(3) << " " << PLAB << " " << energy << " " << (SY_ERP/SY_ERM)     << " " << REF1 << " " << REF4 << endl ;
    }

    if((energy_unc_minus < 0) || (energy_unc_plus < 0))
    {
      cout << "Error: wrong process" << endl ;
      exit(1) ;
    }

    if(process == process_pp)
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

  sigma_total_data.close() ;

}

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

  c.SaveAs("fig/sigma_total_graph.pdf") ;
}


int main(int argc, char *argv[])
{
  gStyle->SetLineScalePS(.3) ;
  gStyle->SetOptStat(0) ;    

  TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1e0, 1e5, 100, 0, 140) ;

  hist_2d->GetXaxis()->SetTitle("#sqrt{s}  (GeV)") ;
  hist_2d->GetXaxis()->SetTitleOffset(1.2) ;
  hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

  hist_2d->SetTitle("") ;
  hist_2d->Draw() ;

  add_TOTEM() ;
  add_PDG(process_pp) ;
  add_PDG(process_ppbar) ;
  
  post_process() ;

  cout << "Completed" << endl ;  
}

  
