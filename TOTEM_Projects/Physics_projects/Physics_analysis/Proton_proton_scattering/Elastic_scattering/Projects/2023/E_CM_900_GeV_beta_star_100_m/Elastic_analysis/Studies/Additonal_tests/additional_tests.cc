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
#include "TTree.h"

const string directory = "/eos/cms/store/group/phys_diffraction/fnemes/E_CM_900_GeV/Beta_star_11_m/Reduced_ntuples/7302/" ;

main()
{
	ifstream filenames("filenames.txt") ;
	
	string filename ;
	
	TCanvas c ;

	string com = "track_right_far_y:track_right_far_x" ;

	com = "track_right_near_y:track_right_near_x" ;
	com = "track_right_far_y:track_right_far_x" ;

	com = "track_left_near_y:track_left_near_x" ;
	com = "track_left_far_y:track_left_far_x" ;
	
	while(filenames >> filename)
	{
		cout << filename << endl ;
		
		TFile *myfile = TFile::Open((directory + filename).c_str()) ;
		
		TTree *tree = ((TTree *)myfile->Get("TReducedNtuple")) ;

//		tree->StartViewer() ;
		tree->Draw((com + ">>h2(100, -30, 30, 100, -40, 40)").c_str(),"","colz");
		c.SetLogz() ;
		c.SaveAs(("plots/c1" + filename + "_.pdf").c_str()) ;

		
		myfile->Close() ;
	}
}
