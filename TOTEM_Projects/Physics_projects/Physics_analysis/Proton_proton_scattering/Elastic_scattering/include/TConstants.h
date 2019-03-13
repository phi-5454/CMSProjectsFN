#ifndef TConstants_h
#define TConstants_h

#include "TAnalysisObject.h"
#include "TColor.h"

class TConstants
{
	public:

	static const int total_number_of_branches ;
	static const int total_number_of_validity_branches ;

	static const double conversion_factor_from_m_to_mm ;
	static const double conversion_factor_from_mbarn_to_ubarn ;
	static const double conversion_factor_from_fraction_to_percent ;
	static const ULong64_t MAX_timestamp ;

	static const int beam_1_number ;
	static const int beam_2_number ;

	static const int NO_ORIENTATION ;
	static const int TOP ;
	static const int BOTTOM ;
	static const int HORIZONTAL ;
	static const int LEFT ;
	static const int RIGHT ;
	static const int NEAR ;
	static const int FAR ;

	static const int NTUPLE_BLOCK_INFORMATION ;
	static const int NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP ;
	static const int NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM ;
	static const int NTUPLE_BLOCK_HORIZONTALS ;

	static const bool NO_BEGIN_END ;
	static const bool NEW_LINE ;
	static const double WIDTH_HALF_PAGE ;
	static const double WIDTH_60_PERCENT ;

	static const string AT_LEAST_1_VALID_RP_TRACK ;
	static const string AT_LEAST_3_VALID_RP_TRACK ;

	static const string SETTING_INSTALLDIRECTORYOFCODE ;

	static const double alignment_x ;
	static const double alignment_y ;

	static const EColor color_plus_1sigma_line ;

	static const int filled_circle_marker_style ;	

	static const int scale_between_xi_and_its_index = 10000 ;

	static const int twiss_position_in_line_of_horizontal_beam_position_m = 3 ;

	static const int TriggerBlockShift = 264 ;
	static const int NegativeBunchNumberAdjusment = 3564 ;

	string ConvertOrientationCodeToString(int) ;
	
	static const int buffersize_for_property_string = 80 ;

	static const double hbarc = 0.19732697 ;				 // [hbarc] = GeV * fm

	static double Get_hbarc2_GeV_2_mb() { return (10.0 * hbarc * hbarc) ; } ; 

} ;

const int TConstants::total_number_of_branches = 6 ;
const int TConstants::total_number_of_validity_branches = 4 ;

const double TConstants::conversion_factor_from_m_to_mm 		= 1000.0 ;
const double TConstants::conversion_factor_from_mbarn_to_ubarn 		= 1000.0 ;
const double TConstants::conversion_factor_from_fraction_to_percent 	= 100.0 ;

const int TConstants::NO_ORIENTATION	= 0 ;
const int TConstants::TOP 		= 1 ;
const int TConstants::BOTTOM 		= 2 ;
const int TConstants::HORIZONTAL	= 3 ;
const int TConstants::LEFT 		= 4 ;
const int TConstants::RIGHT		= 5 ;
const int TConstants::NEAR 		= 6 ;
const int TConstants::FAR 		= 7 ;

const int TConstants::NTUPLE_BLOCK_INFORMATION		 		= 1 ;
const int TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP 	= 2 ;
const int TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM 	= 3 ;
const int TConstants::NTUPLE_BLOCK_HORIZONTALS		     		= 4 ;

const bool TConstants::NO_BEGIN_END = false;
const bool TConstants::NEW_LINE = true ;
const double TConstants::WIDTH_HALF_PAGE  = 0.49 ;
const double TConstants::WIDTH_60_PERCENT  = 0.60 ;

const int TConstants::beam_1_number = 1 ;
const int TConstants::beam_2_number = 2 ;

const string TConstants::AT_LEAST_1_VALID_RP_TRACK = "AT_LEAST_1_VALID_RP_TRACK" ;
const string TConstants::AT_LEAST_3_VALID_RP_TRACK = "AT_LEAST_3_VALID_RP_TRACK" ;

const string TConstants::SETTING_INSTALLDIRECTORYOFCODE = "InstallDirectoryOfCode" ;

const double TConstants::alignment_x = 1.0 ;
const double TConstants::alignment_y = 2.0 ;

const EColor TConstants::color_plus_1sigma_line = kBlue ;

const int TConstants::filled_circle_marker_style = 20 ;	

string TConstants::ConvertOrientationCodeToString(int code)
{
	string code_name = "" ;

	if(code == NO_ORIENTATION) code_name = "NO_ORIENTATION" ;
	else if(code == TOP) code_name = "TOP" ;
	else if(code == BOTTOM) code_name = "BOTTOM" ;
	else if(code == HORIZONTAL) code_name = "HORIZONTAL" ;
	else if(code == LEFT) code_name = "LEFT" ;
	else if(code == RIGHT) code_name = "RIGHT" ;
	else if(code == NEAR) code_name = "NEAR" ;
	else if(code == FAR) code_name = "FAR" ;

	return code_name ;
}

#endif // #ifndef TConstants_h
