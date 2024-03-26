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

// const string basic_path = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_run_" ;
const string basic_path = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM/All_root_files_to_define_cuts_run_" ;

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

int main()
{

	vector<string> plotnames ;
	plotnames.push_back("P0045_PlotsCollection_theta_y_star_left_rad_theta_y_star_right_rad_define_cut") ;
	plotnames.push_back("P0046_PlotsCollection_y_mm_near_dy_mm_left_define_cut") ;
	plotnames.push_back("P0047_PlotsCollection_y_mm_near_dy_mm_right_define_cut") ;
	plotnames.push_back("P0048_PlotsCollection_x_star_left_mm_x_star_right_mm_define_cut") ;
	plotnames.push_back("P0049_PlotsCollection_x_mm_near_dx_mm_left_define_cut") ;
	plotnames.push_back("P0050_PlotsCollection_y_star_left_mm_y_star_right_mm_define_cut") ;
	plotnames.push_back("P0051_PlotsCollection_x_mm_near_dx_mm_right_define_cut") ;
	plotnames.push_back("P0052_PlotsCollection_theta_x_star_left_rad_theta_x_star_right_rad_define_cut") ;

   gErrorIgnoreLevel = kWarning ;
	
	for(int i = 0 ; i < plotnames.size() ; ++i) test(plotnames[i]) ;

}
