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
#include "TPave.h"
#include "TBox.h"
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
		TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP_2RP/All_root_files_to_define_cuts_run_" + word + "/Generic.root").c_str()) ;
		// TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_" + word + "/Generic.root").c_str()) ;
		
		if(file == NULL) continue ;
		
		TH1D *hist = ((TH1D *)file->Get("P0018_PlotsCollection_theta_x_star_left_far_rad_theta_x_star_right_far_rad_with_cut_distance_from_cut")) ;

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

map<string, double> reference_offsets_LBRT ;
map<string, double> reference_offsets_LTRB ;

bool write_file = false ;
// bool write_file = true ;

void horizontal_elastic_alignment_per_run(string run_to_test)
{
	gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);
	gStyle->SetOptStat("ne");

	gStyle->SetPalette(kCividis);

	// ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;
	// string word ;
	// bool first = true ;
	// while(runs >> word)

	TCanvas c;
	c.SetLogz() ;
	
	const int scenario_null = 0 ;
	const int scenario_LBRT = 1 ;
	const int scenario_LTRB = 2 ;
	
	int scenario = scenario_LBRT ;
	
	TFile *file = NULL ;
	
	int additional_sign = 1 ;

	stringstream ss ;

	string text_for_filename = "" ;

	if(scenario == scenario_LBRT)
	{
		file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;
		additional_sign = -1 ;
	   text_for_filename = "LBRT" ;

		cout << "Diagonal LBRT " << run_to_test << endl ;
	}
	else if(scenario == scenario_LTRB)
	{
		file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;
	   text_for_filename = "LTRB" ;

		cout << "Diagonal LTRB " << run_to_test << endl ;
	}

	vector<string> histograms ;

	histograms.push_back("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP") ;
	histograms.push_back("P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP") ;
	histograms.push_back("P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP") ;
	histograms.push_back("P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP") ;

	const double width_mm = 1.5 ;
	const double pos_l_mm = 6 ;
	const double pos_u_mm = 20 ;

	string corr_filename = "corrections_LBRT_" + run_to_test + ".prj" ;
	if(scenario == scenario_LTRB) corr_filename = "corrections_LTRB_" + run_to_test + ".prj" ;

	ofstream correction_file ;

	if(write_file) correction_file.open("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/" + corr_filename) ;

	if(write_file && (run_to_test.compare("324536") != 0)) correction_file << "<begin> alignment_block" << endl ;

	for(int i = 0 ; i < histograms.size() ; ++i)
	{
		int sign = additional_sign ;

		if((histograms[i].substr(0, 5) == "P0027") || (histograms[i].substr(0, 5) == "P0028")) sign = (-1.0 * additional_sign) ;

		TH2D *hist1 = ((TH2D *)file->Get(histograms[i].c_str())) ;

		// cout << "min " << hist1->FindBin(0,-1) << endl ;
		// cout << "max " << hist1->FindBin(0, 1) << endl ;
		const int bin_min = hist1->FindBin(0,-1) ;
		const int bin_max = hist1->FindBin(0, 1) ;

		for(int j = bin_min ; j <= bin_max ; ++j)
		{
			hist1->SetBinContent(j, 0) ;
			// hist1->SetBinContent(j, 1e6) ;
		}

		double w_l = hist1->GetYaxis()->FindBin(sign * pos_l_mm) ;
		double w_u = hist1->GetYaxis()->FindBin(sign * pos_u_mm) ;

		TH1D *hist_1_proj = NULL ;

		if(sign == 1) hist_1_proj = hist1->ProjectionX("px1", w_l, w_u) ;
		else hist_1_proj = hist1->ProjectionX("px1", w_u, w_l) ;

		hist_1_proj->Rebin(4) ;

		double mean = 0.0 ;
		double meane = 0.0 ;

		for(int i = 0 ; i < 10 ; ++i)
		{
			TFitResultPtr fit_result = hist_1_proj->Fit("gaus", "SQ", "", -width_mm + mean, width_mm + mean) ;
			hist_1_proj->GetFunction("gaus")->SetLineWidth(1) ;
			mean = fit_result->Parameter(1) ;
			meane = fit_result->ParError(1) ;
		}

		string warning = "" ;

		bool write_info = false ;

		if((fabs(mean) > fabs(meane)) || (fabs(mean) > 2e-3))
		{
			warning = "(to be checked)" ;

			write_info = true ;
		}

		const double fraction = 2.0 ;

		cout << "Mean: " << histograms[i] << " \t\t" << mean << "\t\t +/- \t\t" << meane << " " << warning << endl ;

		string alignment_string = "" ;

		if(histograms[i].compare("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP") == 0)	 alignment_string = "RP_alignment_left_near_x_mm" ;
		if(histograms[i].compare("P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP") == 0)   alignment_string = "RP_alignment_left_far__x_mm" ;
		if(histograms[i].compare("P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP") == 0) alignment_string = "RP_alignment_right_near_x_mm" ;
		if(histograms[i].compare("P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP") == 0)  alignment_string = "RP_alignment_right_far__x_mm" ;

		if(write_info && scenario == scenario_LBRT) cout << alignment_string << " " << reference_offsets_LBRT[histograms[i]] - (mean / fraction) << endl ;
		if(write_info && scenario == scenario_LTRB) cout << alignment_string << " " << reference_offsets_LTRB[histograms[i]] - (mean / fraction) << endl ;

		hist1->Draw("colz") ;

		TBox *box = new TBox(-width_mm + mean, (sign*pos_l_mm), width_mm + mean, (sign*pos_u_mm)) ;

		// cout << (-width_mm + mean) << " " << (sign*pos_l_mm) << " " << (width_mm + mean) << " " << (sign*pos_u_mm) << endl ;
		box->SetFillStyle(0) ;
		box->Draw("same") ;

		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".png").c_str()) ;
		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".pdf").c_str()) ;
		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".root").c_str()) ;

		hist_1_proj->Draw("") ;
		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.png").c_str()) ;
		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.pdf").c_str()) ;
		c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.root").c_str()) ;

		hist_1_proj->Delete() ;

		if(write_file && (run_to_test.compare("324536") != 0))
		{
			if(write_info)
			{
				if(scenario == scenario_LBRT) correction_file << "\t <update_parameter> " << alignment_string << "\t" << reference_offsets_LBRT[histograms[i]] - (mean / fraction) << endl ;
				if(scenario == scenario_LTRB) correction_file << "\t <update_parameter> " << alignment_string << "\t" << reference_offsets_LTRB[histograms[i]] - (mean / fraction) << endl ;
			}
			else
			{
				if(scenario == scenario_LBRT) correction_file << "\t <update_parameter> " << alignment_string << "\t" << reference_offsets_LBRT[histograms[i]] << endl ;
				if(scenario == scenario_LTRB) correction_file << "\t <update_parameter> " << alignment_string << "\t" << reference_offsets_LTRB[histograms[i]] << endl ;
			}
		}
	}

	if(write_file && (run_to_test.compare("324536") != 0)) correction_file << "<end> alignment_block" << endl ;

	correction_file.close() ;
	
	cout << endl ;
}

void load_runs()
{
	string word ;

	ifstream cuts_LBRT("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Fill_7291/Cuts_test_left_bottom_right_top_4_sigma_2RP.prj") ;

	while(cuts_LBRT >> word)
	{
		if(word.substr(0, 13).compare("RP_alignment_") == 0)
		{
			double param ;
			cuts_LBRT >> param ;

			if(word.compare("RP_alignment_left_near_x_mm") == 0) reference_offsets_LBRT["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] = param ;
			if(word.compare("RP_alignment_left_far__x_mm") == 0) reference_offsets_LBRT["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_near_x_mm") == 0) reference_offsets_LBRT["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_far__x_mm") == 0) reference_offsets_LBRT["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] = param  ;
		}
	}

	cuts_LBRT.close() ;

	cout << reference_offsets_LBRT["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] << endl ;
   cout << endl ;

	ifstream cuts_LTRB("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Fill_7291/Cuts_test_left_top_right_bottom_4_sigma_2RP.prj") ;

	while(cuts_LTRB >> word)
	{
		if(word.substr(0, 13).compare("RP_alignment_") == 0)
		{
			double param ;
			cuts_LTRB >> param ;

			if(word.compare("RP_alignment_left_near_x_mm") == 0) reference_offsets_LTRB["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] = param ;
			if(word.compare("RP_alignment_left_far__x_mm") == 0) reference_offsets_LTRB["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_near_x_mm") == 0) reference_offsets_LTRB["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_far__x_mm") == 0) reference_offsets_LTRB["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] = param  ;
		}
	}

	cuts_LTRB.close() ;

	cout << reference_offsets_LTRB["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] << endl ;
   cout << endl ;

}

void load_runs(string run)
{
	string word ;

	reference_offsets_LBRT.clear() ;
	reference_offsets_LTRB.clear() ;

	ifstream cuts_LBRT("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Temp/corrections_LBRT_"+ run + ".prj") ;

	while(cuts_LBRT >> word)
	{
		if(word.substr(0, 13).compare("RP_alignment_") == 0)
		{
			double param ;
			cuts_LBRT >> param ;

			if(word.compare("RP_alignment_left_near_x_mm") == 0) reference_offsets_LBRT["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] = param ;
			if(word.compare("RP_alignment_left_far__x_mm") == 0) reference_offsets_LBRT["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_near_x_mm") == 0) reference_offsets_LBRT["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_far__x_mm") == 0) reference_offsets_LBRT["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] = param  ;
		}
	}

	cuts_LBRT.close() ;

	cout << reference_offsets_LBRT["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] << endl ;
	cout << reference_offsets_LBRT["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] << endl ;
   cout << endl ;

	ifstream cuts_LTRB("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Temp/corrections_LTRB_"+ run + ".prj") ;

	while(cuts_LTRB >> word)
	{
		if(word.substr(0, 13).compare("RP_alignment_") == 0)
		{
			double param ;
			cuts_LTRB >> param ;

			if(word.compare("RP_alignment_left_near_x_mm") == 0) reference_offsets_LTRB["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] = param ;
			if(word.compare("RP_alignment_left_far__x_mm") == 0) reference_offsets_LTRB["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_near_x_mm") == 0) reference_offsets_LTRB["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] = param  ;
			if(word.compare("RP_alignment_right_far__x_mm") == 0) reference_offsets_LTRB["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] = param  ;
		}
	}

	cuts_LTRB.close() ;

	cout << reference_offsets_LTRB["P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP"] << endl ;
	cout << reference_offsets_LTRB["P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP"] << endl ;
   cout << endl ;

}


void horizontal_elastic_alignment()
{
	// load_runs() ;

	string word ;

	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;
	
	while(runs >> word)
	{
		if(word.compare("324457") == 0) continue ;

		load_runs(word) ;

		horizontal_elastic_alignment_per_run(word) ;
	}
}

int main()
{
	gStyle->SetOptStat("");

	gErrorIgnoreLevel = kError ;
	
	const int main_scenario_acceptance_polygons = 1 ;
	const int main_scenario_signal_to_bkg_for_2RP = 2 ;
	const int main_scenario_elastic_alignment = 3 ;

	int main_scenario = main_scenario_elastic_alignment ;

	if(main_scenario == main_scenario_acceptance_polygons) main_studies() ;
	else if(main_scenario == main_scenario_signal_to_bkg_for_2RP) other_studies() ;
	else if(main_scenario == main_scenario_elastic_alignment) horizontal_elastic_alignment() ;
}
