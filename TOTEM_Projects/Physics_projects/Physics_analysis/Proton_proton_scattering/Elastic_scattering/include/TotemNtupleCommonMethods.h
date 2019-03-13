
TotemNtuple::TotemNtuple(string root_file_name, string input_directory, TTree *tree): root_file_name(root_file_name), input_directory(input_directory)
{

	string complete_filename = input_directory + root_file_name ;
	cout << complete_filename << endl ;

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(complete_filename.c_str());
      
      cout << root_file_name << endl ;

      TFile *f = TFile::Open(complete_filename.c_str()); 
      
      if (!f) {
         f = new TFile(complete_filename.c_str());
	 cout << "I am here" << endl ;
      }
      
      tree = (TTree*)gDirectory->Get("TotemNtuple");

   }
   Init(tree);

   left_bottom_right_top_branch_vectors = new TBranchVectors() ;
   left_top_right_bottom_branch_vectors = new TBranchVectors() ;

   horizontal_branch_vectors = new TBranchVectors() ;

   if((left_bottom_right_top_branch_vectors == NULL) || (left_top_right_bottom_branch_vectors == NULL) || (horizontal_branch_vectors == NULL))
   {
   	cout << "Error: TotemNtuple::TotemNtuple: Memory allocation for branch vectors failed ! " << endl ;
	exit(1) ;
   }
}

TotemNtuple::TotemNtuple(string root_file_name, string input_directory, int): root_file_name(root_file_name), input_directory(input_directory)
{

	string complete_filename = input_directory + root_file_name ;
	cout << complete_filename << endl ;

// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	{
      // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(complete_filename.c_str());
      
      cout << root_file_name << endl ;

      TFile *f = TFile::Open(complete_filename.c_str(), "RECREATE"); 
      
      if (!f) {
         f = new TFile(complete_filename.c_str());
	 cout << "I am here" << endl ;
      }
      
      // tree = (TTree*)gDirectory->Get("TotemNtuple");

   }

   TTree *tree = new TTree() ;

   Init(tree);

   left_bottom_right_top_branch_vectors = new TBranchVectors() ;
   left_top_right_bottom_branch_vectors = new TBranchVectors() ;

   horizontal_branch_vectors = new TBranchVectors() ;

   if((left_bottom_right_top_branch_vectors == NULL) || (left_top_right_bottom_branch_vectors == NULL) || (horizontal_branch_vectors == NULL))
   {
   	cout << "Error: TotemNtuple::TotemNtuple: Memory allocation for branch vectors failed ! " << endl ;
	exit(1) ;
   }
}


void TotemNtuple::TestNumberOfBrunches()
{
	if(!(left_bottom_right_top_branch_vectors->Test() && left_top_right_bottom_branch_vectors->Test() && horizontal_branch_vectors->Test()))
	{
		cout << "Error in  TotemNtuple::SetupBranches: number of bunches is not correct !" << endl ;
		exit(1) ;
	}
}

RP_struct_type TotemNtuple::GetAddress(string MADX_label, int detector_orientation)
{
	int length_of_MADX_label = MADX_label.length() ;

	string start_of_MADX_label = MADX_label.substr(0,4) ;

	if(start_of_MADX_label.compare("XRPV") != 0)
	{
		cout << "Error : TotemNtuple::GetAddress: the MADX labels of the near far detector should start with letters XRPV ! The present one is " << start_of_MADX_label << endl ;
		exit(1) ;
	}

	string beam_string = MADX_label.substr(length_of_MADX_label-3, length_of_MADX_label) ;

	TBranchVectors *branch_vectors = NULL ;

	if(beam_string.compare(".B1") == 0)		// right arm
	{
		if(detector_orientation == TConstants::TOP)
		{
			branch_vectors = left_bottom_right_top_branch_vectors ;
		}
		else if(detector_orientation == TConstants::BOTTOM)
		{
			branch_vectors = left_top_right_bottom_branch_vectors ;
		}
	}
	else if(beam_string.compare(".B2") == 0)	// left arm
	{
		if(detector_orientation == TConstants::TOP)
		{
			branch_vectors = left_top_right_bottom_branch_vectors ;
		}
		else if(detector_orientation == TConstants::BOTTOM)
		{
			branch_vectors = left_bottom_right_top_branch_vectors ;
		}
	}
	else
	{
		cout << "Error: TotemNtuple::GetAddress: wrong beam marker in MAD-X label !" << endl ;
		exit(1) ;
	}
	
	RP_struct_type RP_struct ;

	if(detector_orientation == TConstants::TOP)
	{
		if(MADX_label.compare("XRPV.A6R5.B1") == 0)				// verified
		{
			RP_struct.Pvalidity 	= &track_rp_120_valid ;
			RP_struct.Px 		= &track_rp_120_x ;
			RP_struct.Py 		= &track_rp_120_y ;
			RP_struct.Pthx 		= &track_rp_120_thx ;
			RP_struct.Pthy 		= &track_rp_120_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_120_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_120_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_120_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_120_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_120_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_120_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_120_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_120_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_120_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_120_vPlanesOn) ;

		}
		else if(MADX_label.compare("XRPV.B6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_124_valid ;
			RP_struct.Px 		= &track_rp_124_x ;
			RP_struct.Py 		= &track_rp_124_y ;
			RP_struct.Pthx 		= &track_rp_124_thx ;
			RP_struct.Pthy 		= &track_rp_124_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_124_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_124_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_124_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_124_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_124_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_124_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_124_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_124_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_124_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_124_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.A6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_20_valid ;
			RP_struct.Px 		= &track_rp_20_x ;
			RP_struct.Py 		= &track_rp_20_y ;
			RP_struct.Pthx 		= &track_rp_20_thx ;
			RP_struct.Pthy 		= &track_rp_20_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_20_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_20_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_20_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_20_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_20_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_20_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_20_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_20_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_20_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_20_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.B6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_24_valid ;
			RP_struct.Px 		= &track_rp_24_x ;
			RP_struct.Py 		= &track_rp_24_y ;
			RP_struct.Pthx 		= &track_rp_24_thx ;
			RP_struct.Pthy 		= &track_rp_24_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_24_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_24_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_24_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_24_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_24_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_24_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_24_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_24_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_24_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_24_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_104_valid ;
			RP_struct.Px 		= &track_rp_104_x ;
			RP_struct.Py 		= &track_rp_104_y ;
			RP_struct.Pthx 		= &track_rp_104_thx ;
			RP_struct.Pthy 		= &track_rp_104_thy ;
			RP_struct.PuPlanesOn	= &digi_rp_104_uPlanesOn ;
			RP_struct.PvPlanesOn	= &digi_rp_104_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_104_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_104_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_104_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_104_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_104_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_104_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_104_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_104_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_4_valid ;
			RP_struct.Px 		= &track_rp_4_x ;
			RP_struct.Py 		= &track_rp_4_y ;
			RP_struct.Pthx 		= &track_rp_4_thx ;
			RP_struct.Pthy 		= &track_rp_4_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_4_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_4_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_4_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_4_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_4_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_4_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_4_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_4_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_4_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_4_vPlanesOn) ;
		}
		else
		{
			cout << "Error in TotemNtuple::GetAddress: unknown MADX_label !: " << MADX_label <<  endl ;
			exit(1) ;
		}
	}
	else if(detector_orientation == TConstants::BOTTOM)
	{
		if(MADX_label.compare("XRPV.A6R5.B1") == 0)				// verified
		{
			RP_struct.Pvalidity 	= &track_rp_121_valid ;
			RP_struct.Px 		= &track_rp_121_x ;
			RP_struct.Py 		= &track_rp_121_y ;
			RP_struct.Pthx 		= &track_rp_121_thx ;
			RP_struct.Pthy 		= &track_rp_121_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_121_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_121_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_121_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_121_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_121_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_121_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_121_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_121_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_121_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_121_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.B6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_125_valid ;
			RP_struct.Px 		= &track_rp_125_x ;
			RP_struct.Py 		= &track_rp_125_y ;
			RP_struct.Pthx 		= &track_rp_125_thx ;
			RP_struct.Pthy 		= &track_rp_125_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_125_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_125_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_125_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_125_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_125_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_125_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_125_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_125_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_125_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_125_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.A6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_21_valid ;
			RP_struct.Px 		= &track_rp_21_x ;
			RP_struct.Py 		= &track_rp_21_y ;
			RP_struct.Pthx 		= &track_rp_21_thx ;
			RP_struct.Pthy 		= &track_rp_21_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_21_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_21_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_21_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_21_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_21_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_21_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_21_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_21_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_21_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_21_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.B6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_25_valid ;
			RP_struct.Px 		= &track_rp_25_x ;
			RP_struct.Py 		= &track_rp_25_y ;
			RP_struct.Pthx 		= &track_rp_25_thx ;
			RP_struct.Pthy 		= &track_rp_25_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_25_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_25_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_25_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_25_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_25_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_25_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_25_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_25_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_25_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_25_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_105_valid ;
			RP_struct.Px 		= &track_rp_105_x ;
			RP_struct.Py 		= &track_rp_105_y ;
			RP_struct.Pthx 		= &track_rp_105_thx ;
			RP_struct.Pthy 		= &track_rp_105_thy ;
			RP_struct.PuPlanesOn	= &digi_rp_105_uPlanesOn ;
			RP_struct.PvPlanesOn	= &digi_rp_105_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_105_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_105_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_105_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_105_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_105_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_105_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_105_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_105_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_5_valid ;
			RP_struct.Px 		= &track_rp_5_x ;
			RP_struct.Py 		= &track_rp_5_y ;
			RP_struct.Pthx 		= &track_rp_5_thx ;
			RP_struct.Pthy 		= &track_rp_5_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_5_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_5_vPlanesOn ;

			branch_vectors->validity_branches_to_get->insert(b_track_rp_5_valid) ;
			branch_vectors->validity_branches->insert(&track_rp_5_valid) ;
			branch_vectors->branches_to_get->insert(b_track_rp_5_x) ;
			branch_vectors->branches_to_get->insert(b_track_rp_5_y) ;
			branch_vectors->branches_to_get->insert(b_track_rp_5_thx) ;
			branch_vectors->branches_to_get->insert(b_track_rp_5_thy) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_5_uPlanesOn) ;
			branch_vectors->branches_to_get->insert(b_digi_rp_5_vPlanesOn) ;
		}
		else
		{
			cout << "Error in TotemNtuple::GetAddress: unknown MADX_label !:" << MADX_label << endl ;
			exit(1) ;
		}
	}
	else if(detector_orientation == TConstants::HORIZONTAL)
	{
		if(MADX_label.compare("XRPV.A6R5.B1") == 0)				// verified
		{
			RP_struct.Pvalidity 	= &track_rp_122_valid ;
			RP_struct.Px 		= &track_rp_122_x ;
			RP_struct.Py 		= &track_rp_122_y ;
			RP_struct.Pthx 		= &track_rp_122_thx ;
			RP_struct.Pthy 		= &track_rp_122_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_122_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_122_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_122_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_122_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_122_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_122_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_122_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_122_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_122_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_122_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.B6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_123_valid ;
			RP_struct.Px 		= &track_rp_123_x ;
			RP_struct.Py 		= &track_rp_123_y ;
			RP_struct.Pthx 		= &track_rp_123_thx ;
			RP_struct.Pthy 		= &track_rp_123_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_123_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_123_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_123_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_123_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_123_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_123_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_123_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_123_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_123_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_123_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.A6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_22_valid ;
			RP_struct.Px 		= &track_rp_22_x ;
			RP_struct.Py 		= &track_rp_22_y ;
			RP_struct.Pthx 		= &track_rp_22_thx ;
			RP_struct.Pthy 		= &track_rp_22_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_22_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_22_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_22_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_22_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_22_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_22_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_22_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_22_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_22_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_22_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.B6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_23_valid ;
			RP_struct.Px 		= &track_rp_23_x ;
			RP_struct.Py 		= &track_rp_23_y ;
			RP_struct.Pthx 		= &track_rp_23_thx ;
			RP_struct.Pthy 		= &track_rp_23_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_23_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_23_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_23_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_23_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_23_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_23_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_23_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_23_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_23_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_23_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6R5.B1") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_103_valid ;
			RP_struct.Px 		= &track_rp_103_x ;
			RP_struct.Py 		= &track_rp_103_y ;
			RP_struct.Pthx 		= &track_rp_103_thx ;
			RP_struct.Pthy 		= &track_rp_103_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_103_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_103_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_103_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_103_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_103_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_103_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_103_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_103_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_103_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_103_vPlanesOn) ;
		}
		else if(MADX_label.compare("XRPV.D6L5.B2") == 0)			// verified
		{
			RP_struct.Pvalidity 	= &track_rp_3_valid ;
			RP_struct.Px 		= &track_rp_3_x ;
			RP_struct.Py 		= &track_rp_3_y ;
			RP_struct.Pthx 		= &track_rp_3_thx ;
			RP_struct.Pthy 		= &track_rp_3_thy ;
			RP_struct.PuPlanesOn 	= &digi_rp_3_uPlanesOn ;
			RP_struct.PvPlanesOn 	= &digi_rp_3_vPlanesOn ;

			horizontal_branch_vectors->validity_branches_to_get->insert(b_track_rp_3_valid) ;
			horizontal_branch_vectors->validity_branches->insert(&track_rp_3_valid) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_3_x) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_3_y) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_3_thx) ;
			horizontal_branch_vectors->branches_to_get->insert(b_track_rp_3_thy) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_3_uPlanesOn) ;
			horizontal_branch_vectors->branches_to_get->insert(b_digi_rp_3_vPlanesOn) ;
		}
		else
		{
			cout << "Error in TotemNtuple::GetAddress: unknown MADX_label !" << MADX_label << endl ;
			exit(1) ;
		}
	}
	else
	{
		cout << "Error in TotemNtuple::GetAddress: unknown orientation in variable detector_orientation !" << endl ;
		exit(1) ;
	}

	return RP_struct ;
}

void TotemNtuple::SetupBranches(TTree *tree, TProjectParameters *ProjectParameters, int ntuple_tree)
{
	tree->Branch("event_info_timestamp", 	       	&event_info_timestamp,  	   "event_info_timestamp/l") ;  	   
	tree->Branch("trigger_data_run_num", 	       	&trigger_data_run_num,  	   "trigger_data_run_num/i") ;  	   
	tree->Branch("trigger_data_bunch_num", 	       	&trigger_data_bunch_num,	   "trigger_data_bunch_num/i") ;	   
	tree->Branch("trigger_data_event_num", 	       	&trigger_data_event_num,	   "trigger_data_event_num/i") ;	   
	tree->Branch("trigger_data_trigger_num",	&trigger_data_trigger_num,	   "trigger_data_trigger_num/i") ;	   
	tree->Branch("trigger_data_input_status_bits",	&trigger_data_input_status_bits,   "trigger_data_input_status_bits/i") ;

	int left_orientation = TConstants::NO_ORIENTATION ;

	RP_struct_type	left_near, left_far, right_near, right_far ;

	if(ntuple_tree == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP)
	{
		left_orientation = TConstants::BOTTOM ;
	}
	else if(ntuple_tree == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM)
	{
		left_orientation = TConstants::TOP ;
	}
	else
	{
			cout << "Error in  TotemNtuple::SetupBranches: unknown diagonal !" << endl ;
			exit(1) ;
	}

	int right_orientation = TConstants::NO_ORIENTATION ;
	
	if(left_orientation == TConstants::BOTTOM)
	{
		right_orientation = TConstants::TOP ;
	}
	else if(left_orientation == TConstants::TOP)
	{
		right_orientation = TConstants::BOTTOM ;
	}
	else
	{
			cout << "Error in  TotemNtuple::SetupBranches: unknown orientation in variable left_orientation !" << endl ;
			exit(1) ;
	}
	
	left_near	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector"),	left_orientation) ;
	left_far	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector"),	left_orientation) ;
	right_near	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector"),right_orientation) ;
	right_far	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector"),	right_orientation) ;

	tree->Branch("track_left_near_valid", 		left_near.Pvalidity,         	"track_left_near_valid/O") ;			       
	tree->Branch("track_left_near_x", 		left_near.Px,     		"track_left_near_x/D") ;
	tree->Branch("track_left_near_y", 		left_near.Py,			"track_left_near_y/D") ;
	tree->Branch("track_left_near_thx", 		left_near.Pthx,			"track_left_near_thx/D") ;
	tree->Branch("track_left_near_thy", 		left_near.Pthy,			"track_left_near_thy/D") ;
	tree->Branch("track_left_near_uPlanesOn", 	left_near.PuPlanesOn,		"track_left_near_uPlanesOn/i") ;
	tree->Branch("track_left_near_vPlanesOn",	left_near.PvPlanesOn,		"track_left_near_vPlanesOn/i") ;

	tree->Branch("track_left_far_valid", 		left_far.Pvalidity,         	"track_left_far_valid/O") ;
	tree->Branch("track_left_far_x", 		left_far.Px,     		"track_left_far_x/D") ;
	tree->Branch("track_left_far_y", 		left_far.Py,			"track_left_far_y/D") ;
	tree->Branch("track_left_far_thx", 		left_far.Pthx,     		"track_left_far_thx/D") ;
	tree->Branch("track_left_far_thy", 		left_far.Pthy,			"track_left_far_thy/D") ;
	tree->Branch("track_left_far_uPlanesOn", 	left_far.PuPlanesOn,    	"track_left_far_uPlanesOn/i") ;
	tree->Branch("track_left_far_vPlanesOn", 	left_far.PvPlanesOn,		"track_left_far_vPlanesOn/i") ;

	tree->Branch("track_right_near_valid", 		right_near.Pvalidity,         	"track_right_near_valid/O") ;
	tree->Branch("track_right_near_x", 		right_near.Px,     		"track_right_near_x/D") ;
	tree->Branch("track_right_near_y", 		right_near.Py,			"track_right_near_y/D") ;
	tree->Branch("track_right_near_thx", 		right_near.Pthx,     		"track_right_near_thx/D") ;
	tree->Branch("track_right_near_thy", 		right_near.Pthy,		"track_right_near_thy/D") ;
	tree->Branch("track_right_near_uPlanesOn", 	right_near.PuPlanesOn,  	"track_right_near_uPlanesOn/i") ;
	tree->Branch("track_right_near_vPlanesOn",	right_near.PvPlanesOn,		"track_right_near_vPlanesOn/i") ;

	tree->Branch("track_right_far_valid", 		right_far.Pvalidity,         	"track_right_far_valid/O") ;
	tree->Branch("track_right_far_x", 		right_far.Px,     		"track_right_far_x/D") ;
	tree->Branch("track_right_far_y", 		right_far.Py,			"track_right_far_y/D") ;
	tree->Branch("track_right_far_thx", 		right_far.Pthx,     		"track_right_far_thx/D") ;
	tree->Branch("track_right_far_thy", 		right_far.Pthy,			"track_right_far_thy/D") ;
	tree->Branch("track_right_far_uPlanesOn", 	right_far.PuPlanesOn,   	"track_right_far_uPlanesOn/i") ;
	tree->Branch("track_right_far_vPlanesOn", 	right_far.PvPlanesOn,		"track_right_far_vPlanesOn/i") ;

	RP_struct_type	left_near_horizontal	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector"),	TConstants::HORIZONTAL) ;
	RP_struct_type	left_far_horizontal	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector"),	TConstants::HORIZONTAL) ;
	RP_struct_type	right_near_horizontal	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector"),TConstants::HORIZONTAL) ;
	RP_struct_type	right_far_horizontal	= GetAddress(ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector"),	TConstants::HORIZONTAL) ;

	tree->Branch("track_left_near_horizontal_valid", 	left_near_horizontal.Pvalidity,         "track_left_near_horizontal_valid/O") ;
	tree->Branch("track_left_near_horizontal_x", 		left_near_horizontal.Px,     		"track_left_near_horizontal_x/D") ;
	tree->Branch("track_left_near_horizontal_y", 		left_near_horizontal.Py,		"track_left_near_horizontal_y/D") ;
	tree->Branch("track_left_near_horizontal_thx", 		left_near_horizontal.Pthx,     		"track_left_near_horizontal_thx/D") ;
	tree->Branch("track_left_near_horizontal_thy", 		left_near_horizontal.Pthy,		"track_left_near_horizontal_thy/D") ;

	tree->Branch("track_left_far_horizontal_valid", 	left_far_horizontal.Pvalidity,         	"track_left_far_horizontal_valid/O") ;
	tree->Branch("track_left_far_horizontal_x", 		left_far_horizontal.Px,     		"track_left_far_horizontal_x/D") ;
	tree->Branch("track_left_far_horizontal_y", 		left_far_horizontal.Py,			"track_left_far_horizontal_y/D") ;
	tree->Branch("track_left_far_horizontal_thx", 		left_far_horizontal.Pthx,     		"track_left_far_horizontal_thx/D") ;
	tree->Branch("track_left_far_horizontal_thy", 		left_far_horizontal.Pthy,		"track_left_far_horizontal_thy/D") ;

	tree->Branch("track_right_near_horizontal_valid", 	right_near_horizontal.Pvalidity,        "track_right_near_horizontal_valid/O") ;
	tree->Branch("track_right_near_horizontal_x", 		right_near_horizontal.Px,     		"track_right_near_horizontal_x/D") ;
	tree->Branch("track_right_near_horizontal_y", 		right_near_horizontal.Py,		"track_right_near_horizontal_y/D") ;
	tree->Branch("track_right_near_horizontal_thx",		right_near_horizontal.Pthx,		"track_right_near_horizontal_thx/D") ;
	tree->Branch("track_right_near_horizontal_thy",		right_near_horizontal.Pthy,		"track_right_near_horizontal_thy/D") ;

	tree->Branch("track_right_far_horizontal_valid", 	right_far_horizontal.Pvalidity,         "track_right_far_horizontal_valid/O") ;
	tree->Branch("track_right_far_horizontal_x", 		right_far_horizontal.Px,     		"track_right_far_horizontal_x/D") ;
	tree->Branch("track_right_far_horizontal_y", 		right_far_horizontal.Py,		"track_right_far_horizontal_y/D") ;
	tree->Branch("track_right_far_horizontal_thx", 		right_far_horizontal.Pthx,     		"track_right_far_horizontal_thx/D") ;
	tree->Branch("track_right_far_horizontal_thy", 		right_far_horizontal.Pthy,		"track_right_far_horizontal_thy/D") ;

	cout << "************* Begin : test of branch vectors ***********" << endl ;

	cout << "left_bottom_right_top_branch_vectors" << endl ;
	left_bottom_right_top_branch_vectors->Print() ;
	cout << endl ;

	cout << "left_top_right_bottom_branch_vectors" << endl ;
	left_top_right_bottom_branch_vectors->Print() ;
	cout << endl ;

	cout << "horizontal_branch_vectors" << endl ;
	horizontal_branch_vectors->Print() ;

	cout << "************* End : test of branch vectors ***********" << endl ;

}
