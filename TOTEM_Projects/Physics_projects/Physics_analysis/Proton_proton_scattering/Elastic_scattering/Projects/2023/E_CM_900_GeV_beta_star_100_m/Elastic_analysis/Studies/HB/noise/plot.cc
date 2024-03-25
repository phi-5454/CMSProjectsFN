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

const string basic_path = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_run_" ;

main()
{
   gErrorIgnoreLevel = kWarning ;

	string files = "../../../../General_settings/List_of_runs.txt" ;
	
	ifstream myfiles(files) ;
	
	string word ;
	
	TCanvas c ;
//	c.SetLogy() ;			
	
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
		}
	}
}
