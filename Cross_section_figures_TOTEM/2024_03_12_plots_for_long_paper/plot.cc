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
      
      // cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

const int scenario_prelim_1 = 1 ;
const int scenario_prelim_2 = 2 ;

int plot_sigtot(int scenario)
{
   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph_1p96 = new TGraphErrors ;
	
	TCanvas c ;
	
	TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1, 14, 100, 75, 115) ;	
	hist_2d->SetTitle("") ;

	graph->SetMarkerStyle(20) ;
	graph_1p96->SetMarkerStyle(20) ;

	graph_1p96->SetMarkerColor(kBlue) ;

	string myfilename = "data/TOTEM_D0_14_PRL_preliminary_1_cross_section.txt" ;
	string myplotname = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.pdf" ;
	
	if(scenario ==  scenario_prelim_2)
	{
		myfilename = "data/TOTEM_D0_14_PRL_preliminary_2_cross_section.txt" ;
		myplotname = "fig/TOTEM_D0_14_PRL_preliminary_2_cross_section.pdf" ;
	}
	
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

   double epsilon = 0.01 ;
	TF1 *func = new TF1("func",  log_like_function, epsilon, 14, 3) ;	
	
	
   func->SetParameters(73.75607, 8.41508, 2.29419) ;
   func->SetNpx(100) ;	

	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle("#sqrt{s} (TeV)") ;
	hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

	graph->Draw("same p") ;
	graph_1p96->Draw("same p") ;
	func->SetRange(1.5, 15) ;
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
