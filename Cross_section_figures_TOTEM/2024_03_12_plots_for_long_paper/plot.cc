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
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
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

        double f = par[3] * (f1 + f2 + f3) ;
      
        cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

Double_t log_like_function2(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0] / 10.0), 0) ;
        double f2 = par[1] * pow(log(x[0] / 10.0), 1) ;
        double f3 = par[2] * pow(log(x[0] / 10.0), 2) ;

        double f = par[3] * (f1 + f2 + f3) ;
      
        cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

const int scenario_prelim_1 = 1 ;
const int scenario_prelim_2 = 2 ;

void mydraw()
{
   TLatex *latex = new TLatex() ;

   latex->SetNDC() ;
   latex->SetTextFont(132) ;
   latex->SetTextColor(kBlack) ;
	
	latex->DrawLatex(.7, .14, "TOTEM-D#oslash") ;
}

const double axis_title_size = (0.035*1.2) ;

int plot_sigtot(int scenario)
{

	string myfilename = "data/TOTEM_D0_14_PRL_preliminary_1_cross_section.txt" ;
	string myplotname = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.pdf" ;
	string myplotname2 = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.root" ;
	string myplotname3 = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.png" ;
	
	double cp = 73.75607 ;
	double bp = 8.41508 ;
	double ap = 2.29419 ;
	
	string axis_title = "#sqrt{s} (TeV)" ;

   double epsilon = 0.01 ;
	double TeV_to_GeV = 1.0 ;
	
	double low_limit = 1.0 * TeV_to_GeV ;
	double y_low_limit = 75 ;

	TCanvas c ;

	if(scenario ==  scenario_prelim_2)
	{
		c.SetLogx() ;

		myfilename = "data/TOTEM_D0_14_PRL_preliminary_2_cross_section.txt" ;
		myplotname = "fig/TOTEM_D0_14_PRL_preliminary_2_cross_section.pdf" ;
		myplotname2 = "fig/TOTEM_D0_14_PRL_preliminary_2_cross_section.root" ;
		myplotname3 = "fig/TOTEM_D0_14_PRL_preliminary_1_cross_section.png" ;

		cp = 125.098837177212 ;
		bp = -23.2803355035864 ;
		ap = 2.29419079175536 ;

   	TeV_to_GeV = 1.0e3 ;

		axis_title = "#sqrt{s} (GeV)" ;

		low_limit = 0.001 * TeV_to_GeV ;
		y_low_limit = 30 ;

	}

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, low_limit, 14.0 * TeV_to_GeV, 100, y_low_limit, 115) ;	

   TF1 *func = new TF1("afunc", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	
   TF1 *func_p = new TF1("func_p", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	
   TF1 *func_m = new TF1("func_m", log_like_function, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	

   TF1 *func_15_GeV = new TF1("afunc2", log_like_function2, epsilon * TeV_to_GeV, 14.0 *  TeV_to_GeV, 4) ;	

   gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph_1p96 = new TGraphErrors ;
	TGraphErrors *graph_10_GeV = new TGraphErrors ;

	hist_2d->SetTitle("") ;

	graph->SetMarkerStyle(20) ;
	graph->SetMarkerSize(0.8) ;

	graph_1p96->SetMarkerStyle(4) ;
	graph_1p96->SetMarkerSize(0.8) ;

	graph_10_GeV->SetMarkerStyle(4) ;
	graph_10_GeV->SetMarkerSize(0.8) ;

	graph_1p96->SetMarkerColor(kBlue) ;
	graph_10_GeV->SetMarkerColor(kRed) ;

	ifstream data(myfilename.c_str()) ;
	
	string word ;
	
	double energy, sigtot, sigtotunc ;

	int i = 0 ;
	
	while(data >> energy >> sigtot >> sigtotunc)
	{
		cout << energy << " " << sigtot << " " << sigtotunc << endl ;

		if(energy == 1.96 * TeV_to_GeV)
		{
			graph_1p96->SetPoint(0, energy, sigtot) ;
			graph_1p96->SetPointError(0, 0, sigtotunc) ;

			if(scenario ==  scenario_prelim_2)
			{
				graph_1p96->SetPointError(0, 0, 0) ;
			}
		}
		else
		{
			graph->SetPoint(i, energy, sigtot) ;
			if(scenario ==  scenario_prelim_2) graph->SetPointError(i, 0, 0) ;
			else graph->SetPointError(i, 0, sigtotunc) ;
			++i ;
		}
	}
	
	if(scenario ==  scenario_prelim_2)
	{
		graph_10_GeV->SetPoint(0, 10, 38.69) ;
		graph_10_GeV->SetPointError(0, 0.0, 0.4865) ;
   }

	double uncertainy_factor = 1.01 ;

   func->SetParameters(cp, bp, ap, 1.0) ;
   func_p->SetParameters(cp, bp, ap, uncertainy_factor) ;
   func_m->SetParameters(cp, bp, ap, 1.0 / uncertainy_factor) ;
	
   func_15_GeV->SetParameters(cp, bp, ap, 1.0) ;
	func_15_GeV->SetParameters(38.6901, 0.941957, 1.289) ;
	// func_15_GeV->SetParameters(1.289, 0.941957, 38.6901) ;
	func_15_GeV->SetRange(0.001, 13.0 * TeV_to_GeV) ; 
	
	double value = func_15_GeV->Eval(15) ;
	cout << "blabla func_15_GeV " << value << endl << endl;


   func->SetNpx(100) ;	
   func_p->SetNpx(100) ;	
   func_m->SetNpx(100) ;	

   func_15_GeV->SetNpx(100) ;	

   func->SetLineColor(kRed) ;	
   func_p->SetLineColor(kMagenta) ;	
   func_m->SetLineColor(kMagenta) ;	

   func_p->SetLineStyle(kDashed) ;
   func_m->SetLineStyle(kDashed) ;

	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle(axis_title.c_str()) ;
	hist_2d->GetYaxis()->SetTitle("#sigma_{tot} (mb)") ;

	hist_2d->GetXaxis()->SetTitleSize(axis_title_size) ;
	hist_2d->GetYaxis()->SetTitleSize(axis_title_size) ;

	graph->Draw("same p") ;
	graph_1p96->Draw("same p") ;

	TGraph *graph_for_plot = NULL ;
	TArrow *myarrow2 = NULL ;
	TArrow *myarrow3 = NULL ;

	if(scenario ==  scenario_prelim_2)
	{
		graph_10_GeV->Draw("same p") ;

		double mylength = 2.0 ;

		const double limit_at_15_GeV = 39.3 ;

		TLine *myline = new TLine(15 -mylength, limit_at_15_GeV, 15 + mylength, limit_at_15_GeV) ;
		myline->SetLineWidth(14) ;

		TArrow *myarrow = new TArrow(15, limit_at_15_GeV, 15, limit_at_15_GeV - 2*mylength,0.01, "|>") ;
		myarrow->SetAngle(40) ;
		myarrow->SetLineWidth(2) ;

		myline->Draw("same") ;
		myarrow->Draw("") ;
		
		TFile *afile = TFile::Open("./fit_test/graph.root") ;
		
		graph_for_plot = ((TGraph *)afile->Get("graph_for_plot")) ;
		graph_for_plot->Draw("same") ;
		
		double anx = 1960 ;
		double avalue = graph_for_plot->Eval(anx) ;
		double avalue_2 = 80.4579 ;

		myarrow2 = new TArrow(1960, avalue, 1960, avalue_2, 0.01, "") ;
		myarrow2->SetAngle(40) ;
		myarrow2->SetLineWidth(14) ;
		myarrow2->SetLineColor(6) ;
		myarrow2->Draw("") ;
		
	   TLatex *latex = new TLatex() ;

	   latex->SetNDC() ;
   	latex->SetTextFont(132) ;
   	latex->SetTextSize(0.02) ;
	   latex->SetTextColor(kRed) ;
	
		latex->DrawLatex(.72, .08, "1.96 TeV") ;

		myarrow3 = new TArrow(1960, avalue, 1960, 30, 0.01, "") ;
		myarrow3->SetAngle(40) ;
		myarrow3->SetLineWidth(5) ;
		myarrow3->SetLineColor(kRed) ;
		myarrow3->SetLineStyle(kDashed) ;
		myarrow3->Draw("") ;
	}
	
	cout << "Test: " << func->Eval(2760) << endl ;

	func->SetRange(1.5 * TeV_to_GeV, 15 * TeV_to_GeV) ;
	func->Draw("same l") ;
	func_15_GeV->Draw("same l") ;

	if(scenario ==  scenario_prelim_2) func->SetRange(1.96 * TeV_to_GeV, 13 * TeV_to_GeV) ;
	// func_p->Draw("same l") ;
	// func_m->Draw("same l") ;
	
	TLegend *legend = new TLegend(0.12, 0.65, 0.5, 0.88) ;
	
	legend->AddEntry(graph, "TOTEM measurements", "p") ;
	legend->AddEntry(graph_1p96, "extrapolation", "p") ;
	if(scenario ==  scenario_prelim_2) legend->AddEntry(graph_10_GeV, "constraint", "pe") ;
	if(scenario ==  scenario_prelim_2) legend->AddEntry(graph_for_plot, "fit", "l") ;
	if(scenario ==  scenario_prelim_2) legend->AddEntry(myarrow2, "uncertainty", "l") ;
	// legend->AddEntry(func, "fit, prelim. uncert!", "l") ;
	
	legend->Draw("same") ;

	mydraw() ;	

	c.SaveAs(myplotname.c_str()) ;
	c.SaveAs(myplotname2.c_str()) ;
	c.SaveAs(myplotname3.c_str()) ;

	delete hist_2d ;
	delete graph ;
	delete graph_1p96 ;
	delete func ;
	delete legend ;
}

int plot_dsdt()
{
	TCanvas c ;
	c.SetLogy() ;

   gStyle->SetLineScalePS(.5) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("");

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0.45, 1.0, 100, 0.005, 0.1) ;	

	hist_2d->SetTitle("") ;
	TGraphErrors *graph = new TGraphErrors ;
	TGraphErrors *graph2 = new TGraphErrors ;

	TGraphErrors *graph_band_up = new TGraphErrors ;
	TGraphErrors *graph_band_down = new TGraphErrors ;

	TGraphErrors *graph_band_up_2 = new TGraphErrors ;
	TGraphErrors *graph_band_down_2 = new TGraphErrors ;

	TGraphAsymmErrors *graph_center = new TGraphAsymmErrors ;
	TGraphAsymmErrors *graph_center2 = new TGraphAsymmErrors ;

	graph->SetMarkerStyle(20) ;
	graph2->SetMarkerStyle(22) ;
	graph2->SetMarkerSize(1.1) ;

	graph->SetMarkerColor(kBlue) ;
	graph2->SetMarkerColor(kRed) ;
	
	graph_center->SetMarkerStyle(22) ;
	graph_center->SetMarkerSize(1.0) ;
	graph_center->SetMarkerColor(kRed) ;

	graph_center2->SetMarkerStyle(22) ;
	graph_center2->SetMarkerSize(1.0) ;
	graph_center2->SetMarkerColor(kRed) ;

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

	ifstream data4("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated_uncertainty_center.txt") ;

	i = 0 ;


	double dsdt_down, dsdt_up, f_norm_down, f_norm_up ;
	double dummy ;

	while(data4 >> t_value >> dsdt >> dsdt_up >> dsdt_down >> dummy >> dummy)
	{
		graph_center->SetPoint(i, t_value, dsdt) ;

		++i ;
	}

	data4.close() ;
	
	ifstream data3("data/TOTEM_D0_14_PRL_preliminary_1_dsdt_extrapolated_uncertainty_band.txt") ;

	i = 0 ;
	int j = 0 ;

	while(data3 >> t_value >> dsdt_down >> dsdt_up >> f_norm_down >> f_norm_up)
	{
		graph_band_up->SetPoint(i, t_value, (dsdt_up + dsdt_down) / 2.0) ;
		graph_band_up->SetPointError(i, 0, fabs((dsdt_up - dsdt_down) / 2.0)) ;

		// graph_band_down->SetPoint(i, t_value, dsdt_down) ;

		graph_band_up_2->SetPoint(i, t_value, f_norm_down) ;
		graph_band_down_2->SetPoint(i, t_value, f_norm_up) ;


		double myx, myy ;

		graph_center->GetPoint(i, myx, myy) ;
		graph_center->SetPointError(i, 0, 0, myy - f_norm_down, f_norm_up - myy) ;

		double myx2, myy2 ;
		graph2->GetPoint(j, myx2, myy2) ;

		if(myx == myx2)
		{
			graph_center2->SetPoint(j, myx, myy) ;
			graph_center2->SetPointError(j, 0, 0, myy - f_norm_down, f_norm_up - myy) ;
			
			++j ;
		}

		++i ;
	}

	data3.close() ;
	
	cout << endl << "Table D0" << endl << endl ;

	for(int i = 0 ; i < graph->GetN() ; ++i)
	{
		double x, y, y_unc ;

		graph->GetPoint(i, x, y) ;
		y_unc = graph->GetErrorY(i) ;

		if((x >= 0.4977680) && (x <= 0.9591980)) cout << std::setprecision(15) << "\t" << x << "   " << y << "\t" << y_unc << endl ;
	}

	cout << endl << "Extrapolation center and its uncertainy" << endl << endl ;

	for(int i = 0 ; i < graph_center2->GetN() ; ++i)
	{
		double x, y, y_unc_h, y_unc_l ;

		graph_center2->GetPoint(i, x, y) ;
		y_unc_h = graph_center2->GetErrorYhigh(i) ;
		y_unc_l = graph_center2->GetErrorYlow(i) ;

		cout << std::setprecision(6) << "\t" <<  x << "   " << y << "\t" << y_unc_h << "\t" << y_unc_l << endl ;
	}

	cout << endl << "Band up" << endl << endl ;

	for(int i = 0 ; i < graph_band_up->GetN() ; ++i)
	{
		double x, y ;

		graph_band_up->GetPoint(i, x, y) ;

		cout << std::setprecision(6) << "\t" <<  x << " \t" << y << endl ;
	}

	cout << endl << "Band down" << endl << endl ;

	for(int i = 0 ; i < graph_band_down->GetN() ; ++i)
	{
		double x, y ;

		graph_band_down->GetPoint(i, x, y) ;

		cout << std::setprecision(6) << "\t" <<  x << " \t" << y << endl ;
	}

	hist_2d->Draw() ;
	hist_2d->GetXaxis()->SetTitle("|t| (GeV^{2})") ;
	hist_2d->GetYaxis()->SetTitle("d#sigma/dt (mb/GeV^{2})") ;

	hist_2d->GetXaxis()->SetTitleSize(axis_title_size) ;
	hist_2d->GetYaxis()->SetTitleSize(axis_title_size) ;
	// cout << "titlesize " << hist_2d->GetXaxis()->GetTitleSize() ;

	// graph2->Draw("same p") ;

	graph_band_up->SetFillColor(18) ;
	graph_band_up->SetFillStyle(1001) ;
	graph_band_up->Draw("same 3") ;
	// graph_band_down->Draw("same l") ;
	
	graph_center2->Draw("same p") ;
	graph_band_up_2->Draw("same l") ;
	graph_band_down_2->Draw("same l") ;
	
	graph_band_up->SetLineColor(kRed) ;
	graph_band_down->SetLineColor(kRed) ;
	graph_band_up->SetLineStyle(kDashed) ;
	graph_band_down->SetLineStyle(kDashed) ;

	graph_band_up_2->SetLineColor(kRed) ;
	graph_band_down_2->SetLineColor(kRed) ;
	graph_band_up_2->SetLineStyle(kDashed) ;
	graph_band_down_2->SetLineStyle(kDashed) ;

	graph->Draw("same p") ;

	TLegend *legend = new TLegend(0.38, 0.65, 0.88, 0.84) ;
	
	legend->AddEntry(graph, "D#oslash p#bar{p} measurement at #sqrt{s}=1.96 TeV", "pe") ;
	legend->AddEntry(graph2, "pp extrapolated", "pe") ;
	legend->AddEntry(graph_band_up, "band width (#pm 1 #sigma)", "l") ;
	
	legend->Draw("same") ;

	mydraw() ;

   c.SaveAs("fig/TOTEM_D0_14_PRL_preliminary_1_dsdt.pdf") ;
}

int main(int argc, char *argv[])
{
	plot_sigtot(scenario_prelim_1) ;
	plot_sigtot(scenario_prelim_2) ;
	plot_dsdt() ;
}
