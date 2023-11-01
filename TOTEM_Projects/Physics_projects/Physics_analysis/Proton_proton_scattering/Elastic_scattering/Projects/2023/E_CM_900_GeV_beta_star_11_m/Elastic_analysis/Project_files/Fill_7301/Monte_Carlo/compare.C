void compare()
{

	const int scenario_1_no_beam_divergence_no_vertex_single_mc = 1 ;
	
	int scenario = scenario_1_no_beam_divergence_no_vertex_single_mc ;
	
	if(scenario == scenario_1_no_beam_divergence_no_vertex_single_mc)
	{
	}
	else
	{
		cout << "There is no such scenario!" << endl ;
		exit(1) ;
	}


	TFile *myfile_reference = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc_reference/Generic.root") ;
	TFile *myfile_accep_corrected = TFile::Open("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc/Generic.root") ;
	
	TH1D *reference = (TH1D *)myfile_reference->Get("P0025_PlotsCollection_dN_dt_GeV2") ;
	TH1D *accep_corrected = (TH1D *)myfile_accep_corrected->Get("P0031_PlotsCollection_dN_dt_GeV2_geometrical_acceptance_corrected") ;
	
	cout << reference << endl ;
	cout << accep_corrected << endl ;
	
	TCanvas c ;
	
	reference->Draw() ;
	accep_corrected->Draw("same") ;
	
	c.SaveAs("plots/test_1.root") ;
	c.SaveAs("plots/test_1.pdf") ;
	
	accep_corrected->Divide(reference) ;

	accep_corrected->Draw("s") ;

	c.SaveAs("plots/test_2.root") ;
	c.SaveAs("plots/test_2.pdf") ;
	
	cout << "Finished" << endl ;
}
