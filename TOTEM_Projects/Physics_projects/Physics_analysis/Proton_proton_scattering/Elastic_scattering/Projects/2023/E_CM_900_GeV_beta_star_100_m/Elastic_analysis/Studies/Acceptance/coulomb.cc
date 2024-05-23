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

int main(int argc, char *argv[])
{

  if(argc != 2) 
  {
    cout << "Please provide ..." << endl ;
    exit(1) ;
  }
   const double epsilon = 1e-5 ;

  TF1 *func = new TF1("sigma",  sigma, epsilon, 1, 0) ;
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
}
