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
	
	TH1D *hist2 = new TH1D("hist2", "hist2", 800, 0, 0.1) ;
	TH2D *hist3 = new TH2D("hist3", "hist3", 1024, -35, 35, 1024, -35, 35) ;
	
	int files_ok = 0 ;

	while(myfiles >> word)
	{
		string filename = "/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_100_m/Monte_Carlo/Condor/MC_ver/" + word ;
		cout << filename << endl ;

		if(access(filename.c_str(), F_OK) == 0)
		{
			cout << "adding" << endl ;
			TFile *myfile = NULL ;
			myfile = new TFile(filename.c_str(), "READ") ;

			if(myfile != NULL)
			if(!myfile->IsZombie())
			{

				// TH1D *hist = ((TH1D *)myfile->Get("P0016_PlotsCollection_dN_dt_far_GeV2")) ;
				// TH1D *hist = ((TH1D *)myfile->Get("P0019_PlotsCollection_dN_dt_far_GeV2")) ;
				TH1D *hist2_temp = ((TH1D *)myfile->Get("P0020_PlotsCollection_dN_dt_far_GeV2_geometrical_acceptance_corrected")) ;
				TH2D *hist3_temp = ((TH2D *)myfile->Get("P0000_PlotsCollection_x_mm_y_mm_far_left")) ;


				hist2->Add(hist2_temp) ;
				hist3->Add(hist3_temp) ;

				files_ok++ ;
			}

			cout << hist2 << endl ;

			delete myfile ;
		}
	}

	cout << "files_ok " << files_ok << endl ;

	hist2->SaveAs("hist2.root") ;	
	hist3->SaveAs("hist3.root") ;	
}
