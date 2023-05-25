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
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"
#include "TFitResult.h"

#include <iomanip>

string subdir_per_optics = "100_m" ;

string diagonal_45t_56b_str = "/diagonal_45t_56b/" ;
string diagonal_45b_56t_str = "/diagonal_45b_56t/" ;

const int diagonal_45b_56t = 1 ;
const int diagonal_45t_56b = 2 ;

class TRecord
{
	string name ;

	TGraph 	 *graph ;
	TH2D 	 *histogram ;
	TProfile *profile ;
	
	double x_low, x_high ;
	double angle ;
	double angle_uncertainty ;

	double residual_angle ;
	double residual_angle_uncertainty ;

	public:

	TRecord(string, TFile *, string, double, double) ;

	void fillHistogram(double) ;
	void createHistogram_and_Profile() ;
	void saveHistogram_and_Profile(TCanvas *, string, int) ;
	void fitProfile() ;
};

TRecord::TRecord(string name, TFile *root_file, string graph_name, double x_low, double x_high): name(name), x_low(x_low), x_high(x_high), angle(0), angle_uncertainty(0)
{
	graph = (TGraph *)root_file->Get(graph_name.c_str()) ;
	
	createHistogram_and_Profile() ;
}

void TRecord::fillHistogram(double angle)
{
	profile->Reset() ;
	histogram->Reset() ;

	for(int i = 0 ; i < graph->GetN() ; ++i)
	{
		double x, y ;
		
		graph->GetPoint(i, x, y) ;

		double x_rot = (cos(angle) * x) - (sin(angle) * y) ; 
		double y_rot = (sin(angle) * x) + (cos(angle) * y) ; 

		histogram->Fill(x_rot,y_rot) ;
		profile->Fill(x_rot,y_rot) ;
	}
	
	string xaxis = graph->GetXaxis()->GetTitle() ;
	string yaxis = graph->GetYaxis()->GetTitle() ;

	histogram->GetXaxis()->SetTitle(xaxis.c_str()) ;
	histogram->GetYaxis()->SetTitle(yaxis.c_str()) ;

}

void TRecord::createHistogram_and_Profile()
{
	string histogram_name = name + "_histogram" ;
	string profile_name = name + "_profile" ;

	histogram = new TH2D(histogram_name.c_str(), histogram_name.c_str(), 100, 100, 100, 100, 100, 100) ;
	profile   = new TProfile(profile_name.c_str(), profile_name.c_str(), 100, 100, 100) ;


	fillHistogram(0) ;
}

void TRecord::saveHistogram_and_Profile(TCanvas *c, string text, int diagonal)
{
	string subdir_per_optics_local = subdir_per_optics ;

	if(diagonal == diagonal_45b_56t) subdir_per_optics_local += diagonal_45b_56t_str ;
	else if(diagonal == diagonal_45t_56b) subdir_per_optics_local += diagonal_45t_56b_str ;



	histogram->SaveAs(("plots/" + subdir_per_optics_local +  name + text + "_histogram.root").c_str()) ;

	c->cd() ;
	c->SetFillColor(kWhite);
   	c->SetFrameBorderMode(0) ;   	

	profile->Draw("") ;
	
	if(text.compare("") == 0)
	{
	
		TLatex *latex = new TLatex() ;
		latex->SetNDC() ;

		stringstream angle_string ;
		stringstream angle_uncertainty_string ;
		angle_string << residual_angle ;
		angle_uncertainty_string << residual_angle_uncertainty ;

		stringstream constraint_string ;
		stringstream constraint_uncertainty_string ;
		constraint_string << tan(angle) ;
		constraint_uncertainty_string << 0 ;
		
		cout << "printout " << subdir_per_optics_local << "  " << constraint_string.str() << endl ;

		latex->SetTextColor(kRed) ;
		latex->SetTextSize(0.06) ;
		latex->DrawLatex(0.14, 0.75, ("Constraint tan(#alpha) = " + constraint_string.str() + " #pm " + constraint_uncertainty_string.str()).c_str()) ;
		latex->SetTextColor(kBlack) ;
		
		if(fabs(residual_angle) > fabs(residual_angle_uncertainty)) latex->SetTextColor(kRed) ;
		else latex->SetTextColor(kBlue) ;
		latex->DrawLatex(0.14, 0.65, ("After rotation #alpha flatness = " + angle_string.str() + " #pm " + angle_uncertainty_string.str()).c_str()) ;
	}
	
	c->SaveAs(("plots/" + subdir_per_optics_local +  name + text + "_profile.pdf").c_str()) ;

	histogram->Draw("colz") ;
	c->SaveAs(("plots/" + subdir_per_optics_local +  name + text + "_histogram.pdf").c_str()) ;

	profile->Draw("same") ;

	if(text.compare("") == 0)
	{
	
		TLatex *latex = new TLatex() ;
		latex->SetNDC() ;

		stringstream angle_string ;
		stringstream angle_uncertainty_string ;
		angle_string << residual_angle ;
		angle_uncertainty_string << residual_angle_uncertainty ;

		stringstream constraint_string ;
		stringstream constraint_uncertainty_string ;
		constraint_string << tan(angle) ;
		constraint_uncertainty_string << 0 ;

		latex->SetTextSize(0.06) ;
		latex->DrawLatex(0.14, 0.75, ("Constraint tan(#alpha) = " + constraint_string.str() + " #pm " + constraint_uncertainty_string.str()).c_str()) ;
		
		if(fabs(residual_angle) > fabs(residual_angle_uncertainty)) latex->SetTextColor(kRed) ;
		else latex->SetTextColor(kBlue) ;
		latex->DrawLatex(0.14, 0.65, ("After rotation #alpha flatness = " + angle_string.str() + " #pm " + angle_uncertainty_string.str()).c_str()) ;
	}
		
	c->SaveAs(("plots/" + subdir_per_optics_local +  name + text + "_histogram_with_profile.pdf").c_str()) ;
	c->SaveAs(("plots/" + subdir_per_optics_local +  name + text + "_histogram_with_profile.root").c_str()) ;
}

void TRecord::fitProfile()
{

	cout << "Name: " << name << endl ;

	// ########### Fit 1

	TFitResultPtr fitresult = profile->Fit("pol1", "S", "", x_low, x_high) ;
	
	double pol1  = fitresult->Parameter(1) ;
	angle_uncertainty = fitresult->ParError(1) ;

	angle = atan(pol1) ;

	// #################
	
	cout << "Result: " << fitresult->Parameter(1) << endl ;
	cout << "Angle: " << atan(pol1) << endl ;
	
	fillHistogram(-angle);

	// ########### Fit 2

	fitresult = profile->Fit("pol1", "S", "", x_low, x_high) ;
	
	pol1  = fitresult->Parameter(1) ;
	residual_angle_uncertainty = fitresult->ParError(1) ;

	residual_angle = atan(pol1) ;

	// ########### Fit 3 if necessary

	if(fabs(pol1) > fabs(residual_angle_uncertainty))
	{
		angle = angle + residual_angle ;
	
		fillHistogram(-angle);

		fitresult = profile->Fit("pol1", "S", "", x_low, x_high) ;

		pol1  = fitresult->Parameter(1) ;
		residual_angle_uncertainty = fitresult->ParError(1) ;

		residual_angle = atan(pol1) ;
	}

	// #################
	
	cout << "Result2: " << fitresult->Parameter(1) << endl ;
	cout << "Angle2: " << atan(pol1) << endl ;
	cout << name << " Final_angle: " << tan(angle) << "  " << angle_uncertainty << "  " << (angle_uncertainty / tan(angle)) << endl ;
	cout << name << " Fit_quality: " << fitresult->Chi2() << " " << fitresult->Ndf() << " " << (fitresult->Chi2() / fitresult->Ndf()) << " " << (TMath::Prob(fitresult->Chi2(), fitresult->Ndf())) << endl ;
	
	profile->GetFunction("pol1")->SetLineColor(kRed) ;

	profile->GetFunction("pol1")->SetLineColor(kRed) ;

	cout << endl ;
	cout << endl ;
	cout << endl ;
}


vector<string> filenames ;

map<string, TRecord *> records ;

int scenario = 0 ;

const int scenario_11_m = 1 ;
const int scenario_100_m = 2 ;

int main()
{
	scenario = scenario_11_m ; 
	int diagonal = diagonal_45b_56t ;

	gStyle->SetOptStat("nemrou");	
	gStyle->SetOptFit(1111);

	gStyle->SetPalette(1) ;
	gStyle->SetStatColor(kWhite) ;  
	gStyle->SetTitleFillColor(kWhite) ;
	gStyle->SetFillColor(kWhite) ;
	
	ofstream *status ;

	
	if(scenario == scenario_11_m)
	{
		//filenames.push_back("/afs/cern.ch/user/j/jkaspar/public/900GeV/beta11/DS-fill7302/distributions_45b_56t.root") ;
		
                if(diagonal == diagonal_45b_56t)
		{
			filenames.push_back("/afs/cern.ch/user/j/jkaspar/public/ForFrici/beta11/version_2020_08_17/DS-fill7301/distributions_45b_56t.root") ;
			
			status = new ofstream("Results/diagonal_45b_56t.txt") ;
		}
                else if(diagonal == diagonal_45t_56b)
		{
			filenames.push_back("/afs/cern.ch/user/j/jkaspar/public/ForFrici/beta11/version_2020_08_17/DS-fill7301/distributions_45t_56b.root") ;

			status = new ofstream("Results/diagonal_45t_56b.txt") ;
		}
		else
		{
			cout << "No such scenario!" << endl ;
			exit(1) ;
		}
		
		// filenames.push_back("/afs/cern.ch/user/j/jkaspar/public/900GeV/beta11/DS-fill7302/distributions_45t_56b.root") ;
		subdir_per_optics = "11_m/" ;
	}
	else if(scenario == scenario_100_m)
	{
		filenames.push_back("/afs/cern.ch/user/j/jkaspar/public/900GeV/beta100/DS-fill7280//distributions_45b_56t.root") ;
		subdir_per_optics = "100_m/" ;
	}
	else
	{
		cout << "There is no such scenario!" << endl ;
		exit(1) ;
	}

	
	TFile *root_files = TFile::Open(filenames[0].c_str()) ;
	
	if(scenario == scenario_11_m)
	{

		if(diagonal == diagonal_45t_56b) 
		{
			records["record_1"] =  new TRecord("record_1",  root_files,  "optics/matching input, full/g_thl_x_R_vs_thl_x_L"		, -0.15e-3, 0.15e-3) ;
			records["record_2"] =  new TRecord("record_2",  root_files,  "optics/matching input, full/g_y_R_220_F_vs_y_L_220_F"	, 9, 14) ;
			records["record_3"] =  new TRecord("record_3",  root_files,   "optics/matching input, full/g_thl_y_R_vs_y_R_220_F"	,  -9, -5) ;
			records["record_4"] =  new TRecord("record_4",  root_files,  "optics/matching input, full/g_thl_y_L_vs_y_L_220_F"	,  6, 11) ;
			records["record_5"] =  new TRecord("record_5",  root_files,  "optics/matching input, full/g_thl_x_R_vs_x_R_220_F"	, -1.5, 1.5) ;
			records["record_6"] =  new TRecord("record_6",  root_files,  "optics/matching input, full/g_thl_x_L_vs_x_L_220_F"	, -1, 1) ;
			records["record_7"] =  new TRecord("record_7",  root_files,  "optics/matching input, full/g_x_L_220_F_vs_y_L_220_F"	, 6, 10) ;
			records["record_8"] =  new TRecord("record_8",  root_files,  "optics/matching input, full/g_x_L_210_F_vs_y_L_210_F"	, 6, 10) ;
			records["record_9"] =  new TRecord("record_9",  root_files,  "optics/matching input, full/g_x_R_210_F_vs_y_R_210_F"	, -10, -5) ;
			records["record_10"] = new TRecord("record_10", root_files,  "optics/matching input, full/g_x_R_220_F_vs_y_R_220_F"	, -5, -10) ;
		}
		else if(diagonal == diagonal_45b_56t) 
		{
			records["record_1"] =  new TRecord("record_1",  root_files,  "optics/matching input, full/g_thl_x_R_vs_thl_x_L"		, -0.2e-3, 0.2e-3) ;
			records["record_2"] =  new TRecord("record_2",  root_files,  "optics/matching input, full/g_y_R_220_F_vs_y_L_220_F"	, -14 , -8) ;
			records["record_3"] =  new TRecord("record_3",  root_files,   "optics/matching input, full/g_thl_y_R_vs_y_R_220_F"	, 5 , 9) ;
			records["record_4"] =  new TRecord("record_4",  root_files,  "optics/matching input, full/g_thl_y_L_vs_y_L_220_F"	, -11 , -6) ;
			records["record_5"] =  new TRecord("record_5",  root_files,  "optics/matching input, full/g_thl_x_R_vs_x_R_220_F"	, -1.5 , 1.5) ;
			records["record_6"] =  new TRecord("record_6",  root_files,  "optics/matching input, full/g_thl_x_L_vs_x_L_220_F"	, -1 , 1) ;
			records["record_7"] =  new TRecord("record_7",  root_files,  "optics/matching input, full/g_x_L_220_F_vs_y_L_220_F"	, -10, -6) ;
			records["record_8"] =  new TRecord("record_8",  root_files,  "optics/matching input, full/g_x_L_210_F_vs_y_L_210_F"	, -10, -6) ;
			records["record_9"] =  new TRecord("record_9",  root_files,  "optics/matching input, full/g_x_R_210_F_vs_y_R_210_F"	,  6, 10) ;
			records["record_10"] = new TRecord("record_10", root_files,  "optics/matching input, full/g_x_R_220_F_vs_y_R_220_F"	,  6, 10) ;
		}
		else
		{
			cout << "No such scenario!" << endl ;
			exit(1) ;
		}
	}
	else if(scenario == scenario_100_m)
	{
		records["record_1"] =  new TRecord("record_1",  root_files,  "optics/matching input, full/g_thl_x_R_vs_thl_x_L"		, -0.4e-3, 0.4e-3) ;
		records["record_2"] =  new TRecord("record_2",  root_files,  "optics/matching input, full/g_y_R_220_F_vs_y_L_220_F"	, -25, -15) ;
		records["record_3"] =  new TRecord("record_3",  root_files,   "optics/matching input, full/g_thl_y_R_vs_y_R_220_F"	,  10, 20) ;
		records["record_4"] =  new TRecord("record_4",  root_files,  "optics/matching input, full/g_thl_y_L_vs_y_L_220_F"	, -20, -10) ;
		records["record_5"] =  new TRecord("record_5",  root_files,  "optics/matching input, full/g_thl_x_R_vs_x_R_220_F"	, -5, 5) ;
		records["record_6"] =  new TRecord("record_6",  root_files,  "optics/matching input, full/g_thl_x_L_vs_x_L_220_F"	, -5, 5) ;
		records["record_7"] =  new TRecord("record_7",  root_files,  "optics/matching input, full/g_x_L_220_F_vs_y_L_220_F"	, -10, -6) ;
		records["record_8"] =  new TRecord("record_8",  root_files,  "optics/matching input, full/g_x_L_210_F_vs_y_L_210_F"	, -10, -6) ;
		records["record_9"] =  new TRecord("record_9",  root_files,  "optics/matching input, full/g_x_R_210_F_vs_y_R_210_F"	, 5, 10) ;
		records["record_10"] = new TRecord("record_10", root_files,  "optics/matching input, full/g_x_R_220_F_vs_y_R_220_F"	, 5, 10) ;
	}

	TCanvas *c = new TCanvas ;

        for (std::map<string, TRecord *>::iterator it = records.begin(); it != records.end(); ++it)
	{
		(*it).second->saveHistogram_and_Profile(c, "_before", diagonal) ;
	}

        for (std::map<string, TRecord *>::iterator it = records.begin(); it != records.end(); ++it)
	{
		(*it).second->fitProfile() ;
	}

        for (std::map<string, TRecord *>::iterator it = records.begin(); it != records.end(); ++it)
	{
		(*it).second->saveHistogram_and_Profile(c, "", diagonal) ;
	}

}
	
