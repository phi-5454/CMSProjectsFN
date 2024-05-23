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

const double Lambda2_GeV2 = 0.71 ;
const double Fine_structure_constant = 1/137.035 ;

double G_proton(double t)
{
    return 1 / pow(1-(-t/Lambda2_GeV2),2) ;
}

double Coulomb_amplitude(double t)
{
    return 2*sqrt(TMath::Pi())*(Fine_structure_constant)*(1/abs(t))*pow(G_proton(t),2) ;
}


double sigma(double *t, double *par)
{
    double ca = Coulomb_amplitude(t[0]) * pow(0.1973,2) * 2*sqrt(TMath::Pi()); // to have the same dimension, normalization, as in BB
    double result = (10. * (pow(abs(ca),2) / pow(0.1973,2)))  / (4*TMath::Pi()) ;

    cout << "test at t=" << (*t) << "\t " << result << endl ;

	 return result ;
}


int main()
{
	double t_value = 0.02 ;

	cout << sigma(&t_value, NULL) << endl ;

	double epsilon = 1e-5 ;

	TF1 *func = new TF1("sigma",  sigma, epsilon, 1, 0) ;
	func->SetNpx(1e4) ;

	TCanvas c ;

   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 0, 1, 100, 0, 5) ;
	hist_2d->SetTitle("") ;
	hist_2d->Draw() ;

	func->Draw("same l") ;
	func->SaveAs("plots/coulomb/func_coulomb_test.root") ;
	c.SaveAs("plots/coulomb/canvas_coulomb_test.root") ;
}
