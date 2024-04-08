#ifndef TProtonReconstruction_h
#define TProtonReconstruction_h

#include "TConstants.h"
#include "TBeamOptics.h"
#include "TRPAlignment.h"

#include "TMath.h"
#include "math.h"
#include <set>

class TProtonReconstruction  
{
	private:
	
	double near_far_RP_units_distance_mm_Beam_1 ;
	double near_far_RP_units_distance_mm_Beam_2 ;

	set<string> *list_of_excluded_variables ;
	bool align_source ;
        bool use_vertex_for_theta_x_star ;

	ULong64_t event_info_timestamp ;
	UInt_t trigger_data_run_num ; 
	UInt_t trigger_data_event_num ;

	Double_t x_l_f_mm, x_l_n_mm, x_r_f_mm, x_r_n_mm ;
	Double_t y_l_f_mm, y_l_n_mm, y_r_f_mm, y_r_n_mm ;

	Double_t x_l_f_aligned_mm, x_l_n_aligned_mm, x_r_f_aligned_mm, x_r_n_aligned_mm ;
	Double_t y_l_f_aligned_mm, y_l_n_aligned_mm, y_r_f_aligned_mm, y_r_n_aligned_mm ;

	// Local angles

	Double_t thx_l_f_rad, thx_l_n_rad, thx_r_f_rad, thx_r_n_rad ;
	Double_t thy_l_f_rad, thy_l_n_rad, thy_r_f_rad, thy_r_n_rad ;

	// Coordinate differences

	double dx_left_mm, dy_left_mm, dx_right_mm, dy_right_mm ;

	// Local angles
	
	double theta_x_left_rad, theta_x_right_rad ;
	double theta_y_left_rad, theta_y_right_rad ;

	// IP5 angles 

	double theta_x_star_right_far_rad ;
	double theta_x_star_right_near_rad ;
	double theta_x_star_left_near_rad ;
	double theta_x_star_left_far_rad ;

	double theta_x_star_left_rad, theta_x_star_right_rad ;
	double theta_y_star_left_rad, theta_y_star_right_rad ;

	double theta_x_star_left_rad_aligned, theta_x_star_right_rad_aligned ;
	double theta_y_star_left_rad_aligned, theta_y_star_right_rad_aligned ;

	double theta_y_star_left_near_rad ;
	double theta_y_star_left_far_rad ;
	double theta_y_star_right_near_rad ;
	double theta_y_star_right_far_rad ;

	double theta_y_star_without_right_near_rad ;
	double theta_y_star_without_right_far_rad ;
	double theta_y_star_without_left_near_rad ;
	double theta_y_star_without_left_far_rad ;

	double minus_t_without_right_near_GeV2 ;
	double minus_t_without_right_far_GeV2 ;
	double minus_t_without_left_near_GeV2 ;
	double minus_t_without_left_far_GeV2 ;

	double theta_x_star_without_right_near_rad ;
	double theta_x_star_without_right_far_rad ;
	double theta_x_star_without_left_near_rad ;
	double theta_x_star_without_left_far_rad ;

	double minus_t_y_without_right_near_GeV2 ;
	double minus_t_y_without_right_far_GeV2 ;
	double minus_t_y_without_left_near_GeV2 ; 
	double minus_t_y_without_left_far_GeV2 ; 

	double minus_t_x_without_right_near_GeV2 ;
	double minus_t_x_without_right_far_GeV2 ;
	double minus_t_x_without_left_near_GeV2 ; 
	double minus_t_x_without_left_far_GeV2 ; 

	double theta_x_star_rad, theta_y_star_rad ;
	double theta_x_star_far_rad, theta_y_star_far_rad ;
	double theta_x_star_rad_aligned, theta_y_star_rad_aligned ;
	
	double theta_star_rad ;
	double theta_star_without_right_far_rad ;
	double theta_star_without_left_far_rad ;
	
	double phi_star_rad ;
	double phi_star_without_right_far_rad ;
	double phi_star_without_left_far_rad ;

	// IP5 azimuthal angle
	
	// Vertex position

	double x_star_left_mm, x_star_right_mm ;
	double y_star_left_mm, y_star_right_mm ;

	double y_star_mm ;

	double x_star_mm ;

	// t value

	double t_left_GeV2, t_right_GeV2, t_GeV2, t_y_GeV2, t_left_far_GeV2, t_right_far_GeV2 ;
	double minus_t_left_GeV2, minus_t_right_GeV2, minus_t_GeV2, minus_t_y_GeV2, minus_t_y_aligned_GeV2 ;

	double minus_t_aligned_GeV2 ;

	double minus_t_from_theta_y_GeV2 ;

	// Validity
	
	bool valid ;
	bool elastic ;
	
	public:

	TProtonReconstruction() ;
	TProtonReconstruction(double, double, set<string> *, bool, bool) ;

	void Print() ;
	double ReconstructThetaYStarRad(double, double, TBeamOptics *) ;
	double ReconstructXStarMm(double, double, TBeamOptics *) ;
	double ReconstructYStarMm(double, double, TBeamOptics *) ;
	double ReconstructThetaXStarRad(double , double, TBeamOptics *) ;
	double ReconstructFourMomentumTransferSquaredGeV2(double , double , TBeamOptics *) ;
	double ReconstructThetaXStarRadFromPosition(double , double, double, TBeamOptics *) ;

	void Delete() ;
	void Reconstruct(ULong64_t event_info_timestamp, UInt_t trigger_data_run_num, UInt_t trigger_data_event_num,
	 double x_l_n_mm, double x_l_f_mm, double x_r_n_mm, double x_r_f_mm, double y_l_n_mm, double y_l_f_mm, double y_r_n_mm, double y_r_f_mm,
							  double thx_l_n_mm, double thx_l_f_mm, double thx_r_n_mm, double thx_r_f_mm, double thy_l_n_mm, double thy_l_f_mm, double thy_r_n_mm, double thy_r_f_mm,
							  TBeamOptics *BeamOptics_Beam_1, TBeamOptics *BeamOptics_Beam_2, TRPAlignment *) ;

	bool IsValid() { return (valid == true) ; } ;

	ULong64_t *Get_event_info_timestamp() { return &event_info_timestamp ; } ;

	const double *GetReconstructedVariable(string) ;
	void SetVariable(string , double ) ;

	double GetThetaXRightRad() { return theta_x_right_rad ; } ;
	double GetThetaYRightRad() { return theta_y_right_rad ; } ;

	double GetThetaXStarRightRad() { return theta_x_star_right_rad ; } ;
	double GetThetaYStarRightRad() { return theta_y_star_right_rad ; } ;

	void SetElastic(bool elasticity) 	{ elastic = elasticity ; }  ;
	bool IsElastic() 			{ return elastic ; }  ;
	const bool *GetElasticPointer() ;
	const UInt_t *GetTriggerDataRunNum() ;
	const UInt_t *GetTriggerDataEventNum() ;
} ;

TProtonReconstruction::TProtonReconstruction()
{
}

TProtonReconstruction::TProtonReconstruction(double near_far_RP_units_distance_m_Beam_1 , double near_far_RP_units_distance_m_Beam_2 , set<string> *list_of_excluded_variables, bool align_source, bool use_vertex_for_theta_x_star) : list_of_excluded_variables(list_of_excluded_variables), align_source(align_source), use_vertex_for_theta_x_star(use_vertex_for_theta_x_star)
{
	near_far_RP_units_distance_mm_Beam_1 = (near_far_RP_units_distance_m_Beam_1 * TConstants::conversion_factor_from_m_to_mm) ;
	near_far_RP_units_distance_mm_Beam_2 = (near_far_RP_units_distance_m_Beam_2 * TConstants::conversion_factor_from_m_to_mm) ;
}

void TProtonReconstruction::Print()
{
	cout << "x_l_f_mm: " << x_l_f_mm << "x_l_n_mm: " <<  x_l_n_mm << endl ;
	cout << "x_r_f_mm: " << x_r_f_mm << "x_r_n_mm: " <<  x_r_n_mm << endl << endl ;

	cout << "theta_x_left_rad: " << theta_x_left_rad << endl ;
	cout << "theta_x_right_rad: " << theta_x_right_rad << endl << endl ;

	cout << "theta_x_star_left_rad: " << theta_x_star_left_rad << endl ;
	cout << "theta_x_star_right_rad: " << theta_x_star_right_rad << endl ;

	cout << "theta_x_star_rad: " << theta_x_star_rad << endl ;
}

const double *TProtonReconstruction::GetReconstructedVariable(string variable_name)
{
	// cout << "variable_name: " << variable_name << endl ;

	if(list_of_excluded_variables != NULL)
	{
		bool found = false ;

		// cout << "Before for !" << endl ;

		for(set<string> ::iterator it = list_of_excluded_variables->begin() ; it != list_of_excluded_variables->end() ; ++it)
		{
			// cout << "	Comparing " << (*it) << " to " << variable_name << endl ;

			if(it->compare(variable_name) == 0.0)
			{	
				found = true ;
				break ;
			}
		}

		if(found)
		{
			cout << "Error: TProtonReconstruction::GetReconstructedVariable: the requested variable \"" + variable_name + "\" is exluded (e.q. due to 3/4 inefficiency calculation)" <<  endl ;
			exit(1) ;
		}
	}

	// cout << "After test: " << variable_name << endl ;

	if(variable_name.compare("phi_star_rad")==0)
	{
		return &phi_star_rad ; 
	}
	if(variable_name.compare("phi_star_without_right_far_rad")==0)
	{
		return &phi_star_without_right_far_rad ; 
	}
	if(variable_name.compare("phi_star_without_left_far_rad")==0)
	{
		return &phi_star_without_left_far_rad ; 
	}
	else if(variable_name.compare("minus_t_GeV2")==0)
	{
		return &minus_t_GeV2 ; 
	}
	else if(variable_name.compare("theta_star_rad")==0)
	{
		return &theta_star_rad ; 
	}
	else if(variable_name.compare("theta_star_without_right_far_rad")==0)
	{
		return &theta_star_without_right_far_rad ; 
	}
	else if(variable_name.compare("theta_star_without_left_far_rad")==0)
	{
		return &theta_star_without_left_far_rad ; 
	}
	else if(variable_name.compare("dx_left_mm")==0)
	{
		return &dx_left_mm ; 
	}
	else if(variable_name.compare("dx_right_mm")==0)
	{
		return &dx_right_mm ; 
	}
	else if(variable_name.compare("dy_left_mm")==0)
	{
		return &dy_left_mm ; 
	}
	else if(variable_name.compare("dy_right_mm")==0)
	{
		return &dy_right_mm ; 
	}
	else if(variable_name.compare("theta_x_left_rad")==0)
	{
		return &theta_x_left_rad ; 
	}
	else if(variable_name.compare("theta_x_right_rad")==0)
	{
		return &theta_x_right_rad ; 
	}
	else if(variable_name.compare("theta_x_star_left_rad")==0)
	{
		return &theta_x_star_left_rad ; 
	}
	else if(variable_name.compare("theta_x_star_rad")==0)
	{
		return &theta_x_star_rad ; 
	}
        else if(variable_name.compare("theta_x_star_far_rad")==0)
        {
                return &theta_x_star_far_rad ;
        }
	else if(variable_name.compare("theta_x_star_rad_aligned")==0)
	{
		return &theta_x_star_rad_aligned ; 
	}
	else if(variable_name.compare("theta_x_star_right_rad")==0)
	{
		return &theta_x_star_right_rad ; 
	}
	else if(variable_name.compare("theta_x_star_right_far_rad")==0)
	{
		return &theta_x_star_right_far_rad ; 
	}
	else if(variable_name.compare("theta_x_star_left_far_rad")==0)
	{
		return &theta_x_star_left_far_rad ; 
	}
	else if(variable_name.compare("theta_x_star_right_near_rad")==0)
	{
		return &theta_x_star_right_near_rad ; 
	}
	else if(variable_name.compare("theta_x_star_left_near_rad")==0)
	{
		return &theta_x_star_left_near_rad ; 
	}
	else if(variable_name.compare("theta_y_left_rad")==0)
	{
		return &theta_y_left_rad ; 
	}
	else if(variable_name.compare("theta_y_right_rad")==0)
	{
		return &theta_y_right_rad ; 
	}
	else if(variable_name.compare("theta_y_star_left_rad")==0)
	{
		return &theta_y_star_left_rad ; 
	}
	else if(variable_name.compare("theta_x_star_left_aligned_rad")==0)
	{
		return &theta_x_star_left_rad_aligned ; 
	}
	else if(variable_name.compare("theta_x_star_right_aligned_rad")==0)
	{
		return &theta_x_star_right_rad_aligned ; 
	}
	else if(variable_name.compare("theta_y_star_left_aligned_rad")==0)
	{
		return &theta_y_star_left_rad_aligned ; 
	}
	else if(variable_name.compare("theta_y_star_right_aligned_rad")==0)
	{
		return &theta_y_star_right_rad_aligned ; 
	}
	else if(variable_name.compare("theta_y_star_rad")==0)
	{
		return &theta_y_star_rad ; 
	}
	else if(variable_name.compare("theta_y_star_rad_aligned")==0)
	{
		return &theta_y_star_rad_aligned ; 
	}
	else if(variable_name.compare("theta_y_star_right_rad")==0)
	{
		return &theta_y_star_right_rad ; 
	}
	else if(variable_name.compare("theta_y_star_left_near_rad")==0)
	{
		return &theta_y_star_left_near_rad ; 
	}
	else if(variable_name.compare("theta_y_star_left_far_rad")==0)
	{
		return &theta_y_star_left_far_rad ; 
	}
	else if(variable_name.compare("theta_y_star_right_near_rad")==0)
	{
		return &theta_y_star_right_near_rad ; 
	}
	else if(variable_name.compare("theta_y_star_right_far_rad")==0)
	{
		return &theta_y_star_right_far_rad ; 
	}
	else if(variable_name.compare("theta_y_star_without_right_near_rad")==0)
	{
		return &theta_y_star_without_right_near_rad ; 
	}
	else if(variable_name.compare("theta_y_star_without_right_far_rad")==0)
	{
		return &theta_y_star_without_right_far_rad ; 
	}
	else if(variable_name.compare("theta_y_star_without_left_near_rad")==0)
	{
		return &theta_y_star_without_left_near_rad ; 
	}
	else if(variable_name.compare("theta_y_star_without_left_far_rad")==0)
	{
		return &theta_y_star_without_left_far_rad ; 
	}
	else if(variable_name.compare("t_left_GeV2")==0)
	{
		return &t_left_GeV2 ; 
	}
	else if(variable_name.compare("t_right_GeV2")==0)
	{
		return &t_right_GeV2 ; 
	}
	else if(variable_name.compare("x_star_left_mm")==0)
	{
		return &x_star_left_mm ; 
	}
	else if(variable_name.compare("x_star_right_mm")==0)
	{
		return &x_star_right_mm ; 
	}
	else if(variable_name.compare("x_star_mm")==0)
	{
		return &x_star_mm ; 
	}
	else if(variable_name.compare("y_star_left_mm")==0)
	{
		return &y_star_left_mm ; 
	}
	else if(variable_name.compare("y_star_right_mm")==0)
	{
		return &y_star_right_mm ; 
	}
	else if(variable_name.compare("y_star_mm")==0)
	{
		return &y_star_mm ; 
	}
	else if(variable_name.compare("y_l_n_mm")==0)
	{
		return &y_l_n_mm ; 
	}
	else if(variable_name.compare("x_l_n_mm")==0)
	{
		return &x_l_n_mm ; 
	}
	else if(variable_name.compare("y_l_f_mm")==0)
	{
		return &y_l_f_mm ; 
	}
	else if(variable_name.compare("x_l_f_mm")==0)
	{
		return &x_l_f_mm ; 
	}
	else if(variable_name.compare("y_r_n_mm")==0)
	{
		return &y_r_n_mm ; 
	}
	else if(variable_name.compare("x_r_n_mm")==0)
	{
		return &x_r_n_mm ; 
	}
	else if(variable_name.compare("y_r_f_mm")==0)
	{
		return &y_r_f_mm ; 
	}
	else if(variable_name.compare("x_r_f_mm")==0)
	{
		return &x_r_f_mm ; 
	}
	else if(variable_name.compare("thy_l_n_rad")==0)
	{
		return &thy_l_n_rad ; 
	}
	else if(variable_name.compare("thx_l_n_rad")==0)
	{
		return &thx_l_n_rad ; 
	}
	else if(variable_name.compare("thy_l_f_rad")==0)
	{
		return &thy_l_f_rad ; 
	}
	else if(variable_name.compare("thx_l_f_rad")==0)
	{
		return &thx_l_f_rad ; 
	}
	else if(variable_name.compare("thy_r_n_rad")==0)
	{
		return &thy_r_n_rad ; 
	}
	else if(variable_name.compare("thx_r_n_rad")==0)
	{
		return &thx_r_n_rad ; 
	}
	else if(variable_name.compare("thy_r_f_rad")==0)
	{
		return &thy_r_f_rad ; 
	}
	else if(variable_name.compare("thx_r_f_rad")==0)
	{
		return &thx_r_f_rad ; 
	}
	else if(variable_name.compare("minus_t_left_GeV2")==0)
	{
		return &minus_t_left_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_right_GeV2")==0)
	{
		return &minus_t_right_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_GeV2")==0)
	{
		return &minus_t_y_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_aligned_GeV2")==0)
	{
		return &minus_t_y_aligned_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_from_theta_y_GeV2")==0)
	{
		return &minus_t_from_theta_y_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_without_right_near_GeV2")==0)
	{
		return &minus_t_without_right_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_without_right_far_GeV2")==0)
	{
		return &minus_t_without_right_far_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_without_left_near_GeV2")==0)
	{
		return &minus_t_without_left_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_without_left_far_GeV2")==0)
	{
		return &minus_t_without_left_far_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_without_right_near_GeV2")==0)
	{
		return &minus_t_y_without_right_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_without_right_far_GeV2")==0)
	{
		return &minus_t_y_without_right_far_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_without_left_near_GeV2")==0)
	{
		return &minus_t_y_without_left_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_y_without_left_far_GeV2")==0)
	{
		return &minus_t_y_without_left_far_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_x_without_right_near_GeV2")==0)
	{
		return &minus_t_x_without_right_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_x_without_right_far_GeV2")==0)
	{
		return &minus_t_x_without_right_far_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_x_without_left_near_GeV2")==0)
	{
		return &minus_t_x_without_left_near_GeV2 ; 
	}
	else if(variable_name.compare("minus_t_x_without_left_far_GeV2")==0)
	{
		return &minus_t_x_without_left_far_GeV2 ; 
	}
	else if(variable_name.compare("y_l_n_aligned_mm")==0)
	{
		return &y_l_n_aligned_mm ; 
	}
	else if(variable_name.compare("x_l_n_aligned_mm")==0)
	{
		return &x_l_n_aligned_mm ; 
	}
	else if(variable_name.compare("y_l_f_aligned_mm")==0)
	{
		return &y_l_f_aligned_mm ; 
	}
	else if(variable_name.compare("x_l_f_aligned_mm")==0)
	{
		return &x_l_f_aligned_mm ; 
	}
	else if(variable_name.compare("y_r_n_aligned_mm")==0)
	{
		return &y_r_n_aligned_mm ; 
	}
	else if(variable_name.compare("x_r_n_aligned_mm")==0)
	{
		return &x_r_n_aligned_mm ; 
	}
	else if(variable_name.compare("y_r_f_aligned_mm")==0)
	{
		return &y_r_f_aligned_mm ; 
	}
	else if(variable_name.compare("x_r_f_aligned_mm")==0)
	{
		return &x_r_f_aligned_mm ; 
	}
	else if(variable_name.compare("minus_t_aligned_GeV2")==0)
	{
		return &minus_t_aligned_GeV2 ; 
	}
	else
	{
                cout << "The requested variable is not defined ! " << variable_name << endl ;
                exit(1) ;
	}

}

const bool *TProtonReconstruction::GetElasticPointer()
{
		return &elastic ; 
}

const UInt_t *TProtonReconstruction::GetTriggerDataRunNum()
{
		return &trigger_data_run_num ; 
}

const UInt_t *TProtonReconstruction::GetTriggerDataEventNum()
{
		return &trigger_data_event_num ; 
}

double TProtonReconstruction::ReconstructThetaYStarRad(double y_n_mm, double y_f_mm, TBeamOptics *BeamOptics)
{
	return (0.5*(	(y_n_mm / BeamOptics->Get_transport_matrix_near_vertical().Ly_mm()) +
			(y_f_mm / BeamOptics->Get_transport_matrix_far_vertical().Ly_mm()))) ;
}

double TProtonReconstruction::ReconstructXStarMm(double x_n_mm, double x_f_mm, TBeamOptics *BeamOptics)
{
	return ((1/BeamOptics->GetDeterminant_x_mm()) * ((BeamOptics->Get_transport_matrix_far_vertical().Lx_mm() * (x_n_mm)) - (BeamOptics->Get_transport_matrix_near_vertical().Lx_mm() *	(x_f_mm)))) ;
}

double TProtonReconstruction::ReconstructYStarMm(double y_n_mm, double y_f_mm, TBeamOptics *BeamOptics)
{
	return ((1/BeamOptics->GetDeterminant_y_mm()) * ((BeamOptics->Get_transport_matrix_far_vertical().Ly_mm() * (y_n_mm)) - (BeamOptics->Get_transport_matrix_near_vertical().Ly_mm() *	(y_f_mm)))) ;
}

double TProtonReconstruction::ReconstructThetaXStarRad(double theta_x_rad, double x_star_m, TBeamOptics *BeamOptics)
{
	return ((1/(BeamOptics->Get_transport_matrix_near_vertical().dLxds())) * (theta_x_rad  - (BeamOptics->Get_transport_matrix_near_vertical().dvxds() * x_star_m))) ;
}

double TProtonReconstruction::ReconstructFourMomentumTransferSquaredGeV2(double theta_x_star_rad, double theta_y_star_rad, TBeamOptics *BeamOptics)
{
	return ((-1.0)*((BeamOptics->GetBeamEnergyGeV() * BeamOptics->GetBeamEnergyGeV()) * ((theta_x_star_rad * theta_x_star_rad) + (theta_y_star_rad * theta_y_star_rad)))) ;
}

double TProtonReconstruction::ReconstructThetaXStarRadFromPosition(double x_mm, double x_star_mm, double Lx, TBeamOptics *BeamOptics)
{
	return ((1.0/Lx) * ((x_mm / TConstants::conversion_factor_from_m_to_mm)  - (BeamOptics->Get_transport_matrix_near_vertical().vx() * (x_star_mm / TConstants::conversion_factor_from_m_to_mm)))) ;
}

void TProtonReconstruction::Reconstruct(ULong64_t a_event_info_timestamp,  UInt_t a_trigger_data_run_num, UInt_t a_trigger_data_event_num,
	double a_x_l_n_mm, double a_x_l_f_mm, double a_x_r_n_mm, double a_x_r_f_mm, double a_y_l_n_mm, double a_y_l_f_mm, double a_y_r_n_mm, double a_y_r_f_mm,
	double a_thx_l_n_rad, double a_thx_l_f_rad, double a_thx_r_n_rad, double a_thx_r_f_rad, double a_thy_l_n_rad, double a_thy_l_f_rad, double a_thy_r_n_rad, double a_thy_r_f_rad,
	TBeamOptics *BeamOptics_Beam_1, TBeamOptics *BeamOptics_Beam_2, TRPAlignment *RPAlignment)
{
	valid = false ;
	elastic = false ;
	
	event_info_timestamp = a_event_info_timestamp ;
	trigger_data_run_num = a_trigger_data_run_num ;
	trigger_data_event_num = a_trigger_data_event_num ;

	x_l_n_mm = a_x_l_n_mm ;
	x_l_f_mm = a_x_l_f_mm ;
	x_r_n_mm = a_x_r_n_mm ;
	x_r_f_mm = a_x_r_f_mm ;

	y_l_n_mm = a_y_l_n_mm ;
	y_l_f_mm = a_y_l_f_mm ;
	y_r_n_mm = a_y_r_n_mm ;
	y_r_f_mm = a_y_r_f_mm ;

	x_l_n_aligned_mm = a_x_l_n_mm ;
	x_l_f_aligned_mm = a_x_l_f_mm ;
	x_r_n_aligned_mm = a_x_r_n_mm ;
	x_r_f_aligned_mm = a_x_r_f_mm ;

	y_l_n_aligned_mm = a_y_l_n_mm ;
	y_l_f_aligned_mm = a_y_l_f_mm ;
	y_r_n_aligned_mm = a_y_r_n_mm ;
	y_r_f_aligned_mm = a_y_r_f_mm ;

	thx_l_n_rad = a_thx_l_n_rad ;
	thx_l_f_rad = a_thx_l_f_rad ;
	thx_r_n_rad = a_thx_r_n_rad ;
	thx_r_f_rad = a_thx_r_f_rad ;

	thy_l_n_rad = a_thy_l_n_rad ;
	thy_l_f_rad = a_thy_l_f_rad ;
	thy_r_n_rad = a_thy_r_n_rad ;
	thy_r_f_rad = a_thy_r_f_rad ;

	if(RPAlignment != NULL)
	{
	  if(align_source)
		{
			x_l_n_mm += RPAlignment->Get_RP_alignment_left_near_x_mm() ;
			x_l_f_mm += RPAlignment->Get_RP_alignment_left_far__x_mm() ;

			x_r_n_mm += RPAlignment->Get_RP_alignment_right_near_x_mm() ;
			x_r_f_mm += RPAlignment->Get_RP_alignment_right_far__x_mm() ;

			y_l_n_mm += RPAlignment->Get_RP_alignment_left_near_y_mm() ;
			y_l_f_mm += RPAlignment->Get_RP_alignment_left_far__y_mm() ;

			y_r_n_mm += RPAlignment->Get_RP_alignment_right_near_y_mm() ;
			y_r_f_mm += RPAlignment->Get_RP_alignment_right_far__y_mm() ;
		}
	
		x_l_n_aligned_mm += RPAlignment->Get_RP_alignment_left_near_x_mm() ;
		x_l_f_aligned_mm += RPAlignment->Get_RP_alignment_left_far__x_mm() ;

		x_r_n_aligned_mm += RPAlignment->Get_RP_alignment_right_near_x_mm() ;
		x_r_f_aligned_mm += RPAlignment->Get_RP_alignment_right_far__x_mm() ;

		y_l_n_aligned_mm += RPAlignment->Get_RP_alignment_left_near_y_mm() ;
		y_l_f_aligned_mm += RPAlignment->Get_RP_alignment_left_far__y_mm() ;

		y_r_n_aligned_mm += RPAlignment->Get_RP_alignment_right_near_y_mm() ;
		y_r_f_aligned_mm += RPAlignment->Get_RP_alignment_right_far__y_mm() ;

		double dx_left_mm_aligned 	= (x_l_f_aligned_mm - x_l_n_aligned_mm) ;
		double dx_right_mm_aligned 	= (x_r_f_aligned_mm - x_r_n_aligned_mm) ;

		double theta_x_left_rad_aligned	= (dx_left_mm_aligned 	/ near_far_RP_units_distance_mm_Beam_2) ;
		double theta_x_right_rad_aligned 	= (dx_right_mm_aligned	/ near_far_RP_units_distance_mm_Beam_1) ;

		double x_star_left_mm_aligned  = ReconstructXStarMm(x_l_n_aligned_mm, x_l_f_aligned_mm, BeamOptics_Beam_2) ;
		double x_star_right_mm_aligned = ReconstructXStarMm(x_r_n_aligned_mm, x_r_f_aligned_mm, BeamOptics_Beam_1) ;

		double x_star_left_m_aligned  = (x_star_left_mm_aligned  / TConstants::conversion_factor_from_m_to_mm) ; 
		double x_star_right_m_aligned = (x_star_right_mm_aligned / TConstants::conversion_factor_from_m_to_mm) ;

		theta_x_star_left_rad_aligned	= ReconstructThetaXStarRad(theta_x_left_rad_aligned,  x_star_left_m_aligned, BeamOptics_Beam_2) ;
		theta_x_star_right_rad_aligned	= ReconstructThetaXStarRad(theta_x_right_rad_aligned, x_star_right_m_aligned, BeamOptics_Beam_1) ;

		theta_y_star_left_rad_aligned	= ReconstructThetaYStarRad(y_l_n_aligned_mm, y_l_f_aligned_mm, BeamOptics_Beam_2) ;
		theta_y_star_right_rad_aligned	= ReconstructThetaYStarRad(y_r_n_aligned_mm, y_r_f_aligned_mm, BeamOptics_Beam_1) ;

		theta_x_star_rad_aligned = 0.5 * (theta_x_star_left_rad_aligned + ((-1.0)*theta_x_star_right_rad_aligned)) ;
		theta_y_star_rad_aligned = 0.5 * (theta_y_star_left_rad_aligned + ((-1.0)*theta_y_star_right_rad_aligned)) ;

		double t_aligned_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_rad_aligned, theta_y_star_rad_aligned, BeamOptics_Beam_1) ;
  	double t_y_aligned_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_rad_aligned, BeamOptics_Beam_1) ;

		minus_t_aligned_GeV2		= -t_aligned_GeV2 ;
  	minus_t_y_aligned_GeV2	= -t_y_aligned_GeV2 ;
		
	}
	
	dx_left_mm 	= (x_l_f_mm - x_l_n_mm) ;
	dy_left_mm 	= (y_l_f_mm - y_l_n_mm) ;
	dx_right_mm 	= (x_r_f_mm - x_r_n_mm) ;
	dy_right_mm 	= (y_r_f_mm - y_r_n_mm) ;

	theta_x_left_rad	= (dx_left_mm 	/ near_far_RP_units_distance_mm_Beam_2) ;
	theta_y_left_rad 	= (dy_left_mm	/ near_far_RP_units_distance_mm_Beam_2) ;
	theta_x_right_rad 	= (dx_right_mm	/ near_far_RP_units_distance_mm_Beam_1) ;
	theta_y_right_rad 	= (dy_right_mm	/ near_far_RP_units_distance_mm_Beam_1) ;

	theta_y_star_left_rad	= ReconstructThetaYStarRad(y_l_n_mm, y_l_f_mm, BeamOptics_Beam_2) ;
	theta_y_star_right_rad	= ReconstructThetaYStarRad(y_r_n_mm, y_r_f_mm, BeamOptics_Beam_1) ;

	// For inefficiency calculations theta_y_star is calculated per device

	theta_y_star_left_near_rad =  (y_l_n_mm / BeamOptics_Beam_2->Get_transport_matrix_near_vertical().Ly_mm()) ;
	theta_y_star_left_far_rad  =  (y_l_f_mm / BeamOptics_Beam_2->Get_transport_matrix_far_vertical().Ly_mm()) ;

	theta_y_star_right_near_rad = (y_r_n_mm / BeamOptics_Beam_1->Get_transport_matrix_near_vertical().Ly_mm()) ;
	theta_y_star_right_far_rad  = (y_r_f_mm / BeamOptics_Beam_1->Get_transport_matrix_far_vertical().Ly_mm()) ;

	x_star_left_mm  = ReconstructXStarMm(x_l_n_mm, x_l_f_mm, BeamOptics_Beam_2) ;
	x_star_right_mm = ReconstructXStarMm(x_r_n_mm, x_r_f_mm, BeamOptics_Beam_1) ;

	y_star_left_mm  = ReconstructYStarMm(y_l_n_mm, y_l_f_mm, BeamOptics_Beam_2) ;
	y_star_right_mm = ReconstructYStarMm(y_r_n_mm, y_r_f_mm, BeamOptics_Beam_1) ;

	y_star_mm = (y_star_left_mm + y_star_right_mm) / 2.0 ;

	double x_star_left_m  = (x_star_left_mm  / TConstants::conversion_factor_from_m_to_mm) ; 
	double x_star_right_m = (x_star_right_mm / TConstants::conversion_factor_from_m_to_mm) ;
	
	x_star_mm = (x_star_left_mm + x_star_right_mm) / 2.0 ;

	theta_x_star_left_rad	= ReconstructThetaXStarRad(theta_x_left_rad,  x_star_left_m, BeamOptics_Beam_2) ;
	theta_x_star_right_rad	= ReconstructThetaXStarRad(theta_x_right_rad, x_star_right_m, BeamOptics_Beam_1) ;

	if(use_vertex_for_theta_x_star)
	{
		theta_x_star_right_far_rad =  	ReconstructThetaXStarRadFromPosition(x_r_f_mm, x_star_left_mm,  (BeamOptics_Beam_1->Get_transport_matrix_far_vertical().Lx_m()),  BeamOptics_Beam_1) ;
		theta_x_star_right_near_rad = 	ReconstructThetaXStarRadFromPosition(x_r_n_mm, x_star_left_mm,  (BeamOptics_Beam_1->Get_transport_matrix_near_vertical().Lx_m()), BeamOptics_Beam_1) ;
		theta_x_star_left_near_rad = 	ReconstructThetaXStarRadFromPosition(x_l_n_mm, x_star_right_mm, (BeamOptics_Beam_2->Get_transport_matrix_near_vertical().Lx_m()), BeamOptics_Beam_2) ;
		theta_x_star_left_far_rad =	ReconstructThetaXStarRadFromPosition(x_l_f_mm, x_star_right_mm, (BeamOptics_Beam_2->Get_transport_matrix_far_vertical().Lx_m()),  BeamOptics_Beam_2) ;
	}
	else
	{
                theta_x_star_right_far_rad =    ReconstructThetaXStarRadFromPosition(x_r_f_mm, 0.0,  (BeamOptics_Beam_1->Get_transport_matrix_far_vertical().Lx_m()),  BeamOptics_Beam_1) ;
                theta_x_star_right_near_rad =   ReconstructThetaXStarRadFromPosition(x_r_n_mm, 0.0,  (BeamOptics_Beam_1->Get_transport_matrix_near_vertical().Lx_m()), BeamOptics_Beam_1) ;
                theta_x_star_left_near_rad =    ReconstructThetaXStarRadFromPosition(x_l_n_mm, 0.0,  (BeamOptics_Beam_2->Get_transport_matrix_near_vertical().Lx_m()), BeamOptics_Beam_2) ;
                theta_x_star_left_far_rad =     ReconstructThetaXStarRadFromPosition(x_l_f_mm, 0.0,  (BeamOptics_Beam_2->Get_transport_matrix_far_vertical().Lx_m()),  BeamOptics_Beam_2) ;
	}

	// Average of arms

	theta_x_star_rad = 0.5 * (theta_x_star_left_rad + ((-1.0)*theta_x_star_right_rad)) ;
	theta_y_star_rad = 0.5 * (theta_y_star_left_rad + ((-1.0)*theta_y_star_right_rad)) ;

	theta_x_star_without_right_near_rad = 0.5 * (theta_x_star_left_rad 	+ ((-1.0)*theta_x_star_right_far_rad)) ;
	theta_x_star_without_right_far_rad  = 0.5 * (theta_x_star_left_rad 	+ ((-1.0)*theta_x_star_right_near_rad)) ;
	theta_x_star_without_left_near_rad  = 0.5 * (theta_x_star_left_far_rad 	+ ((-1.0)*theta_x_star_right_rad)) ;
	theta_x_star_without_left_far_rad   = 0.5 * (theta_x_star_left_near_rad + ((-1.0)*theta_x_star_right_rad)) ;

        theta_x_star_far_rad = 0.5 * (theta_x_star_left_far_rad + ((-1.0)*theta_x_star_right_far_rad)) ;
        theta_y_star_far_rad = 0.5 * (theta_y_star_left_far_rad + ((-1.0)*theta_y_star_right_far_rad)) ;

	// For 3 out 4 inefficency calculations

	theta_y_star_without_right_near_rad = 0.5 * (theta_y_star_left_rad 	+ ((-1.0)*theta_y_star_right_far_rad)) ;
	theta_y_star_without_right_far_rad  = 0.5 * (theta_y_star_left_rad 	+ ((-1.0)*theta_y_star_right_near_rad)) ;
	theta_y_star_without_left_near_rad  = 0.5 * (theta_y_star_left_far_rad 	+ ((-1.0)*theta_y_star_right_rad)) ;
	theta_y_star_without_left_far_rad   = 0.5 * (theta_y_star_left_near_rad + ((-1.0)*theta_y_star_right_rad)) ;

	theta_star_rad = sqrt((theta_x_star_rad * theta_x_star_rad) + (theta_y_star_rad * theta_y_star_rad)) ;
	
	// Azimuthal angle

	phi_star_rad = atan(fabs(theta_y_star_rad / theta_x_star_rad)) ;

	if((theta_y_star_rad > 0) && (theta_x_star_rad < 0)) phi_star_rad = TMath::Pi() - phi_star_rad ;
	else if((theta_y_star_rad < 0) && (theta_x_star_rad < 0)) phi_star_rad = TMath::Pi() + phi_star_rad ;
	else if((theta_y_star_rad < 0) && (theta_x_star_rad > 0)) phi_star_rad = (2*TMath::Pi()) - phi_star_rad ;

	t_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_rad, theta_y_star_rad, BeamOptics_Beam_1) ;
	t_y_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_rad, BeamOptics_Beam_1) ;

	minus_t_y_without_right_near_GeV2  = -ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_without_right_near_rad, BeamOptics_Beam_2) ;
	minus_t_y_without_right_far_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_without_right_far_rad, BeamOptics_Beam_2) ;
	minus_t_y_without_left_near_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_without_left_near_rad, BeamOptics_Beam_2) ;
	minus_t_y_without_left_far_GeV2    = -ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_without_left_far_rad, BeamOptics_Beam_2) ;

	minus_t_x_without_right_near_GeV2  = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_rad, 0, BeamOptics_Beam_2) ;
	minus_t_x_without_right_far_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_rad, 0, BeamOptics_Beam_2) ;
	minus_t_x_without_left_near_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_rad, 0, BeamOptics_Beam_2) ;
	minus_t_x_without_left_far_GeV2    = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_rad, 0, BeamOptics_Beam_2) ;

	t_left_GeV2  = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_rad, theta_y_star_left_rad, BeamOptics_Beam_2) ;
	t_right_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_rad, theta_y_star_right_rad, BeamOptics_Beam_1) ; 

        t_left_far_GeV2  = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_far_rad, theta_y_star_left_far_rad, BeamOptics_Beam_2) ;
        t_right_far_GeV2 = ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_far_rad, theta_y_star_right_far_rad, BeamOptics_Beam_1) ;

	minus_t_without_right_near_GeV2  = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_rad, theta_y_star_without_right_near_rad, BeamOptics_Beam_2) ;
	minus_t_without_right_far_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_left_rad, theta_y_star_without_right_far_rad, BeamOptics_Beam_2) ;
	minus_t_without_left_near_GeV2   = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_rad, theta_y_star_without_left_near_rad, BeamOptics_Beam_2) ;
	minus_t_without_left_far_GeV2    = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_right_rad, theta_y_star_without_left_far_rad, BeamOptics_Beam_2) ;

	minus_t_GeV2		= -t_GeV2 ;
	minus_t_left_GeV2	= -t_left_GeV2 ;
	minus_t_right_GeV2	= -t_right_GeV2 ;
	minus_t_y_GeV2		= -t_y_GeV2 ;

	double theta_y_star_left_from_theta_y_rad = theta_y_left_rad / BeamOptics_Beam_2->Get_transport_matrix_far_vertical().dLyds() ;
	double theta_y_star_right_from_theta_y_rad = theta_y_right_rad / BeamOptics_Beam_1->Get_transport_matrix_far_vertical().dLyds() ;

	double theta_y_star_from_theta_y_rad = 0.5*(theta_y_star_left_from_theta_y_rad + ((-1.0)*theta_y_star_right_from_theta_y_rad)) ;

	minus_t_from_theta_y_GeV2 = -ReconstructFourMomentumTransferSquaredGeV2(theta_x_star_rad, theta_y_star_from_theta_y_rad, BeamOptics_Beam_1) ;

	// Additional variables for 3/4 topologies

	// no right far

	phi_star_without_right_far_rad = atan(fabs(theta_y_star_without_right_far_rad / theta_x_star_left_rad)) ;

	if((theta_y_star_without_right_far_rad > 0) && (theta_x_star_left_rad < 0)) phi_star_without_right_far_rad = TMath::Pi() - phi_star_without_right_far_rad ;
	else if((theta_y_star_without_right_far_rad < 0) && (theta_x_star_left_rad < 0)) phi_star_without_right_far_rad = TMath::Pi() + phi_star_without_right_far_rad ;
	else if((theta_y_star_without_right_far_rad < 0) && (theta_x_star_left_rad > 0)) phi_star_without_right_far_rad = (2*TMath::Pi()) - phi_star_without_right_far_rad ;

	theta_star_without_right_far_rad = sqrt((theta_x_star_left_rad * theta_x_star_left_rad) + (theta_y_star_without_right_far_rad * theta_y_star_without_right_far_rad)) ;

	// no left far

	phi_star_without_left_far_rad = atan(fabs(theta_y_star_without_left_far_rad / theta_x_star_right_rad)) ;

	if((theta_y_star_without_left_far_rad > 0) && (theta_x_star_right_rad < 0)) phi_star_without_left_far_rad = TMath::Pi() - phi_star_without_left_far_rad ;
	else if((theta_y_star_without_left_far_rad < 0) && (theta_x_star_right_rad < 0)) phi_star_without_left_far_rad = TMath::Pi() + phi_star_without_left_far_rad ;
	else if((theta_y_star_without_left_far_rad < 0) && (theta_x_star_right_rad > 0)) phi_star_without_left_far_rad = (2*TMath::Pi()) - phi_star_without_left_far_rad ;

	theta_star_without_left_far_rad = sqrt((theta_x_star_right_rad * theta_x_star_right_rad) + (theta_y_star_without_left_far_rad * theta_y_star_without_left_far_rad)) ;

	valid = true ;
}


#endif // #ifndef TProtonReconstruction_h
