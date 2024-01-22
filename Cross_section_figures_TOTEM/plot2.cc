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

bool constraint = true ;

class MyTPoint
{
  public:  
  
  double energy ;
  double sigtot ;
  double uncertainty_up ;
  double uncertainty_down ;


  MyTPoint(double, double, double) ;
} ;

TF1 *func = NULL ;
vector<MyTPoint *> points ;

MyTPoint::MyTPoint(double energy, double sigtot, double uncertainty) : energy(energy), sigtot(sigtot), uncertainty_up(uncertainty), uncertainty_down(uncertainty)
{

}

Double_t log_like_function(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0]), 0) ;
        double f2 = par[1] * pow(log(x[0]), 1) ;
        double f3 = par[2] * pow(log(x[0]), 2) ;

        double f = f1 + f2 + f3 ;
      
      // cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
  double chi2 = 0 ;
  
  for(int i = 0 ; i < points.size() ; ++i)
  {
    double energy = points[i]->energy ;
    double sigtot = points[i]->sigtot ;
    double uncertainty_up = points[i]->uncertainty_up ;

    double value = log_like_function(&energy, par) ;

    double delta = (value - sigtot) / uncertainty_up ;

    chi2 += delta*delta ;
  }
  
  f = chi2 ;
}

void MinuitFit()
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];

  arglist[0] = -1 ;
  Int_t ierflg = 0 ;
  gMinuit2->mnexcm("SET PRI", arglist ,1,ierflg);

  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);


  gMinuit2->mnparm(0, "a", 27, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(1, "b", -4.37008e-02, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(2, "c",  0.15, 0.1, 0, 0, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  double epsilon = 0.01 ;
  func = new TF1("func",  log_like_function, epsilon, 1400.0, 3) ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);

  double func_par[4] ;
  double func_pare[4] ;
  
  gMinuit2->GetParameter(0, func_par[0], func_pare[0]) ;
  gMinuit2->GetParameter(1, func_par[1], func_pare[1]) ;
  gMinuit2->GetParameter(2, func_par[2], func_pare[2]) ;
  
  func->SetParameters(func_par[0], func_par[1], func_par[2]) ;
  func->SetNpx(100000) ;
}

TGraphErrors *graph = new TGraphErrors() ;

void init()
{
  gStyle->SetLineScalePS(.3) ;
  
  ifstream data("hepdata/most_relevant_points.txt") ;
  
  double energy, sigtot, sigtot_unc ;
  
  int n_points = 0 ;
  
  while(data >> energy >> sigtot >> sigtot_unc)
  {
    if(constraint)
    {
       if((energy == 0.5) || (energy == 1.5)) continue ;
    }
   
    graph->SetPoint(n_points, energy, sigtot) ;
    graph->SetPointError(n_points, 0, sigtot_unc) ;
    
    ++n_points ;
  }

  graph->Draw("ap") ;  
  graph->GetXaxis()->SetTitle("#sqrt{s} (GeV)") ;
  graph->GetXaxis()->SetTitleOffset(1.5) ;
  graph->SetMarkerStyle(20) ;
  graph->SetMarkerSize(0.8) ;

  graph->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;  
  graph->GetYaxis()->SetRangeUser(0, 120.) ;
  
}

double alpha = 1.0 ;
TRandom3 myrand ;

TCanvas *c = new TCanvas ;

int counter = 0 ;

TH1D *hist = new TH1D("hist", "hist", 1000, 0, 100) ;
TH1D *hist2 = new TH1D("hist2", "hist2", 1000, 0, 100) ;
TH1D *hist3 = new TH1D("hist3", "hist3", 1000, 0, 100) ;

void test()
{
  double func_par[4] ;
  double func_pare[4] ;

  ifstream data("hepdata/most_relevant_points.txt") ;
  
  double energy, sigtot, sigtot_unc ;
  
  int n_points = 0 ;
  
  while(data >> energy >> sigtot >> sigtot_unc)
  {
    double perturb = myrand.Gaus() ;

    if(constraint)
    {
      if((energy == 0.5) || (energy == 1.5)) continue ;
      
      if(energy > 200.0) sigtot += (alpha * sigtot_unc * perturb) ;
    }
   
    MyTPoint *p = new MyTPoint(energy, sigtot, sigtot_unc) ;
    points.push_back(p) ;
  }
  
  data.close() ;
  
  MinuitFit() ;
  
  c->cd() ;
  c->SetLogx() ;
  
  if(func->Eval(1.5) <= 39.3)
  {
    // cout << "wasok: " << counter << endl ;
    hist->Fill(func->Eval(1.5)) ;
    
    if(((func->Eval(1.5) > 39.2)) && (func->Eval(1.5) < 39.3)) hist2->Fill(func->Eval(196.0)) ;
    // if((func->Eval(1.5) < 39.0) && ((func->Eval(1.5) > 38.9))) hist2->Fill(func->Eval(196.0)) ;
    // func->Draw("same l") ;
  }

  hist3->Fill(func->Eval(1.5)) ;
  
  counter++ ;
  
  stringstream ss[4], ssc[4] ;
  stringstream rs, cs ;
  
  double result = func->Eval(196.0) ;
  // cout << "result:" << result << endl ;
  rs << std::setprecision(4) << result ;

  double result2 = func->Eval(1.0) ;
  // cout << "result2:" << result2 << endl ;

  double result3 = func->Eval(1.5) ;
  // cout << "result3:" << result3 << endl ;

  ss[0]  << std::setprecision(4) << func_par[0] ;
  ssc[0] << std::setprecision(2) << func_pare[0] ;
  ss[1]  << std::setprecision(4) << func_par[1] ;
  ssc[1] << std::setprecision(2) << func_par[1] ;
  ss[2]  << std::setprecision(4) << func_par[2] ;
  ssc[2] << std::setprecision(2) << func_pare[2] ;

  if(!constraint) 
  {
    TLatex *latex = new TLatex() ;

    latex->SetNDC() ;
    latex->SetTextFont(132) ;
    latex->SetTextColor(kBlack) ;

    latex->DrawLatex(.18, .84, ("a = " + ss[2].str() + " #pm " + ssc[2].str()).c_str()) ;
    latex->DrawLatex(.18, .74, ("b = " + ss[1].str() + " #pm " + ssc[1].str()).c_str()) ;
    latex->DrawLatex(.18, .64, ("c = " + ss[0].str() + " #pm " + ssc[0].str()).c_str()) ;
    latex->DrawLatex(.18, .54, ("#sigma_{tot}(1.96 TeV) = " + rs.str()).c_str()) ;
    latex->DrawLatex(.18, .44, ("#sigma_{tot}(15 GeV) = " + cs.str()).c_str()) ;
  }
  
  points.clear() ;
}
  
int main(int argc, char *argv[])
{
  init() ;

  for(int i = 0 ; i < 100000 ; ++i) test() ;  

  // graph->SaveAs("results/graph.root") ;
  c->cd() ;
  // func->SaveAs("results/func.root") ;
  
  // c->SaveAs("results/c.root") ;
  // c->SaveAs("results/c.pdf") ;
  
  hist->SaveAs("hist.root") ;
  hist2->SaveAs("hist2.root") ;
  hist3->SaveAs("hist3.root") ;
}
