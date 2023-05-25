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

#include <iomanip>

int main()
{
	TH2D *hist = new TH2D("hist", "hist", 100, -5e+2, 5e+2, 100, -1000, 1000) ;

	TH2D *hist_compare_theta_y_star = new TH2D("hist_compare_theta_y_star", "hist_compare_theta_y_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;
	TH2D *hist_compare_y_star = new TH2D("hist_compare_y_star", "hist_compare_y_star", 100, -16.0e-4, 16.0e-4,  100, -16.0e-4, 16.0e-4) ;

	TRandom3 rand ;

	// Near: XRPV.C6R5.B1
	// Far:  XRPV.B6R5.B1
	// -2.47E+000	1.91E+001
	// -3.13E+000	1.74E+001

	double Ly_near = 1.91E+001 ;
	double Ly_far  = 1.74E+001 ; 

	// Best effect
	double delta_n = -2.0 ;
	double delta_f = -3.0 ;

	// 
  delta_n = -2.0 * 0.0 ;
  delta_f = -3.0 * 0.0 ;

  double Ly_near_reco = Ly_near + delta_n ;
  double Ly_far_reco = Ly_far   + delta_f ;

	double vy_near = 2.47 ;
	double vy_far  = 3.13 ;

	for(int i = 0 ; i < 1e6 ; ++i)
	{
	  double theta_y_star = 128e-6 * rand.Gaus() ;
	  double y_star = 1.0 * 200e-6 * rand.Gaus() ;
		
		double beam_divergence = 0.0 * 20e-6 * rand.Gaus() ;
		
		double theta_y_star_pert = theta_y_star + beam_divergence ;

		double y_near = (Ly_near * theta_y_star_pert) + (vy_near * y_star) ;
		double y_far =  (Ly_far *  theta_y_star_pert) + (vy_far *  y_star) ;

		// cout << y_far << endl ;

		// if((y_far < -1.e-3) && (y_far > -10.0e-3) )
		// if((y_far < -1.e-3) && (y_far > -10.0e-3) && (y_near < (-1.e-3*1.4*(Ly_far/Ly_near))) && (y_near > (-10.0e-3*(Ly_far/Ly_near))))
		{

			double theta_y_star_reco = ((y_near * vy_far) - (y_far * vy_near)) / ((Ly_near_reco * vy_far) - (Ly_far_reco * vy_near)) ;
			double y_star_reco = -((y_near * Ly_far_reco) - (y_far * Ly_near_reco)) / ((Ly_near_reco * vy_far) - (Ly_far_reco * vy_near)) ;

		// cout <<	theta_y_star << " " << theta_y_star_reco << " " << y_star << " " << y_star_reco << endl ;

			hist->Fill(theta_y_star_reco * 1e6, y_star_reco * 1e6) ;

			hist_compare_theta_y_star->Fill(theta_y_star, theta_y_star_reco) ;
			hist_compare_y_star->Fill(y_star, y_star_reco) ;
		}

	}
	
	gStyle->SetPalette(1) ;

	TCanvas c ;
	
  gStyle->SetTitleFillColor(kWhite) ;
  gStyle->SetStatColor(kWhite) ;
  gStyle->SetFillColor(kWhite) ;

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	
	hist->SetFillColor(kWhite) ;
	hist->SetTitle("") ;

	hist->Draw("colz") ;
	
	hist->GetXaxis()->SetTitle("#theta_{x,reco}* [#murad]") ;
	hist->GetYaxis()->SetTitle("x_{reco}* [#mum]") ;
	hist->GetYaxis()->SetTitleOffset(1.2) ;

	TLatex *latex = new TLatex() ;
	latex->SetTextFont(132) ;
	latex->SetNDC();
	latex->DrawText(0.24, 0.92, "simulation preliminary") ;

	latex->SetTextFont(22) ;
	latex->DrawText(0.12, 0.92, "TOTEM") ;
	latex->Draw("same") ;
	
	
	c.SetFillColor(kWhite) ;
	
	gStyle->SetOptStat(0);

        c.SetGridx() ;
        c.SetGridy() ;	
	
	hist->GetXaxis()->SetAxisColor(17);
	hist->GetYaxis()->SetAxisColor(17);	

	c.SetLogz() ;
	c.SaveAs("plots/hist.pdf") ;
	
	hist_compare_theta_y_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_theta_y_star.pdf") ;

	hist_compare_y_star->Draw("colz") ;
	c.SaveAs("plots/hist_compare_y_star.pdf") ;

}	
