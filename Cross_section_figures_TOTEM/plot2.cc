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
#include "TMultiGraph.h"

#include <iomanip>

class MyTPoint
{
	public:	
	
	double energy ;
	double sigtot ;
	double uncertainty_up ;
	double uncertainty_down ;


	MyTPoint(double, double, double) ;
} ;

TF1 *func = NULL ;
vector<MyTPoint *> points ;

Double_t log_like_function(Double_t *x, Double_t *par)
{
        double f1 = par[0] * pow(log(x[0]), 2) ;
        double f2 = par[1] * pow(log(x[0]), 1) ;
        double f3 = par[1] * pow(log(x[0]), 0) ;

        double f = f1 + f2 + f3 ;

        return f ;
}

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
  double chi2 = 0 ;
	
  for(int i = 0 ; i < points.size() ; ++i)
  {
		double energy = points[i]->energy ;
		double sigtot = points[i]->sigtot ;
		double uncertainty_up = points[i]->uncertainty_up ;

    double value = log_like_function(&energy, par) ;

		double delta = (value - sigtot) / uncertainty_up ;
		chi2 += delta*delta ;
  }

  f = chi2 ;
}

void MinuitFit()
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "constant", 27, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(1, "slope",  -4.37008e-02, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(2, "ratio",    0.15, 0.1, 0, 0, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  func = new TF1("func",  log_like_function, 0.0, 13000.0, 3) ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
}

int main(int argc, char *argv[])
{
	ifstream data("hepdata/most_relevant_points.txt") ;
	
	double energy, sigtot, sigtot_unc ;
	
	while(data >> energy >> sigtot >> sigtot_unc)
	{
	}
}

  
