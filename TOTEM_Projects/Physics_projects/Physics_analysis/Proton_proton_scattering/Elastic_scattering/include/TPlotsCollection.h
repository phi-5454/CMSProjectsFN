#ifndef TPlotsCollection_h
#define TPlotsCollection_h

#include "TStyle.h"
#include "TH2F.h"
#include "TCanvas.h"

#include "TAnalysisObject.h"
#include "TProtonReconstruction.h"

// #include "TAnalysisPlot1D_dNdt.h"

#include "TAnalysisPlot1D.h"
#include "TAnalysisPlot2D.h"
#include "TAnalysisComment.h"

#include <sstream>
#include <iomanip>

typedef TAnalysisPlot *PTAnalysisPlot_type ;
typedef map<string, PTAnalysisPlot_type> map_from_string_to_PTAnalysisPlot_type ;
typedef vector<PTAnalysisPlot_type> vector_of_PTAnalysisPlot_type ;

// #include "TAnalysisPlot1D_dNdtScaled.h"

class TPlotsCollection : public TAnalysisObject
{

	TProjectParameters *ProjectParameters ;
	string output_directory ;

	map_from_string_to_PTAnalysisPlot_type map_from_string_to_PTAnalysisPlot ;
	vector_of_PTAnalysisPlot_type vector_of_PTAnalysisPlot ;
	vector_of_PTAnalysisPlot_type vector_of_PTAnalysisPlot_with_comments ;

	public:

	TPlotsCollection(string, string, TProjectParameters *, TLuminosity *, TProtonReconstruction *) ;
	void PushBackNewPAnalysisPlot(PTAnalysisPlot_type, string) ;
	PTAnalysisPlot_type GetPAnalysisPlotByLabel(string) ;

	void Clear() ;
	void Reset() ;
	void Save() ;
	bool Fill(TProtonReconstruction *, double=1.0) ;
	bool Test(TProtonReconstruction *, double=1.0) ;
	bool AdjustCuts() ;
	void PrintCutCoordinates() ;
	
	string GetNextId() ;
} ;

typedef map<string, TPlotsCollection *> map_from_string_to_PTPlotsCollection_type ;

string TPlotsCollection::GetNextId()
{

	stringstream ss ;
	ss << std::setfill('0') << setw(4) << vector_of_PTAnalysisPlot.size() ;

	return ("P" + ss.str() + "_" + name) ;
}

PTAnalysisPlot_type TPlotsCollection::GetPAnalysisPlotByLabel(string label)
{
	map_from_string_to_PTAnalysisPlot_type::iterator it ; 

	it = map_from_string_to_PTAnalysisPlot.find(label) ;
	
	if(it == map_from_string_to_PTAnalysisPlot.end())
	{
		cout << "void TPlotsCollection::GetPAnalysisPlotByLabel: The label cannot be found:" << label << endl ;
		exit(1) ;
	}

	return map_from_string_to_PTAnalysisPlot[label] ;
}

void TPlotsCollection::PushBackNewPAnalysisPlot(PTAnalysisPlot_type PTAnalysisPlot, string label)
{
	map_from_string_to_PTAnalysisPlot_type::iterator it ; 
	
	it = map_from_string_to_PTAnalysisPlot.find(label) ;
	
	if(it != map_from_string_to_PTAnalysisPlot.end())
	{
		cout << "void TPlotsCollection::PushBackNewPAnalysisPlot: The label within the collection is not unique:" << label << endl ;
		exit(1) ;
	}
	
	if(PTAnalysisPlot->GetDimension() != 0) vector_of_PTAnalysisPlot.push_back(PTAnalysisPlot) ;
	map_from_string_to_PTAnalysisPlot[label] = PTAnalysisPlot ;

	vector_of_PTAnalysisPlot_with_comments.push_back(PTAnalysisPlot) ;
}

TPlotsCollection::TPlotsCollection(string name, string id, TProjectParameters *ProjectParameters, TLuminosity *luminosity, TProtonReconstruction *ProtonReconstruction) : TAnalysisObject(name, id), ProjectParameters(ProjectParameters)
{
	cout << "\033[1;31m Check this assignments \033[0m\n" ; 
	
	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	for(vector<string>::iterator it = ProjectParameters->GetSettingsVector("analysis_plot")->begin() ; it != ProjectParameters->GetSettingsVector("analysis_plot")->end() ; ++it)
	{
		string plotname = *it ;
		string plot_block_name = plotname + "_plot_block" ;

		cout << "plotname: " << plotname << endl ;

		if(ProjectParameters->IsBlockDefined(plot_block_name))
		{
			TProjectParameters *plot_block = ProjectParameters->GetBlock(plot_block_name) ;

			int dimension_of_plot = plot_block->GetParameterValue("dimension") ;

			string next_id = GetNextId() ;
			cout << "\t\t" << next_id << endl ;
			
			if(dimension_of_plot == 1.0)
			{
				TAnalysisPlot *plot = new TAnalysisPlot1D(plotname, next_id, ProjectParameters, ProtonReconstruction) ;
				PushBackNewPAnalysisPlot(plot, plot->GetId()) ;
			}
			else if(dimension_of_plot == 2.0)
			{
				TAnalysisPlot *plot = new TAnalysisPlot2D(plotname, next_id, ProjectParameters, ProtonReconstruction) ;
				PushBackNewPAnalysisPlot(plot, plot->GetId()) ;
			}
			else if(dimension_of_plot == 0.0)
			{
				TAnalysisPlot *plot =  new TAnalysisComment(plotname, next_id, ProjectParameters, ProtonReconstruction) ;
				PushBackNewPAnalysisPlot(plot, plot->GetId()) ;
			}
			else
			{
				cout << "TPlotsCollection::TPlotsCollection: unknown dimenson ! : " << dimension_of_plot << endl ;
				exit(1) ;
			}

		}
		else
		{
			cout << "TPlotsCollection::TPlotsCollection: the block is not defined ! : " << plot_block_name << endl ;
			exit(1) ;
		}
	}

}

bool TPlotsCollection::Fill(TProtonReconstruction *ProtonReconstruction, double weight)
{
	const bool test = false ;
	bool status = true ;

	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		(*it)->Fill(weight, test) ;
		
		if(!((*it)->GetFillStatus()))
		{
			status = false ;
			break ;
		}
	}

	return status ;
}

bool TPlotsCollection::Test(TProtonReconstruction *ProtonReconstruction, double weight)
{
	const bool test = true ;
	bool status = true ;

	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		(*it)->Fill(weight, test) ;
		
		if(!((*it)->GetFillStatus()))
		{
			status = false ;
			break ;
		}
	}

	return status ;
}

void TPlotsCollection::Save()
{
	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot_with_comments.begin() ; it != vector_of_PTAnalysisPlot_with_comments.end() ; ++it)
	{
		(*it)->Save() ;
	}

	ProjectParameters->GetLatexFile()->PrintTables() ;	

	(*ProjectParameters->GetPLatexFile())	<< "\\end{landscape}" << endl ;
	(*ProjectParameters->GetPLatexFile()) << "\\end{document}" << endl ;
}

void TPlotsCollection::Clear()
{
	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		(*it)->Delete() ;
		delete (*it) ;
	}

	vector_of_PTAnalysisPlot.clear() ;
}

void TPlotsCollection::Reset()
{
	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		(*it)->Reset() ;
	}
}

bool TPlotsCollection::AdjustCuts()
{
	bool status = false ;

	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		cout << "profile name " <<  (*it)->GetName() << endl ;
		bool actual_status = (*it)->AdjustCut() ;
		
		status = (status || actual_status) ;
	}

	return status ;
}

void TPlotsCollection::PrintCutCoordinates()
{
	for(vector_of_PTAnalysisPlot_type::iterator it = vector_of_PTAnalysisPlot.begin() ; it != vector_of_PTAnalysisPlot.end() ; ++it)
	{
		(*it)->PrintCutCoordinates() ;
	}
}


#endif // #ifndef TPlotsCollection_h
