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

int main()
{
	gStyle->SetOptStat("");

	TFile *file = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/Tests/frame2.root") ;
	
	graph = ((TGraph *)file->Get("Graph")) ;

	c.cd() ;
	
	double R = (300e-6 / 2.0) ;

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, -1e-3, 1e-3, 100, -0.5e-3, 0.5e-3) ;	
	TEllipse ellipse(0, 0, R, R) ;
	ellipse.SetFillStyle(0) ;
	ellipse.SetLineColor(kBlue) ;

	hist_2d->Draw() ;
	graph->Draw("same") ;	
	ellipse.Draw("same") ;

	accep_based_on_frame(R) ;
	
	c.SaveAs("test.pdf") ;
}
