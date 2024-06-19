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


int main()
{
	TFile *general = new TFile("generic.root", "UPDATE") ;

	string word ;
	ifstream myfiles("tmp/files.txt") ;
	
	TH1D *hist2 = new TH1D("hist", "hist", 800, 0, 0.1) ;
	
	while(myfiles >> word)
	{
		cout << word << endl ;
		TFile *myfile = new TFile(word.c_str(), "READ") ;
		
		if(!myfile->IsZombie() && (myfile != NULL)) ;
		{

			TH1D *hist = ((TH1D *)myfile->Get("P0016_PlotsCollection_dN_dt_far_GeV2")) ;
			
			hist2->Add(hist) ;
		}
		
		cout << hist2 << endl ;

		delete myfile ;
	}

	hist2->SaveAs("hist2.root") ;	
}
