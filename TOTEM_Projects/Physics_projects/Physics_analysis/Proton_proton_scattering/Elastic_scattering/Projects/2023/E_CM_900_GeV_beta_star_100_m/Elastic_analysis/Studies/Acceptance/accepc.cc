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
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TRandom3.h"
#include "TEllipse.h"
#include "TMultiGraph.h"

#include <fstream>

#include <iomanip>

TCanvas c ;

TGraph *graph = NULL ;
TGraph *graph_clone = NULL ;

double accep_based_on_frame(double theta_star_rad)
{
	int number_of_points = graph->GetN() ;

	if(number_of_points < 3)
	{
		cout << "There has to be at least 3 points for an acceptance frame" << endl ;
		exit(1) ;
	}

	double x1, y1 ;
	graph->GetPoint(0, x1, y1) ;

	for(int i = 1 ; i < number_of_points ; ++i)
	{
		cout << x1 << endl ;
		double x2, y2 ;

		graph->GetPoint(i, x2, y2) ;

		double m = (y2 - y1) / (x2 - x1) ;
		double p = y1 - m * x1 ;

		double a = ((m*m) + 1) ;
		double b = 2 * p * m ;
		double c = ((p * p) - (theta_star_rad * theta_star_rad)) ;

		double D = ((b*b) - (4 * a * c)) ;

		if(D > 0.0)
		{					 
			double x_s_1 = (-b + sqrt(D)) / (2*a);
			double x_s_2 = (-b - sqrt(D)) / (2*a);

			double y_s_1 = (m * x_s_1) + p ;
			double y_s_2 = (m * x_s_2) + p ;

			cout << " " << x_s_1 << " " << y_s_1 << endl ;
			cout << " " << x_s_2 << " " << y_s_2 << endl ;

			TEllipse *ellipse1 = new TEllipse(x_s_1, y_s_1, 10e-6) ;
			TEllipse *ellipse2 = new TEllipse(x_s_2, y_s_2, 10e-6) ;

			TEllipse *ellipse3 = new TEllipse(x_s_1, y_s_1, 10e-6) ;
			TEllipse *ellipse4 = new TEllipse(x_s_2, y_s_2, 10e-6) ;

			double tempx1 = x1 ;
			double tempx2 = x2 ;
			
			if(x1 > x2)
			{
				tempx1 = x2 ;
				tempx2 = x1 ;
			}

			ellipse1->SetLineColor(kGreen) ;
			ellipse2->SetLineColor(kGreen) ;

			ellipse1->Draw("same") ;
			ellipse2->Draw("same") ;
			
			ellipse3->SetLineColor(kRed) ;
			ellipse4->SetLineColor(kRed) ;

			if((tempx1 < x_s_1) && (x_s_1 < tempx2)) ellipse3->Draw("same") ;
			if((tempx1 < x_s_2) && (x_s_2 < tempx2)) ellipse4->Draw("same") ;
		}


		x1 = x2 ;
		y1 = y2 ;

	}

	cout << "an acceptance computed" << endl ;

}

void other_studies()
{
	TCanvas b ;
	TCanvas d ;
	
	c.SetLogy() ;			
	d.SetLogy() ;			


	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	string word ;
	
	bool first = true ;
	
	while(runs >> word)
	{
		// TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP_2RP/All_root_files_to_define_cuts_run_" + word + "/Generic.root").c_str()) ;
		TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_" + word + "/Generic.root").c_str()) ;
		
		if(file == NULL) continue ;
		
		TH1D *hist = ((TH1D *)file->Get("P0014_PlotsCollection_theta_x_star_left_far_rad_theta_x_star_right_far_rad_with_cut_distance_from_cut")) ;

		if(hist == NULL) continue ;

		TH1D *hist2 = ((TH1D *)hist->Clone("hist2")) ;
		
		double signal_fit_boundary = 2e-4 ;
		double bkg_fit_boundary = 4e-4 ;
		
		if(word.compare("324536") == 0.0)
		{
			bkg_fit_boundary = 3e-4 ;
		}

		hist->Fit("gaus", "", "", -signal_fit_boundary, signal_fit_boundary) ;
		
		for(int i = 170 ; i <= 230 ; ++i)
		{
			hist2->SetBinContent(i, 0) ;
			hist2->SetBinError(i, 0) ;
		}

		hist2->Fit("gaus", "", "", -bkg_fit_boundary, bkg_fit_boundary) ;

		b.cd() ;
		if(first) hist->Draw("") ;
		hist->Draw("same") ;
		
		first = false ;
	
		d.cd() ;

		hist->Draw("") ;
		hist2->Draw("same") ;
		
		d.SaveAs(("plots/plot_" + word + ".pdf").c_str()) ;
		d.SaveAs(("plots/plot_" + word + ".root").c_str()) ;
		
		double val1 = hist->GetFunction("gaus")->Eval(0) ;
		double val2 = hist2->GetFunction("gaus")->Eval(0) ;
		
		cout << "run " << word << "  " << val2 / val1 << endl ;
		
	}
	
	b.SaveAs("b.root") ;
	b.SaveAs("b.pdf") ;
}

void main_studies()
{

	TFile *file = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/Tests/frame2.root") ;
	
	graph = ((TGraph *)file->Get("Graph")) ;

	c.cd() ;
	
	double R = (300e-6 / 2.0) ;

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, -0.7e-3, 0.7e-3, 100, -0.25e-3, 0.25e-3) ;	
	hist_2d->SetTitle("") ;
	TEllipse ellipse(0, 0, R, R) ;
	ellipse.SetFillStyle(0) ;
	ellipse.SetLineColor(kBlue) ;

	hist_2d->Draw() ;
	graph->Draw("same") ;	
	ellipse.Draw("same") ;

	accep_based_on_frame(R) ;
	
//	c.SetGridx() ;
//	c.SetGridy() ;
	
	hist_2d->GetXaxis()->SetTitle("#theta_{x}* (rad)") ;
	hist_2d->GetYaxis()->SetTitle("#theta_{y}* (rad)") ;

	c.SaveAs("test.pdf") ;
}

void horizontal_elastic_alignment()
{
	// ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;
	// string word ;
	// bool first = true ;
	// while(runs >> word)

	TFile *file = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_324536/Generic.root") ;

	vector<string> histograms ;

	histogams.push_back("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP") ;
	histogams.push_back("P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP") ;
	histogams.push_back("P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP") ;
	histogams.push_back("P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP") ;

	TH2D *hist1 = ((TH2D *)file->Get("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP")) ;

	double w_l = hist1->GetYaxis()->FindBin(6) ;
	double w_u = hist1->GetYaxis()->FindBin(25) ;
	TH1D *hist_1_proj = hist1->ProjectionX("px1", w_l, w_u) ;

	hist_1_proj->Fit("gaus", "", "", -2, 2) ;
}

int main()
{
	gStyle->SetOptStat("");

	// main_studies() ;
	// other_studies() ;

	horizontal_elastic_alignment() ;
}
