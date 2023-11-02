void compare()
{

	const int scenario_1_no_beam_divergence_no_vertex_single_mc = 1 ;
	
	int scenario = scenario_1_no_beam_divergence_no_vertex_single_mc ;
	
	string filename1 = "" ;
	string filename2 = "" ;
	
	string histname1 = "" ;
	string histname2 = "" ;
	
	if(scenario == scenario_1_no_beam_divergence_no_vertex_single_mc)
	{
		filename1 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc_reference/Generic.root"" ;
		filename2 = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_11_m/Analysis_output_files/7301/Diagonals/DIAGONAL_LEFT_BOTTOM_RIGHT_TOP/All_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_mc/Generic.root" ;
		
		histname1= "P0025_PlotsCollection_dN_dt_GeV2" ;
		histname2= "P0031_PlotsCollection_dN_dt_GeV2_geometrical_acceptance_corrected" ;
	}
	else
	{
		cout << "There is no such scenario!" << endl ;
		exit(1) ;
	}


	TFile *myfile_reference = TFile::Open(filename1.c_str()) ;
	TFile *myfile_accep_corrected = TFile::Open(filename2.c_str()) ;
	
	TH1D *reference = (TH1D *)myfile_reference->Get(histname1.c_str()) ;
	TH1D *accep_corrected = (TH1D *)myfile_accep_corrected->Get(histname2.c_str()) ;
	
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
