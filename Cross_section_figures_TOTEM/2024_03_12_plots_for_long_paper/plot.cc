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

	string myfilename = "data/TOTEM_D0_14_PRL_preliminary_1_cross_section.txt" ;
	string myplotname = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.pdf" ;
	
	double cp = 73.75607 ;
	double bp = 8.41508 ;
	double ap = 2.29419 ;
	
	string axis_title = "#sqrt{s} (TeV)" ;

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

		axis_title = "#sqrt{s} (GeV)" ;

	}

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 1.0 * TeV_to_GeV, 14.0 * TeV_to_GeV, 100, 75, 115) ;	
   TF1 *func = new TF1("func", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 3) ;	

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
	hist_2d->GetXaxis()->SetTitle(axis_title.c_str()) ;
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

int plot_dsdt()
{
	TCanvas c ;
	c.SetLogy() ;

   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0.45, 1.0, 100, 0.005, 0.1) ;	

	hist_2d->SetTitle("") ;
	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph2 = new TGraphErrors ;

	graph->SetMarkerStyle(20) ;
	graph2->SetMarkerStyle(20) ;

	graph->SetMarkerColor(kBlue) ;
	graph2->SetMarkerColor(kRed) ;

 	ifstream data("data/TOTEM_D0_14_PRL_preliminary_1_dsdt.txt") ;

	double t_value , t_unc , dsdt , dsdt_unc , norm_plus , norm_minus ;

	int i = 0 ;

	while(data >> t_value >> t_unc >> dsdt >> dsdt_unc >> norm_plus >> norm_minus)
	{
		graph->SetPoint(i, t_value, dsdt) ;
		graph->SetPointError(i, 0, dsdt_unc) ;
		
		++i ;
	}
	
	data.close() ;

	ifstream data2("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated.txt") ;

	i = 0 ;
	
	double chi2start ;

	while(data2 >> t_value >> dsdt >> dsdt_unc >> chi2start)
	{
		graph2->SetPoint(i, t_value, dsdt) ;
		graph2->SetPointError(i, 0, dsdt_unc) ;
		
		++i ;
	}
	
	data2.close() ;
	
	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle("|t| (GeV^{2})") ;
	hist_2d->GetYaxis()->SetTitle("d#sigma/dt (mb/GeV^{2})") ;

	graph->Draw("same p") ;
	graph2->Draw("same p") ;

   c.SaveAs("fig/TOTEM_D0_14_PRL_preliminary_1_dsdt.pdf") ;
}

int main(int argc, char *argv[])
{
	plot_sigtot(scenario_prelim_1) ;
	plot_sigtot(scenario_prelim_2) ;
	plot_dsdt() ;
}
