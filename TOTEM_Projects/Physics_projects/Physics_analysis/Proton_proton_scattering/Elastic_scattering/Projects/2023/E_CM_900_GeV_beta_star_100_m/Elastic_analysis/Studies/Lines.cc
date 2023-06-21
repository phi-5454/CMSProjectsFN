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
	TH2F *histo = new TH2F("histo", "histo", 100, -20, 20, 100, -20, 20) ;

	string header ;
	string name ;

	double hor_x, hor_y,  ver_x,  ver_y ;

	ifstream myfile("lola.txt") ;
	
	const double a =  5.0 ;
	const double b = -3.0 ;
	
	TCanvas c ;
	
	histo->Draw("colz") ;
	
	int number_of_points = 0 ;

	while(myfile >> header >> name >> hor_x >> hor_y >> ver_x >> ver_y)
	{
		if(name.compare("123_124") == 0)
		{
			histo->Fill(hor_x, hor_y) ;
			histo->Fill(ver_x + a, ver_y + b) ;
			
			TLine *line = new TLine(hor_x, hor_y, ver_x + a, ver_y + b) ;
			
			if((number_of_points % 10) == 0) line->Draw("same") ;
			
			++number_of_points ;
		}
	}
	
	c.SaveAs("histo.root") ;
}	
