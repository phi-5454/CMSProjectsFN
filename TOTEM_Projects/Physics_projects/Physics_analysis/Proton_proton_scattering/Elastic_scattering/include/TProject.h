#ifndef TProject_h
#define TProject_h

#include <sys/time.h>
#include <sys/resource.h>

#include "TMinuit.h"

#include "TAnalysisObject.h"
#include "TRPAlignment.h"
#include "TProjectParameters.h"
#include "TLuminosity.h"
#include "TPlotsCollection.h"
#include "TBeamOptics.h"
#include "TProtonReconstruction.h"

#include "TMonteCarlo.h"
#include "TRandom3.h"
#include "TFrame.h"
#include "TGraphErrors.h"

#include <fstream>

#define TotemNtuple_cxx
#define TReducedNtuple_cxx
#define TotemNtuple_CTPPS_2016_cxx

#include "include/TBranchVectors.h"

// #include "TotemNtuple_LHC_Run_II.h"
#include "TotemNtuple.h"
#include "TotemNtuple_CTPPS_2016.h"
#include "TReducedNtuple.h"

#include "include/TotemNtupleCommonMethods.h"

class TProject : TAnalysisObject
{
	TFile *generic_file ;
	map<UInt_t, ULong64_t> timestamps ;

	TProjectParameters *ProjectParameters ;
	TProtonReconstruction *ProtonReconstruction ;
	
	TLuminosity *luminosity ;
	map_from_string_to_PTPlotsCollection_type PlotsCollections ;

	public:
	
	TProject(string, string, string) ;
	virtual ~TProject() {} ;

	int 	Execute() ;
	void 	ExecuteMonteCarlo() ;
	void 	InitMonteCarlo(double &, double &, double &, double &, double &, double &, double &, double &) ;
	string 	GenerateOutputFilenameWithPath(int, string) ;
	void 	CreatePlotsCollections() ;
	void 	FindCuts(TReducedNtuple *, string, TProjectInformation *, int) ;
	void 	TestBeamProperties() ;
	void 	SetupAcceptanceCorrections() ;
	string 	GetDetectorTopology(string) ;
	void 	CollectDependencies(string, set<string> *) ;

	void 	MergeAndPostProcess() ;
	void 	CalculateSystematicErrors() ;
	void 	CalculateOverallNormalization(TH1D *, double) ;

	map_from_string_to_PTPlotsCollection_type *GetPPlotsCollections() { return &PlotsCollections ; } ;

	void SaveAdditionalInformationToRootFile(TFile *, TProjectInformation *) ;
	void CollectDependenciesOfDetector(string, set<string> *) ;
} ;

double *DEBUG_1 = NULL ;

TProject::TProject(string name, string id, string config_or_project_filename) : TAnalysisObject(name, id), luminosity(NULL), generic_file(NULL), ProtonReconstruction(NULL)
{
	ProjectParameters = new TProjectParameters("ProjectParameters", "ProjectParameters", config_or_project_filename) ;

	cout << "TProject: ProjectParameters have been read in." << endl ;

	string install_directory_of_code = "" ;
	if(ProjectParameters->IsSettingDefined(TConstants::SETTING_INSTALLDIRECTORYOFCODE)) install_directory_of_code = ProjectParameters->GetSettingValue(TConstants::SETTING_INSTALLDIRECTORYOFCODE) + "/" ;

	// Load optics files from .tfs

	if(ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0)
	{
		if((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("find_cuts") == 0) ||
		(ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("diffraction") == 0))
		{
			// Optics initialization

			bool xi_dependent = false ;

			if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("diffraction") == 0.0)) xi_dependent = true ;

			if(xi_dependent) cout << "TProject: Optics is xi dependent for MC." << endl ;
			cout << "TProject: Load optics..." << endl ;

			TBeamOptics *BeamOptics_Beam_1 = new TBeamOptics("BeamOptics_Beam_1", "BeamOptics_Beam_1", install_directory_of_code + ProjectParameters->GetSettingValue("MAD_X_twiss_beam_1"), ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector"),
									ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector"), xi_dependent) ;

			TBeamOptics *BeamOptics_Beam_2 = new TBeamOptics("BeamOptics_Beam_2", "BeamOptics_Beam_2", install_directory_of_code + ProjectParameters->GetSettingValue("MAD_X_twiss_beam_2"), ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector"),
									ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector"), xi_dependent) ;

			cout << "TProject: Optics loaded." << endl ;

			ProjectParameters->SetBeam_1_Optics(BeamOptics_Beam_1) ;
			ProjectParameters->SetBeam_2_Optics(BeamOptics_Beam_2) ;

			BeamOptics_Beam_1->SetLatexFile(ProjectParameters->GetPLatexFile()) ;
			BeamOptics_Beam_2->SetLatexFile(ProjectParameters->GetPLatexFile()) ;

			TestBeamProperties() ;

			// Optics is loaded

			if(ProjectParameters->GetSettingValue("project_subtask").compare("diffraction") != 0) SetupAcceptanceCorrections() ;

			if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_CMS_luminosity_calculations") == 1.0) luminosity = new TLuminosity("CMS_luminosity", "CMS_luminosity", ProjectParameters) ;

			// Alignment

			if(ProjectParameters->IsBlockDefined("alignment_block"))
			{
				TProjectParameters *alignment_block = ProjectParameters->GetBlock("alignment_block") ;

				TRPAlignment *RPAlignment = new TRPAlignment("RPAlignment", "RPAlignment",
					alignment_block->GetParameterValue("RP_alignment_left_far__x_mm"),
					alignment_block->GetParameterValue("RP_alignment_left_near_x_mm"),
					alignment_block->GetParameterValue("RP_alignment_right_far__x_mm"),
					alignment_block->GetParameterValue("RP_alignment_right_near_x_mm"),
					alignment_block->GetParameterValue("RP_alignment_left_far__y_mm"),
					alignment_block->GetParameterValue("RP_alignment_left_near_y_mm"),
					alignment_block->GetParameterValue("RP_alignment_right_far__y_mm"),
					alignment_block->GetParameterValue("RP_alignment_right_near_y_mm")) ;

				ProjectParameters->SetRPAlignment(RPAlignment) ;
			}

		}
		else if(ProjectParameters->GetSettingValue("project_subtask").compare("merge_and_post_process") == 0)
		{
		}
		else
		{
			cout << "TProject::TProject, unknown project subtask ! " << ProjectParameters->GetSettingValue("project_subtask") << endl ;
			exit(1) ;
		}
	}
	else if(ProjectParameters->GetSettingValue("project_task").compare("data_reduction") == 0)
	{
		if(ProjectParameters->GetSettingValue("project_subtask").compare("elastic") == 0)
		{
		}
	}	
	else
	{
		cout << "TProject::TProject, unknown project task ! " << ProjectParameters->GetSettingValue("project_task") << endl ;
		exit(1) ;
	}
} ;

void TProject::SetupAcceptanceCorrections()
{
	
	if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("diffraction") == 0.0))
	{
	}
	else
	{
		double theta_y_star_rad_min = 0 ;
		double theta_y_star_rad_max = 0 ;

		// Vertical beam divergence correction parameters

		double theta_y_star_rad_left_cut_edge_plus = 0 ;
		double theta_y_star_rad_left_cut_edge_minus = 0 ;

		double theta_y_star_rad_right_cut_edge_plus = 0 ;
		double theta_y_star_rad_right_cut_edge_minus = 0 ;

		double cut_p0 = 0 ;
		double cut_p1 = 0 ;

		double effective_beam_divergence = 0 ;

		// Horizontal beam divergence correction parameters

		double theta_x_star_rad_left_cut_edge_plus = 0 ;
		double theta_x_star_rad_left_cut_edge_minus = 0 ;

		double theta_x_star_rad_right_cut_edge_plus = 0 ;
		double theta_x_star_rad_right_cut_edge_minus = 0 ;

		double cut_p0_on_theta_x_star_plane = 0 ;
		double cut_p1_on_theta_x_star_plane = 0 ;

		double horizontal_beam_divergence = 0 ;


		string cut_block_name = ProjectParameters->GetSettingValue("geometrical_acceptance_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			theta_y_star_rad_min = fabs(cut_block->GetParameterValue("y1")) - cut_block->GetParameterValue("sigma") ;
			theta_y_star_rad_max = fabs(cut_block->GetParameterValue("y1")) + cut_block->GetParameterValue("sigma") ;

			if(cut_block->GetParameterValue("number_of_sigmas") != 1)
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
				exit(1) ;
			}

			if(cut_block->GetParameterValue("y1") != cut_block->GetParameterValue("y2"))
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut y1 and y2 has to be equal ! " << endl ;
				exit(1) ;
			}

			if(theta_y_star_rad_min > theta_y_star_rad_max)
			{
				cout << "Error: TProject::TProject: theta_y_star_rad_min > theta_y_star_rad_max ! " << endl ;
				exit(1) ;
			}

		}
		else
		{
			cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}


		cut_block_name = ProjectParameters->GetSettingValue("vertical_angular_left_acceptance_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			if(cut_block->GetParameterValue("number_of_sigmas") != 1)
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
				exit(1) ;
			}

			if(cut_block->GetParameterValue("x1") != cut_block->GetParameterValue("x2"))
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut x1 and x2 has to be equal ! " << endl ;
				exit(1) ;
			}

			theta_y_star_rad_left_cut_edge_plus = (cut_block->GetParameterValue("x1") + cut_block->GetParameterValue("sigma")) ;
			theta_y_star_rad_left_cut_edge_minus = (cut_block->GetParameterValue("x1") - cut_block->GetParameterValue("sigma")) ;
		}
		else
		{
			cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = ProjectParameters->GetSettingValue("vertical_angular_right_acceptance_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			if(cut_block->GetParameterValue("number_of_sigmas") != 1)
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
				exit(1) ;
			}

			if(cut_block->GetParameterValue("y1") != cut_block->GetParameterValue("y2"))
			{
				cout << "Error: TProject::TProject: in case of an acceptance cut y1 and y2 has to be equal ! " << endl ;
				exit(1) ;
			}

			theta_y_star_rad_right_cut_edge_plus = (cut_block->GetParameterValue("y1") + cut_block->GetParameterValue("sigma")) ;
			theta_y_star_rad_right_cut_edge_minus = (cut_block->GetParameterValue("y1") - cut_block->GetParameterValue("sigma")) ;

		}
		else
		{
			cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = ProjectParameters->GetSettingValue("vertical_beam_divergence_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			double x1 = cut_block->GetParameterValue("x1") ;
			double y1 = cut_block->GetParameterValue("y1") ;
			double x2 = cut_block->GetParameterValue("x2") ;
			double y2 = cut_block->GetParameterValue("y2") ;

			cut_p1 = ((y2 - y1) / (x2 - x1)) ;
			cut_p0 = y1 - cut_p1 * x1 ;

			effective_beam_divergence = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}
		
		double theta_x_star_rad_cut_left = 0 ;
		double theta_x_star_rad_cut_right = 0 ;

		if(ProjectParameters->IsSettingDefined("horizontal_geometrical_acceptance_cut_block"))
		{
			cut_block_name = ProjectParameters->GetSettingValue("horizontal_geometrical_acceptance_cut_block") ;

			if(ProjectParameters->IsBlockDefined(cut_block_name))
			{
				TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

				if(cut_block->GetParameterValue("number_of_sigmas") != 1)
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
					exit(1) ;
				}

				if(cut_block->GetParameterValue("x1") != cut_block->GetParameterValue("x2"))
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut x1 and x2 has to be equal ! " << endl ;
					exit(1) ;
				}

				theta_x_star_rad_cut_left  = cut_block->GetParameterValue("x1") - cut_block->GetParameterValue("sigma") ;
				theta_x_star_rad_cut_right = cut_block->GetParameterValue("x1") + cut_block->GetParameterValue("sigma") ;


			}
			else
			{
				cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
				exit(1) ;
			}
		}


		double analysed_diagonal = 0 ;
		double opposite_diagonal = 0 ;
		
		if(ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0)
		{
			analysed_diagonal = TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP ;
			opposite_diagonal = TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM ;
		}
		else if(ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0)
		{
			analysed_diagonal = TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM ;
			opposite_diagonal = TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP ;
		}
		else
		{
			cout << "Error: TProject::SetupAcceptanceCorrections: Unknown diagonal !" << endl ;
			exit(1) ;
		}

		if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
		{
			// Due to sign conventions in case of 3/4 the diagonal definition ONLY for the acceptance has to be inverted see below

			if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) analysed_diagonal = opposite_diagonal ;
			if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) analysed_diagonal = opposite_diagonal ;
		}
		
		// =========================================================
		// Start of horizontal beam divergence correction parameters
		// =========================================================

		if(ProjectParameters->IsSettingDefined("horizontal_beam_divergence_cut_block"))
		{

			cut_block_name = ProjectParameters->GetSettingValue("horizontal_angular_left_acceptance_cut_block") ;

			if(ProjectParameters->IsBlockDefined(cut_block_name))
			{
				TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

				if(cut_block->GetParameterValue("number_of_sigmas") != 1)
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
					exit(1) ;
				}

				if(cut_block->GetParameterValue("x1") != cut_block->GetParameterValue("x2"))
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut x1 and x2 has to be equal ! " << endl ;
					exit(1) ;
				}

				theta_x_star_rad_left_cut_edge_plus = (cut_block->GetParameterValue("x1") + cut_block->GetParameterValue("sigma")) ;
				theta_x_star_rad_left_cut_edge_minus = (cut_block->GetParameterValue("x1") - cut_block->GetParameterValue("sigma")) ;
			}
			else
			{
				cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
				exit(1) ;
			}

			cut_block_name = ProjectParameters->GetSettingValue("horizontal_angular_right_acceptance_cut_block") ;

			if(ProjectParameters->IsBlockDefined(cut_block_name))
			{
				TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

				if(cut_block->GetParameterValue("number_of_sigmas") != 1)
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut the number_of_sigmas must be 1 ! " << endl ;
					exit(1) ;
				}

				if(cut_block->GetParameterValue("y1") != cut_block->GetParameterValue("y2"))
				{
					cout << "Error: TProject::TProject: in case of an acceptance cut y1 and y2 has to be equal ! " << endl ;
					exit(1) ;
				}

				theta_x_star_rad_right_cut_edge_plus = (cut_block->GetParameterValue("y1") + cut_block->GetParameterValue("sigma")) ;
				theta_x_star_rad_right_cut_edge_minus = (cut_block->GetParameterValue("y1") - cut_block->GetParameterValue("sigma")) ;

			}
			else
			{
				cout << "Error: TProject::TProject: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
				exit(1) ;
			}
		}
		
		// =========================================================
		// End of horizontal beam divergence correction parameters
		// =========================================================

		ProjectParameters->Set_preliminary_acceptance_beam_1(new TAcceptance("preliminary_acceptance_beam_1", "preliminary_acceptance_beam_1", ProjectParameters->GetSettingValue("Output_directory"),
				theta_y_star_rad_min, theta_y_star_rad_max,
				ProjectParameters->GetBeam_1_Optics()->GetBeamEnergyGeV(),
				theta_y_star_rad_left_cut_edge_plus,  theta_y_star_rad_left_cut_edge_minus,
				theta_y_star_rad_right_cut_edge_plus, theta_y_star_rad_right_cut_edge_minus,
				cut_p0, cut_p1, analysed_diagonal, effective_beam_divergence, theta_x_star_rad_cut_left, theta_x_star_rad_cut_right,
				theta_x_star_rad_left_cut_edge_plus,  theta_x_star_rad_left_cut_edge_minus,
				theta_x_star_rad_right_cut_edge_plus, theta_x_star_rad_right_cut_edge_minus)) ;

	}

}

string TProject::GetDetectorTopology(string detector_name)
{
	TConstants *constants = new TConstants() ;

	if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare(detector_name) == 0.0)
	{
		return (constants->ConvertOrientationCodeToString(TConstants::LEFT) + "_" + constants->ConvertOrientationCodeToString(TConstants::NEAR)) ;
	}
	else if(ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector").compare(detector_name) == 0.0)
	{
		return (constants->ConvertOrientationCodeToString(TConstants::LEFT) + "_" + constants->ConvertOrientationCodeToString(TConstants::FAR)) ;
	}
	else if(ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector").compare(detector_name) == 0.0)
	{
		return (constants->ConvertOrientationCodeToString(TConstants::RIGHT) + "_" + constants->ConvertOrientationCodeToString(TConstants::NEAR)) ;
	}
	else if(ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector").compare(detector_name) == 0.0)
	{
		return (constants->ConvertOrientationCodeToString(TConstants::RIGHT) + "_" + constants->ConvertOrientationCodeToString(TConstants::FAR)) ;
	}
	else
	{
		cout << "Error: TProject::GetDetectorTopology: the detector name doest not appear in topology !" << detector_name << endl ;
		exit(1) ;
	}
}

void TProject::CollectDependencies(string variable_name, set<string> *list_of_excluded_variables)
{
	const string postfix_of_variables = "_variable_block" ;
	vector<string> *list_of_block_names = new vector<string> ;

	ProjectParameters->GetBlockNamesWithSetting("depends_on_variable", variable_name, list_of_block_names) ;

	for(vector<string>::iterator it = list_of_block_names->begin(); it != list_of_block_names->end(); ++it)
	{
		if(it->find("_variable_block") == string::npos)
		{
			cout << "Error: TProject::CollectDependencies: a not variable block \"" + (*it) +  "\" depends on variable: " + variable_name << endl ;
			exit(1) ;
		}

		string name_of_dependent_variable = (it->substr(0, ((int)it->size() - (int)(postfix_of_variables.size())))) ;
		
		list_of_excluded_variables->insert(name_of_dependent_variable) ;

		CollectDependencies(name_of_dependent_variable, list_of_excluded_variables) ;

		// cout << "CollectDependencies " << name_of_dependent_variable << endl ;
	}

	delete list_of_block_names ;
}

void TProject::CollectDependenciesOfDetector(string detector_name, set<string> *list_of_excluded_variables)
{

	string block_name = "Inefficiency_3_out_of_4_RP_excluded_variables_" + GetDetectorTopology(detector_name) ;

	TProjectParameters *excluded_variables_block = ProjectParameters->GetBlock(block_name) ;

	for(vector<string>::iterator it = excluded_variables_block->GetSettingsVector("variable_name")->begin() ; it != excluded_variables_block->GetSettingsVector("variable_name")->end() ; ++it)
	{
		set<string>::iterator it_find = list_of_excluded_variables->find(*it) ;

		if(it_find ==  list_of_excluded_variables->end()) list_of_excluded_variables->insert(*it) ;
		CollectDependencies(*it, list_of_excluded_variables) ;
	}

	cout << endl << "Excluded variables :" << endl ;

	for(set<string> ::iterator it = list_of_excluded_variables->begin() ; it != list_of_excluded_variables->end() ; ++it) cout << (*it) <<endl ;

	cout << endl <<  endl ;
}

void TProject::TestBeamProperties()
{
	double near_far_distance_m_beam_1 = ProjectParameters->GetBeam_1_Optics()->GetNearFarDistanceM() ;
	double near_far_distance_m_beam_2 = ProjectParameters->GetBeam_2_Optics()->GetNearFarDistanceM() ;

	if((near_far_distance_m_beam_1 - near_far_distance_m_beam_2) > 1e-12)
	{
		cout << "Error: TProject::TProject: near_far_distance_m_beam_1 != near_far_distance_m_beam_2 !" << endl ;
		// exit(1) ;
	}

	if(fabs(near_far_distance_m_beam_1 - ProjectParameters->GetParameterValue("NearFarRPUnitsDistanceM")) > 1e-7)
	{
		cout << "Error: TProject::TProject: near_far_distance_m_beam_1 from optics disagree with validation !" << (near_far_distance_m_beam_1 - ProjectParameters->GetParameterValue("NearFarRPUnitsDistanceM") ) <<  endl ;
		exit(1) ;
	}

	ProjectParameters->GetBlockNamesWithSetting("depends_on_variable") ;

	set<string> *list_of_excluded_variables = NULL ;

	if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
	{
		list_of_excluded_variables = new set<string> ;
		CollectDependenciesOfDetector(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP"), list_of_excluded_variables) ;
	}
	else if(ProjectParameters->IsSettingDefined("Inefficiency_2_out_of_4_RP_far"))
	{
		list_of_excluded_variables = new set<string> ;
		CollectDependenciesOfDetector(ProjectParameters->GetSettingValue("Inefficiency_2_out_of_4_RP_far"), 	list_of_excluded_variables) ;
		CollectDependenciesOfDetector(ProjectParameters->GetSettingValue("Inefficiency_2_out_of_4_RP_near"), 	list_of_excluded_variables) ;
	}

	bool align_source = false ;
	if(ProjectParameters->IsSettingDefined("align_source_files"))
	{
		if(ProjectParameters->GetSettingValue("align_source_files").compare("yes") == 0)
		{
			align_source = true ;
		}
	}

        bool use_vertex_for_theta_x_star = true ;

        if(ProjectParameters->IsSettingDefined("use_vertex_for_theta_x_star"))
        {
                if(ProjectParameters->GetSettingValue("use_vertex_for_theta_x_star").compare("no") == 0)
                {
                        use_vertex_for_theta_x_star = false ;
                }
        }

	ProtonReconstruction = new TProtonReconstruction(near_far_distance_m_beam_1, near_far_distance_m_beam_2, list_of_excluded_variables, align_source, use_vertex_for_theta_x_star) ;
}

void TProject::CreatePlotsCollections()
{
	PlotsCollections["PlotsCollection"] = new TPlotsCollection("PlotsCollection", "PlotsCollection", ProjectParameters, luminosity, ProtonReconstruction) ;
}

TProtonReconstruction TotemNtuple::ProtonReconstruction(Long64_t ientry, int diagonal, TProtonReconstruction *ProtonReconstruction, map_from_string_to_PTPlotsCollection_type *PlotsCollections, TBeamOptics *BeamOptics_Beam_1, TBeamOptics *BeamOptics_Beam_2, TProjectParameters *ProjectParameters)
{
	double zero = 0 ;

	if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare("XRPV.A6L5.B2") == 0)
	{
		if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP)
		{

			// x coordinate

			b_track_rp_21_x->GetEntry(ientry);
			b_track_rp_25_x->GetEntry(ientry);

			b_track_rp_120_x->GetEntry(ientry);
			b_track_rp_124_x->GetEntry(ientry);

			// y coordinate

			b_track_rp_21_y->GetEntry(ientry);
			b_track_rp_25_y->GetEntry(ientry);

			b_track_rp_120_y->GetEntry(ientry);
			b_track_rp_124_y->GetEntry(ientry);

			ProtonReconstruction->Reconstruct(event_info_timestamp, trigger_data_run_num, trigger_data_event_num,
				track_rp_21_x, track_rp_25_x, track_rp_120_x, track_rp_124_x, track_rp_21_y, track_rp_25_y, track_rp_120_y, track_rp_124_y,
				zero, zero, zero, zero, zero, zero, zero, zero, 
				BeamOptics_Beam_1, BeamOptics_Beam_2, ProjectParameters->GetRPAlignment(), 4) ;

			// cout << DIAGONAL_LEFT_BOTTOM_RIGHT_TOP << endl ;

		}
		else if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM)
		{

			// x coordinate

			b_track_rp_20_x->GetEntry(ientry);
			b_track_rp_24_x->GetEntry(ientry);

			b_track_rp_121_x->GetEntry(ientry);
			b_track_rp_125_x->GetEntry(ientry);

			// y coordinate

			b_track_rp_20_y->GetEntry(ientry);
			b_track_rp_24_y->GetEntry(ientry);

			b_track_rp_121_y->GetEntry(ientry);
			b_track_rp_125_y->GetEntry(ientry);

			ProtonReconstruction->Reconstruct(event_info_timestamp, trigger_data_run_num, trigger_data_event_num,
				track_rp_20_x, track_rp_24_x, track_rp_121_x, track_rp_125_x, track_rp_20_y, track_rp_24_y, track_rp_121_y, track_rp_125_y,
				zero, zero, zero, zero, zero, zero, zero, zero, 
				BeamOptics_Beam_1, BeamOptics_Beam_2, ProjectParameters->GetRPAlignment(), 4) ;

			// cout << DIAGONAL_LEFT_TOP_RIGHT_BOTTOM << endl ;

		}
		else
		{
			cout << "Wrong diagonal code !" << endl ;
			exit(1) ;
		}
	}
	else if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare("XRPV.D6L5.B2") == 0)
	{
		if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP)
		{

			// x coordinate

			b_track_rp_5_x->GetEntry(ientry);
			b_track_rp_25_x->GetEntry(ientry);

			b_track_rp_104_x->GetEntry(ientry);
			b_track_rp_124_x->GetEntry(ientry);

			// y coordinate

			b_track_rp_5_y->GetEntry(ientry);
			b_track_rp_25_y->GetEntry(ientry);

			b_track_rp_104_y->GetEntry(ientry);
			b_track_rp_124_y->GetEntry(ientry);

			ProtonReconstruction->Reconstruct(event_info_timestamp, trigger_data_run_num, trigger_data_event_num,
				track_rp_5_x, track_rp_25_x, track_rp_104_x, track_rp_124_x, track_rp_5_y, track_rp_25_y, track_rp_104_y, track_rp_124_y,
				zero, zero, zero, zero, zero, zero, zero, zero, 
				BeamOptics_Beam_1, BeamOptics_Beam_2, ProjectParameters->GetRPAlignment(), 4) ;

			// cout << DIAGONAL_LEFT_BOTTOM_RIGHT_TOP << endl ;

		}
		else if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM)
		{

			// x coordinate

			b_track_rp_20_x->GetEntry(ientry);
			b_track_rp_4_x->GetEntry(ientry);

			b_track_rp_105_x->GetEntry(ientry);
			b_track_rp_125_x->GetEntry(ientry);

			// y coordinate

			b_track_rp_20_y->GetEntry(ientry);
			b_track_rp_4_y->GetEntry(ientry);

			b_track_rp_105_y->GetEntry(ientry);
			b_track_rp_125_y->GetEntry(ientry);

			ProtonReconstruction->Reconstruct(event_info_timestamp, trigger_data_run_num, trigger_data_event_num,
				track_rp_20_x, track_rp_4_x, track_rp_105_x, track_rp_125_x, track_rp_20_y, track_rp_4_y, track_rp_105_y, track_rp_125_y,
				zero, zero, zero, zero, zero, zero, zero, zero, 
				BeamOptics_Beam_1, BeamOptics_Beam_2, ProjectParameters->GetRPAlignment(), 4) ;

			// cout << DIAGONAL_LEFT_TOP_RIGHT_BOTTOM << endl ;

		}
		else
		{
			cout << "Wrong diagonal code !" << endl ;
			exit(1) ;
		}
	}
	else
	{
		cout << "Wrong configuration: TotemNtuple::ProtonReconstruction !" << endl ;
		exit(1) ;
	}

	return *ProtonReconstruction ;
}

void TotemNtuple::Loop(TProjectParameters *ProjectParameters, TProtonReconstruction *protonreconstruction, map_from_string_to_PTPlotsCollection_type *PlotsCollections, map<UInt_t, ULong64_t> *timestamps)
{
//   In a ROOT session, you can do:
//      Root > .L TotemNtuple.C
//      Root > TotemNtuple t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//    Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
//    METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
//    METHOD2: replace line
//    Chain->GetEntry(jentry);       //read all branches
//    by  b_branchname->GetEntry(ientry); //read only this branch

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   TH1D *timestamp_s_event_num = new TH1D("timestamp_s_event_num","timestamp_s_event_num", 100, -500, 3000) ;
   
   ofstream *OutReducedBinaryDataset = NULL ;

   if(ProjectParameters->GetParameterValue("SaveReducedBinaryDataset") == 1.0)
   {
	OutReducedBinaryDataset = new ofstream ;

	std::size_t found = root_file_name.find(".root");
	
	if (found == std::string::npos)
	{
		cout << "Error: void TotemNtuple::Loop: the file name should end with .root !" << endl ;
		exit(1) ;
	}
	
	string reduced_binary_dataset_name = root_file_name.substr(0,found) + ".dat" ;

	cout << "reduced_binary_dataset_name: " << reduced_binary_dataset_name << endl ;
   	OutReducedBinaryDataset->open(reduced_binary_dataset_name.c_str(), ios::binary | ios::out);
	
	if(!OutReducedBinaryDataset->is_open())
	{
		cout << "Error: TotemNtuple::Loop: OutReducedBinaryDataset cannot be opened !" << endl ;
		exit(1) ;
	}
   }

   bool Inefficiency_3_out_of_4_RP_Exclude_Detector = false ;

   if(ProjectParameters->IsParameterDefined("Inefficiency_3_out_of_4_RP_Exclude_Detector"))
   {
	   if(ProjectParameters->GetParameterValue("Inefficiency_3_out_of_4_RP_Exclude_Detector") == 1.0)
	   {
		   Inefficiency_3_out_of_4_RP_Exclude_Detector = true ;
	   }
   }

   // Required and excluded trigger bits

   string required_trigger_bit_name = ProjectParameters->GetSettingValue("required_trigger_bit_name") ;
   int required_trigger_bit = pow(2, ProjectParameters->GetBlock("TOTEM_trigger_bits_block")->GetParameterValue(required_trigger_bit_name)) ;

   // string excluded_trigger_bit_name = ProjectParameters->GetSettingValue("excluded_trigger_bit_name") ;
   // int excluded_trigger_bit = pow(2, ProjectParameters->GetBlock("TOTEM_trigger_bits_block")->GetParameterValue(excluded_trigger_bit_name)) ;

   // Required bunches stored to a map

   map<int, bool> trigger_data_bunch_nums ;

	if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger_data_bunch_num") == 1.0)
	{
	   for(vector<double>::iterator it = ProjectParameters->GetParametersVector("trigger_data_bunch_num")->begin() ; it != ProjectParameters->GetParametersVector("trigger_data_bunch_num")->end() ; ++it)
	   {
   		trigger_data_bunch_nums[*it] = true ;
	   }
	}

   for (Long64_t jentry = 0; jentry < nentries; jentry++)
   {
	if((ProjectParameters->GetParameterValue("MaxNumberOfEventsToAnalyzeIsSet") == 1.0) && (jentry > ProjectParameters->GetParameterValue("MaxNumberOfEventsToAnalyze"))) break ;

	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	// nb = fChain->GetEntry(jentry);   nbytes += nb;
      
	b_trigger_data_bunch_num->GetEntry(ientry);	
	b_trigger_data_input_status_bits->GetEntry(ientry);	
	b_trigger_data_event_num->GetEntry(ientry);	

	b_event_info_timestamp->GetEntry(ientry) ;
	// cout << "timestamp: " << event_info_timestamp << "  trigger_data_event_num :" << trigger_data_event_num << endl ;
	// if(trigger_data_bunch_num != 0) cout << "trigger_data_bunch_num:" << trigger_data_bunch_num << endl ;
	// cout << "trigger_data_bunch_num:" << trigger_data_bunch_num << endl ;

	(*timestamps)[trigger_data_event_num] = event_info_timestamp ;
	if(timestamps->size() != trigger_data_event_num)
	{
		// cout << "Warning: TotemNtuple::Loop: trigger_data_event_num is not continous ! " << endl ;
		// exit(1) ;
	}

	// cout << "hello" << endl ;

	timestamp_s_event_num->Fill((Double_t)(event_info_timestamp - 1360547340)) ;
	
	// cout << "event_info_timestamp: " << event_info_timestamp << endl ;

	bool bunch_is_triggered = false ;

	if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger_data_bunch_num") == 1.0)
	{
		map<int, bool>::iterator it ;
		it = trigger_data_bunch_nums.find(trigger_data_bunch_num) ;
		if (it != trigger_data_bunch_nums.end()) bunch_is_triggered = true ;
	}
	else
	{
		bunch_is_triggered = true ;
	}
	

	if(bunch_is_triggered)
	{

		bool valid_left_bottom_right_top = false ;
		bool valid_left_top_right_bottom = false ;
		
		bool trigger_is_ok = false ;

		if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger") == 1.0)
		{
			bool required_trigger_bit_is_set = ((trigger_data_input_status_bits & required_trigger_bit) != 0) ;
			// bool excluded_trigger_bit_is_set = ((trigger_data_input_status_bits & excluded_trigger_bit) != 0) ;

			// if(required_trigger_bit_is_set && (!excluded_trigger_bit_is_set)) trigger_is_ok = true ;
			if(required_trigger_bit_is_set) trigger_is_ok = true ;			
		}
		else
		{
			trigger_is_ok = true ;
		}
		

		if(trigger_is_ok) 
		{
			// cout << "timestamp: " << event_info_timestamp << "  trigger_data_event_num :" << trigger_data_event_num << endl ;

			if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare("XRPV.A6L5.B2") == 0)
			{
				{
					b_track_rp_25_valid->GetEntry(ientry);			// RP220 left bottom far
					b_track_rp_21_valid->GetEntry(ientry);			// RP220 left bottom near
					b_track_rp_120_valid->GetEntry(ientry);			// RP220 right top near
					b_track_rp_124_valid->GetEntry(ientry);			// RP220 right top far

					bool track_valid = false ;
					
					if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
					{
						if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6R5.B1") == 0.0)	 track_valid = ((track_rp_25_valid && track_rp_21_valid && track_rp_124_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_120_valid))) ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6R5.B1") == 0.0) track_valid = ((track_rp_25_valid && track_rp_21_valid && track_rp_120_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_124_valid))) ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) track_valid = ((track_rp_25_valid && track_rp_120_valid && track_rp_124_valid) && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_21_valid)))  ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) track_valid = ((track_rp_21_valid && track_rp_120_valid && track_rp_124_valid) && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_25_valid)))  ;
						else
						{
							cout << "Error: TotemNtuple::Loop: Unknown device: " << ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP") << " !" << endl ;
							exit(1) ;
						}
					}
					else track_valid = (track_rp_25_valid && track_rp_21_valid && track_rp_120_valid && track_rp_124_valid) ;

					if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0) && track_valid)
					{
						ProtonReconstruction(ientry, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP, protonreconstruction, PlotsCollections, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters) ;

						(*PlotsCollections)["PlotsCollection"]->Fill(protonreconstruction) ;
						valid_left_bottom_right_top = true ;
					}
				}

				{

					b_track_rp_125_valid->GetEntry(ientry);			// RP220 right bottom far
					b_track_rp_121_valid->GetEntry(ientry);			// RP220 right bottom near
					b_track_rp_20_valid->GetEntry(ientry);			// RP220 left top near
					b_track_rp_24_valid->GetEntry(ientry);			// RP220 left top far

					bool track_valid = false ;

					if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP"))
					{
						if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6R5.B1") == 0.0)	 track_valid = ((track_rp_125_valid && track_rp_20_valid && track_rp_24_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_121_valid))) ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6R5.B1") == 0.0) track_valid = ((track_rp_121_valid && track_rp_20_valid && track_rp_24_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_125_valid))) ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) track_valid = ((track_rp_125_valid && track_rp_121_valid && track_rp_24_valid) && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_20_valid))) ;
						else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) track_valid = ((track_rp_125_valid && track_rp_121_valid && track_rp_20_valid) && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_rp_24_valid))) ;
						else
						{
							cout << "Error: TotemNtuple::Loop: Unknown device: " << ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP") << " !" << endl ;
							exit(1) ;
						}
					}
					else track_valid = (track_rp_125_valid && track_rp_121_valid && track_rp_20_valid && track_rp_24_valid) ;

					if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0) && track_valid)
					{
						ProtonReconstruction(ientry, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM, protonreconstruction, PlotsCollections, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters) ;

						(*PlotsCollections)["PlotsCollection"]->Fill(protonreconstruction) ;
						valid_left_top_right_bottom = true ;
					}

				}
			}
			else if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare("XRPV.D6L5.B2") == 0)
			{
				{
					b_track_rp_25_valid->GetEntry(ientry);			// RP220 left bottom far
					b_track_rp_5_valid->GetEntry(ientry);			// RP210 left bottom far
					b_track_rp_104_valid->GetEntry(ientry);			// RP210 right top far
					b_track_rp_124_valid->GetEntry(ientry);			// RP220 right top far

					if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0) && track_rp_25_valid && track_rp_5_valid && track_rp_104_valid && track_rp_124_valid)
					{
						ProtonReconstruction(ientry, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP, protonreconstruction, PlotsCollections, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters) ;

						(*PlotsCollections)["PlotsCollection"]->Fill(protonreconstruction) ;
						valid_left_bottom_right_top = true ;
					}
				}

				{
					b_track_rp_125_valid->GetEntry(ientry);			// RP220 right bottom far
					b_track_rp_105_valid->GetEntry(ientry);			// RP210 right bottom far
					b_track_rp_4_valid->GetEntry(ientry);			// RP210 left top far
					b_track_rp_24_valid->GetEntry(ientry);			// RP220 left top far

					if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0) && track_rp_125_valid && track_rp_105_valid && track_rp_4_valid && track_rp_24_valid)
					{
						ProtonReconstruction(ientry, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM, protonreconstruction, PlotsCollections, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters) ;

						(*PlotsCollections)["PlotsCollection"]->Fill(protonreconstruction) ;
						valid_left_top_right_bottom = true ;
					}

				}
			}
			else
			{
				cout << "Wrong configuration: TotemNtuple::Loop !" << endl ;
				exit(1) ;
			}
		}
	}

	// if (Cut(ientry) < 0) continue;
   }


	// Plot about timestamps

	TCanvas *canvas = new TCanvas() ;

	gStyle->SetPalette(1);    
	gStyle->SetTitleFillColor(kWhite) ;
	gStyle->SetStatColor(kWhite) ;  

	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);
	canvas->SetGridx() ;
	canvas->SetGridy() ;	

	canvas->SetFillColor(kWhite) ;
	canvas->SetFrameBorderMode(0) ;   
   
	timestamp_s_event_num->Draw() ;
	timestamp_s_event_num->GetXaxis()->SetTitle("Timestamp") ;

	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	// canvas->SaveAs((output_directory + "/timestamp_s_event_num.eps").c_str()) ;
	// canvas->SaveAs((output_directory + "/timestamp_s_event_num.root").c_str()) ;

	for(int i = 1 ; i <= timestamp_s_event_num->GetSize() ; ++i)
	{
		if(timestamp_s_event_num->GetBinContent(i) == 0)
		{
			cout << "Empty bin in timestamp: " << (ULong64_t)(timestamp_s_event_num->GetBinCenter(i)) << endl ;
		}
	}

	if(ProjectParameters->GetParameterValue("SaveReducedBinaryDataset") == 1.0) OutReducedBinaryDataset->close() ;
}

TH2F *track_rp_2_x_track_rp_2_y = NULL ;
TH2F *track_rp_3_x_track_rp_3_y = NULL ;

TH2F *track_rp_102_x_track_rp_102_y = NULL ;
TH2F *track_rp_103_x_track_rp_103_y = NULL ;

TH2F *track_rp_3_x_track_rp_3_y_with_cut = NULL ;
TH2F *track_rp_103_x_track_rp_103_y_with_cut  = NULL ;
TH2F *track_rp_102_x_track_rp_102_y_with_cut  = NULL ;

TH2F *x_far_mm_minus_x_near_vs_xnear_left = NULL ;
TH2F *x_far_mm_minus_x_near_vs_xnear_left_with_x_cut = NULL ;

TH2F *x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_1 = NULL ;
TH2F *x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_2 = NULL ;
TH2F *x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_3 = NULL ;

TH2F *x_far_mm_minus_x_near_vs_xnear_left_with_y_far_per_y_near_cut_slice_1 = NULL ;

TH2F *y_far_per_y_near_y_near_right = NULL ;
TH2F *y_far_per_y_near_x_near_right = NULL ;

TH2F *y_far_per_y_near_x_far_left = NULL ;

TAnalysisPlot2D *x_mm_near_x_mm_far_left_with_cut = NULL ;
TAnalysisPlot2D *x_mm_near_x_mm_far_right_with_cut = NULL ;

TAnalysisPlot2D *x_mm_near_x_mm_far_left_after_y_cut_with_cut = NULL ;
TAnalysisPlot2D *x_mm_near_x_mm_far_right_after_y_cut_with_cut = NULL ;

TH1F *track_rp_2_xi_distribution = NULL ;
TH1F *track_rp_3_xi_distribution = NULL ;

vector <TH1F *> vector_track_rp_3_xi_distribution;
vector <TH1F *> vector_track_rp_103_xi_distribution;

vector <TH1F *> vector_track_rp_3_xi_distribution_shifted ;
vector <TH1F *> vector_track_rp_103_xi_distribution_shifted ;

TH1F *track_rp_3_xi_distribution_with_x_xi_function = NULL ;
TH1F *track_rp_103_xi_distribution_with_x_xi_function = NULL ;

TH1F *track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut = NULL ;
TH1F *track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut = NULL ;

TH1F *track_rp_102_xi_distribution = NULL ;
TH1F *track_rp_103_xi_distribution = NULL ;

TH1F *track_rp_2_xi_distribution_after_cut = NULL ;
TH1F *track_rp_3_xi_distribution_after_cut = NULL ;

TH1F *track_rp_102_xi_distribution_after_cut = NULL ;
TH1F *track_rp_103_xi_distribution_after_cut = NULL ;

TH1F *track_rp_2_xi_distribution_with_cut = NULL ;
TH1F *track_rp_3_xi_distribution_with_cut = NULL ;

TH1F *track_rp_102_xi_distribution_with_cut = NULL ;
TH1F *track_rp_103_xi_distribution_with_cut = NULL ;

TAnalysisPlot2D *x_mm_near_x_mm_far_left = NULL ;

TH1F *number_of_RPs_close_to_overload_hist = NULL ;
TH1F *elastic_number_of_RPs_close_to_overload_hist = NULL ;


double ctpps_number_of_histograms = 60 ;

void TotemNtuple_CTPPS_2016::Loop(TProjectParameters *ProjectParameters, TProtonReconstruction *protonreconstruction, map_from_string_to_PTPlotsCollection_type *PlotsCollections, map<UInt_t, ULong64_t> *timestamps)
{
//   In a ROOT session, you can do:
//      Root > .L TotemNtuple.C
//      Root > TotemNtuple t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

	string MAD_X_Xi_dependent_optics_beam_1 = ProjectParameters->GetSettingValue("MAD_X_Xi_dependent_optics_beam_1") ;
	string MAD_X_Xi_dependent_optics_beam_2 = ProjectParameters->GetSettingValue("MAD_X_Xi_dependent_optics_beam_2") ;

	TFile *MAD_X_Xi_dependent_optics_beam_1_file = new TFile(MAD_X_Xi_dependent_optics_beam_1.c_str(), "READ") ;
	TFile *MAD_X_Xi_dependent_optics_beam_2_file = new TFile(MAD_X_Xi_dependent_optics_beam_2.c_str(), "READ") ;

	if((MAD_X_Xi_dependent_optics_beam_1_file == NULL) || (MAD_X_Xi_dependent_optics_beam_2_file == NULL))
	{
		cout << "Error: TotemNtuple_CTPPS_2016: the optics graphs are empty !" << endl ;
		exit(1) ;
	}

	TGraph *MAD_X_Xi_dependent_optics_beam_1_graph = (TGraph *)MAD_X_Xi_dependent_optics_beam_1_file->Get("XRPH_D6R5_B1") ;
	TGraph *MAD_X_Xi_dependent_optics_beam_2_graph = (TGraph *)MAD_X_Xi_dependent_optics_beam_2_file->Get("XRPH_D6L5_B2") ;

	if((MAD_X_Xi_dependent_optics_beam_1_graph == NULL) || (MAD_X_Xi_dependent_optics_beam_2_graph == NULL))
	{
		cout << "Error: TotemNtuple_CTPPS_2016: the optics graphs are empty !" << endl ;
		exit(1) ;
	}

	double alignment_uncertainty_mm = 0.1 ;

	//double Dx_rp_2_m   = -7.940002430162743e-02 ;
	//double Dx_rp_3_m   = -7.756017112313687e-02 ;
	//double Dx_rp_102_m = -8.100000001352275e-02 ;
	//double Dx_rp_103_m = -7.729383623786196e-02 ;

	// First estimation

	// double Dx_rp_2_m   = -9.26e-02 ;
	// double Dx_rp_3_m   = -9.22e-02 ;
	// double Dx_rp_102_m = -5.81e-02 ;
	// double Dx_rp_103_m = -5.16e-02 ;

	// Second estimation

	// double Dx_rp_2_m   = -9.607088464992548e-02 ;
	// double Dx_rp_3_m   = -9.479694386792839e-02 ;
	// double Dx_rp_102_m = -6.934379295696134e-02 ;
	// double Dx_rp_103_m = -6.538430945433377e-02 ;

	// Third estimation

	double Dx_rp_2_m   = -9.805e-02 ;
	double Dx_rp_3_m   = -9.656e-02 ;
	double Dx_rp_102_m = -7.038e-02 ;
	double Dx_rp_103_m = -6.685e-02 ;
	
	//--------------------------------------
	
	// Play

	double dDx = -2.03e-02  ;

	// double Dx_rp_2_m   = -9.0e-02 ;
	// double Dx_rp_3_m   = -8.8e-02 ;
	// double Dx_rp_102_m = -6.06e-02 ;
	// double Dx_rp_103_m = -5.6e-02 ;
	
	// Dx_rp_2_m   = -1.0 ;
	// Dx_rp_3_m   = -1.0 ;
	// Dx_rp_102_m = -1.0 ;
	// Dx_rp_103_m = -1.0 ;
	
	double y_cut_mm = 0.5 ;

	for (Long64_t jentry=0; jentry<nentries;jentry++)
	{
		Long64_t ientry = LoadTree(jentry);

		if (ientry < 0) break;

		// nb = fChain->GetEntry(jentry);   nbytes += nb;

		b_track_rp_2_valid->GetEntry(ientry);
		b_track_rp_3_valid->GetEntry(ientry);
		b_track_rp_102_valid->GetEntry(ientry);
		b_track_rp_103_valid->GetEntry(ientry);

		b_track_rp_2_x->GetEntry(ientry);
		b_track_rp_3_x->GetEntry(ientry);
		b_track_rp_102_x->GetEntry(ientry);
		b_track_rp_103_x->GetEntry(ientry);

		b_track_rp_2_y->GetEntry(ientry);
		b_track_rp_3_y->GetEntry(ientry);
		b_track_rp_102_y->GetEntry(ientry);
		b_track_rp_103_y->GetEntry(ientry);

		double xi_rp_2 	 = ((track_rp_2_x   / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_2_m) ;
		double xi_rp_3 	 = ((track_rp_3_x   / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_3_m) ;
		double xi_rp_102 = ((track_rp_102_x / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_102_m) ;
		double xi_rp_103 = ((track_rp_103_x / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_103_m) ;


		if(track_rp_2_valid)
		{
			track_rp_2_xi_distribution->Fill(-xi_rp_2) ;
			track_rp_2_x_track_rp_2_y->Fill(track_rp_2_x, track_rp_2_y) ;
		}

		if(track_rp_3_valid)
		{
			if((fabs(track_rp_3_y) < y_cut_mm))
			{
				track_rp_3_xi_distribution->Fill(-xi_rp_3) ;

				
				for(int i = 0 ; i < ctpps_number_of_histograms  ; ++i)
				{
					double perturbation = 1.0 + ((i - (ctpps_number_of_histograms / 2.0)) / 100) ;

					double Dx_rp_3_m_perturbed  = Dx_rp_3_m + ((dDx) * perturbation) ;
					double xi_rp_3_perturbed = ((track_rp_3_x   / TConstants::conversion_factor_from_m_to_mm) / (Dx_rp_3_m_perturbed)) ;
					double xi_rp_3_perturbed_shifted = (((track_rp_3_x - alignment_uncertainty_mm)   / TConstants::conversion_factor_from_m_to_mm) / (Dx_rp_3_m_perturbed)) ;

					vector_track_rp_3_xi_distribution[i]->Fill(-xi_rp_3_perturbed) ;
					vector_track_rp_3_xi_distribution_shifted[i]->Fill(-xi_rp_3_perturbed_shifted) ;

					// cout << perturbation << endl ;
				}

				// track_rp_3_xi_distribution_with_x_xi_function->Fill(-((MAD_X_Xi_dependent_optics_beam_2_graph->Eval((track_rp_3_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_2_offset"))))) ;

				if(track_rp_2_valid)
				{
					x_mm_near_x_mm_far_left_after_y_cut_with_cut->Fill(track_rp_3_x, track_rp_2_x) ;

					if(x_mm_near_x_mm_far_left_after_y_cut_with_cut->GetFillStatus())
					{
						track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Fill(-((MAD_X_Xi_dependent_optics_beam_2_graph->Eval((track_rp_3_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_2_offset"))))) ;
					}
				}
			}

			track_rp_3_x_track_rp_3_y->Fill(track_rp_3_x, track_rp_3_y) ;

			x_mm_near_x_mm_far_left_with_cut->Fill(track_rp_3_x, track_rp_2_x) ;

			if(track_rp_2_valid)
			{
				if(x_mm_near_x_mm_far_left_with_cut->GetFillStatus())
				{
					track_rp_3_x_track_rp_3_y_with_cut->Fill(track_rp_3_x, track_rp_3_y) ;
					track_rp_2_xi_distribution_with_cut->Fill(-xi_rp_2) ;
					track_rp_3_xi_distribution_with_cut->Fill(-xi_rp_3) ;
					
					track_rp_3_xi_distribution_with_x_xi_function->Fill(-((MAD_X_Xi_dependent_optics_beam_2_graph->Eval((track_rp_3_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_2_offset"))))) ;
					
					y_far_per_y_near_x_far_left->Fill(track_rp_3_x, track_rp_3_y / track_rp_2_y) ;
					
					if(((track_rp_3_y / track_rp_2_y) > 1.28) && ((track_rp_3_y / track_rp_2_y) < 1.35))
					{
						x_far_mm_minus_x_near_vs_xnear_left_with_y_far_per_y_near_cut_slice_1->Fill(track_rp_2_x, track_rp_3_x - track_rp_2_x) ; 
					}
					
				}
		

			}
		}

		if(track_rp_102_valid)
		{
			track_rp_102_xi_distribution->Fill(-xi_rp_102) ;
			track_rp_102_x_track_rp_102_y->Fill(track_rp_102_x, track_rp_102_y) ;
		}

		if(track_rp_103_valid)
		{
			if((fabs(track_rp_103_y) < y_cut_mm))
			{
				track_rp_103_xi_distribution->Fill(-xi_rp_103) ;
				
				for(int i = 0 ; i < ctpps_number_of_histograms  ; ++i)
				{
					double perturbation = 1.0 + ((i - (ctpps_number_of_histograms / 2.0)) / 100) ;
					double Dx_rp_103_m_perturbed  = Dx_rp_103_m - ((dDx) * perturbation) ;
					double xi_rp_103_perturbed = ((track_rp_103_x   / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_103_m_perturbed) ;
					double xi_rp_103_perturbed_shifted = (((track_rp_103_x)   / TConstants::conversion_factor_from_m_to_mm) / Dx_rp_103_m_perturbed) ;

					vector_track_rp_103_xi_distribution[i]->Fill(-xi_rp_103_perturbed) ;
					vector_track_rp_103_xi_distribution_shifted[i]->Fill(-xi_rp_103_perturbed_shifted) ;
					// cout << perturbation << endl ;
				}

				// track_rp_103_xi_distribution_with_x_xi_function->Fill(-((MAD_X_Xi_dependent_optics_beam_1_graph->Eval((track_rp_103_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_1_offset"))))) ;

				if(track_rp_102_valid)
				{
					x_mm_near_x_mm_far_right_after_y_cut_with_cut->Fill(track_rp_103_x, track_rp_102_x) ;

					if(x_mm_near_x_mm_far_right_after_y_cut_with_cut->GetFillStatus())
					{
						track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Fill(-((MAD_X_Xi_dependent_optics_beam_1_graph->Eval((track_rp_103_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_1_offset"))))) ;						
					}
				}

			}
		
			track_rp_103_x_track_rp_103_y->Fill(track_rp_103_x, track_rp_103_y) ;

			if(track_rp_102_valid)
			{
				x_mm_near_x_mm_far_right_with_cut->Fill(track_rp_103_x, track_rp_102_x) ;

				if(x_mm_near_x_mm_far_right_with_cut->GetFillStatus())
				{
					track_rp_103_x_track_rp_103_y_with_cut->Fill(track_rp_103_x, track_rp_103_y) ;
					track_rp_102_x_track_rp_102_y_with_cut->Fill(track_rp_102_x, track_rp_102_y) ;
					
					x_mm_near_x_mm_far_left->Fill(track_rp_3_x, track_rp_103_x) ;
					
					track_rp_102_xi_distribution_with_cut->Fill(-xi_rp_102) ;
					track_rp_103_xi_distribution_with_cut->Fill(-xi_rp_103) ;
					
					track_rp_103_xi_distribution_with_x_xi_function->Fill(-((MAD_X_Xi_dependent_optics_beam_1_graph->Eval((track_rp_103_x / TConstants::conversion_factor_from_m_to_mm) + ProjectParameters->GetParameterValue("MAD_X_Xi_dependent_optics_beam_1_offset"))))) ;

					x_far_mm_minus_x_near_vs_xnear_left->Fill(track_rp_102_x, track_rp_103_x - track_rp_102_x) ; 
					
					y_far_per_y_near_y_near_right->Fill(track_rp_102_y, track_rp_103_y / track_rp_102_y) ;
					y_far_per_y_near_x_near_right->Fill(track_rp_102_x, track_rp_103_y / track_rp_102_y) ;
					
					if(((track_rp_103_y / track_rp_102_y) < -0.56) && ((track_rp_103_y / track_rp_102_y) > -0.6))
					{
						x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_1->Fill(track_rp_102_x, track_rp_103_x - track_rp_102_x) ; 
					}

					if(((track_rp_103_y / track_rp_102_y) < -0.64) && ((track_rp_103_y / track_rp_102_y) > -0.68))
					{
						x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_2->Fill(track_rp_102_x, track_rp_103_x - track_rp_102_x) ; 
					}

					if(((track_rp_103_y / track_rp_102_y) < 3.04) && ((track_rp_103_y / track_rp_102_y) > 3.0))
					{
						x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_3->Fill(track_rp_102_x, track_rp_103_x - track_rp_102_x) ; 
					}

					if((6.0 < track_rp_102_x) && (track_rp_102_x < 6.1))
					{
						x_far_mm_minus_x_near_vs_xnear_left_with_x_cut->Fill(track_rp_102_x, track_rp_103_x - track_rp_102_x) ; 
					}
				}
			}

		}

	}

}


Int_t TotemNtuple::Cut(Long64_t entry)
{
	return 1;
}

bool RPTrackCondition(Pset_of_Pvalidities validity_set, TProjectParameters *ProjectParameters)
{
	int number_of_true_logic_variables = 0 ;

	for (set_of_Pvalidities::iterator it = validity_set->begin(); it != validity_set->end(); ++it) if(*(*it)) ++number_of_true_logic_variables ;

	if(ProjectParameters->GetSettingValue("logical_condition_of_data_reduction").compare(TConstants::AT_LEAST_3_VALID_RP_TRACK) == 0)
	{
		return (number_of_true_logic_variables >= 3) ;
	}
	else if(ProjectParameters->GetSettingValue("logical_condition_of_data_reduction").compare(TConstants::AT_LEAST_1_VALID_RP_TRACK) == 0)
	{
		return (number_of_true_logic_variables >= 1) ;
	}
	else
	{
		cout << "Error in  RPTrackCondition: unknown logical_condition_of_data_reduction setting !: " << ProjectParameters->GetSettingValue("logical_condition_of_data_reduction") << endl ;
		exit(1) ;

	}
	
}

bool FillIfRPTrackConditionIsTrue(Long64_t ientry, TBranchVectors *branch_sets, TProjectParameters *ProjectParameters)
{
	bool status = false ;
	
	for(set_of_Pbranch::iterator it = branch_sets->validity_branches_to_get->begin(); it != branch_sets->validity_branches_to_get->end(); ++it) (*it)->GetEntry(ientry) ;

	if(RPTrackCondition(branch_sets->validity_branches, ProjectParameters))
	{
		for(set_of_Pbranch::iterator it = branch_sets->branches_to_get->begin(); it != branch_sets->branches_to_get->end(); ++it) (*it)->GetEntry(ientry) ;

		status = true ;
	}

	return status ;
}

void TotemNtuple::Loop(TProjectParameters *ProjectParameters, TTree *mytree_left_bottom_right_top,  TTree *mytree_left_top_right_bottom)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   signed int offset = 0;   

   for (Long64_t jentry=0; jentry < nentries; jentry++)
   {
	Long64_t ientry = LoadTree(jentry);

	b_event_info_timestamp->GetEntry(ientry);
	b_event_info_optoRx_BX->GetEntry(ientry) ;

	for(set_of_Pbranch::iterator it = horizontal_branch_vectors->validity_branches_to_get->begin(); it != horizontal_branch_vectors->validity_branches_to_get->end(); ++it) (*it)->GetEntry(ientry) ;
	for(set_of_Pbranch::iterator it = horizontal_branch_vectors->branches_to_get->begin(); it != horizontal_branch_vectors->branches_to_get->end(); ++it) (*it)->GetEntry(ientry) ;

	bool status_left_bottom_right_top =  FillIfRPTrackConditionIsTrue(ientry, left_bottom_right_top_branch_vectors, ProjectParameters) ;
	bool status_left_top_right_bottom =  FillIfRPTrackConditionIsTrue(ientry, left_top_right_bottom_branch_vectors, ProjectParameters) ;

	// Trigger block realignment if necessary

	if(ProjectParameters->IsParameterDefined("Correct_for_trigger_block_shift") && (ProjectParameters->GetParameterValue("Correct_for_trigger_block_shift") == 1.0))
	{
		signed int bunch_ei = event_info_optoRx_BX[0] - TConstants::TriggerBlockShift;

		if (bunch_ei < 0)
			bunch_ei += TConstants::NegativeBunchNumberAdjusment;

		if (offset != 0)
		{
			ientry = LoadTree(jentry - offset);
		}

		b_trigger_data_bunch_num->GetEntry(ientry) ;

		// cout << jentry << " event" << endl ;	

		if (bunch_ei != trigger_data_bunch_num)
		{
			if(ProjectParameters->GetParameterValue("Correct_for_trigger_block_shift") != 1.0)
			{
				// cout << "Error:  TotemNtuple: trigger block shift is observed, but \"Correct_for_trigger_block_shift\" settings is not 1.0 !" << endl ;
				// exit(1) ;
			}

			offset++ ;
			cout << jentry << " event: trigger block is shifted w.r.t. to the other blocks" << endl ;
			continue ;
		}
	}

	b_trigger_data_run_num->GetEntry(ientry);
	b_trigger_data_bunch_num->GetEntry(ientry);
	b_trigger_data_event_num->GetEntry(ientry);
	b_trigger_data_trigger_num->GetEntry(ientry);
	b_trigger_data_input_status_bits->GetEntry(ientry);

	// End of trigger block realignment

	if(status_left_bottom_right_top) mytree_left_bottom_right_top->Fill() ;
	if(status_left_top_right_bottom) mytree_left_top_right_bottom->Fill() ;

   }

}

class TEventInfo
{
	public:

	int number_of_occurances ;

	TEventInfo() ;
};

TEventInfo::TEventInfo() : number_of_occurances(0)
{
} ;

class TLookUpInformation
{
	public:

	int number_of_event_left_far ;
	int number_of_event_left_near ;
	int number_of_event_right_far ;
	int number_of_event_right_near ;

	int digi_rp_left_far_uPlanesOn, digi_rp_left_near_uPlanesOn, digi_rp_right_near_uPlanesOn, digi_rp_right_far_uPlanesOn ;
	int digi_rp_left_far_vPlanesOn, digi_rp_left_near_vPlanesOn, digi_rp_right_near_vPlanesOn, digi_rp_right_far_vPlanesOn ;

	vector<int> digi_rp_left_far_numberOfClusters, digi_rp_left_near_numberOfClusters, digi_rp_right_near_numberOfClusters,  digi_rp_right_far_numberOfClusters ;
} ;


typedef pair	<ULong_t, ULong_t> run_number_event_number_pair_type ;
typedef vector	<TProtonReconstruction> vector_TProtonreconstruction_type ;

map <run_number_event_number_pair_type, TLookUpInformation> map_lookup_table ;

TH1F *elastic_events_per_multiplicity = NULL ;
TH1F *events_per_multiplicity = NULL ;

bool EvaluateLogicalCondition(TProjectParameters *ProjectParameters, run_number_event_number_pair_type run_number_event_number_pair	)
{
	int count = 0 ;

	TLookUpInformation *LookUpInformation = &map_lookup_table[run_number_event_number_pair] ;

	if(ProjectParameters->IsParameterDefined("general_event_category"))
	{
		count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_first_order"))
	{
		bool logical_condition = ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 1)) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_a"))
	{
		bool condition_4_out_of_4_2nd_order_a_only_one_in_left = ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1)) ;
		bool condition_4_out_of_4_2nd_order_a_only_one_in_right = ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near == 1)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_a_only_one_in_left || condition_4_out_of_4_2nd_order_a_only_one_in_right) ;
		if(logical_condition) count++ ;

	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_b"))
	{
		bool logical_condition = ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 2)) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_a_N_greater_than_2"))
	{
		bool condition_4_out_of_4_2nd_order_a_only_one_in_left 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1)) && ((LookUpInformation->number_of_event_right_far  > 2) && (LookUpInformation->number_of_event_right_near  > 2)) ;
		bool condition_4_out_of_4_2nd_order_a_only_one_in_right = ((LookUpInformation->number_of_event_left_far  > 2) && (LookUpInformation->number_of_event_left_near  > 2)) && ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near == 1)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_a_only_one_in_left || condition_4_out_of_4_2nd_order_a_only_one_in_right) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_b_N_equals_2"))
	{
		bool condition_4_out_of_4_2nd_order_b 		= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 2)) ;
		bool condition_4_out_of_4_2nd_order_b_left 	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_right 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 2)) ;

		bool logical_condition = ((condition_4_out_of_4_2nd_order_b || condition_4_out_of_4_2nd_order_b_left || condition_4_out_of_4_2nd_order_b_right)) ;
		if(logical_condition) count++ ;
	}
	
	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_b_one_N_equals_2"))
	{
		bool condition_4_out_of_4_2nd_order_b_1	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_2	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_3	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_4	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 2)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_b_1 || condition_4_out_of_4_2nd_order_b_2 || condition_4_out_of_4_2nd_order_b_3 || condition_4_out_of_4_2nd_order_b_4) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_b_three_N_equals_2"))
	{
		bool condition_4_out_of_4_2nd_order_b_1	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_2	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 2)) ;
		bool condition_4_out_of_4_2nd_order_b_3	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 2)) ;
		bool condition_4_out_of_4_2nd_order_b_4	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 2)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_b_1 || condition_4_out_of_4_2nd_order_b_2 || condition_4_out_of_4_2nd_order_b_3 || condition_4_out_of_4_2nd_order_b_4) ;
		if(logical_condition) count++ ;
	}
	
	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_b_two_N_equals_2_in_different_sectors"))
	{
		bool condition_4_out_of_4_2nd_order_b_1	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 2)) ;
		bool condition_4_out_of_4_2nd_order_b_2	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 2) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 1)) ;
		bool condition_4_out_of_4_2nd_order_b_3	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 1) && (LookUpInformation->number_of_event_right_far == 2)) ;
		bool condition_4_out_of_4_2nd_order_b_4	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 1) && (LookUpInformation->number_of_event_right_near == 2) && (LookUpInformation->number_of_event_right_far == 1)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_b_1 || condition_4_out_of_4_2nd_order_b_2 || condition_4_out_of_4_2nd_order_b_3 || condition_4_out_of_4_2nd_order_b_4) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_2nd_order_a_N_greater_than_2_in_one_RP"))
	{
		bool condition_1 	= ((LookUpInformation->number_of_event_left_far  > 2) && (LookUpInformation->number_of_event_left_near == 1)) && ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near == 1)) ;
		bool condition_2 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near  > 2)) && ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near == 1)) ;
		bool condition_3 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1)) && ((LookUpInformation->number_of_event_right_far  > 2) && (LookUpInformation->number_of_event_right_near == 1)) ;
		bool condition_4 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 1)) && ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near  > 2)) ;

		bool logical_condition = (condition_1 || condition_2 || condition_3 || condition_4) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_3rd_order_a_N_greater_than_2_in_two_RP"))
	{
		bool condition_4_out_of_4_2nd_order_a_only_one_in_left 		= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 2)) && ((LookUpInformation->number_of_event_right_far  > 2) && (LookUpInformation->number_of_event_right_near  > 2)) ;
		bool condition_4_out_of_4_2nd_order_a_only_one_in_right 	= ((LookUpInformation->number_of_event_left_far  > 2) && (LookUpInformation->number_of_event_left_near  > 2)) && ((LookUpInformation->number_of_event_right_far == 2) && (LookUpInformation->number_of_event_right_near == 2)) ;

		bool condition_4_out_of_4_2nd_order_a_only_one_in_left_1 	= ((LookUpInformation->number_of_event_left_far == 1) && (LookUpInformation->number_of_event_left_near == 2)) && ((LookUpInformation->number_of_event_right_far  > 2) && (LookUpInformation->number_of_event_right_near  > 2)) ;
		bool condition_4_out_of_4_2nd_order_a_only_one_in_right_1 	= ((LookUpInformation->number_of_event_left_far  > 2) && (LookUpInformation->number_of_event_left_near  > 2)) && ((LookUpInformation->number_of_event_right_far == 1) && (LookUpInformation->number_of_event_right_near == 2)) ;

		bool condition_4_out_of_4_2nd_order_a_only_one_in_left_2 	= ((LookUpInformation->number_of_event_left_far == 2) && (LookUpInformation->number_of_event_left_near == 1)) && ((LookUpInformation->number_of_event_right_far  > 2) && (LookUpInformation->number_of_event_right_near  > 2)) ;
		bool condition_4_out_of_4_2nd_order_a_only_one_in_right_2 	= ((LookUpInformation->number_of_event_left_far  > 2) && (LookUpInformation->number_of_event_left_near  > 2)) && ((LookUpInformation->number_of_event_right_far == 2) && (LookUpInformation->number_of_event_right_near == 1)) ;

		bool logical_condition = (condition_4_out_of_4_2nd_order_a_only_one_in_left || condition_4_out_of_4_2nd_order_a_only_one_in_right || condition_4_out_of_4_2nd_order_a_only_one_in_left_1 || condition_4_out_of_4_2nd_order_a_only_one_in_right_1 || condition_4_out_of_4_2nd_order_a_only_one_in_left_2 || condition_4_out_of_4_2nd_order_a_only_one_in_right_2) ;
		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("Simone_check_1_N_greater_than_1"))
	{
		bool Simone_check_1_N_greater_than_1_left =  ((LookUpInformation->number_of_event_left_far > 1) && (LookUpInformation->number_of_event_left_near > 1)) ;
		bool Simone_check_1_N_greater_than_1_right = ((LookUpInformation->number_of_event_right_far > 1) && (LookUpInformation->number_of_event_right_near > 1)) ;

		bool logical_condition = (Simone_check_1_N_greater_than_1_left && Simone_check_1_N_greater_than_1_right) ;
		if(logical_condition) count++ ;
	}

	int number_of_RP_with_one_track = 0 ;

	if(LookUpInformation->number_of_event_left_far   == 1) ++number_of_RP_with_one_track ;
	if(LookUpInformation->number_of_event_left_near  == 1) ++number_of_RP_with_one_track ;
	if(LookUpInformation->number_of_event_right_far  == 1) ++number_of_RP_with_one_track ;
	if(LookUpInformation->number_of_event_right_near == 1) ++number_of_RP_with_one_track ;

	int number_of_RP_with_two_tracks = 0 ;

	if(LookUpInformation->number_of_event_left_far   == 2) ++number_of_RP_with_two_tracks ;
	if(LookUpInformation->number_of_event_left_near  == 2) ++number_of_RP_with_two_tracks ;
	if(LookUpInformation->number_of_event_right_far  == 2) ++number_of_RP_with_two_tracks ;
	if(LookUpInformation->number_of_event_right_near == 2) ++number_of_RP_with_two_tracks ;

	int number_of_RP_with_more_than_two_tracks = 0 ;

	if(LookUpInformation->number_of_event_left_far   > 2) ++number_of_RP_with_more_than_two_tracks ;
	if(LookUpInformation->number_of_event_left_near  > 2) ++number_of_RP_with_more_than_two_tracks ;
	if(LookUpInformation->number_of_event_right_far  > 2) ++number_of_RP_with_more_than_two_tracks ;
	if(LookUpInformation->number_of_event_right_near > 2) ++number_of_RP_with_more_than_two_tracks ;


	if(ProjectParameters->IsParameterDefined("4_out_of_4_3rd_order_a_N_greater_than_2_in_one_RP"))
	{

		bool logical_condition_1 = (number_of_RP_with_one_track == 2) && (number_of_RP_with_two_tracks == 1) && (number_of_RP_with_more_than_two_tracks == 1) ;
		bool logical_condition_2 = (number_of_RP_with_one_track == 1) && (number_of_RP_with_two_tracks == 2) && (number_of_RP_with_more_than_two_tracks == 1) ;
		bool logical_condition_3 = (number_of_RP_with_one_track == 0) && (number_of_RP_with_two_tracks == 3) && (number_of_RP_with_more_than_two_tracks == 1) ;

		bool logical_condition = (logical_condition_1 || logical_condition_2 || logical_condition_3) ;

		if(logical_condition) count++ ;
	}

	if(ProjectParameters->IsParameterDefined("4_out_of_4_4th_order"))
	{
		bool logical_condition_1 = (number_of_RP_with_one_track == 1) && (number_of_RP_with_two_tracks == 0) && (number_of_RP_with_more_than_two_tracks == 3) ;
		bool logical_condition_2 = (number_of_RP_with_one_track == 0) && (number_of_RP_with_two_tracks == 1) && (number_of_RP_with_more_than_two_tracks == 3) ;

		bool logical_condition = (logical_condition_1 || logical_condition_2) ;

		if(logical_condition) count++ ;
	}

	if((count != 0) && (count != 1))
	{
		cout << "Error in EvaluateLogicalCondition: logical conditions are not disjoint for this event !" << endl ;
		exit(1) ;
	}

	bool logical_condition = false ;

	if(count == 1) logical_condition = true ;

	return logical_condition ;
}

int N_multitrack_3_out_4_inefficiency(TLookUpInformation LookUpInformation)
{
	int n_multitrack_1 = 				0 			+ LookUpInformation.number_of_event_left_near 	+ LookUpInformation.number_of_event_right_far 	+ LookUpInformation.number_of_event_right_near ;
	int n_multitrack_2 = LookUpInformation.number_of_event_left_far  	+ 			0 			+ LookUpInformation.number_of_event_right_far 	+ LookUpInformation.number_of_event_right_near ;
	int n_multitrack_3 = LookUpInformation.number_of_event_left_far 	+ LookUpInformation.number_of_event_left_near 	+ 			0 			+ LookUpInformation.number_of_event_right_near ;
	int n_multitrack_4 = LookUpInformation.number_of_event_left_far 	+ LookUpInformation.number_of_event_left_near 	+ LookUpInformation.number_of_event_right_far 	+ 			0 ;

	// cout << "n_multitrack_1: " << n_multitrack_1 << endl ;
	// cout << "n_multitrack_2: " << n_multitrack_2 << endl ;
	// cout << "n_multitrack_3: " << n_multitrack_3 << endl ;
	// cout << "n_multitrack_4: " << n_multitrack_4 << endl ;

	return ((n_multitrack_1 + n_multitrack_2 + n_multitrack_3 + n_multitrack_4) / 4.0) ;
}

int N_multitrack_2_out_4_inefficiency(TLookUpInformation LookUpInformation)
{
	int n_multitrack_1 = LookUpInformation.number_of_event_left_far		+ 0 						+ LookUpInformation.number_of_event_right_far 	+ 0 ;
	int n_multitrack_2 = LookUpInformation.number_of_event_left_far		+ 0						+ 0					 	+ LookUpInformation.number_of_event_right_near ;
	int n_multitrack_3 = 0							+ LookUpInformation.number_of_event_left_near 	+ LookUpInformation.number_of_event_right_far 	+ 0 ;
	int n_multitrack_4 = 0							+ LookUpInformation.number_of_event_left_near 	+ 0 						+ LookUpInformation.number_of_event_right_near ;

	// cout << "n_multitrack_1: " << n_multitrack_1 << endl ;
	// cout << "n_multitrack_2: " << n_multitrack_2 << endl ;
	// cout << "n_multitrack_3: " << n_multitrack_3 << endl ;
	// cout << "n_multitrack_4: " << n_multitrack_4 << endl ;

	return ((n_multitrack_1 + n_multitrack_2 + n_multitrack_3 + n_multitrack_4) / 4.0) ;
}

void FillNumberOfClusters(run_number_event_number_pair_type prev_run_number_event_number_pair, TH1F *events_per_number_of_clusters_per_event_hist_below_overload_hist, TH1F *events_per_number_of_clusters_per_event_hist, TH1F *max_number_of_clusters_in_RPs_per_event_hist_temp, TH1F *number_of_RPs_close_to_overload_hist_temp, TH1F *histogram, TH1F *histogram_2, TH1F *histogram_up, TH1F *histogram_down, TH1F *close_to_overload_minus_one_hist, TH1F *close_to_overload_minus_two_hist, TH1F *close_to_overload_plus_one_hist, TH1F *close_to_overload_plus_two_hist)
{
	// Test of overload close cases

	int number_of_RPs_close_to_overload = 0 ;

	int number_of_RPs_with_7_uclasters = 0 ;
	int number_of_RPs_with_7_vclasters = 0 ;

	int number_of_RPs_with_6_uclasters = 0 ;
	int number_of_RPs_with_6_vclasters = 0 ;

	int number_of_RPs_with_5_uclasters = 0 ;
	int number_of_RPs_with_5_vclasters = 0 ;

	int number_of_RPs_with_4_uclasters = 0 ;
	int number_of_RPs_with_4_vclasters = 0 ;

	int number_of_RPs_with_3_uclasters = 0 ;
	int number_of_RPs_with_3_vclasters = 0 ;

	int number_of_RPs_with_more_than_5_uclasters = 0 ;
	int number_of_RPs_with_more_than_5_vclasters = 0 ;

	bool close_to_overload = false ;
	bool close_to_overload_up = false ;
	bool close_to_overload_down = false ;

	bool close_to_overload_minus_one = false ;
	bool close_to_overload_minus_two = false ;

	bool close_to_overload_plus_one = false ;
	bool close_to_overload_plus_two = false ;

	int total_number_of_clusters_per_event = 0 ;
	int total_number_of_clusters_below_overload_per_event = 0 ;
	// LEFT FAR

	int total_number_of_clusters = 0 ;
	int total_number_of_clusters_below_overload = 0 ;
	map<int,int> total_number_of_clusters_vector ;

	for(int i = 0 ; i < 10 ; ++i)
	{
		total_number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ;
		total_number_of_clusters_vector[0] += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ;


		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] <= 5) total_number_of_clusters_below_overload += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] == 7)
		{
			if((i % 2) == 0)	number_of_RPs_with_7_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_7_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] == 6)
		{
			if((i % 2) == 0)	number_of_RPs_with_6_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_6_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] == 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_5_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] == 4)
		{
			if((i % 2) == 0)	number_of_RPs_with_4_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_4_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] == 3)
		{
			if((i % 2) == 0)	number_of_RPs_with_3_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_3_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] > 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_more_than_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_more_than_5_uclasters++ ;
		}
	}
	histogram->Fill(total_number_of_clusters) ;

	total_number_of_clusters_per_event += total_number_of_clusters ;
	total_number_of_clusters_below_overload_per_event += total_number_of_clusters_below_overload ;

	if((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))
	{
		close_to_overload = true ;
		number_of_RPs_close_to_overload++ ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_up = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_down = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_minus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_3_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_3_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_minus_two = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_plus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_7_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_7_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_plus_two = true ;
	}

	// LEFT NEAR

	total_number_of_clusters = 0 ;
	total_number_of_clusters_below_overload = 0 ;

        number_of_RPs_with_7_uclasters = 0 ;
        number_of_RPs_with_7_vclasters = 0 ;

        number_of_RPs_with_6_uclasters = 0 ;
        number_of_RPs_with_6_vclasters = 0 ;

	number_of_RPs_with_5_uclasters = 0 ;
	number_of_RPs_with_5_vclasters = 0 ;

	number_of_RPs_with_4_uclasters = 0 ;
	number_of_RPs_with_4_vclasters = 0 ;

	number_of_RPs_with_3_uclasters = 0 ;
	number_of_RPs_with_3_vclasters = 0 ;

	number_of_RPs_with_more_than_5_uclasters = 0 ;
	number_of_RPs_with_more_than_5_vclasters = 0 ;

	for(int i = 0 ; i < 10 ; ++i)
	{
		total_number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ;
		total_number_of_clusters_vector[1] += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] <= 5) total_number_of_clusters_below_overload += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] == 7)
		{
			if((i % 2) == 0)	number_of_RPs_with_7_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_7_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] == 6)
		{
			if((i % 2) == 0)	number_of_RPs_with_6_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_6_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] == 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_5_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] == 4)
		{
			if((i % 2) == 0)	number_of_RPs_with_4_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_4_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] == 3)
		{
			if((i % 2) == 0)	number_of_RPs_with_3_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_3_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] > 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_more_than_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_more_than_5_uclasters++ ;
		}
	}
	histogram->Fill(total_number_of_clusters) ;
	total_number_of_clusters_per_event += total_number_of_clusters ;
	total_number_of_clusters_below_overload_per_event += total_number_of_clusters_below_overload ;

	if((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))
	{
		close_to_overload = true ;
		number_of_RPs_close_to_overload++ ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_up = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_down = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_minus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_3_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_3_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_minus_two = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_plus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_7_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_7_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_plus_two = true ;
	}

	// RIGHT FAR
	
	total_number_of_clusters = 0 ;
	total_number_of_clusters_below_overload = 0 ;

        number_of_RPs_with_7_uclasters = 0 ;
        number_of_RPs_with_7_vclasters = 0 ;

        number_of_RPs_with_6_uclasters = 0 ;
        number_of_RPs_with_6_vclasters = 0 ;

	number_of_RPs_with_5_uclasters = 0 ;
	number_of_RPs_with_5_vclasters = 0 ;

	number_of_RPs_with_4_uclasters = 0 ;
	number_of_RPs_with_4_vclasters = 0 ;

	number_of_RPs_with_3_uclasters = 0 ;
	number_of_RPs_with_3_vclasters = 0 ;

	number_of_RPs_with_more_than_5_uclasters = 0 ;
	number_of_RPs_with_more_than_5_vclasters = 0 ;

	for(int i = 0 ; i < 10 ; ++i)
	{
		total_number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ;
		total_number_of_clusters_vector[2] += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] <= 5) total_number_of_clusters_below_overload += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] == 7)
		{
			if((i % 2) == 0)	number_of_RPs_with_7_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_7_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] == 6)
		{
			if((i % 2) == 0)	number_of_RPs_with_6_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_6_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] == 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_5_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] == 4)
		{
			if((i % 2) == 0)	number_of_RPs_with_4_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_4_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] == 3)
		{
			if((i % 2) == 0)	number_of_RPs_with_3_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_3_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] > 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_more_than_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_more_than_5_uclasters++ ;
		}
	}
	histogram->Fill(total_number_of_clusters) ;
	total_number_of_clusters_per_event += total_number_of_clusters ;
	total_number_of_clusters_below_overload_per_event += total_number_of_clusters_below_overload ;
	
	if((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))
	{
		close_to_overload = true ;
		number_of_RPs_close_to_overload++ ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_up = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_down = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_minus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_3_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_3_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_minus_two = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_plus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_7_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_7_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_plus_two = true ;
	}

	// RIGHT NEAR

	total_number_of_clusters = 0 ;
	total_number_of_clusters_below_overload = 0 ;

        number_of_RPs_with_7_uclasters = 0 ;
        number_of_RPs_with_7_vclasters = 0 ;

        number_of_RPs_with_6_uclasters = 0 ;
        number_of_RPs_with_6_vclasters = 0 ;

	number_of_RPs_with_5_uclasters = 0 ;
	number_of_RPs_with_5_vclasters = 0 ;

	number_of_RPs_with_4_uclasters = 0 ;
	number_of_RPs_with_4_vclasters = 0 ;

	number_of_RPs_with_3_uclasters = 0 ;
	number_of_RPs_with_3_vclasters = 0 ;

	number_of_RPs_with_more_than_5_uclasters = 0 ;
	number_of_RPs_with_more_than_5_vclasters = 0 ;

	for(int i = 0 ; i < 10 ; ++i)
	{
		total_number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ;
		total_number_of_clusters_vector[3] += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] <= 5) total_number_of_clusters_below_overload += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ;

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] == 7)
		{
			if((i % 2) == 0)	number_of_RPs_with_7_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_7_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] == 6)
		{
			if((i % 2) == 0)	number_of_RPs_with_6_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_6_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] == 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_5_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] == 4)
		{
			if((i % 2) == 0)	number_of_RPs_with_4_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_4_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] == 3)
		{
			if((i % 2) == 0)	number_of_RPs_with_3_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_3_uclasters++ ;
		}

		if(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] > 5)
		{
			if((i % 2) == 0)	number_of_RPs_with_more_than_5_vclasters++ ;
			if((i % 2) == 1)	number_of_RPs_with_more_than_5_uclasters++ ;
		}
	}
	histogram->Fill(total_number_of_clusters) ;
	total_number_of_clusters_per_event += total_number_of_clusters ;
	total_number_of_clusters_below_overload_per_event += total_number_of_clusters_below_overload ;
	
	if((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))
	{
		close_to_overload = true ;
		number_of_RPs_close_to_overload++ ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_up = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_down = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_minus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_4_uclasters == 1) && (number_of_RPs_with_4_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_3_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_3_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 2)))
	{
		close_to_overload_minus_two = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
    	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2))) 
	{
		close_to_overload_plus_one = true ;
	}

	if(((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_6_uclasters == 1) && (number_of_RPs_with_6_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 3)) ||
	   ((number_of_RPs_with_5_uclasters == 2) && (number_of_RPs_with_5_vclasters == 3) && (number_of_RPs_with_7_uclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 3) && (number_of_RPs_with_more_than_5_vclasters == 2)) ||
	   ((number_of_RPs_with_5_uclasters == 3) && (number_of_RPs_with_5_vclasters == 2) && (number_of_RPs_with_7_vclasters == 1) && (number_of_RPs_with_more_than_5_uclasters == 2) && (number_of_RPs_with_more_than_5_vclasters == 3)))
	{
		close_to_overload_plus_two = true ;
	}

	if(close_to_overload) histogram_2->Fill(1) ;
	if(close_to_overload_up) histogram_up->Fill(1) ;
	if(close_to_overload_down) histogram_down->Fill(1) ;

	if(close_to_overload_minus_one) close_to_overload_minus_one_hist->Fill(1) ;
	if(close_to_overload_minus_two) close_to_overload_minus_two_hist->Fill(1) ;

	if(close_to_overload_plus_one) close_to_overload_plus_one_hist->Fill(1) ;
	if(close_to_overload_plus_two) close_to_overload_plus_two_hist->Fill(1) ;

	events_per_number_of_clusters_per_event_hist->Fill(total_number_of_clusters_per_event) ;
	events_per_number_of_clusters_per_event_hist_below_overload_hist->Fill(total_number_of_clusters_below_overload_per_event) ;

	if(number_of_RPs_close_to_overload == 1)
	{
		number_of_RPs_close_to_overload_hist_temp->Fill(1) ;
	}

	int max_number_of_clusters_in_RPs_per_event = 0 ;	

	for(int i = 0 ; i < 4 ; ++i)
	{
		if(total_number_of_clusters_vector[i] > max_number_of_clusters_in_RPs_per_event)
		{
			max_number_of_clusters_in_RPs_per_event = total_number_of_clusters_vector[i] ;
		}
	}

	max_number_of_clusters_in_RPs_per_event_hist_temp->Fill(max_number_of_clusters_in_RPs_per_event) ;

}

// Histograms for Simone

TH1F *elastic_events_per_number_of_clusters = NULL ;
TH1F *elastic_events_per_number_of_multitracks = NULL ;
TH1F *events_per_number_of_multitracks = NULL ;
TH1F *events_closest_to_overload = NULL ;
TH1F *elastic_events_closest_to_overload = NULL ;

TH1F *events_per_number_of_clusters = NULL ;

TH1F *events_closest_to_overload_pert_up_1 = NULL ;
TH1F *elastic_events_closest_to_overload_pert_up_1 = NULL ;

TH1F *events_closest_to_overload_pert_down_1 = NULL ;
TH1F *elastic_events_closest_to_overload_pert_down_1 = NULL ;

TH1F *events_close_to_overload_minus_one = NULL ;
TH1F *events_close_to_overload_minus_two = NULL ;

TH1F *elastic_events_close_to_overload_minus_one = NULL ;
TH1F *elastic_events_close_to_overload_minus_two = NULL ;

TH1F *events_close_to_overload_plus_one = NULL ;
TH1F *events_close_to_overload_plus_two = NULL ;

TH1F *elastic_events_close_to_overload_plus_one = NULL ;
TH1F *elastic_events_close_to_overload_plus_two = NULL ;

TH1F *number_of_clusters_per_plane = NULL ;

TH1F *events_per_number_of_clusters_per_event = NULL ;
TH1F *elastic_events_per_number_of_clusters_per_event = NULL ;
TH1F *max_number_of_clusters_in_RPs_per_event_hist = NULL ;
TH1F *elastic_max_number_of_clusters_in_RPs_per_event_hist = NULL ;

TH1D *normalization_for_3_out_of_4_most_general_case = NULL ;
TH1D *normalization_for_2_out_of_4_most_general_case = NULL ;

TH1F *number_of_cluster_for_elastic_events_below_threshold = NULL ;

TH1F *events_per_number_of_clusters_per_below_overload_per_event = NULL ;
TH1F *elastic_events_per_number_of_clusters_per_below_overload_per_event = NULL ;

TH2F *number_of_clusters_vs_t_distribution = NULL ;

bool empty_pot_condition_trigger(UInt_t uPlanesOn, UInt_t vPlanesOn)
{
	return ((uPlanesOn < 3) && (vPlanesOn < 3)) ;
}

TH1D theta_y_star_difference = TH1D("theta_y_star_difference", "theta_y_star_difference", 100, -1e-3, 1e-3) ;
TH1D theta_x_star_difference = TH1D("theta_x_star_difference", "theta_x_star_difference", 100, -1e-3, 1e-3) ;

TF1 *function_3_out_of_4_ratios_file_left_far = NULL ;
TF1 *function_3_out_of_4_ratios_file_left_near = NULL ;
TF1 *function_3_out_of_4_ratios_file_right_far = NULL ;
TF1 *function_3_out_of_4_ratios_file_right_near = NULL ;

void TReducedNtuple::Loop(TProjectParameters *ProjectParameters, TProtonReconstruction *ProtonReconstruction, map_from_string_to_PTPlotsCollection_type *PlotsCollections, string collection_per_run_name, TProjectInformation *ProjectInformation, int index_of_root_file)
{
	// For single tracking standard 3/4

	// For multitracking 3/4 empty pot

	TH1D *empty_pot_case_Mul_Mul_Mul_E_hist = NULL ;
	TH1D *empty_pot_case_Mul_E_Mul_E_hist = NULL ;
	TH1D *overload_inefficiency_correction_hist = NULL ;

	TH1D *empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters = NULL ;
	TH1D *empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters = NULL ;

	if(ProjectParameters->IsSettingDefined("3_out_4_inefficiency_correction"))
	{
		if(multitrack)
		{
			TFile *empty_pot_case_Mul_Mul_Mul_E_file = new TFile(ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction").c_str(), "READ") ;

			if(empty_pot_case_Mul_Mul_Mul_E_file->IsZombie())
			{
				cout << "Error in TReducedNtuple::Loop: the .root file for 3 out of 4 inefficiency calculation cannot be opened: " << ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction") << endl ;
				exit(1) ;
			}

			empty_pot_case_Mul_Mul_Mul_E_hist = (TH1D *)empty_pot_case_Mul_Mul_Mul_E_file->Get("empty_pot_case_Mul_Mul_Mul_E_hist") ;

			if(empty_pot_case_Mul_Mul_Mul_E_hist == NULL)
			{
				cout << "Error in TReducedNtuple::Loop: the histogram empty_pot_case_Mul_Mul_Mul_E_hist cannot be found in .root file: " << ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction") << endl ;
				exit(1) ;
			}
		}
		else if(function_3_out_of_4_ratios_file_left_far == NULL)
		{
			string diagonal_code = "" ;

			if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0))
			{
				diagonal_code = "LBRT" ;
			}

			if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0))
			{
				diagonal_code = "LTRB" ;
			}

			string main_directory = ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction") ;

			string detector_name = ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector") ;
			std::replace(detector_name.begin(), detector_name.end(), '.', '_'); 
			string histogram_3_out_of_4_ratios_file_left_far = main_directory + "/" + diagonal_code + "_3_out_of_4_ratio_" + detector_name + ".root" ;
			TFile *histogram_3_out_of_4_ratios_file_left_far_file = new TFile(histogram_3_out_of_4_ratios_file_left_far.c_str(), "READ") ;
			function_3_out_of_4_ratios_file_left_far = ((TH1D *)histogram_3_out_of_4_ratios_file_left_far_file->Get((diagonal_code + "_3_out_of_4_ratio_" + detector_name).c_str()))->GetFunction("pol1") ;

			detector_name = ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector") ;
			std::replace(detector_name.begin(), detector_name.end(), '.', '_'); 
			string histogram_3_out_of_4_ratios_file_left_near = main_directory + "/" + diagonal_code + "_3_out_of_4_ratio_" + detector_name + ".root" ;
			TFile *histogram_3_out_of_4_ratios_file_left_near_file = new TFile(histogram_3_out_of_4_ratios_file_left_near.c_str(), "READ") ;
			function_3_out_of_4_ratios_file_left_near = ((TH1D *)histogram_3_out_of_4_ratios_file_left_near_file->Get((diagonal_code + "_3_out_of_4_ratio_" + detector_name).c_str()))->GetFunction("pol1") ;

			detector_name = ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector") ;
			std::replace(detector_name.begin(), detector_name.end(), '.', '_'); 
			string histogram_3_out_of_4_ratios_file_right_far = main_directory + "/" + diagonal_code + "_3_out_of_4_ratio_" + detector_name + ".root" ;
			TFile *histogram_3_out_of_4_ratios_file_right_far_file = new TFile(histogram_3_out_of_4_ratios_file_right_far.c_str(), "READ") ;
			function_3_out_of_4_ratios_file_right_far = ((TH1D *)histogram_3_out_of_4_ratios_file_right_far_file->Get((diagonal_code + "_3_out_of_4_ratio_" + detector_name).c_str()))->GetFunction("pol1") ;

			detector_name = ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector") ;
			std::replace(detector_name.begin(), detector_name.end(), '.', '_'); 
			string histogram_3_out_of_4_ratios_file_right_near = main_directory + "/" + diagonal_code + "_3_out_of_4_ratio_" + detector_name + ".root" ;
			TFile *histogram_3_out_of_4_ratios_file_right_near_file = new TFile(histogram_3_out_of_4_ratios_file_right_near.c_str(), "READ") ;
			function_3_out_of_4_ratios_file_right_near = ((TH1D *)histogram_3_out_of_4_ratios_file_right_near_file->Get((diagonal_code + "_3_out_of_4_ratio_" + detector_name).c_str()))->GetFunction("pol1") ;

			if(histogram_3_out_of_4_ratios_file_left_far_file->IsZombie() || histogram_3_out_of_4_ratios_file_left_near_file->IsZombie() || histogram_3_out_of_4_ratios_file_right_far_file->IsZombie() || histogram_3_out_of_4_ratios_file_right_near_file->IsZombie())
			{
				cout << "Error: 3_out_of_4_ratio file cannot be opened!" << endl ;
				exit(1) ;
			}

		}


	}

	if(ProjectParameters->IsSettingDefined("2_out_4_inefficiency_correction"))
	{
		TFile *empty_pot_case_Mul_E_Mul_E_file = new TFile(ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction").c_str(), "READ") ;

		if(empty_pot_case_Mul_E_Mul_E_file->IsZombie())
		{
			cout << "Error in TReducedNtuple::Loop: the .root file for 2 out of 4 inefficiency calculation cannot be opened: " << ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction") << endl ;
			exit(1) ;
		}

		empty_pot_case_Mul_E_Mul_E_hist = (TH1D *)empty_pot_case_Mul_E_Mul_E_file->Get("empty_pot_case_Mul_E_Mul_E_hist") ;

		if(empty_pot_case_Mul_E_Mul_E_hist == NULL)
		{
			cout << "Error in TReducedNtuple::Loop: the histogram empty_pot_case_Mul_E_Mul_E_hist cannot be found in .root file: " << ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction") << endl ;
			exit(1) ;
		}
		// cout << "I have the 2/4!" << endl ;
		// exit(1) ;
	}

	if(ProjectParameters->IsSettingDefined("3_out_4_inefficiency_correction_clusters"))
	{
		TFile *empty_pot_case_Mul_Mul_Mul_E_file = new TFile(ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction_clusters").c_str(), "READ") ;

		if(empty_pot_case_Mul_Mul_Mul_E_file->IsZombie())
		{
			cout << "Error in TReducedNtuple::Loop: the .root file for 3 out of 4 inefficiency calculation cannot be opened: " << ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction_clusters") << endl ;
			exit(1) ;
		}

		empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters = (TH1D *)empty_pot_case_Mul_Mul_Mul_E_file->Get("empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters") ;
		
		if(empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters == NULL)
		{
			cout << "Error in TReducedNtuple::Loop: the histogram empty_pot_case_Mul_Mul_Mul_E_hist cannot be found in .root file: " << ProjectParameters->GetSettingValue("3_out_4_inefficiency_correction_clusters") << endl ;
			exit(1) ;
		}
	}

	if(ProjectParameters->IsSettingDefined("2_out_4_inefficiency_correction_clusters"))
	{
		TFile *empty_pot_case_Mul_E_Mul_E_file = new TFile(ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction_clusters").c_str(), "READ") ;

		if(empty_pot_case_Mul_E_Mul_E_file->IsZombie())
		{
			cout << "Error in TReducedNtuple::Loop: the .root file for 2 out of 4 inefficiency calculation cannot be opened: " << ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction_clusters") << endl ;
			exit(1) ;
		}

		empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters = (TH1D *)empty_pot_case_Mul_E_Mul_E_file->Get("empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters") ;

		if(empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters == NULL)
		{
			cout << "Error in TReducedNtuple::Loop: the histogram empty_pot_case_Mul_E_Mul_E_hist cannot be found in .root file: " << ProjectParameters->GetSettingValue("2_out_4_inefficiency_correction_clusters") << endl ;
			exit(1) ;
		}
		// cout << "I have the 2/4!" << endl ;
		// exit(1) ;
	}

	if(ProjectParameters->IsSettingDefined("overload_inefficiency_correction"))
	{
		TFile *overload_inefficiency_correction_file = new TFile(ProjectParameters->GetSettingValue("overload_inefficiency_correction").c_str(), "READ") ;

		if(overload_inefficiency_correction_file->IsZombie())
		{
			cout << "Error in TReducedNtuple::Loop: the .root file for overload inefficiency calculation cannot be opened: " << ProjectParameters->GetSettingValue("overload_inefficiency_correction") << endl ;
			exit(1) ;
		}

		overload_inefficiency_correction_hist = (TH1D *)overload_inefficiency_correction_file->Get("number_of_clusters_below_threshold_hist") ;

		if(overload_inefficiency_correction_hist == NULL)
		{
			cout << "Error in TReducedNtuple::Loop: the histogram number_of_clusters_below_threshold_hist cannot be found in .root file: " << ProjectParameters->GetSettingValue("overload_inefficiency_correction") << endl ;
			exit(1) ;
		}
		// cout << "I have the 2/4!" << endl ;
		// exit(1) ;
	}

	bool lookup_table_exist = false ;

	if(ProjectParameters->IsParameterDefined("lookup_table"))
	{
		if(ProjectParameters->GetParameterValue("lookup_table") == 1.0) lookup_table_exist = true ;
	}

	if(multitrack && lookup_table_exist)
	{
		if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0) )
		{
			ULong_t run_number, event_number ;
			TLookUpInformation LookUpInformation ;

			ifstream lookup_table("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_9009_LBRT.txt") ;

			string word ; 
			while(lookup_table >> word)
			{
				lookup_table >> run_number >> event_number >> LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				map_lookup_table[run_number_event_number_pair] = LookUpInformation ;

			}

			ifstream lookup_table2("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_9010_LBRT.txt") ;

			while(lookup_table2 >> word)
			{
				lookup_table2 >> run_number >> event_number >> LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				map_lookup_table[run_number_event_number_pair] = LookUpInformation ;

			}

			ifstream lookup_table3("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_number_of_clusters/lookup_table_number_of_clusters_LBRT.txt") ;

			while(lookup_table3 >> word)
			{
				lookup_table3 >> run_number >> event_number ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_uPlanesOn ;

				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_vPlanesOn ;

				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_left_far_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_left_near_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_right_near_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_right_far_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ; } ;


				// LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;

				// cout << run_number << "  " << event_number << endl ;
				// cout << map_lookup_table[run_number_event_number_pair].digi_rp_right_far_vPlanesOn << endl ;
				// cout << map_lookup_table[run_number_event_number_pair].digi_rp_right_far_numberOfClusters[3] << endl ;

			}

		}

		if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0) )
		{
			ULong_t run_number, event_number ;
			TLookUpInformation LookUpInformation ;

			ifstream lookup_table("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_9009_LTRB.txt") ;

			string word ; 
			while(lookup_table >> word)
			{
				lookup_table >> run_number >> event_number >> LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				map_lookup_table[run_number_event_number_pair] = LookUpInformation ;

			}

			ifstream lookup_table2("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_9010_LTRB.txt") ;

			while(lookup_table2 >> word)
			{
				lookup_table2 >> run_number >> event_number >> LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				map_lookup_table[run_number_event_number_pair] = LookUpInformation ;

			}
			ifstream lookup_table3("/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_2p76_TeV_Beta_star_11_m/2013_02_11_data/Reduced_ntuples/2016_10_14_Multitrack/2016_10_26_no_single_track_preselection_in_b2_RP_trigger_on/Lookup_table_number_of_clusters/lookup_table_number_of_clusters_LTRB.txt") ;

			while(lookup_table3 >> word)
			{
				lookup_table3 >> run_number >> event_number ;

				run_number_event_number_pair_type run_number_event_number_pair(run_number, event_number) ;

				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_uPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_uPlanesOn ;

				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_vPlanesOn ;
				lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_vPlanesOn ;

				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_left_far_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_left_near_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_right_near_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ; } ;
				for(int i = 0 ; i < 10 ; ++i)	{ map_lookup_table[run_number_event_number_pair].digi_rp_right_far_numberOfClusters.push_back(0) ; lookup_table3 >> map_lookup_table[run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ; } ;


				// LookUpInformation.number_of_event_left_far >> LookUpInformation.number_of_event_left_near >> LookUpInformation.number_of_event_right_near >> LookUpInformation.number_of_event_right_far ;



			}
		}
	}

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	// Required and excluded trigger bits

	string required_trigger_bit_name = ProjectParameters->GetSettingValue("required_trigger_bit_name") ;
	int required_trigger_bit = pow(2, ProjectParameters->GetBlock("TOTEM_trigger_bits_block")->GetParameterValue(required_trigger_bit_name)) ;

	// string excluded_trigger_bit_name = ProjectParameters->GetSettingValue("excluded_trigger_bit_name") ;
	// int excluded_trigger_bit = pow(2, ProjectParameters->GetBlock("TOTEM_trigger_bits_block")->GetParameterValue(excluded_trigger_bit_name)) ;

	// Required bunches stored to a map

	map<int, bool> trigger_data_bunch_nums ;

	if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger_data_bunch_num") == 1.0)
	{
		for(vector<double>::iterator it = ProjectParameters->GetParametersVector("trigger_data_bunch_num")->begin() ; it != ProjectParameters->GetParametersVector("trigger_data_bunch_num")->end() ; ++it)
		{
   			trigger_data_bunch_nums[*it] = true ;
		}
	}

	vector_TProtonreconstruction_type vector_TProtonreconstruction ;

	run_number_event_number_pair_type prev_run_number_event_number_pair(0, 0) ;
	bool prev_run_number_event_number_pair_is_set = false ;
	UInt_t prev_trigger_data_input_status_bits = 0 ;

	int number_of_elastic_1 = 0 ;
	int number_of_elastic_2 = 0 ;

	long int number_of_events = 0 ;

	double value_2_out_4_same_sector_inefficiency_correction_fraction = 0.0 ;

	if(ProjectParameters->IsParameterDefined("2_out_4_same_sector_inefficiency_correction_percent"))
	{
		value_2_out_4_same_sector_inefficiency_correction_fraction = (ProjectParameters->GetParameterValue("2_out_4_same_sector_inefficiency_correction_percent") / 100.0) ;
	}

	bool Force_valid_4_tracks_in_3_out_of_4 = false ;
	double inefficiency_different_sector_2_4 = 0 ;

	if(ProjectParameters->IsParameterDefined("Force_valid_4_tracks_in_3_out_of_4"))
	{
		if(ProjectParameters->GetParameterValue("Force_valid_4_tracks_in_3_out_of_4") == 1.0) Force_valid_4_tracks_in_3_out_of_4 = true ;
	}

        bool a_2_far_RP_analysis = false ;

	vector<UInt_t> vector_rejected_events ;

        if(ProjectParameters->IsParameterDefined("a_2_far_RP_analysis"))
        {
                if(ProjectParameters->GetParameterValue("a_2_far_RP_analysis") == 1.0) a_2_far_RP_analysis = true ;

		if(ProjectParameters->IsSettingDefined("file_rejected_events"))
		{
			string filename = ProjectParameters->GetSettingValue("file_rejected_events") ;
			ifstream file_rejected_events(filename.c_str()) ;

			string word, tmp_event_num ;

			while(file_rejected_events >> word >> word >> word >> tmp_event_num >> word)
			{
				UInt_t tmp_event_num_i = atoi(tmp_event_num.c_str())  ;
				cout << "tmp_event_num " << tmp_event_num << endl ;
				vector_rejected_events.push_back(tmp_event_num_i) ;
			}

			file_rejected_events.close() ;
		}
        }

	if(ProjectParameters->IsParameterDefined("inefficiency_different_sector_2_4"))
	{
		inefficiency_different_sector_2_4 = ProjectParameters->GetParameterValue("inefficiency_different_sector_2_4") ;

		if((empty_pot_case_Mul_Mul_Mul_E_hist != NULL) || (empty_pot_case_Mul_E_Mul_E_hist != NULL) || (empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters != NULL) ||
	  		(empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters != NULL) || (overload_inefficiency_correction_hist != NULL))
		{
			cout << "Error: TotemNtuple::Loop: inefficiency corrections may interfere or double count !" << endl ;
			exit(1) ;
		}


	}

        bool Inefficiency_3_out_of_4_RP_Exclude_Detector = false ;

	if(ProjectParameters->IsParameterDefined("Inefficiency_3_out_of_4_RP_Exclude_Detector"))
	{
		if(ProjectParameters->GetParameterValue("Inefficiency_3_out_of_4_RP_Exclude_Detector") == 1.0)
		{	
			Inefficiency_3_out_of_4_RP_Exclude_Detector = true ;
		}
	}

	for(Long64_t jentry=0; jentry < nentries; jentry++)
	{
		fChain->GetEntry(jentry);
		
		// cout << "run_num_event_num " << trigger_data_run_num << "  " << trigger_data_event_num << endl ; 

		if(std::find(vector_rejected_events.begin(), vector_rejected_events.end(), trigger_data_event_num) != vector_rejected_events.end())
		{
			cout << "nowcont" << endl ;
			continue ;
		}
		
		// cout << "." << endl ;

		if(ProjectInformation->valid)
		{
			if(ProjectInformation->event_info_timestamp_start > event_info_timestamp) ProjectInformation->event_info_timestamp_start = event_info_timestamp ;
			if(ProjectInformation->event_info_timestamp_end   < event_info_timestamp) ProjectInformation->event_info_timestamp_end	 = event_info_timestamp ;
		} else
		{
			ProjectInformation->event_info_timestamp_start = event_info_timestamp ;
			ProjectInformation->event_info_timestamp_end   = event_info_timestamp ;
			
			ProjectInformation->valid = true ;
		}

		if((ProjectParameters->GetParameterValue("MaxNumberOfEventsToAnalyzeIsSet") == 1.0) && (jentry > ProjectParameters->GetParameterValue("MaxNumberOfEventsToAnalyze"))) break ;

		bool bunch_is_triggered = false ;

		if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger_data_bunch_num") == 1.0)
		{
			map<int, bool>::iterator it ;
			it = trigger_data_bunch_nums.find(trigger_data_bunch_num) ;
			if (it != trigger_data_bunch_nums.end()) bunch_is_triggered = true ;
		}
		else
		{
			bunch_is_triggered = true ;
		}

		if(bunch_is_triggered)
		{

			bool trigger_is_ok = false ;

			if((UInt_t)ProjectParameters->GetParameterValue("Ask_for_trigger") == 1.0)
			{
				bool required_trigger_bit_is_set = ((trigger_data_input_status_bits & required_trigger_bit) != 0) ;
				// bool excluded_trigger_bit_is_set = ((trigger_data_input_status_bits & excluded_trigger_bit) != 0) ;

				if(required_trigger_bit_is_set) trigger_is_ok = true ;
			}
			else
			{
				trigger_is_ok = true ;
			}

			if(trigger_is_ok) 
			{

				bool track_valid = false ;
				int number_of_valid_tracks = 4 ;

				if(ProjectParameters->IsSettingDefined("Inefficiency_3_out_of_4_RP") && (!Force_valid_4_tracks_in_3_out_of_4))
				{
					if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6R5.B1") == 0.0)	 track_valid = ((track_left_far_valid && track_left_near_valid && track_right_far_valid)   && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_right_near_valid))) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6R5.B1") == 0.0) track_valid = ((track_left_far_valid && track_left_near_valid && track_right_near_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_right_far_valid))) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.A6L5.B2") == 0.0) track_valid = ((track_left_far_valid && track_right_far_valid && track_right_near_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_left_near_valid))) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.B6L5.B2") == 0.0) track_valid = ((track_left_near_valid && track_right_far_valid && track_right_near_valid) && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_left_far_valid))) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.D6L5.B2") == 0.0) track_valid = ((track_left_far_valid && track_right_far_valid && track_right_near_valid)  && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_left_near_valid))) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP").compare("XRPV.D6R5.B1") == 0.0) track_valid = ((track_left_far_valid && track_left_near_valid && track_right_far_valid)   && ((!Inefficiency_3_out_of_4_RP_Exclude_Detector) || (!track_right_near_valid))) ;
					else
					{
						cout << "Error: TotemNtuple::Loop: Unknown device: " << ProjectParameters->GetSettingValue("Inefficiency_3_out_of_4_RP") << " !" << endl ;
						exit(1) ;
					}
				}
				else if(ProjectParameters->IsSettingDefined("Inefficiency_2_out_of_4_RP_far") && (!Force_valid_4_tracks_in_3_out_of_4))
				{
					if(ProjectParameters->GetSettingValue("Inefficiency_2_out_of_4_RP_far").compare("XRPV.B6R5.B1") == 0.0)	 track_valid = (track_left_far_valid && track_left_near_valid) ;
					else if(ProjectParameters->GetSettingValue("Inefficiency_2_out_of_4_RP_far").compare("XRPV.B6L5.B2") == 0.0) track_valid = (track_right_far_valid && track_right_near_valid) ;
					else
					{
						cout << "Error: TotemNtuple::Loop: Unknown device: " << ProjectParameters->GetSettingValue("Inefficiency_2_out_of_4_RP_far") << " !" << endl ;
						exit(1) ;
					}
				}
				else if(a_2_far_RP_analysis)
                                {
					track_valid = track_left_far_valid && track_right_far_valid ;

					if(!(track_left_far_valid && track_left_near_valid && track_right_far_valid && track_right_near_valid))
					{
						number_of_valid_tracks = 2 ;
					}
                                }
				else track_valid = (track_left_far_valid && track_left_near_valid && track_right_far_valid && track_right_near_valid) ;

				// cout << track_left_far_valid << "  " << track_left_near_valid << " " << track_right_far_valid << "  " << track_right_near_valid << endl ;
	
				double probability_of_candidate = (1.0 ) ;
				// cout << "number_of_occurances: " << number_of_occurances << endl ;

				run_number_event_number_pair_type run_number_event_number_pair(trigger_data_run_num, trigger_data_event_num) ;
				// cout << "run_number_event_number_pair " << run_number_event_number_pair.first << "  " << run_number_event_number_pair.second << endl ;

				if(trigger_data_event_num == 3200)
				{
					// cout << "Found 1: 3200 " << endl ;
				}

				if((run_number_event_number_pair != prev_run_number_event_number_pair) || !prev_run_number_event_number_pair_is_set)
				{
					bool elastic_is_stored = false ;

					bool logical_condition = true ;

					if(multitrack && lookup_table_exist) logical_condition = EvaluateLogicalCondition(ProjectParameters, prev_run_number_event_number_pair) ;

					int number_of_combinations = map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far  * map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near * map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near * map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far ;
					// cout << "number_of_combinations: " << number_of_combinations << endl ;

					if(multitrack && lookup_table_exist && (number_of_combinations != vector_TProtonreconstruction.size()))
					{
						// #####################################################
						// #####################################################
						// #####################################################
						// PUT THIS BACK 2016.11.22  (DONE)
						// #####################################################
						// #####################################################
						// #####################################################

						cout << "Error in TReducedNtuple::Loop: number_of_combinations != vector_TProtonreconstruction.size() !" << endl ;
						exit(1) ;
					}

					if(logical_condition)
					{
						number_of_events++ ;
						// ProjectInformation->trigger_bits[]
						// cout << "Logical condition is ok." << endl ;

						for(int i = 0 ; i < 16 ; ++i)
						{
							// cout << trigger_data_input_status_bits << endl ;

							if((trigger_data_input_status_bits & (ProjectInformation->trigger_bit_mask[i])) != 0)
							{
								// cout << "Bit " << i << "is set" <<  endl ;
								ProjectInformation->trigger_bits[i]++ ;
							}
						}
					}

					for(int i = 0 ; i < vector_TProtonreconstruction.size() ; ++i)
					{
						double weight = 1.0 ;

						*ProtonReconstruction  = vector_TProtonreconstruction[i] ;

						// #####################################################
						// #####################################################
						// #####################################################
						// PUT THE CONDITION BELOW BACK 2016.11.22 (DONE)
						// #####################################################
						// #####################################################
						// #####################################################

						if(logical_condition)
						{
							if(!ProtonReconstruction->IsElastic()) (*PlotsCollections)["PlotsCollection"]->Fill(ProtonReconstruction, weight) ;

							if(ProtonReconstruction->IsElastic() && !elastic_is_stored) 
							{
								// cout << "trigger_data_bunch_num: " << trigger_data_bunch_num << endl ;

								for(int i = 0 ; i < 16 ; ++i)
								{
									// cout << trigger_data_input_status_bits << endl ;

									if((trigger_data_input_status_bits & (ProjectInformation->trigger_bit_mask[i])) != 0)
									{
										// cout << "Bit " << i << "is set" <<  endl ;
										ProjectInformation->elastic_trigger_bits[i]++ ;
									}
								}


								bool is_RP_trigger_bit_on = (prev_trigger_data_input_status_bits & 1) ;

								// cout << "Within elastic: prev_trigger_data_input_status_bits: " << prev_trigger_data_input_status_bits << "   is_RP_trigger_bit_on: " << is_RP_trigger_bit_on << endl ;
								// cout << "prev_run_number_event_number_pair: " << prev_run_number_event_number_pair.first << "  " << prev_run_number_event_number_pair.second << endl ;
								// cout << "run_number_event_number_pair: " << run_number_event_number_pair.first << "  " << run_number_event_number_pair.second << endl ;

								double number_of_clusters = 0 ;

								if(multitrack)
								{

									if(lookup_table_exist)
									{
										for(int i = 0 ; i < 10 ; ++i)
										{
											number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ;
											number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ;
											number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ;
											number_of_clusters += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ;

										}
									}
									
									//cout << "After clusters" << endl ;

									double inefficiency_fraction = 0 ;

									if(empty_pot_case_Mul_Mul_Mul_E_hist != NULL)
									{
										int n_multitrack_3_out_4_inefficiency = N_multitrack_3_out_4_inefficiency(map_lookup_table[prev_run_number_event_number_pair]) ;

										// cout << "n_multitrack_3_out_4_inefficiency: " << n_multitrack_3_out_4_inefficiency << endl ;

										inefficiency_fraction += empty_pot_case_Mul_Mul_Mul_E_hist->GetBinContent(n_multitrack_3_out_4_inefficiency + 1) ;

										cout << "inefficiency_fraction1: " << inefficiency_fraction << endl ;
										// cout << "weight: " << weight << endl ;

									}

									if(empty_pot_case_Mul_E_Mul_E_hist != NULL)
									{
										int n_multitrack_2_out_4_inefficiency = N_multitrack_2_out_4_inefficiency(map_lookup_table[prev_run_number_event_number_pair]) ;

										// cout << "n_multitrack_3_out_4_inefficiency: " << n_multitrack_3_out_4_inefficiency << endl ;

										inefficiency_fraction += empty_pot_case_Mul_E_Mul_E_hist->GetBinContent(n_multitrack_2_out_4_inefficiency + 1) ;

										cout << "inefficiency_fraction2: " << inefficiency_fraction << endl ;
										// cout << "weight: " << weight << endl ;


									}

									if(empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters != NULL)
									{
										int n_multitrack_3_out_4_inefficiency = (0.75 * number_of_clusters);

										// cout << "n_multitrack_3_out_4_inefficiency: " << n_multitrack_3_out_4_inefficiency << endl ;

										inefficiency_fraction += empty_pot_case_Mul_Mul_Mul_E_hist_number_of_clusters->GetBinContent(n_multitrack_3_out_4_inefficiency + 1) ;

										// cout << "inefficiency_fraction1: " << inefficiency_fraction << endl ;
										// cout << "weight: " << weight << endl ;
										cout << "Hi" << endl ;

									}
									else
									{
										cout << "Ni" << endl ;
									}	

									if(empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters != NULL)
									{
										int n_multitrack_2_out_4_inefficiency = (0.5 * number_of_clusters);

										// cout << "n_multitrack_3_out_4_inefficiency: " << n_multitrack_3_out_4_inefficiency << endl ;

										inefficiency_fraction += empty_pot_case_Mul_E_Mul_E_hist_number_of_clusters->GetBinContent(n_multitrack_2_out_4_inefficiency + 1) ;

										// cout << "inefficiency_fraction2: " << inefficiency_fraction << endl ;
										// cout << "weight: " << weight << endl ;


									}

									if(overload_inefficiency_correction_hist != NULL)
									{
										inefficiency_fraction += overload_inefficiency_correction_hist->GetBinContent(number_of_clusters + 1) ;
										cout << "inefficiency_fraction3: " << inefficiency_fraction << endl ;
									}

									weight = weight * (1.0 + inefficiency_fraction + value_2_out_4_same_sector_inefficiency_correction_fraction) ; 
								}
								else
								{

									if(ProjectParameters->IsSettingDefined("3_out_4_inefficiency_correction"))
									{
										double theta_y_star_left_rad =  (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_left_rad")) ;
										double theta_y_star_right_rad = (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_right_rad")) ;

										double inefficiency_left_far 	= (1.0 - function_3_out_of_4_ratios_file_left_far->Eval(theta_y_star_right_rad)) ;
										double inefficiency_left_near 	= (1.0 - function_3_out_of_4_ratios_file_left_near->Eval(theta_y_star_right_rad)) ;
										double inefficiency_right_far 	= (1.0 - function_3_out_of_4_ratios_file_right_far->Eval(theta_y_star_left_rad));
										double inefficiency_right_near 	= (1.0 - function_3_out_of_4_ratios_file_right_near->Eval(theta_y_star_left_rad)) ;

										if((inefficiency_left_far > 1.0) || (inefficiency_left_near > 1.0) || (inefficiency_right_far > 1.0) || (inefficiency_right_near > 1.0))
										{
											cout << "Error in TReducedNtuple::Loop: efficiency is greater than 1.0 !" << endl ;
											// exit(1) ;
										}

										Double_t min, max ;
										function_3_out_of_4_ratios_file_left_far->GetRange(min, max) ;

										if((theta_y_star_right_rad / (min)) < 0)
										{
											cout << "Error in TReducedNtuple::Loop: inconsistency in elastic status !" << endl ;
											exit(1) ;
										}

										// cout << "inefficiency_left_far: " << inefficiency_left_far << endl ;
										// cout << "inefficiency_left_near: " << inefficiency_left_near << endl  ;
										// cout << "inefficiency_right_far: " << inefficiency_right_far << endl  ;
										// cout << "inefficiency_right_near: " << inefficiency_right_near << endl  ;

										weight = (1.0 / (1.0 - (inefficiency_left_far + inefficiency_left_near + inefficiency_right_far + inefficiency_right_near + inefficiency_different_sector_2_4))) ; 

										// cout << "weight_for_3_4 : " << weight << endl ;
									}

								}

								bool status = (*PlotsCollections)["PlotsCollection"]->Fill(ProtonReconstruction, weight) ;
								elastic_is_stored = true ;

								if(multitrack && lookup_table_exist)
								{
								
									elastic_events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far) ;
									elastic_events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near) ;
									elastic_events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far) ;
									elastic_events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near) ;

									if(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far == 20)
									{
										int total_number_of_cluster = 0 ;
										for(int i = 0 ; i < 10 ; ++i)
										{
											total_number_of_cluster += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i] ;
										}

										number_of_cluster_for_elastic_events_below_threshold->Fill(total_number_of_cluster) ;
									}

									if(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near == 20)
									{
										int total_number_of_cluster = 0 ;
										for(int i = 0 ; i < 10 ; ++i)
										{
											total_number_of_cluster += map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i] ;
										}

										number_of_cluster_for_elastic_events_below_threshold->Fill(total_number_of_cluster) ;
									}

									if(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far == 20)
									{
										int total_number_of_cluster = 0 ;
										for(int i = 0 ; i < 10 ; ++i)
										{
											total_number_of_cluster += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i] ;
										}

										number_of_cluster_for_elastic_events_below_threshold->Fill(total_number_of_cluster) ;
									}

									if(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near == 20)
									{
										int total_number_of_cluster = 0 ;
										for(int i = 0 ; i < 10 ; ++i)
										{
											total_number_of_cluster += map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i] ;
										}

										number_of_cluster_for_elastic_events_below_threshold->Fill(total_number_of_cluster) ;
									}

									int sum_number_of_tracks = map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far + map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near + map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far + map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near ;

									elastic_events_per_number_of_multitracks->Fill(sum_number_of_tracks) ;

									FillNumberOfClusters(prev_run_number_event_number_pair, elastic_events_per_number_of_clusters_per_below_overload_per_event, elastic_events_per_number_of_clusters_per_event, elastic_max_number_of_clusters_in_RPs_per_event_hist , elastic_number_of_RPs_close_to_overload_hist, elastic_events_per_number_of_clusters, elastic_events_closest_to_overload, elastic_events_closest_to_overload_pert_up_1, elastic_events_closest_to_overload_pert_down_1, elastic_events_close_to_overload_minus_one, elastic_events_close_to_overload_minus_two,  elastic_events_close_to_overload_plus_one, elastic_events_close_to_overload_plus_two) ;

									if(!status)
									{
										cout << "Error in TReducedNtuple::Loop: inconsistency in elastic status !" << endl ;
										exit(1) ;
									}

									number_of_clusters_vs_t_distribution->Fill(*ProtonReconstruction->GetReconstructedVariable("minus_t_GeV2"), number_of_clusters) ;								
								}
							}
						}
	
						if(ProtonReconstruction->IsElastic()) ++number_of_elastic_2 ;
					}

					// ==== Clusterization
					
					
					if(multitrack && lookup_table_exist && prev_run_number_event_number_pair_is_set)
					{
						FillNumberOfClusters(prev_run_number_event_number_pair, events_per_number_of_clusters_per_below_overload_per_event, events_per_number_of_clusters_per_event, max_number_of_clusters_in_RPs_per_event_hist, number_of_RPs_close_to_overload_hist, events_per_number_of_clusters, events_closest_to_overload, events_closest_to_overload_pert_up_1, events_closest_to_overload_pert_down_1, events_close_to_overload_minus_one, events_close_to_overload_minus_two, events_close_to_overload_plus_one, events_close_to_overload_plus_two) ;

						for(int i = 0 ; i < 10 ; ++i)
						{
							number_of_clusters_per_plane->Fill(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_far_numberOfClusters[i]) ;
							number_of_clusters_per_plane->Fill(map_lookup_table[prev_run_number_event_number_pair].digi_rp_left_near_numberOfClusters[i]) ;
							number_of_clusters_per_plane->Fill(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_near_numberOfClusters[i]) ;
							number_of_clusters_per_plane->Fill(map_lookup_table[prev_run_number_event_number_pair].digi_rp_right_far_numberOfClusters[i]) ;

						}

						int sum_number_of_tracks = map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far + map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near + map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far + map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near ;

						events_per_number_of_multitracks->Fill(sum_number_of_tracks) ;

						events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_far) ;
						events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_left_near) ;
						events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_far) ;
						events_per_multiplicity->Fill(map_lookup_table[prev_run_number_event_number_pair].number_of_event_right_near) ;
						
						normalization_for_3_out_of_4_most_general_case->Fill(N_multitrack_3_out_4_inefficiency(map_lookup_table[prev_run_number_event_number_pair])) ;
						normalization_for_2_out_of_4_most_general_case->Fill(N_multitrack_2_out_4_inefficiency(map_lookup_table[prev_run_number_event_number_pair])) ;

					}
					

					// if(elastic_is_stored && logical_condition) cout << "Elastic_events_per_multiplicity: " << vector_TProtonreconstruction.size() << endl ;

					if(number_of_elastic_1 != number_of_elastic_2)
					{
						cout << "number_of_elastic_1 != number_of_elastic_2" << endl ;
						exit(1) ;
					}

					// if(number_of_elastic_1 == 1) cout << "Singlesolution" << endl ;
					// if(number_of_elastic_1 > 1) cout << "Multisolution" << endl ;

					number_of_elastic_1 = 0 ;
					number_of_elastic_2 = 0 ;
					vector_TProtonreconstruction.clear() ;

					if(multitrack && (prev_run_number_event_number_pair_is_set) && (prev_run_number_event_number_pair.second > run_number_event_number_pair.second))
					{
						cout << "Error in TReducedNtuple::Loop: the event numbers are not increasing !" << endl ;
						exit(1) ;
					}

					prev_run_number_event_number_pair = run_number_event_number_pair ;
					prev_run_number_event_number_pair_is_set = true ;
					prev_trigger_data_input_status_bits = trigger_data_input_status_bits ;
				}

				if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_BOTTOM_RIGHT_TOP") == 0.0) && track_valid)
				{

					// cout << "I am here" << endl ;
					ProtonReconstruction->Reconstruct(event_info_timestamp, index_of_root_file, trigger_data_event_num,
						track_left_near_x, track_left_far_x, track_right_near_x, track_right_far_x,
						track_left_near_y, track_left_far_y, track_right_near_y, track_right_far_y,
						track_left_near_thx, track_left_far_thx, track_right_near_thx, track_right_far_thx,
						track_left_near_thy, track_left_far_thy, track_right_near_thy, track_right_far_thy,
						ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters->GetRPAlignment(), number_of_valid_tracks) ;

					bool status = (*PlotsCollections)["PlotsCollection"]->Test(ProtonReconstruction, probability_of_candidate) ;
					// if(ProjectParameters->GetParameterValue("SaveRuns") == 1.0) (*PlotsCollections)[collection_per_run_name]->Fill(ProtonReconstruction) ;

					if(status)
					{
						// cout << "Accepted elastic event: " << trigger_data_run_num << " " << trigger_data_event_num << "  probability_of_candidate:" << probability_of_candidate << endl ;
						// cout << track_left_near_x << "  " << track_left_far_x << "  " << track_right_near_x << "  " << track_right_far_x << "\t theta_x_right_rad: " <<  ProtonReconstruction->GetThetaXRightRad() << "\t theta_x_star_right_rad: " <<  ProtonReconstruction->GetThetaXStarRightRad() << "\t\t  track_right_near_chi2ndf: " << track_right_near_chi2ndf << endl ;
						// cout << track_left_near_y << "  " << track_left_far_y << "  " << track_right_near_y << "  " << track_right_far_y << "\t theta_y_right_rad: " <<  ProtonReconstruction->GetThetaYRightRad() << "\t\t theta_y_star_right_rad: " <<  ProtonReconstruction->GetThetaYStarRightRad() << "\t\t  track_right_far_chi2ndf: " << track_right_far_chi2ndf << endl ;
						number_of_elastic_1++ ;
						// cout << "event_info_timestamp: " << event_info_timestamp << endl ;
						// cout << "root_file_index_and_event_number: " << index_of_root_file << "  " << trigger_data_event_num << " " << track_right_near_y <<  " " << track_right_far_y <<  " " << track_left_near_y <<  " " << track_left_far_y << endl ;
					}

					ProtonReconstruction->SetElastic(status) ;

					if(status && false)
					{
						double theta_y_star_left_rad  =  (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_left_rad")) ;
						double theta_y_star_right_rad =  (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_right_rad")) ;
						
						double theta_x_star_left_rad  =  (*ProtonReconstruction->GetReconstructedVariable("theta_x_star_left_rad")) ;
						double theta_x_star_right_rad =  (*ProtonReconstruction->GetReconstructedVariable("theta_x_star_right_rad")) ;

						theta_y_star_difference.Fill(theta_y_star_left_rad + theta_y_star_right_rad) ;
						theta_x_star_difference.Fill(theta_x_star_left_rad + theta_x_star_right_rad) ;
					}

					vector_TProtonreconstruction.push_back(*ProtonReconstruction) ;

				}

				if((ProjectParameters->GetSettingValue("analysed_diagonal").compare("DIAGONAL_LEFT_TOP_RIGHT_BOTTOM") == 0.0) && track_valid)
				{

					ProtonReconstruction->Reconstruct(event_info_timestamp, index_of_root_file, trigger_data_event_num,
							track_left_near_x, track_left_far_x, track_right_near_x, track_right_far_x,
							track_left_near_y, track_left_far_y, track_right_near_y, track_right_far_y,
							track_left_near_thx, track_left_far_thx, track_right_near_thx, track_right_far_thx,
							track_left_near_thy, track_left_far_thy, track_right_near_thy, track_right_far_thy,
							ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters->GetRPAlignment(), number_of_valid_tracks) ;

					bool status = (*PlotsCollections)["PlotsCollection"]->Test(ProtonReconstruction, probability_of_candidate) ;
					// if(ProjectParameters->GetParameterValue("SaveRuns") == 1.0) (*PlotsCollections)[collection_per_run_name]->Fill(ProtonReconstruction) ;

					if(status)
					{
						cout << "Accepted elastic event: " << trigger_data_run_num << " " << trigger_data_event_num << "  probability_of_candidate:" << probability_of_candidate << endl ;
						number_of_elastic_1++ ;
						// cout << "root_file_index_and_event_number: " << index_of_root_file << "  " << trigger_data_event_num << " " << track_right_near_y <<  " " << track_right_far_y <<  " " << track_left_near_y <<  " " << track_left_far_y << endl ;
					}
					else
					{
						cout << "Rejected event: " << trigger_data_run_num << " " << trigger_data_event_num << "  probability_of_candidate:" << probability_of_candidate << endl ;
					}

					ProtonReconstruction->SetElastic(status) ;

					vector_TProtonreconstruction.push_back(*ProtonReconstruction) ;

					if(false & status)
					{
						double theta_y_star_left_rad  =  (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_left_rad")) ;
						double theta_y_star_right_rad =  (*ProtonReconstruction->GetReconstructedVariable("theta_y_star_right_rad")) ;
						
						double theta_x_star_left_rad  =  (*ProtonReconstruction->GetReconstructedVariable("theta_x_star_left_rad")) ;
						double theta_x_star_right_rad =  (*ProtonReconstruction->GetReconstructedVariable("theta_x_star_right_rad")) ;

						theta_y_star_difference.Fill(theta_y_star_left_rad + theta_y_star_right_rad) ;
						theta_x_star_difference.Fill(theta_x_star_left_rad + theta_x_star_right_rad) ;
					}

					// cout << "Test Test =====================" << endl ;
					// ProtonReconstruction->Print() ;
					// vector_TProtonreconstruction.back().Print() ;
					// cout << "Test Test =====================" << endl ;
				}
			}
		}
	}

	system("date") ;
	cout << "number_of_events_fullfill_conditions: " << number_of_events << endl ;

}

void SetupProperty(string label, TTree *tree, TProjectParameters *ProjectParameters, string value="")
{
	if((ProjectParameters != NULL) && (value != ""))
	{
		cout << "Error in SetupProperty ! The ProjectParameters and value parameters cannot be nonzero at the same time!" << endl ;
		exit(1) ;
	}

	string label_with_type = label + "[100]/C" ;
	char *cstr = NULL ;

	if(ProjectParameters != NULL)
	{
		cstr = new char[ProjectParameters->GetSettingValue(label).length() + 1];
		strcpy(cstr, ProjectParameters->GetSettingValue(label).c_str());
	}
	else
	{
		cstr = new char[value.length() + 1];
		strcpy(cstr, value.c_str());
	}

	tree->Branch(label.c_str(),	cstr, label_with_type.c_str()) ;
}

string GetLocalTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[TConstants::buffersize_for_property_string];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,TConstants::buffersize_for_property_string,"%d-%m-%Y %H:%M:%S",timeinfo);

	string time_string(buffer) ;

	return time_string ;
}

string CreatePostixForDiagonal(int diagonal) 
{
	string postfix = "" ;

	if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP)
	{
		postfix = "DIAGONAL_LEFT_BOTTOM_RIGHT_TOP" ;
	}
	else if(diagonal == TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM)
	{
		postfix = "DIAGONAL_LEFT_TOP_RIGHT_BOTTOM" ;
	}
	else
	{
		cout << "TProject::GenerateOutputFilenameWithPath, wrong diagonal !" << endl ;
		exit(1) ;
	}

	return postfix ;
}

void SetupProperties(TProjectParameters *ProjectParameters, int diagonal)
{
	TTree *tree = new TTree("Properties", "Properties") ;

	SetupProperty("VersionOfAnalysisCode", tree, ProjectParameters) ;
	SetupProperty("project_task", tree, ProjectParameters) ;
	SetupProperty("project_subtask", tree, ProjectParameters) ;

	SetupProperty("MADX_label_of_left_near_RP_detector", tree, ProjectParameters) ;
	SetupProperty("MADX_label_of_left_far_RP_detector", tree, ProjectParameters) ;
	SetupProperty("MADX_label_of_right_near_RP_detector", tree, ProjectParameters) ;
	SetupProperty("MADX_label_of_right_far_RP_detector", tree, ProjectParameters) ;

	SetupProperty("logical_condition_of_data_reduction", tree, ProjectParameters) ;

	// Diagonal

	SetupProperty("diagonal", tree, NULL, CreatePostixForDiagonal(diagonal)) ;	

	// Time

	char buffer[TConstants::buffersize_for_property_string];
	string time = GetLocalTime() ;
	strcpy(buffer, time.c_str()) ;
	stringstream ss ;
	ss << TConstants::buffersize_for_property_string ;

	tree->Branch("Timestamp", buffer, ("Timestamp[" + ss.str() + "]/C").c_str()) ;
	
	tree->Fill() ;

}

string TProject::GenerateOutputFilenameWithPath(int diagonal, string filename)
{
	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	string postfix = CreatePostixForDiagonal(diagonal) ;

	int first_input_root_file_name_length = ((int) filename.size()) ;
	string output_root_file_name = output_directory + filename.substr(0, first_input_root_file_name_length-5) + "_" + postfix + ".root" ;

	return output_root_file_name ;
}


void TProject::InitMonteCarlo(double &sigma_x_of_effective_beam_divergence_rad, double &sigma_y_of_effective_beam_divergence_rad,
	double &theta_x_star_rad_theta_y_star_rad_cut_block_y1, double &theta_x_star_rad_theta_y_star_rad_cut_block_sigma,
	double &theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_x1, double &theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_sigma,
	double &theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_y1, double &theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_sigma)
{
	if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("diffraction") == 0.0))
	{
	}
	else
	{
		string cut_block_name = ProjectParameters->GetSettingValue("vertical_beam_divergence_cut_block") ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			sigma_y_of_effective_beam_divergence_rad = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TProject::InitMonteCarlo: the block (" + cut_block_name + ") which is necessary for MC vertical beam divergence calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = "theta_x_star_left_rad_theta_x_star_right_rad_cut_block" ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			sigma_x_of_effective_beam_divergence_rad = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TProject::InitMonteCarlo: the block (" + cut_block_name + ") which is necessary for MC horizontal beam divergence calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = "theta_x_star_rad_theta_y_star_rad_cut_block" ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			theta_x_star_rad_theta_y_star_rad_cut_block_y1 = cut_block->GetParameterValue("y1") ;
			theta_x_star_rad_theta_y_star_rad_cut_block_sigma = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TProject::InitMonteCarlo: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = "theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block" ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_x1 = cut_block->GetParameterValue("x1") ;
			theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_sigma = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TProject::InitMonteCarlo: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}

		cut_block_name = "theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block" ;

		if(ProjectParameters->IsBlockDefined(cut_block_name))
		{
			TProjectParameters *cut_block = ProjectParameters->GetBlock(cut_block_name) ;

			theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_y1 = cut_block->GetParameterValue("y1") ;
			theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_sigma = cut_block->GetParameterValue("sigma") ;
		}
		else
		{
			cout << "Error:  TProject::InitMonteCarlo: the block (" + cut_block_name + ") which is necessary for acceptance calculations is not defined ! : " << cut_block_name << endl ;
			exit(1) ;
		}
	
	}

}

bool IsBetweenValues(double value, double cut, double sigma)
{
	return (fabs(cut - value) < sigma) ;
}

void TProject::CalculateSystematicErrors()
{
}

void TProject::CalculateOverallNormalization(TH1D *merged_histogram_normalized, double bin_density)
{
	double OverallNormalization = 1.0 ;

	if(!ProjectParameters->IsBlockDefined("common_normalization_steps"))
	{
		cout << "Error: TProject::CalculateOverallNormalization: there is no common_normalization_steps block in the .prj file! "  << endl ;
		exit(1) ;

	}

	TProjectParameters *common_normalization_steps = ProjectParameters->GetBlock("common_normalization_steps") ;

	if(!ProjectParameters->IsBlockDefined("common_normalization_steps_values"))
	{
		cout << "Error: TProject::CalculateOverallNormalization: there is no common_normalization_steps_values block in the .prj file! "  << endl ;
		exit(1) ;

	}

	TProjectParameters *common_normalization_steps_values = ProjectParameters->GetBlock("common_normalization_steps_values") ;
	
	merged_histogram_normalized->Write() ;
	TH1D *aux = ((TH1D *) merged_histogram_normalized->Clone("Before_normalization")) ;
	aux->Write() ;

	TF1 *identity_function = new TF1("identity_function", "1.0", 0, 100) ;

	(*(ProjectParameters->GetPLogFile())) << endl << "Normalization steps" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "===================" << endl ;

	for(vector<string>::iterator it = common_normalization_steps->GetSettingsVector("normalization_step")->begin() ; it != common_normalization_steps->GetSettingsVector("normalization_step")->end() ; ++it)
	{
		cout << "Normalization step: "  << (*it) << endl ;
		(*(ProjectParameters->GetPLogFile())) <<  "Normalization step: "  << (*it) << endl ;

		if(!common_normalization_steps_values->IsParameterDefined((*it)))
		{
			cout << "Error: TProject::CalculateOverallNormalization: the required parameter " << (*it) << " is not defined in the common_normalization_steps_values block ! "  << endl ;
			exit(1) ;
		}

		if(it->compare("Bin_density") == 0.0)
		{
			merged_histogram_normalized->Scale(1.0 ,"width") ;
		}
		else if(it->compare("Unfolding_correction") == 0.0)
		{
			string root_file_name_with_unfolding_correction = common_normalization_steps_values->GetSettingValue(*it) ;
			TFile *root_file_with_unfolding_correction = new TFile(root_file_name_with_unfolding_correction.c_str(), "READ") ;
			generic_file->cd() ;
			
			TH1D *unfolding_correction = (TH1D *)root_file_with_unfolding_correction->Get("dN_dt_GeV2_correction_for_unfolding") ;
			TF1 *unfolding_correction_function = unfolding_correction->GetFunction("PrevFitTMP") ;

			// if((unfolding_correction_function == NULL) || (unfolding_correction == NULL))
			if((unfolding_correction == NULL))
			{
				cout << "Error: TProject::CalculateOverallNormalization: the unfolding correction function or histogram does not exist!"  << endl ;
				exit(1) ;
			}

			if((ProjectParameters->IsParameterDefined("do_not_apply_unfolding") && (ProjectParameters->GetParameterValue("do_not_apply_unfolding") == 1.0)))
			{
				(*(ProjectParameters->GetPLogFile())) <<  "Unfolding not applied !" << endl ;
				cout << "Unfolding not applied !" << endl ;
			}
			else
			{
				merged_histogram_normalized->Multiply(unfolding_correction) ;
			}



		}
		else
		{
			OverallNormalization = (OverallNormalization / common_normalization_steps_values->GetParameterValue(*it)) ;
			(*(ProjectParameters->GetPLogFile())) <<  "\tValue: "  << common_normalization_steps_values->GetParameterValue(*it) << endl ;
		}

		TH1D *aux = ((TH1D *) merged_histogram_normalized->Clone(it->c_str())) ;
		aux->Multiply(identity_function, OverallNormalization) ;
		aux->Write() ;
	}

	merged_histogram_normalized->Multiply(identity_function, OverallNormalization) ;
	merged_histogram_normalized->Write() ;
}

void PrintLatexDoubleParameter(double parameter, double parameter_error, string prefix, string postfix, double x, double y, double text_size)
{
	stringstream ss ;
	ss << parameter ;

	stringstream ss_error ;
	ss_error << parameter_error ;

	TLatex *text = new TLatex ;
	text->SetNDC() ;
	text->SetTextSize(text_size) ;
	
	if(parameter_error != 0)
		text->DrawLatex(x, y, (prefix + ss.str() + " #pm " + ss_error.str() + postfix).c_str()) ;
	else
		text->DrawLatex(x, y, (prefix + ss.str() + postfix).c_str()) ;

	text->Draw("same") ;
}

/*
class PhysicsFormula
{
	static const double hbarc = 0.1973 ; 				// [hbarc] = GeV * fm
	static const double hbarc2 = hbarc * hbarc ; 			// [hbarc] = GeV * fm
	static const double hbarc_square = 10 * hbarc2 ; 		// [hbarc_square] = GeV^2 * mb 

	double CalculateSigmaTotWithLuminosityIndependentMethod() ;
} ;

double PhysicsFormula::CalculateSigmaTotWithLuminosityIndependentMethod(double rho, double dNdt_t0_elastic double N_elastic, double N_inelastic)
{
	double sigma_tot_mb = ((16.0 * TMath::Pi() * hbarc_square) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) ;
	return sigma_tot_mb ;
} */

double weighted_bin_center(double a, double b, double B, TF1 *fit)
{

	double result = 1.0 ;

	if(fit == NULL)
	{
		double numerator = ((((1 + (a * B))*exp(-a*B)) - ((1 + (b * B))*exp(-b*B))) / (B*B)) ;
		double denominator = ((exp(-a*B) - exp(-b*B)) / B) ;	

		result = (numerator/denominator) ;
	}
	else
	{
		int subdivisions = 100000 ;
		double delta = ((b - a) / ((double)subdivisions)) ;

		double integral_of_fx = 0 ;
		double integral_of_x_times_fx = 0 ;

		for(int i = 0 ; i < subdivisions ; ++i)
		{
			double point_x = a + (delta * i) ;

			integral_of_fx = integral_of_fx + (delta * fit->Eval(point_x)) ;
			integral_of_x_times_fx = integral_of_x_times_fx + (point_x * (delta * fit->Eval(point_x))) ;

			result = (integral_of_x_times_fx / integral_of_fx) ;
		}
	}

	return result ;
}

void TProject::MergeAndPostProcess()
{
	TH1D *merged_histogram = NULL ;
	string merged_histogram_name = "merged_histogram" ;

	map<string, int> *map_from_Input_root_file_name_to_occurancies = new map<string, int> ;

	TProjectInformation *ProjectInformation = new TProjectInformation ;

	ULong64_t Total_elapsed_time_from_nutples = 0 ;

	for(vector<string>::iterator it = ProjectParameters->GetSettingsVector("Input_root_file_name")->begin() ; it != ProjectParameters->GetSettingsVector("Input_root_file_name")->end() ; ++it)
	{
		cout << "Processing file: " << (*it) << endl ;

		(*map_from_Input_root_file_name_to_occurancies)[(*it)]++ ;

		if((*map_from_Input_root_file_name_to_occurancies)[(*it)] != 1)
		{
			cout << "Error: TProject::MergeAndPostProcess: the input file occurs at least two times ! " << (*it) << endl ;
			exit(1) ;
		}
		
		(*(ProjectParameters->GetPLogFile())) << "Process input file: " << (*it ) ;

		TFile *file_to_be_merged = new TFile(it->c_str(),"READ");
		generic_file->cd() ;
		
		if((file_to_be_merged == NULL) || (file_to_be_merged->IsZombie()))
		{
			cout << "\033[1;31m" ;
			cout << "Error: TProject::MergeAndPostProcess: the file cannot be opened !" << (*it) << endl ;
			cout << "\033[0m" ;

			(*(ProjectParameters->GetPLogFile())) << " could not be opened!" << endl ;

			exit(1) ;
			continue ;
		}
		else
		{
			(*(ProjectParameters->GetPLogFile())) << " ok" << endl ;
		}


		TH1D *TH1D_to_be_merged = (TH1D *)file_to_be_merged->Get(ProjectParameters->GetSettingValue("name_of_histogram_to_merge").c_str()) ;

		if(TH1D_to_be_merged == NULL)
		{
			cout << "Error: TProject::MergeAndPostProcess: the histogram " << ProjectParameters->GetSettingValue("name_of_histogram_to_merge") << " cannot be opened in " << (*it) << " !" << endl ;
			exit(1) ;
		}

		cout << "TH1D_to_be_merged " << TH1D_to_be_merged << endl ;

		if(merged_histogram == NULL)
		{
			merged_histogram = ((TH1D *) TH1D_to_be_merged->Clone()) ;
			merged_histogram->SetName(merged_histogram_name.c_str()) ;

			(*(ProjectParameters->GetPLogFile())) << "\tHistogram created for merging: ok" << endl ;
		}
		else
		{
			(*(ProjectParameters->GetPLogFile())) << "\tAdd: ok" << endl ;

			merged_histogram->Add(TH1D_to_be_merged) ;
		}

		if(ProjectParameters->IsBlockDefined("luminosity_dependent_method"))
		{
			TTree *Properties = (TTree *)file_to_be_merged->Get("Properties");

			if(Properties == NULL)
			{
				cout << "Error: TProject::MergeAndPostProcess: no Properties branch  in root file: " << (*it) << endl   ;
				exit(1) ;
			}

			Properties->SetBranchAddress("Start_timestamp", &ProjectInformation->event_info_timestamp_start);
			Properties->SetBranchAddress("End_timestamp",   &ProjectInformation->event_info_timestamp_end);

			Properties->GetEntry(0) ;

			cout << "Start timestamp: " << ProjectInformation->event_info_timestamp_start << endl ;
			cout << "End timestamp: " << ProjectInformation->event_info_timestamp_end << endl ;

			Total_elapsed_time_from_nutples += (ProjectInformation->event_info_timestamp_end - ProjectInformation->event_info_timestamp_start) ;
		}

		cout << "File successfully processed." << endl ;
	}

	cout << "Total_elapsed_time_from_nutples: " << Total_elapsed_time_from_nutples << endl ;
	cout << "Number of entries in merged histogram before overall normalization:" << merged_histogram->GetEntries() << endl ;
	
	gStyle->SetPadGridX(kTRUE);
	gStyle->SetPadGridY(kTRUE);

	TLatex *text = new TLatex() ;
	text->SetNDC() ;
	text->SetTextSize(0.04) ;
	text->DrawLatex(0.2, 0.4, "test") ;

	merged_histogram->Write() ;

	string merged_histogram_normalized_name = "merged_histogram_normalized" ;
	TH1D *merged_histogram_normalized = (TH1D *)merged_histogram->Clone() ;
	merged_histogram_normalized->SetName(merged_histogram_normalized_name.c_str()) ;

	double bin_density = merged_histogram_normalized->GetXaxis()->GetNbins() / (merged_histogram_normalized->GetXaxis()->GetXmax() - merged_histogram_normalized->GetXaxis()->GetXmin()) ;

	cout << "bin_density: " << bin_density << endl ;

	CalculateOverallNormalization(merged_histogram_normalized, bin_density) ;

	TProjectParameters *block = ProjectParameters->GetBlock("Exponential_part_settings") ;

	Double_t lower_abs_t_boundary_of_fit_GeV2 = block->GetParameterValue("lower_abs_t_boundary_of_fit_GeV2") ;
	Double_t upper_abs_t_boundary_of_fit_GeV2 = block->GetParameterValue("upper_abs_t_boundary_of_fit_GeV2") ;

	// ===================== Beginning of exponential fit of the data in the selected interval

	TFitResultPtr fitresult = merged_histogram_normalized->Fit("expo", "SI", "", lower_abs_t_boundary_of_fit_GeV2, upper_abs_t_boundary_of_fit_GeV2) ;
	TF1 *fitted_polinomial = merged_histogram_normalized->GetFunction("expo") ;
	// fitted_polinomial->SetParameter(1, fitresult->Parameter(1) + 0.1) ;
			
	double constant = exp(fitresult->Parameter(0)) ;
	double constant_up = exp(fitresult->Parameter(0) + fitresult->ParError(0)) ;
	double constant_down = exp(fitresult->Parameter(0) - fitresult->ParError(0)) ;
	double slope = fitresult->Parameter(1) ;

	// Print out the result on the plot
	
	string merged_histogram_normalized_canvas_name = merged_histogram_normalized_name + "_canvas" ;
	TCanvas *merged_histogram_normalized_canvas = new TCanvas(merged_histogram_normalized_canvas_name.c_str() , merged_histogram_normalized_canvas_name.c_str()) ;

	merged_histogram_normalized->Draw() ;
	merged_histogram_normalized_canvas->SetLogy() ;
	
	PrintLatexDoubleParameter(constant, 	0.0, "dN/dt_{t=0, exp}= ", "", 0.2, 0.8, 0.04) ;
	PrintLatexDoubleParameter(slope, 	fitresult->ParError(1), "B_{exp}= ", " (1/GeV^{2})", 0.2, 0.7, 0.04) ;

	// Calculate N_{el}
	
	double N_elastic = fitted_polinomial->Integral(0, 10) ;

	TF1 *fitted_polinomial_up_up		= (TF1 *)fitted_polinomial->Clone() ;
	TF1 *fitted_polinomial_up_down		= (TF1 *)fitted_polinomial->Clone() ;
	TF1 *fitted_polinomial_down_up		= (TF1 *)fitted_polinomial->Clone() ;
	TF1 *fitted_polinomial_down_down	= (TF1 *)fitted_polinomial->Clone() ;

	fitted_polinomial_up_up->SetParameter(0, 	fitresult->Parameter(0)  + fitresult->ParError(0)) ;
	fitted_polinomial_up_down->SetParameter(0, 	fitresult->Parameter(0)  + fitresult->ParError(0)) ;
	fitted_polinomial_down_up->SetParameter(0, 	fitresult->Parameter(0)  - fitresult->ParError(0)) ;
	fitted_polinomial_down_down->SetParameter(0, 	fitresult->Parameter(0)  - fitresult->ParError(0)) ;

	fitted_polinomial_up_up->SetParameter(1, 	fitresult->Parameter(1)  + fitresult->ParError(1)) ;
	fitted_polinomial_up_down->SetParameter(1, 	fitresult->Parameter(1)  - fitresult->ParError(1)) ;
	fitted_polinomial_down_up->SetParameter(1, 	fitresult->Parameter(1)  + fitresult->ParError(1)) ;
	fitted_polinomial_down_down->SetParameter(1, 	fitresult->Parameter(1)  - fitresult->ParError(1)) ;

	double N_elastic_up_up = fitted_polinomial_up_up->Integral(0, 10) ;
	double N_elastic_up_down = fitted_polinomial_up_down->Integral(0, 10) ;
	double N_elastic_down_up = fitted_polinomial_down_up->Integral(0, 10) ;
	double N_elastic_down_down = fitted_polinomial_down_down->Integral(0, 10) ;

	// fitted_polinomial->SetParameter(
	// double N_elastic_up = fitted_polinomial->Integral(0, 10) ;
	// double N_elastic_down = fitted_polinomial->Integral(0, 10) ;
	
	PrintLatexDoubleParameter(N_elastic, 	0.0, "N_{el, preliminary}= ", "", 0.2, 0.6, 0.04) ;

	if(ProjectParameters->IsBlockDefined("luminosity_independent_method"))
	{
		TProjectParameters *luminosity_independent_method = ProjectParameters->GetBlock("luminosity_independent_method") ;

		double N_inelastic	= luminosity_independent_method->GetParameterValue("N_inelastic") ;
		double rho		= luminosity_independent_method->GetParameterValue("rho") ;

		PrintLatexDoubleParameter(N_inelastic, 	0.0, "N_{inelastic, preliminary}= ", " mb (from Jan Welti)",	0.2, 0.5, 0.04) ;
		PrintLatexDoubleParameter(rho, 		0.0, "#rho= ", "", 0.2, 0.4, 0.04) ;

		double sigma_tot_mb = 		((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) ;
		double sigma_tot_mb_up = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) ;
		double sigma_tot_mb_down = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) ;

		double sigma_el_mb = 		((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic));
		double sigma_el_mb_up = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic));
		double sigma_el_mb_down = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic)) ;

		double sigma_inel_mb = 		((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;
		double sigma_inel_mb_up = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;
		double sigma_inel_mb_down = 	((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;

		double sigma_el_per_tot = sigma_el_mb / sigma_tot_mb ;
		double sigma_el_per_tot_up = sigma_el_mb_up / sigma_tot_mb_up ;
		double sigma_el_per_tot_down = sigma_el_mb_down / sigma_tot_mb_down	 ;

		cout << "\033[1;31m" ;
		cout << "*********************************" << endl; 
		cout << "Slope:      " 	<< setiosflags(ios::fixed) << std::setprecision(10) << slope << endl ;
		cout << "dN/dt_t=0:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << constant << endl ;
		cout << "N_elastic:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_elastic << endl ;
		cout << "N_inelastic:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_inelastic << endl ;
		cout << "rho:            " 	<< rho << endl ;
		cout << "sigma_tot_mb:   " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_tot_mb << endl ;
		cout << "sigma_el_mb:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_el_mb << endl ;
		cout << "sigma_inel_mb:  " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_inel_mb << endl ;
		cout << "*********************************" << endl; 
		cout << "sigma_el_per_tot: " 	<< sigma_el_per_tot << endl ;
		cout << "\033[0m" ;
		cout << endl << endl << endl ;
		cout << "********** Error up *************" << endl; 
		cout << "Slope:      " 		<< setiosflags(ios::fixed) << std::setprecision(10) << slope << endl ;
		cout << "dN/dt_t=0:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << constant_up << endl ;
		cout << "N_elastic:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_elastic << endl ;
		cout << "N_inelastic:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_inelastic << endl ;
		cout << "rho:            " 	<< rho << endl ;
		cout << "sigma_tot_mb:   " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_tot_mb_up << endl ;
		cout << "sigma_el_mb:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_el_mb_up << endl ;
		cout << "sigma_inel_mb:  " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_inel_mb_up << endl ;
		cout << "*********************************" << endl; 

		cout << "********** Error Down ***********" << endl; 
		cout << "Slope:      " 		<< setiosflags(ios::fixed) << std::setprecision(10) << slope << endl ;
		cout << "dN/dt_t=0:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << constant_down << endl ;
		cout << "N_elastic:      " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_elastic << endl ;
		cout << "N_inelastic:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << N_inelastic << endl ;
		cout << "rho:            " 	<< rho << endl ;
		cout << "sigma_tot_mb:   " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_tot_mb_down << endl ;
		cout << "sigma_el_mb:    " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_el_mb_down << endl ;
		cout << "sigma_inel_mb:  " 	<< setiosflags(ios::fixed) << std::setprecision(4) << sigma_inel_mb_down << endl ;
		cout << "*********************************" << endl; 

	
		cout << "*********************************" << endl; 
		cout << "Perturbations" << endl; 
		cout << "*********************************" << endl; 

		N_inelastic = N_inelastic * 1.021 ;

		sigma_tot_mb = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) ;
		sigma_tot_mb_up = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) ;
		sigma_tot_mb_down = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) ;

		sigma_el_mb = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic));
		sigma_el_mb_up = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic));
		sigma_el_mb_down = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) * (N_elastic/(N_elastic + N_inelastic)) ;

		sigma_inel_mb = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;
		sigma_inel_mb_up = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_up / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;
		sigma_inel_mb_down = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * (constant_down / (N_elastic + N_inelastic)) * (N_inelastic/(N_elastic + N_inelastic)) ;

		sigma_el_per_tot = sigma_el_mb / sigma_tot_mb ;
		sigma_el_per_tot_up = sigma_el_mb_up / sigma_tot_mb_up ;
		sigma_el_per_tot_down = sigma_el_mb_down / sigma_tot_mb_down	  ;

		cout << "*********************************" << endl; 
		cout << "sigma_tot_mb: " 	<< sigma_tot_mb << endl ;
		cout << "sigma_el_mb: " 	<< sigma_el_mb << endl ;
		cout << "sigma_inel_mb: " 	<< sigma_inel_mb << endl ;
		cout << "sigma_el_per_tot: " 	<< sigma_el_per_tot << endl ;
		cout << "*********************************" << endl; 

		cout << "*********************************" << endl; 
		cout << "sigma_el_mb_up: " 	<< sigma_el_mb_up << endl ;
		cout << "sigma_el_mb_down: " 	<< sigma_el_mb_down << endl ;
		cout << "*********************************" << endl; 

		cout << "*********************************" << endl; 
		cout << "sigma_inel_mb_up: " 	<< sigma_inel_mb_up << endl ;
		cout << "sigma_inel_mb_down: " 	<< sigma_inel_mb_down << endl ;
		cout << "*********************************" << endl; 

		cout << "*********************************" << endl; 
		cout << "sigma_el_per_tot_up: " 	<< sigma_el_per_tot_up << endl ;
		cout << "sigma_el_per_tot_down: " 	<< sigma_el_per_tot_down << endl ;
		cout << "*********************************" << endl; 

	}
	
	if(ProjectParameters->IsBlockDefined("luminosity_dependent_method"))
	{
		TProjectParameters *luminosity_dependent_method = ProjectParameters->GetBlock("luminosity_dependent_method") ;

		double Luminosity_one_per_cm2_s	= luminosity_dependent_method->GetParameterValue("Luminosity_one_per_cm2_s") ;
		double rho			= luminosity_dependent_method->GetParameterValue("rho") ;

		double Luminosity_one_per_cm2	= (Luminosity_one_per_cm2_s * Total_elapsed_time_from_nutples) ;

		double dsigma_over_dt_at_t_0 = (constant / Luminosity_one_per_cm2) * 1e27 ;

		double sigma_tot_squared_mb_over_2 = ((16.0 * TMath::Pi() * TConstants::Get_hbarc2_GeV_2_mb()) / (1 + (rho * rho))) * dsigma_over_dt_at_t_0 ;
		double sigma_tot_mb = sqrt(sigma_tot_squared_mb_over_2) ;

		PrintLatexDoubleParameter(sigma_tot_mb,	0.0, "#sigma_{tot}= ", 	" mb", 0.2, 0.3, 0.04) ;

		cout << "*********************************" << endl; 
		cout << "sigma_tot_mb: " << sigma_tot_mb << endl ;
		cout << "*********************************" << endl; 
	}

	merged_histogram_normalized_canvas->Write() ;

	// ===================== Non-exponential test

	TF1 *t_GeV2_distribution = new TF1("t_GeV2_distribution", my_exponential_distribution, 0.0, 4.0, 2) ;

	t_GeV2_distribution->SetParameters(constant, slope) ;
	t_GeV2_distribution->SetNpx(100000) ;

	TH1D *merged_histogram_normalized_non_exponential_test = (TH1D *)merged_histogram_normalized->Clone() ;
	string merged_histogram_normalized_non_exponential_test_name = "merged_histogram_normalized_non_exponential_test" ;
	merged_histogram_normalized_non_exponential_test->SetName(merged_histogram_normalized_non_exponential_test_name.c_str()) ;

	merged_histogram_normalized_non_exponential_test->Add(t_GeV2_distribution, -1.0) ;
	merged_histogram_normalized_non_exponential_test->Divide(t_GeV2_distribution, (1.0 / TConstants::conversion_factor_from_fraction_to_percent)) ;
	
	// Begin: non-exp. test with bin center

	int number_of_entries = merged_histogram_normalized->GetNbinsX() ;

	Double_t *x_values = new Double_t[number_of_entries] ;
	Double_t *y_values = new Double_t[number_of_entries] ;

	for(int i = 0 ; i < number_of_entries ; ++i)
	{
		x_values[i] = merged_histogram_normalized->GetBinCenter(i) ;

		if(merged_histogram_normalized->GetBinContent(i) != 0) y_values[i] = ((merged_histogram_normalized->GetBinContent(i) - t_GeV2_distribution->Eval(merged_histogram_normalized->GetBinCenter(i))) / t_GeV2_distribution->Eval(merged_histogram_normalized->GetBinCenter(i))) ;
		else y_values[i] = 0 ;
	}

	TGraph *delta_dsigma_per_dt_graph = new TGraph(number_of_entries,x_values,y_values) ;
	delta_dsigma_per_dt_graph->Draw("AP") ;
	delta_dsigma_per_dt_graph->SetMarkerStyle(20) ;
	delta_dsigma_per_dt_graph->SetMarkerSize(1) ;
	delta_dsigma_per_dt_graph->GetXaxis()->SetRangeUser(0, upper_abs_t_boundary_of_fit_GeV2) ;

	double relative_delta_sigma_per_delta_t_range = block->GetParameterValue("relative_delta_sigma_per_delta_t_range") ;

	delta_dsigma_per_dt_graph->GetYaxis()->SetRangeUser(-relative_delta_sigma_per_delta_t_range, relative_delta_sigma_per_delta_t_range) ;

	// delta_dsigma_per_dt_graph->Fit("pol2", "", "", lower_abs_t_boundary_of_fit_GeV2, upper_abs_t_boundary_of_fit_GeV2) ;
	// delta_dsigma_per_dt_graph->GetFunction("pol2")->SetLineColor(kBlue) ;
	// TLine *line = new TLine(lower_abs_t_boundary_of_fit_GeV2, 0, upper_abs_t_boundary_of_fit_GeV2, 0) ;
	// line->Draw() ;
	// line->SetLineColor(kRed) ;

	string id_non_exp_test_graph = id + "_non_exp_test_graph" ;
	delta_dsigma_per_dt_graph->SetName(id_non_exp_test_graph.c_str()) ;
	delta_dsigma_per_dt_graph->Write() ;

	// End: non-exp. test with bin center

	merged_histogram_normalized_non_exponential_test->GetYaxis()->SetTitle("dN/dt w.r.t. exponential fit [%] ") ;
	merged_histogram_normalized_non_exponential_test->GetYaxis()->SetRangeUser(-12, 12) ;
	merged_histogram_normalized_non_exponential_test->GetXaxis()->SetRangeUser(0, 0.2) ;
	merged_histogram_normalized_non_exponential_test->SetMarkerStyle(TConstants::filled_circle_marker_style) ;
	merged_histogram_normalized_non_exponential_test->Write() ;

	string merged_histogram_normalized_non_exponential_test_canvas_name = merged_histogram_normalized_non_exponential_test_name + "_canvas" ;
	TCanvas *merged_histogram_normalized_non_exponential_test_canvas = new TCanvas(merged_histogram_normalized_non_exponential_test_canvas_name.c_str() , merged_histogram_normalized_non_exponential_test_canvas_name.c_str()) ;

	merged_histogram_normalized_non_exponential_test_canvas->SetGridx() ;
	merged_histogram_normalized_non_exponential_test_canvas->SetGridy() ;	

        gStyle->SetFillColor(kWhite) ;
        gStyle->SetFrameFillColor(kWhite) ;
	gStyle->SetPadBorderMode(0) ;
	gStyle->SetFrameBorderMode(0) ;

        merged_histogram_normalized_non_exponential_test_canvas->SetFillColor(kWhite);
        merged_histogram_normalized_non_exponential_test_canvas->SetFrameFillColor(kWhite);
        merged_histogram_normalized_non_exponential_test_canvas->GetFrame()->SetFillColor(kWhite) ;
        merged_histogram_normalized_non_exponential_test_canvas->SetBorderMode(0) ;
        merged_histogram_normalized_non_exponential_test_canvas->GetFrame()->SetBorderMode(0) ;
	
	merged_histogram_normalized_non_exponential_test->Draw() ;

	merged_histogram_normalized_non_exponential_test_canvas->Write() ;
	
	CalculateSystematicErrors() ;
	
	// Exponential fit with displaced bin center === begin

	int number_of_points = merged_histogram_normalized->GetNbinsX() ;

	double x[1000] ;
	double y[1000] ;
	double ex[1000] ;
	double ey[1000] ;

	TF1 *fit_for_bin_integral = NULL ;

	if(ProjectParameters->IsSettingDefined("fit_for_bin_integral"))
	{
		string fit_for_bin_integral_name = ProjectParameters->GetSettingValue("fit_for_bin_integral") ;

		TFile *fit_for_bin_integral_name_file = TFile::Open(fit_for_bin_integral_name.c_str()) ;

		if(fit_for_bin_integral_name_file->IsZombie())
		{
			cout << "Error: TProject::MergeAndPostProcess: the file cannot be opened ! " << fit_for_bin_integral_name << endl ;
			exit(1) ;
		}

		fit_for_bin_integral = (TF1 *)fit_for_bin_integral_name_file->Get("fgv20") ;
		
		if(fit_for_bin_integral == NULL)
		{
			cout << "Error: TProject::MergeAndPostProcess: the file cannot be opened ! " << fit_for_bin_integral_name << endl ;
			exit(1) ;
		}
	}

	generic_file->cd() ;

	double scale_factor_for_fit = 1 ;
	double artificial_lower_uncertainty_boundary = 0 ;

	if(ProjectParameters->IsParameterDefined("scale_factor_for_fit"))
	{
		scale_factor_for_fit = ProjectParameters->GetParameterValue("scale_factor_for_fit") ;
	}

	if(ProjectParameters->IsParameterDefined("artificial_lower_uncertainty_boundary"))
	{
		artificial_lower_uncertainty_boundary = ProjectParameters->GetParameterValue("artificial_lower_uncertainty_boundary") ;
	}

	const int column_width = 25 ;

	(*(ProjectParameters->GetPLogFile())) << endl ;
	(*(ProjectParameters->GetPLogFile())) << endl ;
	(*(ProjectParameters->GetPLogFile())) << "Corrected bins and bin contents after merging:" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "1. Bin centers are shifted according to function \"fit_for_bin_integral\"" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "2. Bin contents are scaled with bin size" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "3. Bin contents are scaled with \"scale_factor_for_fit\"" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "==================================== Start =========================================================" << endl ;
	(*(ProjectParameters->GetPLogFile())) << "       Bin low edge | Bin up edge  | Repr. point |           Value           |      Uncertainty        " << endl ;
	(*(ProjectParameters->GetPLogFile())) << "====================================================================================================" << endl ;

	string column_separator = "&" ;

	for(int i = 0 ; i < number_of_points ; ++i)
	{

		int processed_bin_index = (i+1) ;

		double x_lower = merged_histogram_normalized->GetBinLowEdge(processed_bin_index) ;
		double x_upper = (merged_histogram_normalized->GetBinLowEdge(processed_bin_index) + merged_histogram_normalized->GetBinWidth(processed_bin_index)) ;

		double my_weighted_bin_center = weighted_bin_center(x_lower, x_upper, -slope, fit_for_bin_integral) ;
	
		double bin_center = ((x_lower + x_upper) / 2.0) ;
	
		double relative_difference  = (((bin_center - my_weighted_bin_center) / bin_center) * 100) ;
	
		// cout << setiosflags(ios::fixed) << std::setprecision(10) << endl ;
		cout << bin_center << " " << my_weighted_bin_center << "  relative_difference: " << relative_difference << endl ;
		if((relative_difference < 0 ) || (relative_difference > 3))
		{
			cout << endl << "\033[1;31m" ;
			cout << "Warning: TProject::MergeAndPostProcess: suspicious bin integrals for mean correction !" << endl ;
			cout << endl << "\033[0m" ;
		}
		
		if(merged_histogram_normalized->GetBinContent(processed_bin_index) != 0)
		{
			double scaled_value = (merged_histogram_normalized->GetBinContent(processed_bin_index)) ;
			double scaled_error = (merged_histogram_normalized->GetBinError(processed_bin_index)) ;

			stringstream ss1, ss2 ;
			
			ss1 << fixed << setprecision(10) << scaled_value ;
			ss2 << fixed << setprecision(10) << scaled_error ;

			if((ss1.str().length() > column_width) || (ss2.str().length() > column_width))
			{
				cout << "Error: TProject::MergeAndPostProcess: column width is not enough ! " << endl ;
				exit(1) ;
			}

			string pad1 = "" ;
			string pad2 = "" ;

			for(int j = 0 ; j < (column_width - ss1.str().length()) ; ++j) pad1 += " " ;
			for(int j = 0 ; j < (column_width - ss2.str().length()) ; ++j) pad2 += " " ;

			(*(ProjectParameters->GetPLogFile())) << "       " << fixed << setprecision(10) 	<< "\t\t " << "  " << x_lower << "\t\t " << column_separator << "  " << x_upper
				<< "\t\t " << column_separator << "  " << my_weighted_bin_center << "\t\t " << column_separator << "  " << pad1 << scaled_value << "\t\t " << column_separator << "  " << pad2 << "\t\t " << scaled_error << "\t\t " << column_separator << endl ;
		}
	
		x[i] = my_weighted_bin_center ;
		y[i] = merged_histogram_normalized->GetBinContent(processed_bin_index) ;
		ex[i] = 0 ;
		ey[i] = merged_histogram_normalized->GetBinError(processed_bin_index) ;
	}

	(*(ProjectParameters->GetPLogFile())) << "==================================== End ===========================================================" << endl ;

	if(scale_factor_for_fit != 1)	
	{
		cout << endl << "\033[1;31m" ;
		cout << "Warning: TProject::MergeAndPostProcess: scale factor is set !" << endl ;
		cout << endl << "\033[0m" ;
	}

	if(artificial_lower_uncertainty_boundary != 0.0)	
	{
		cout << endl << "\033[1;31m" ;
		cout << "Warning: TProject::MergeAndPostProcess: artificial lower uncertainty boundary is set !" << endl ;
		cout << endl << "\033[0m" ;
	}

	TGraphErrors *gr = new TGraphErrors(number_of_points,x,y,ex,ey);	

	gr->SetMarkerSize(0.5) ;
	gr->SetMarkerStyle(20) ;
	
	TCanvas *c1 = new TCanvas("c1", "c1") ; ;

	gr->SaveAs("histogram_with_offset.root") ;
	
	c1->SetLogy() ;
	gr->GetXaxis()->SetRangeUser(0.0, 0.4) ;
	gr->GetXaxis()->SetTitle("-t [GeV^{2}]") ;
	gr->GetYaxis()->SetTitle("dN/dt [GeV^{-2}]") ;

	gr->Fit("expo", "", "", 0.012, 0.2) ;
	gr->SetName("merged_histogram_normalized_graph") ;

	gr->Draw("AP") ;
	gr->Write() ;

	// Exponential fit with displaced bin center === end
}

void TProject::ExecuteMonteCarlo()
{
	TMonteCarlo *MonteCarlo = new TMonteCarlo("MonteCarlo","MonteCarlo") ;

	string histogram_name_for_unfolding = ProjectParameters->GetSettingValue("histogram_name_for_unfolding") ;

	// General variables

	Double_t beam_momentum_GeV = (ProjectParameters->GetBeam_1_Optics()->GetBeamEnergyGeV()) ;
	
	double beam_momentum_GeV_beam_1 = beam_momentum_GeV + ((beam_momentum_GeV / TConstants::conversion_factor_from_fraction_to_percent) * ProjectParameters->GetParameterValue("beam_momentum_perturbation_percent_beam_1")) ;
	double beam_momentum_GeV_beam_2 = beam_momentum_GeV + ((beam_momentum_GeV / TConstants::conversion_factor_from_fraction_to_percent) * ProjectParameters->GetParameterValue("beam_momentum_perturbation_percent_beam_2")) ;

	// Histograms

	string Output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

        TF1 *t_GeV2_distribution = NULL ;
        TF1 *xi_fraction_distribution = NULL ;

	double dN_dt_GeV2_horizontal_axis_left_boundary = 0.0 ;
	double dN_dt_GeV2_horizontal_axis_right_boundary = 0.0 ; 
	double dN_dt_GeV2_number_of_horizontal_bins = 0.0 ;

	string dN_dt_GeV2_type_block_name = "dN_dt_GeV2_type_block" ;

	if(ProjectParameters->IsBlockDefined(dN_dt_GeV2_type_block_name))
	{
		TProjectParameters *dN_dt_GeV2_type_block = ProjectParameters->GetBlock(dN_dt_GeV2_type_block_name) ;

		dN_dt_GeV2_horizontal_axis_left_boundary =  dN_dt_GeV2_type_block->GetParameterValue("horizontal_axis_left_boundary") ;
		dN_dt_GeV2_horizontal_axis_right_boundary = dN_dt_GeV2_type_block->GetParameterValue("horizontal_axis_right_boundary") ;
		dN_dt_GeV2_number_of_horizontal_bins = dN_dt_GeV2_type_block->GetParameterValue("number_of_horizontal_bins") ;
	}

	long int MaxNumberOfEventsToAnalyze = (long int)ProjectParameters->GetParameterValue("MaxNumberOfEventsToAnalyze") ;

	double sigma_x_of_effective_beam_divergence_rad = 0 ;
	double sigma_y_of_effective_beam_divergence_rad = 0 ;

	double theta_x_star_rad_theta_y_star_rad_cut_block_y1 = 0 ;
	double theta_x_star_rad_theta_y_star_rad_cut_block_sigma = 0 ;

	double theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_x1 = 0 ;
	double theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_sigma = 0 ;

	double theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_y1 = 0 ;
	double theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_sigma = 0 ;

	double xi_fraction_distribution_lower_edge = 0 ;
	double xi_fraction_distribution_upper_edge = 0 ;
	double xi_fraction_distribution_constant = 0 ;
	double xi_fraction_distribution_slope = 0 ;

	double vertex_size_x_m = 0 ;
	double vertex_size_y_m = 0 ;

	long int number_of_simulated_particles = 0 ;
	long int number_of_simulated_background_particles = 0 ;

	Double_t track_left_near_x_mm = 0 ;
	Double_t track_left_far_x_mm = 0 ;
	Double_t track_right_near_x_mm = 0 ;
	Double_t track_right_far_x_mm = 0 ;

	Double_t track_left_near_y_mm = 0 ;
	Double_t track_left_far_y_mm = 0 ;
	Double_t track_right_near_y_mm = 0 ;
	Double_t track_right_far_y_mm = 0 ;

	Double_t t_GeV2_distribution_lower_edge = 0 ;
	Double_t t_GeV2_distribution_upper_edge = 0 ;

	double max_number_of_phi_divisions = 0 ;

	ULong64_t event_info_timestamp = 0 ;

	InitMonteCarlo(sigma_x_of_effective_beam_divergence_rad, sigma_y_of_effective_beam_divergence_rad,
		theta_x_star_rad_theta_y_star_rad_cut_block_y1,			 theta_x_star_rad_theta_y_star_rad_cut_block_sigma,
		theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_x1,  theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_sigma,
		theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_y1, theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_sigma) ;

	double theta_y_star_rad_min = fabs(theta_x_star_rad_theta_y_star_rad_cut_block_y1) - theta_x_star_rad_theta_y_star_rad_cut_block_sigma ;
	double theta_y_star_rad_max = fabs(theta_x_star_rad_theta_y_star_rad_cut_block_y1) + theta_x_star_rad_theta_y_star_rad_cut_block_sigma ;

	const string MC_generator_settings_block_name = "MC_generator_settings_block" ;
	const string MC_generator_optics_perturbations_block_name = "MC_generator_optics_perturbations_block" ;

	TMatrixD beam_1_transport_matrix_near_vertical = ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_near_vertical().GetTransportMatrixWithoutCoupling() ;
	TMatrixD beam_1_transport_matrix_far_vertical = ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_far_vertical().GetTransportMatrixWithoutCoupling() ;

	TMatrixD beam_2_transport_matrix_near_vertical = ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_near_vertical().GetTransportMatrixWithoutCoupling() ;
	TMatrixD beam_2_transport_matrix_far_vertical = ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_far_vertical().GetTransportMatrixWithoutCoupling() ;

	if(ProjectParameters->IsBlockDefined(MC_generator_optics_perturbations_block_name))
	{
		TProjectParameters *MC_generator_optics_perturbations_block = ProjectParameters->GetBlock(MC_generator_optics_perturbations_block_name) ;

		double Beam_1_Ly_perturbation_percent		= 1.0 + (MC_generator_optics_perturbations_block->GetParameterValue("Beam_1_Ly_perturbation_percent")	 / 100.0) ;
		double Beam_1_dLxds_perturbation_percent	= 1.0 + (MC_generator_optics_perturbations_block->GetParameterValue("Beam_1_dLxds_perturbation_percent") / 100.0) ;

		double Beam_2_Ly_perturbation_percent		= 1.0 + (MC_generator_optics_perturbations_block->GetParameterValue("Beam_2_Ly_perturbation_percent")	 / 100.0) ;
		double Beam_2_dLxds_perturbation_percent	= 1.0 + (MC_generator_optics_perturbations_block->GetParameterValue("Beam_2_dLxds_perturbation_percent") / 100.0) ;

		beam_1_transport_matrix_far_vertical(3,4) *= Beam_1_Ly_perturbation_percent ;
		beam_1_transport_matrix_far_vertical(2,2) *= Beam_1_dLxds_perturbation_percent ;

		beam_1_transport_matrix_near_vertical(3,4) *= Beam_1_Ly_perturbation_percent ;
		beam_1_transport_matrix_near_vertical(2,2) *= Beam_1_dLxds_perturbation_percent ;

		beam_2_transport_matrix_far_vertical(3,4) *= Beam_2_Ly_perturbation_percent ;
		beam_2_transport_matrix_far_vertical(2,2) *= Beam_2_dLxds_perturbation_percent ;

		beam_2_transport_matrix_near_vertical(3,4) *= Beam_2_Ly_perturbation_percent ;
		beam_2_transport_matrix_far_vertical(2,2) *= Beam_2_dLxds_perturbation_percent ;

	}

	cout << "Monte Carlo optics" << endl ;

        beam_1_transport_matrix_near_vertical.Print();
        beam_1_transport_matrix_far_vertical.Print();

        beam_2_transport_matrix_near_vertical.Print();
        beam_2_transport_matrix_far_vertical.Print();

	cout << "Monte Carlo optics" << endl ;

	unsigned int seed_of_mc = 1 ;

	unsigned int smooth_iterations = 800 ;

	if(ProjectParameters->IsBlockDefined(MC_generator_settings_block_name))
	{
		TProjectParameters *MC_generator_settings_block = ProjectParameters->GetBlock(MC_generator_settings_block_name) ;

		if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("diffraction") == 0.0))
		{
			xi_fraction_distribution_lower_edge = MC_generator_settings_block->GetParameterValue("xi_fraction_distribution_lower_edge") ;
			xi_fraction_distribution_upper_edge = MC_generator_settings_block->GetParameterValue("xi_fraction_distribution_upper_edge") ;

			xi_fraction_distribution_constant = MC_generator_settings_block->GetParameterValue("xi_fraction_distribution_constant") ;
			xi_fraction_distribution_slope = MC_generator_settings_block->GetParameterValue("xi_fraction_distribution_slope") ;

			xi_fraction_distribution = new TF1("xi_fraction_distribution", my_exponential_distribution, xi_fraction_distribution_upper_edge, xi_fraction_distribution_lower_edge, 2) ;
			xi_fraction_distribution->SetParameters(xi_fraction_distribution_constant, xi_fraction_distribution_slope) ;
			xi_fraction_distribution->SetNpx(1000000) ;
			xi_fraction_distribution->SaveAs("xi_fraction_distribution.root") ;
		}

		vertex_size_x_m = MC_generator_settings_block->GetParameterValue("vertex_size_x_m") ;
		vertex_size_y_m = MC_generator_settings_block->GetParameterValue("vertex_size_y_m") ;

		if(MC_generator_settings_block->IsParameterDefined("sigma_of_effective_beam_divergence_rad"))
		{
			sigma_x_of_effective_beam_divergence_rad = MC_generator_settings_block->GetParameterValue("sigma_of_effective_beam_divergence_rad") ;
			sigma_y_of_effective_beam_divergence_rad = MC_generator_settings_block->GetParameterValue("sigma_of_effective_beam_divergence_rad") ;
		}

		if(MC_generator_settings_block->IsParameterDefined("sigma_x_of_effective_beam_divergence_rad"))
		{
			sigma_x_of_effective_beam_divergence_rad = MC_generator_settings_block->GetParameterValue("sigma_x_of_effective_beam_divergence_rad") ;
		}

		if(MC_generator_settings_block->IsParameterDefined("sigma_y_of_effective_beam_divergence_rad"))
		{
			sigma_y_of_effective_beam_divergence_rad = MC_generator_settings_block->GetParameterValue("sigma_y_of_effective_beam_divergence_rad") ;
		}

		if(MC_generator_settings_block->IsParameterDefined("seed"))
		{
			seed_of_mc = MC_generator_settings_block->GetParameterValue("seed") ;
		}

		if(MC_generator_settings_block->IsParameterDefined("smooth_iterations"))
		{
			smooth_iterations = MC_generator_settings_block->GetParameterValue("smooth_iterations") ;
		}

		number_of_simulated_particles = MC_generator_settings_block->GetParameterValue("number_of_simulated_particles") ;
		number_of_simulated_background_particles = MC_generator_settings_block->GetParameterValue("number_of_simulated_background_particles") ;

		t_GeV2_distribution_lower_edge = MC_generator_settings_block->GetParameterValue("t_GeV2_distribution_lower_edge") ;
		t_GeV2_distribution_upper_edge = MC_generator_settings_block->GetParameterValue("t_GeV2_distribution_upper_edge") ;
		max_number_of_phi_divisions = MC_generator_settings_block->GetParameterValue("max_number_of_phi_divisions") ;
	}

	string Exponential_fit_block_name = "Exponential_fit_block" ;

	if(ProjectParameters->IsBlockDefined(Exponential_fit_block_name))
	{
		TProjectParameters *Exponential_fit_block = ProjectParameters->GetBlock(Exponential_fit_block_name) ;

		Double_t Constant = Exponential_fit_block->GetParameterValue("Constant") ;
		Double_t Slope = Exponential_fit_block->GetParameterValue("Slope") ;

		t_GeV2_distribution = new TF1("t_GeV2_distribution", my_exponential_distribution, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge, 2) ;
		t_GeV2_distribution->SetParameters(Constant, Slope) ;
		t_GeV2_distribution->SetNpx(1000000) ;
		t_GeV2_distribution->SaveAs("t_GeV2_distribution.root") ;
		

	}

	TGraph *interpolation_fit = NULL ;

	if(ProjectParameters->IsBlockDefined("Custom_fit_block"))
	{
		TProjectParameters *Custom_fit_block = ProjectParameters->GetBlock("Custom_fit_block") ;

		string Root_file_of_fit_name = Custom_fit_block->GetSettingValue("Root_file_of_fit") ;
		
		TFile *Root_file_of_fit = new TFile(Root_file_of_fit_name.c_str(),"READ");
		
		t_GeV2_distribution = ((TF1 *)Root_file_of_fit->Get("fgv20")) ;

		generic_file->cd() ;
		t_GeV2_distribution->Write() ;
	}

	string Flat_distribution_block_name = "Flat_distribution_block" ;

	if(ProjectParameters->IsBlockDefined(Flat_distribution_block_name))
	{
	        t_GeV2_distribution = new TF1("t_GeV2_distribution", flat_distribution, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge, 0) ;
	}

	if(t_GeV2_distribution == NULL)
	{
		cout << "Error: TProject::ExecuteMonteCarlo: t_GeV2_distribution == NULL" << endl ;
		exit(1) ;
	}

	const double PI_TIMES_2 = (2.0 * TMath::Pi()) ;

	double max_deviation = 0 ;
	double sum = 0 ;

	double minus_t_max_GeV2 = 0.2 ;

	// ProjectParameters->Get_preliminary_acceptance_beam_1()->GetPreliminaryAcceptance_TF1()->Integral(0.0, 1.0) ;

	bool did_any_of_the_cuts_move = false ;

	int iteration = 1 ;
	int maxiteration = 1 ;

	if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("unfolding") == 0.0))
	{
		maxiteration = 7 ;
	}
	
	bool MC_with_split_between_fit_and_extrapolations = false ;

	if(ProjectParameters->IsParameterDefined("MC_with_split_between_fit_and_extrapolations"))
	{
		if(ProjectParameters->GetParameterValue("MC_with_split_between_fit_and_extrapolations")  == 1.0)
		{
			MC_with_split_between_fit_and_extrapolations = true ;
		}
	}


	gRandom->SetSeed(seed_of_mc) ;

	const int max_number_of_generators = 30 ;

	TRandom3 *random_numbers_vector[max_number_of_generators] ;
	for(int i = 0 ; i < max_number_of_generators ; ++i)
	{
		random_numbers_vector[i] = new TRandom3(*((TRandom3*) gRandom)) ;
	}

	// double dt = ((t_GeV2_distribution_upper_edge - t_GeV2_distribution_lower_edge) / (number_of_simulated_particles)) ;

	DEBUG_1 = (double *)PlotsCollections["PlotsCollection"] ; // Only for debugging the code (e.q. in case of seg. fault or bus error)
	
	int MC_dN_dt_GeV2_number_of_bins = dN_dt_GeV2_number_of_horizontal_bins ;

	TH2D *response_matrix_histogram = new TH2D("response_matrix_histogram","response_matrix_histogram", MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge,  MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;

	TH1D *MC_dN_dt_GeV2_constant_bin_width = new TH1D("MC_dN_dt_GeV2_constant_bin_width","MC_dN_dt_GeV2_constant_bin_width", MC_dN_dt_GeV2_number_of_bins, 0, dN_dt_GeV2_horizontal_axis_right_boundary) ;
	TH1D *MC_dN_dt_GeV2 = (TH1D *)((TAnalysisPlot1D *)PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding))->GetPlot()->Clone("MC_dN_dt_GeV2") ;
	TGraph *MC_dN_dt_GeV2_graph_for_unfolding = new TGraph() ;
	MC_dN_dt_GeV2_graph_for_unfolding->SetName("proba") ;

	dN_dt_GeV2_number_of_horizontal_bins = MC_dN_dt_GeV2->GetNbinsX();
	t_GeV2_distribution_lower_edge = MC_dN_dt_GeV2->GetXaxis()->GetBinLowEdge(1) ;
	t_GeV2_distribution_upper_edge = MC_dN_dt_GeV2->GetXaxis()->GetBinLowEdge(dN_dt_GeV2_number_of_horizontal_bins) + MC_dN_dt_GeV2->GetXaxis()->GetBinWidth(dN_dt_GeV2_number_of_horizontal_bins) ;

	cout << "dN_dt_GeV2_number_of_horizontal_bins: " << dN_dt_GeV2_number_of_horizontal_bins << endl ;
	cout << "t_GeV2_distribution_lower_edge: " << t_GeV2_distribution_lower_edge << endl ;
	cout << "t_GeV2_distribution_upper_edge: " << t_GeV2_distribution_upper_edge << endl ;

	TH1D *MC_dN_dt_GeV2_corrected = (TH1D *)((TAnalysisPlot1D *)PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding))->GetPlot()->Clone("MC_dN_dt_GeV2_corrected") ;

	TH1D *dN_dt_GeV2_correction_for_unfolding = NULL ;

	TH1D *MC_dN_dt_GeV2_acceptance_corrected = new TH1D("MC_dN_dt_GeV2_acceptance_corrected","MC_dN_dt_GeV2_acceptance_corrected", MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;
	TH1D *MC_dN_dt_GeV2_acceptance_corrected_divergence_corrected = new TH1D("MC_dN_dt_GeV2_acceptance_corrected_divergence_corrected","MC_dN_dt_GeV2_acceptance_corrected_divergence_corrected", MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;

	TH1D *MC_dN_dt_with_beam_divergence_GeV2 = new TH1D("MC_dN_dt_with_beam_divergence_GeV2","MC_dN_dt_with_beam_divergence_GeV2", MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;

	TH1D *MC_dN_dt_GeV2_reconstructed = new TH1D("MC_dN_dt_GeV2_reconstructed","MC_dN_dt_GeV2_reconstructed", MC_dN_dt_GeV2_number_of_bins, t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;

	MC_dN_dt_GeV2->Sumw2() ;
	MC_dN_dt_GeV2_reconstructed->Sumw2() ;
	// MC_dN_dt_GeV2_acceptance_corrected->Sumw2() ;

	TH1D *beam_divergence_rad_distribution = new TH1D("beam_divergence_rad_distribution","beam_divergence_rad_distribution", 100, -1000e-6, 1000e-6) ;
	TH1D *theta_y_IP5_rad_distribution = new TH1D("theta_y_IP5_rad_distribution","theta_y_IP5_rad_distribution", 100, -300e-6, 300e-6) ;
	TH1D *theta_y_IP5_rad_distribution_left = new TH1D("theta_y_IP5_rad_distribution_left","theta_y_IP5_rad_distribution_left", 100, -300e-6, 300e-6) ;
	TH1D *theta_y_IP5_rad_distribution_right = new TH1D("theta_y_IP5_rad_distribution_right","theta_y_IP5_rad_distribution_right", 100, -300e-6, 300e-6) ;

	TH2D *theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution = new TH2D("theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution","theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution", 100, -0.3e-3, 0.3e-3, 100, -0.3e-3, 0.3e-3) ;
	TH2D *theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution_after_cut = new TH2D("theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution_after_cut","theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution_after_cut", 100, -0.3e-3, 0.3e-3, 100, -0.3e-3, 0.3e-3) ;

	TH1D *t_distribution = new TH1D("t_distribution","t_distribution",       100, -5, 5) ;
	TH1D *t_distribution_2 = new TH1D("t_distribution_2","t_distribution_2", 100, -5, 5) ;
	TH1D *t_distribution_3 = new TH1D("t_distribution_3","t_distribution_3", 100, -5, 5) ;

	TH1D *minus_t_y_GeV2_distribution = new TH1D("minus_t_y_GeV2_distribution","minus_t_y_GeV2_distribution", 400, 0, t_GeV2_distribution_upper_edge) ;
	TH1D *minus_t_y_GeV2_distribution_after_cuts = new TH1D("minus_t_y_GeV2_distribution_after_cuts","minus_t_y_GeV2_distribution_after_cuts", 400, 0, t_GeV2_distribution_upper_edge) ;

	TH1D *difference_effective_beam_divergence_rad_x = new TH1D("difference_effective_beam_divergence_rad_x","difference_effective_beam_divergence_rad_x", 100, -1000e-6, 1000e-6) ;
	TH1D *difference_effective_beam_divergence_rad_y = new TH1D("difference_effective_beam_divergence_rad_y","difference_effective_beam_divergence_rad_y", 100, -1000e-6, 1000e-6) ;

	TH1D *difference_theta_x_IP5_rad_rot = new TH1D("difference_theta_x_IP5_rad_rot","difference_theta_x_IP5_rad_rot", 100, 0, 0) ;
	TH1D *difference_theta_y_IP5_rad_rot = new TH1D("difference_theta_y_IP5_rad_rot","difference_theta_y_IP5_rad_rot", 100, 0, 0) ;

	TH1D *MC_minus_t_GeV2_reconstructed_test_of_t_slice = new TH1D("MC_minus_t_GeV2_reconstructed_test_of_t_slice","MC_minus_t_GeV2_reconstructed_test_of_t_slice", 100, 0, 0) ;
	TH1D *MC_minus_t_left_GeV2_reconstructed_test_of_t_slice = new TH1D("MC_minus_t_left_GeV2_reconstructed_test_of_t_slice","MC_minus_t_left_GeV2_reconstructed_test_of_t_slice", 100, 0, 0) ;
	TH1D *MC_minus_t_right_GeV2_reconstructed_test_of_t_slice = new TH1D("MC_minus_t_right_GeV2_reconstructed_test_of_t_slice","MC_minus_t_right_GeV2_reconstructed_test_of_t_slice", 100, 0, 0) ;

	TH1D *difference_theta_x_IP5_rad_rot_with_beam_divergence = new TH1D("difference_theta_x_IP5_rad_rot_with_beam_divergence","difference_theta_x_IP5_rad_rot_with_beam_divergence", 100, -1000e-6, 1000e-6) ;
	TH1D *difference_theta_y_IP5_rad_rot_with_beam_divergence = new TH1D("difference_theta_y_IP5_rad_rot_with_beam_divergence","difference_theta_y_IP5_rad_rot_with_beam_divergence", 100, -1000e-6, 1000e-6) ;

	TH2D *minus_t_y_GeV2_minus_t_GeV2 = new TH2D("minus_t_y_GeV2_minus_t_GeV2","minus_t_y_GeV2_minus_t_GeV2", 400, 0, t_GeV2_distribution_upper_edge, 400, 0, t_GeV2_distribution_upper_edge) ;
	TH2D *MC_minus_t_GeV2_minus_t_GeV2_reconstructed = new TH2D("MC_minus_t_GeV2_minus_t_GeV2_reconstructed","MC_minus_t_GeV2_minus_t_GeV2_reconstructed", 4000, 0, t_GeV2_distribution_upper_edge, 4000, 0, t_GeV2_distribution_upper_edge) ;
	TH2D *MC_minus_t_GeV2_minus_t_GeV2_reconstructed_after_cuts = new TH2D("MC_minus_t_GeV2_minus_t_GeV2_reconstructed_after_cuts","MC_minus_t_GeV2_minus_t_GeV2_reconstructed_after_cuts", 4000, 0, t_GeV2_distribution_upper_edge, 4000, 0, t_GeV2_distribution_upper_edge) ;

	TH2D *theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution = new TH2D("theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution","theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution", 100, -0.8e-3, 0.8e-3, 100, -0.8e-3, 0.8e-3) ;
	TH2D *theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution_with_beam_divergence = new TH2D("theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution_with_beam_divergence","theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution_with_beam_divergence", 100, -0.8e-3, 0.8e-3, 100, -0.8e-3, 0.8e-3) ;

	TH1D *MC_x_star_m = new TH1D("MC_x_star_m", "MC_x_star_m", 100, -0.0001, 0.0001) ;
	TH1D *MC_y_star_m = new TH1D("MC_y_star_m", "MC_y_star_m", 100, -0.0001, 0.0001) ;

	TTree *Monte_Carlo_ntuple = new TTree("TReducedNtuple", "TReducedNtuple") ;

	UInt_t          MC_track_num = 0 ;

	ULong64_t       MC_event_info_timestamp = 0 ;
	UInt_t          MC_trigger_data_run_num = 0 ;
	UInt_t          MC_trigger_data_bunch_num = 0 ;
	UInt_t          MC_trigger_data_event_num = 0 ;
	UInt_t          MC_trigger_data_trigger_num = 0 ;
	UInt_t          MC_trigger_data_input_status_bits = 1 ;

	Bool_t valid_MC = true ;

	Monte_Carlo_ntuple->Branch("event_info_timestamp", 	       	&MC_event_info_timestamp,  	   	"event_info_timestamp/l") ;  	   
	Monte_Carlo_ntuple->Branch("trigger_data_run_num", 	       	&MC_trigger_data_run_num,  	   	"trigger_data_run_num/i") ;  	   
	Monte_Carlo_ntuple->Branch("trigger_data_bunch_num", 	       	&MC_trigger_data_bunch_num,	   	"trigger_data_bunch_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_event_num", 	       	&MC_trigger_data_event_num,	   	"trigger_data_event_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_trigger_num",		&MC_trigger_data_trigger_num,	   	"trigger_data_trigger_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_input_status_bits",	&MC_trigger_data_input_status_bits,   	"trigger_data_input_status_bits/i") ;
	
	Monte_Carlo_ntuple->Branch("track_left_far_x"	, &track_left_far_x_mm, "track_left_far_x/D") ;
	Monte_Carlo_ntuple->Branch("track_left_far_y"	, &track_left_far_y_mm, "track_left_far_y/D") ;
	Monte_Carlo_ntuple->Branch("track_left_far_valid"	, &valid_MC, "track_left_far_valid/O)") ;

	Monte_Carlo_ntuple->Branch("track_left_near_x"	, &track_left_near_x_mm, "track_left_near_x/D") ;
	Monte_Carlo_ntuple->Branch("track_left_near_y"	, &track_left_near_y_mm, "track_left_near_y/D") ;
	Monte_Carlo_ntuple->Branch("track_left_near_valid"	, &valid_MC, "track_left_near_valid/O") ;

	Monte_Carlo_ntuple->Branch("track_right_far_x"	, &track_right_far_x_mm, "track_right_far_x/D") ;
	Monte_Carlo_ntuple->Branch("track_right_far_y"	, &track_right_far_y_mm, "track_right_far_y/D") ;
	Monte_Carlo_ntuple->Branch("track_right_far_valid"	, &valid_MC, "track_right_far_valid/O") ;

	Monte_Carlo_ntuple->Branch("track_right_near_x"	, &track_right_near_x_mm, "track_right_near_x/D") ;
	Monte_Carlo_ntuple->Branch("track_right_near_y"	, &track_right_near_y_mm, "track_right_near_y/D") ;
	Monte_Carlo_ntuple->Branch("track_right_near_valid"	, &valid_MC, "track_right_near_valid/O") ;
	
	// TotemNtuple *ntuple = new TotemNtuple("Monte_Carlo_ntuple.root", ProjectParameters->GetSettingValue("Input_directory"), 1) ;
	// ntuple->GetTree()->Branch("track_rp_25_x"	, &track_left_far_x_mm, "track_rp_25_x/D") ;
	// ntuple->Branch("Start_timestamp"	, &ProjectInformation->event_info_timestamp_start	, "Start_timestamp/l") ;

	// TReducedNtuple *Monte_Carlo_ntuple = new TReducedNtuple() ;

	int second_particle = 0 ;
	int thirteenth_particle = 13 ;

	int index_for_unfolding = 0 ;

	TF1 *identity_function = new TF1("identity_function", "1.0", -1000.0, 1000.0); 

	do
	{
		PlotsCollections["PlotsCollection"]->Reset() ;
		MC_dN_dt_GeV2->Reset() ;
		MC_dN_dt_GeV2_corrected->Reset() ;		

		cerr << "Iteration : " << iteration << endl ;

		if(iteration > max_number_of_generators)
		{
			cout << "Error: TProject::ExecuteMonteCarlo(): too many iterations. Increase the max_number_of_generators variable." << endl ;
			exit(1) ;
		}

		if(iteration > 1) delete gRandom ; 

		gRandom = random_numbers_vector[iteration-1] ;	
		gRandom->SetSeed(seed_of_mc + (iteration-1)) ;

		for(int i = 0 ; i < number_of_simulated_particles ; ++i)
		{
			TMatrixD proton_1_kinematics_at_IP5(1, 5, 1, 1) ;
			TMatrixD proton_2_kinematics_at_IP5(1, 5, 1, 1) ;

			int number_of_phi_divisions = (int)(max_number_of_phi_divisions) ;

			double dphi = (2*TMath::Pi() / ((double)number_of_phi_divisions)) ;
			dphi_for_test = dphi ;

			for(int j = 0 ; j < number_of_phi_divisions ; ++j)
			{

				Double_t x_star_m = 1.0 * gRandom->Gaus() * vertex_size_x_m ;
				Double_t y_star_m = 1.0 * gRandom->Gaus() * vertex_size_y_m ;
	
				MC_x_star_m->Fill(x_star_m) ;
				MC_y_star_m->Fill(y_star_m) ;
			
				Double_t phi_IP5_rad = gRandom->Uniform(0, PI_TIMES_2) ;
				// Double_t phi_IP5_rad = (dphi * j) ;

				double minus_t_GeV2 = 	0.0 ;
				double MC_weight = 	1.0 ;
				double MC_weight_corrected = 	1.0 ;
				
				if(ProjectParameters->IsParameterDefined("MC_without_weights") && (ProjectParameters->GetParameterValue("MC_without_weights") == 1.0))
				{
					minus_t_GeV2 = t_GeV2_distribution->GetRandom() ;
				}
				else
				{
					minus_t_GeV2 = gRandom->Uniform(t_GeV2_distribution_lower_edge, t_GeV2_distribution_upper_edge) ;

					if(MC_with_split_between_fit_and_extrapolations)
					{
						if(minus_t_GeV2 < 0.368)
						{
							MC_weight = exp(6.696e+00) * exp(-18.7 * minus_t_GeV2) ;
						}
						else if(minus_t_GeV2 > 0.77)
						{
							MC_weight = exp(1.545e+00) * exp(-7.9 * minus_t_GeV2);
						}
						else
						{
							MC_weight = t_GeV2_distribution->Eval(minus_t_GeV2) ;
						}
						
					}
					else
					{
						MC_weight = t_GeV2_distribution->Eval(minus_t_GeV2) ;
					}

					MC_weight_corrected = MC_weight ;

				}

				for(int k = 0 ; k < 1 ; ++k)
				{


					if(!ProjectParameters->IsParameterDefined("take_into_account_acceptance_cuts_in_unfolding"))
					{
						MC_dN_dt_GeV2->Fill(minus_t_GeV2, MC_weight) ;
					}

					MC_dN_dt_GeV2_constant_bin_width->Fill(minus_t_GeV2, MC_weight) ;
					MC_dN_dt_GeV2_corrected->Fill(minus_t_GeV2, MC_weight_corrected) ;

					Double_t beam_divergence_rad_Beam_1_x = 1.0 * gRandom->Gaus() * (sigma_x_of_effective_beam_divergence_rad) ;
					Double_t beam_divergence_rad_Beam_1_y = 1.0 * gRandom->Gaus() * (sigma_y_of_effective_beam_divergence_rad) ;

					Double_t beam_divergence_rad_Beam_2_x = 1.0 * gRandom->Gaus() * (sigma_x_of_effective_beam_divergence_rad) ;
					Double_t beam_divergence_rad_Beam_2_y = 1.0 * gRandom->Gaus() * (sigma_y_of_effective_beam_divergence_rad) ;

					// cout << "sigma_of_effective_beam_divergence_rad: " << sigma_of_effective_beam_divergence_rad << endl ;

					beam_divergence_rad_distribution->Fill(beam_divergence_rad_Beam_1_x) ;

					difference_effective_beam_divergence_rad_x->Fill(beam_divergence_rad_Beam_2_x - beam_divergence_rad_Beam_1_x) ;
					difference_effective_beam_divergence_rad_y->Fill(beam_divergence_rad_Beam_2_y - beam_divergence_rad_Beam_1_y) ;

					Double_t  theta_x_IP5_rad_rot_Beam_1, theta_y_IP5_rad_rot_Beam_1 ;
					Double_t  theta_x_IP5_rad_rot_Beam_2, theta_y_IP5_rad_rot_Beam_2 ;

					Double_t  theta_x_IP5_rad_rot_Beam_1_with_beam_divergence, theta_y_IP5_rad_rot_Beam_1_with_beam_divergence ;
					Double_t  theta_x_IP5_rad_rot_Beam_2_with_beam_divergence, theta_y_IP5_rad_rot_Beam_2_with_beam_divergence ;

					MonteCarlo->CalculateThetaStarRadWithBeamDivergence(theta_x_IP5_rad_rot_Beam_1, theta_y_IP5_rad_rot_Beam_1, minus_t_GeV2, beam_momentum_GeV_beam_1, phi_IP5_rad, 0, 0) ;
					MonteCarlo->CalculateThetaStarRadWithBeamDivergence(theta_x_IP5_rad_rot_Beam_2, theta_y_IP5_rad_rot_Beam_2, minus_t_GeV2, beam_momentum_GeV_beam_2, phi_IP5_rad, 0, 0) ;

					MonteCarlo->CalculateThetaStarRadWithBeamDivergence(theta_x_IP5_rad_rot_Beam_1_with_beam_divergence, theta_y_IP5_rad_rot_Beam_1_with_beam_divergence, minus_t_GeV2, beam_momentum_GeV_beam_1, phi_IP5_rad, beam_divergence_rad_Beam_1_x, beam_divergence_rad_Beam_1_y) ;
					MonteCarlo->CalculateThetaStarRadWithBeamDivergence(theta_x_IP5_rad_rot_Beam_2_with_beam_divergence, theta_y_IP5_rad_rot_Beam_2_with_beam_divergence, minus_t_GeV2, beam_momentum_GeV_beam_2, phi_IP5_rad, beam_divergence_rad_Beam_2_x, beam_divergence_rad_Beam_2_y) ;

					double theta_x_IP5_rad_rot_with_beam_divergence = (theta_x_IP5_rad_rot_Beam_1_with_beam_divergence + theta_x_IP5_rad_rot_Beam_2_with_beam_divergence) / 2.0 ;
					double theta_y_IP5_rad_rot_with_beam_divergence = (theta_y_IP5_rad_rot_Beam_1_with_beam_divergence + theta_y_IP5_rad_rot_Beam_2_with_beam_divergence) / 2.0 ;

					double theta_IP5_rad_rot_with_beam_divergence_for_check_only = sqrt((theta_x_IP5_rad_rot_with_beam_divergence*theta_x_IP5_rad_rot_with_beam_divergence) + (theta_y_IP5_rad_rot_with_beam_divergence * theta_y_IP5_rad_rot_with_beam_divergence)) ;

					double minus_t_y_GeV2 = ((theta_y_IP5_rad_rot_with_beam_divergence * theta_y_IP5_rad_rot_with_beam_divergence) * (beam_momentum_GeV * beam_momentum_GeV) ) ;

					double minus_t_with_beam_divergence_GeV2 = (((theta_x_IP5_rad_rot_with_beam_divergence * theta_x_IP5_rad_rot_with_beam_divergence) + (theta_y_IP5_rad_rot_with_beam_divergence * theta_y_IP5_rad_rot_with_beam_divergence)) * (beam_momentum_GeV * beam_momentum_GeV) ) ;

					MC_dN_dt_with_beam_divergence_GeV2->Fill(minus_t_with_beam_divergence_GeV2) ;

					proton_1_kinematics_at_IP5(1,1) = x_star_m ;
					proton_1_kinematics_at_IP5(2,1) = theta_x_IP5_rad_rot_Beam_1_with_beam_divergence ;
					proton_1_kinematics_at_IP5(3,1) = y_star_m ;
					proton_1_kinematics_at_IP5(4,1) = theta_y_IP5_rad_rot_Beam_1_with_beam_divergence ;
					proton_1_kinematics_at_IP5(5,1) = 0 ;

					proton_2_kinematics_at_IP5(1,1) = x_star_m ;
					proton_2_kinematics_at_IP5(2,1) = -theta_x_IP5_rad_rot_Beam_2_with_beam_divergence ;
					proton_2_kinematics_at_IP5(3,1) = y_star_m ;
					proton_2_kinematics_at_IP5(4,1) = -theta_y_IP5_rad_rot_Beam_2_with_beam_divergence ;
					proton_2_kinematics_at_IP5(5,1) = 0 ;

					difference_theta_x_IP5_rad_rot->Fill(theta_x_IP5_rad_rot_Beam_2 - theta_x_IP5_rad_rot_Beam_1) ;
					difference_theta_y_IP5_rad_rot->Fill(theta_y_IP5_rad_rot_Beam_2 - theta_y_IP5_rad_rot_Beam_1) ;

					difference_theta_x_IP5_rad_rot_with_beam_divergence->Fill(theta_x_IP5_rad_rot_Beam_2_with_beam_divergence - theta_x_IP5_rad_rot_Beam_1_with_beam_divergence) ;
					difference_theta_y_IP5_rad_rot_with_beam_divergence->Fill(theta_y_IP5_rad_rot_Beam_2_with_beam_divergence - theta_y_IP5_rad_rot_Beam_1_with_beam_divergence) ;

					theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution->Fill(theta_x_IP5_rad_rot_Beam_1, theta_y_IP5_rad_rot_Beam_1) ;

					theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution->Fill(-theta_y_IP5_rad_rot_Beam_2, -theta_y_IP5_rad_rot_Beam_1) ;
					theta_y_IP5_rad_rot_left_theta_y_IP5_rad_rot_right_distribution_with_beam_divergence->Fill(-theta_y_IP5_rad_rot_Beam_2_with_beam_divergence, -theta_y_IP5_rad_rot_Beam_1_with_beam_divergence) ;

					TMatrixD proton_kinematics_at_beam_1_near(1, 5, 1, 1) ;
					TMatrixD proton_kinematics_at_beam_1_far(1, 5, 1, 1) ;

					TMatrixD proton_kinematics_at_beam_2_near(1, 5, 1, 1) ;
					TMatrixD proton_kinematics_at_beam_2_far(1, 5, 1, 1) ;

					if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("diffraction") == 0.0))
					{
						double proton_xi_1 = xi_fraction_distribution->GetRandom() ;
						double proton_xi_2 = xi_fraction_distribution->GetRandom() ;
						
						proton_kinematics_at_beam_1_near = ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_near_horizontal(proton_xi_1).GetTransportMatrixWithoutCoupling() * proton_1_kinematics_at_IP5 ;
						proton_kinematics_at_beam_1_far =  ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_far_horizontal(proton_xi_1).GetTransportMatrixWithoutCoupling() * proton_1_kinematics_at_IP5 ;
						
						proton_kinematics_at_beam_2_near = ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_near_horizontal(proton_xi_2).GetTransportMatrixWithoutCoupling() * proton_2_kinematics_at_IP5 ;
						proton_kinematics_at_beam_2_far =  ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_far_horizontal(proton_xi_2).GetTransportMatrixWithoutCoupling() * proton_2_kinematics_at_IP5 ;
						
						proton_kinematics_at_beam_1_near(1,1) += ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_near_horizontal(proton_xi_1).GetHorizontalBeamPositionM() - ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_near_horizontal(0).GetHorizontalBeamPositionM();
						proton_kinematics_at_beam_1_far(1,1)  += ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_far_horizontal(proton_xi_1).GetHorizontalBeamPositionM()  - ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_far_horizontal(0).GetHorizontalBeamPositionM();

						proton_kinematics_at_beam_2_near(1,1) -= ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_near_horizontal(proton_xi_2).GetHorizontalBeamPositionM() - ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_near_horizontal(0).GetHorizontalBeamPositionM();
						proton_kinematics_at_beam_2_far(1,1)  -= ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_far_horizontal(proton_xi_2).GetHorizontalBeamPositionM()  - ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_far_horizontal(0).GetHorizontalBeamPositionM();

					}
					else
					{
						proton_kinematics_at_beam_1_near 	= beam_1_transport_matrix_near_vertical * proton_1_kinematics_at_IP5 ;
						proton_kinematics_at_beam_1_far 	= beam_1_transport_matrix_far_vertical * proton_1_kinematics_at_IP5 ;

						proton_kinematics_at_beam_2_near 	= beam_2_transport_matrix_near_vertical * proton_2_kinematics_at_IP5 ;
						proton_kinematics_at_beam_2_far 	= beam_2_transport_matrix_far_vertical * proton_2_kinematics_at_IP5 ;
					}

					// Perturbations

					track_left_near_x_mm  = ((proton_kinematics_at_beam_2_near(1,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_near_dx_mm") ;
					track_left_far_x_mm   = ((proton_kinematics_at_beam_2_far(1,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_far_dx_mm") ;
					track_right_near_x_mm = ((proton_kinematics_at_beam_1_near(1,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_near_dx_mm") ;
					track_right_far_x_mm  = ((proton_kinematics_at_beam_1_far(1,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_far_dx_mm") ;

					track_left_near_y_mm  = ((proton_kinematics_at_beam_2_near(3,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_near_dy_mm") ;
					track_left_far_y_mm   = ((proton_kinematics_at_beam_2_far(3,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_far_dy_mm") ;
					track_right_near_y_mm = ((proton_kinematics_at_beam_1_near(3,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_near_dy_mm") ;
					track_right_far_y_mm  = ((proton_kinematics_at_beam_1_far(3,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_far_dy_mm") ;

					// Reconstruction

					double zero = 0.0 ;

					ProtonReconstruction->Reconstruct(event_info_timestamp, 0, 0, track_left_near_x_mm, track_left_far_x_mm, track_right_near_x_mm, track_right_far_x_mm, track_left_near_y_mm, track_left_far_y_mm, track_right_near_y_mm, track_right_far_y_mm,
					zero, zero, zero, zero, zero, zero, zero, zero, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters->GetRPAlignment(), 4) ;

					// Check out reconstructed variables

					double minus_t_GeV2_reconstructed = *(ProtonReconstruction->GetReconstructedVariable("minus_t_GeV2")) ;
					double minus_t_y_GeV2_reconstructed = *(ProtonReconstruction->GetReconstructedVariable("minus_t_y_GeV2")) ;
					double minus_t_left_GeV2_reconstructed = *(ProtonReconstruction->GetReconstructedVariable("minus_t_left_GeV2")) ;
					double minus_t_right_GeV2_reconstructed = *(ProtonReconstruction->GetReconstructedVariable("minus_t_right_GeV2")) ;
					
					if(dN_dt_GeV2_correction_for_unfolding != NULL)
					{

						int find_bin_number = dN_dt_GeV2_correction_for_unfolding->FindBin(minus_t_GeV2_reconstructed) ;
						MC_weight_corrected = (MC_weight_corrected * dN_dt_GeV2_correction_for_unfolding->GetBinContent(find_bin_number)) ;
					}


					// cout << minus_t_GeV2_reconstructed << endl ;

					// Fill histograms

					MC_dN_dt_GeV2_reconstructed->Fill(minus_t_GeV2_reconstructed, MC_weight) ;
					MC_minus_t_GeV2_minus_t_GeV2_reconstructed->Fill(minus_t_GeV2, minus_t_GeV2_reconstructed) ;

					if((minus_t_GeV2 > 0.5) && (minus_t_GeV2 < 0.51))
					{
						MC_minus_t_GeV2_reconstructed_test_of_t_slice->Fill(minus_t_GeV2_reconstructed) ;
						MC_minus_t_left_GeV2_reconstructed_test_of_t_slice->Fill(minus_t_left_GeV2_reconstructed) ;
						MC_minus_t_right_GeV2_reconstructed_test_of_t_slice->Fill(minus_t_right_GeV2_reconstructed) ;
					}

					double relative_t_delta = ((minus_t_with_beam_divergence_GeV2 - minus_t_GeV2_reconstructed) / minus_t_GeV2) * TConstants::conversion_factor_from_fraction_to_percent ;
					double relative_t_delta_2 = ((minus_t_with_beam_divergence_GeV2 - minus_t_GeV2) / minus_t_GeV2) * TConstants::conversion_factor_from_fraction_to_percent ;
					double relative_t_delta_3 = ((minus_t_GeV2 - minus_t_GeV2_reconstructed) / minus_t_GeV2) * TConstants::conversion_factor_from_fraction_to_percent ;

					t_distribution->Fill(relative_t_delta) ;
					t_distribution_2->Fill(relative_t_delta_2) ;
					t_distribution_3->Fill(relative_t_delta_3) ;
					
					if(relative_t_delta > 0.4 && false)
					{
						cout << "minus_t_with_beam_divergence_GeV2: " << minus_t_with_beam_divergence_GeV2 << endl ;
						cout << "minus_t_GeV2_reconstructed: " << minus_t_GeV2_reconstructed << endl ;
						cout << "phi_IP5_rad: " << phi_IP5_rad << endl ;
						
						cout << "theta_x_IP5_rad_rot_Beam_1: " << theta_x_IP5_rad_rot_Beam_1 << endl ;
						cout << "theta_x_IP5_rad_rot_Beam_2: " << theta_x_IP5_rad_rot_Beam_2 << endl ;

						cout << "theta_y_IP5_rad_rot_Beam_1: " << theta_y_IP5_rad_rot_Beam_1 << endl ;
						cout << "theta_y_IP5_rad_rot_Beam_2: " << theta_y_IP5_rad_rot_Beam_2 << endl ;
						
						cout <<  "theta_x_star_right_rad: "  << (*(ProtonReconstruction->GetReconstructedVariable("theta_x_star_right_rad"))) << endl ;
						cout <<  "theta_x_star_left_rad:  "  << (*(ProtonReconstruction->GetReconstructedVariable("theta_x_star_left_rad"))) << endl ;
						cout <<  "theta_y_star_right_rad: "  << (*(ProtonReconstruction->GetReconstructedVariable("theta_y_star_right_rad"))) << endl ;
						cout <<  "theta_y_star_left_rad:  "  << (*(ProtonReconstruction->GetReconstructedVariable("theta_y_star_left_rad"))) << endl ;
					}

					MC_track_num++ ;
					// cout << "second_particle:" <<  second_particle << endl ;

					if(ProjectParameters->IsSettingDefined("Increase_MC_event_number_only_per_2_steps_to_test_multitrack"))
					{
						if(second_particle == 1)
						{
							MC_trigger_data_event_num++ ;
						}
					}
					else if(ProjectParameters->IsSettingDefined("Increase_MC_event_number_only_per_13_steps_to_test_multitrack"))
					{
						if(thirteenth_particle == 0)
						{
							MC_trigger_data_event_num++ ;
							
							thirteenth_particle = 13 ;
						}
					}
					else
						MC_trigger_data_event_num++ ;
						
					second_particle = 1 - second_particle ;
					thirteenth_particle = thirteenth_particle - 1 ;
						
					// cout << MC_trigger_data_event_num << endl ;

					if(ProjectParameters->IsSettingDefined("MC_trigger_data_input_status_bits_set_random_over_RP_Vertical")) MC_trigger_data_input_status_bits = (MC_trigger_data_input_status_bits | MC_trigger_data_event_num) ;
					if(ProjectParameters->IsSettingDefined("MC_trigger_data_input_status_bits_set_random_over_RP_Vertical_invert")) MC_trigger_data_input_status_bits = ~(1 | MC_trigger_data_event_num) ;

					if(false) Monte_Carlo_ntuple->Fill() ;

					bool cuts_are_fulfilled = false ;

					if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("multitrack") == 0.0))
					{

						// Multitracking part

						vector<double>	coordinates_of_segments ;

						// Step A: Save the coordinates of the elastic

						coordinates_of_segments.push_back(track_left_near_x_mm) ;
						coordinates_of_segments.push_back(track_left_far_x_mm) ; 
						coordinates_of_segments.push_back(track_right_near_x_mm) ;
						coordinates_of_segments.push_back(track_right_far_x_mm) ;

						coordinates_of_segments.push_back(track_left_near_y_mm) ; 
						coordinates_of_segments.push_back(track_left_far_y_mm) ;
						coordinates_of_segments.push_back(track_right_near_y_mm) ;
						coordinates_of_segments.push_back(track_right_far_y_mm) ;

						vector<vector<double > > vector_of_coordinates_of_segments ;
						vector_of_coordinates_of_segments.push_back(coordinates_of_segments) ;

						for(int index_of_simulated_background_particles = 0 ; index_of_simulated_background_particles < number_of_simulated_background_particles ; ++index_of_simulated_background_particles)
						{
							coordinates_of_segments.clear() ;

							TRandom3 *random_for_multitrack = new TRandom3 ;	

							Double_t x_star_m_background_particles = 10.0 * gRandom->Gaus() * vertex_size_x_m ;
							Double_t y_star_m_background_particles = 10.0 * gRandom->Gaus() * vertex_size_y_m ;

							proton_1_kinematics_at_IP5(1,1) = x_star_m_background_particles ;
							proton_1_kinematics_at_IP5(2,1) = 10.0 * gRandom->Gaus() * (sigma_x_of_effective_beam_divergence_rad) ;
							proton_1_kinematics_at_IP5(3,1) = y_star_m_background_particles ;
							proton_1_kinematics_at_IP5(4,1) = 10.0 * gRandom->Gaus() * (sigma_y_of_effective_beam_divergence_rad) ;
							proton_1_kinematics_at_IP5(5,1) = 0 ;

							proton_2_kinematics_at_IP5(1,1) = x_star_m_background_particles ;
							proton_2_kinematics_at_IP5(2,1) = 10.0 * gRandom->Gaus() * (sigma_x_of_effective_beam_divergence_rad) ;
							proton_2_kinematics_at_IP5(3,1) = y_star_m_background_particles ;
							proton_2_kinematics_at_IP5(4,1) = 10.0 * gRandom->Gaus() * (sigma_y_of_effective_beam_divergence_rad) ;
							proton_2_kinematics_at_IP5(5,1) = 0 ;
	
							proton_kinematics_at_beam_1_near = ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_near_vertical().GetTransportMatrixWithoutCoupling() * proton_1_kinematics_at_IP5 ;
							proton_kinematics_at_beam_1_far  = ProjectParameters->GetBeam_1_Optics()->Get_transport_matrix_far_vertical().GetTransportMatrixWithoutCoupling() * proton_1_kinematics_at_IP5 ;

							proton_kinematics_at_beam_2_near = ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_near_vertical().GetTransportMatrixWithoutCoupling() * proton_2_kinematics_at_IP5 ;
							proton_kinematics_at_beam_2_far  = ProjectParameters->GetBeam_2_Optics()->Get_transport_matrix_far_vertical().GetTransportMatrixWithoutCoupling() * proton_2_kinematics_at_IP5 ;

							track_left_near_x_mm  = ((proton_kinematics_at_beam_2_near(1,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_near_dx_mm") ;
							track_left_far_x_mm   = ((proton_kinematics_at_beam_2_far(1,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_far_dx_mm") ;
							track_right_near_x_mm = ((proton_kinematics_at_beam_1_near(1,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_near_dx_mm") ;
							track_right_far_x_mm  = ((proton_kinematics_at_beam_1_far(1,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_far_dx_mm") ;

							track_left_near_y_mm  = ((proton_kinematics_at_beam_2_near(3,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_near_dy_mm") ;
							track_left_far_y_mm   = ((proton_kinematics_at_beam_2_far(3,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_left_far_dy_mm") ;
							track_right_near_y_mm = ((proton_kinematics_at_beam_1_near(3,1) * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_near_dy_mm") ;
							track_right_far_y_mm  = ((proton_kinematics_at_beam_1_far(3,1)  * TConstants::conversion_factor_from_m_to_mm)) + ProjectParameters->GetParameterValue("track_right_far_dy_mm") ;

							coordinates_of_segments.push_back(track_left_near_x_mm) ;
							coordinates_of_segments.push_back(track_left_far_x_mm) ; 
							coordinates_of_segments.push_back(track_right_near_x_mm) ;
							coordinates_of_segments.push_back(track_right_far_x_mm) ;

							coordinates_of_segments.push_back(track_left_near_y_mm) ; 
							coordinates_of_segments.push_back(track_left_far_y_mm) ;
							coordinates_of_segments.push_back(track_right_near_y_mm) ;
							coordinates_of_segments.push_back(track_right_far_y_mm) ;

							vector_of_coordinates_of_segments.push_back(coordinates_of_segments) ;

						}

						int overall_index = 0 ;

						bool multitrack_general_case_N_N_N_N_pieces_of_track_segments = false ;

						if(multitrack_general_case_N_N_N_N_pieces_of_track_segments)
						{
							for(int iterator_over_track_segments_1  = 0 ; iterator_over_track_segments_1 < vector_of_coordinates_of_segments.size() ; ++iterator_over_track_segments_1)
							for(int iterator_over_track_segments_2  = 0 ; iterator_over_track_segments_2 < vector_of_coordinates_of_segments.size() ; ++iterator_over_track_segments_2)
							for(int iterator_over_track_segments_3  = 0 ; iterator_over_track_segments_3 < vector_of_coordinates_of_segments.size() ; ++iterator_over_track_segments_3)
							for(int iterator_over_track_segments_4  = 0 ; iterator_over_track_segments_4 < vector_of_coordinates_of_segments.size() ; ++iterator_over_track_segments_4)
							{
								double zero = 0.0 ;

								track_left_near_x_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_1][0] ;
								track_left_far_x_mm   = vector_of_coordinates_of_segments[iterator_over_track_segments_2][1] ;

								track_right_near_x_mm = vector_of_coordinates_of_segments[iterator_over_track_segments_3][2] ;
								track_right_far_x_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_4][3] ;

								track_left_near_y_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_1][4] ;
								track_left_far_y_mm   = vector_of_coordinates_of_segments[iterator_over_track_segments_2][5] ;

								track_right_near_y_mm = vector_of_coordinates_of_segments[iterator_over_track_segments_3][6] ;
								track_right_far_y_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_4][7] ;

								ProtonReconstruction->Reconstruct(event_info_timestamp, 0, 0, track_left_near_x_mm, track_left_far_x_mm, track_right_near_x_mm, track_right_far_x_mm, track_left_near_y_mm, track_left_far_y_mm, track_right_near_y_mm, track_right_far_y_mm,
								zero, zero, zero, zero, zero, zero, zero, zero, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters->GetRPAlignment(), 4) ;

								bool status = PlotsCollections["PlotsCollection"]->Fill(ProtonReconstruction, MC_weight) ;

								if(status) cout << "elastic is catched with index: " << overall_index << endl ;
								overall_index++ ;
							}
						}
						else
						{

							// Only one track segment is varied (case 1-1-1-N)

							for(int iterator_over_track_segments_1  = 0 ; iterator_over_track_segments_1 < vector_of_coordinates_of_segments.size() ; ++iterator_over_track_segments_1)
							{
								double zero = 0.0 ;

								track_left_near_x_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_1][0] ;
								track_left_far_x_mm   = vector_of_coordinates_of_segments[0][1] ;

								track_right_near_x_mm = vector_of_coordinates_of_segments[0][2] ;
								track_right_far_x_mm  = vector_of_coordinates_of_segments[0][3] ;

								track_left_near_y_mm  = vector_of_coordinates_of_segments[iterator_over_track_segments_1][4] ;
								track_left_far_y_mm   = vector_of_coordinates_of_segments[0][5] ;

								track_right_near_y_mm = vector_of_coordinates_of_segments[0][6] ;
								track_right_far_y_mm  = vector_of_coordinates_of_segments[0][7] ;

								ProtonReconstruction->Reconstruct(event_info_timestamp, 0, 0, track_left_near_x_mm, track_left_far_x_mm, track_right_near_x_mm, track_right_far_x_mm, track_left_near_y_mm, track_left_far_y_mm, track_right_near_y_mm, track_right_far_y_mm,
								zero, zero, zero, zero, zero, zero, zero, zero, ProjectParameters->GetBeam_1_Optics(), ProjectParameters->GetBeam_2_Optics(), ProjectParameters->GetRPAlignment(), 4) ;

								bool status = PlotsCollections["PlotsCollection"]->Fill(ProtonReconstruction, MC_weight) ;

								if(status) cout << "(" << i << "," << j << ")" << "th event: elastic is catched with index: " << overall_index << endl ;
								overall_index++ ;
							}
						}


						cout << endl ;
					}
					else
					{
						// Standard Monte Carlo without multitrack

						cuts_are_fulfilled = PlotsCollections["PlotsCollection"]->Fill(ProtonReconstruction, MC_weight_corrected) ;
					}


					if(DEBUG_1 != (double *)PlotsCollections["PlotsCollection"])
					{
						cout << "DEBUG_1 changed: " << i <<"   "<< j << "   " << k << endl ;
					}

					response_matrix_histogram->Fill(minus_t_GeV2, minus_t_GeV2_reconstructed) ;
					// MC_dN_dt_GeV2_graph_for_unfolding->SetPoint(index_for_unfolding, minus_t_GeV2, minus_t_GeV2_reconstructed) ;
					index_for_unfolding++ ;

					double theta_y_star_rad = (theta_y_IP5_rad_rot_Beam_1 + theta_y_IP5_rad_rot_Beam_2) / 2.0 ;
					double theta_y_star_rad_with_beam_divergence = (theta_y_IP5_rad_rot_Beam_1_with_beam_divergence + theta_y_IP5_rad_rot_Beam_2_with_beam_divergence) / 2.0 ;

					theta_y_IP5_rad_distribution->Fill(theta_y_star_rad) ;
					theta_y_IP5_rad_distribution_left->Fill(theta_y_IP5_rad_rot_Beam_2_with_beam_divergence) ;
					theta_y_IP5_rad_distribution_right->Fill(theta_y_IP5_rad_rot_Beam_1_with_beam_divergence) ;

					minus_t_y_GeV2_minus_t_GeV2->Fill(minus_t_y_GeV2, minus_t_GeV2) ;

					if(cuts_are_fulfilled)
					{
						minus_t_y_GeV2_distribution->Fill(minus_t_y_GeV2) ;
						
			 			// if(IsBetweenValues(-theta_y_IP5_rad_rot_Beam_2_with_beam_divergence, theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_x1,	theta_y_star_left_rad_theta_y_star_right_rad_left_cut_block_sigma) &&
						// IsBetweenValues(theta_y_IP5_rad_rot_Beam_1_with_beam_divergence, theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_y1, 	theta_y_star_left_rad_theta_y_star_right_rad_right_cut_block_sigma))
						{
							// double minus_t_y_GeV2 = -ProtonReconstruction->ReconstructFourMomentumTransferSquaredGeV2(0, theta_y_star_rad, ProjectParameters->GetBeam_1_Optics()) ;

							Double_t parameters[3] ;

							parameters[0] = beam_momentum_GeV ;
							parameters[1] = theta_y_star_rad_min ;
							parameters[2] = theta_y_star_rad_max ;

							double weight_geometrical = preliminary_acceptance_beam_1_diagonal_left_bottom_right_top_function(&minus_t_GeV2_reconstructed, parameters) ;
							double weight_beam_divergence = ProjectParameters->Get_preliminary_acceptance_beam_1()->GetBeamDivergenceAcceptanceCorrection_TF1()->Eval(minus_t_y_GeV2) ;

							MC_dN_dt_GeV2_acceptance_corrected->Fill(minus_t_with_beam_divergence_GeV2, weight_geometrical * 1.0) ;
							MC_dN_dt_GeV2_acceptance_corrected_divergence_corrected->Fill(minus_t_with_beam_divergence_GeV2, weight_geometrical * weight_beam_divergence) ;

							theta_x_IP5_rad_rot_theta_y_IP5_rad_rot_distribution_after_cut->Fill(theta_x_IP5_rad_rot_Beam_1, theta_y_IP5_rad_rot_Beam_1) ;
							minus_t_y_GeV2_distribution_after_cuts->Fill(minus_t_with_beam_divergence_GeV2) ;
							
							MC_minus_t_GeV2_minus_t_GeV2_reconstructed_after_cuts->Fill(minus_t_GeV2, minus_t_GeV2_reconstructed) ;							

							if(ProjectParameters->IsParameterDefined("take_into_account_acceptance_cuts_in_unfolding") && (ProjectParameters->GetParameterValue("take_into_account_acceptance_cuts_in_unfolding") == 1.0))
							{
								MC_dN_dt_GeV2->Fill(minus_t_GeV2, MC_weight) ;
							}

						}
					}

				}
			}
		}

		MC_dN_dt_GeV2->Scale(1.0, "width") ;
		MC_dN_dt_GeV2_corrected->Scale(1.0, "width") ;

		if((ProjectParameters->IsSettingDefined("MonteCarlo_task")) && (ProjectParameters->GetSettingValue("MonteCarlo_task").compare("unfolding") == 0.0))
		{
			if(ProjectParameters->IsParameterDefined("New_unfolding_method"))
			{
				cout << "I am here unfolding1" << endl ;
				TH1D *corrected_analysis_plot = (TH1D *)((TAnalysisPlot1D *)PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding))->GetPlot()->Clone("corrected_analysis_plot") ;
				corrected_analysis_plot->Scale(1.0, "width") ;
				// corrected_analysis_plot->Multiply(dN_dt_GeV2_correction_for_unfolding) ;

				MC_dN_dt_GeV2->Write() ;
				MC_dN_dt_GeV2_corrected->Write() ;
				MC_dN_dt_GeV2_constant_bin_width->Write() ;

				cout << "I am here unfolding2" << endl ;
				MC_dN_dt_GeV2->Divide(corrected_analysis_plot) ;

				MC_dN_dt_GeV2->Clone("division") ;

				cout << "I am here unfolding3" << endl ;
				if(dN_dt_GeV2_correction_for_unfolding != NULL) dN_dt_GeV2_correction_for_unfolding->Multiply(MC_dN_dt_GeV2) ; 
				else dN_dt_GeV2_correction_for_unfolding = (TH1D *)MC_dN_dt_GeV2->Clone("unfolding_correction") ;
					

				cout << "I am here unfolding4" << endl ;
				// TH1D *dN_dt_GeV2_correction_for_unfolding_graph_temp = (TH1D *)dN_dt_GeV2_correction_for_unfolding->Clone("dN_dt_GeV2_correction_for_unfolding_graph_temp") ;
				// dN_dt_GeV2_correction_for_unfolding_graph_temp->Rebin(2) ;
				
				// dN_dt_GeV2_correction_for_unfolding_graph_temp->Multiply(identity_function, 0.5) ;
				
				// dN_dt_GeV2_correction_for_unfolding_graph_temp->Smooth(5) ;
				// dN_dt_GeV2_correction_for_unfolding->Smooth(15) ;

				if(smooth_iterations > 0)
				{
					dN_dt_GeV2_correction_for_unfolding->GetXaxis()->SetRangeUser(0.04, 4.7) ;
					dN_dt_GeV2_correction_for_unfolding->Smooth(smooth_iterations, "R") ;
				}
				else
				{
					if(smooth_iterations < 0)
					{
						cout << "Error: TProject::ExecuteMonteCarlo(): smooth iterations is less than 0 !" << endl ;
						exit(1) ;
					}
				}

				TGraph *dN_dt_GeV2_correction_for_unfolding_graph_dense = new TGraph(dN_dt_GeV2_correction_for_unfolding) ;

				/*for(int graph_i = 0 ; graph_i < ((MC_dN_dt_GeV2_number_of_bins/2)-2) ; ++graph_i)
				{
					double x1,y1 ;
					double x2,y2 ;

					dN_dt_GeV2_correction_for_unfolding_graph->GetPoint((2 * graph_i),   x1, y1) ;
					dN_dt_GeV2_correction_for_unfolding_graph->GetPoint((2 * graph_i)+1, x2, y2) ;

					double avg_y = (y1 + y2) / 2.0 ;

					dN_dt_GeV2_correction_for_unfolding_graph->SetPoint((2 * graph_i),   x1,  avg_y) ;
					dN_dt_GeV2_correction_for_unfolding_graph->SetPoint((2 * graph_i)+1, x2,  avg_y) ;
				}*/

				dN_dt_GeV2_correction_for_unfolding_graph_dense->SetName("graph_dense") ;
				dN_dt_GeV2_correction_for_unfolding_graph_dense->Write() ;

				cout << "I am here unfolding5" << endl ;
				dN_dt_GeV2_correction_for_unfolding->Write() ;
			}
			else
			{
				if(PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding) != NULL)
				{
					TH1D *analysis_plot = (TH1D *)((TAnalysisPlot1D *)PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding))->GetPlot()->Clone("analysis_plot") ;
					TH1D *corrected_analysis_plot = (TH1D *)((TAnalysisPlot1D *)PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(histogram_name_for_unfolding))->GetPlot()->Clone("corrected_analysis_plot") ;

					if(dN_dt_GeV2_correction_for_unfolding != NULL) corrected_analysis_plot->Multiply(dN_dt_GeV2_correction_for_unfolding) ;

					cout << "Division 1 start: " << corrected_analysis_plot->GetBinLowEdge(1) << "  " << MC_dN_dt_GeV2->GetBinLowEdge(1) << endl ;
					cout << "Division 1 start: " << corrected_analysis_plot->GetNbinsX() << "  " << MC_dN_dt_GeV2->GetNbinsX() << endl ;
					cout << "Division 1 start: " << corrected_analysis_plot->GetBinWidth(1) << "  " << MC_dN_dt_GeV2->GetBinWidth(1) << endl ;

					corrected_analysis_plot->Divide(MC_dN_dt_GeV2) ;
					cout << "Division 1 end" << endl ;

					stringstream ss ;
					ss << iteration ;

					if(dN_dt_GeV2_correction_for_unfolding != NULL) dN_dt_GeV2_correction_for_unfolding->Divide(corrected_analysis_plot) ;
					else
					{
						dN_dt_GeV2_correction_for_unfolding = (TH1D *)MC_dN_dt_GeV2->Clone("dN_dt_GeV2_correction_for_unfolding") ;
						cout << "Division 2 start" << endl ;
						dN_dt_GeV2_correction_for_unfolding->Divide(analysis_plot) ;
						cout << "Division 2 end" << endl ;
					}

					for(int i = 0 ; i < dN_dt_GeV2_correction_for_unfolding->GetEntries() ; ++i) dN_dt_GeV2_correction_for_unfolding->SetBinError(i, 0.0) ;
					dN_dt_GeV2_correction_for_unfolding->Write() ;

				}
				else
				{
					cout << "Error: TProject::ExecuteMonteCarlo(): there is no reference histogram for unfolding!" << endl ;
					exit(1) ;
				}
			}
		}

		// did_any_of_the_cuts_move = PlotsCollections["PlotsCollection"]->AdjustCuts() ;
		// if(did_any_of_the_cuts_move) PlotsCollections["PlotsCollection"]->Reset() ;

		iteration++ ;
		did_any_of_the_cuts_move = false ;
		
		// MC_dN_dt_GeV2_graph_for_unfolding->Write() ;

	} while(iteration < maxiteration) ;

	TMatrixD *response_matrix = new TMatrixD(39, 39) ; // improve the dimensions

	for(int i = 0 ; i < 39 ; ++i) // improve the dimensions
	for(int j = 0 ; j < 39 ; ++j) // improve the dimensions
	{
		(*response_matrix)(i,j) = response_matrix_histogram->GetBinContent(i+1, j+1) ; // improve the dimensions
	}

	response_matrix->Print() ; // improve the dimensions

	TMatrixD response_matrix_inverted = response_matrix->Invert() ; // improve the dimensions
	response_matrix_inverted.Print() ; // improve the dimensions
}

void TProject::FindCuts(TReducedNtuple *ntuple, string collection_name, TProjectInformation *ProjectInformation, int index_of_root_file)
{
	int iteration = 1 ;
	bool did_any_of_the_cuts_move = false ;

	do
	{
		cerr << "Iteration : " << iteration << endl ;

		ntuple->Loop(ProjectParameters, ProtonReconstruction, &PlotsCollections, collection_name, ProjectInformation, index_of_root_file) ;

		did_any_of_the_cuts_move = PlotsCollections["PlotsCollection"]->AdjustCuts() ;

		PlotsCollections["PlotsCollection"]->Reset() ;

		iteration++ ;

	} while(did_any_of_the_cuts_move) ;

	cout << "Final summary:" << endl ;

	PlotsCollections["PlotsCollection"]->PrintCutCoordinates() ;
}

void TProject::SaveAdditionalInformationToRootFile(TFile *,  TProjectInformation *ProjectInformation)
{
	TTree *tree = new TTree("Properties", "Properties") ;


	tree->Branch("Start_timestamp"	, &ProjectInformation->event_info_timestamp_start	, "Start_timestamp/l") ;
	tree->Branch("End_timestamp"	, &ProjectInformation->event_info_timestamp_end		, "End_timestamp/l") ;

	tree->Fill() ;
}


int TProject::Execute()
{
	// To generate the timeslices saved in prj file
	//
	// ULong64_t start_value = 1360547340 ;
	// ULong64_t end_value = start_value + 100 ;
	//
	// for(int i = 0 ; i < 26 ; ++i)
	// {
	//	cout << "<nparameter>		timeslice_start_end_pairs_second	" << start_value << endl ;
	//	cout << "<nparameter>		timeslice_start_end_pairs_second	" << end_value << endl << endl ;
	//
	//	start_value += 100 ;
	// 	end_value += 100 ;
	// }

	const string generic_filename = "Generic.root" ;

	if((ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0) &&
		((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0)
		|| (ProjectParameters->GetSettingValue("project_subtask").compare("merge_and_post_process") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("diffraction") == 0)))
	{
		generic_file = new TFile((ProjectParameters->GetSettingValue("Output_directory") + "/" + generic_filename).c_str(),"RECREATE","Generic");

		elastic_events_per_multiplicity = new TH1F("elastic_events_per_multiplicity", "elastic_events_per_multiplicity", 100, 0, 100) ;
		events_per_multiplicity = new TH1F("events_per_multiplicity", "events_per_multiplicity", 100, 0, 100) ;
		elastic_events_per_number_of_clusters = new TH1F("elastic_events_per_number_of_clusters", "elastic_events_per_number_of_clusters", 100, 0, 100) ;
		events_per_number_of_clusters = new TH1F("events_per_number_of_clusters", "events_per_number_of_clusters", 100, 0, 100) ;
		elastic_events_per_number_of_multitracks = new TH1F("elastic_events_per_number_of_multitracks", "elastic_events_per_number_of_multitracks", 100, 0, 100) ;
		events_per_number_of_multitracks = new TH1F("events_per_number_of_multitracks", "events_per_number_of_multitracks", 100, 0, 100) ;
		
		number_of_cluster_for_elastic_events_below_threshold = new TH1F("number_of_cluster_for_elastic_events_below_threshold", "number_of_cluster_for_elastic_events_below_threshold", 100, 0, 100) ;

		number_of_RPs_close_to_overload_hist = new TH1F("number_of_RPs_close_to_overload_hist", "number_of_RPs_close_to_overload_hist", 100, 0, 100) ;
		elastic_number_of_RPs_close_to_overload_hist = new TH1F("elastic_number_of_RPs_close_to_overload_hist", "elastic_number_of_RPs_close_to_overload_hist", 100, 0, 100) ;

		elastic_events_closest_to_overload = new TH1F("elastic_events_closest_to_overload", "elastic_events_closest_to_overload", 100, 0, 100) ;
		elastic_events_closest_to_overload_pert_up_1 = new TH1F("elastic_events_closest_to_overload_pert_up_1", "elastic_events_closest_to_overload_pert_up_1", 100, 0, 100) ;
		elastic_events_closest_to_overload_pert_down_1 = new TH1F("elastic_events_closest_to_overload_pert_down_1", "elastic_events_closest_to_overload_pert_down_1", 100, 0, 100) ;

		events_closest_to_overload = new TH1F("events_closest_to_overload", "events_closest_to_overload", 100, 0, 100) ;
		events_closest_to_overload_pert_up_1 = new TH1F("events_closest_to_overload_pert_up_1", "events_closest_to_overload_pert_up_1", 100, 0, 100) ;
		events_closest_to_overload_pert_down_1 = new TH1F("events_closest_to_overload_pert_down_1", "events_closest_to_overload_pert_down_1", 100, 0, 100) ;

		events_close_to_overload_minus_one = new TH1F("events_close_to_overload_minus_one", "events_close_to_overload_minus_one", 100, 0, 100) ;
		events_close_to_overload_minus_two = new TH1F("events_close_to_overload_minus_two", "events_close_to_overload_minus_two", 100, 0, 100) ;

		elastic_events_close_to_overload_minus_one = new TH1F("elastic_events_close_to_overload_minus_one", "elastic_events_close_to_overload_minus_one", 100, 0, 100) ;
		elastic_events_close_to_overload_minus_two = new TH1F("elastic_events_close_to_overload_minus_two", "elastic_events_close_to_overload_minus_two", 100, 0, 100) ;

		events_close_to_overload_plus_one = new TH1F("events_close_to_overload_plus_one", "events_close_to_overload_plus_one", 100, 0, 100) ;
		events_close_to_overload_plus_two = new TH1F("events_close_to_overload_plus_two", "events_close_to_overload_plus_two", 100, 0, 100) ;

		elastic_events_close_to_overload_plus_one = new TH1F("elastic_events_close_to_overload_plus_one", "elastic_events_close_to_overload_plus_one", 100, 0, 100) ;
		elastic_events_close_to_overload_plus_two = new TH1F("elastic_events_close_to_overload_plus_two", "elastic_events_close_to_overload_plus_two", 100, 0, 100) ;

		events_per_number_of_clusters_per_event = new TH1F("events_per_number_of_clusters_per_event", "events_per_number_of_clusters_per_event", 2000, 0, 2000) ;
		events_per_number_of_clusters_per_below_overload_per_event = new TH1F("events_per_number_of_clusters_per_below_overload_per_event", "events_per_number_of_clusters_per_below_overload_per_event", 2000, 0, 2000) ;

		elastic_events_per_number_of_clusters_per_event = new TH1F("elastic_events_per_number_of_clusters_per_event", "elastic_events_per_number_of_clusters_per_event", 2000, 0, 2000) ;
		elastic_events_per_number_of_clusters_per_below_overload_per_event = new TH1F("elastic_events_per_number_of_clusters_per_below_overload_per_event", "elastic_events_per_number_of_clusters_per_below_overload_per_event", 2000, 0, 2000) ;

		number_of_clusters_vs_t_distribution = new TH2F("number_of_clusters_vs_t_distribution", "number_of_clusters_vs_t_distribution", 100, 0, 1, 200, 0, 200) ;

		normalization_for_3_out_of_4_most_general_case = new TH1D("normalization_for_3_out_of_4_most_general_case", "normalization_for_3_out_of_4_most_general_case", 100, 0, 100) ;
		normalization_for_2_out_of_4_most_general_case = new TH1D("normalization_for_2_out_of_4_most_general_case", "normalization_for_2_out_of_4_most_general_case", 100, 0, 100) ;
		
		normalization_for_3_out_of_4_most_general_case->Sumw2() ;
		normalization_for_2_out_of_4_most_general_case->Sumw2() ;

		number_of_clusters_per_plane = new TH1F("number_of_clusters_per_plane", "number_of_clusters_per_plane", 100, 0, 100) ;

		max_number_of_clusters_in_RPs_per_event_hist = new TH1F("max_number_of_clusters_in_RPs_per_event_hist", "max_number_of_clusters_in_RPs_per_event_hist", 100, 0, 100) ;
		elastic_max_number_of_clusters_in_RPs_per_event_hist = new TH1F("elastic_max_number_of_clusters_in_RPs_per_event_hist", "elastic_max_number_of_clusters_in_RPs_per_event_hist", 100, 0, 100) ;

	}

	if((ProjectParameters->GetSettingValue("project_task").compare("data_reduction") != 0) && ((generic_file == NULL) || (generic_file->IsZombie())))
	{
		cout << "Error: TProject::Execute: the file cannot be opened ! " << generic_filename << endl ;
		exit(1) ;
	}

	if((ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0) && ((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0)))
	{
		CreatePlotsCollections() ;
	}


	if(ProjectParameters->GetSettingValue("project_subtask").compare("find_cuts") == 0)
	{
		if(ProjectParameters->GetParameterValue("SaveRuns") == 1.0)
		{
			cout << "Error: TProject::Execute: plots per run cannot be saved if project_subtask is \"find_cuts\"." << endl ;
			exit(1) ;
		}
	}

	if((ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0) && ((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0)))
	{
		string label_of_vertical_beam_divergence_plot = ProjectParameters->GetSettingValue("label_of_vertical_beam_divergence_plot") ;
		// PTAnalysisPlot_type theta_y_star_left_rad_theta_y_star_right_rad_with_cut = PlotsCollections["PlotsCollection"]->GetPAnalysisPlotByLabel(label_of_vertical_beam_divergence_plot) ;
		// ProjectParameters->Get_preliminary_acceptance_beam_1()->SetTestPlot(((TAnalysisPlot2D *)theta_y_star_left_rad_theta_y_star_right_rad_with_cut)->GetPlot()) ; // for test only

		ProjectParameters->Get_preliminary_acceptance_beam_1()->SaveAcceptanceFunctions() ;
	}

	if(ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0)
	{
		ExecuteMonteCarlo() ;
	}
	else if(ProjectParameters->GetSettingValue("project_subtask").compare("merge_and_post_process") == 0)
	{
		MergeAndPostProcess() ;
	}
	else if(ProjectParameters->GetSettingValue("project_subtask").compare("diffraction") == 0)
	{

			// To be improved with TPlotsCollection and with .prj description

			x_mm_near_x_mm_far_left_with_cut = new TAnalysisPlot2D("x_mm_near_x_mm_far_left_with_cut", "x_mm_near_x_mm_far_left_with_cut", ProjectParameters, ProtonReconstruction) ;
			x_mm_near_x_mm_far_right_with_cut = new TAnalysisPlot2D("x_mm_near_x_mm_far_right_with_cut", "x_mm_near_x_mm_far_right_with_cut", ProjectParameters, ProtonReconstruction) ;

			x_mm_near_x_mm_far_left_after_y_cut_with_cut = new TAnalysisPlot2D("x_mm_near_x_mm_far_left_after_y_cut_with_cut", "x_mm_near_x_mm_far_left_after_y_cut_with_cut", ProjectParameters, ProtonReconstruction) ;
			x_mm_near_x_mm_far_right_after_y_cut_with_cut = new TAnalysisPlot2D("x_mm_near_x_mm_far_right_after_y_cut_with_cut", "x_mm_near_x_mm_far_right_after_y_cut_with_cut", ProjectParameters, ProtonReconstruction) ;

			x_mm_near_x_mm_far_left = new TAnalysisPlot2D("x_mm_near_x_mm_far_left", "x_mm_near_x_mm_far_left", ProjectParameters, ProtonReconstruction) ;
			
			int bin_number = 3200 ;

			track_rp_2_x_track_rp_2_y = new TH2F("track_rp_2_x_track_rp_2_y", "track_rp_2_x_track_rp_2_y", bin_number, 0, 40, bin_number,-20,20) ;
			track_rp_3_x_track_rp_3_y = new TH2F("track_rp_3_x_track_rp_3_y", "track_rp_3_x_track_rp_3_y", bin_number, 0, 40, bin_number,-20,20) ;

			track_rp_102_x_track_rp_102_y = new TH2F("track_rp_102_x_track_rp_102_y", "track_rp_102_x_track_rp_102_y", bin_number,0,40,bin_number,-20,20) ;
			track_rp_103_x_track_rp_103_y = new TH2F("track_rp_103_x_track_rp_103_y", "track_rp_103_x_track_rp_103_y", bin_number,0,40,bin_number,-20,20) ;

			track_rp_3_x_track_rp_3_y_with_cut = new TH2F("track_rp_3_x_track_rp_3_y_with_cut", "track_rp_3_x_track_rp_3_y_with_cut", bin_number,0,40,bin_number,-20,20) ;

			track_rp_102_x_track_rp_102_y_with_cut = new TH2F("track_rp_102_x_track_rp_102_y_with_cut", "track_rp_102_x_track_rp_102_y_with_cut", bin_number,0,40,bin_number,-20,20) ;
			track_rp_103_x_track_rp_103_y_with_cut = new TH2F("track_rp_103_x_track_rp_103_y_with_cut", "track_rp_103_x_track_rp_103_y_with_cut", bin_number,0,40,bin_number,-20,20) ;
			
			x_far_mm_minus_x_near_vs_xnear_left = new TH2F("x_far_mm_minus_x_near_vs_xnear_left", "x_far_mm_minus_x_near_vs_xnear_left", bin_number, 0, 20, bin_number, -3, 3) ;
			x_far_mm_minus_x_near_vs_xnear_left_with_x_cut = new TH2F("x_far_mm_minus_x_near_vs_xnear_left_with_x_cut", "x_far_mm_minus_x_near_vs_xnear_left_with_x_cut", bin_number, 0, 20, bin_number, -3, 3) ;

			x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_1 = new TH2F("x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_1", "x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_1", 200, 0, 20, 200, -3, 3) ;
			x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_2 = new TH2F("x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_2", "x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_2", 200, 0, 20, 200, -3, 3) ;
			x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_slice_3 = new TH2F("x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_3", "x_far_mm_minus_x_near_vs_xnear_right_with_y_far_per_y_near_cut_3", 200, 0, 20, 200, -3, 3) ;

			x_far_mm_minus_x_near_vs_xnear_left_with_y_far_per_y_near_cut_slice_1 = new TH2F("x_far_mm_minus_x_near_vs_xnear_left_with_y_far_per_y_near_cut_slice_1", "x_far_mm_minus_x_near_vs_xnear_left_with_y_far_per_y_near_cut_slice_1", 200, 0, 20, 200, -3, 3) ;
						
			y_far_per_y_near_y_near_right = new TH2F("y_far_per_y_near_y_near_right", "y_far_per_y_near_y_near_right", bin_number, -20, 20, bin_number, -10, 10) ;
			y_far_per_y_near_x_near_right = new TH2F("y_far_per_y_near_x_near_right", "y_far_per_y_near_x_near_right", bin_number, 0, 20, bin_number, -10, 10) ;
			
			y_far_per_y_near_x_far_left =  new TH2F("y_far_per_y_near_x_far_left", "y_far_per_y_near_x_far_left", bin_number, 0, 20, bin_number, -10, 10) ;
			
			// for x double upper_limit = 0.04 ;
			double upper_limit = 1.0 ;
			int number_of_bins_in_xi_distributions = 400 ;
			
			track_rp_2_xi_distribution = new TH1F("track_rp_2_xi_distribution","track_rp_2_xi_distribution", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_3_xi_distribution = new TH1F("track_rp_3_xi_distribution","track_rp_3_xi_distribution", number_of_bins_in_xi_distributions, 0, upper_limit) ;

			for(int i = 0 ; i < ctpps_number_of_histograms ; ++i)
			{
				stringstream ss ;
				
				ss << i ;
				
				string name = ("track_rp_3_xi_distribution_" + ss.str()) ;
				string name2 = ("track_rp_103_xi_distribution_" + ss.str()) ;

				string name3 = ("track_rp_3_xi_distribution_shifted_" + ss.str()) ;
				string name4 = ("track_rp_103_xi_distribution_shifted_" + ss.str()) ;

				TH1F *aux = new TH1F(name.c_str(), name.c_str(), number_of_bins_in_xi_distributions, 0, upper_limit) ;
				TH1F *aux2 = new TH1F(name2.c_str(), name2.c_str(), number_of_bins_in_xi_distributions, 0, upper_limit) ;

				TH1F *aux3 = new TH1F(name3.c_str(), name3.c_str(), number_of_bins_in_xi_distributions, 0, upper_limit) ;
				TH1F *aux4 = new TH1F(name4.c_str(), name4.c_str(), number_of_bins_in_xi_distributions, 0, upper_limit) ;
			
				aux->Sumw2() ;
				aux2->Sumw2() ;

				aux3->Sumw2() ;
				aux4->Sumw2() ;

				vector_track_rp_3_xi_distribution.push_back(aux) ;
				vector_track_rp_103_xi_distribution.push_back(aux2) ;

				vector_track_rp_3_xi_distribution_shifted.push_back(aux3) ;
				vector_track_rp_103_xi_distribution_shifted.push_back(aux4) ;
			}

			for(int i = 0 ; i < ctpps_number_of_histograms ; ++i)
			{
				cout << "i:" << i << " vector " << vector_track_rp_3_xi_distribution[i]->GetName() << endl ;
			}
			

			track_rp_3_xi_distribution_with_x_xi_function = new TH1F("track_rp_3_xi_distribution_with_x_xi_function","track_rp_3_xi_distribution_with_x_xi_function", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut = new TH1F("track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut","track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;

			track_rp_102_xi_distribution = new TH1F("track_rp_102_xi_distribution","track_rp_102_xi_distribution", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_103_xi_distribution = new TH1F("track_rp_103_xi_distribution","track_rp_103_xi_distribution", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_103_xi_distribution_with_x_xi_function = new TH1F("track_rp_103_xi_distribution_with_x_xi_function","track_rp_103_xi_distribution_with_x_xi_function", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut = new TH1F("track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut","track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;

			track_rp_2_xi_distribution_with_cut = new TH1F("track_rp_2_xi_distribution_with_cut","track_rp_2_xi_distribution_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_3_xi_distribution_with_cut = new TH1F("track_rp_3_xi_distribution_with_cut","track_rp_3_xi_distribution_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;

			track_rp_102_xi_distribution_with_cut = new TH1F("track_rp_102_xi_distribution_with_cut","track_rp_102_xi_distribution_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;
			track_rp_103_xi_distribution_with_cut = new TH1F("track_rp_103_xi_distribution_with_cut","track_rp_103_xi_distribution_with_cut", number_of_bins_in_xi_distributions, 0, upper_limit) ;

			for(vector<string>::iterator it = ProjectParameters->GetSettingsVector("Input_root_file_name")->begin() ; it != ProjectParameters->GetSettingsVector("Input_root_file_name")->end() ; ++it)
			{
				TotemNtuple_CTPPS_2016 *ntuple = new TotemNtuple_CTPPS_2016(*it, ProjectParameters->GetSettingValue("Input_directory")) ;	

				generic_file->cd() ;

				ntuple->Loop(ProjectParameters, ProtonReconstruction, &PlotsCollections, &timestamps) ;
			}

			generic_file->cd() ;

			x_mm_near_x_mm_far_left_with_cut->SavePlot() ;
			x_mm_near_x_mm_far_right_with_cut->SavePlot() ;

			x_mm_near_x_mm_far_left_after_y_cut_with_cut->SavePlot() ;
			x_mm_near_x_mm_far_right_after_y_cut_with_cut->SavePlot() ;

			x_mm_near_x_mm_far_left->SavePlot() ;

			TH1D *track_rp_3_minus_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut = (TH1D *)track_rp_3_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Clone("track_rp_3_minus_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut") ;
			track_rp_3_minus_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Add(track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut, -1.0) ;
			track_rp_3_minus_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Divide(track_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut) ;

			track_rp_3_minus_rp_103_xi_distribution_with_x_xi_function_after_y_cut_with_cut->Write() ;
			
			TCanvas *difference_summary = new TCanvas("difference_summary", "difference_summary") ;
			TCanvas *difference_summary_scaled = new TCanvas("difference_summary_scaled", "difference_summary_scaled") ;
			TCanvas *difference_summary_shifted = new TCanvas("difference_summary_shifted", "difference_summary_shifted") ;

			for(int i = 0 ; i < ctpps_number_of_histograms  ; ++i)
			{
				stringstream ss ;

				ss << i ;

				string name = ("difference_" + ss.str()) ;

				TH1D *difference = (TH1D *)vector_track_rp_3_xi_distribution[i]->Clone(name.c_str()) ;

				difference->Add(vector_track_rp_103_xi_distribution[i], -1) ;
				
				difference_summary->cd() ;

				difference->SetLineColor(i+1) ;
				
				if(i == (ctpps_number_of_histograms / 2)) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) + 4) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) - 4) difference->SetLineWidth(5) ;
			
				if(i == 0) difference->Draw() ;
				else difference->Draw("same") ;
			}

			TF1 *identity_function = new TF1("identity_function", "1.0", -10.0, 10.0); 
			double detector_inefficiency_factor = 1.02 ;
		
			for(int i = 0 ; i < ctpps_number_of_histograms  ; ++i)
			{
				stringstream ss ;

				ss << i ;

				string name = ("difference_scaled_" + ss.str()) ;

				TH1D *difference = (TH1D *)vector_track_rp_3_xi_distribution[i]->Clone(name.c_str()) ;
				
				difference->Multiply(identity_function, detector_inefficiency_factor) ;

				difference->Add(vector_track_rp_103_xi_distribution[i], -1) ;
				
				difference_summary_scaled->cd() ;

				difference->SetLineColor(i+1) ;
				
				if(i == (ctpps_number_of_histograms / 2)) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) + 4) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) - 4) difference->SetLineWidth(5) ;
			
				if(i == 0) difference->Draw() ;
				else difference->Draw("same") ;
			}

			for(int i = 0 ; i < ctpps_number_of_histograms  ; ++i)
			{
				stringstream ss ;

				ss << i ;

				string name = ("difference_shifted_" + ss.str()) ;

				TH1D *difference = (TH1D *)vector_track_rp_3_xi_distribution_shifted[i]->Clone(name.c_str()) ;

				difference->Add(vector_track_rp_103_xi_distribution_shifted[i], -1) ;
				
				difference_summary_shifted->cd() ;

				difference->SetLineColor(i+1) ;
				
				if(i == (ctpps_number_of_histograms / 2)) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) + 4) difference->SetLineWidth(5) ;
				if(i == (ctpps_number_of_histograms / 2) - 4) difference->SetLineWidth(5) ;
			
				if(i == 0) difference->Draw() ;
				else difference->Draw("same") ;
			}
			
			difference_summary->Write() ;
			difference_summary_shifted->Write() ;
			difference_summary_scaled->Write() ;

	}
	else
	{
		TProjectInformation *ProjectInformation = new TProjectInformation ;

		int index_of_root_file = 0 ;

		for(vector<string>::iterator it = ProjectParameters->GetSettingsVector("Input_root_file_name")->begin() ; it != ProjectParameters->GetSettingsVector("Input_root_file_name")->end() ; ++it)
		{

			index_of_root_file++ ;

			if(ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0)
			{
				if(ProjectParameters->IsSettingsDefined("ntuple_name_with_shifted_trigger_block"))
					for(vector<string>::iterator it_excluded = ProjectParameters->GetSettingsVector("ntuple_name_with_shifted_trigger_block")->begin() ; it_excluded != ProjectParameters->GetSettingsVector("ntuple_name_with_shifted_trigger_block")->end() ; ++it_excluded)
					{
						if(it_excluded->compare(*it) == 0.0)
						{
							cout << "Error: TProject::Execute: the ntuple is exluded due to trigger block shift ! : " << (*it) << endl ;
							exit(1) ;
						}	
					}
			}
			
			string collection_name = "PlotsCollection_" + (*it) ;

			if(ProjectParameters->GetParameterValue("SaveRuns") == 1.0) PlotsCollections[collection_name] = new TPlotsCollection(collection_name, collection_name, ProjectParameters, luminosity, ProtonReconstruction) ;

			cout << "Input directory: " << ProjectParameters->GetSettingValue("Input_directory") << endl ;
			(*(ProjectParameters->GetPLogFile())) << GetLocalTime() << ": " << (*it) << " is now analyzed." << endl ;

			system("date") ;

			string filename = ProjectParameters->GetSettingValue("Input_directory") + (*it) ;

			(*ProjectParameters->GetPLatexFile()) << "{\\tiny Input file: "  ;
			ProjectParameters->GetLatexFile()->PrintTextWithUnderScore(filename) ;
			(*ProjectParameters->GetPLatexFile()) << "} \\newline " << endl ;

			ProjectParameters->GetLatexFile()->PrintTextWithUnderScore("MADX_label_of_left_near_RP_detector:  " + ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector") + " \\newline ") ;
			ProjectParameters->GetLatexFile()->PrintTextWithUnderScore("MADX_label_of_left_far_RP_detector:   " + ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector") + " \\newline ") ;

			ProjectParameters->GetLatexFile()->PrintTextWithUnderScore("MADX_label_of_right_near_RP_detector: " + ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector") + " \\newline ") ;
			ProjectParameters->GetLatexFile()->PrintTextWithUnderScore("MADX_label_of_right_far_RP_detector:  " + ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector") + " \\newline ") ;

			if(ProjectParameters->GetParameterValue("ReducedBinaryDataset") == 2.0)
			{
				string treename = "TReducedNtuple" ;

				/*
				TFile *input_file = TFile::Open((ProjectParameters->GetSettingValue("Input_directory") + (*it)).c_str(), "READ") ;

				char buffer[TConstants::buffersize_for_property_string] ;

			        TTree *Properties = (TTree *)input_file->Get("Properties");
				Properties->SetBranchAddress("diagonal", buffer);
			        Properties->GetEntry(0) ;

				input_file->Close() ;

				string diagonal(buffer) ;
				if(diagonal.compare(ProjectParameters->GetSettingValue("analysed_diagonal")) != 0)
				{
					cout << "Error: TProject::Execute: the analyzed diagonal disagree with the reduced ntuple diagonal !" << endl ;
					exit(1) ;
				} */
				
				// Few tests

				TFile *input_file = TFile::Open((ProjectParameters->GetSettingValue("Input_directory") + (*it)).c_str(), "READ") ;
			        TTree *Properties = (TTree *)input_file->Get("Properties");

				if(Properties != NULL)
				{
					char buffer[TConstants::buffersize_for_property_string] ;
					Properties->SetBranchAddress("MADX_label_of_left_near_RP_detector", buffer);
			        	Properties->GetEntry(0) ;
					if(ProjectParameters->GetSettingValue("MADX_label_of_left_near_RP_detector").compare(buffer) != 0.0)
					{
						cout << "Error: TProject::Execute: wrong topology, left near !" << endl ;
						exit(1) ;
					}

					Properties->SetBranchAddress("MADX_label_of_left_far_RP_detector", buffer);
			        	Properties->GetEntry(0) ;
					if(ProjectParameters->GetSettingValue("MADX_label_of_left_far_RP_detector").compare(buffer) != 0.0)
					{
						cout << "Error: TProject::Execute: wrong topology, left far !" << endl ;
						exit(1) ;
					}

					Properties->SetBranchAddress("MADX_label_of_right_near_RP_detector", buffer);
			        	Properties->GetEntry(0) ;
					if(ProjectParameters->GetSettingValue("MADX_label_of_right_near_RP_detector").compare(buffer) != 0.0)
					{
						cout << "Error: TProject::Execute: wrong topology, right near !" << endl ;
						exit(1) ;
					}

					Properties->SetBranchAddress("MADX_label_of_right_far_RP_detector", buffer);
			        	Properties->GetEntry(0) ;
					if(ProjectParameters->GetSettingValue("MADX_label_of_right_far_RP_detector").compare(buffer) != 0.0)
					{
						cout << "Error: TProject::Execute: wrong topology, right far !" << endl ;
						exit(1) ;
					}
				}
				else
				{
					cout << "Error: TProject::Execute: property tree is missing to validate topology !" << endl ;
					// exit(1) ;
				}
				
				// ==========================================

				bool multitrack_in_elastic = false ;

				if((ProjectParameters->IsParameterDefined("multitrack_in_elastic")) && (ProjectParameters->GetParameterValue("multitrack_in_elastic") == 1.0))
				{
					multitrack_in_elastic = true ;
				}

				TReducedNtuple *ntuple = new TReducedNtuple(*it, ProjectParameters->GetSettingValue("Input_directory"), treename, multitrack_in_elastic) ;
				generic_file->cd() ;

				if(ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0)
				{
					ntuple->Loop(ProjectParameters, ProtonReconstruction, &PlotsCollections, collection_name, ProjectInformation, index_of_root_file) ;
					generic_file->cd() ;

					if(ProjectParameters->IsSettingDefined("project_subsubtask"))
					{
						if(ProjectParameters->GetSettingValue("project_subsubtask").compare("find_cuts") == 0)
						{
							FindCuts(ntuple, collection_name, ProjectInformation, index_of_root_file) ;
						}
					}
				}
				else if(ProjectParameters->GetSettingValue("project_subtask").compare("find_cuts") == 0)
				{
					FindCuts(ntuple, collection_name, ProjectInformation, index_of_root_file) ;
				}

				delete ntuple ;
				
				input_file->Close() ;
				generic_file->cd() ;
			}
			else
			{
				TotemNtuple *ntuple = new TotemNtuple(*it, ProjectParameters->GetSettingValue("Input_directory")) ;

				if(generic_file != NULL) generic_file->cd() ;

				if(ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0)
				{
					ntuple->Loop(ProjectParameters, ProtonReconstruction, &PlotsCollections, &timestamps) ;
				}
				else if(ProjectParameters->GetSettingValue("project_task").compare("data_reduction") == 0)
				{
					(*(ProjectParameters->GetPLogFile())) << *it << " data reduction is starting." << endl ;

					if(ProjectParameters->IsParameterDefined("Correct_for_trigger_block_shift"))
					{
						(*(ProjectParameters->GetPLogFile())) << *it << ": Correct_for_trigger_block_shift parameter is defined and =" << ProjectParameters->GetParameterValue("Correct_for_trigger_block_shift") << endl ;
					}
					else
					{
						(*(ProjectParameters->GetPLogFile())) << *it << ": Correct_for_trigger_block_shift parameter is not defined" << endl ;
					}

					string ntuple_left_bottom_right_top_filename = GenerateOutputFilenameWithPath(TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP, (*it)) ;

					TFile *ntuple_left_bottom_right_top = TFile::Open(ntuple_left_bottom_right_top_filename.c_str(),"RECREATE");

					if((ntuple_left_bottom_right_top == NULL) || (ntuple_left_bottom_right_top->IsZombie()))
					{
						cout << "Error: TProject::Execute: the file cannot be opened !" << ntuple_left_bottom_right_top_filename << endl ;
						exit(1) ;
					}

					SetupProperties(ProjectParameters, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP) ;
					TTree *mytree_left_bottom_right_top = new TTree("TReducedNtuple", "TReducedNtuple") ;

					string ntuple_left_top_right_bottom_filename = GenerateOutputFilenameWithPath(TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM, (*it)) ;

					TFile *ntuple_left_top_right_bottom = TFile::Open(ntuple_left_top_right_bottom_filename.c_str(),"RECREATE");

					if((ntuple_left_top_right_bottom == NULL) || (ntuple_left_top_right_bottom->IsZombie()))
					{
						cout << "Error: TProject::Execute: the file cannot be opened !" << ntuple_left_top_right_bottom_filename << endl ;
						exit(1) ;
					}

					SetupProperties(ProjectParameters, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM) ;
					TTree *mytree_left_top_right_bottom = new TTree("TReducedNtuple", "TReducedNtuple");

					ntuple->SetupBranches(mytree_left_bottom_right_top, ProjectParameters, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_BOTTOM_RIGHT_TOP) ;
					ntuple->SetupBranches(mytree_left_top_right_bottom, ProjectParameters, TConstants::NTUPLE_BLOCK_DIAGONAL_LEFT_TOP_RIGHT_BOTTOM) ;

					ntuple->TestNumberOfBrunches() ;

					ntuple->Loop(ProjectParameters, mytree_left_bottom_right_top, mytree_left_top_right_bottom) ;

		        		ntuple_left_bottom_right_top->Write("",TObject::kOverwrite);
		        		ntuple_left_top_right_bottom->Write("",TObject::kOverwrite);
					(*(ProjectParameters->GetPLogFile())) << GetLocalTime() << ": " << (*it) << " data reduction finished, writing out files." << endl ;

	        			ntuple_left_bottom_right_top->Close();
	        			ntuple_left_top_right_bottom->Close();
					(*(ProjectParameters->GetPLogFile())) << GetLocalTime() << ": " << (*it) << " files are closed." << endl ;

				}
				else
				{
					cout << "TProject::TProject, unknown project task ! " << ProjectParameters->GetSettingValue("project_task") << endl ;
					exit(1) ;
				}

			}
			
			if(ProjectParameters->GetParameterValue("SaveRuns") == 1.0)
			{
				PlotsCollections[collection_name]->Save() ;
				PlotsCollections[collection_name]->Clear() ;
			}

			(*(ProjectParameters->GetPLogFile())) << GetLocalTime() << (*it) << " is finished successfully." << endl << endl ;
		}

		ProjectInformation->PrintTriggerBits() ;		

		SaveAdditionalInformationToRootFile(generic_file, ProjectInformation) ;

		cout << "First timestamp:" << ProjectInformation->event_info_timestamp_start << endl ;
		cout << "Last timestamp:" <<  ProjectInformation->event_info_timestamp_end << endl ;

	}

	(*ProjectParameters->GetPLatexFile()) << "\\newpage" << endl ;

	if((ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0) && ((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0)))
	{

		cout << "Start to print parameters: "  << endl ;
		cout << "=========================================================="  << endl ;

		PlotsCollections["PlotsCollection"]->PrintCutCoordinates() ;

		cout << "End of print parameters: "  << endl ;
		cout << "=========================================================="  << endl ;

		bool SaveEpsFiles = true ;

		if((ProjectParameters->IsParameterDefined("DoNotSaveEpsFiles")) && (ProjectParameters->GetParameterValue("DoNotSaveEpsFiles") == 1.0)) SaveEpsFiles = false ;

		if(SaveEpsFiles) PlotsCollections["PlotsCollection"]->Save() ;
	}

	if((ProjectParameters->GetSettingValue("project_task").compare("analyze") == 0) &&
		((ProjectParameters->GetSettingValue("project_subtask").compare("dsigma/dt") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("MonteCarlo") == 0) ||
		(ProjectParameters->GetSettingValue("project_subtask").compare("merge_and_post_process") == 0) || (ProjectParameters->GetSettingValue("project_subtask").compare("diffraction") == 0)))
	{
		generic_file->Write();
	}
	
	if(luminosity != NULL) luminosity->Save(&timestamps) ;

	(*(ProjectParameters->GetPLogFile())) << GetLocalTime() << ": " << "Successfully completed." << endl ;

	cout << "Successfully completed." << endl ;

	theta_y_star_difference.SaveAs("theta_y_star_difference.root") ;
	theta_x_star_difference.SaveAs("theta_x_star_difference.root") ;

	return 0 ;
}

#endif // #ifndef TProject_h
