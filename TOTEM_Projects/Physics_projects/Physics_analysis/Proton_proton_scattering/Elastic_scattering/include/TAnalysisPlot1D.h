#ifndef TAnalysisPlot1D_h
#define TAnalysisPlot1D_h

#include "TFitResult.h"
#include "TAnalysisPlot.h"

class TAnalysisPlot1D : public TAnalysisPlot
{

	private:

	TH1D *plot ;
	string x_variable_name ;
	const bool *Pelastic ;
	const UInt_t *Ptrigger_data_run_num ;
	const UInt_t *Ptrigger_data_event_num ;

	const double *x_variable ;
	const double *minus_t_y_GeV2_for_beam_divergence_acceptance_correction ;
	const double *theta_x_star_rad_for_beam_divergence_acceptance_correction ;

	double beam_momentum_GeV ;
	double theta_x_star_rad_theta_y_star_rad_cut_block_y1 ;
	double theta_x_star_rad_theta_y_star_rad_cut_block_sigma ;
	double pile_up_inefficiency_correction ;
	double theta_x_star_rad_cut_left, theta_x_star_rad_cut_right ;

	protected:

	string plot_collection_name ;
	string axis_title ;

	double horizontal_axis_left_boundary, horizontal_axis_right_boundary ;
	int number_of_horizontal_bins ;
	
	public:
	
	TAnalysisPlot1D(string, string, TProjectParameters *, TProtonReconstruction *) ;
	
	void Fill(double, bool) ;

	TH1D *GetPlot() { return plot ; }
	void SetPlot(TH1D *aplot) { plot  = aplot ; }

	double GetSafeMaxBoundary() { return (2.0 * max(fabs(horizontal_axis_right_boundary), fabs(horizontal_axis_left_boundary))) ; } ;  // Boundary: to be sure that the function is defined in the right range, including the endpoints
	TF1 *GetIdentityFunction(string name) { return new TF1(name.c_str(), "1.0", -GetSafeMaxBoundary(), GetSafeMaxBoundary()); } ;

	void CreatePlot() ;
	virtual void SavePlot() ;
	void Reset() ;
} ;

TAnalysisPlot1D::TAnalysisPlot1D(string name, string plot_collection_name, TProjectParameters *ProjectParameters, TProtonReconstruction *ProtonReconstruction) : TAnalysisPlot(name, plot_collection_name + "_" + name, ProjectParameters, ProtonReconstruction),
	pile_up_inefficiency_correction(0.0), Pelastic(NULL), plot(NULL), theta_x_star_rad_theta_y_star_rad_cut_block_y1(0.0), theta_x_star_rad_theta_y_star_rad_cut_block_sigma(0.0), theta_x_star_rad_cut_left(0.0), theta_x_star_rad_cut_right(0.0)
{
	plot = NULL ;
	AnalysisCut = NULL ;
	x_variable = NULL ;
	minus_t_y_GeV2_for_beam_divergence_acceptance_correction = NULL ;
	theta_x_star_rad_for_beam_divergence_acceptance_correction = NULL ;

	beam_momentum_GeV = (ProjectParameters->GetBeam_1_Optics()->GetBeamEnergyGeV()) ;

	dimension = 1 ;
	
	axis_title = "" ;

	string plot_block_name = name + "_plot_block" ;

	if(ProjectParameters->IsBlockDefined(plot_block_name))
	{
		TProjectParameters *plot_block = ProjectParameters->GetBlock(plot_block_name) ;

		x_variable_name = plot_block->GetSettingValue("x_variable") ;
		x_variable = ProtonReconstruction->GetReconstructedVariable(x_variable_name) ;
		Pelastic = ProtonReconstruction->GetElasticPointer() ;
		Ptrigger_data_run_num = ProtonReconstruction->GetTriggerDataRunNum() ;
		Ptrigger_data_event_num = ProtonReconstruction->GetTriggerDataEventNum() ;

		if(beam_divergence_corrected)
		{
			minus_t_y_GeV2_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable(plot_block->GetSettingValue("beam_divergence_parameter")) ;

			if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
			{
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) minus_t_y_GeV2_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("minus_t_y_without_left_near_GeV2") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6R5.B1") == 0.0) minus_t_y_GeV2_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("minus_t_y_without_right_near_GeV2") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) minus_t_y_GeV2_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("minus_t_y_without_left_far_GeV2") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6R5.B1") == 0.0) minus_t_y_GeV2_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("minus_t_y_without_right_far_GeV2") ;
			}

		}

		if(horizontal_beam_divergence_corrected)
		{
			if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
			{
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) theta_x_star_rad_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("theta_x_star_right_rad") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6R5.B1") == 0.0) theta_x_star_rad_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("theta_x_star_left_rad") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) theta_x_star_rad_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("theta_x_star_right_rad") ;
				if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6R5.B1") == 0.0) theta_x_star_rad_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("theta_x_star_left_rad") ;
			}
			else
			{
				theta_x_star_rad_for_beam_divergence_acceptance_correction = ProtonReconstruction->GetReconstructedVariable("theta_x_star_rad") ;
			}

		}

		string plot_type_block_name = plot_block->GetSettingValue("type") ;
		
		if(ProjectParameters->IsBlockDefined(plot_type_block_name))
		{
			TProjectParameters *plot_type_block = ProjectParameters->GetBlock(plot_type_block_name) ;

			horizontal_axis_left_boundary = plot_type_block->GetParameterValue("horizontal_axis_left_boundary") ;
			horizontal_axis_right_boundary = plot_type_block->GetParameterValue("horizontal_axis_right_boundary") ;
			number_of_horizontal_bins = plot_type_block->GetParameterValue("number_of_horizontal_bins") ;
		}
		else
		{
			cout << "TAnalysisPlot1D::TAnalysisPlot1D: the type block is not defined ! : " << plot_type_block_name <<  endl ;
			exit(1) ;
		}

		string x_variable_block_name = x_variable_name + "_variable_block" ;

		if(ProjectParameters->IsBlockDefined(x_variable_block_name))
		{
			TProjectParameters *x_variable_block = ProjectParameters->GetBlock(x_variable_block_name) ;

			string unit = x_variable_block->GetSettingValue("unit") ;
			axis_title = x_variable_block->GetSettingValue("axis_name") + " [" + unit + "]" ;
		}
		else
		{
			cout << "TAnalysisPlot1D::TAnalysisPlot1D: the x variable block is not defined ! : " << x_variable_block_name <<  endl ;
			exit(1) ;
		}

		CreatePlot() ;
	}
	else
	{
		cout << "TAnalysisPlot1D::TAnalysisPlot1D: the block is not defined ! : " << plot_block_name << endl ;
		exit(1) ;
	}

	string cut_block_name = ProjectParameters->GetSettingValue("geometrical_acceptance_cut_block") ;

	if(ProjectParameters->IsBlockDefined(cut_block_name))
	{
		TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

		theta_x_star_rad_theta_y_star_rad_cut_block_y1 = cut_block->GetParameterValue("y1") ;
		theta_x_star_rad_theta_y_star_rad_cut_block_sigma = cut_block->GetParameterValue("sigma") ;
	}
	else
	{
		cout << "Error:  TAnalysisPlot1D::TAnalysisPlot1D: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
		exit(1) ;
	}

	if(ProjectParameters->IsSettingDefined("horizontal_geometrical_acceptance_cut_block"))
	{
		cut_block_name = ProjectParameters->GetSettingValue("horizontal_geometrical_acceptance_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			theta_x_star_rad_cut_left  = cut_block->GetParameterValue("x1") - cut_block->GetParameterValue("sigma") ;
			theta_x_star_rad_cut_right = cut_block->GetParameterValue("x1") + cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TAnalysisPlot1D::TAnalysisPlot1D: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}
	}

	if(pile_up_inefficiency_corrected)
	{
		const double hundred_percent = 100.0 ;
		pile_up_inefficiency_correction = hundred_percent / (hundred_percent - ProjectParameters->GetParameterValue("pile_up_inefficiency_correction_percent")) ;
	}

}

void TAnalysisPlot1D::CreatePlot()
{
	if(variable_bin_size)
	{
		if(variable_bin_size_description.compare("variable_bin_size_from_binning_file") == 0)
		{
			number_of_horizontal_bins = 0 ;
			Double_t bin_edges[1000] ;

			string binning_file_name = ProjectParameters->GetSettingValue("binning_file_name") ;		

			ifstream binning_file ;

			binning_file.open(binning_file_name.c_str()) ;

			if(!binning_file.fail())
			{
				double next_edge = 0 ;

				while(binning_file >> next_edge)
				{
					bin_edges[number_of_horizontal_bins] = next_edge ;
					++number_of_horizontal_bins ;
				}
			}
			else
			{
				cout << "Error:  TAnalysisPlot1D::TAnalysisPlot1D: the binning file " << binning_file_name << " cannot be opened!" << endl ;
				exit(1) ;
			}

			number_of_horizontal_bins = number_of_horizontal_bins - 1 ;

			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, bin_edges) ;

		}
		else if(variable_bin_size_description.compare("0p005_between_0_and_0p28_GeV2_0p04_between_0p28_and_4_GeV2") == 0)
		{
			number_of_horizontal_bins = 150 ;
			Double_t bin_edges[151] ;
			Double_t bin_edge = 0 ;
			Double_t bin_dt = 0.005 ;
			
			for(int bin_index = 0 ; bin_index < 56 ; ++bin_index)
			{
				bin_edges[bin_index] = bin_edge ;
				cout << bin_edge << endl ;
				bin_edge += bin_dt ;
			}
			
			bin_dt = 0.04 ;

			for(int bin_index = 0 ; bin_index < 95 ; ++bin_index)
			{
				bin_edges[56 + bin_index] = bin_edge ;
				cout << bin_edge << endl ;
				bin_edge += bin_dt ;
			}
			
			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, bin_edges) ;

		}
		else if(variable_bin_size_description.compare("variable_bin_size_test") == 0)
		{


			/*
			number_of_horizontal_bins = 400 ;
			Double_t bin_edges[401] ;
			Double_t bin_edge = 0.2 ;
			Double_t bin_dt = 0.00005 ;
			
			for(int bin_index = 0 ; bin_index < (number_of_horizontal_bins+1) ; ++bin_index)
			{
				bin_edges[bin_index] = bin_edge ;
				cout << bin_edge << endl ;
				bin_edge += bin_dt + (bin_index*(0.0125e-4)) ;
			}

			

			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, bin_edges) ;*/
			
			number_of_horizontal_bins = 400 ;
			Double_t bin_edges[401] ;
			Double_t bin_edge = 0 ;
			Double_t bin_dt = 0.0005 ;
			
			for(int bin_index = 0 ; bin_index < (number_of_horizontal_bins+1) ; ++bin_index)
			{
				bin_edges[bin_index] = bin_edge ;
				cout << bin_edge << endl ;
				bin_edge += bin_dt + (bin_index*(0.0125e-3)) ;
			}

			

			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, bin_edges) ; 

			/*
			number_of_horizontal_bins = 200 ;
			Double_t bin_edges[201] ;
			Double_t bin_edge = 0 ;
			Double_t bin_dt = 0.0025 ;
			
			for(int bin_index = 0 ; bin_index < (number_of_horizontal_bins+1) ; ++bin_index)
			{
				bin_edges[bin_index] = bin_edge ;
				cout << bin_edge << endl ;
				bin_edge += bin_dt + (bin_index*(1.25e-3)) ;
			}
			
			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, bin_edges) ; */

		}
		else if(variable_bin_size_description.compare("variable_bin_size_test_wrong") == 0)
		{
			number_of_horizontal_bins = 100 ;
			int number_of_horizontal_subbins = 16 ;

			int number_of_all_bins = (number_of_horizontal_bins * number_of_horizontal_subbins) ;

			Double_t bin_edges[number_of_all_bins+1] ;
			Double_t bin_edge = 0 ;
			Double_t bin_dt = 0.005 ;
			
			double bin_width = 0 ;

			for(int bin_index = 0 ; bin_index < (number_of_horizontal_bins) ; ++bin_index)
			{
			
				bin_width = bin_dt + (bin_index * (1.25e-3)) ;
				double dt2 = (bin_width / number_of_horizontal_subbins) ;

				for(int bin_index_2 = 0 ; bin_index_2 < (number_of_horizontal_subbins) ; ++bin_index_2)
				{
					bin_edges[(bin_index * number_of_horizontal_subbins) + bin_index_2] = bin_edge + (bin_index_2 * dt2) ;
					cout << "bin_index:" << (bin_index * number_of_horizontal_subbins) + bin_index_2 << endl ;
					cout << "bin_edge:" << (bin_edge + (bin_index_2 * dt2)) << endl ;

				}

				bin_edge += bin_width ;
			}

			bin_edge += bin_width ;

			bin_edges[number_of_all_bins] = bin_edge ;

			plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_all_bins, bin_edges) ;

		}
		else
		{
			cout << "Error:  TAnalysisPlot1D::TAnalysisPlot1D: there is no description for the variable bin size !" << endl ;
			exit(1) ;
		}
	}
	else
	{
		plot = new TH1D(id.c_str(), id.c_str(),	(Int_t)number_of_horizontal_bins, horizontal_axis_left_boundary, horizontal_axis_right_boundary) ;
	}

	plot->Sumw2() ;

	// if(!ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP")) plot->Sumw2() ;
}

void TAnalysisPlot1D::Fill(double a_weight, bool test)
{

	double weight = 1.0 ;
	double weight_geometrical_acceptance_correction = 1.0 ;
	double weight_beam_divergence_correction = 1.0 ;
	double weight_horizontal_beam_divergence_correction = 1.0 ;
	double weight_pile_up_inefficiency_correction = 1.0 ;

	if(weighted)
	{
		weight = a_weight ;
	}

	if(geometrical_acceptance_corrected)
	{
		Double_t parameters[5] ;

		parameters[0] = beam_momentum_GeV ;
		parameters[1] = fabs(theta_x_star_rad_theta_y_star_rad_cut_block_y1) - theta_x_star_rad_theta_y_star_rad_cut_block_sigma ;
		parameters[2] = fabs(theta_x_star_rad_theta_y_star_rad_cut_block_y1) + theta_x_star_rad_theta_y_star_rad_cut_block_sigma ;
		parameters[3] = theta_x_star_rad_cut_left ;
		parameters[4] = theta_x_star_rad_cut_right ;

		ProjectParameters->Get_preliminary_acceptance_beam_1()->GetPreliminaryAcceptance_TF1()->SetParameters(parameters) ;
		weight_geometrical_acceptance_correction = ProjectParameters->Get_preliminary_acceptance_beam_1()->GetPreliminaryAcceptance_TF1()->Eval(*x_variable) ;
	}

	if(beam_divergence_corrected)
	{
		weight_beam_divergence_correction = ProjectParameters->Get_preliminary_acceptance_beam_1()->GetBeamDivergenceAcceptanceCorrection_TF1()->Eval(*minus_t_y_GeV2_for_beam_divergence_acceptance_correction) ;
	}

	if(horizontal_beam_divergence_corrected)
	{
		weight_horizontal_beam_divergence_correction = ProjectParameters->Get_preliminary_acceptance_beam_1()->GetHorizontalBeamDivergenceAcceptanceCorrection_TF1_in_angle()->Eval(*theta_x_star_rad_for_beam_divergence_acceptance_correction) ;
	}

	if(pile_up_inefficiency_corrected)
	{
		weight_pile_up_inefficiency_correction = pile_up_inefficiency_correction ;
		// cout << "weight_pile_up_inefficiency_correction: " << weight_pile_up_inefficiency_correction << endl ;
	}

	double weight_of_corrections = weight * weight_geometrical_acceptance_correction * weight_beam_divergence_correction * weight_horizontal_beam_divergence_correction * weight_pile_up_inefficiency_correction ;

	if(!test)
	{
		plot->Fill(*x_variable, weight_of_corrections) ;

		if(to_be_saved_for_smart_merging)
		{
			(*(ProjectParameters->GetPLogFile())) << "Proton_kinematics_for_smart_merging: " << *Ptrigger_data_run_num << "  " << *Ptrigger_data_event_num << "  " << (*x_variable) << "  "  << weight_of_corrections << "  " << weight << endl ;
		}
	}

	fill_status = true ;
}


void TAnalysisPlot1D::Reset()
{
	plot->Reset() ;
}

void TAnalysisPlot1D::SavePlot()
{
	string canvas_name = (id + "_canvas").c_str() ;
	TCanvas *canvas = new TCanvas(canvas_name.c_str(), canvas_name.c_str()) ;

	gStyle->SetPalette(1);    
	gStyle->SetTitleFillColor(kWhite) ;
	gStyle->SetStatColor(kWhite) ;  

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	canvas->SetGridx() ;
	canvas->SetGridy() ;	
   
	canvas->SetFillColor(kWhite) ;

	canvas->SetFrameBorderMode(0) ;   
	
	plot->Draw("") ;
	plot->GetXaxis()->SetTitle(axis_title.c_str()) ;

	AlignmentWarning() ;

	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	SaveEpsRootLatex(id, canvas, true, 1.0, false, true) ;	

	string id_log = id + "_log" ;

	canvas->SetLogy() ;
	SaveEpsRootLatex(id_log, canvas, true, 1.0, false, true) ;	
	canvas->SetLogy(0) ;

	if((name.compare("dN_dt_GeV2") == 0) || (name.compare("dN_dt_GeV2_geometrical_acceptance_corrected") == 0) || (name.compare("dN_dt_GeV2_geometrical_acceptance_corrected_beam_divergence_corrected") == 0) || (name.compare("dN_dt_GeV2_geometrical_acceptance_corrected_beam_divergence_corrected_pile_up_inefficiency_corrected") == 0))
	{

		/*
		if(ProjectParameters->IsBlockDefined("Overall_normalization_based_on_other_mesurements_luminosity_integral") )
		{
			TProjectParameters *block = ProjectParameters->GetBlock("Overall_normalization_based_on_other_mesurements_luminosity_integral") ;

			Double_t lower_abs_t_boundary_of_integral_GeV2 = block->GetParameterValue("lower_abs_t_boundary_of_integral_GeV2") ;
			Double_t upper_abs_t_boundary_of_integral_GeV2 = block->GetParameterValue("upper_abs_t_boundary_of_integral_GeV2") ;
			Double_t Integrated_luminosity_in_the_range_mb = block->GetParameterValue("Integrated_luminosity_in_the_range_mb") ;

			Double_t actual_integral_mb = plot->Integral(plot->FindBin(lower_abs_t_boundary_of_integral_GeV2), plot->FindBin(upper_abs_t_boundary_of_integral_GeV2)) ;

			Double_t ratio = (Integrated_luminosity_in_the_range_mb / actual_integral_mb) ;

			plot->Multiply(GetIdentityFunction("Constant_function_1"), ratio) ;

			string id_accep_luminosity_corrected = id + "_acceptance_corrected_luminosity_corrected" ;
			SaveEpsRootLatex(id_accep_luminosity_corrected, canvas) ;			

			string id_luminosity_corrected_log = id + "_luminosity_corrected_log" ;
			canvas->SetLogy() ;

			plot->Fit("expo") ;
			SaveEpsRootLatex(id_luminosity_corrected_log, canvas) ;
			canvas->SetLogy(0) ;
		
		}*/
		
		if(ProjectParameters->IsBlockDefined("Exponential_part_settings") && false)
		{
			TProjectParameters *block = ProjectParameters->GetBlock("Exponential_part_settings") ;

			Double_t lower_abs_t_boundary_of_fit_GeV2 = block->GetParameterValue("lower_abs_t_boundary_of_fit_GeV2") ;
			Double_t upper_abs_t_boundary_of_fit_GeV2 = block->GetParameterValue("upper_abs_t_boundary_of_fit_GeV2") ;

			// ===================== Beginning of exponential fit of the data in the selected interval

			TFitResultPtr fitresult = plot->Fit("expo", "S", "", lower_abs_t_boundary_of_fit_GeV2, upper_abs_t_boundary_of_fit_GeV2) ;
			TF1 *fitted_polinomial = plot->GetFunction("expo") ;
			
			// Print of the result

			// Slope

			stringstream ss_slope ;
			ss_slope << (-fitresult->Parameter(1)) ;

			stringstream ss_slope_error ;
			ss_slope_error << (fitresult->ParError(1)) ;

			TLatex *text_slope = new TLatex ;
			text_slope->SetNDC() ;
			text_slope->SetTextSize(0.04) ;
			text_slope->DrawLatex(0.2, 0.5, ("B_{exp}=" + ss_slope.str() + " #pm " + ss_slope_error.str()).c_str()) ;
			text_slope->Draw("same") ;
			text_slope->Write() ;
			
			stringstream ss_mean ;
			ss_mean << exp((fitresult->Parameter(0))) ;

			double mean_plus__error = fabs(exp(fitresult->Parameter(0) + fitresult->ParError(0)) - exp(fitresult->Parameter(0))) ;
			double mean_minus_error = fabs(exp(fitresult->Parameter(0) - fitresult->ParError(0)) - exp(fitresult->Parameter(0))) ;

			double mean_error = max(mean_plus__error, mean_minus_error) ;

			stringstream ss_mean_error ;
			ss_mean_error << mean_error ;

			TLatex *text_mean = new TLatex ;
			text_mean->SetNDC() ;
			text_mean->SetTextSize(0.04) ;
			text_mean->DrawLatex(0.2, 0.4, ("dN/dt_{t=0, exp}=" + ss_mean.str() ).c_str()) ;
			text_mean->Draw("same") ;
			text_mean->Write() ;

			AlignmentWarning() ;

			canvas->SetLogy() ;
			canvas->Write() ;
			canvas->SetLogy(0) ;

			string id_exponential = id + "_exponential" ;
			SaveEpsRootLatex(id_exponential, canvas, true, 1.0, false, true) ;

			// ===================== End of exponential fit

			int number_of_entries = plot->GetNbinsX() ;

			Double_t *x_values = new Double_t[number_of_entries] ;
			Double_t *y_values = new Double_t[number_of_entries] ;

			for(int i = 0 ; i < number_of_entries ; ++i)
			{
				x_values[i] = plot->GetBinCenter(i) ;

				if(plot->GetBinContent(i) != 0) y_values[i] = ((plot->GetBinContent(i) - fitted_polinomial->Eval(plot->GetBinCenter(i))) / fitted_polinomial->Eval(plot->GetBinCenter(i))) ;
				else y_values[i] = 0 ;
			}

			TGraph *delta_dsigma_per_dt_graph = new TGraph(number_of_entries,x_values,y_values) ;
			delta_dsigma_per_dt_graph->Draw("AP") ;
			delta_dsigma_per_dt_graph->SetMarkerStyle(20) ;
			delta_dsigma_per_dt_graph->SetMarkerSize(1) ;
			delta_dsigma_per_dt_graph->GetXaxis()->SetRangeUser(0, upper_abs_t_boundary_of_fit_GeV2) ;

			double relative_delta_sigma_per_delta_t_range = block->GetParameterValue("relative_delta_sigma_per_delta_t_range") ;

			delta_dsigma_per_dt_graph->GetYaxis()->SetRangeUser(-relative_delta_sigma_per_delta_t_range, relative_delta_sigma_per_delta_t_range) ;

			delta_dsigma_per_dt_graph->Fit("pol2", "", "", lower_abs_t_boundary_of_fit_GeV2, upper_abs_t_boundary_of_fit_GeV2) ;
			delta_dsigma_per_dt_graph->GetFunction("pol2")->SetLineColor(kBlue) ;

			TLine *line = new TLine(lower_abs_t_boundary_of_fit_GeV2, 0, upper_abs_t_boundary_of_fit_GeV2, 0) ;
			line->Draw() ;
			line->SetLineColor(kRed) ;

			AlignmentWarning() ;

			string id_non_exp_test_graph = id + "_non_exp_test_graph" ;
			delta_dsigma_per_dt_graph->SetName(id_non_exp_test_graph.c_str()) ;
			delta_dsigma_per_dt_graph->Write() ;

			SaveEpsRootLatex(id_non_exp_test_graph, canvas, true, 1.0, false, true) ;
		}


	}
}


#endif // #ifndef TAnalysisPlot1D_h
