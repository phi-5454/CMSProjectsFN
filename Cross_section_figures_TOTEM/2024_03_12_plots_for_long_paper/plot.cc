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

Double_t log_like_function(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0]), 0) ;
        double f2 = par[1] * pow(log(x[0]), 1) ;
        double f3 = par[2] * pow(log(x[0]), 2) ;

        double f = f1 + f2 + f3 ;
      
        cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

const int scenario_prelim_1 = 1 ;
const int scenario_prelim_2 = 2 ;

int plot_sigtot(int scenario)
{

	TH2D *hist_2d = NULL ;
   TF1 *func = NULL ;

	string myfilename = "data/TOTEM_D0_14_PRL_preliminary_1_cross_section.txt" ;
	string myplotname = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.pdf" ;
	
	double cp = 73.75607 ;
	double bp = 8.41508 ;
	double ap = 2.29419 ;

   double epsilon = 0.01 ;
	double TeV_to_GeV = 1.0 ;
	
	if(scenario ==  scenario_prelim_2)
	{
		myfilename = "data/TOTEM_D0_14_PRL_preliminary_2_cross_section.txt" ;
		myplotname = "fig/TOTEM_D0_14_PRL_preliminary_2_cross_section.pdf" ;

		cp = 125.098837177212 ;
		bp = -23.2803355035864 ;
		ap = 2.29419079175536 ;

   	TeV_to_GeV = 1.0e3 ;
      hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1000, 14000, 100, 75, 115) ;	
      func = new TF1("func",  log_like_function, 10, 14000, 3) ;	

	}
	else
	{
      hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1, 14, 100, 75, 115) ;	
      func = new TF1("func",  log_like_function, epsilon, 14, 3) ;	

	}

   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph_1p96 = new TGraphErrors ;
	
	TCanvas c ;
	
	hist_2d->SetTitle("") ;

	graph->SetMarkerStyle(20) ;
	graph_1p96->SetMarkerStyle(20) ;

	graph_1p96->SetMarkerColor(kBlue) ;

	ifstream data(myfilename.c_str()) ;
	
	string word ;
	
	double energy, sigtot, sigtotunc ;

	int i = 0 ;
	
	while(data >> energy >> sigtot >> sigtotunc)
	{
		cout << energy << " " << sigtot << " " << sigtotunc << endl ;

		if(energy == 1.96)
		{
			graph_1p96->SetPoint(0, energy, sigtot) ;
			graph_1p96->SetPointError(0, 0, sigtotunc) ;
		}
		else
		{
			graph->SetPoint(i, energy, sigtot) ;
			graph->SetPointError(i, 0, sigtotunc) ;
			++i ;
		}
	}

   func->SetParameters(cp, bp, ap) ;
   func->SetNpx(100) ;	

	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle("#sqrt{s} (TeV)") ;
	hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

	graph->Draw("same p") ;
	graph_1p96->Draw("same p") ;
	func->SetRange(1.5 * TeV_to_GeV, 15 * TeV_to_GeV) ;
	func->Draw("same l") ;

	c.SaveAs(myplotname.c_str()) ;

	delete hist_2d ;
	delete graph ;
	delete graph_1p96 ;
	delete func ;
}

int main(int argc, char *argv[])
{
	plot_sigtot(scenario_prelim_1) ;
	plot_sigtot(scenario_prelim_2) ;
}
