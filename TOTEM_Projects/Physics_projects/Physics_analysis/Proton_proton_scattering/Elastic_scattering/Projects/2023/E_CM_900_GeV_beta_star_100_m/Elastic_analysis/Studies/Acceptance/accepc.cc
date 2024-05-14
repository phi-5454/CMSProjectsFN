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

TGraph graph2 ;

TGraph graph2_golden ;
TGraph graph2_normal ;
TGraph graph2_bad ;

const int scenario_null = 0 ;
const int scenario_LBRT = 1 ;
const int scenario_LTRB = 2 ;

int scenario = scenario_LBRT ;

void horizontal_elastic_alignment_per_run(string run_to_test, int type)
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

		bool save_figures = false ;

		int run_to_test_int = atoi(run_to_test.c_str()) ;

		graph2.AddPoint(run_to_test_int, mean) ;

		if(type == 1) graph2_golden.AddPoint(run_to_test_int, mean) ;
		if(type == 2) graph2_normal.AddPoint(run_to_test_int, mean) ;
		if(type == 3) graph2_bad.AddPoint(run_to_test_int, mean) ;

		if(save_figures)
		{
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".png").c_str()) ;
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".pdf").c_str()) ;
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + ".root").c_str()) ;

			hist_1_proj->Draw("") ;
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.png").c_str()) ;
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.pdf").c_str()) ;
			c.SaveAs(("plots/alignment/" + run_to_test + "_" + text_for_filename + "_" + histograms[i] + "_proj_fit.root").c_str()) ;
			hist_1_proj->Delete() ;

		}

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


const int align_fit_standard = 1 ;
const int align_fit_with_coulomb = 2 ;

const int align_fit_scenario = align_fit_with_coulomb ;

Double_t my_gaus(Double_t *x, Double_t *par)
{
			double myarg = ((x[0] - par[1]) / par[2]) ;
			double myarg2 = ((x[0] - par[1]) / par[4]) ;

        double f1 = (par[0] * exp(-0.5 * (myarg * myarg))) ;
        double f2 = (par[3] * exp(-0.5 * (myarg2 * myarg2))) ;

        double f = f1 ;
		  
		  if(align_fit_scenario == align_fit_with_coulomb) f = f1 + f2 ;

      // cout << "energy: " << x[0] << " " << f << endl ;

        return f ;
}

TH1D *hist_to_fit = NULL ;

double lo_x = 12 ;
double hi_x = 20 ;

double lo_x_standard = 12 ;
double hi_x_standard = 20 ;

double lo_x_coulomb = 8 ;
double hi_x_coulomb = 20 ;

double chi2_global = 0 ;
double ndf_global = 0 ;

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
	double chi2 = 0 ;

	chi2_global = 0 ;
	ndf_global = 0 ;

	for(int i = 0 ; i < hist_to_fit->GetNbinsX() ; ++i)
	{
		if(hist_to_fit->GetBinError(i) != 0.0)
		{
			double y_pos_mm = hist_to_fit->GetBinCenter(i) ;

			if((lo_x < fabs(y_pos_mm)) && (fabs(y_pos_mm) < hi_x))
			{
				double value = hist_to_fit->GetBinContent(i) ;
				double unc = hist_to_fit->GetBinError(i) ;

				double func_value = my_gaus(&y_pos_mm, par) ;

				double delta = (value - func_value) / unc ;

				chi2 += delta*delta ;
				++ndf_global ;
			}
		}
	}

	f = chi2 ;
	chi2_global = chi2 ;
}

TCanvas acanvas ;

void vertical_elastic_alignment_per_run(string run_to_test, int type)
{
	TF1 *func = new TF1("func",  my_gaus, -30, 30, 5) ;
	func->SetLineColor(kBlue) ;

	double func_par[5] ;
	double func_pare[5] ;

	cout << "here" << endl ;

	TFile *file_LBRT = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;
	TFile *file_LTRB = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;

	cout << file_LBRT << " " << file_LTRB << endl ;

	vector<string> histograms ;

	histograms.push_back("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP") ;
	histograms.push_back("P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP") ;
	histograms.push_back("P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP") ;
	histograms.push_back("P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP") ;

	bool test_migrad_gaus_with_built_in = true ;

	for(int i = 0 ; i < histograms.size() ; ++i)
	{
		TMinuit *gMinuit2 = new TMinuit(10);
		gMinuit2->SetFCN(fcn);

		Double_t arglist[10];

		arglist[0] = -1 ;
		Int_t ierflg = 0 ;
		// gMinuit2->mnexcm("SET PRI", arglist ,1,ierflg);

		arglist[0] = 1 ;
		gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);


		arglist[0] = 0 ;
		arglist[1] = 3 ;
		arglist[2] = 1 ;

		TH2D *hist_1 = ((TH2D *)file_LBRT->Get(histograms[i].c_str())) ;
		TH2D *hist_2 = ((TH2D *)file_LTRB->Get(histograms[i].c_str())) ;

		hist_1->Add(hist_2) ;

		TH1D *hist_1_proj = NULL ;
		TH1D *hist_1_proj_clone = NULL ;

		hist_1_proj = hist_1->ProjectionY("py1") ;
		hist_1_proj_clone = ((TH1D *)hist_1_proj->Clone("clone")) ;
		hist_1_proj->SetLineColor(kGreen) ;
		hist_1_proj_clone->SetLineColor(kBlue) ;

		for(int j = 0 ; j < hist_1_proj_clone->GetNbinsX() ; ++j)
		{
			double bin_position = fabs(hist_1_proj->GetBinCenter(j)) ;

			if((lo_x_standard < bin_position) && (bin_position < hi_x_standard))
			{

			}
			else
			{
				if(test_migrad_gaus_with_built_in) hist_1_proj_clone->SetBinContent(j, 0) ;
				if(test_migrad_gaus_with_built_in) hist_1_proj_clone->SetBinError(j, 0) ;
			}

		}

		hist_1->SaveAs(("plots/vertical_alignment/hist_1_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;

		hist_to_fit = hist_1_proj ;
		TFitResultPtr fit_result ;
		if(test_migrad_gaus_with_built_in) fit_result = hist_1_proj_clone->Fit("gaus", "SQ") ;

		if(test_migrad_gaus_with_built_in)
		if(fabs(fit_result->Parameter(1) > 0.2))
		{
			cout << "run_to_test " << run_to_test << " " << histograms[i] << " " << fit_result->Parameter(1) << endl ;
		}
		
		cout << "Start minimization " << run_to_test << endl ;

		gMinuit2->mnparm(0, "const", 100, 0.1, 0, 0, ierflg);
		gMinuit2->mnparm(1, "mean",  0, 0.1, 0, 0, ierflg);
		gMinuit2->mnparm(2, "sigma", 20, 0.1, 0, 0, ierflg);

	   arglist[0] = 20000 ;
		arglist[1] = 3 ;
	   arglist[2] = 1 ;
		
		chi2_global = 0 ;

		gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
		
		gMinuit2->GetParameter(0, func_par[0], func_pare[0]) ;
		gMinuit2->GetParameter(1, func_par[1], func_pare[1]) ;
		gMinuit2->GetParameter(2, func_par[2], func_pare[2]) ;

		double gaus_mean = func_par[1] ;
		double gaus_sigma = func_par[2] ;

		if(align_fit_scenario == align_fit_with_coulomb)		
		{
			gMinuit2->mnparm(0, "const", func_par[0], 0.1, 0, 0, ierflg);
			gMinuit2->mnparm(1, "mean",  func_par[1], 0.1, 0, 0, ierflg);
			gMinuit2->mnparm(2, "sigma", func_par[2], 0.1, 0, 0, ierflg);
			gMinuit2->mnparm(3, "const2", 100, 0.1, 0, 0, ierflg);
			gMinuit2->mnparm(4, "sigma2", 20, 0.1, 0, 0, ierflg);
			
		   arglist[0] = 20000 ;
			arglist[1] = 3 ;
	   	arglist[2] = 1 ;

			chi2_global = 0 ;
			ndf_global = 0 ;

			gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
			
			cout << "ierflg " << run_to_test << " " << ierflg << " " << ierflg << " " << endl ;
		}

		cout << "End minimization " << run_to_test << " " << histograms[i] << endl ;

		gMinuit2->GetParameter(0, func_par[0], func_pare[0]) ;
		gMinuit2->GetParameter(1, func_par[1], func_pare[1]) ;
		gMinuit2->GetParameter(2, func_par[2], func_pare[2]) ;

		if(align_fit_scenario == align_fit_with_coulomb)		
		{
			gMinuit2->GetParameter(3, func_par[3], func_pare[3]) ;
			gMinuit2->GetParameter(4, func_par[4], func_pare[4]) ;

			double double_gaus_mean = func_par[1] ;
			double double_gaus_sigma = func_par[2] ;

			double sigma_rel_diff = 100.0 * ((gaus_sigma - double_gaus_sigma) / double_gaus_sigma) ;

			double mean_diff = 1000.0 * ((gaus_mean - double_gaus_mean)) ;
			double mean_rel_diff = 100.0 * ((gaus_mean - double_gaus_mean) / double_gaus_mean) ;
			
			// cout << "sigma_rel_diff " << run_to_test << " " << std::fixed <<  std::setprecision(1) <<  std::setw(10) << " " << chi2_global " " << std::setw(10) << std::right  << double_gaus_sigma << " " << std::setw(10) << sigma_rel_diff << endl ;
			cout << "mean_rel_diff " << run_to_test << " " << std::fixed <<  std::setprecision(1) <<  std::setw(10) << " " << std::right << chi2_global << std::setw(10) << (double_gaus_mean * 1000.0) << " " <<  std::setw(10) << mean_diff << " " << std::setw(10) << std::setprecision(1) << mean_rel_diff << endl ;
		}

		if(test_migrad_gaus_with_built_in)
		{
			double delta_1 = 100.0 * fabs((fit_result->Parameter(0) - func_par[0]) / fit_result->Parameter(0)) ;
			double delta_2 = 100.0 * fabs((fit_result->Parameter(1) - func_par[1]) / fit_result->Parameter(1)) ;
			double delta_3 = 100.0 * fabs((fit_result->Parameter(2) - func_par[2]) / fit_result->Parameter(2)) ;

			if((delta_1 > 0.1) || (delta_2 > 0.1) || (delta_3 > 0.1))
			{
				cout << "Warning1: fit " << run_to_test + "_" + histograms[i] << "  " << delta_1 << "  " << delta_2 << "  " << delta_3 << endl ;
			}

			if(delta_3 > 0.1)
			{
				cout << "Warning2: fit " << run_to_test + "_" + histograms[i] << "  " << delta_3 << endl ;
			}
		}

		if(align_fit_scenario == align_fit_standard) func->SetParameters(func_par[0], func_par[1], func_par[2], 0, 0) ;
		if(align_fit_scenario == align_fit_with_coulomb) func->SetParameters(func_par[0], func_par[1], func_par[2], func_par[3], func_par[4]) ;

		acanvas.cd() ;

		acanvas.SetLogy() ;

		hist_1_proj_clone->Draw() ;
		hist_1_proj->Draw("same") ;
		func->Draw("same") ;

		TLatex *latex = new TLatex() ;

		latex->SetNDC() ;
		latex->SetTextFont(132) ;
		latex->SetTextColor(kBlack) ;

		stringstream ss_mean, ss_meane ;
		stringstream ss_sigma1, ss_sigma1e ;
		stringstream ss_sigma2, ss_sigma2e ;
		stringstream ss_chi2, ss_ndf, ss_p_value ;

		ss_mean << std::setprecision(3) << func_par[1] ;
		ss_meane << std::setprecision(3) << func_pare[1] ;

		ss_sigma1 << std::setprecision(3) << func_par[2] ;
		ss_sigma1e << std::setprecision(3) << func_pare[2] ;

		ss_sigma2 << std::setprecision(3) << func_par[4] ;
		ss_sigma2e << std::setprecision(3) << func_pare[4] ;

		const int number_of_parameters = 5 ;
		double myprob = TMath::Prob(chi2_global, ndf_global - number_of_parameters) ;

		ss_chi2 << std::setprecision(1) << chi2_global ;
		ss_ndf << std::setprecision(0) << ndf_global ;
		ss_p_value << std::setprecision(2) << myprob ;

		const double text_x = 0.12 ;

		latex->DrawLatex(text_x, .85, ("#chi2 / ndf = " + ss_chi2.str() + " / " + ss_ndf.str() + ",p-value=" + ss_p_value.str()).c_str()) ;

		if(ierflg == 0) latex->DrawLatex(text_x, .78, "CONVERGED") ;
		if(ierflg == 4) latex->DrawLatex(text_x, .78, "FAILED") ;

		latex->DrawLatex(text_x, .72, ("Mean : " + ss_mean.str() + " #pm " + ss_meane.str()).c_str()) ;
		latex->DrawLatex(text_x, .66, ("#sigma1 : " + ss_sigma1.str() + " #pm " + ss_sigma1e.str()).c_str()) ;
		latex->DrawLatex(text_x, .60, ("#sigma2 : " + ss_sigma2.str() + " #pm " + ss_sigma2e.str()).c_str()) ;
		
		double max = func->Eval(0.0) ;

		if(max >= 0) hist_1_proj_clone->GetYaxis()->SetRangeUser(0.1 * max, 1.3 * max) ;
		

		acanvas.SaveAs(("plots/vertical_alignment/canvas_1_proj_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;
		acanvas.SaveAs(("plots/vertical_alignment/canvas_1_proj_run_" + run_to_test + "_" + histograms[i] + ".pdf").c_str()) ;
		hist_1_proj_clone->SaveAs(("plots/vertical_alignment/hist_1_proj_clone_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;
		hist_1_proj->SaveAs(("plots/vertical_alignment/hist_1_proj_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;
		func->SaveAs(("plots/vertical_alignment/func_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;

		hist_1_proj->Delete() ;
		hist_1_proj_clone->Delete() ;

		delete gMinuit2 ;
	}

	file_LBRT->Close() ;
	file_LTRB->Close() ;
	
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

void load_runs_y(string run, int type)
{
}


void load_runs(string run, int type)
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

	vector<string> golden_runs_vector ;
	vector<string> normal_runs_vector ;
	vector<string> bad_runs_vector ;

	ifstream golden_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_golden.txt") ;
	while(golden_runs >> word) golden_runs_vector.push_back(word) ;
	golden_runs.close() ;

	ifstream normal_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_normal.txt") ;
	while(normal_runs >> word) normal_runs_vector.push_back(word) ;
	normal_runs.close() ;

	ifstream bad_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_bad.txt") ;
	while(bad_runs >> word) bad_runs_vector.push_back(word) ;
	bad_runs.close() ;

	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	while(runs >> word)
	{
		if(word.compare("324457") == 0) continue ;

		int type = 0 ;

		if(std::find(golden_runs_vector.begin(), golden_runs_vector.end(), word) != golden_runs_vector.end()) type = 1 ;
		if(std::find(normal_runs_vector.begin(), normal_runs_vector.end(), word) != normal_runs_vector.end()) type = 2 ;
		if(std::find(bad_runs_vector.begin(), bad_runs_vector.end(), word) != bad_runs_vector.end()) type = 3 ;

		load_runs(word, type) ;

		horizontal_elastic_alignment_per_run(word, type) ;
	}

	if(scenario == scenario_LBRT) graph2.SaveAs("plots/graph2_LBRT.root") ;
	if(scenario == scenario_LTRB) graph2.SaveAs("plots/graph2_LTRB.root") ;

	if(scenario == scenario_LBRT) graph2_golden.SaveAs("plots/graph2_golden_LBRT.root") ;
	if(scenario == scenario_LTRB) graph2_golden.SaveAs("plots/graph2_golden_LTRB.root") ;

	if(scenario == scenario_LBRT) graph2_normal.SaveAs("plots/graph2_normal_LBRT.root") ;
	if(scenario == scenario_LTRB) graph2_normal.SaveAs("plots/graph2_normal_LTRB.root") ;

	if(scenario == scenario_LBRT) graph2_bad.SaveAs("plots/graph2_bad_LBRT.root") ;
	if(scenario == scenario_LTRB) graph2_bad.SaveAs("plots/graph2_bad_LTRB.root") ;
}

void additional_plots()
{
	TCanvas canvas ;

	vector<string> histograms ;

	histograms.push_back("P0025_PlotsCollection_x_mm_y_mm_near_left_for_2RP") ;
	histograms.push_back("P0026_PlotsCollection_x_mm_y_mm_far_left_for_2RP") ;
	histograms.push_back("P0027_PlotsCollection_x_mm_y_mm_near_right_for_2RP") ;
	histograms.push_back("P0028_PlotsCollection_x_mm_y_mm_far_right_for_2RP") ;

	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	string run_to_test ;

	bool first = true ;

	while(runs >> run_to_test)
	{
		if(run_to_test.compare("324457") == 0) continue ;

		for(int i = 0 ; i < histograms.size() ; ++i)
		{
			TFile *file1 = TFile::Open(("plots/vertical_alignment/hist_1_proj_clone_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;
			TFile *file2 = TFile::Open(("plots/vertical_alignment/hist_1_proj_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;
			TFile *file3 = TFile::Open(("plots/vertical_alignment/func_run_" + run_to_test + "_" + histograms[i] + ".root").c_str()) ;

			TH1D *histo_1 = ((TH1D *)file1->Get("clone")) ;
			TH1D *histo_2 = ((TH1D *)file2->Get("py1")) ;
			TF1 *func = ((TF1 *)file3->Get("func")) ;

			canvas.cd() ;

			if(first) histo_1->Draw("") ;
			else histo_1->Draw("same") ;

			histo_2->Draw("same") ;
			func->Draw("same") ;

			first = false ;
		}
	}

	canvas.SaveAs("test.root") ;
}

void vertical_elastic_alignment()
{
	// load_runs() ;

	string word ;

	vector<string> golden_runs_vector ;
	vector<string> normal_runs_vector ;
	vector<string> bad_runs_vector ;

	ifstream golden_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_golden.txt") ;
	while(golden_runs >> word) golden_runs_vector.push_back(word) ;
	golden_runs.close() ;

	ifstream normal_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_normal.txt") ;
	while(normal_runs >> word) normal_runs_vector.push_back(word) ;
	normal_runs.close() ;

	ifstream bad_runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs_bad.txt") ;
	while(bad_runs >> word) bad_runs_vector.push_back(word) ;
	bad_runs.close() ;

	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	if(align_fit_scenario == align_fit_with_coulomb)
	{
		lo_x = lo_x_coulomb ;
		hi_x = hi_x_coulomb ;
	}

	while(runs >> word)
	{
		if(word.compare("324457") == 0) continue ;

		int type = 0 ;

		if(std::find(golden_runs_vector.begin(), golden_runs_vector.end(), word) != golden_runs_vector.end()) type = 1 ;
		if(std::find(normal_runs_vector.begin(), normal_runs_vector.end(), word) != normal_runs_vector.end()) type = 2 ;
		if(std::find(bad_runs_vector.begin(), bad_runs_vector.end(), word) != bad_runs_vector.end()) type = 3 ;

		load_runs_y(word, type) ;

		vertical_elastic_alignment_per_run(word, type) ;
	}

	runs.close() ;

	additional_plots() ;
}

void a_test(string diagonal)
{
	string run_to_test ;

	TGraph *alignx_graph_left_near = new TGraph() ;
	TGraph *alignx_graph_left_far = new TGraph() ;
	TGraph *alignx_graph_right_near = new TGraph() ;
	TGraph *alignx_graph_right_far = new TGraph() ;

	ifstream runs3("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	string word ;

	while(runs3 >> run_to_test)
	{
		cout << "Analysed run " << run_to_test << endl ;

		ifstream prj_file("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/corrections_" + diagonal + "_" + run_to_test + ".prj") ;

		while(prj_file >> word)
		{
			int run_to_test_int = atoi(run_to_test.c_str()) ;

			if(word.compare("RP_alignment_left_near_x_mm") == 0)
			{
				double value = 0 ;

				prj_file >> value ;

				alignx_graph_left_near->AddPoint(run_to_test_int, value) ;
			}
			else if(word.compare("RP_alignment_left_far__x_mm") == 0)
			{
				double value = 0 ;

				prj_file >> value ;

				alignx_graph_left_far->AddPoint(run_to_test_int, value) ;
			}
			else if(word.compare("RP_alignment_right_near_x_mm") == 0)
			{
				double value = 0 ;

				prj_file >> value ;

				alignx_graph_right_near->AddPoint(run_to_test_int, value) ;
			}
			else if(word.compare("RP_alignment_right_far__x_mm") == 0)
			{
				double value = 0 ;

				prj_file >> value ;

				alignx_graph_right_far->AddPoint(run_to_test_int, value) ;
			}

		}

		prj_file.close() ;
	}

	runs3.close() ;

	alignx_graph_left_near->GetYaxis()->SetRangeUser(-1.2,0.3) ;

	c.cd() ;

	alignx_graph_left_near->GetYaxis()->SetTitle("#Delta x (mm)") ;
	alignx_graph_left_near->GetXaxis()->SetTitle("Run number") ;

	alignx_graph_left_near->Draw("ap") ;
	alignx_graph_left_far->Draw("same p") ;
	alignx_graph_right_near->Draw("same p") ;
	alignx_graph_right_far->Draw("same p") ;

	alignx_graph_left_near->SetMarkerStyle(20) ;
	alignx_graph_left_far->SetMarkerStyle(20) ;
	alignx_graph_right_near->SetMarkerStyle(20) ;
	alignx_graph_right_far->SetMarkerStyle(20) ;

	alignx_graph_left_near->SetMarkerColor(kBlack) ;
	alignx_graph_left_far->SetMarkerColor(kRed) ;
	alignx_graph_right_near->SetMarkerColor(kGreen) ;
	alignx_graph_right_far->SetMarkerColor(kBlue) ;

	c.SetGridx() ;
	c.SetGridy() ;

	TLegend *legend = new TLegend() ;

	legend->AddEntry(alignx_graph_left_near, "left near", "p") ;
	legend->AddEntry(alignx_graph_left_far, "left far", "p") ;
	legend->AddEntry(alignx_graph_right_near, "right near", "p") ;
	legend->AddEntry(alignx_graph_right_far, "right far", "p") ;

	legend->Draw() ;

	c.SaveAs(("plots/alignment_test/alignx_graph_"+ diagonal + ".root").c_str()) ;
	c.SaveAs(("plots/alignment_test/alignx_graph_"+ diagonal + ".pdf").c_str()) ;

}

void test_of_cuts()
{
	TCanvas c ;

	TGraph *graph_cut_qualitiy_LBRT = new TGraph() ;
	TGraph *graph_cut_qualitiy_LTRB = new TGraph() ;

	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	cout << "Test of cuts" << endl << endl ;

	string run_to_test ;

	double mean_max = 0 ;

	while(runs >> run_to_test)
	{
		if(run_to_test.compare("324457") == 0) continue ;

		TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;

		TH1D *histo = ((TH1D *)file->Get("P0021_PlotsCollection_theta_x_star_left_far_rad_theta_x_star_right_far_rad_define_cut_distance_from_cut")) ;

		double mean = histo->GetMean() ;

		int run_to_test_int = atoi(run_to_test.c_str()) ;

		graph_cut_qualitiy_LBRT->AddPoint(run_to_test_int, mean) ;

		if(mean > mean_max) mean_max = mean ;

		cout << "run " << run_to_test << " " << histo->GetMean() << endl ;

		file->Close() ;
	}

	cout << "mean_max: " << mean_max << endl ;

	runs.close() ;



	ifstream runs2("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	cout << "Test of cuts" << endl << endl ;

	mean_max = 0 ;

	while(runs2 >> run_to_test)
	{
		if(run_to_test.compare("324457") == 0) continue ;

		TFile *file = TFile::Open(("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM_2RP/All_root_files_to_define_cuts_run_" + run_to_test + "/Generic.root").c_str()) ;

		TH1D *histo = ((TH1D *)file->Get("P0021_PlotsCollection_theta_x_star_left_far_rad_theta_x_star_right_far_rad_define_cut_distance_from_cut")) ;

		double mean = histo->GetMean() ;

		int run_to_test_int = atoi(run_to_test.c_str()) ;

		graph_cut_qualitiy_LTRB->AddPoint(run_to_test_int, mean) ;

		if(mean > mean_max) mean_max = mean ;

		cout << "run " << run_to_test << " " << histo->GetMean() << endl ;

		file->Close() ;
	}

	cout << "mean_max: " << mean_max << endl ;

	runs2.close() ;

	c.cd() ;

	graph_cut_qualitiy_LBRT->GetXaxis()->SetTitle("Run number") ;
	graph_cut_qualitiy_LBRT->GetYaxis()->SetTitle("#theta_{x}* (rad)") ;

	graph_cut_qualitiy_LBRT->Draw("ap") ;
	graph_cut_qualitiy_LTRB->Draw("same p") ;

	graph_cut_qualitiy_LBRT->SetMarkerStyle(20) ;
	graph_cut_qualitiy_LBRT->SetMarkerSize(0.8) ;

	graph_cut_qualitiy_LTRB->SetMarkerStyle(20) ;
	graph_cut_qualitiy_LTRB->SetMarkerSize(0.8) ;

	graph_cut_qualitiy_LBRT->SetMarkerColor(kRed) ;
	graph_cut_qualitiy_LTRB->SetMarkerColor(kBlue) ;

	c.SetGridx() ;
	c.SetGridy() ;

	TLegend *legend = new TLegend() ;

	legend->AddEntry(graph_cut_qualitiy_LBRT, "LBRT", "p") ;
	legend->AddEntry(graph_cut_qualitiy_LTRB, "LTRB", "p") ;

	legend->Draw() ;

	c.SaveAs("plots/alignment_test/cut_qualitiy.root") ;
	c.SaveAs("plots/alignment_test/cut_qualitiy.pdf") ;

   a_test("LBRT") ;
   a_test("LTRB") ;
}

int main()
{
	gStyle->SetLineScalePS(.3) ;

	gStyle->SetOptStat("");
	gStyle->SetOptFit(1111) ;

	gErrorIgnoreLevel = kError ;
	
	const int main_scenario_acceptance_polygons = 1 ;
	const int main_scenario_signal_to_bkg_for_2RP = 2 ;
	const int main_scenario_elastic_alignment = 3 ;
	const int main_scenario_elastic_alignment_vertical = 4 ;

	int main_scenario = main_scenario_elastic_alignment_vertical ;


	if(main_scenario == main_scenario_acceptance_polygons) main_studies() ;
	else if(main_scenario == main_scenario_signal_to_bkg_for_2RP) other_studies() ;
	else if(main_scenario == main_scenario_elastic_alignment) horizontal_elastic_alignment() ;
	else if(main_scenario == main_scenario_elastic_alignment_vertical)
	{
		vertical_elastic_alignment() ;
		test_of_cuts() ;
	}
}
