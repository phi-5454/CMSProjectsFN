#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

#include "stdlib.h"
#include "math.h"

#include "TH1D.h"
#include "TH2D.h"

#include "TGraph.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TRandom3.h"

#include "TMinuit.h"

#include <iomanip>

// #### Optics filename 

string filename = "optics/xi_as_a_function_of_x_graph_b1_140_murad.root" ;

// #### The two detectors, near and far, in the study 

const double sigma_of_beam_divergence = 35e-6 ;

TGraph *near_detector_xi_x_inverse = NULL ;
TGraph *far_detector_xi_x_inverse = NULL ;


TGraph *near_detector_xi_x 	= NULL ;
TGraph *near_detector_xi_Ly 	= NULL ;
TGraph *near_detector_xi_Lx 	= NULL ;
TGraph *near_detector_xi_Dx 	= NULL ;

TGraph *far_detector_xi_Lx 	= NULL ;
TGraph *far_detector_xi_Dx 	= NULL ;

double sign = 1.0 ;

void reconstruct(double x_far, double x_near, double &theta_x_star, double &xi)
{
	// Evaluate optical functions

	double Lx_far 	= far_detector_xi_Lx->Eval(-xi) ;
	double Lx_near 	= near_detector_xi_Lx->Eval(-xi) ;
	
	double Dx_far 	= sign * (far_detector_xi_x_inverse->Eval(xi)/xi) ;
	double Dx_near 	= sign * (near_detector_xi_x_inverse->Eval(xi)/xi) ;

	// Reconstruction

	double determinant = (Dx_near * Lx_far - Dx_far * Lx_near) ;
	
	xi = (Lx_far * x_near - Lx_near * x_far) / determinant ;
	theta_x_star = (Dx_near * x_far - Dx_far * x_near) / determinant ;

}

double x_far_data = 0 ;
double x_near_data = 0 ;

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
    double chi2 = 0 ;

	double x_near_MC 		= ( sign * near_detector_xi_x_inverse->Eval(-par[0])) + (near_detector_xi_Lx->Eval(par[0]) * par[1]) ;
	double x_far_MC 		= ( sign * far_detector_xi_x_inverse->Eval(-par[0])) 	+ (far_detector_xi_Lx->Eval(par[0]) * par[1]) ;
	
	chi2 = pow((x_far_data - x_far_MC)/0.001,2) + pow((x_near_data - x_near_MC)/0.001,2) ;

    f = chi2 ;
}

const int beam_1 = 1 ;
const int beam_2 = 2 ;

int beam = beam_2 ;

string near_detector = "" ;
string far_detector =  "" ;


int main()
{

	// #################################################
	// ####						   #
	// ####		Definitions			   #
	// #### 					   #
	// #################################################

	const int number_of_particles = 1000000 ;	
	// const int number_of_particles = 1000 ;	

	// #### Some histograms for MC and reconstruction study
	
	TH2D *x_y_hitmap_near = new TH2D("x_y_hitmap_near", "x_y_hitmap_near", 200, -0.0025, 0.018, 200, -0.011, 0.011) ;
	TH2D *xi_MC_xi_reco = new TH2D("xi_MC_xi_reco", "xi_MC_xi_reco", 100, -0.25, 0.25, 100, -0.25, 0.25) ;
	TH2D *theta_x_star_MC_theta_x_star_reco = new TH2D("theta_x_star_MC_theta_x_star_reco", "theta_x_star_MC_theta_x_star_reco", 100, -200e-6, 200e-6, 100, -200e-6, 200e-6) ;

	if(beam == beam_1)	
	{
		near_detector = "XRPH_C6R5_B1" ;
		far_detector =  "XRPH_D6R5_B1" ;
		
		cout << "Beam 1" << endl ;
	}
	else if(beam == beam_2)	
	{
		near_detector = "XRPH_C6L5_B2" ;
		far_detector =  "XRPH_D6L5_B2" ;
		
		sign = -1.0 ;
		filename = "optics/xi_as_a_function_of_x_graph_b2_140_murad.root" ;
		cout << "Beam 2" << endl ;
	}
	else
	{
		cout << "Wrong beam !" << endl ;
		exit(1) ;
	}

	// #### Open optics root file and load...
	// ###  2 Dispersions
	// ###  2 Horizontal effective lengths
	// ###  1 Vertical effective length


	TFile *optics_file = new TFile(filename.c_str()) ;

	TGraph *near_detector_xi_x = (TGraph *)optics_file->Get((near_detector).c_str()) ;
	TGraph *far_detector_xi_x =  (TGraph *)optics_file->Get((far_detector).c_str()) ;


	near_detector_xi_x_inverse = new TGraph() ;
	far_detector_xi_x_inverse = new TGraph() ;
	
	for(int i = 0 ; i < near_detector_xi_x->GetN() ; ++i)
	{
		double x,y ;

		near_detector_xi_x->GetPoint(i, x, y) ;
		near_detector_xi_x_inverse->SetPoint(i, y, x) ;
		

		far_detector_xi_x->GetPoint(i, x, y) ;
		far_detector_xi_x_inverse->SetPoint(i, y, x) ;
	}

	near_detector_xi_x_inverse->SaveAs("near_detector_xi_x_inverse.root") ;
	far_detector_xi_x_inverse->SaveAs("far_detector_xi_x_inverse.root") ;

	near_detector_xi_Dx = (TGraph *)optics_file->Get(("Dx_" + near_detector).c_str()) ;
	far_detector_xi_Dx =  (TGraph *)optics_file->Get(("Dx_" + far_detector).c_str()) ;
	near_detector_xi_Lx = (TGraph *)optics_file->Get(("Lx_" + near_detector).c_str()) ;
	far_detector_xi_Lx =  (TGraph *)optics_file->Get(("Lx_" + far_detector).c_str()) ;
	near_detector_xi_Ly = (TGraph *)optics_file->Get(("Ly_" + near_detector).c_str()) ;

	if((near_detector_xi_Ly == NULL) || (near_detector_xi_Lx == NULL) || (near_detector_xi_Dx == NULL))
	{
		cout << "A graph is missing !" << endl ;
		exit(1) ;
	}

	// #################################################
	// ####						   #
	// ####		Monte Carlo generation of events   #
	// #### 					   #
	// #################################################

	ofstream MC_events("MC_events.txt") ;
	
	// #### Switches

	double horizontal_dispersion_on = 1.0 ;
	double vertical_dispersion_on 	= 1.0 ;
	double theta_x_star_on 		= 1.0 ;
	double theta_y_star_on 		= 1.0 ;
	
	bool detector_edge		= true ;
	bool collimator			= true ;
	
	double Dy = -1.3e-2 ;

	for(int i = 0 ; i < number_of_particles ; ++i)
	{
	
		// #### IP5 toy model (can be Pythia, etc.)

		double xi		= (horizontal_dispersion_on)*(-gRandom->Uniform(0, 0.25)) ;
		double theta_x_star 	= (theta_x_star_on)*(gRandom->Gaus() * sigma_of_beam_divergence) ;
		double theta_y_star 	= (theta_y_star_on)*(gRandom->Gaus() * sigma_of_beam_divergence) ;

		// #### IP5 -> RP

		double x_near 		= (sign * near_detector_xi_x_inverse->Eval(xi)) + (near_detector_xi_Lx->Eval(-xi) * theta_x_star) ;
		double y_near 		= ((vertical_dispersion_on)*(Dy * xi)) 	+ (near_detector_xi_Ly->Eval(-xi) * theta_y_star) ;
		double x_far 		= (sign * far_detector_xi_x_inverse->Eval(xi)) 	+ (far_detector_xi_Lx->Eval(-xi) * theta_x_star) ;

		// #### Save

		MC_events << x_near << " " << x_far << " " << xi << "  " << theta_x_star << endl ;

		if(detector_edge && (x_near < 1.5e-3)) continue ;
		//if(collimator    && ((sign * xi * near_detector_xi_Dx->Eval(-xi)) > 9e-3)) continue ;

		x_y_hitmap_near->Fill(x_near, y_near) ;

	}
	
	MC_events.close() ;

	// #################################################
	// ####						   #
	// ####	   Reconstruction of previous MC events	   #
	// #### 					   #
	// #################################################
	
	system("date") ;
	
	// Minuit
	
	TMinuit *gMinuit2 = new TMinuit(10);
	gMinuit2->SetFCN(fcn);

    	Double_t arglist[10];
    	Int_t ierflg = 0 ;
    	arglist[0] = 1 ;
    	gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);
    	gMinuit2->Command("SET PRINT -1") ;

	double start_value1= 0 ;
	double start_value2= 0 ;

	// Minuit
	
	double x_near = 0 ;
	double x_far = 0 ;
	double xi = 0 ;
	double theta_x_star = 0 ;

	ifstream MC_events_in("MC_events.txt") ;
	
	while(MC_events_in >> x_near)
	{
		MC_events_in >> x_far >> xi >> theta_x_star ;
		
		x_near_data = x_near ;
		x_far_data = x_far ;

		double theta_x_star_reco = 0 ;
		double xi_reco = x_near / (sign * near_detector_xi_Dx->Eval(0)) ;
		// cout << "xi_reco " << xi_reco << endl ;

		gMinuit2->mnparm(0, "xi", 	       xi_reco, 0.1, 0, 0, ierflg);
		gMinuit2->mnparm(1, "theta_x_star",    theta_x_star_reco, 0.1, 0, 0, ierflg);

		double xi_reco_uncertainty = 0 ;
		double theta_x_star_reco_uncertainty = 0 ;

		// Iterative reconstruction

		arglist[0] = 10000 ;
		arglist[1] = 0 ;

		gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);

		gMinuit2->GetParameter(0, xi_reco, xi_reco_uncertainty) ;
		gMinuit2->GetParameter(1, theta_x_star_reco, theta_x_star_reco_uncertainty) ;
		
		xi_MC_xi_reco->Fill(xi, -xi_reco) ;
		theta_x_star_MC_theta_x_star_reco->Fill(theta_x_star, theta_x_star_reco) ;

	}

	system("date") ;

	// #################################################
	// ####						   #
	// ####	   Reconstruction data			   #
	// #### 					   #
	// #################################################
	
	ifstream data_events("data_events.txt") ;
	ofstream data_events_reconstructed("data_events_reconstructed.txt") ;
	
	while(data_events >> x_near)
	{
		data_events >> x_far ;

		double theta_x_star_reco = 0 ;
		double xi_reco = x_near / (sign * near_detector_xi_Dx->Eval(0)) ;

		// Iterative reconstruction

		// reconstruct_iteratively(x_far, x_near, theta_x_star_reco, xi_reco, 8) ;
		
		data_events_reconstructed << theta_x_star_reco << " " << xi_reco << endl ;
		
	}

	// #################################################
	// ####						   #
	// ####	   Save histograms			   #
	// #### 					   #
	// #################################################

	theta_x_star_MC_theta_x_star_reco->SaveAs("plots/theta_x_star_MC_theta_x_star_reco.root") ;
	xi_MC_xi_reco->SaveAs("plots/xi_MC_xi_reco.root") ;

	TCanvas *c1 = new TCanvas("c1") ;

	gStyle->SetPalette(1) ;
	x_y_hitmap_near->Draw("colz") ;
	c1->SetLogz() ;
	
	c1->SaveAs("plots/x_y_hitmap_near.root") ;
}
	
