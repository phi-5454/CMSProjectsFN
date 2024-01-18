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

TGraphErrors *graph = NULL ;

MyTPoint::MyTPoint(double energy, double sigtot, double uncertainty) : energy(energy), sigtot(sigtot), uncertainty_up(uncertainty), uncertainty_down(uncertainty)
{

}

Double_t log_like_function(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0]), 0) ;
        double f2 = par[1] * pow(log(x[0]), 1) ;
        double f3 = par[2] * pow(log(x[0]), 2) ;

        double f = f1 + f2 + f3 ;

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
    
    if(constraint && (energy == 1.5) && (value > 39.28)) chi2 += 1.0 ;
  }
  
  cout << chi2 << endl ;

  f = chi2 ;
}

double par[4] ;
double pare[4] ;

void MinuitFit()
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "a", 27, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(1, "b", -4.37008e-02, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(2, "c",  0.15, 0.1, 0, 0, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  func = new TF1("func",  log_like_function, 0.0, 1400.0, 3) ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
  
  gMinuit2->GetParameter(0, par[0], pare[0]) ;
  gMinuit2->GetParameter(1, par[1], pare[1]) ;
  gMinuit2->GetParameter(2, par[2], pare[2]) ;
  
  func->SetParameters(par[0], par[1], par[2]) ;
  func->SetNpx(100000) ;
}

void init()
{
  gStyle->SetLineScalePS(.3) ;
  graph = new TGraphErrors() ;
}

int main(int argc, char *argv[])
{
  init() ;

	ifstream data("hepdata/most_relevant_points.txt") ;
	
	double energy, sigtot, sigtot_unc ;
  
  int n_points = 0 ;
	
	while(data >> energy >> sigtot >> sigtot_unc)
	{
    if(constraint && (energy == 1.5)) sigtot_unc *= 1e6 ;

    MyTPoint *p = new MyTPoint(energy, sigtot, sigtot_unc) ;
    points.push_back(p) ;
    
    graph->SetPoint(n_points, energy, sigtot) ;
    graph->SetPointError(n_points, 0, sigtot_unc) ;
    
    ++n_points ;
	}
  
  MinuitFit() ;
  
  TCanvas *c = new TCanvas ;
 
  c->cd() ;
 	c->SetLogx() ;

  graph->Draw("ap") ;  
	graph->GetXaxis()->SetTitle("#sqrt{s} (GeV)") ;
	graph->GetXaxis()->SetTitleOffset(1.5) ;
	graph->SetMarkerStyle(20) ;
	graph->SetMarkerSize(0.8) ;
  
 	graph->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;  
 	graph->GetYaxis()->SetRangeUser(0, 120.) ;
  
  func->Draw("same l") ;
  
  stringstream ss[4], ssc[4] ;
  stringstream rs, cs ;
  
  double result = func->Eval(196.0) ;
  cout << "result:" << result << endl ;
  rs << std::setprecision(4) << result ;

  double result2 = func->Eval(1.5) ;
  cs << std::setprecision(4) << result2 ;

  ss[0]  << std::setprecision(4) << par[0] ;
  ssc[0] << std::setprecision(2) << pare[0] ;
  ss[1]  << std::setprecision(4) << par[1] ;
  ssc[1] << std::setprecision(2) << pare[1] ;
  ss[2]  << std::setprecision(4) << par[2] ;
  ssc[2] << std::setprecision(2) << pare[2] ;
  
	TLatex *latex = new TLatex() ;

	latex->SetNDC() ;
	latex->SetTextFont(132) ;
	latex->SetTextColor(kBlack) ;

	latex->DrawLatex(.18, .84, ("a = " + ss[2].str() + " #pm " + ssc[2].str()).c_str()) ;
	latex->DrawLatex(.18, .74, ("b = " + ss[1].str() + " #pm " + ssc[1].str()).c_str()) ;
	latex->DrawLatex(.18, .64, ("c = " + ss[0].str() + " #pm " + ssc[0].str()).c_str()) ;
	latex->DrawLatex(.18, .54, ("#sigma_{tot}(1.96 TeV) = " + rs.str()).c_str()) ;
	latex->DrawLatex(.18, .44, ("#sigma_{tot}(15 GeV) = " + cs.str()).c_str()) ;

  graph->SaveAs("results/graph.root") ;

  
  c->SaveAs("results/c.root") ;
  c->SaveAs("results/c.pdf") ;
}

  
