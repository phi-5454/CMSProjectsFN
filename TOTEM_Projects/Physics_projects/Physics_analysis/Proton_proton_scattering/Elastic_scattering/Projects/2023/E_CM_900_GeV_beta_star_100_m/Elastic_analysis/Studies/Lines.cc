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

void test(string name_to_test)
{
	gStyle->SetLineScalePS(.3);
	TH2F *histo = new TH2F("histo", "histo", 100, 0, 20, 100, -15, 15) ;

	string header ;
	string name ;

	double hor_x, hor_y,  ver_x,  ver_y ;
	double prev_hor_x, prev_hor_y,  prev_ver_x,  prev_ver_y ;

	ifstream myfile("lola.txt") ;
	
	const double a =  5.0 ;
	const double b = -3.0 ;
	
	TCanvas c ;
	
	histo->Draw("colz") ;
	
	int number_of_points = 0 ;
	int color_index = 0 ;
	
	bool test_correlation = true ;
	
	while(myfile >> header >> name >> hor_x >> hor_y >> ver_x >> ver_y)
	{
		if(name.compare(name_to_test) == 0)
		{
			histo->Fill(hor_x, hor_y) ;
			histo->Fill(ver_x + a, ver_y + b) ;
			
			TLine *line = NULL ;
			
			if(!test_correlation) line = new TLine(hor_x, hor_y, ver_x + a, ver_y + b) ;
			else
			 line = new TLine(hor_x, hor_y, prev_ver_x + a, prev_ver_y + b) ;
			
			if((number_of_points % 20) == 0)
			{
				++color_index;
				line->Draw("same") ;
				line->SetLineColor(color_index % 20) ;
			}		
			
			++number_of_points ;

			prev_hor_x = hor_x ;
			prev_hor_y = hor_y ;
			prev_ver_x = ver_x ;
			prev_ver_y = ver_y ;
		}

	}

	string hist_name = "histo" ; 
	
	if(test_correlation) hist_name += "_test" ;

	c.SaveAs((hist_name + "_" + name_to_test + ".pdf").c_str()) ;
	c.SaveAs((hist_name + "_" + name_to_test + ".root").c_str()) ;
}	

int main()
{
	test("3_4");
	test("3_5");
	test("23_24");
	test("23_25");
	test("103_104");
	test("103_105");
	test("123_124");
	test("123_125");
}
