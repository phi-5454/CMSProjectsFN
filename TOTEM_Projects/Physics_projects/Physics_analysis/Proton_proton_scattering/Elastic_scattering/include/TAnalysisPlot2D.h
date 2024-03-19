#ifndef TAnalysisPlot2D_h
#define TAnalysisPlot2D_h

#include "TAnalysisPlot.h"

#include "TProfile.h"
#include "TGraph.h"
#include "TLatex.h"

#include "TFitResult.h"

#include <iomanip>

class TAnalysisPlot2D: public TAnalysisPlot
{
	private:
	
	string x_variable_name ;
	string y_variable_name ; 
	
	const double *x_variable ;
	const double *y_variable ;
	ULong64_t *Pevent_info_timestamp ;

	TH2D *plot, *rotated_plot ;
	TH1D *distance_from_cut_plot ;
	TH1D *distance_from_cut_plot_within_range ;
	TProfile *profile, *profile_with_no_range ;
	TGraph *graph ;
	TGraph *cut_diagnostic_graph ;
	TFitResultPtr fitresult ;
	bool fitted ;

	protected:

	double horizontal_axis_boundary, number_of_horizontal_bins ;
	double vertical_axis_boundary, number_of_vertical_bins ;

	double distance_from_cut_plot_within_range_axis_boundary ;

	public:
	
	TAnalysisPlot2D(string, string, TProjectParameters *ProjectParameters, TProtonReconstruction *) ;
	
	void Fill(double = 1.0, bool test=false) ;
	void Fill(double, double, bool) ;
	void FillRotatedPlotAndProfile(double, double) ;

	string x_axis_title, y_axis_title ;

	void CreatePlot(bool = false) ;
	virtual void SavePlot() ;

	bool AdjustCut() ;
	void PrintCutCoordinates() ;
	
	void Reset() ;

	void FitProfileAndCalculateChi2OfCutPlacement(TCanvas *) ;
	bool MoveCut() ;
	
	double GetDistributionRMS() { return rotated_plot->GetRMS(2) ; } ;
	TH2D *GetPlot() { return plot ; } ;
} ;

TAnalysisPlot2D::TAnalysisPlot2D(string name, string plot_collection_name, TProjectParameters *ProjectParameters, TProtonReconstruction *ProtonReconstruction) :
	TAnalysisPlot(name, plot_collection_name + "_" + name, ProjectParameters, ProtonReconstruction), x_variable_name(""), y_variable_name(""), x_variable(NULL), y_variable(NULL),
	Pevent_info_timestamp(NULL), plot(NULL), rotated_plot(NULL), distance_from_cut_plot(NULL), distance_from_cut_plot_within_range(NULL), profile(NULL), profile_with_no_range(NULL), graph(NULL), cut_diagnostic_graph(NULL), fitresult(0), fitted(false)
{
	dimension = 2 ;

	AnalysisCut = NULL ;

	bool find_cuts = false ;
	if (ProjectParameters->GetSettingValue("project_subtask").compare("find_cuts") == 0) find_cuts = true ;

	horizontal_axis_boundary = 0 ;
	number_of_horizontal_bins = 0 ;
	vertical_axis_boundary = 0 ;
	number_of_vertical_bins = 0 ;
	distance_from_cut_plot_within_range_axis_boundary = 0 ;

	x_axis_title = y_axis_title = "" ;

	string plot_block_name = name + "_plot_block" ;
	
	if(ProjectParameters->IsBlockDefined(plot_block_name))
	{
		TProjectParameters *plot_block = ProjectParameters->GetBlock(plot_block_name) ;

		if(plot_block->IsSettingDefined("x_variable"))
		{
			x_variable_name = plot_block->GetSettingValue("x_variable") ;
		}
		else
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the x_variable is not defined ! "  << endl  ;
			exit(1) ;
		}

		if(plot_block->IsSettingDefined("y_variable"))
		{
			y_variable_name = plot_block->GetSettingValue("y_variable") ;
		}
		else
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the y_variable is not defined ! "  << endl ;
			exit(1) ;
		}

		x_variable = ProtonReconstruction->GetReconstructedVariable(x_variable_name) ;
		y_variable = ProtonReconstruction->GetReconstructedVariable(y_variable_name) ;

		if((x_variable == NULL) || (y_variable == NULL))
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the x_variable or y_variable is not defined !"  << endl ;
			exit(1) ;
		}

		// if(plot_block_name.compare(x_variable_name + "_" + y_variable_name + "_plot_block") != 0)
		// {
		//	cout << "TAnalysisPlot2D::TAnalysisPlot2D: the plot name should be built from its variable name ! : " <<  plot_block_name << " should be: " << x_variable_name << "_" << y_variable_name << "_plot_block" << endl ;
		//	exit(1) ;
		// }

		Pevent_info_timestamp = ProtonReconstruction->Get_event_info_timestamp() ;

		if(plot_block->IsParameterDefined("cut"))
		{
			if(plot_block->GetParameterValue("cut") == 1.0)
			{
				apply_cut = true ;
			}
		}

		if(plot_block->IsParameterDefined("define_cut"))
		{
			if(plot_block->GetParameterValue("define_cut") == 1.0)
			{
				define_cut = true ;
			}
		}
		
		if(plot_block->IsParameterDefined("draw_cut"))
		{
			if(plot_block->GetParameterValue("draw_cut") == 1.0)
			{
				draw_cut = true ;
			}
		}

		if(draw_cut && define_cut)
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the parameters draw_cut and define_cut cannot be true at the same time !" << endl ;
			exit(1) ;
		}

		string plot_type_block_name = plot_block->GetSettingValue("type") ;
		
		if(ProjectParameters->IsBlockDefined(plot_type_block_name))
		{
			TProjectParameters *plot_type_block = ProjectParameters->GetBlock(plot_type_block_name) ;

			horizontal_axis_boundary = plot_type_block->GetParameterValue("horizontal_axis_boundary") ;
			number_of_horizontal_bins = plot_type_block->GetParameterValue("number_of_horizontal_bins") ;

			vertical_axis_boundary = plot_type_block->GetParameterValue("vertical_axis_boundary") ;
			number_of_vertical_bins = plot_type_block->GetParameterValue("number_of_vertical_bins") ;

			if(plot_type_block->IsParameterDefined("distance_from_cut_plot_within_range_axis_boundary"))
			{
				distance_from_cut_plot_within_range_axis_boundary = plot_type_block->GetParameterValue("distance_from_cut_plot_within_range_axis_boundary") ;
			}
			else
			{
				if(horizontal_axis_boundary >= vertical_axis_boundary)
					distance_from_cut_plot_within_range_axis_boundary = (2*horizontal_axis_boundary) ;
				else
					distance_from_cut_plot_within_range_axis_boundary = (2*vertical_axis_boundary) ;
			}
		}
		else
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the type block is not defined ! : " << plot_type_block_name <<  endl ;
			exit(1) ;
		}

		string x_variable_block_name = x_variable_name + "_variable_block" ;
		string y_variable_block_name = y_variable_name + "_variable_block" ;

		if(ProjectParameters->IsBlockDefined(x_variable_block_name))
		{
			TProjectParameters *x_variable_block = ProjectParameters->GetBlock(x_variable_block_name) ;

			string unit = x_variable_block->GetSettingValue("unit") ;
			x_axis_title = x_variable_block->GetSettingValue("axis_name") + " [" + unit + "]" ;
		}
		else
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the x variable block is not defined ! : " << x_variable_block_name <<  endl ;
			exit(1) ;
		}

		if(ProjectParameters->IsBlockDefined(y_variable_block_name))
		{
			TProjectParameters *y_variable_block = ProjectParameters->GetBlock(y_variable_block_name) ;

			string unit = y_variable_block->GetSettingValue("unit") ;
			y_axis_title = y_variable_block->GetSettingValue("axis_name") + " [" + unit + "]" ;
		}
		else
		{
			cout << "TAnalysisPlot2D::TAnalysisPlot2D: the y variable block is not defined ! : " << y_variable_block_name <<  endl ;
			exit(1) ;
		}

		CreatePlot(find_cuts) ;
	}
	else
	{
		cout << "TAnalysisPlot2D::TAnalysisPlot2D: the block is not defined ! : " << plot_block_name << endl ;
		exit(1) ;
	}

	if(apply_cut || define_cut || draw_cut)
	{
		TProjectParameters *plot_block = ProjectParameters->GetBlock(plot_block_name) ;
		string cut_name = plot_block->GetSettingValue("cut_name") ;
		string cut_id	= plot_collection_name + "_" + cut_name ;

		string cut_block_name = cut_name + "_block" ;
		if(define_cut) cut_name += "_define" ;
		if(draw_cut) cut_name += "_draw" ;
		
		AnalysisCut = new TAnalysisCut2D(cut_name, cut_id, ProjectParameters, cut_block_name) ;

		if(!find_cuts)
		{
			graph = new TGraph();
			cut_diagnostic_graph = new TGraph();
		}
	}
}


void TAnalysisPlot2D::Reset()
{
	if(profile != NULL) profile->Reset() ;
	if(plot != NULL) plot->Reset() ;
	if(rotated_plot != NULL) rotated_plot->Reset() ;
	if(distance_from_cut_plot != NULL) distance_from_cut_plot->Reset() ;
	if(distance_from_cut_plot_within_range != NULL) distance_from_cut_plot_within_range->Reset() ;
	if(profile_with_no_range != NULL) profile_with_no_range->Reset() ;
	if(graph != NULL) graph->Set(0) ;
	if(cut_diagnostic_graph != NULL) cut_diagnostic_graph->Set(0) ;
	
//	delete profile ;
//	CreatePlot(true) ;
}

void TAnalysisPlot2D::CreatePlot(bool find_cuts)
{
	string id_rotated_plot = id + "_rotated" ;
	string id_profile = id_rotated_plot + "_prof" ;

	if(!find_cuts)
	{
		plot = new TH2D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, -horizontal_axis_boundary, horizontal_axis_boundary,
							(Int_t)number_of_vertical_bins,   -vertical_axis_boundary, vertical_axis_boundary) ;

		double horizontal_axis_boundary_rotated_plot = horizontal_axis_boundary ;
		double vertical_axis_boundary_rotated_plot   = vertical_axis_boundary ;

		if(horizontal_axis_boundary < vertical_axis_boundary)
		{
			horizontal_axis_boundary_rotated_plot = vertical_axis_boundary ;
			vertical_axis_boundary_rotated_plot   = horizontal_axis_boundary ;
		}

		rotated_plot = new TH2D(id_rotated_plot.c_str(), id_rotated_plot.c_str(), (Int_t)number_of_horizontal_bins, -horizontal_axis_boundary_rotated_plot, horizontal_axis_boundary_rotated_plot,
					(Int_t)number_of_vertical_bins,   -vertical_axis_boundary_rotated_plot, vertical_axis_boundary_rotated_plot) ;

		profile_with_no_range = new TProfile((id_profile + "_with_no_range").c_str(), id_profile.c_str(),	(2*(Int_t)number_of_horizontal_bins), -distance_from_cut_plot_within_range_axis_boundary, distance_from_cut_plot_within_range_axis_boundary) ;

		string id_distance_from_cut_plot_within_range_plot = id + "_distance_from_cut_within_range" ;
		string id_distance_from_cut_plot = id + "_distance_from_cut" ;

		distance_from_cut_plot_within_range = new TH1D(id_distance_from_cut_plot_within_range_plot.c_str(), id_distance_from_cut_plot_within_range_plot.c_str(), (2*(Int_t)number_of_horizontal_bins), -distance_from_cut_plot_within_range_axis_boundary, distance_from_cut_plot_within_range_axis_boundary) ;
		distance_from_cut_plot = new TH1D(id_distance_from_cut_plot.c_str(), id_distance_from_cut_plot.c_str(), (2*(Int_t)number_of_horizontal_bins), -distance_from_cut_plot_within_range_axis_boundary, distance_from_cut_plot_within_range_axis_boundary) ;
	}

	profile = new TProfile(id_profile.c_str(), id_profile.c_str(),	(2*(Int_t)number_of_horizontal_bins), -distance_from_cut_plot_within_range_axis_boundary, distance_from_cut_plot_within_range_axis_boundary) ;
}

void TAnalysisPlot2D::Fill(double weight, bool test)
{
	Fill(*x_variable, *y_variable, test) ;
}

void TAnalysisPlot2D::Fill(double x, double y, bool test)
{
	fill_status = false ;

	if((plot != NULL) && (!test))
	{
		plot->Fill(x,y) ;
	}

	if(AnalysisCut == NULL)
	{
		fill_status = true ;
	}
	else
	{
		if(plot != NULL) 
		{
			if( ProjectParameters->GetParameterValue("CollectCutDiagnosticData") == 1.0)
			{
				int number_of_points = graph->GetN() ;

				if(!test)
				{
					graph->SetPoint(number_of_points, x,y) ;
				}
			}
		}

		if(!test) 
		{
			FillRotatedPlotAndProfile(x,y) ;
		}

		if(draw_cut || AnalysisCut->IsTrue(x,y))
		{
			fill_status = true ;
		}
	}
}

void TAnalysisPlot2D::FillRotatedPlotAndProfile(double x, double y)
{
	double position  = ((TAnalysisCut2D *)AnalysisCut)->PositionAlongCut(x, y) ;
	double distance  = ((TAnalysisCut2D *)AnalysisCut)->DistanceFromCut(x, y) ;

	if(plot != NULL)
	{
		profile_with_no_range->Fill(position, distance) ; // plot is NULL only if find_cuts was true
		distance_from_cut_plot->Fill(distance) ;
	}


	if(((TAnalysisCut2D *)AnalysisCut)->IsThePointWithinRange(x,y))
	{
		profile->Fill(position, distance) ;
		
		if(plot != NULL)
		{
			rotated_plot->Fill(position,distance) ;
			distance_from_cut_plot_within_range->Fill(distance) ;
			
			if(ProjectParameters->GetParameterValue("CollectCutDiagnosticData") == 1.0)
			{
				int number_of_points = cut_diagnostic_graph->GetN() ;
				cut_diagnostic_graph->SetPoint(number_of_points, (*Pevent_info_timestamp) , distance) ;
			}
		}
	}
}

bool TAnalysisPlot2D::MoveCut()
{
	bool DidCutMove = false ;

	std::cout << "Move cuts" << endl ;

        bool do_fit = false ;

	if(profile->GetEntries() != 0 && define_cut && do_fit)
	{
		TFitResultPtr fitresult = profile->Fit("pol1","S") ;
		const int y_axis = 2 ;

		double sigma_of_fit = rotated_plot->GetRMS(y_axis) ;

		if(AnalysisCut->GetSigma() != sigma_of_fit)
		{
			((TAnalysisCut2D *)AnalysisCut)->Shift(0, sigma_of_fit) ;

			DidCutMove = true ;
		}

		if(fabs(fitresult->Parameter(0)) > fabs(fitresult->ParError(0)))
		{
			((TAnalysisCut2D *)AnalysisCut)->Shift((fitresult->Parameter(0)), sigma_of_fit) ;

			DidCutMove = true ;
		}

		if(fabs(fitresult->Parameter(1)) > fabs(fitresult->ParError(1)))
		{
			((TAnalysisCut2D *)AnalysisCut)->Rotate((fitresult->Parameter(1)), sigma_of_fit) ;

			DidCutMove = true ;
		}
	}

	return DidCutMove ;
}


bool TAnalysisPlot2D::AdjustCut()
{
	bool DidCutMove = false ;

	if((AnalysisCut != NULL) && (define_cut))
	{
		DidCutMove = MoveCut() ;
	}

	return DidCutMove ;
}

void TAnalysisPlot2D::PrintCutCoordinates()
{

	if((AnalysisCut != NULL))
	{
		string cut_name = AnalysisCut->GetName() + "_block" ;

		bool acceptance_cut = false ;

		if((cut_name.compare(ProjectParameters->GetSettingValue("geometrical_acceptance_cut_block")) == 0) ||
		(cut_name.compare(ProjectParameters->GetSettingValue("vertical_angular_left_acceptance_cut_block")) == 0) ||
		(cut_name.compare(ProjectParameters->GetSettingValue("vertical_angular_right_acceptance_cut_block")) == 0))
		{
			acceptance_cut = true ;
		}
	
		if(acceptance_cut || define_cut)
		{
			((TAnalysisCut2D *)AnalysisCut)->Print() ;
			cout << endl ;
		}
	}
}

void TAnalysisPlot2D::FitProfileAndCalculateChi2OfCutPlacement(TCanvas *canvas)
{

	gStyle->SetOptStat("nemrou");	
	gStyle->SetOptFit(1111);

	bool do_fit = false ;

	if(define_cut && (profile->GetEntries() != 0) && do_fit)
	{
		fitresult = profile->Fit("pol1","S") ;
		fitted = true ;

		gStyle->SetOptStat("nemrou");	
		gStyle->SetOptFit(1111);

		profile->GetFunction("pol1")->SetLineColor(kRed) ;
		profile->GetFunction("pol1")->SetLineStyle(kDashed) ;

		if(define_cut)
		{
			stringstream ss_ndf ;
			ss_ndf << (fitresult->Ndf()) ;

			stringstream ss_chi2 ;
			ss_chi2 << (fitresult->MinFcnValue()) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Chi2/NDF", (ss_chi2.str() + "/" + ss_ndf.str())) ;

			bool prob_bold = false ; 
			if((fitresult->Prob() < 0.01) || ((fitresult->Ndf() < 5))) prob_bold = true ;

			stringstream ss_prob ;
			ss_prob << (fitresult->Prob()) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Prob", (ss_prob.str()), prob_bold) ;

			// Slope

			bool slope_bold = false ; 
			if(fabs(fitresult->Parameter(1)) > fabs(fitresult->ParError(1))) slope_bold = true ;

			stringstream ss_slope ;
			ss_slope << (fitresult->Parameter(1)) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Slope", ss_slope.str(), slope_bold) ;

			stringstream ss_slope_error ;
			ss_slope_error << (fitresult->ParError(1)) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Slope error", ss_slope_error.str(), slope_bold) ;

			// Mean

			bool mean_bold = false ; 
			if(fabs(fitresult->Parameter(0)) > fabs(fitresult->ParError(0))) mean_bold = true ;

			stringstream ss_mean ;
			ss_mean << (fitresult->Parameter(0)) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Mean", ss_mean.str(), mean_bold) ;

			stringstream ss_mean_error ;
			ss_mean_error << (fitresult->ParError(0)) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "Mean error", ss_mean_error.str(), mean_bold) ;

			// RMS

			const int y_axis = 2 ;
			bool rms_bold = false ; 
			if(fabs(((AnalysisCut->GetSigma()) - (rotated_plot->GetRMS(y_axis))) / AnalysisCut->GetSigma()) > 0.01) rms_bold = true ;

			stringstream ss_sigma ;
			ss_sigma << AnalysisCut->GetSigma() ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "$\\sigma_{cut}$", ss_sigma.str(), rms_bold) ;

			stringstream ss_RMS ;
			ss_RMS << rotated_plot->GetRMS(y_axis) ;
			ProjectParameters->GetLatexFile()->SetElementInTable("SummaryOfCuts", name, "RMS", ss_RMS.str(), rms_bold) ;

		}
	}

	profile_with_no_range->Draw("") ;
	profile_with_no_range->SetLineColor(kGreen) ;
	profile_with_no_range->GetXaxis()->SetTitle((x_axis_title + " rotated").c_str()) ;  
	profile_with_no_range->GetYaxis()->SetTitle((y_axis_title + "rotated").c_str()) ;     

	profile->Draw("same") ;

	AnalysisCut->DrawCutOnRotatedPlot(-2*horizontal_axis_boundary, 2*horizontal_axis_boundary) ;

	AlignmentWarning() ;

        bool save_only_canvas = true ;

        if(ProjectParameters->GetParameterValue("WriteEverything") == 1.0)
        {
                save_only_canvas = false ;
        }

        if(!save_only_canvas) SaveEpsRootLatex(id + "_rotated_profile", canvas, TConstants::NO_BEGIN_END, TConstants::WIDTH_60_PERCENT, false, true) ;
}

void TAnalysisPlot2D::SavePlot()
{
	// cout << "TAnalysisPlot2D::SavePlot: I am here 0" << endl ;

	ProjectParameters->GetLatexFile()->BeginFigure() ;

	string canvas_name = (id + "_canvas").c_str() ;
	TCanvas *canvas = new TCanvas(canvas_name.c_str(), canvas_name.c_str()) ;

	cout << "TAnalysisPlot2D::SavePlot: I am here 1" << endl ;

	gStyle->SetPalette(1);    
	gStyle->SetTitleFillColor(kWhite) ;
	gStyle->SetStatColor(kWhite) ;  

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	canvas->SetGridx() ;
	canvas->SetGridy() ;	
	canvas->SetLogz() ;
   
	canvas->SetFillColor(kWhite) ;

	canvas->SetFrameBorderMode(0) ;   
	
	gStyle->SetOptStat("nemrou");
	
	plot->Draw("colz") ;
	plot->GetXaxis()->SetTitle(x_axis_title.c_str()) ;  
	plot->GetYaxis()->SetTitle(y_axis_title.c_str()) ;     

	TLatex text ;
	string str ;

	if(AnalysisCut != NULL)
	{
		AnalysisCut->DrawCut() ;

	}

	// cout << "TAnalysisPlot2D::SavePlot: I am here 2" << endl ;

	DrawExtras() ;
	AlignmentWarning() ;

	bool save_only_canvas = true ;

	if(ProjectParameters->GetParameterValue("WriteEverything") == 1.0)
	{
		save_only_canvas = false ;
	}

	if(!save_only_canvas) SaveEpsRootLatex(id, canvas, TConstants::NO_BEGIN_END, TConstants::WIDTH_60_PERCENT, false, true) ;
	canvas->Write() ;

	if(AnalysisCut != NULL)
	{
		// cout << "TAnalysisPlot2D::SavePlot: I am here 3" << endl ;

		rotated_plot->Draw("colz") ;
		rotated_plot->GetXaxis()->SetTitle((x_axis_title + " rotated").c_str()) ;  
		rotated_plot->GetYaxis()->SetTitle((y_axis_title + "rotated").c_str()) ;     
		
		text.DrawLatex(-horizontal_axis_boundary/2.0 , vertical_axis_boundary/2.0 , ("#sigma=" + str).c_str()) ;
		AnalysisCut->DrawCutOnRotatedPlot(-horizontal_axis_boundary, horizontal_axis_boundary) ;
		AlignmentWarning() ;

		double step = vertical_axis_boundary / 10 ;

		if(!save_only_canvas) SaveEpsRootLatex(id + "_rotated", canvas, TConstants::NO_BEGIN_END, TConstants::WIDTH_60_PERCENT, TConstants::NEW_LINE, true) ;

		FitProfileAndCalculateChi2OfCutPlacement(canvas) ;

		distance_from_cut_plot_within_range->Draw() ;
		if(distance_from_cut_plot_within_range->GetEntries() != 0) distance_from_cut_plot_within_range->Fit("gaus") ;

		// Try to simplify this with constructor

		gStyle->SetOptFit(1111);	
		AlignmentWarning() ;
		
		std::ostringstream strs2, strs3 ;
		strs2 << ((TAnalysisCut2D *)AnalysisCut)->GetSlope() ;

		if(fitted) strs3 << fitresult->ParError(1) ;

		text.SetNDC() ;
		text.SetTextColor(kRed) ;
		text.DrawLatex(0.2, 0.8, ("m =" + strs2.str() + "#pm" + strs3.str()).c_str()) ;

		if(!save_only_canvas) SaveEpsRootLatex(distance_from_cut_plot_within_range->GetTitle(), canvas, TConstants::NO_BEGIN_END, TConstants::WIDTH_60_PERCENT, false, true) ;

		// cout << "TAnalysisPlot2D::SavePlot: I am here 4" << endl ;

		if(ProjectParameters->GetParameterValue("CollectCutDiagnosticData") == 1.0)
		{
			if(graph == NULL)
			{
				cout << "Error: TAnalysisPlot2D::SavePlot: graph == NULL !" << endl ;
				exit(1) ;
			}

			graph->Draw("AP") ;
			graph->GetXaxis()->SetTitle(x_axis_title.c_str()) ;  
			graph->GetXaxis()->SetRangeUser( -horizontal_axis_boundary, horizontal_axis_boundary) ;

			graph->GetYaxis()->SetTitle(y_axis_title.c_str()) ;
			graph->GetYaxis()->SetRangeUser( -vertical_axis_boundary, vertical_axis_boundary) ;
		
			DrawExtras() ;	
			AnalysisCut->DrawCut() ;
			text.DrawLatex(-horizontal_axis_boundary/2, vertical_axis_boundary/2, ("#sigma=" + str).c_str()) ;
			
			if(!save_only_canvas) SaveEpsRootLatex(id + "_graph", canvas, TConstants::NO_BEGIN_END, TConstants::WIDTH_60_PERCENT, false, false) ;
		}

		if(define_cut) MoveCut() ;

	}

	cout << "TAnalysisPlot2D::SavePlot: I am here 5" << endl ;

	ProjectParameters->GetLatexFile()->EndFigure() ;

	cout << "TAnalysisPlot2D::SavePlot: I am here 6" << endl ;

	if((AnalysisCut != NULL))
	{

		if(ProjectParameters->GetParameterValue("CollectCutDiagnosticData") == 1.0)
		{
			if(!save_only_canvas) SaveEpsRootLatex(id + "_cut_diagnostic_graph", canvas, true, 1.0, false, false) ;
		}
	}

	cout << "TAnalysisPlot2D::SavePlot: I am here 8" << endl ;
	
	
}

#endif // #ifndef TAnalysisPlot2D_h
