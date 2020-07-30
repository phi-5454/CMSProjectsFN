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
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TMarker.h"
#include "TKey.h"

#include <iomanip>

const double sigma  = 50e-6 ;

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
} ;

TAcceptanceFrame::TAcceptanceFrame(string name, TObject *anobject) : name(name), upper(NULL), lower(NULL), mean_fit(NULL), complete(false)
{
	Add(name, anobject) ;
}

void TAcceptanceFrame::Add(string key_name, TObject *anobject)
{
	if(key_name.substr(13,5).compare("upper") == 0)
	{
		upper = (TGraph *)anobject ;
	}
	else if(key_name.substr(13,5).compare("lower") == 0)
	{
		lower = (TGraph *)anobject ;
	}
	else if(key_name.substr(13,5).compare("mean_") == 0)
	{
		mean_fit = (TF1 *)anobject ;
	}
	else
	{
		cout << "Problematic" << endl ;
	}
	
	if((upper != NULL) && (lower != NULL) && (mean_fit != NULL))
	{
		complete = true ;
		cout << "Complete" << endl ;

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

		upper->Draw("") ;
		lower->Draw("") ;
		mean_fit->Draw("same") ;

	}
}

void TAcceptanceFrame::CalculateAcceptance(double xi, double &visible_distance_upper, double &visible_distance_lower, double &visible_distance_upper_sigma, double &visible_distance_lower_sigma, double &visible_distance_sigma, double &correction)
{
	if(complete)
	{
		visible_distance_upper = fabs(mean_fit->Eval(xi) - upper->Eval(xi)) ;
		visible_distance_lower = fabs(mean_fit->Eval(xi) - lower->Eval(xi)) ;

		visible_distance_upper_sigma = TMath::Erf(visible_distance_upper / (sigma * sqrt(2.0))) / 2.0 ;
		visible_distance_lower_sigma = TMath::Erf(visible_distance_lower / (sigma * sqrt(2.0))) / 2.0 ;

		visible_distance_sigma = visible_distance_upper_sigma + visible_distance_lower_sigma ;
		correction = (1.0 / visible_distance_sigma) ;
	}
	else
	{
		cout << "Not_complete!" << endl ;
	}
}

map<string, TGraph *> graphs ;

void Draw(string graphname_1, string graphname_2, string output_filename, TCanvas *c, double min, double max)
{
        graphs[graphname_1.c_str()]->SetMarkerStyle(20) ;
        graphs[graphname_2.c_str()]->SetMarkerStyle(20) ;

        graphs[graphname_1.c_str()]->SetMarkerColor(kRed) ;
        graphs[graphname_2.c_str()]->SetMarkerColor(kBlue) ;

        graphs[graphname_1.c_str()]->Draw("ap") ;
        graphs[graphname_2.c_str()]->Draw("same p") ;

        graphs[graphname_1.c_str()]->GetYaxis()->SetRangeUser(min, max) ;

        gStyle->SetTitleFillColor(kWhite) ;
        gStyle->SetStatColor(kWhite) ;
        gStyle->SetPadBorderMode(0) ;
        gStyle->SetFrameBorderMode(0) ;

        c->SetFillColor(kWhite) ;
        c->SetFrameBorderMode(0) ;


        graphs[graphname_1.c_str()]->GetXaxis()->SetTitle("Fill number") ;
        graphs[graphname_1.c_str()]->GetYaxis()->SetTitle("Visible #theta_{x}^{*} [#murad]") ;
        graphs[graphname_1.c_str()]->GetYaxis()->SetTitleOffset(1.1) ;

        graphs[graphname_1.c_str()]->GetXaxis()->SetTitleFont(132) ;
        graphs[graphname_1.c_str()]->GetYaxis()->SetTitleFont(132) ;

        graphs[graphname_1.c_str()]->GetXaxis()->SetLabelFont(132) ;
        graphs[graphname_1.c_str()]->GetYaxis()->SetLabelFont(132) ;

        c->SaveAs(output_filename.c_str()) ;

}

int main()
{
	map<string, TAcceptanceFrame *> frames ;

	TFile *file = new TFile("result/generic.root");
	TObject *obj;
	TKey *key;
	TIter next( file->GetListOfKeys());
	
	int index = 0 ;
	TCanvas *c = new TCanvas ;
	
	TH2D *frame = new TH2D("frame", "frame", 100, 0, 0.3, 100, -0.3e-3, 0.3e-3) ;
	frame->Draw("") ;
	
	while ((key = (TKey *) next())) {
		obj = file->Get(key->GetName()); // copy object to memory

		string key_name = key->GetName() ;
		string main_key = key_name.substr(0, 12) ;

		if(frames[main_key] == NULL)
		{
			frames[main_key] = new TAcceptanceFrame(key_name, obj) ;
			cout << "hi" ;
		}
		else
		{
			frames[main_key]->Add(key_name, obj) ;
		}

		printf(" found object:%s\n",key->GetName());
		++index ;
	}

        c->SaveAs("summary.root") ;
        c->SaveAs("summary.png") ;
	

	graphs["visible_distance_upper_45"] = new TGraph() ;
	graphs["visible_distance_upper_56"] = new TGraph() ;

        graphs["visible_distance_lower_45"] = new TGraph() ;
        graphs["visible_distance_lower_56"] = new TGraph() ;

	// Sigmas

        graphs["visible_distance_upper_sigma_45"] = new TGraph() ;
        graphs["visible_distance_upper_sigma_56"] = new TGraph() ;

        graphs["visible_distance_lower_sigma_45"] = new TGraph() ;
        graphs["visible_distance_lower_sigma_56"] = new TGraph() ;

	// Final values

        graphs["visible_distance_sigma_45"] = new TGraph() ;
        graphs["visible_distance_sigma_56"] = new TGraph() ;

        graphs["correction_45"] = new TGraph() ;
        graphs["correction_56"] = new TGraph() ;

	for(map<string, TAcceptanceFrame *>::iterator it = frames.begin() ; it != frames.end() ; ++it)
	{

                double visible_distance_upper = 0 ;
                double visible_distance_lower = 0 ;

                double visible_distance_upper_sigma = 0 ;
                double visible_distance_lower_sigma = 0 ;

                double visible_distance_sigma = 0 ;
                double correction = 0 ;

		it->second->CalculateAcceptance(15e-2, visible_distance_upper, visible_distance_lower, visible_distance_upper_sigma, visible_distance_lower_sigma, visible_distance_sigma, correction) ;
		cout << "vd: " << visible_distance_upper << endl ;

		char * pEnd ;
		int fill = strtol((it->first).substr(8,4).c_str(), &pEnd, 10) ;
		int arm = strtol((it->first).substr(0,2).c_str(), &pEnd, 10) ;
		
		cout << "Fill: " << (it->first).substr(8,4) << " " << fill << endl ;
		
		if(fill > 1000)
		{
			if(arm == 45)
			{
				string graph_name = "visible_distance_upper_45" ;
				int index = graphs[graph_name.c_str()]->GetN() ;
				graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_upper * 1e6)) ;

                                graph_name = "visible_distance_lower_45" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_lower * 1e6)) ;

                                graph_name = "visible_distance_upper_sigma_45" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_upper_sigma)) ;

                                graph_name = "visible_distance_lower_sigma_45" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_lower_sigma)) ;

                                graph_name = "visible_distance_sigma_45" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_sigma)) ;

                                graph_name = "correction_45" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (correction)) ;


			}
			else if(arm == 56)
			{
                                string graph_name = "visible_distance_upper_56" ;
                                int index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_upper * 1e6)) ;

                                graph_name = "visible_distance_lower_56" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_lower * 1e6)) ;

                                graph_name = "visible_distance_upper_sigma_56" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_upper_sigma)) ;

                                graph_name = "visible_distance_lower_sigma_56" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_lower_sigma)) ;

                                graph_name = "visible_distance_sigma_56" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (visible_distance_sigma)) ;

                                graph_name = "correction_56" ;
                                index = graphs[graph_name.c_str()]->GetN() ;
                                graphs[graph_name.c_str()]->SetPoint(index, fill, (correction)) ;

			}
			else
			{
				cout << "Problem" << endl ;
			}
			
		}
		
	}
	
	Draw("visible_distance_upper_45", "visible_distance_upper_56", "visible_distance_upper.pdf", c, 0, 300) ;
	Draw("visible_distance_lower_45", "visible_distance_lower_56", "visible_distance_lower.pdf", c, 0, 300) ;

        Draw("visible_distance_upper_sigma_45", "visible_distance_upper_sigma_56", "visible_distance_upper_sigma.pdf", c, 0, 1.0) ;
        Draw("visible_distance_lower_sigma_45", "visible_distance_lower_sigma_56", "visible_distance_lower_sigma.pdf", c, 0, 1.0) ;

        Draw("correction_45", "correction_56", "correction.pdf", c, 0, 5.0) ;

}
	
