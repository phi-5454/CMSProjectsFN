#ifndef TAnalysisCut1D_h
#define TAnalysisCut1D_h

#include "math.h"
#include "TAnalysisCut.h"

class TAnalysisCut1D : public TAnalysisCut
{

	double cut_position ;
	
	public:
	
	TAnalysisCut1D(string name, string id, TProjectParameters *ProjectParameters, string cut_block_name) : TAnalysisCut(name, id, ProjectParameters, cut_block_name)
	{
		valid = false ;
	}
	
	virtual bool IsTrue(double, double) ;
} ;

bool TAnalysisCut1D::IsTrue(double x, double y)
{
	return ((fabs(x-cut_position) / sigma) < number_of_sigmas) ;
}

#endif // #ifndef TAnalysisCut1D_h
