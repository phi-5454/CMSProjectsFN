#ifndef TAnalysisPlot_h
#define TAnalysisPlot_h

#include "TAnalysisCut1D.h"
#include "TAnalysisCut2D.h"

#include "TProtonReconstruction.h"

class TAnalysisPlot : public TAnalysisObject
{
	protected:

	int dimension ;
	int number_of_needed_RPs ;

	TAnalysisCut *AnalysisCut ;
	TProjectParameters *ProjectParameters ;
	bool fill_status ;

	bool apply_cut ;
	bool define_cut ;
	bool draw_cut ;

	bool weighted ;
	bool geometrical_acceptance_corrected ;
	bool beam_divergence_corrected ;
	bool horizontal_beam_divergence_corrected ;
	bool pile_up_inefficiency_corrected ;
	bool to_be_saved_for_smart_merging ;
	bool variable_bin_size ;
	
	string variable_bin_size_description ;

	string text_before ;
	string text_after ;

	public:

	TAnalysisPlot(string, string,  TProjectParameters *, TProtonReconstruction *) ;
	virtual void Save() ;
	virtual void SavePlot() ;
	virtual void SaveLatex() {} ;

	virtual void Fill(double weight, bool test) { } ;
	bool GetFillStatus() { return fill_status ; } ;

	virtual void DrawExtras() {} ;
	virtual void Delete() {} ;
	virtual bool AdjustCut() { return 0 ; } ;
	
	virtual void Reset() {} ;
	void SaveEpsRootLatex(string , TCanvas *, bool, double, bool, bool) ;
	
	int GetDimension() { return dimension ; } ;
	virtual void PrintCutCoordinates() {} ;

        int GetNumNeededRPs() { return number_of_needed_RPs ; } ;

	void AlignmentWarning() ;
} ;

TAnalysisPlot::TAnalysisPlot(string name, string id, TProjectParameters *ProjectParameters, TProtonReconstruction *ProtonReconstruction) : TAnalysisObject(name, id), AnalysisCut(NULL), ProjectParameters(ProjectParameters),
		text_before(""), text_after(""), dimension(0), number_of_needed_RPs(4), fill_status(false), apply_cut(false), define_cut(false), draw_cut(false), weighted(false), geometrical_acceptance_corrected(false), beam_divergence_corrected(false), horizontal_beam_divergence_corrected(false),
		pile_up_inefficiency_corrected(false), to_be_saved_for_smart_merging(false), variable_bin_size(false), variable_bin_size_description("")
{
	string plot_block_name = name + "_plot_block" ;
	
	if(ProjectParameters->IsBlockDefined(plot_block_name))
	{
		TProjectParameters *plot_block = ProjectParameters->GetBlock(plot_block_name) ;

		if(plot_block->IsSettingDefined("text_before"))
		{
			text_before = plot_block->GetSettingValue("text_before") ;
		}

		if(plot_block->IsSettingDefined("text_after"))
		{
			text_after = plot_block->GetSettingValue("text_after") ;
		}

		if(plot_block->IsParameterDefined("weighted"))
		{
			if(plot_block->GetParameterValue("weighted") == 1.0)
			{
				weighted = true ;
			}
		}

		if(plot_block->IsParameterDefined("geometrical_acceptance_corrected"))
		{
			if(plot_block->GetParameterValue("geometrical_acceptance_corrected") == 1.0)
			{
				geometrical_acceptance_corrected = true ;
			}
		}

		if(plot_block->IsParameterDefined("beam_divergence_corrected"))
		{
			if(plot_block->GetParameterValue("beam_divergence_corrected") == 1.0)
			{
				beam_divergence_corrected = true ;
			}
		}

		if(plot_block->IsParameterDefined("horizontal_beam_divergence_corrected"))
		{
			if(plot_block->GetParameterValue("horizontal_beam_divergence_corrected") == 1.0)
			{
				horizontal_beam_divergence_corrected = true ;
			}
		}

		if(plot_block->IsParameterDefined("pile_up_inefficiency_corrected"))
		{
			if(plot_block->GetParameterValue("pile_up_inefficiency_corrected") == 1.0)
			{
				pile_up_inefficiency_corrected = true ;
			}
		}

		if(plot_block->IsParameterDefined("to_be_saved_for_smart_merging"))
		{
			if(plot_block->GetParameterValue("to_be_saved_for_smart_merging") == 1.0)
			{
				to_be_saved_for_smart_merging = true ;
			}
		}

		if(plot_block->IsParameterDefined("variable_bin_size"))
		{
			if(plot_block->GetParameterValue("variable_bin_size") == 1.0)
			{
				variable_bin_size = true ;

				if(plot_block->IsSettingDefined("variable_bin_size_description"))
				{
					variable_bin_size_description = plot_block->GetSettingValue("variable_bin_size_description") ;
				}
				else
				{
					cout << "TAnalysisPlot::TAnalysisPlot: variable_bin_size_description is not defined !" << endl ;
					exit(1) ;
				}

			}
		}

		if(plot_block->IsParameterDefined("number_of_needed_RPs"))
		{
                        number_of_needed_RPs = plot_block->GetParameterValue("number_of_needed_RPs") ;
			cout << "changingrpnumber for " << name << " " << number_of_needed_RPs << endl ;
		}

	}
	else
	{
		cout << "TAnalysisPlot::TAnalysisPlot: the block is not defined ! : " << plot_block_name << endl ;
		exit(1) ;
	}

}

void TAnalysisPlot::Save()
{

	if(text_before.compare("") != 0)
	{
		(*(ProjectParameters->GetPLatexFile())) << "\\section{" << text_before << "}" << endl ;
	}

	SavePlot() ;

	if(AnalysisCut != NULL)
	{
		AnalysisCut->Save() ;
	}

	if(text_after.compare("") != 0)
	{
		(*(ProjectParameters->GetPLatexFile())) << "\\section{" << text_after << "}" << endl ;
	}
}

void TAnalysisPlot::SavePlot()
{
}

void TAnalysisPlot::SaveEpsRootLatex(string fig_filename, TCanvas *canvas, bool begin_end, double width, bool new_line, bool include_to_latex)
{
	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	canvas->SaveAs((output_directory + "/" + fig_filename + ".pdf").c_str()) ;

	if( ProjectParameters->GetParameterValue("CollectCutDiagnosticData") == 1.0)
	{
		canvas->SaveAs((output_directory + "/" + fig_filename + ".root").c_str()) ;
	}

	if(include_to_latex)
	{
		if(begin_end) ProjectParameters->GetLatexFile()->BeginFigure() ;

		ProjectParameters->GetLatexFile()->IncludeGraphics(fig_filename + ".pdf", width, new_line) ; 

		if(begin_end) ProjectParameters->GetLatexFile()->EndFigure() ;
	}
}

void TAnalysisPlot::AlignmentWarning()
{
	if(ProjectParameters->GetRPAlignment() != NULL)
	{
		TLatex *text = new TLatex ;
		text->SetNDC() ;
		text->SetTextSize(0.03) ;
		text->SetTextColor(kRed) ;
		text->DrawLatex(0.5, 0.5, "Software alignment corrections !") ;
		text->Draw("same") ;
	}

	string options = "" ;

	if(weighted) options += "w" ;
	if(geometrical_acceptance_corrected) options += "g" ;
	if(beam_divergence_corrected) options += "b" ;
	if(horizontal_beam_divergence_corrected) options += "h" ;
	if(pile_up_inefficiency_corrected) options += "p" ;
	if(to_be_saved_for_smart_merging) options += "s" ;
	if(variable_bin_size) options += "v" ;

	if(options.compare("") != 0)
	{
		TLatex *text = new TLatex ;
		text->SetNDC() ;
		text->SetTextSize(0.03) ;
		text->SetTextColor(kRed) ;
		text->DrawLatex(0.5, 0.6, ("Options for this plot: " + options).c_str()) ;
		text->Draw("same") ;
	}
}

#endif // #ifndef TAnalysisPlot_h
