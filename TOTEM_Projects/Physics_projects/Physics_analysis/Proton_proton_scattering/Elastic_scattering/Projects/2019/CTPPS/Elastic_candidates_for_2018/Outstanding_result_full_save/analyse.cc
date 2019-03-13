#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "stdlib.h"
#include <iomanip>

#include "TNtuple.h"
#include "TH1D.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TMath.h"

#include "math.h"

#include "TH2D.h"
#include "TProfile.h"

#include "TFitResult.h"

string directory = "/afs/cern.ch/work/f/fnemes/tmp/pp/CTPPS/2018/Elastic_candidates/From_Jan/" ;
string output_root_directory = "/home/fnemes/Documents/Analysis/pp/CTPPS/2018/Optics_matching/Elastic_constraints/Alignment_run_2018_04_13/" ;
string output_directory = "" ;

TH2D *hist_left_far = NULL ;
TH2D *hist_left_near = NULL ;

TH2D *hist_right_far = NULL ;
TH2D *hist_right_near = NULL ;

TH2D *hist_dx_left_x_near = NULL ;
TH2D *hist_dx_right_x_near = NULL ;

TH2D *hist_dy_left_y_near = NULL ;
TH2D *hist_dy_right_y_near = NULL ;

TH2D *hist_dy_left_y_far = NULL ;
TH2D *hist_dy_right_y_far = NULL ;

TH2D *hist_theta_x_left_x_near = NULL ;
TH2D *hist_theta_x_right_x_near = NULL ;

TH2D *hist_theta_x_left_x_near_right = NULL ;
TH2D *hist_theta_x_right_x_near_left = NULL ;

TProfile *profile_hist_theta_x_left_x_near = NULL ;
TProfile *profile_hist_theta_x_right_x_near = NULL ;

TH2D *hist_theta_y_left_y_near = NULL ;
TH2D *hist_theta_y_right_y_near = NULL ;

TProfile *profile_hist_theta_y_left_y_near = NULL ;
TProfile *profile_hist_theta_y_right_y_near = NULL ;

TProfile *profile_hist_theta_y_left_y_far = NULL ;
TProfile *profile_hist_theta_y_right_y_far = NULL ;

TProfile *profile_hist_theta_y_right_y_near_rotated = NULL ;

TH2D *hist_y_near_left_right = NULL ;
TH2D *hist_y_far_left_right  = NULL ;

TH2D *hist_x_near_left_right = NULL ;
TH2D *hist_x_far_left_right  = NULL ;

TH2D *hist_theta_x_left_right  = NULL ;

// Summary plots

TCanvas *profile_hist_theta_y_left_y_near_summary = NULL ;
TCanvas *profile_hist_theta_y_left_y_far_summary = NULL ;

double profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = 0 ;
double profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = 0 ;

double profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = 0 ;
double profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = 0 ;

double profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = 0 ;
double profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = 0 ;

double profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = 0 ;
double profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = 0 ;


const double distance_m = 7.0 ;

void rotate(double x, double y, double alpha, double &x_rotated, double &y_rotated)
{
	x_rotated = (x * cos(alpha)) - (y * sin(alpha)) ;
	y_rotated = (x * sin(alpha)) + (y * cos(alpha)) ;
}

void test(string filename)
{
	int run_number, event_number ;
	
	bool first = true ;
	bool done = false ;

	double alpha = 0 ;

	while(!done)
	{

		ifstream elastic_candidates(filename.c_str()) ; 

		while(elastic_candidates >> run_number)
		{
			string for_comma ;

			elastic_candidates >> for_comma ;
			elastic_candidates >> event_number ;
			elastic_candidates >> for_comma ;

			double left_far_x, left_far_y, left_near_x, left_near_y, right_near_x, right_near_y, right_far_x, right_far_y ;

			elastic_candidates >> left_far_x >> for_comma >> left_far_y >> for_comma >> left_near_x >> for_comma >> left_near_y >> for_comma >> right_near_x >> for_comma >> right_near_y >> for_comma >> right_far_x >> for_comma >> right_far_y ;

			// cout << run_number << " " << event_number << " " << left_far_x << " " << left_far_y << " " <<  left_near_x << " " <<  left_near_y << " " <<  right_near_x << " " <<  right_near_y << " " <<  right_far_x << " " <<  right_far_y << endl ;

			double dx_left = (left_far_x - left_near_x) ;
			double dx_right = (right_far_x - right_near_x) ;

			double dy_left = (left_far_y - left_near_y) ;
			double dy_right = (right_far_y - right_near_y) ;

			double theta_x_left = (left_far_x - left_near_x)  / distance_m ;
			double theta_x_right = (right_far_x - right_near_x) / distance_m ;

			double theta_y_left = (left_far_y - left_near_y) / distance_m ;
			double theta_y_right = (right_far_y - right_near_y) / distance_m ;

			if(first)
			{

				hist_left_far->Fill(left_far_x, left_far_y) ;
				hist_left_near->Fill(left_near_x, left_near_y) ;

				hist_right_far->Fill(right_far_x, right_far_y) ;
				hist_right_near->Fill(right_near_x, right_near_y) ;

				// Difference plots

				hist_dx_left_x_near->Fill(left_near_x, dx_left) ;
				hist_dx_right_x_near->Fill(right_near_x, dx_right) ;

				hist_dy_left_y_near->Fill(left_near_y, dy_left) ;
				hist_dy_right_y_near->Fill(right_near_y, dy_right) ;

				hist_dy_left_y_far->Fill(left_far_y, dy_left) ;
				hist_dy_right_y_far->Fill(right_far_y, dy_right) ;

				// Spectrometer plots

				hist_theta_x_left_x_near->Fill(left_near_x, theta_x_left) ;
				hist_theta_x_right_x_near->Fill(right_near_x, theta_x_right) ;

				profile_hist_theta_x_left_x_near->Fill(left_near_x, theta_x_left) ;
				profile_hist_theta_x_right_x_near->Fill(right_near_x, theta_x_right) ;

				hist_theta_y_left_y_near->Fill(left_near_y, theta_y_left) ;
				hist_theta_y_right_y_near->Fill(right_near_y, theta_y_right) ;

				profile_hist_theta_y_left_y_near->Fill(left_near_y, theta_y_left) ;
				profile_hist_theta_y_right_y_near->Fill(right_near_y, theta_y_right) ;

				profile_hist_theta_y_left_y_far->Fill(left_far_y, theta_y_left) ;
				profile_hist_theta_y_right_y_far->Fill(right_far_y, theta_y_right) ;

				// Left - right correlations vertical

				hist_y_near_left_right->Fill(left_near_y, right_near_y) ;
				hist_y_far_left_right->Fill(left_far_y, right_far_y) ;

				// Left - right correlations horizontal

				hist_x_near_left_right->Fill(left_near_x, right_near_x) ;
				hist_x_far_left_right->Fill(left_far_x, right_far_x) ;

				hist_theta_x_left_right->Fill(theta_x_left, theta_x_right) ;

				// Spectrometer plots

				hist_theta_x_left_x_near_right->Fill(right_near_x, theta_x_left) ;
				hist_theta_x_right_x_near_left->Fill(left_near_x, theta_x_right) ;

			}
			
			// For fits
			
			double x_rotated, y_rotated ;
			
			rotate(right_near_y, theta_y_right, alpha, x_rotated, y_rotated) ;
			profile_hist_theta_y_right_y_near_rotated->Fill(x_rotated, y_rotated) ;
			
		}
		
		first = false ;
		done = true ;

		/*
		TFitResultPtr fitresult = profile_hist_theta_y_right_y_near_rotated->Fit("pol1", "SQ", "", profile_hist_theta_y_right_y_near_rotated_fit_range_low, profile_hist_theta_y_right_y_near_rotated_fit_range_high) ;

		alpha = alpha - fitresult->Parameter(1) ;
		double alpha_error = fitresult->ParError(1) ;
		
		if(fabs(fitresult->Parameter(1) / alpha_error) < 1) done = true ;

		elastic_candidates.close() ;	

		if(!done)
		{
			profile_hist_theta_y_right_y_near_rotated->Reset() ;
		}
		*/
	}

	cout << "alpha: " << alpha << endl ;	
}


int main(int argc, char *argv[])
{

	gStyle->SetOptStat(11);
	gStyle->SetStatFontSize(0.01) ;

	bool dofit = true ;
	bool only_good_shaped_profiles = true ;

	TCanvas *general = new TCanvas("general", "general") ;

	profile_hist_theta_y_left_y_near_summary = new TCanvas("profile_hist_theta_y_left_y_near_summary", "profile_hist_theta_y_left_y_near_summary") ;
	profile_hist_theta_y_left_y_far_summary = new TCanvas("profile_hist_theta_y_left_y_far_summary", "profile_hist_theta_y_left_y_far_summary") ;

	vector<string> filenames ;

	filenames.push_back("DS-xangle-130-beta-25-dgn-45b-56t.txt") ;
	filenames.push_back("DS-xangle-130-beta-25-dgn-45t-56b.txt") ;
	filenames.push_back("DS-xangle-130-beta-30-dgn-45b-56t.txt") ;
	filenames.push_back("DS-xangle-130-beta-30-dgn-45t-56b.txt") ;
	filenames.push_back("DS-xangle-131-beta-30-dgn-45b-56t.txt") ;
	filenames.push_back("DS-xangle-131-beta-30-dgn-45t-56b.txt") ;
	filenames.push_back("DS-xangle-160-beta-30-dgn-45b-56t.txt") ;
	filenames.push_back("DS-xangle-160-beta-30-dgn-45t-56b.txt") ;
	
	TFitResultPtr fitresult ;
	
	TGraphErrors *results_with_far_left = new TGraphErrors() ;
	TGraphErrors *results_with_near_left = new TGraphErrors() ;

	TGraphErrors *results_with_far_right = new TGraphErrors() ;
	TGraphErrors *results_with_near_right = new TGraphErrors() ;
	
	int number_of_points_in_summary_graph  = 0 ;
	
	for(int filename_index = 0 ; filename_index < filenames.size() ; ++filename_index)
	{

		general->cd() ;

		string filename = filenames[filename_index] ;
	
		if(filename.compare("DS-xangle-130-beta-25-dgn-45b-56t.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = -3.6 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = -2.9 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = -3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = -2.4 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = -3.0 ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = -2.9  + 0.5 ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = -3.4 + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = -2.2  ;

			output_directory = output_root_directory + "DS_xangle_130_beta_25_dgn_45b_56t/" ;
		}
		else if(filename.compare("DS-xangle-130-beta-25-dgn-45t-56b.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = 2.7 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = 3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = 3.0 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = 4.0 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = 2.3 ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = 3.4 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = 3.0 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = 4.0 - 0.5  ;

			output_directory = output_root_directory + "DS_xangle_130_beta_25_dgn_45t_56b/" ;
		}
		else if(filename.compare("DS-xangle-130-beta-30-dgn-45b-56t.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = -3.8 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = -2.9 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = -3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = -2.4 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = -3.0 ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = -2.9 +  0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = -3.4 + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = -2.2  ;

			output_directory = output_root_directory + "DS_xangle_130_beta_30_dgn_45b_56t/" ;
		}
		else if(filename.compare("DS-xangle-130-beta-30-dgn-45t-56b.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = 2.7 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = 3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = 3.0 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = 4.0 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = 2.3  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = 3.4 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = 3.0 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = 4.0 - 0.5  ;

			output_directory = output_root_directory + "DS_xangle_130_beta_30_dgn_45t_56b/" ;

		}
		else if(filename.compare("DS-xangle-131-beta-30-dgn-45b-56t.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = -3.6 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = -2.9 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = -3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = -2.4 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = -3.0  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = -2.9  + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = -3.4 + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = -2.2  ;

			output_directory = output_root_directory + "DS_xangle_131_beta_30_dgn_45b_56t/" ;
		}
		else if(filename.compare("DS-xangle-131-beta-30-dgn-45t-56b.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = 2.7 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = 3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = 3.0 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = 4.0 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = 2.3  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = 3.4 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = 3.0 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = 4.0 - 0.5  ;

			output_directory = output_root_directory + "DS_xangle_131_beta_30_dgn_45t_56b/" ;
		}
		else if(filename.compare("DS-xangle-160-beta-30-dgn-45b-56t.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = -3.6 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = -2.9 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = -3.4 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = -2.4 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = -3.0  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = -2.9  + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = -3.4 + 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = -2.2  ;

			output_directory = output_root_directory + "DS_xangle_160_beta_30_dgn_45b_56t/" ;
		}
		else if(filename.compare("DS-xangle-160-beta-30-dgn-45t-56b.txt") == 0)
		{
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_left  = 2.7 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_left = 3.3 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_low_right  = 3.0 ;
			profile_hist_theta_y_right_y_near_rotated_fit_range_high_right = 4.0 ;

			profile_hist_theta_y_right_y_far_rotated_fit_range_low_left  = 2.3  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_left = 3.3 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_low_right  = 3.0 - 0.5  ;
			profile_hist_theta_y_right_y_far_rotated_fit_range_high_right = 4.0 - 0.5  ;

			output_directory = output_root_directory + "DS_xangle_160_beta_30_dgn_45t_56b/" ;
		}
		else
		{
			cout << "No such filename !" << endl ;
			exit(1) ;
		}

		string full_filename = directory + filename ;

		hist_left_far = new TH2D("hist_left_far", "hist_left_far", 		100, -10, 10, 100, -10, 10) ;
		hist_left_near = new TH2D("hist_left_near", "hist_left_near", 		100, -10, 10, 100, -10, 10) ;

		hist_right_far = new TH2D("hist_right_far", "hist_right_far", 		100, -10, 10, 100, -10, 10) ;
		hist_right_near = new TH2D("hist_right_near", "hist_right_near", 	100, -10, 10, 100, -10, 10) ;

		hist_dx_left_x_near = new TH2D("hist_dx_left_x_near", "hist_dx_left_x_near", 		400, -5, 5, 400, -3, 3) ;
		hist_dx_right_x_near = new TH2D("hist_dx_right_x_near", "hist_dx_right_x_near", 	400, -5, 5, 400, -3, 3) ;

		hist_dy_left_y_near = new TH2D("hist_dy_left_y_near", "hist_dy_left_y_near", 		400, -5, 5, 400, -3, 3) ;
		hist_dy_right_y_near = new TH2D("hist_dy_right_y_near", "hist_dy_right_y_near", 	400, -5, 5, 400, -3, 3) ;

		hist_dy_left_y_far = new TH2D("hist_dy_left_y_far", "hist_dy_left_y_far", 		400, -5, 5, 400, -3, 3) ;
		hist_dy_right_y_far = new TH2D("hist_dy_right_y_far", "hist_dy_right_y_far", 		400, -5, 5, 400, -3, 3) ;

		// spectrometer cuts, horizontal

		hist_theta_x_left_x_near = new TH2D("hist_theta_x_left_x_near", "hist_theta_x_left_x_near", 		100, -5, 5, 100, -0.4, 0.4) ;
		hist_theta_x_right_x_near = new TH2D("hist_theta_x_right_x_near", "hist_theta_x_right_x_near", 		100, -5, 5, 100, -0.4, 0.4) ;

		profile_hist_theta_x_left_x_near = new TProfile("profile_hist_theta_x_left_x_near", "profile_hist_theta_x_left_x_near", 100, -5, 5) ;
		profile_hist_theta_x_right_x_near = new TProfile("profile_hist_theta_x_right_x_near", "profile_hist_theta_x_right_x_near", 100, -5, 5) ;

		hist_theta_x_left_x_near_right = new TH2D("hist_theta_x_left_x_near_right", "hist_theta_x_left_x_near_right", 100, -5, 5, 100, -0.4, 0.4) ;
		hist_theta_x_right_x_near_left = new TH2D("hist_theta_x_right_x_near_left", "hist_theta_x_right_x_near_left", 100, -5, 5, 100, -0.4, 0.4) ;

		// spectrometer cuts, vertical

		hist_theta_y_left_y_near = new TH2D("hist_theta_y_left_y_near", "hist_theta_y_left_y_near", 	100, -5, 5, 100, -0.4, 0.4) ;
		hist_theta_y_right_y_near = new TH2D("hist_theta_y_right_y_near", "hist_theta_y_right_y_near", 	100, -5, 5, 100, -0.4, 0.4) ;

		profile_hist_theta_y_left_y_near = new TProfile(("profile_hist_theta_y_left_y_near" + filename).c_str(), ("profile_hist_theta_y_left_y_near" + filename).c_str(),  100, -5, 5) ;
		profile_hist_theta_y_right_y_near = new TProfile(("profile_hist_theta_y_right_y_near" + filename).c_str(), ("profile_hist_theta_y_right_y_near" + filename).c_str(), 100, -5, 5) ;

		profile_hist_theta_y_left_y_far = new TProfile(("profile_hist_theta_y_left_y_far" + filename).c_str(), ("profile_hist_theta_y_left_y_far" + filename).c_str(),  100, -5, 5) ;
		profile_hist_theta_y_right_y_far = new TProfile(("profile_hist_theta_y_right_y_far" + filename).c_str(), ("profile_hist_theta_y_right_y_far" + filename).c_str(), 100, -5, 5) ;

		profile_hist_theta_y_right_y_near_rotated = new TProfile("profile_hist_theta_y_right_y_near_rotated", "profile_hist_theta_y_right_y_near_rotated", 100, -5, 5) ;

		// left right y-correlations

		hist_y_near_left_right = new TH2D("hist_y_near_left_right", "hist_y_near_left_right", 100, -10, 10, 100, -10, 10) ;
		hist_y_far_left_right  = new TH2D("hist_y_far_left_right",  "hist_y_far_left_right",  100, -10, 10, 100, -10, 10) ;

		hist_x_near_left_right = new TH2D("hist_x_near_left_right", "hist_x_near_left_right", 100, -10, 10, 100, -10, 10) ;
		hist_x_far_left_right  = new TH2D("hist_x_far_left_right",  "hist_x_far_left_right",  100, -10, 10, 100, -10, 10) ;

		hist_theta_x_left_right  = new TH2D("hist_theta_x_left_right",  "hist_theta_x_left_right",  100, -0.4, 0.4, 100, -0.4, 0.4) ;

		test(full_filename) ;

		hist_left_far->SaveAs((output_directory + "hist_left_far.root").c_str()) ;
		hist_left_near->SaveAs((output_directory + "hist_left_near.root").c_str()) ;

		hist_right_far->SaveAs((output_directory + "hist_right_far.root").c_str()) ;
		hist_right_near->SaveAs((output_directory + "hist_right_near.root").c_str()) ;

		// Difference plots

		hist_dx_left_x_near->SaveAs((output_directory + "hist_dx_left_x_near.root").c_str()) ;
		hist_dx_right_x_near->SaveAs((output_directory + "hist_dx_right_x_near.root").c_str()) ;

		hist_dy_left_y_near->SaveAs((output_directory + "hist_dy_left_y_near.root").c_str()) ;
		hist_dy_right_y_near->SaveAs((output_directory + "hist_dy_right_y_near.root").c_str()) ;

		hist_dy_left_y_far->SaveAs((output_directory + "hist_dy_left_y_far.root").c_str()) ;
		hist_dy_right_y_far->SaveAs((output_directory + "hist_dy_right_y_far.root").c_str()) ;

		// Angular plots 

		// Fits

		int color_index = (filename_index + 1) ;
		
		gStyle->SetStatColor(color_index) ;

		cout << "profile_hist_theta_x_right_x_near" << endl ;
		profile_hist_theta_x_right_x_near->Fit("pol1", "S", "", -1.0, 1.0) ;
		gStyle->SetOptFit(1111) ;

		cout << "profile_hist_theta_y_right_y_near" << endl ;

		if(dofit)
		{
			fitresult = profile_hist_theta_y_right_y_near->Fit("pol1", "S", "", -profile_hist_theta_y_right_y_near_rotated_fit_range_high_right, -profile_hist_theta_y_right_y_near_rotated_fit_range_low_right) ;
			
			double ndf  = fitresult->Ndf() ;
			double fcn  = fitresult->MinFcnValue() ;
			double prob = fitresult->Prob() ;
			double par1 = fitresult->Parameter(1) ;
			double per1 = fitresult->ParError(1) ;


			if(!only_good_shaped_profiles || (filename_index != 6))
			{
				results_with_near_right->SetPoint(number_of_points_in_summary_graph, number_of_points_in_summary_graph, par1) ;
				results_with_near_right->SetPointError(number_of_points_in_summary_graph, 0, per1) ;
				results_with_near_right->SetLineColor(kBlue) ;
				results_with_near_right->SetLineWidth(2) ;
			}
			
			fitresult = profile_hist_theta_y_right_y_far->Fit("pol1", "S", "", -profile_hist_theta_y_right_y_far_rotated_fit_range_high_right, -profile_hist_theta_y_right_y_far_rotated_fit_range_low_right) ;
			
			profile_hist_theta_y_right_y_near->GetFunction("pol1")->SetLineColor(color_index) ;
			profile_hist_theta_y_right_y_far->GetFunction("pol1")->SetLineColor(color_index) ;

			ndf  = fitresult->Ndf() ;
			fcn  = fitresult->MinFcnValue() ;
			prob = fitresult->Prob() ;
			par1 = fitresult->Parameter(1) ;
			per1 = fitresult->ParError(1) ;

			if(!only_good_shaped_profiles || (filename_index != 6))	
			{
				results_with_far_right->SetPoint(number_of_points_in_summary_graph, number_of_points_in_summary_graph, par1) ;
				results_with_far_right->SetPointError(number_of_points_in_summary_graph, 0, per1) ;
				results_with_far_right->SetLineColor(kBlue) ;
				results_with_far_right->SetLineWidth(2) ;
			}

		}
		
		gStyle->SetOptFit(1111) ;

		cout << "profile_hist_theta_x_left_x_near" << endl ;
		profile_hist_theta_x_left_x_near->Fit("pol1",  "S", "", -1.0, 1.0) ;
		gStyle->SetOptFit(1111) ;

		cout << "profile_hist_theta_y_left_y_near" << endl ;
		
		if(dofit)
		{
			fitresult = profile_hist_theta_y_left_y_near->Fit("pol1",  "S", "", profile_hist_theta_y_right_y_near_rotated_fit_range_low_left, profile_hist_theta_y_right_y_near_rotated_fit_range_high_left) ;
			
			double ndf  = fitresult->Ndf() ;
			double fcn  = fitresult->MinFcnValue() ;
			double prob = fitresult->Prob() ;
			double par1 = fitresult->Parameter(1) ;
			double per1 = fitresult->ParError(1) ;

			if(!only_good_shaped_profiles || (filename_index != 6))			
			{
				results_with_near_left->SetPoint(number_of_points_in_summary_graph, number_of_points_in_summary_graph, par1) ;
				results_with_near_left->SetPointError(number_of_points_in_summary_graph, 0, per1) ;
				results_with_near_left->SetLineColor(kBlue) ;
				results_with_near_left->SetLineWidth(2) ;
			}

			fitresult = profile_hist_theta_y_left_y_far->Fit("pol1",  "S", "", profile_hist_theta_y_right_y_far_rotated_fit_range_low_left, profile_hist_theta_y_right_y_far_rotated_fit_range_high_left) ;

			ndf  = fitresult->Ndf() ;
			fcn  = fitresult->MinFcnValue() ;
			prob = fitresult->Prob() ;
			par1 = fitresult->Parameter(1) ;
			per1 = fitresult->ParError(1) ;

			if(!only_good_shaped_profiles || (filename_index != 6))			
			{
				results_with_far_left->SetPoint(number_of_points_in_summary_graph, number_of_points_in_summary_graph, par1) ;
				results_with_far_left->SetPointError(number_of_points_in_summary_graph, 0, per1) ;
				results_with_far_left->SetLineColor(kBlue) ;
				results_with_far_left->SetLineWidth(2) ;
			}

			profile_hist_theta_y_left_y_near->GetFunction("pol1")->SetLineColor(color_index) ;
			profile_hist_theta_y_left_y_far->GetFunction("pol1")->SetLineColor(color_index) ;
		}
		
		gStyle->SetOptFit(1111) ;

		// Saves

		hist_theta_x_left_x_near->SaveAs((output_directory + "hist_theta_x_left_x_near.root").c_str()) ;
		hist_theta_x_right_x_near->SaveAs((output_directory + "hist_theta_x_right_x_near.root").c_str()) ;

		profile_hist_theta_x_left_x_near->SaveAs((output_directory + "profile_hist_theta_x_left_x_near.root").c_str()) ;
		profile_hist_theta_x_right_x_near->SaveAs((output_directory + "profile_hist_theta_x_right_x_near.root").c_str()) ;

		hist_theta_y_left_y_near->SaveAs((output_directory + "hist_theta_y_left_y_near.root").c_str()) ;
		hist_theta_y_right_y_near->SaveAs((output_directory + "hist_theta_y_right_y_near.root").c_str()) ;

		hist_theta_y_left_y_near->SaveAs((output_directory + "hist_theta_y_left_y_near.root").c_str()) ;
		hist_theta_y_right_y_near->SaveAs((output_directory + "hist_theta_y_right_y_near.root").c_str()) ;

		profile_hist_theta_y_left_y_near->SaveAs((output_directory + "profile_hist_theta_y_left_y_near.root").c_str()) ;
		profile_hist_theta_y_right_y_near->SaveAs((output_directory + "profile_hist_theta_y_right_y_near.root").c_str()) ;

		profile_hist_theta_y_left_y_far->SaveAs((output_directory + "profile_hist_theta_y_left_y_far.root").c_str()) ;
		profile_hist_theta_y_right_y_far->SaveAs((output_directory + "profile_hist_theta_y_right_y_far.root").c_str()) ;

		// Rotated

		profile_hist_theta_y_right_y_near_rotated->SaveAs((output_directory + "profile_hist_theta_y_right_y_near_rotated.root").c_str()) ;

		// Left - right correlations

		hist_y_near_left_right->SaveAs((output_directory + "hist_y_near_left_right.root").c_str()) ;
		hist_y_far_left_right->SaveAs((output_directory + "hist_y_far_left_right.root").c_str()) ;

		hist_x_near_left_right->SaveAs((output_directory + "hist_x_near_left_right.root").c_str()) ;
		hist_x_far_left_right->SaveAs((output_directory + "hist_x_far_left_right.root").c_str()) ;

		hist_theta_x_left_right->SaveAs((output_directory + "hist_theta_x_left_right.root").c_str()) ;

		// Angular correlations

		hist_theta_x_left_x_near_right->SaveAs((output_directory + "hist_theta_x_left_x_near_right.root").c_str()) ;
		hist_theta_x_right_x_near_left->SaveAs((output_directory + "hist_theta_x_right_x_near_left.root").c_str()) ;
		
		// ######################################
		// Summary plots
		// ######################################

		if(!only_good_shaped_profiles || (filename_index != 6))
		{

			// Near

			string draw_option = "same" ;

			if(filename_index == 0) draw_option = "" ;

			profile_hist_theta_y_left_y_near_summary->cd() ;
			
			profile_hist_theta_y_left_y_near->SetLineColor(color_index) ;
			profile_hist_theta_y_right_y_near->SetLineColor(color_index) ;

			profile_hist_theta_y_left_y_near->SetMarkerColor(color_index) ;
			profile_hist_theta_y_right_y_near->SetMarkerColor(color_index) ;

			profile_hist_theta_y_left_y_near->SetMarkerStyle(20) ;
			profile_hist_theta_y_right_y_near->SetMarkerStyle(20) ;
			
			profile_hist_theta_y_left_y_near->Draw(draw_option.c_str()) ;
			profile_hist_theta_y_right_y_near->Draw("same") ;
			
			// Far

			profile_hist_theta_y_left_y_far_summary->cd() ;
			
			profile_hist_theta_y_left_y_far->SetLineColor(color_index) ;
			profile_hist_theta_y_right_y_far->SetLineColor(color_index) ;

			profile_hist_theta_y_left_y_far->SetMarkerColor(color_index) ;
			profile_hist_theta_y_right_y_far->SetMarkerColor(color_index) ;

			profile_hist_theta_y_left_y_far->SetMarkerStyle(20) ;
			profile_hist_theta_y_right_y_far->SetMarkerStyle(20) ;
			
			profile_hist_theta_y_left_y_far->Draw(draw_option.c_str()) ;
			profile_hist_theta_y_right_y_far->Draw("same") ;
		}
		

			if(!only_good_shaped_profiles || (filename_index != 6)) number_of_points_in_summary_graph++ ;
		
	}
	
	profile_hist_theta_y_left_y_near_summary->SaveAs("profile_hist_theta_y_left_y_near_summary.root") ;
	profile_hist_theta_y_left_y_far_summary->SaveAs("profile_hist_theta_y_left_y_far_summary.root") ;

	results_with_far_left->Fit("pol0") ;
	results_with_far_right->Fit("pol0") ;
	results_with_near_left->Fit("pol0") ;
	results_with_near_right->Fit("pol0") ;
	
	results_with_far_left->SetDrawOption("P") ;
	results_with_far_right->SetDrawOption("P") ;
	results_with_near_left->SetDrawOption("P") ;
	results_with_near_right->SetDrawOption("P") ;
	
	results_with_far_left->SetMarkerStyle(20) ;
	results_with_far_right->SetMarkerStyle(20) ;
	results_with_near_left->SetMarkerStyle(20) ;
	results_with_near_right->SetMarkerStyle(20) ;
	
	results_with_far_left->SaveAs("results_with_far_left.root") ;
	results_with_far_right->SaveAs("results_with_far_right.root") ;
	results_with_near_left->SaveAs("results_with_near_left.root") ;
	results_with_near_right->SaveAs("results_with_near_right.root") ;
}
