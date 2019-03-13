#ifndef TAnalysisComment_h
#define TAnalysisComment_h

#include "TAnalysisPlot.h"

#include "TProfile.h"
#include "TGraph.h"
#include "TLatex.h"

#include "TFitResult.h"

#include <iomanip>
 
class TAnalysisComment : public TAnalysisPlot
{
	private:
	
	string description ;

	public:

	TAnalysisComment(string , string , TProjectParameters *, TProtonReconstruction *) ;

} ;

TAnalysisComment::TAnalysisComment(string name, string plot_collection_name, TProjectParameters *ProjectParameters, TProtonReconstruction *ProtonReconstruction) :
	 TAnalysisPlot(name, plot_collection_name + "_" + name, ProjectParameters, ProtonReconstruction)
{
	AnalysisCut = NULL ;
	description = "" ;

	bool find_cuts = false ;
}

#endif // #ifndef TAnalysisComment_h
