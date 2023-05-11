#ifndef TMonteCarlo_h
#define TMonteCarlo_h

#include "TVector3.h"

Double_t my_exponential_distribution(Double_t *x, Double_t *par)
{
      return par[0] * exp(par[1] * (x[0])) ;
}

Double_t flat_distribution(Double_t *x, Double_t *par)
{
      return 1.0 ;
}

class TMonteCarlo : TAnalysisObject
{
	public:

	TMonteCarlo(string, string) ;
	void CalculateThetaStarRadWithBeamDivergence(double &, double &, double, double, double, double, double) ;
} ;

TMonteCarlo::TMonteCarlo(string name, string id) : TAnalysisObject(name, id)
{
}

void TMonteCarlo::CalculateThetaStarRadWithBeamDivergence(double &theta_x_IP5_rad_rot, double &theta_y_IP5_rad_rot, double minus_t_GeV2, double beam_momentum_GeV, double phi_IP5_rad,
		double effective_beam_divergence_rad_x, double effective_beam_divergence_rad_y)
{
	double theta_star_rad = theta_star_rad_from_t_GeV2(-minus_t_GeV2, beam_momentum_GeV) ;

	theta_x_IP5_rad_rot = (theta_star_rad * cos(phi_IP5_rad)) + effective_beam_divergence_rad_x ;
	theta_y_IP5_rad_rot = (theta_star_rad * sin(phi_IP5_rad)) + effective_beam_divergence_rad_y ;
}

#endif // #ifndef TMonteCarlo_h
