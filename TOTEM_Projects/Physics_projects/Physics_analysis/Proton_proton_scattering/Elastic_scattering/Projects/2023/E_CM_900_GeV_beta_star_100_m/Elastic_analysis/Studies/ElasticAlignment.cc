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

TH2D *flipHistogram(TH2D *histogram)
{
	int xbins = histogram->GetXaxis()->GetNbins() ;
	int ybins = histogram->GetYaxis()->GetNbins() ;

	int x_axis_min = histogram->GetXaxis()->GetXmin() ;
	int x_axis_max = histogram->GetXaxis()->GetXmax() ;

	int y_axis_min = histogram->GetYaxis()->GetXmin() ;
	int y_axis_max = histogram->GetYaxis()->GetXmax() ;

	TH2D *histogram_rotated = new TH2D("test", "test", ybins, y_axis_min, y_axis_max, xbins, x_axis_min, x_axis_max) ;
	
	for(int i = 0 ; i < xbins ; ++i)
	for(int j = 0 ; j < ybins ; ++j)
	{
		double bincontent = histogram->GetBinContent(i, j) ;
		double bin_unc = histogram->GetBinError(i, j) ;
	
		histogram_rotated->SetBinContent(j, i, bincontent) ;
		histogram_rotated->SetBinError(j, i, bin_unc) ;
	}
	
	// histogram_rotated->SaveAs("histogram_rotated.root") ;
	
	return histogram_rotated ;
}


TFile *file_LBRT = NULL ;
TFile *file_LTRB = NULL ;

void combine_and_fit(string name, double low, double high, bool swap = false)
{
	TCanvas *c = new TCanvas() ;

	TH2D *LBRT = ((TH2D *)(file_LBRT->Get(name.c_str()))) ;
	TH2D *LTRB = ((TH2D *)(file_LTRB->Get(name.c_str()))) ;
	
	c->SetTitle(LBRT->GetName()) ;
	cout << "myname " << LBRT->GetName() << endl ;
	c->SetTitle("") ;
	
	// LBRT->SaveAs("delete_it.root") ;

	TH1D *px1 = NULL ;
	TH1D *px2 = NULL ;
	
	if(swap)
	{
		px1 = LBRT->ProjectionX("px1", 600, 660) ;
		px2 = LTRB->ProjectionX("px2", 366, 425) ;	
	}
	else
	{
		px1 = LTRB->ProjectionX("px1", 600, 660) ;
		px2 = LBRT->ProjectionX("px2", 366, 425) ;	
	}
	
	double px1_mean = px1->GetMean() ;
	double px2_mean = px2->GetMean() ;

	TFitResultPtr ptr1 = px1->Fit("gaus","S", "", px1_mean + low, px1_mean + high) ;
	TFitResultPtr ptr2 = px2->Fit("gaus","S", "", px2_mean + low, px2_mean + high) ;
	
	double mean_1 = ptr1->Parameter(1) ;
	double mean_2 = ptr2->Parameter(1) ;

	double avg = (mean_1 + mean_2) / 2.0 ;
	double var = fabs(avg - mean_1) ;
	
	cout << "fit_result mean: " << mean_1 << endl ;
	cout << "fit_result mean: " << mean_2 << endl ;
	cout << "fit_result avg: " << avg << endl ;
	cout << "fit_result var: " << var << endl ;
	cout << "fit_result" << endl ;

	px1->Draw() ;
	px1->GetXaxis()->SetRangeUser(-10.0, 10.0) ;
	px2->Draw("same") ;

	c->SaveAs(("plots/ElasticAlignment/proj_" + name + ".root").c_str()) ;
	c->SaveAs(("plots/ElasticAlignment/proj_" + name + ".pdf").c_str()) ;
	
	delete px1 ;
	delete px2 ;
	
	delete c ;

}

main()
{
	gStyle->SetLineScalePS(.3) ;
	gStyle->SetOptFit(1111);

	file_LBRT = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts/Generic.root") ;
	file_LTRB = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_TOP_RIGHT_BOTTOM/All_root_files_to_define_cuts/Generic.root") ;

	combine_and_fit("P0079_PlotsCollection_x_mm_y_mm_near_left_aligned", -2.0, 2.0) ;
	combine_and_fit("P0080_PlotsCollection_x_mm_y_mm_far_left_aligned", -1.5, 1.5) ;
	combine_and_fit("P0081_PlotsCollection_x_mm_y_mm_near_right_aligned", -2.0, 2.0, true) ;
	combine_and_fit("P0082_PlotsCollection_x_mm_y_mm_far_right_aligned", -1.5, 1.5, true) ;
}
