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
#include "TMultiGraph.h"

#include <iomanip>

const string diag_LBRT = "DIAGONAL_LEFT_BOTTOM_RIGHT_TOP" ;
const string diag_LTRB = "DIAGONAL_LEFT_TOP_RIGHT_BOTTOM" ;
const string basic_path = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/" ;
const string basic_path_2 = "/All_root_files_to_define_cuts_run_" ;

const string basic_path_3 = "/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Project_files/Per_run/" ;

const string basic_path_4_LBRT = "Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_" ;
const string basic_path_4_LTRB = "Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_run_" ;

TCanvas *main_canvas = NULL ;

void test(string histoname)
{

	vector<string> plotnames ;
	plotnames.push_back("P0045_PlotsCollection_theta_y_star_left_rad_theta_y_star_right_rad_define_cut_rotated") ;
	plotnames.push_back("P0046_PlotsCollection_y_mm_near_dy_mm_left_define_cut_rotated") ;
	plotnames.push_back("P0047_PlotsCollection_y_mm_near_dy_mm_right_define_cut_rotated") ;
	plotnames.push_back("P0048_PlotsCollection_x_star_left_mm_x_star_right_mm_define_cut_rotated") ;
	plotnames.push_back("P0049_PlotsCollection_x_mm_near_dx_mm_left_define_cut_rotated") ;
	plotnames.push_back("P0050_PlotsCollection_y_star_left_mm_y_star_right_mm_define_cut_rotated") ;
	plotnames.push_back("P0051_PlotsCollection_x_mm_near_dx_mm_right_define_cut_rotated") ;
	plotnames.push_back("P0052_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_define_cut_rotated") ;

   gErrorIgnoreLevel = kWarning ;

	string files = "../../../../General_settings/List_of_runs.txt" ;
	
	ifstream myfiles(files) ;
	
	string word ;
	
	TCanvas c ;
//	c.SetLogy() ;			

  double maxmeanperrms = 0 ;		
  double minmeanperrms = 1e10 ;		
	
	while(myfiles >>  word)
	{
		cout << word << endl ;
		
		string actual_filename = basic_path + word + "/Generic.root" ;
		
		std::cout << actual_filename << endl ;
		
		TFile *myroot = TFile::Open(actual_filename.c_str()) ;
		
		TH1D *hist1 = ((TH1D *)myroot->Get("P0027_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_1_distance_from_cut")) ;
		TH1D *hist2 = ((TH1D *)myroot->Get("P0029_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_2_distance_from_cut")) ;
		TH1D *hist3 = ((TH1D *)myroot->Get("P0031_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_3_distance_from_cut")) ;
		TH1D *hist4 = ((TH1D *)myroot->Get("P0033_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_4_distance_from_cut")) ;
		TH1D *hist5 = ((TH1D *)myroot->Get("P0035_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_5_distance_from_cut")) ;
		TH1D *hist6 = ((TH1D *)myroot->Get("P0037_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_6_distance_from_cut")) ;
		TH1D *hist7 = ((TH1D *)myroot->Get("P0039_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_7_distance_from_cut")) ;
		TH1D *hist8 = ((TH1D *)myroot->Get("P0041_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_8_distance_from_cut")) ;
		TH1D *hist9 = ((TH1D *)myroot->Get("P0043_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_9_distance_from_cut")) ;

		
		if(hist1 != NULL)
		{
			hist1->Draw() ;
			hist2->Draw("same") ;
			hist3->Draw("same") ;
			hist4->Draw("same") ;
			hist5->Draw("same") ;
			hist6->Draw("same") ;
			hist7->Draw("same") ;
			hist8->Draw("same") ;
			hist9->Draw("same") ;
			
			hist1->GetXaxis()->SetRangeUser(-0.0005, 0.0005) ;
			
			c.SaveAs(("fig/fig_" + word + ".png").c_str()) ;
			
			double int1 = hist1->Integral() ;
			double int2 = hist9->Integral() ;
			
			cout << "ratio " << word << " " << (int2 / int1) << endl ;

			TH1D *hist10 = ((TH1D *)myroot->Get("P0027_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_draw_cut_1")) ;
			TH1D *hist11 = ((TH1D *)myroot->Get("P0028_PlotsCollection_theta_y_star_left_rad_theta_y_star_right_rad_with_cut")) ;
// P0030_PlotsCollection_y_mm_near_dy_mm_left_with_cut
// P0032_PlotsCollection_y_mm_near_dy_mm_right_with_cut
// P0034_PlotsCollection_x_star_left_mm_x_star_right_mm_with_cut
// P0036_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_with_cut


			TH1D *hist12 = ((TH1D *)myroot->Get((histoname + "_rotated").c_str())) ;
			TH1D *hist13 = ((TH1D *)myroot->Get(histoname.c_str())) ;
			
			int entries = hist13->GetEntries() ;
			cout << histoname << "entries " << entries << endl ;
			
			// cout << "mean " << hist12->GetMean(2)<< endl ;
			// cout << "RMS " << hist12->GetRMS(2)<< endl ;

			double meanperrms = fabs(hist12->GetMean(2)/hist12->GetRMS(2)) ;
			
			if(maxmeanperrms < meanperrms) maxmeanperrms = meanperrms ;
			if(minmeanperrms > meanperrms) minmeanperrms = meanperrms ;

			cout << "meanperrms: " << meanperrms << endl ;

			hist10->Draw("colz") ;
			c.SaveAs(("fig/fig_x_" + word + ".png").c_str()) ;		

			hist11->Draw("colz") ;
			c.SaveAs(("fig/fig_y_" + word + ".png").c_str()) ;		

		}

	}

	cout << histoname << " finalmaxmeanperrms: " << maxmeanperrms << endl ;
	cout << histoname << " finalminmeanperrms: " << minmeanperrms << endl ;
}

bool test3(string filename, string histoname, string prj_filename, string run_number)
{
	TFile *myroot = TFile::Open(filename.c_str()) ;
	bool found = false ;
	
	bool cut_is_present_in_file = false ;
	
	const bool save_figs = false ;

	if(myroot != NULL)
	{
		// cout << filename << "ok" << endl ;

		TCanvas *hist_basic = ((TCanvas *)myroot->Get((histoname + "_canvas").c_str())) ;
		TH2D *hist = ((TH2D *)myroot->Get((histoname + "_rotated").c_str())) ;
		
		if(save_figs && (hist != NULL))
		{
			if(main_canvas == NULL)
			{
				cout << "No canvas" << endl ;
				exit(1) ;
			}


			hist_basic->Draw("colz") ;
			hist_basic->SaveAs(("fig/" + run_number + histoname + ".png").c_str()) ;

			main_canvas->cd() ;
			hist->Draw("colz") ;

			int nbins = hist->GetYaxis()->GetNbins() ;
			double lo = fabs(hist->GetYaxis()->GetBinCenter(1) * 0.2) ;

			hist->GetYaxis()->SetRangeUser(-lo, lo) ;
			main_canvas->SaveAs(("fig/" + run_number + histoname + "_rotated" + ".png").c_str()) ;
		}

		if(hist != NULL)
		{
			double myRMS = hist->GetRMS(2) ;
			double myMean = hist->GetMean(2) ;
			double ratio = fabs(myMean / myRMS) ;

			if(ratio > 0.2) cout << "ratio " << ratio << " " << prj_filename << endl ;

			myroot->Close() ;
			
			ifstream project_file(prj_filename.c_str()) ;
		   // cout << prj_filename << " ok" << endl ;
			
			string word = "" ;
			int length = histoname.length() - 11 - 22 ;
			string cut_name = histoname.substr(22, length) + "_cut_block" ;
			double RMS_in_prj_file = myRMS ;
			
			while(project_file >> word)
			{
				if(word.compare(cut_name) == 0)
				{
				
							cut_is_present_in_file = true ;

							project_file >> word ;
							project_file >> word ;
							project_file >> RMS_in_prj_file ;

							// cout << "found " << RMS_in_prj_file << endl ;
							
							while(project_file >> word) if(word.compare("<end>")) break ;
				}
			}
			
			if((fabs(RMS_in_prj_file - myRMS) / RMS_in_prj_file) > 0.01)
			{
				found = true ;
				cout << "here" << endl ;
			}
			
			
			project_file.close() ;
		}
			
	}
	
	if(!cut_is_present_in_file) cout << "Warning: cut is absent fir " << histoname << " in " << prj_filename << endl ;
	
	return found ;
}

void test2(string filename, string histoname, ofstream &project_file2)
{
	TFile *myroot = TFile::Open(filename.c_str()) ;
	
	if(myroot != NULL)
	{
		cout << filename << "ok" << endl ;

		TH2D *hist = ((TH2D *)myroot->Get((histoname + "_rotated").c_str())) ;

		project_file2 << endl << endl ;

		if(hist != NULL)
		{
			double myRMS = hist->GetRMS(2) ;

			int length = histoname.length() - 11 - 22 ;
			string cut_name = histoname.substr(22, length) + "_cut_block" ;

			string spacing = "" ;
			for(int j = 0 ; j < 50 - length ; ++j) spacing += " " ;

			project_file2 << "<begin> " << cut_name << endl ;
			project_file2 << "  <update_parameter> sigma " << myRMS << endl ;
			project_file2 << "<end>   " << cut_name << endl << endl ;

		}

		myroot->Close() ;
	}
}


int main()
{
	main_canvas = new TCanvas() ;
	main_canvas->SetGridx() ;
	main_canvas->SetGridy() ;

	vector<string> plotnames ;
	plotnames.push_back("P0045_PlotsCollection_theta_y_star_left_rad_theta_y_star_right_rad_define_cut") ;
	plotnames.push_back("P0046_PlotsCollection_y_mm_near_dy_mm_left_define_cut") ;
	plotnames.push_back("P0047_PlotsCollection_y_mm_near_dy_mm_right_define_cut") ;
	plotnames.push_back("P0048_PlotsCollection_x_star_left_mm_x_star_right_mm_define_cut") ;
	plotnames.push_back("P0049_PlotsCollection_x_mm_near_dx_mm_left_define_cut") ;
	// plotnames.push_back("P0050_PlotsCollection_y_star_left_mm_y_star_right_mm_define_cut") ;
	plotnames.push_back("P0051_PlotsCollection_x_mm_near_dx_mm_right_define_cut") ;
	plotnames.push_back("P0052_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_define_cut") ;

	const int iteration = 1 ;

   string post_fix1 = "" ;	
	string post_fix2 = "_new" ;	

	string files = "../../../../General_settings/List_of_runs.txt" ;

	string word ;
	ifstream myfiles(files) ;

	for(int i = 1 ; i < iteration ; ++i)
	{
		post_fix1 += "_new" ;
		post_fix2 += "_new" ;
	}

	while(myfiles >>  word)
	{
		
	
		string actual_filename = basic_path + diag_LBRT + basic_path_2 + word + "/Generic.root" ;
		string prj_filename = (basic_path_3 + basic_path_4_LBRT + word + post_fix1 + ".prj" ) ;
		string prj_filename_new = (basic_path_3 + basic_path_4_LBRT + word + post_fix2 + ".prj" ) ;
		
		bool mytest = false ;

		for(int i = 0 ; i < plotnames.size() ; ++i) mytest |= test3(actual_filename, plotnames[i], prj_filename, word) ;
		
		if(mytest) cout << "To be corrected: " << word << endl ;
		if(!mytest) cout << "Allright " << word << endl ;

		if(false)
		{		
			ifstream project_file(prj_filename.c_str()) ;
			ofstream project_file2(prj_filename_new.c_str()) ;
			string myline ;

			while(getline(project_file, myline)) 
			{
				// cout << myline << endl ;
				project_file2 << myline << endl ;
			}

			if(mytest) for(int i = 0 ; i < plotnames.size() ; ++i) test2(actual_filename, plotnames[i], project_file2) ;

			project_file.close() ;
			project_file2.close() ;

			cout << endl ;
		}
	}
}
