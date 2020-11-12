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
#include "TF1.h"
#include "TGraph.h"
#include "TObject.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TProfile.h"
#include "TMinuit.h"
#include "TMarker.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TMarker.h"
#include "TFrame.h"
#include "TRandom3.h"
#include "TKey.h"

#include <iomanip>

const double sigma = 50e-6 ;

TLegend *legend = NULL ;
bool legend_complete = false ;

class FillClass
{
	public:
	
	bool present ;
	FillClass() : present(false) {} ;
};

map<int, FillClass> map_of_fills ;

class TAcceptanceFrame
{
	string name ;

	TGraph *upper, *lower ;
	TF1 *mean_fit ;
	bool complete ;

	public:
	
	TAcceptanceFrame(string, TObject *) ;
	void Add(string, TObject *) ;
	void CalculateAcceptance(double, double &, double &, double &, double &, double &, double &) ;
	
	bool IsWithinAperture(int, double, double) ;
} ;

TAcceptanceFrame::TAcceptanceFrame(string name, TObject *anobject) : name(name), upper(NULL), lower(NULL), mean_fit(NULL), complete(false)
{
	// Add object

	Add(name, anobject) ;
}

const double test_xi = 3e-2 ;

void TAcceptanceFrame::Add(string key_name, TObject *anobject)
{
	if(key_name.substr(34,5).compare("upper") == 0)
	{
		upper = (TGraph *)anobject ;
	}
	else if(key_name.substr(34,5).compare("lower") == 0)
	{
		lower = (TGraph *)anobject ;
	}
	else if(key_name.substr(34,5).compare("mean_") == 0)
	{
		mean_fit = (TF1 *)anobject ;
	}
	else
	{
		// cout << "Problematic" << endl ;
	}


	char * pEnd ;
        int fill = strtol(name.substr(8,4).c_str(),      &pEnd, 10) ;
        int arm = strtol(name.substr(0,2).c_str(),       &pEnd, 10) ;
        int xangle = strtol(name.substr(20,3).c_str(),   &pEnd, 10) ;
        double beta = (strtol(name.substr(31,2).c_str(), &pEnd, 10) / 100.0) ;

        // cout << "stamp:" << fill << " " << arm << " " << xangle << " " <<  beta << endl ;
	
	if((upper != NULL) && (lower != NULL) && (mean_fit != NULL) )
	{
		complete = true ;

		double linewidth = 12 + (6 * (xangle - 120.0) / (160.0 - 120.0)) ;
		// cout << "linewidth: " << linewidth << endl ;

		upper->SetLineWidth(1) ;
		lower->SetLineWidth(1) ;
		
                if(key_name.substr(0,2).compare("56") == 0)
		{
			upper->SetLineColor(kBlue) ;
			lower->SetLineColor(kBlue) ;
		}
		else
		{
                       	upper->SetLineColor(kRed) ;
			lower->SetLineColor(kRed) ;
		}

                if(!legend_complete)
                {
                        legend->SetTextFont(132) ;

                        legend->AddEntry(upper, "Fiducial cut(s) Sec45", "l" ) ;
                        legend_complete = true ;
                }

                complete = true ;

		
		//upper->SetLineColor(linewidth) ;
		//lower->SetLineColor(linewidth) ;

		
		if(beta == 0.25)
		{
                        upper->SetLineColor(kGreen) ;
                       	lower->SetLineColor(kGreen) ;
		}

                if(beta == 0.4)
                {        
                        upper->SetLineColor(kMagenta) ;
                        lower->SetLineColor(kMagenta) ;
                }

                int number_of_points_upper = upper->GetN() ;
                double x_max, y_max, x_min, y_min ;

                upper->GetPoint(number_of_points_upper - 1, x_max, y_max) ;
                lower->GetPoint(0, x_min, y_min) ;

                double max = upper->Eval(test_xi) ;
                double min = lower->Eval(test_xi) ;

		bool make_it_green = false ;

                if(max != min)
                {
                        if(x_max < test_xi) make_it_green = true ;

                        if(test_xi < x_min) make_it_green = true ;
                }
                else
                {
                        make_it_green = true ;
                        make_it_green = true ;
                }


		if(make_it_green)
		{
			upper->SetLineColor(kGreen) ;
			lower->SetLineColor(kGreen) ;
		}

		upper->Draw("") ;
		lower->Draw("") ;

		double mean = mean_fit->Eval(test_xi) ;
		double mymin = x_min ;
		double mymax = x_max ;

		int number_of_steps = 100 ;
		const double dxi = 0.3 / number_of_steps ;

		double myxi = 0 ;

		for(int i = 0 ; i < number_of_steps ; ++i)
		{
			if((upper->Eval(myxi) > mean)  && (upper->Eval(myxi + dxi) < mean))
			{
				mymax = myxi ;
			}

			myxi = myxi + dxi ;
		}

		myxi = 0 ;

                for(int i = 0 ; i < number_of_steps ; ++i)
                {
                        if((lower->Eval(myxi) > mean)  && (lower->Eval(myxi + dxi) < mean))
                        {
                                mymin = myxi ;
                        }

                        myxi = myxi + dxi ;
                }


	        TLine *line = new TLine(mymin, mean, mymax, mean) ;
	        line->SetLineWidth(.001) ;
	        line->SetLineColor(kGreen) ;
        	// line->SetLineStyle(kDashed) ;
	        // line->Draw("same") ;


	}
}

void TAcceptanceFrame::CalculateAcceptance(double xi, double &visible_distance_upper, double &visible_distance_lower, double &visible_distance_upper_sigma, double &visible_distance_lower_sigma, double &visible_distance_sigma, double &correction)
{
	if(complete)
	{
		int number_of_points_upper = upper->GetN() ;
		double x_max, y_max, x_min, y_min ;

		upper->GetPoint(number_of_points_upper - 1, x_max, y_max) ;
		lower->GetPoint(0, x_min, y_min) ;

		double max = upper->Eval(xi) ;
		double min = lower->Eval(xi) ;
		
		if(max != min)
		{
			if(x_max < xi) visible_distance_upper = 0 ;
			else visible_distance_upper = (upper->Eval(xi) - mean_fit->Eval(xi)) ;

                	if(xi < x_min) visible_distance_lower = 0 ;
	                else visible_distance_lower = (lower->Eval(xi) - mean_fit->Eval(xi)) ;
		}
		else
		{
			visible_distance_upper =  0 ;
			visible_distance_lower =  0 ;
		}


		if(visible_distance_upper > 0.0)
		{
			visible_distance_upper_sigma = TMath::Erf(visible_distance_upper / (sigma * sqrt(2.0))) / 2.0 ;
		}
		else
		{
			visible_distance_upper_sigma = -TMath::Erf(-visible_distance_upper / (sigma * sqrt(2.0))) / 2.0 ;
		}

		if(visible_distance_lower <  0.0)
		{
			visible_distance_lower_sigma = TMath::Erf(-visible_distance_lower / (sigma * sqrt(2.0))) / 2.0 ;
		}
		else
		{
			visible_distance_lower_sigma = -TMath::Erf(visible_distance_lower / (sigma * sqrt(2.0))) / 2.0 ;
		}

		visible_distance_sigma = visible_distance_upper_sigma + visible_distance_lower_sigma ;
		correction = (1.0 / visible_distance_sigma) ;
	}
	else
	{
		// cout << "Not_complete!" << endl ;
	}
}


bool TAcceptanceFrame::IsWithinAperture(int fill, double xi, double theta_x_star)
{
}

map<string, TCanvas *> graphs ;

void Draw(string graphname_1)
{
        gStyle->SetFillColor(kWhite) ;
        gStyle->SetFrameFillColor(kWhite) ;
        gStyle->SetTitleFillColor(kWhite) ;
        gStyle->SetStatColor(kWhite) ;
        gStyle->SetPadBorderMode(0) ;
        gStyle->SetFrameBorderMode(0) ;

        graphs[graphname_1.c_str()]->SetFillColor(kWhite) ;
        graphs[graphname_1.c_str()]->SetFrameFillColor(kWhite) ;
        graphs[graphname_1.c_str()]->SetFrameBorderMode(0) ;

	graphs[graphname_1.c_str()]->Draw("") ;
	graphs[graphname_1.c_str()]->cd() ;

        TLatex *latex = new TLatex ;
        latex->SetNDC() ;
        latex->SetTextFont(132) ;
        latex->SetTextSize(4e-2) ;
        latex->DrawLatex(.35, .92, graphname_1.c_str()) ;
	latex->Draw("") ;

        graphs[graphname_1.c_str()]->SaveAs(("Plots/" + graphname_1 + ".root").c_str()) ;
        graphs[graphname_1.c_str()]->SaveAs(("Plots/" + graphname_1 + ".pdf").c_str()) ;
}

void create_graphs(string name, double low, double high)
{
	graphs[name] = new TCanvas(name.c_str()) ;
	graphs[name]->cd() ;
	graphs[name]->SetGrid() ;

        gStyle->SetFillColor(kWhite) ;
        gStyle->SetFrameFillColor(kWhite) ;
        gStyle->SetTitleFillColor(kWhite) ;
        gStyle->SetStatColor(kWhite) ;
        gStyle->SetPadBorderMode(0) ;
        gStyle->SetFrameBorderMode(0) ;

	gStyle->SetOptStat("");
	graphs[name]->SetTitle("") ;

	TH2D *frame = new TH2D(name.c_str(), name.c_str(), 100, 5500, 7600, 100, low, high) ;
        frame->SetFillColor(kWhite) ;

	frame->SetTitle("") ;

	frame->GetXaxis()->SetAxisColor(16);
	frame->GetYaxis()->SetAxisColor(16);	

        frame->GetXaxis()->SetTitle("Fill");
        frame->GetXaxis()->SetTitleFont(132);
        frame->GetXaxis()->SetLabelFont(132);

        frame->GetYaxis()->SetTitle("#theta_{x}* [rad]");
        frame->GetYaxis()->SetTitleFont(132);
        frame->GetYaxis()->SetLabelFont(132);

	graphs[name]->RedrawAxis() ;
	frame->Draw() ;


	// frame->LabelsDeflate("Y");
	// frame->LabelsOption("v") ;
	
}

bool acceptance_calculation = false ;
bool event_check = false ;
bool all_fills = true ;

int process_1(int argc, char *argv[])
{


	map<string, TAcceptanceFrame *> frames ;
	gStyle->SetOptStat("");
	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);

        gStyle->SetFillColor(kWhite) ;
        gStyle->SetFrameFillColor(kWhite) ;
        gStyle->SetTitleFillColor(kWhite) ;
        gStyle->SetStatColor(kWhite) ;
        gStyle->SetPadBorderMode(0) ;
        gStyle->SetFrameBorderMode(0) ;


	TFile *file = new TFile("Acceptance_frames/generic.root");
	TObject *obj;
	TKey *key;
	TIter next( file->GetListOfKeys());
	
	int index = 0 ;
	TCanvas *c = new TCanvas ;
	
	TH2D *frame = new TH2D("frame", "frame", 100, 0, 0.3, 100, -0.3e-3, 0.3e-3) ;
	frame->SetTitle("") ;

	frame->GetXaxis()->SetTitleFont(132) ;
	frame->GetYaxis()->SetTitleFont(132) ;

        frame->GetXaxis()->SetLabelFont(132) ;
        frame->GetYaxis()->SetLabelFont(132) ;

	frame->GetXaxis()->SetTitle("#xi") ;
	frame->GetYaxis()->SetTitle("#theta_{x}* [rad]") ;


	frame->Draw("") ;

	legend = new TLegend(.6,.7,.85,.85, 0) ;

	// legend->SetHeader(NULL) ;
	legend->SetTextFont(132) ;
	legend->SetFillColor(kWhite) ;


	legend->Draw("") ;

	// cout << "hello" << endl ;

	
	while ((key = (TKey *) next())) {
		obj = file->Get(key->GetName()); // copy object to memory

		string key_name = key->GetName() ;
		string main_key = key_name.substr(0, 33) ;

		if(frames[main_key] == NULL)
		{
			frames[main_key] = new TAcceptanceFrame(key_name, obj) ;
			// cout << "hi" ;
		}
		else
		{
			frames[main_key]->Add(key_name, obj) ;
		}

		// printf(" found object:%s\n",key->GetName());
		++index ;
	}

	c->SetFillColor(kWhite) ;

	TLine *line = new TLine(test_xi, -3e-4, test_xi, 3e-4) ;
	line->SetLineColor(kMagenta) ;
	line->SetLineStyle(kDashed) ;
	line->Draw("same") ;

        c->SaveAs("Plots/summary.root") ;
        c->SaveAs("Plots/summary.png") ;
        c->SaveAs("Plots/summary.pdf") ;

	create_graphs("visible_distance_upper", -300, 300);
        create_graphs("visible_distance_lower", -300, 300);

	// Sigmas

        create_graphs("visible_distance_upper_sigma", -1.0, 1.0);
        create_graphs("visible_distance_lower_sigma", -1.0, 1.0);

	// Final values

        create_graphs("visible_distance_sigma", -2.0, 2.0);

        create_graphs("correction", 0, 5.0);

	const int num_points = 100 ;
	const double xi_upper_boundary = 0.25 ;
	const double dxi = (xi_upper_boundary / num_points) ;

	TGraph sigmas_45, corrections_45 ;
	TGraph sigmas_56, corrections_56 ;
	
	int index_45 = 0 ;
	int index_56 = 0 ;

	for(int i = 0 ; i < num_points ; ++i)
	{
                double xi = (((double)i) * dxi) ;
                double visible_distance_sigma_sum_45 = 0 ;
                double visible_distance_sigma_sum_56 = 0 ;
		int number_of_frames_processed_45 = 0 ;
		int number_of_frames_processed_56 = 0 ;

	for(map<string, TAcceptanceFrame *>::iterator it = frames.begin() ; it != frames.end() ; ++it)
	{

                double visible_distance_upper = 0 ;
                double visible_distance_lower = 0 ;

                double visible_distance_upper_sigma = 0 ;
                double visible_distance_lower_sigma = 0 ;

                double visible_distance_sigma = 0 ;
                double correction = 0 ;

		char * pEnd ;
		int fill = strtol((it->first).substr(8,4).c_str(), 	&pEnd, 10) ;
		int arm = strtol((it->first).substr(0,2).c_str(), 	&pEnd, 10) ;
		int xangle = strtol((it->first).substr(20,3).c_str(), 	&pEnd, 10) ;
		double beta = (strtol((it->first).substr(31,2).c_str(),	&pEnd, 10) / 100.0) ;
	
		if(!all_fills && map_of_fills[fill].present == false) continue ;
		// cout << "Fill: " << fill << endl ;	

		double markersize = 0.2 + (0.8 * (xangle - 120.0) / (160.0 - 120.0)) ;
		
		it->second->CalculateAcceptance(xi, visible_distance_upper, visible_distance_lower, visible_distance_upper_sigma, visible_distance_lower_sigma, visible_distance_sigma, correction) ;
		if(arm == 45) 
		{
			visible_distance_sigma_sum_45 += visible_distance_sigma ;
			number_of_frames_processed_45++ ;
		} else if(arm == 56)
		{
			visible_distance_sigma_sum_56 += visible_distance_sigma ;
			number_of_frames_processed_56++ ;
		}

		// cout << "vd: " << visible_distance_upper << endl ;
		
		if((fill > 1000) && (xi == 7e-2))
		{
			if(arm == 45)
			{
				string graph_name = "visible_distance_upper" ;
				graphs[graph_name]->cd() ;
				TMarker *marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_upper * 1e6)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_lower" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_lower * 1e6)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_upper_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_upper_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_lower_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_lower_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "correction" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kRed) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((correction)) ;
				marker->Draw("same") ;

			}
			else if(arm == 56)
			{
                                string graph_name = "visible_distance_upper" ;
                                graphs[graph_name]->cd() ;
				TMarker *marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_upper * 1e6)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_lower" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_lower * 1e6)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_upper_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_upper_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_lower_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_lower_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "visible_distance_sigma" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((visible_distance_sigma)) ;
				marker->Draw("same") ;

                                graph_name = "correction" ;
                                graphs[graph_name]->cd() ;
				marker = new TMarker() ;
				marker->SetMarkerStyle(20) ;
				marker->SetMarkerColor(kBlue) ;
				marker->SetMarkerSize(markersize) ;
				marker->SetX(fill) ;
				marker->SetY((correction)) ;
				marker->Draw("same") ;

			}
			else
			{
				// cout << "Problem" << endl ;
			}
			
		}
		
	}
		double average_sigma_45 = (visible_distance_sigma_sum_45 / number_of_frames_processed_45) ;
		sigmas_45.SetPoint(i, xi, average_sigma_45) ;

		if((xi >= 0.01) && (xi <= 0.15))
		{
			corrections_45.SetPoint(index_45, xi, 1/average_sigma_45) ;
			index_45++;
		}

                double average_sigma_56 = (visible_distance_sigma_sum_56 / number_of_frames_processed_56) ;
                sigmas_56.SetPoint(i, xi, average_sigma_56) ;

                if((xi >= 0.01) && (xi <= 0.2))
		{
			index_56++;
		
			corrections_56.SetPoint(index_56, xi, 1/average_sigma_56) ;
		}

	}
	
	Draw("visible_distance_upper") ;
	Draw("visible_distance_lower") ;

        Draw("visible_distance_upper_sigma") ;
        Draw("visible_distance_lower_sigma") ;

        Draw("visible_distance_sigma") ;
        Draw("correction") ;

	c->cd() ;

	// 45

	sigmas_45.GetXaxis()->SetTitle("#xi") ;
	sigmas_45.GetYaxis()->SetTitle("Acceptance") ;

	corrections_45.GetXaxis()->SetTitle("#xi") ;
	corrections_45.GetYaxis()->SetTitle("Correction") ;

	sigmas_45.Draw("alp") ;
	
	c->SaveAs("Plots/sigmas_45.root") ;
	c->SaveAs("Plots/sigmas_45.pdf") ;

        corrections_45.Draw("alp") ;
        c->SaveAs("Plots/corrections_45.root") ;
        c->SaveAs("Plots/corrections_45.pdf") ;

        corrections_45.SetName("corrections_45") ;
        corrections_56.SetName("corrections_56") ;
        corrections_45.SaveAs("Acceptance_corrections/corrections_45.root") ;
        corrections_56.SaveAs("Acceptance_corrections/corrections_56.root") ;

	// 56

        sigmas_56.GetXaxis()->SetTitle("#xi") ;
        sigmas_56.GetYaxis()->SetTitle("Acceptance") ;

        corrections_56.GetXaxis()->SetTitle("#xi") ;
        corrections_56.GetYaxis()->SetTitle("Correction") ;

        sigmas_56.Draw("alp") ;

        c->SaveAs("Plots/sigmas_56.root") ;
        c->SaveAs("Plots/sigmas_56.pdf") ;

        corrections_56.Draw("alp") ;
        c->SaveAs("Plots/corrections_56.root") ;
        c->SaveAs("Plots/corrections_56.pdf") ;

	// Mass
	
	TRandom3 rand ;
	TH1D *Mx_hist =  new TH1D("Mx_hist", "Mx_hist", 400, 0, 2.5) ;
	TH1D *Mx_hist_for_norm =  new TH1D("Mx_hist", "Mx_hist", 400, 0, 2.5) ;
	
	for(int i = 0 ; i < 1e7 ; ++i)
	{
		double xi_1 = rand.Uniform() * .2 ;
		double xi_2 = rand.Uniform() * .2 ;

		if((xi_1 < 4e-2) || (xi_1 > 15e-2)) continue ;
		if((xi_2 < 3e-2) || (xi_2 > 20e-2)) continue ;

		double p1 = sigmas_45.Eval(xi_1) ;
		double p2 = sigmas_56.Eval(xi_2) ;

		double Mx = 13.0 * sqrt(xi_1*xi_2) ;

		Mx_hist->Fill(Mx, p1*p2) ;
		Mx_hist_for_norm->Fill(Mx) ;

	}

	Mx_hist->Divide(Mx_hist_for_norm) ;

	Mx_hist->Draw("hist") ;
	c->SaveAs("Plots/Mx_hist.root") ;
	c->SaveAs("Plots/Mx_hist.pdf") ;

        Mx_hist_for_norm->Draw("hist") ;
        c->SaveAs("Plots/Mx_hist_for_norm.root") ;
        c->SaveAs("Plots/Mx_hist_for_norm.pdf") ;
}
	

int process_2(int argc, char *argv[])
{
	map<string, TAcceptanceFrame *> frames ;


	TFile *file = new TFile("Acceptance_frames/generic.root");
	TObject *obj;
	TKey *key;
	TIter next( file->GetListOfKeys());
	
	int index = 0 ;
	
	while ((key = (TKey *) next())) {
		obj = file->Get(key->GetName()); // copy object to memory

		string key_name = key->GetName() ;
		string main_key = key_name.substr(0, 33) ;

		if(frames[main_key] == NULL)
		{
			frames[main_key] = new TAcceptanceFrame(key_name, obj) ;
			// cout << "hi" ;
		}
		else
		{
			frames[main_key]->Add(key_name, obj) ;
		}

		// printf(" found object:%s\n",key->GetName());
		++index ;
	}

}


int main(int argc, char *argv[])
{
	/*
	if(argc == 2)
	{
		all_fills = false ;
	
		ifstream fills(argv[1]) ;
		
		string word ;

		while(fills >> word)
		{
			char *pEnd ;
			int fill = strtol(word.c_str(),	&pEnd, 10) ;
			
			map_of_fills[fill].present = true ;
		}
	}*/
	
	if(argc == 2)
	{
		event_check = false ;
	
		ifstream fills(argv[1]) ;
		
		string word ;

		while(fills >> word)
		{
			char *pEnd ;
			int fill = strtol(word.c_str(),	&pEnd, 10) ;
			
			map_of_fills[fill].present = true ;
		}
	}


	if(acceptance_calculation) process_1(argc, argv) ;
	if(event_check) process_2(argc, argv) ;
}
