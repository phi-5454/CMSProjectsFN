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
#include "TKey.h"

#include <iomanip>

const double sigma = 50e-6 ;

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
		int number_of_points_upper = upper->GetN() ;
		double x, y ;
		upper->GetPoint(number_of_points_upper - 1, x, y) ;
		
		if(x < xi) visible_distance_upper = 0 ;
		else visible_distance_upper = (upper->Eval(xi) - mean_fit->Eval(xi)) ;


		visible_distance_lower = (lower->Eval(xi) - mean_fit->Eval(xi)) ;

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

map<string, TCanvas *> graphs ;

void Draw(string graphname_1)
{
        gStyle->SetTitleFillColor(kWhite) ;
        gStyle->SetStatColor(kWhite) ;
        gStyle->SetPadBorderMode(0) ;
        gStyle->SetFrameBorderMode(0) ;

	graphs[graphname_1.c_str()]->Draw("") ;

        graphs[graphname_1.c_str()]->SetFillColor(kWhite) ;
        graphs[graphname_1.c_str()]->SetFrameBorderMode(0) ;

        graphs[graphname_1.c_str()]->SaveAs((graphname_1 + ".root").c_str()) ;
        graphs[graphname_1.c_str()]->SaveAs((graphname_1 + ".pdf").c_str()) ;



}

void create_graphs(string name, double low, double high)
{
	graphs[name] = new TCanvas(name.c_str()) ;
	graphs[name]->cd() ;

	TH2D *frame = new TH2D(name.c_str(), name.c_str(), 100, 5500, 7600, 100, low, high) ;
	frame->Draw("") ;
	frame->LabelsDeflate("Y");
	frame->LabelsOption("v") ;
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
		string main_key = key_name.substr(0, 33) ;

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
	

	create_graphs("visible_distance_upper", -300, 300);
        create_graphs("visible_distance_lower", -300, 300);

	// Sigmas

        create_graphs("visible_distance_upper_sigma", 0, 1.0);
        create_graphs("visible_distance_lower_sigma", 0, 1.0);

	// Final values

        create_graphs("visible_distance_sigma", 0, 5.0);

        create_graphs("correction", 0, 1.0);

	for(map<string, TAcceptanceFrame *>::iterator it = frames.begin() ; it != frames.end() ; ++it)
	{

                double visible_distance_upper = 0 ;
                double visible_distance_lower = 0 ;

                double visible_distance_upper_sigma = 0 ;
                double visible_distance_lower_sigma = 0 ;

                double visible_distance_sigma = 0 ;
                double correction = 0 ;

		it->second->CalculateAcceptance(12e-2, visible_distance_upper, visible_distance_lower, visible_distance_upper_sigma, visible_distance_lower_sigma, visible_distance_sigma, correction) ;
		cout << "vd: " << visible_distance_upper << endl ;
		
		if(visible_distance_lower > 200e-6)
		{
			cout << "visible_distance_lower > 200 murad" << endl ;
			// exit(1) ;
		}

		char * pEnd ;
		int fill = strtol((it->first).substr(8,4).c_str(), 	&pEnd, 10) ;
		int arm = strtol((it->first).substr(0,2).c_str(), 	&pEnd, 10) ;
		int xangle = strtol((it->first).substr(20,3).c_str(), 	&pEnd, 10) ;
		
		cout << "Fill,arm,xangle: " << (it->first).substr(8,4) << " " << fill << " " << arm << " " << xangle << endl ;

		double markersize = 0.2 + (0.8 * (xangle - 120.0) / (160.0 - 120.0)) ;
		
		if(fill > 1000)
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
				cout << "Problem" << endl ;
			}
			
		}
		
	}
	
	Draw("visible_distance_upper") ;
	Draw("visible_distance_lower") ;

        Draw("visible_distance_upper_sigma") ;
        Draw("visible_distance_lower_sigma") ;

        Draw("correction") ;

}
	
