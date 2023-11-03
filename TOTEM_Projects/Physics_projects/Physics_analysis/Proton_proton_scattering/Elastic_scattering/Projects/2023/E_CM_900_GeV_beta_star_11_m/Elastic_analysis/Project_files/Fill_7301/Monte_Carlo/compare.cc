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
#include "TRandom3.h"
#include "TMinuit.h"
#include "TStyle.h"
#include "TFitResult.h"

void condor()
{
	string file1 = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/Generic_MC_reference.root" ;
	string file2 = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/Generic_MC_reference_with_beam_divergence.root" ;
	string file3 = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/Generic_MC.root" ;
	string file4 = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/Generic_MC_with_beam_divergence.root" ;
	string file5 = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/Generic_MC_with_beam_divergence_stronger_cuts.root" ;
	
	TFile *file_1 = TFile::Open(file1.c_str()) ;
	TFile *file_2 = TFile::Open(file2.c_str()) ;
	TFile *file_3 = TFile::Open(file3.c_str()) ;
	TFile *file_4 = TFile::Open(file4.c_str()) ;
	TFile *file_5 = TFile::Open(file5.c_str()) ;
	
	TH2D *hist_1 = (TH2D *)file_1->Get("P0042_PlotsCollection_phi_star_rad_theta_star_rad") ;
	TH2D *hist_2 = (TH2D *)file_2->Get("P0042_PlotsCollection_phi_star_rad_theta_star_rad") ;
	TH2D *hist_3 = (TH2D *)file_3->Get("P0046_PlotsCollection_phi_star_rad_theta_star_rad") ;
	TH2D *hist_4 = (TH2D *)file_4->Get("P0046_PlotsCollection_phi_star_rad_theta_star_rad") ;
	TH2D *hist_5 = (TH2D *)file_5->Get("P0046_PlotsCollection_phi_star_rad_theta_star_rad") ;
	
	cout <<  hist_1 << " " <<  hist_2 << " " <<  hist_3 << " " <<  hist_4 << " " <<  hist_5 << endl ;
	
	hist_1->GetXaxis()->SetRangeUser(-2, 8) ;
	hist_2->GetXaxis()->SetRangeUser(-2, 8) ;
	hist_3->GetXaxis()->SetRangeUser(-2, 8) ;
	hist_4->GetXaxis()->SetRangeUser(-2, 8) ;
	hist_5->GetXaxis()->SetRangeUser(-2, 8) ;

	hist_1->GetYaxis()->SetRangeUser(-5e-4, 15e-4) ;
	hist_2->GetYaxis()->SetRangeUser(-5e-4, 15e-4) ;
	hist_3->GetYaxis()->SetRangeUser(-5e-4, 15e-4) ;
	hist_4->GetYaxis()->SetRangeUser(-5e-4, 15e-4) ;
	hist_5->GetYaxis()->SetRangeUser(-5e-4, 15e-4) ;
	
	TCanvas c2 ;
   c2.SetLogz() ;
	
	hist_1->Draw("colz") ;
	c2.SaveAs("plots/condor/hist_1.pdf") ;

	hist_2->Draw("colz") ;
	c2.SaveAs("plots/condor/hist_2.pdf") ;

	hist_3->Draw("colz") ;
	c2.SaveAs("plots/condor/hist_3.pdf") ;

	hist_4->Draw("colz") ;
	c2.SaveAs("plots/condor/hist_4.pdf") ;

	hist_5->Draw("colz") ;	
	c2.SaveAs("plots/condor/hist_5.pdf") ;

}

int main()
{
	condor() ;

	const int test_of_vertical_acceptance_cut_without_divergence = 1 ;
	const int test_of_vertical_acceptance_cut_with_divergence = 2 ;
	
	int scenario = test_of_vertical_acceptance_cut_with_divergence ;
	
	string filename1 = "" ;
	string filename2 = "" ;
	
	string histname1 = "" ;
	string histname2 = "" ;
	
	string suffix = "" ;
	
	if(scenario == test_of_vertical_acceptance_cut_without_divergence)
	{
		filename1 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc_reference/Generic.root" ;
		filename2 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc/Generic.root" ;
		
		histname1= "P0025_PlotsCollection_dN_dt_GeV2" ;
		histname2= "P0031_PlotsCollection_dN_dt_GeV2_geometrical_acceptance_corrected" ;
		
		suffix = "1" ;
	}
	else if(scenario == test_of_vertical_acceptance_cut_with_divergence)
	{
		filename1 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc_reference_with_beam_divergence/Generic.root" ;
		filename2 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc_with_beam_divergence/Generic.root" ;
		
		histname1= "P0025_PlotsCollection_dN_dt_GeV2" ;
		histname2= "P0031_PlotsCollection_dN_dt_GeV2_geometrical_acceptance_corrected" ;
		histname2= "P0032_PlotsCollection_dN_dt_GeV2_geometrical_acceptance_corrected_beam_divergence_corrected" ;

		suffix = "2" ;
	}
	else
	{
		cout << "There is no such scenario!" << endl ;
		exit(1) ;
	}


	TFile *myfile_reference = TFile::Open(filename1.c_str()) ;
	TFile *myfile_accep_corrected = TFile::Open(filename2.c_str()) ;
	
	TH1D *reference = (TH1D *)myfile_reference->Get(histname1.c_str()) ;
	TH1D *accep_corrected = (TH1D *)myfile_accep_corrected->Get(histname2.c_str()) ;
	
	cout << reference << endl ;
	cout << accep_corrected << endl ;
	
	TCanvas c ;
	
	reference->Draw() ;
	accep_corrected->Draw("same") ;
	
	c.SaveAs(("plots/test_1_" + suffix + ".root").c_str()) ;
	c.SaveAs(("plots/test_1_" + suffix + ".pdf").c_str()) ;
	
	accep_corrected->Divide(reference) ;

	accep_corrected->Draw("s") ;

	c.SaveAs(("plots/test_2_" + suffix + ".root").c_str()) ;
	c.SaveAs(("plots/test_2_" + suffix + ".pdf").c_str()) ;
	
	cout << "Finished" << endl ;
}
