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
#include "TStyle.h"
#include "TMath.h"
#include "TFitResult.h"
#include "TMarker.h"

#include <iomanip>

const double intercept = 0.5e-3 ;
const double slope = -0.0024 ;
const double beam_momentum = 6500 ;

const double sigma  = 50e-6 ;
const double cutoff = (4.0 * sigma) ;

TGraphErrors *actual_graph = NULL ;

double cutline(double xi)
{
	// return (intercept + (slope * xi)) ;
	
	double theta_x_star_cut = abs(actual_graph->Eval(xi)) ;

	cout << "theta_x_star_cut: "  << theta_x_star_cut << " xi: " << xi << endl ;
	
	return theta_x_star_cut ;
}

double theta_star_from_t(double t)
{
        return (sqrt(t) / beam_momentum) ;
}

double phi_max(double theta_max, double theta)
{
        double value = 0 ;

	theta = fabs(theta) ;

        if(theta < (theta_max))
        {
                value = 0 ;
        }
	else if(theta_max <= (-theta))
	{
                value = TMath::Pi() ;
	}
        else 
        {
                value = acos(theta_max / theta) ;
        }

        return value ;
}

double phi_visible(double theta_max, double theta)
{

        return (2 *((TMath::Pi() - phi_max(theta_max, theta))) / (2 * TMath::Pi())) ;
}

double integral_1(double mom_loss)
{
	const int npoints = 10000 ;

	double delta = cutoff / npoints ;
	double maxangle = cutline(mom_loss) ;
	// maxangle = 0 ;
	cout  << "maxangle: "  << maxangle  << endl ;

	const double width = 50e-6 ;

	double sum_norm = 0 ;
	double A = 0 ;

	for(int hi = 1 ; hi < npoints  ; ++hi)
	{
		double angle = (hi * delta) ;

		double norm = exp(-pow(angle,2) / pow(width,2)) ;

		double dA = phi_visible(maxangle, angle) ;
		cout << "dA: " << dA << ", "  << ", n: " << norm << " " ;
		double dA_weighted = dA * norm * delta;
		A = A + dA_weighted ;

		sum_norm = sum_norm + (norm * delta) ;
	}

	double visible = (A / sum_norm) ;
	cout << "visible: " << visible << endl ;
	double correction = 1.0 / visible ;

	return correction ;
}

TGraphErrors *lower_boundary = NULL ;
TGraphErrors *upper_boundary = NULL ;

double integral_2(double mom_loss, double mean, double &A_l, double &A_u)
{
        double distance_lower = (lower_boundary->Eval(mom_loss) - mean) ;
        double distance_upper = (upper_boundary->Eval(mom_loss) - mean) ;

	if(mean < lower_boundary->Eval(mom_loss)) distance_lower = 0.0 ;
	if(mean > upper_boundary->Eval(mom_loss)) distance_upper = 0.0 ;

	double visible_distance_per_sqrt2_sigma_lower = (fabs(distance_lower) / (sigma * sqrt(2.0))) ;
	double visible_distance_per_sqrt2_sigma_upper = (fabs(distance_upper) / (sigma * sqrt(2.0))) ;

	cout << "lower_boundary->Eval(mom_loss): " << mom_loss << " " << lower_boundary->Eval(mom_loss) << endl ;
	cout << "upper_boundary->Eval(mom_loss): " << mom_loss << " " << upper_boundary->Eval(mom_loss) << endl ;

	cout << "visible_distance_per_sqrt2_sigma_lower: " << visible_distance_per_sqrt2_sigma_lower << endl ;
	cout << "visible_distance_per_sqrt2_sigma_upper: " << visible_distance_per_sqrt2_sigma_upper << endl ;

	A_l = TMath::Erf(visible_distance_per_sqrt2_sigma_lower) / 2.0 ;
	A_u = TMath::Erf(visible_distance_per_sqrt2_sigma_upper) / 2.0 ;

	double A = A_l + A_u ;

	return A ;
}

Double_t acceptance_t(const Double_t *pt_GeV2, Double_t *par)
{
        double value = 0 ;

        double theta_star = (*pt_GeV2) ;
        double mom_loss = par[0] ;

        double c_phi_visible = phi_visible(cutline(mom_loss), theta_star) ;

        if(c_phi_visible != 0) value = (1.0 / c_phi_visible) ;

        return value ;
}

void determine_boundary(string filename, string arm, TFile *generic)
{
	TFile *root_file_1 = TFile::Open(filename.c_str()) ;
	
	if(root_file_1 == NULL)
	{
		cout << "Skip [file does not exist]: " << filename << endl ;
		
		return ;
	}
	
	string path = "" ;
	
	if(arm.compare("45") == 0) path = "multiRPPlots/arm0/h2_th_x_vs_xi" ;
	else if(arm.compare("56") == 0) path = "multiRPPlots/arm1/h2_th_x_vs_xi" ;

	TH2D *histogram = (TH2D *)root_file_1->Get(path.c_str()) ;

	if(histogram != NULL) histogram->SetDirectory(0); 
	root_file_1->Close() ;

	if((histogram == NULL) || (histogram->GetEntries() < 2000))
	{
		cout << "Skip [histogram is null or empty]: " << filename << endl ;
		return ;
	}
	
        string key = filename.substr(70, 20) ;
        string partial_key = arm + "_" + filename.substr(70, 30) ;

        if(key.substr(0, 4).compare("fill") != 0)
        {
        	key = filename.substr(58, 20) ;
                partial_key = arm + "_" + filename.substr(58, 30) ;
                cout << "  new file "  << endl ;
        }

	partial_key[12] = '_' ;
	partial_key[30] = 'p' ;

        cout << "partial_key: " << partial_key << endl ;

	lower_boundary->SetName((partial_key + "_lower_boundary").c_str()) ;
	upper_boundary->SetName((partial_key + "_upper_boundary").c_str()) ;

        if(key.substr(0, 4).compare("fill") != 0)
        {
                key = filename.substr(58, 20) ;
                partial_key = arm + filename.substr(58, 9) ;
                cout << "  new file "  << endl ;
        }

        cout << partial_key << endl ;

	cout << "Processing: " << filename << endl ;

	string histogram_name = histogram->GetName() ;
	cout << histogram_name << endl ;

	string hist_name_mean = histogram_name + "_1" ;

	TObjArray aSlices;	
	histogram->FitSlicesY(0, 0, -1, 0, "QNR", &aSlices);
	TH1D *histogram_mean = (TH1D*)gDirectory->Get(hist_name_mean.c_str()); 
	histogram_mean->Fit("pol0", "S", "", 5e-2, 1e-1) ;
	double p0 = histogram_mean->GetFunction("pol0")->GetParameter(0) ;

	TF1 *mean_fit = (TF1 *)(histogram_mean->GetFunction("pol0")->Clone((partial_key + "_mean_fit").c_str())) ;

	cout << "p0: " << p0 << endl ;

	double maximum = histogram->GetMaximum() ;
	histogram->Scale(1.0 / maximum) ;

	int bin_1 = histogram_mean->FindBin(.15) ;
	int bin_2 = histogram_mean->FindBin(.3) ;

	for(int ij = bin_1 ; ij < bin_2 ; ++ij)
	{
		histogram_mean->SetBinContent(ij, 0) ;
		histogram_mean->SetBinError(ij, 0) ;
	}


	cout << histogram_mean << endl ;

	int lindex	 = 0 ;

	cout << "arm:" << arm << endl ;

	int lastindex_upper = lindex ;
	int lastindex_lower = lindex ;

	double threshold_up = 3e-2 ;
	double threshold_low = 0.14 ;

	if(arm.compare("45") == 0) 
	{
	        upper_boundary->SetPoint( lindex++,  2.0e-2,    cutoff) ;
	        upper_boundary->SetPoint( lindex++,  3.0e-2,    cutoff) ;
	        upper_boundary->SetPoint( lindex++,  3.5e-2,    cutoff) ;
        	upper_boundary->SetPoint( lindex++,  4.0e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++,  6.0e-2,    cutoff) ;
	        upper_boundary->SetPoint( lindex++,  9.0e-2,    cutoff) ;
        	upper_boundary->SetPoint( lindex++, 11.0e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 12.0e-2,    cutoff) ;
	        upper_boundary->SetPoint( lindex++, 12.5e-2,    cutoff) ;
        	upper_boundary->SetPoint( lindex++, 13.0e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 14.0e-2,    cutoff) ;
	        upper_boundary->SetPoint( lindex++, 15.0e-2,    cutoff) ;
        	upper_boundary->SetPoint( lindex++, 18.0e-2,    cutoff) ;

		lastindex_upper = lindex-1 ;

		lindex = 0 ;

		lower_boundary->SetPoint( lindex++,   2.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   3.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   3.5e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   4.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   5.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   6.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,   9.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,  12.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,  14.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,  15.0e-2,  -cutoff) ;
		lower_boundary->SetPoint( lindex++,  18.0e-2,  -cutoff) ;

		lastindex_lower = lindex-1 ;

	}
	else if(arm.compare("56") == 0)
	{
                upper_boundary->SetPoint( lindex++,  2e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++,  3e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++,  4e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++,  6e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++,  9e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 11e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 12e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 12.5e-2,  cutoff) ;
                upper_boundary->SetPoint( lindex++, 13e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 14e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 16e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 20e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 25e-2,    cutoff) ;
                upper_boundary->SetPoint( lindex++, 28e-2,    cutoff) ;

                lastindex_upper = lindex-1 ;

                lindex = 0 ;

                lower_boundary->SetPoint( lindex++,  2e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++,  3e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++,  4e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++,  6e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++,  9e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 12e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 14e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 16e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 20e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 25e-2,  -cutoff) ;
                lower_boundary->SetPoint( lindex++, 28e-2,  -cutoff) ;

		lastindex_lower = lindex-1 ;

	}
	else
	{
		exit(1) ;
	}


	for(int ji = 0 ; ji < lower_boundary->GetN()  ; ++ji)
	{
		double x, y ;

		lower_boundary->GetPoint(ji, x, y) ;

		int bin = histogram->GetXaxis()->FindBin(x) ;
		int bin_y = histogram->GetYaxis()->FindBin(y) ;

		bool was_set = false ;

               for(int k = bin_y ; k < histogram->GetYaxis()->GetNbins() ; ++k)
               {
			double content = histogram->GetBinContent(bin, k) ;

			bool condition_1 = ((x < 5.0e-2)  && (content > threshold_low)) ;
			bool condition_2 = ((x >= 5.0e-2) && (content > (threshold_low*0.05))) ;

                        if(condition_1 || condition_2)
                        {
                                double y_value = histogram->GetYaxis()->GetBinCenter(k)  ;
                                lower_boundary->SetPoint(ji, x, y_value) ;

				was_set = true ;

				break ;
                        }
                }

		if(!was_set) lower_boundary->SetPoint(ji, x, cutoff) ;

		double x2, y2 ;
		lower_boundary->GetPoint(ji,   x,  y) ;
		lower_boundary->GetPoint(ji-1, x2, y2) ;

		if((ji > 1) && (y > y2))
		{
	                lower_boundary->SetPoint(ji, x, y2) ;
		}


	}

	for(int ji = 0 ; ji < upper_boundary->GetN()  ; ++ji)
	{
		double x, y ;

		upper_boundary->GetPoint(ji, x, y) ;
		if(x < .1) continue ; 

		int bin = histogram->GetXaxis()->FindBin(x) ;
		int bin_y = histogram->GetYaxis()->FindBin(y) ;

		bool was_set = false ;

		for(int k = bin_y ; k > 0 ; --k)
		{
			double content = histogram->GetBinContent(bin, k) ;

			if((content > threshold_up) || ((histogram->GetYaxis()->GetBinCenter(k)) <= lower_boundary->Eval(x)))
			{
			
				double y_value = histogram->GetYaxis()->GetBinCenter(k)  ;
				upper_boundary->SetPoint(ji, x, y_value) ;

				was_set = true ;						

				for(int n = (ji + 1) ; n < upper_boundary->GetN()  ; ++n)
				{
					double x2, y2 ;
				        upper_boundary->GetPoint(n, x2, y2) ;

					upper_boundary->SetPoint(n, x2, y_value) ;
				}

				break ;
			}
		}

		if(!was_set) upper_boundary->SetPoint(ji, x, lower_boundary->Eval(x)) ;


	}


	double x, y ;
	upper_boundary->GetPoint(0, x, y) ;
	lower_boundary->SetPoint(0, x, y) ;

	lower_boundary->GetPoint(lastindex_lower, x, y) ;
	upper_boundary->SetPoint(lastindex_upper, x, y) ;

	for(int i = 0 ; i < 4 ; ++i)
	{
		double x2, y2 ;

		lower_boundary->GetPoint(i, x, y) ;
		upper_boundary->GetPoint(i, x2, y2) ;

		if(fabs((y - y2) / y) < 0.1)
		{
			lower_boundary->SetPoint(i, x, y2) ;
		}

	}


	vector<int> to_be_removed ;

	for(int i = 3 ; i >= 0 ; --i)
	{
		double x2, y2 ;

		double x3, y3 ;
		double x4, y4 ;

		lower_boundary->GetPoint(i, x, y) ;
		upper_boundary->GetPoint(i, x2, y2) ;

		lower_boundary->GetPoint(i+1, x3, y3) ;
		upper_boundary->GetPoint(i+1, x4, y4) ;

		if((y == y2) && (x3 == x4) && (y3 == y4))
		{
			lower_boundary->RemovePoint(i) ;
			upper_boundary->RemovePoint(i) ;
		}

	}

	int N1 = lower_boundary->GetN() ;
	int N2 = upper_boundary->GetN() ;

	for(int i = 0 ; i < 4 ; ++i)
	{
		double x2, y2 ;

		double x3, y3 ;
		double x4, y4 ;

		lower_boundary->GetPoint(N1 - i, x, y) ;
		upper_boundary->GetPoint(N2 - i, x2, y2) ;

		lower_boundary->GetPoint(N1 - (i+1), x3, y3) ;
		upper_boundary->GetPoint(N2 - (i+1), x4, y4) ;

		if((y == y2) && (x3 == x4) && (y3 == y4))
		{
			lower_boundary->RemovePoint(N1 - i) ;
			upper_boundary->RemovePoint(N2 - i) ;
		}

	}

	lower_boundary->SetMarkerStyle(20) ;
	upper_boundary->SetMarkerStyle(20) ;

        lower_boundary->SetMarkerSize(0.5) ;
        upper_boundary->SetMarkerSize(0.5) ;

	lower_boundary->SetLineColor(kRed) ;
	upper_boundary->SetLineColor(kMagenta) ;

	lower_boundary->SetLineColor(kBlack) ;
	upper_boundary->SetLineColor(kBlack) ;

	lower_boundary->SetLineWidth(2.5) ;
	upper_boundary->SetLineWidth(2.5) ;

        lower_boundary->SetMarkerColor(kRed) ;
        upper_boundary->SetMarkerColor(kMagenta) ;	

	generic->cd() ;

	mean_fit->Write() ;
	lower_boundary->Write() ;	
	upper_boundary->Write() ;	
	

}

int main()
{
	ifstream filenames("../../lola4") ;
	TFile *generic = new TFile("result/generic.root","UPDATE") ;

	string word ;

	TCanvas canvas ;

	int index = 0 ;

        lower_boundary = new TGraphErrors() ;
        upper_boundary = new TGraphErrors() ;

	TGraphErrors *acceptance_corrections_per_fill_x_angle = new TGraphErrors() ;

	while(filenames >> word)
	{
		determine_boundary(word, "45", generic) ;
		determine_boundary(word, "56", generic) ;
		
		++index ;

	}

}
	
