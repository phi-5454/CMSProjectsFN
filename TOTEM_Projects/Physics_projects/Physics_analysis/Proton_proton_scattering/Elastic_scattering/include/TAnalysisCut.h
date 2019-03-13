#ifndef TAnalysisCut_h
#define TAnalysisCut_h

class TAnalysisCut : public TAnalysisObject
{
	protected:

	bool valid ;
	double sigma, number_of_sigmas, axis_boundary ;

	TProjectParameters *ProjectParameters ;
	
	public:
	
	TAnalysisCut(string name, string id, TProjectParameters *ProjectParameters, string cut_block_name) : TAnalysisObject(name, id), ProjectParameters(ProjectParameters)
	{
		TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;
	
		sigma = cut_block->GetParameterValue("sigma") ;
		number_of_sigmas = cut_block->GetParameterValue("number_of_sigmas") ;

		if((sigma < 0) || (number_of_sigmas < 0) || (number_of_sigmas > 6))
		{
			cout << "Error: TAnalysisCut::TAnalysisCut: the cut is not defined correctly !" << endl ;
			exit(1) ;
		}
	}

	void SetValid()		{ valid = true ; } ;
	void ClearValid()	{ valid = false ; } ;

	double GetSigma()	{ return sigma ; } ;

	bool IsValid()	{ return valid ; } ;
	double DistanceFromCut(double , double ) ;
	virtual bool IsTrue(double, double) { return false ; } ;

	virtual void DrawCut() ;
	virtual void DrawCutOnRotatedPlot(double, double) {} ;

	virtual void Save() {} ;
} ;

double TAnalysisCut::DistanceFromCut(double x, double y)
{
	return 0 ;
}

void TAnalysisCut::DrawCut()
{
}

#endif // #ifndef TAnalysisCut_h
