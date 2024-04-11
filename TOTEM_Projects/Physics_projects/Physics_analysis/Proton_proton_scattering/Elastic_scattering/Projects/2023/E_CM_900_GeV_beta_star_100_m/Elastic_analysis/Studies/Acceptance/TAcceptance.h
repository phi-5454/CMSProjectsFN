#ifndef TAcceptance_h
#define TAcceptance_h

#include "TAnalysisObject.h"
#include "TFile.h"

#include "TH2D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TMath.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGraph.h"

#include "TProjectParameters.h"

TH2D *global_test_plot = NULL ;


class TAcceptance : public TAnalysisObject
{

	double sigma ;
	double theta_y_star_min ;
	double theta_y_star_max ;
	double beam_momentum ;
	double theta_y_star_rad_left_cut_edge_plus ;
	double theta_y_star_rad_left_cut_edge_minus ;
	double theta_y_star_rad_right_cut_edge_plus ;
	double theta_y_star_rad_right_cut_edge_minus ;

	double theta_x_star_rad_left_cut_edge_plus ;
	double theta_x_star_rad_left_cut_edge_minus ;
	double theta_x_star_rad_right_cut_edge_plus ;
	double theta_x_star_rad_right_cut_edge_minus ;

	double cut_p0 ;
	double cut_p1 ;
	double diagonal ;
	double dx_mm ;
	double dy_mm ;
	double Ly_m ;
	double dLxds_1 ;
	double near_far_RP_distance_m ;
	Double_t theta_x_star_rad_left ;
	Double_t theta_x_star_rad_right ;

	// Parameters for horizontal beam divergence correction

	double sigma_theta_x_star ;

	TF1  *geometrical_acceptance_correction_TF1 ;
	TF1  *beam_divergence_acceptance_correction_TF1 ;
	TF1  *horizontal_beam_divergence_acceptance_correction_TF1 ;
	TF1  *horizontal_beam_divergence_acceptance_correction_TF1_in_angle ;

	string output_directory ;

	public:

	TAcceptance(string, string, string , double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double) ;
	TAcceptance(string, string) ;

	TF1 *GetPreliminaryAcceptance_TF1() ;
	TF1 *GetBeamDivergenceAcceptanceCorrection_TF1() ;	
	TF1 *GetHorizontalBeamDivergenceAcceptanceCorrection_TF1() ;	
	TF1 *GetHorizontalBeamDivergenceAcceptanceCorrection_TF1_in_angle() ;

	void SaveAcceptanceFunctions() ;
	void SetTestPlot(TH2D *a_test_plot) { global_test_plot = a_test_plot ; } ;

	double BeamDivergenceCorrectionFactor(double) ;
} ;

Double_t theta_star_rad_from_t_GeV2(double t_GeV2, double beam_momentum_GeV)
{
	return asin(sqrt(-t_GeV2) / beam_momentum_GeV) ;
}

Double_t t_GeV2_from_theta_star_rad(double theta_star_rad, double beam_momentum_GeV)
{
	double p_GeV = (sin(theta_star_rad) * beam_momentum_GeV) ;
	double t_value_GeV2 = -(p_GeV * p_GeV) ;

	return t_value_GeV2 ;
}

Double_t beam_divergence_correction_factor_per_corner(double t_GeV2, double theta_star_rad, double x_cut, double y_cut, double theta_y_star_rad_left_cut_edge, double theta_y_star_rad_right_cut_edge, double sigma)
{
	double x_left 	= theta_y_star_rad_left_cut_edge ;
	double y_left 	= (2 * theta_star_rad) + theta_y_star_rad_left_cut_edge ;

	double x_right 	= -(2 * theta_star_rad) + theta_y_star_rad_right_cut_edge ;
	double y_right 	= theta_y_star_rad_right_cut_edge ;
	
	double vector_from_mean_to_left_edge_abs  = sqrt(pow((x_cut - x_left),2) + pow((y_cut - y_left),2)) ;
	double vector_from_mean_to_right_edge_abs = sqrt(pow(x_cut - x_right,2) + pow((y_cut - y_right),2)) ;
	
	double scalar_product = ((x_cut - x_left)*(x_cut - x_right)) + ((y_cut - y_left) * (y_cut - y_right)) ;
	
	double sign = 1.0 ;

	if(scalar_product > 0) sign = -1.0 ;
	
	double sigma_times_sqrt_2 = (sqrt(2.0) * sigma) ;

	double visible_sigma_left  = ((TMath::Erf(vector_from_mean_to_left_edge_abs  / sigma_times_sqrt_2)) / 2.0) ;
        double visible_sigma_right = ((TMath::Erf(vector_from_mean_to_right_edge_abs / sigma_times_sqrt_2)) / 2.0) ;
	
	double visible_sigma = fabs(visible_sigma_left + (sign * visible_sigma_right)) ;

	double correction_factor = (1 / visible_sigma) ;
	
	// cout << "sigma_of_correction: " << sigma << endl ;

	/*if(global_test_plot != NULL)
	{

		TEllipse *p1_circle = new TEllipse(x_cut, y_cut, (sigma/10.0)) ;
		p1_circle->SetFillColor(kBlack) ;
		p1_circle->SetLineColor(kBlack) ;
		p1_circle->Draw() ;

		TEllipse *p2_circle = new TEllipse(x_left, y_left, (sigma/10.0)) ;
		p2_circle->SetFillColor(kBlack) ;
		p2_circle->SetLineColor(kBlack) ;
		p2_circle->Draw() ;

		TEllipse *p3_circle = new TEllipse(x_right, y_right, (sigma/10.0)) ;
		p3_circle->SetFillColor(kBlack) ;
		p3_circle->SetLineColor(kBlack) ;
		p3_circle->Draw() ;
		
		TLine *line_1 = new TLine(x_cut, y_cut, x_left, y_left) ;
		line_1->SetLineColor(kRed) ;
		line_1->Draw() ;

		TLine *line_2 = new TLine(x_cut, y_cut, x_right, y_right) ;
		line_2->SetLineColor(kBlue) ;
		line_2->Draw() ;

		TLine *line_3 = new TLine(-x_cut, -y_cut, -x_right, -y_right) ;
		line_3->SetLineColor(kBlue) ;
		line_3->Draw() ;

		stringstream ss_t_GeV2, ss_correction_factor, ss_visible_sigma_left, ss_visible_sigma_right ;
		
		ss_t_GeV2 << t_GeV2 ;
		ss_visible_sigma_left << visible_sigma_left ;
		ss_visible_sigma_right << visible_sigma_right ;
		ss_correction_factor << correction_factor ;
				
		TLatex *text_t_GeV2 = new TLatex ;
		text_t_GeV2->SetTextSize(0.01) ;
		text_t_GeV2->DrawLatex(x_cut, y_cut, ("t=" + ss_t_GeV2.str() + " GeV^{2} : #times" + ss_correction_factor.str()).c_str()) ;

		TLatex *text_t_left = new TLatex ;
		text_t_left->SetTextSize(0.01) ;
		text_t_left->DrawLatex(x_left, y_left  + (sigma/10.0), (ss_visible_sigma_left.str()).c_str()) ;

		TLatex *text_t_right = new TLatex ;
		text_t_right->SetTextSize(0.01) ;
		text_t_right->DrawLatex(x_right + (sigma/10.0), y_right, (ss_visible_sigma_right.str()).c_str()) ;

	}*/

	return correction_factor ;

}

double TAcceptance::BeamDivergenceCorrectionFactor(double t_GeV2)
{
	double theta_star_rad = theta_star_rad_from_t_GeV2(t_GeV2, beam_momentum) ;
	
	if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM) theta_star_rad *= -1 ;

	double x_cut = ((2 * theta_star_rad) - cut_p0) / (cut_p1 - 1) ;
	double y_cut = (2 * theta_star_rad)  + x_cut ;

	double correction_factor_corner_1 = beam_divergence_correction_factor_per_corner(t_GeV2, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_plus  , theta_y_star_rad_right_cut_edge_minus, sigma) ;
	double correction_factor_corner_2 = beam_divergence_correction_factor_per_corner(t_GeV2, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_minus , theta_y_star_rad_right_cut_edge_plus,  sigma) ;

	double correction_factor = correction_factor_corner_1 ;

	if(correction_factor_corner_1 < correction_factor_corner_2) correction_factor = correction_factor_corner_2 ;

	return correction_factor ;

}

Double_t beam_divergence_correction_factor_in_angle(Double_t *angle, Double_t *par)
{
	Double_t beam_momentum 				= par[0] ;
	Double_t theta_y_star_rad_min 			= par[1] ;
	Double_t theta_y_star_rad_max 			= par[2] ;
	Double_t theta_y_star_rad_left_cut_edge_plus 	= par[3] ;
	Double_t theta_y_star_rad_left_cut_edge_minus 	= par[4] ;
	Double_t theta_y_star_rad_right_cut_edge_plus 	= par[5] ;
	Double_t theta_y_star_rad_right_cut_edge_minus 	= par[6] ;
	Double_t cut_p0 				= par[7] ;
	Double_t cut_p1 				= par[8] ;
	Double_t sigma 					= par[9] ;
	Double_t diagonal 				= par[10] ;

	double theta_star_rad = (*angle) ;

	if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP) theta_star_rad *= -1 ;

	double x_cut = ((2 * theta_star_rad) - cut_p0) / (cut_p1 - 1) ;
	double y_cut = (2 * theta_star_rad)  + x_cut ;

	double correction_factor_corner_1 = beam_divergence_correction_factor_per_corner(0, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_plus  , theta_y_star_rad_right_cut_edge_minus, sigma) ;
	double correction_factor_corner_2 = beam_divergence_correction_factor_per_corner(0, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_minus , theta_y_star_rad_right_cut_edge_plus,  sigma) ;

	double correction_factor = correction_factor_corner_1 ;

	if(correction_factor_corner_1 < correction_factor_corner_2) correction_factor = correction_factor_corner_2 ;

	return correction_factor ;
}

Double_t beam_divergence_correction_factor(Double_t *pt_GeV2, Double_t *par)
{
	Double_t beam_momentum 				= par[0] ;
	Double_t theta_y_star_rad_min 			= par[1] ;
	Double_t theta_y_star_rad_max 			= par[2] ;
	Double_t theta_y_star_rad_left_cut_edge_plus 	= par[3] ;
	Double_t theta_y_star_rad_left_cut_edge_minus 	= par[4] ;
	Double_t theta_y_star_rad_right_cut_edge_plus 	= par[5] ;
	Double_t theta_y_star_rad_right_cut_edge_minus 	= par[6] ;
	Double_t cut_p0 				= par[7] ;
	Double_t cut_p1 				= par[8] ;
	Double_t sigma 					= par[9] ;
	Double_t diagonal 				= par[10] ;

	Double_t t_GeV2 = -(*pt_GeV2) ;

	double theta_star_rad = theta_star_rad_from_t_GeV2(t_GeV2, beam_momentum) ;

	if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM) theta_star_rad *= -1 ;

	double x_cut = ((2 * theta_star_rad) - cut_p0) / (cut_p1 - 1) ;
	double y_cut = (2 * theta_star_rad)  + x_cut ;

	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER

	double correction_factor_corner_1 = beam_divergence_correction_factor_per_corner(t_GeV2, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_plus  , theta_y_star_rad_right_cut_edge_minus, sigma) ;
	double correction_factor_corner_2 = beam_divergence_correction_factor_per_corner(t_GeV2, theta_star_rad, x_cut, y_cut, theta_y_star_rad_left_cut_edge_minus , theta_y_star_rad_right_cut_edge_plus,  sigma) ;

	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER

	double correction_factor = correction_factor_corner_1 ;

	if(correction_factor_corner_1 < correction_factor_corner_2) correction_factor = correction_factor_corner_2 ;

	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER

	return correction_factor ;

	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER
	// Remove the perturbation !!!!!!!!!!!!!!!!!!!!!!!!OO!!!!!!!!!!!!!!!!!!!!! DANGER

}

double theta_star_result_for_test ;
double dphi_for_test ;

vector<double> xvalues ;
vector<double> yvalues ;

double accep_based_on_frame(double theta_star_rad)
{
	int number_of_points = xvalues.size() ;

	if(number_of_points < 3)
	{
		cout << "There has to be at least 3 points for an acceptance frame" << endl ;
		exit(1) ;
	}

	double x1, y1 ;
	x1 = xvalues[0] ;
	y1 = yvalues[0] ;

	for(int i = 1 ; i < number_of_points ; ++i)
	{
		cout << x1 << endl ;
		double x2, y2 ;

		x2 = xvalues[i] ;
		y2 = yvalues[i] ;

		double m = (y2 - y1) / (x2 - x1) ;
		double b = y1 - m * x1 ;

		x1 = x2 ;
		y1 = y2 ;

	}

	cout << "an acceptance computed" << endl ;

}

Double_t preliminary_acceptance_beam_1_diagonal_left_bottom_right_top_function(Double_t *pt_GeV2, Double_t *par)
{
	Double_t beam_1_momentum 	= par[0] ;
	Double_t theta_y_star_rad_min 	= par[1] ;
	Double_t theta_y_star_rad_max 	= par[2] ;
	Double_t theta_x_star_rad_left	= par[3] ;
	Double_t theta_x_star_rad_right	= par[4] ;

	Double_t t_GeV2 = -(*pt_GeV2) ;
	double theta_star_rad = theta_star_rad_from_t_GeV2(t_GeV2, beam_1_momentum) ;

	if(xvalues.size() != 0)
	{
		// double value = accep_based_on_frame(theta_star_rad) ;
	}
	
	if(theta_star_rad < theta_y_star_rad_min) return 0 ;
	
	double PhiMin = 0.0 ;

	if((theta_y_star_rad_min * theta_y_star_rad_max) > 0) PhiMin = asin(theta_y_star_rad_min / theta_star_rad) ; // if in MC the vertical acceptance cuts are switched off by setting large cuts

	double PhiMax = (TMath::Pi() / 2.0) ;
	
	if(theta_star_rad > theta_y_star_rad_max)
	{
		PhiMax = asin(theta_y_star_rad_max / theta_star_rad) ;
	}

	double VisiblePhiPerDiagonalRadLeft  = (PhiMax - PhiMin) ;
	double VisiblePhiPerDiagonalRadRight = (PhiMax - PhiMin) ;

	if((theta_x_star_rad_left != 0) && (fabs(theta_x_star_rad_left) <= theta_star_rad))
	{
		double PhiMin_from_theta_x_star_rad_left = acos(fabs(theta_x_star_rad_left) / theta_star_rad) ;

		if(PhiMin_from_theta_x_star_rad_left > PhiMin)
		{
			VisiblePhiPerDiagonalRadLeft  = (PhiMax - PhiMin_from_theta_x_star_rad_left) ;
			if(VisiblePhiPerDiagonalRadLeft < 0) VisiblePhiPerDiagonalRadLeft = 0 ;
		}
	}

	if((theta_x_star_rad_right != 0) && (fabs(theta_x_star_rad_right) <= theta_star_rad))
	{
		double PhiMin_from_theta_x_star_rad_right = acos(fabs(theta_x_star_rad_right) / theta_star_rad) ;

		if(PhiMin_from_theta_x_star_rad_right > PhiMin)
		{
			VisiblePhiPerDiagonalRadRight = (PhiMax - PhiMin_from_theta_x_star_rad_right) ;
			if(VisiblePhiPerDiagonalRadRight < 0) VisiblePhiPerDiagonalRadRight = 0 ;
		}
	}

	double VisiblePhiPerDiagonalRad = (VisiblePhiPerDiagonalRadLeft + VisiblePhiPerDiagonalRadRight) ;

	double f = (VisiblePhiPerDiagonalRad / (2.0 * TMath::Pi())) ;

	// cout << "f factor:" << f << "theta_star_rad " << theta_star_rad << "  theta_y_star_rad_max" << theta_y_star_rad_max << endl ;

	if (f != 0) return (1.0 / f) ;
	else return 0 ;
}

TAcceptance::TAcceptance(string name, string id, string output_directory, double theta_y_star_min, double theta_y_star_max, double beam_momentum,
	double theta_y_star_rad_left_cut_edge_plus,  double theta_y_star_rad_left_cut_edge_minus, double theta_y_star_rad_right_cut_edge_plus, double theta_y_star_rad_right_cut_edge_minus,
	double cut_p0, double cut_p1, double diagonal, double sigma, double theta_x_star_rad_left, double theta_x_star_rad_right,
	double theta_x_star_rad_left_cut_edge_plus,  double theta_x_star_rad_left_cut_edge_minus, double theta_x_star_rad_right_cut_edge_plus, double theta_x_star_rad_right_cut_edge_minus) : TAnalysisObject(name, id),
	geometrical_acceptance_correction_TF1(NULL),
	beam_divergence_acceptance_correction_TF1(NULL),
	horizontal_beam_divergence_acceptance_correction_TF1(NULL),
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle(NULL),
	sigma(sigma),
	output_directory(output_directory),
	theta_y_star_min(theta_y_star_min),
	theta_y_star_max(theta_y_star_max),
	beam_momentum(beam_momentum),
	theta_y_star_rad_left_cut_edge_plus(theta_y_star_rad_left_cut_edge_plus),
	theta_y_star_rad_left_cut_edge_minus(theta_y_star_rad_left_cut_edge_minus),
	theta_y_star_rad_right_cut_edge_plus(theta_y_star_rad_right_cut_edge_plus),
	theta_y_star_rad_right_cut_edge_minus(theta_y_star_rad_right_cut_edge_minus),
	cut_p0(cut_p0),
	cut_p1(cut_p1),
	diagonal(diagonal),
	dx_mm(dx_mm) ,
	dy_mm(dy_mm) ,
	Ly_m(dy_mm) ,
	dLxds_1(dy_mm) ,
	near_far_RP_distance_m(dy_mm),
	theta_x_star_rad_left(theta_x_star_rad_left),
	theta_x_star_rad_right(theta_x_star_rad_right),
	sigma_theta_x_star(0),
	theta_x_star_rad_left_cut_edge_plus(theta_x_star_rad_left_cut_edge_plus),
	theta_x_star_rad_left_cut_edge_minus(theta_x_star_rad_left_cut_edge_minus),
	theta_x_star_rad_right_cut_edge_plus(theta_x_star_rad_right_cut_edge_plus),
	theta_x_star_rad_right_cut_edge_minus(theta_x_star_rad_right_cut_edge_minus)
{
	double abs_t_GeV2_low = 0.0 ;
	double abs_t_GeV2_high = 4.0 ; // was 2.0 until 2016.04.07. 16:00 !
	int number_of_parameters = 11 ;
	int number_of_points_in_t_GeV2 = 10000000 ;

	geometrical_acceptance_correction_TF1 = new TF1("geometrical_acceptance_correction_TF1", preliminary_acceptance_beam_1_diagonal_left_bottom_right_top_function, abs_t_GeV2_low, abs_t_GeV2_high, number_of_parameters);
	geometrical_acceptance_correction_TF1->SetNpx(number_of_points_in_t_GeV2) ;
	geometrical_acceptance_correction_TF1->SetParameters(beam_momentum, theta_y_star_min, theta_y_star_max, theta_x_star_rad_left, theta_x_star_rad_right) ;
	geometrical_acceptance_correction_TF1->SetParNames("beam_momentum", "min_t_value_GeV");
	geometrical_acceptance_correction_TF1->SetParNames("beam_momentum", "max_t_value_GeV");
	geometrical_acceptance_correction_TF1->Draw();

	beam_divergence_acceptance_correction_TF1 = new TF1("beam_divergence_acceptance_correction_TF1", beam_divergence_correction_factor, abs_t_GeV2_low, abs_t_GeV2_high, number_of_parameters);
	beam_divergence_acceptance_correction_TF1->SetNpx(number_of_points_in_t_GeV2) ;
	beam_divergence_acceptance_correction_TF1->SetParameters(beam_momentum, theta_y_star_min, theta_y_star_max, theta_y_star_rad_left_cut_edge_plus, theta_y_star_rad_left_cut_edge_minus, theta_y_star_rad_right_cut_edge_plus, theta_y_star_rad_right_cut_edge_minus, cut_p0, cut_p1, sigma, diagonal) ;
	beam_divergence_acceptance_correction_TF1->SetParNames("beam_1_momentum", "min_t_value_GeV");
	beam_divergence_acceptance_correction_TF1->SetParNames("beam_1_momentum", "max_t_value_GeV");
	beam_divergence_acceptance_correction_TF1->Draw();

	horizontal_beam_divergence_acceptance_correction_TF1 = new TF1("horizontal_beam_divergence_acceptance_correction_TF1", beam_divergence_correction_factor, abs_t_GeV2_low, abs_t_GeV2_high, number_of_parameters);
	horizontal_beam_divergence_acceptance_correction_TF1->SetNpx(number_of_points_in_t_GeV2) ;
	horizontal_beam_divergence_acceptance_correction_TF1->SetParameters(beam_momentum, theta_x_star_rad_left, theta_x_star_rad_right, theta_x_star_rad_left_cut_edge_plus, theta_x_star_rad_left_cut_edge_minus, theta_x_star_rad_right_cut_edge_plus, theta_x_star_rad_right_cut_edge_minus, 0.0, -1.0, sigma, diagonal) ;
	horizontal_beam_divergence_acceptance_correction_TF1->SetParNames("beam_1_momentum", "min_t_value_GeV");
	horizontal_beam_divergence_acceptance_correction_TF1->SetParNames("beam_1_momentum", "max_t_value_GeV");
	horizontal_beam_divergence_acceptance_correction_TF1->Draw();

	double theta_x_star_angular_limit = 1.5e-3 ;

	horizontal_beam_divergence_acceptance_correction_TF1_in_angle = new TF1("horizontal_beam_divergence_acceptance_correction_TF1_in_angle", beam_divergence_correction_factor_in_angle, -theta_x_star_angular_limit, theta_x_star_angular_limit, number_of_parameters);
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->SetNpx(number_of_points_in_t_GeV2) ;
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->SetParameters(beam_momentum, theta_x_star_rad_left, theta_x_star_rad_right, theta_x_star_rad_left_cut_edge_plus, theta_x_star_rad_left_cut_edge_minus, theta_x_star_rad_right_cut_edge_plus, theta_x_star_rad_right_cut_edge_minus, 0.0, -1.0, sigma, diagonal) ;
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->SetParNames("beam_1_momentum", "min_t_value_GeV");
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->SetParNames("beam_1_momentum", "max_t_value_GeV");
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->Draw();

}

TF1 *TAcceptance::GetPreliminaryAcceptance_TF1()
{
	if(geometrical_acceptance_correction_TF1 == NULL)
	{
		cout << "Error: In TAcceptance::GetPreliminaryAcceptance_TF1(): preliminary_acceptance_beam_1 is NULL !" << endl ;
		exit(1) ;
	}

	if(sigma == 0.0)
	{
		cout << "Error: In TAcceptance::GetPreliminaryAcceptance_TF1(): sigma = 0 !" << endl ;
		exit(1) ;
	}
	
	return geometrical_acceptance_correction_TF1 ;
}

TF1 *TAcceptance::GetBeamDivergenceAcceptanceCorrection_TF1()
{
	if(geometrical_acceptance_correction_TF1 == NULL)
	{
		cout << "Error: In TAcceptance::GetPreliminaryAcceptance_TF1(): preliminary_acceptance_beam_1 is NULL !" << endl ;
		exit(1) ;
	}

	if(sigma == 0.0)
	{
		cout << "Error: In TAcceptance::GetPreliminaryAcceptance_TF1(): sigma = 0 !" << endl ;
		exit(1) ;
	}
	
	return beam_divergence_acceptance_correction_TF1 ;
}

TF1 *TAcceptance::GetHorizontalBeamDivergenceAcceptanceCorrection_TF1_in_angle()
{
	if(horizontal_beam_divergence_acceptance_correction_TF1_in_angle == NULL)
	{
		cout << "Error: In TAcceptance::GetHorizontalBeamDivergenceAcceptanceCorrection_TF1_in_angle(): preliminary_acceptance_beam_1 is NULL !" << endl ;
		exit(1) ;
	}

	if(sigma == 0.0)
	{
		cout << "Error: In TAcceptance::GetHorizontalBeamDivergenceAcceptanceCorrection_TF1_in_angle(): sigma = 0 !" << endl ;
		exit(1) ;
	}
	
	return horizontal_beam_divergence_acceptance_correction_TF1_in_angle ;
}


double dt_due_to_alignment(Double_t *pt_GeV2, Double_t *par)
{
	Double_t beam_momentum_GeV	 = par[0] ;
	Double_t theta_y_star_rad_min 	 = par[1] ;
	Double_t theta_y_star_rad_max 	 = par[2] ;
	Double_t dx_mm			 = par[3] ;
	Double_t dy_mm			 = par[4] ;
	Double_t Ly_mm			 = par[5] ;
	Double_t dLxds_1		 = par[6] ;
	Double_t near_far_RP_distance_mm = par[7] ;
	Double_t Determinant_x_mm 	 = par[8] ;
	Double_t Lx_mm			 = par[9] ;
	Double_t dvxds 			 = par[10] ;
	

	Double_t t_GeV2 = -(*pt_GeV2) ;
	double theta_star_rad = theta_star_rad_from_t_GeV2(t_GeV2, beam_momentum_GeV) ;

	// Perturbations

	double perturbation_x = (dx_mm / (dLxds_1 * near_far_RP_distance_mm)) - ((dvxds * dx_mm) / (Determinant_x_mm * Lx_mm)) ;
	double perturbation_y = (dy_mm / Ly_mm) ;

	Double_t theta_y_star_rad_min_perturbed = theta_y_star_rad_min - perturbation_y ;
	Double_t theta_y_star_rad_max_perturbed = theta_y_star_rad_max - perturbation_y ;
	
	if(theta_star_rad < theta_y_star_rad_min) return 0 ;

	// 0 order

	double PhiMin = asin(theta_y_star_rad_min / theta_star_rad) ;
	double PhiMax = (TMath::Pi() / 2.0) ;

	if(theta_star_rad > theta_y_star_rad_max)
	{
		PhiMax = asin(theta_y_star_rad_max / theta_star_rad) ;
	}

	double VisiblePhiPerDiagonalRad = (PhiMax - PhiMin) ;

	// Perturbed: 1 order

	double PhiMin_perturbed = asin(theta_y_star_rad_min_perturbed / theta_star_rad) ;
	double PhiMax_perturbed = (TMath::Pi() / 2.0) ;

	if(theta_star_rad > theta_y_star_rad_max_perturbed)
	{
		PhiMax_perturbed = asin(theta_y_star_rad_max_perturbed / theta_star_rad) ;
	}

	double VisiblePhiPerDiagonalRad_perturbed = (PhiMax_perturbed - PhiMin_perturbed) ;

	cout << "t_GeV2: " 	<< 	t_GeV2 << endl ;
	cout << "theta_star_rad: "<<	theta_star_rad << endl ;	
	cout << "dx_mm: " 	<< 	dx_mm << endl ;
	cout << "dy_mm: " 	<< 	dy_mm << endl ;
	cout << "Ly_mm: " 	<< 	Ly_mm << endl ;
	cout << "dLxds_1: " 	<< 	dLxds_1 << endl ;
	cout << "near_far_RP_distance_mm: " << near_far_RP_distance_mm << endl ;
	cout << "perturbation_x: " 	<< 	perturbation_x << endl ;
	cout << "perturbation_y: " 	<< 	perturbation_y << endl ;

	double term_1 = (-pow(beam_momentum_GeV,2)) * (pow(perturbation_x, 2) + pow(perturbation_y, 2)) ; 

	cout << "delta VisiblePhiPerDiagonalRad: " << ((VisiblePhiPerDiagonalRad_perturbed - VisiblePhiPerDiagonalRad) / VisiblePhiPerDiagonalRad) << endl ;

	double term_2 = ((2 * theta_star_rad * (-pow(beam_momentum_GeV,2))) / VisiblePhiPerDiagonalRad_perturbed) *
			((((sin(PhiMax_perturbed) * perturbation_x) - (cos(PhiMax_perturbed) * perturbation_y)) - ((sin(PhiMin_perturbed) * perturbation_x) - (cos(PhiMin_perturbed) * perturbation_y)))) ;

	cout << "term_1: " << term_1 << "term_2: " << term_2 << endl ;

	double result = term_1 + (1.0 * term_2) ;

	return result ;
}

void TAcceptance::SaveAcceptanceFunctions()
{
	geometrical_acceptance_correction_TF1->SaveAs((output_directory + "/geometrical_acceptance_correction_TF1.root").c_str()) ;

	beam_divergence_acceptance_correction_TF1->SaveAs((output_directory + "/beam_divergence_acceptance_correction_TF1.root").c_str()) ;
	horizontal_beam_divergence_acceptance_correction_TF1->SaveAs((output_directory + "/horizontal_beam_divergence_acceptance_correction_TF1.root").c_str()) ;
	horizontal_beam_divergence_acceptance_correction_TF1_in_angle->SaveAs((output_directory + "/horizontal_beam_divergence_acceptance_correction_TF1_in_angle.root").c_str()) ;
}

#endif // #ifndef TAcceptance_h
