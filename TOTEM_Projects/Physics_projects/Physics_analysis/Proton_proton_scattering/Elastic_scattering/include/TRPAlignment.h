#ifndef TRPAlignment_h
#define TRPAlignment_h

#include "TAnalysisObject.h"

class TRPAlignment : public TAnalysisObject
{
	private:

	double RP_alignment_left_far__x_mm ;
	double RP_alignment_left_near_x_mm ;

	double RP_alignment_right_far__x_mm ;
	double RP_alignment_right_near_x_mm ;

	double RP_alignment_left_far__y_mm ;
	double RP_alignment_left_near_y_mm ;

	double RP_alignment_right_far__y_mm ;
	double RP_alignment_right_near_y_mm ;


	public:

	TRPAlignment(string, string, double=0.0, double=0.0, double=0.0, double=0.0, double=0.0, double=0.0, double=0.0, double=0.0) ;

	double Get_RP_alignment_left_far__x_mm() 	{ return RP_alignment_left_far__x_mm ; } ;
	double Get_RP_alignment_left_near_x_mm() 	{ return RP_alignment_left_near_x_mm ; } ;
	double Get_RP_alignment_right_far__x_mm() 	{ return RP_alignment_right_far__x_mm ; } ;
	double Get_RP_alignment_right_near_x_mm() 	{ return RP_alignment_right_near_x_mm ; } ;

	double Get_RP_alignment_left_far__y_mm() 	{ return RP_alignment_left_far__y_mm ; } ;
	double Get_RP_alignment_left_near_y_mm() 	{ return RP_alignment_left_near_y_mm ; } ;
	double Get_RP_alignment_right_far__y_mm() 	{ return RP_alignment_right_far__y_mm ; } ;
	double Get_RP_alignment_right_near_y_mm() 	{ return RP_alignment_right_near_y_mm ; } ;

} ;

TRPAlignment::TRPAlignment(string name, string id,
	double RP_alignment_left_far__x_mm, double RP_alignment_left_near_x_mm, double RP_alignment_right_far__x_mm, double RP_alignment_right_near_x_mm,
	double RP_alignment_left_far__y_mm, double RP_alignment_left_near_y_mm, double RP_alignment_right_far__y_mm, double RP_alignment_right_near_y_mm): TAnalysisObject(name, id),
	RP_alignment_left_far__x_mm(RP_alignment_left_far__x_mm), RP_alignment_left_near_x_mm(RP_alignment_left_near_x_mm), RP_alignment_right_far__x_mm(RP_alignment_right_far__x_mm), RP_alignment_right_near_x_mm(RP_alignment_right_near_x_mm),
	RP_alignment_left_far__y_mm(RP_alignment_left_far__y_mm), RP_alignment_left_near_y_mm(RP_alignment_left_near_y_mm), RP_alignment_right_far__y_mm(RP_alignment_right_far__y_mm), RP_alignment_right_near_y_mm(RP_alignment_right_near_y_mm)
{

}

#endif // #ifndef TRPAlignment_h
