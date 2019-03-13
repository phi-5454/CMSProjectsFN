#ifndef TProjectParameters_h
#define TProjectParameters_h

#include <iostream>
#include <fstream>
#include <sstream>

#include "TAnalysisObject.h"
#include "TBeamOptics.h"
#include "TRPAlignment.h"
#include "TAcceptance.h"

class TAnalysisCut2D ;

class TMinuitFitParameter : public TAnalysisObject
{
	public:

	int parameter_number ;
	string cut_name ;
	string parameter_type ;
	string unit ;
	bool fixed ;
	double start_value,		// from prj file
	lower_limit, upper_limit, step_size, parabolic_error, negative_error, positive_error ;
	string displayed_name ;

	TAnalysisCut2D *analysis_cut ;	// four parameters per cut p1x p1y p2x p2y

	TMinuitFitParameter(string, string) ;
} ;

TMinuitFitParameter::TMinuitFitParameter(string name, string id) : TAnalysisObject(name, id), analysis_cut(NULL)
{
	parameter_number = 0 ;
	cut_name = "" ;
	parameter_type = "" ;
	unit = "" ;
	fixed = false ;

	start_value = 0 ;
	lower_limit = 0 ;
	upper_limit = 0 ;
	step_size = 0 ;
	parabolic_error = 0 ;
	negative_error = 0 ;
	positive_error = 0 ;

	displayed_name = "" ;
} ;

typedef vector<TMinuitFitParameter *> vector_of_minuit_fit_parameters_type ;

class TLogFile : TAnalysisObject
{
	private:
	ofstream *Plogfile ;

	public:

	TLogFile(string, string, string) ;

	ofstream *GetPLogFile() { return Plogfile ; } ;
} ;

TLogFile::TLogFile(string name, string id, string log_file_name_with_path) : TAnalysisObject(name, id)
{
	if (std::ifstream(log_file_name_with_path.c_str()))
	{
		cout << "Error: TLogFile::TLogFile: Log file already exists ! " << log_file_name_with_path << endl;
		exit(1) ;
	}

	Plogfile = new ofstream ;

   	Plogfile->open(log_file_name_with_path.c_str(), ios::out);	
} ;

typedef map <string , string> column_type ;
typedef map< string, column_type > table_type ;

class TLatexTable : TAnalysisObject
{
	private:

	table_type table ;

	public:
	TLatexTable(string, string) ;

	void SetElement(string, string, string, bool) ;

	void PrintToLatexFile(ofstream *) ;
} ;

TLatexTable::TLatexTable(string name, string id) : TAnalysisObject(name, id)
{
}

void TLatexTable::SetElement(string row, string column, string value, bool bold)
{
	string left_string = "" ;
	string right_string = "" ;

	if(bold)
	{
		left_string = " { \\bf\\color{red} " ;
		right_string = " } " ;
	}

	table[row][column] = left_string + value + right_string ; 
} 

void TLatexTable::PrintToLatexFile(ofstream *latexfile)
{
	table_type::iterator it = table.begin() ;

	if(table.size() == 0)
	{
		(*latexfile) << "{\\bf\\color{red} Empty table !}" << endl ;
		return ;
	}

	int number_of_columns = ((int)(table.begin()->second.size())) + 1;
	// cout << "number_of_columns: " << number_of_columns << endl ;

	(*latexfile) << "{\\scriptsize\\begin{tabular}{" ;
	for(int i = 0 ; i < (number_of_columns ); ++i) (*latexfile) << " | c " ;
	(*latexfile) << " | } \\hline " << endl ;

	bool first_column = true ;

	for(table_type::iterator it = table.begin() ; it != table.end() ; ++it)
	{
		string name_of_row = (it->first) ;

		if(first_column)
		{
			name_of_row = "" ;
			first_column = false ;
		}

		std::replace( name_of_row.begin(), name_of_row.end(), '_', ' ') ;
		
		(*latexfile) << "	" << name_of_row ;

		for(column_type::iterator it2 = it->second.begin() ; it2 != it->second.end() ; ++it2)
		{
			(*latexfile) << " & " << it2->first ;
		}

		(*latexfile) << " \\\\ \\hline" << endl ;

		break ;

	}

	for(table_type::iterator it = table.begin() ; it != table.end() ; ++it)
	{
		string name_of_row = (it->first) ;
		std::replace( name_of_row.begin(), name_of_row.end(), '_', ' ') ;
		
		(*latexfile) << "	" << name_of_row ;

		for(column_type::iterator it2 = it->second.begin() ; it2 != it->second.end() ; ++it2)
		{
			(*latexfile) << " & " << it2->second ;
		}

		(*latexfile) << " \\\\ \\hline" << endl ;
	}

	(*latexfile) << "\\end{tabular}}" << endl ;
}

typedef TLatexTable *PTLatexTable ;
typedef map<string, PTLatexTable> LatexTables_type ;

class TLatexFile : TAnalysisObject
{
	private:

	ofstream *Platexfile ;

	LatexTables_type *PLatexTables ;

	public:

	TLatexFile(string, string, string) ;

	ofstream *GetPLatexFile() { return Platexfile ; } ;

	void BeginFigure() ;
	void EndFigure() ;
	void IncludeGraphics(string, double = 1.0, bool = false) ;
	void NewLine() { (*Platexfile) << "\\newline" << endl ; } ;

	void SetElementInTable(string table_name, string row, string column, string value, bool bold=false) { (*PLatexTables)[table_name]->SetElement(row, column, value, bold) ; } ;
	void PrintTable(string table_name) { (*PLatexTables)[table_name]->PrintToLatexFile(Platexfile) ; } ;
	void PrintTables() ;
	void PrintTextWithUnderScore(string) ;
} ;

void TLatexFile::BeginFigure()
{
	(*Platexfile) 	<< "\\begin{figure}[H]" << endl ;
	(*Platexfile) 	<< "	\\flushleft" << endl ;
}

void TLatexFile::IncludeGraphics(string filename, double width, bool new_line)
{
	(*Platexfile)	<< "	\\includegraphics[width=" << width << "\\textwidth]{" << filename << "}" << endl ;
	if(new_line) NewLine() ;
}

void TLatexFile::EndFigure()
{
	(*Platexfile) 	<< "\\end{figure}" << endl ;
}

void TLatexFile::PrintTables()
{
	(*Platexfile)	<< "\\section{Summary tables}" << endl ;

	for(LatexTables_type::iterator it = PLatexTables->begin() ; it != PLatexTables->end() ; ++it) it->second->PrintToLatexFile(Platexfile) ;
}

TLatexFile::TLatexFile(string name, string id, string latex_file_name_with_path) : TAnalysisObject(name, id)
{
	Platexfile = new ofstream ;
   	Platexfile->open(latex_file_name_with_path.c_str(), ios::out);	

	if((Platexfile == NULL) || (!Platexfile->is_open()))
	{
                cout << "Error TLatexFile::TLatexFile: latexfile cannot be created !: " << latex_file_name_with_path << endl ;
                exit(1) ;
	}

	(*Platexfile)	<< "\\documentclass{article}" << endl << endl ;

	(*Platexfile)	<< "\\usepackage{graphicx}" << endl ;
	(*Platexfile)	<< "\\usepackage{lscape}" << endl ;	
	(*Platexfile)	<< "\\usepackage{float}" << endl << endl ;
	(*Platexfile)	<< "\\usepackage[margin=0.1in]{geometry}" << endl << endl ;
	(*Platexfile)	<< "\\usepackage[colorlinks=true,linkcolor=blue]{hyperref}" << endl << endl ;
	(*Platexfile)	<< "\\usepackage{color}" << endl << endl ;

	(*Platexfile)	<< "\\begin{document}" << endl ;
	(*Platexfile)	<< "\\begin{landscape}" << endl ;

	(*Platexfile)	<< "\\tableofcontents" << endl ;
	(*Platexfile)	<< "\\newpage" << endl ;	

	PLatexTables = new LatexTables_type ;

	TLatexTable *SummaryOfCuts = new TLatexTable("SummaryOfCuts","SummaryOfCuts") ;
	(*PLatexTables)["SummaryOfCuts"] = SummaryOfCuts ;
} ;

void TLatexFile::PrintTextWithUnderScore(string text)
{
	for(int i = 0 ; i < text.length() ; ++i)
	{
		string character = text.substr(i,1) ;

		if(character.compare("_") == 0) character = "\\" + character ;

		(*Platexfile) << character ;
	}
		(*Platexfile) << endl ;
}

class TProjectParameters : TAnalysisObject
{
	TLogFile *logfile ;
	TLatexFile *latexfile ;

	typedef map<string, TProjectParameters *> map_from_name_to_projectparameters_type ;

	map_from_string_to_double_type map_from_parameter_to_value ;
	map_from_string_to_string_type map_from_setting_to_value ;

	map_from_string_to_doubles_type map_from_parameter_to_values ;
	map_from_string_to_strings_type map_from_setting_to_values ;
	
	map_from_name_to_projectparameters_type map_from_name_to_projectparameters ;

	public:

	TProjectParameters(string, string, string) ;
	TProjectParameters(string, string, ifstream &) ;

	TBeamOptics *BeamOptics_Beam_1, *BeamOptics_Beam_2 ;
	TRPAlignment *RPAlignment ;

	void SetBeam_1_Optics(TBeamOptics *aBeamOptics_Beam_1) 	{ BeamOptics_Beam_1 = aBeamOptics_Beam_1 ; } ;
	void SetBeam_2_Optics(TBeamOptics *aBeamOptics_Beam_2) 	{ BeamOptics_Beam_2 = aBeamOptics_Beam_2 ; } ;
	void SetRPAlignment(TRPAlignment *aRPAlignment)		{ RPAlignment = aRPAlignment ; } ;

	TBeamOptics *GetBeam_1_Optics() { return BeamOptics_Beam_1 ; } ;
	TBeamOptics *GetBeam_2_Optics() { return BeamOptics_Beam_2 ; } ;
	TRPAlignment *GetRPAlignment() { return RPAlignment ; } ;
	
	TAcceptance *preliminary_acceptance_beam_1 ;

	TAcceptance *Get_preliminary_acceptance_beam_1() ;
	void Set_preliminary_acceptance_beam_1(TAcceptance *) ;

	bool IsParameterDefined(string) ;
	bool IsBlockDefined(string) ;
	bool IsSettingDefined(string) ;
	bool IsSettingsDefined(string) ;

	double GetParameterValue(string) ;
	TProjectParameters *GetBlock(string) ;
	string GetSettingValue(string) ;
	vector<double> *GetParametersVector(string) ;
	vector<string> *GetSettingsVector(string) ;

	map_from_string_to_double_type *GetParameterValueMap() { return &map_from_parameter_to_value ; } ;
	map_from_string_to_string_type *GetSettingValueMap()   { return &map_from_setting_to_value ; } ;

	map_from_string_to_doubles_type *GetParametersValueMap() { return &map_from_parameter_to_values ; } ;
	map_from_string_to_strings_type *GetSettingsValueMap()   { return &map_from_setting_to_values ; } ;

	void GetBlockNamesWithSetting(string, string, vector<string> *) ;
	void GetBlockNamesWithSetting(string) ;

	virtual void Print() ;

	void OpenAndLoadProjectFile(string);
	int Load(ifstream &, string) ;
	void Import(TProjectParameters *) ;

	ofstream *GetPLogFile() { return logfile->GetPLogFile() ; } ;
	ofstream *GetPLatexFile() { return latexfile->GetPLatexFile() ; } ;
	TLatexFile *GetLatexFile() { return latexfile ; } ;

	void WriteFigure(string) ;
} ;

TProjectParameters::TProjectParameters(string name, string id, string config_or_project_filename) : TAnalysisObject(name, id), BeamOptics_Beam_1(NULL), BeamOptics_Beam_2(NULL), preliminary_acceptance_beam_1(NULL), logfile(NULL), latexfile(NULL), RPAlignment(NULL)
{
	size_t found = config_or_project_filename.find(".prj") ;
	
	if(found == std::string::npos)
	{
                cout << "Error TProjectParameters::TProjectParameters: filename has to end with .prj !" << endl ;
                exit(1) ;
	}

	size_t pos = config_or_project_filename.find_last_of("/") + 1;
	string filename_without_path = config_or_project_filename.substr (pos);  

	int length_of_filename = filename_without_path.length() ;
	string filename_without_extension = filename_without_path.substr(0, length_of_filename - 4) ;

	OpenAndLoadProjectFile(config_or_project_filename) ;

	// Log file

	string output_directory = GetSettingValue("Output_directory") ;

	logfile = new TLogFile("Logfile", "Logfile", output_directory + "/" + filename_without_extension + "_analysis.log") ;

	if(logfile == NULL)
	{
                cout << "Error TProjectParameters::TProjectParameters: logfile cannot be created !" << endl ;
                exit(1) ;
	}

	
	latexfile = new TLatexFile("LatexFile", "LatexFile", output_directory + "/" + filename_without_extension + ".tex") ;

	if(latexfile == NULL)
	{
                cout << "Error TProjectParameters::TProjectParameters: latexfile cannot be created !" << endl ;
                exit(1) ;
	}
}

void TProjectParameters::GetBlockNamesWithSetting(string setting, string value, vector<string> *block_names)
{
	for(map_from_name_to_projectparameters_type::iterator it = map_from_name_to_projectparameters.begin() ; it != map_from_name_to_projectparameters.end() ; ++it)
	{
		if(it->second->IsSettingsDefined(setting))
		{
			vector<string> *setting_vector = it->second->GetSettingsVector(setting) ;
			
			for(vector<string>::iterator setting_vector_it = setting_vector->begin() ; setting_vector_it != setting_vector->end() ; ++setting_vector_it)
			{
				if(setting_vector_it->compare(value) == 0)
				{
					block_names->push_back(it->first) ;
					// cout << "   Dependency: " << setting << " " << value << "  " << it->first << endl ;
				}
			}
		}
	}
}

void TProjectParameters::GetBlockNamesWithSetting(string setting)
{
	for(map_from_name_to_projectparameters_type::iterator it = map_from_name_to_projectparameters.begin() ; it != map_from_name_to_projectparameters.end() ; ++it)
	{
		if(it->second->IsSettingsDefined(setting))
		{
			vector<string> *setting_vector = it->second->GetSettingsVector(setting) ;

			for(vector<string>::iterator setting_vector_it = setting_vector->begin() ; setting_vector_it != setting_vector->end() ; ++setting_vector_it)
			{
				if(!IsBlockDefined((*setting_vector_it) + "_variable_block"))
				{
			                cout << "Error: TProjectParameters::GetBlockNamesWithSetting: the variable is not defined: " << (*setting_vector_it) << endl ;
			                exit(1) ;
				}
				else if(GetBlock((*setting_vector_it) + "_variable_block") == it->second)
				{
			                cout << "Error: TProjectParameters::GetBlockNamesWithSetting: the variable depends on itself! " << (*setting_vector_it) << endl ;
			                exit(1) ;
				}
			}

		}
	}
}


void TProjectParameters::WriteFigure(string eps_name)
{
	(*latexfile->GetPLatexFile()) 	<< "\\begin{figure}[H]" << endl ;
	(*latexfile->GetPLatexFile()) 	<< "	\\centering" << endl ;
	(*latexfile->GetPLatexFile())	<< "		\\includegraphics[width=1.0\\textwidth]{" << eps_name << "}" << endl ;
	(*latexfile->GetPLatexFile()) 	<< "\\end{figure}" << endl << endl ;
}

void TProjectParameters::OpenAndLoadProjectFile(string config_or_project_filename)
{
        ifstream project_file ;

        project_file.open(config_or_project_filename.c_str(), ifstream::in) ;

	cout << config_or_project_filename << endl ;

        if(project_file.fail())
        {
                cout << "Cannot open configuration file! " << config_or_project_filename << endl ;
                exit(GetErrorCode("ConfigOrProjectFileDoesNotExist")) ;
        }

	string block_name = "" ;

	/*
        string keyword = "" ;

        while(project_file >> keyword)
	{
		if(keyword.compare("<begin>") == 0)
		{
			project_file >> block_name ;

			break ;
		}
		else
		{
			cout << "The block has to start with <begin> keyword !" << endl ;
			exit(1) ;
		}
	}*/

	Load(project_file, block_name) ;

	project_file.close() ;
}

TProjectParameters::TProjectParameters(string name, string id, ifstream &project_file) : TAnalysisObject(name, id), BeamOptics_Beam_1(NULL), BeamOptics_Beam_2(NULL), preliminary_acceptance_beam_1(NULL), logfile(NULL), latexfile(NULL), RPAlignment(NULL)
{
	Load(project_file, name) ;
}

bool TProjectParameters::IsParameterDefined(string parameter_name)
{
	map_from_string_to_double_type::iterator it ;

	it = map_from_parameter_to_value.find(parameter_name) ;

	return (it != map_from_parameter_to_value.end()) ;
}

bool TProjectParameters::IsSettingDefined(string setting_name)
{
	map_from_string_to_string_type::iterator it ;

	it = map_from_setting_to_value.find(setting_name) ;

	return (it != map_from_setting_to_value.end()) ;
}

bool TProjectParameters::IsSettingsDefined(string setting_name)
{
	map_from_string_to_strings_type::iterator it ;

	it = map_from_setting_to_values.find(setting_name) ;

	return (it != map_from_setting_to_values.end()) ;
}

bool TProjectParameters::IsBlockDefined(string block_name)
{
	map_from_name_to_projectparameters_type::iterator it ;

	it = map_from_name_to_projectparameters.find(block_name) ;

	return (it != map_from_name_to_projectparameters.end()) ;
}

TProjectParameters *TProjectParameters::GetBlock(string block_name)
{
	if(IsBlockDefined(block_name)) return map_from_name_to_projectparameters[block_name] ;
	else
	{
                cout << "The requested block is not defined : " << block_name << " !" <<  endl ;
                exit(1) ;
	}

}

double TProjectParameters::GetParameterValue(string parameter_name)
{
	if (IsParameterDefined(parameter_name)) return map_from_parameter_to_value[parameter_name] ;
	else
	{
                cout << "The requested parameter is not defined : " << parameter_name << " !" <<  endl ;
                exit(1) ;
	}
}

void TProjectParameters::Set_preliminary_acceptance_beam_1(TAcceptance *a_preliminary_acceptance_beam_1)
{
	preliminary_acceptance_beam_1 = a_preliminary_acceptance_beam_1 ;
}

TAcceptance *TProjectParameters::Get_preliminary_acceptance_beam_1()
{
	return preliminary_acceptance_beam_1 ;
}

string TProjectParameters::GetSettingValue(string setting_name)
{
	map_from_string_to_string_type::iterator it ;

	it = map_from_setting_to_value.find(setting_name) ;

	if (it != map_from_setting_to_value.end()) return map_from_setting_to_value[setting_name] ;
	else
	{
                cout << "The requested setting is not defined ! " << setting_name << endl ;
                exit(1) ;
	}
}

vector<string> *TProjectParameters::GetSettingsVector(string setting_name)
{
	map_from_string_to_strings_type::iterator it ;

	it = map_from_setting_to_values.find(setting_name) ;

	if(it != map_from_setting_to_values.end()) return map_from_setting_to_values[setting_name] ;
	else
	{
                cout << "The requested setting is not defined ! " << setting_name << endl ;
                exit(1) ;
	}
}

vector<double> *TProjectParameters::GetParametersVector(string parameter_name)
{
	map_from_string_to_doubles_type::iterator it ;

	it = map_from_parameter_to_values.find(parameter_name) ;

	if(it != map_from_parameter_to_values.end()) return map_from_parameter_to_values[parameter_name] ;
	else
	{
                cout << "The requested parameter is not defined ! : " << parameter_name << endl ;
                exit(1) ;
	}
}


void TProjectParameters::Print()
{
	cout << endl ;
	cout << "Parameter names and values:" << endl ;
	cout << "---------------------------" << endl << endl ;

	if(latexfile != NULL) latexfile->PrintTextWithUnderScore("\\section{Parameters}") ;

	for (map_from_string_to_double_type::iterator it = map_from_parameter_to_value.begin() ; it != map_from_parameter_to_value.end(); ++it)
	{
		cout << it->first << " -> " << it->second << endl ;

		if(latexfile != NULL)
		{
			stringstream ss ;

			ss << it->second ;

			latexfile->PrintTextWithUnderScore("\\subsection{" + it->first + "}") ;
			latexfile->PrintTextWithUnderScore(ss.str() + " \\newline") ;
		}
	}

	cout << endl ;
	cout << "Setting names and values:" << endl ;
	cout << "---------------------------" << endl << endl ;

	if(latexfile != NULL) latexfile->PrintTextWithUnderScore("\\section{Settings}") ;

	for (map_from_string_to_string_type::iterator it = map_from_setting_to_value.begin() ; it != map_from_setting_to_value.end(); ++it)
	{
		string text = it->first + " -> " + it->second ;
		cout << text << endl ;

		if(latexfile != NULL)
		{
			latexfile->PrintTextWithUnderScore("\\subsection{" + it->first + "}") ;
			latexfile->PrintTextWithUnderScore(it->second + " \\newline") ;
		}
	}

	cout << "Parameter names and values:" << endl ;
	cout << "---------------------------" << endl << endl ;

	if(latexfile != NULL) latexfile->PrintTextWithUnderScore("\\section{Parameter vectors}") ;

	for (map_from_string_to_doubles_type::iterator it = map_from_parameter_to_values.begin() ; it != map_from_parameter_to_values.end(); ++it)
	{
		cout << it->first << " -> " << it->second << endl ;

		if(latexfile != NULL)
		{
			for(int i = 0 ; i < it->second->size() ; ++i)
			{
				stringstream ss ;

				ss << (*(it->second))[i] ;

				latexfile->PrintTextWithUnderScore("\\subsection{" + it->first + "}") ;
				latexfile->PrintTextWithUnderScore(ss.str() + " \\newline") ;
			}
		}
	}

	cout << endl ;
	cout << "Setting names and values:" << endl ;
	cout << "---------------------------" << endl << endl ;

	if(latexfile != NULL) latexfile->PrintTextWithUnderScore("\\section{Settings}") ;

	for (map_from_string_to_string_type::iterator it = map_from_setting_to_value.begin() ; it != map_from_setting_to_value.end(); ++it)
	{
		string text = it->first + " -> " + it->second ;
		cout << text << endl ;

		if(latexfile != NULL)
		{
			latexfile->PrintTextWithUnderScore("\\subsection{" + it->first + "}") ;
			latexfile->PrintTextWithUnderScore(it->second + " \\newline") ;
		}
	}
	
	cout << endl ;
}

void TProjectParameters::Import(TProjectParameters *ProjectParametersToImport)
{
	if(ProjectParametersToImport->GetParametersValueMap()->size() != 0)
	{
		cout << "Error: TProjectParameters::Import: the included project file contains parameter maps, which cannot be imported !" << endl ;
		exit(1) ;
	}

	if(ProjectParametersToImport->GetSettingsValueMap()->size() != 0)
	{
		cout << "Error: TProjectParameters::Import: the included project file contains setting maps, which cannot be imported !" << endl ;
		exit(1) ;
	}

	for (map_from_string_to_double_type::iterator it = (ProjectParametersToImport->GetParameterValueMap()->begin()) ; it != (ProjectParametersToImport->GetParameterValueMap()->end()); ++it)
	{
		map_from_parameter_to_value[it->first] = it->second ;
	}

	for(map_from_string_to_string_type::iterator it2 = (ProjectParametersToImport->GetSettingValueMap()->begin()) ; it2 != (ProjectParametersToImport->GetSettingValueMap()->end()); ++it2)
	{
		map_from_setting_to_value[it2->first] = it2->second ;
	}

}

int TProjectParameters::Load(ifstream &project_file, string block_name)
{
        string keyword ;
	bool end_found = false ;
	bool reading_in_if_setting_part	= false ;

        while( project_file >> keyword)
        {
		if((keyword.compare("<comment>") == 0) || (keyword.compare("<section>") == 0) || (keyword.compare("<subsection>") == 0))
		{	
			char buffer[4000] ;
			project_file.getline(buffer,4000) ;

		} else if(keyword.compare("<begin>") == 0)
		{
			string new_block_name ;
			project_file >> new_block_name ;

			if(IsBlockDefined(new_block_name))
			{
				map_from_name_to_projectparameters[new_block_name]->Load(project_file, new_block_name) ;
			}
			else
			{
				TProjectParameters *SubProjectParameter = new TProjectParameters(new_block_name, new_block_name, project_file) ;
			
				map_from_name_to_projectparameters[new_block_name] = SubProjectParameter ;
			}		
		} else if(keyword.compare("<end>") == 0)
		{
			string end_of_block_name ;

			project_file >> end_of_block_name ;
			
			if(block_name.compare(end_of_block_name) != 0)
			{
				cout << "End of block name does not agree with begin block name ! \"" << block_name << "\" not equals with \"" << end_of_block_name << "\"" << endl ;
				exit(1) ;
			}
			
			end_found = true ;
			return 0 ;

		} else if((keyword.compare("<parameter>") == 0) || (keyword.compare("<update_parameter>") == 0))
		{
			bool update = false ;

			if(keyword.compare("<update_parameter>") == 0) update = true ;

			string parameter_name ;
			double value ;
			
                        project_file >> parameter_name ;
                        project_file >> value ;	
			
			if((IsParameterDefined(parameter_name)) && (!update))
			{
				cout << "Error in TProjectParameters::Load" << " The parameter in block \"" << name << "\" is already defined: " << parameter_name << endl ;
				exit(1);
			}

			if((!IsParameterDefined(parameter_name)) && (update))
			{
				cout << "Error in TProjectParameters::Load" << " The parameter in block \"" << name << "\" is not defined: " << parameter_name << endl ;
				exit(1);
			}
			
			if(parameter_name.compare("MaxNumberOfEventsToAnalyze") == 0.0) 
			{
				cout << "Remove MaxNumberOfEventsToAnalyze at the end !" << endl ;
				cout << "Remove MaxNumberOfEventsToAnalyze at the end !" << endl ;
				cout << "Remove MaxNumberOfEventsToAnalyze at the end !" << endl ;
				cout << "Remove MaxNumberOfEventsToAnalyze at the end !" << endl ;
				cout << "Remove MaxNumberOfEventsToAnalyze at the end !" << endl ;
			}
			
			map_from_parameter_to_value[parameter_name] = value ;				

		} else if((keyword.compare("<setting>") == 0) || (keyword.compare("<update_setting>") == 0))
		{
			bool update = false ;

			if(keyword.compare("<update_setting>") == 0) update = true ;

			std::string setting_name ;	
			std::string value ;
		
			project_file >> setting_name ;
			project_file >> value ;

			if(IsSettingDefined(setting_name) && (!update))
			{
				cout << "Error in TProjectParameters::Load" << " The setting in block \"" << name << "\" is already defined: " << setting_name << endl ;
				exit(1);
			}

			if(!IsSettingDefined(setting_name) && (update))
			{
				cout << "Error in TProjectParameters::Load" << " The setting in block \"" << name << "\" is not defined: " << setting_name << endl ;
				exit(1);
			}

			if(value.compare("not") == 0)
			{
				cout << "Error in TProjectParameters::Load: a setting cannot take the value not, which is a reserved word for boolean operations!" ;
				exit(1);
			}

			map_from_setting_to_value[setting_name] = value ;

		} else if(keyword.compare("<setting_plus_string>") == 0)
		{
			std::string setting_name ;	
		
			project_file >> setting_name ;

			if(!IsSettingDefined(setting_name))
			{
				cout << "Error in TProjectParameters::Load" << " The setting in block \"" << name << "\" is not defined: " << setting_name << endl ;
				exit(1);
			}

			std::string value ;
			project_file >> value ;

			map_from_setting_to_value[setting_name] = map_from_setting_to_value[setting_name] + value ;
			cout << "The results of setting plus: " << map_from_setting_to_value[setting_name] << endl ;
		} else if(keyword.compare("<include>") == 0)
		{
			string include_file_name = "" ;

			project_file >> include_file_name ;

			string install_directory_of_code = "" ;
			
			if(IsSettingDefined(TConstants::SETTING_INSTALLDIRECTORYOFCODE)) install_directory_of_code = GetSettingValue(TConstants::SETTING_INSTALLDIRECTORYOFCODE) + "/" ;
			
			OpenAndLoadProjectFile(install_directory_of_code + include_file_name) ;
			
		} else if(keyword.compare("<nsetting>") == 0)
                {
		
                        string setting_name = "" ;
			string value = "" ;

                        project_file >> setting_name ;
                        project_file >> value ;

			if(value.compare("not") == 0)
			{
				cout << "Error in TProjectParameters::Load: a setting cannot take the value not, which is a reserved word for boolean operations!" ;
				exit(1);
			}

			// Test whether the vector exist or not. If not, create it.

			map_from_string_to_strings_type::iterator it ;
			it = map_from_setting_to_values.find(setting_name) ;
			
			if(it == map_from_setting_to_values.end())
			{
				vector<string> *ptr = new vector<string> ;
				map_from_setting_to_values[setting_name] = ptr ;
			}
		
                        map_from_setting_to_values[setting_name]->push_back(value) ;

                } else if(keyword.compare("<nparameter>") == 0)
                {
		
                        string parameter_name = "" ;
			double value = 0 ;

                        project_file >> parameter_name ;
                        project_file >> value ;

			// Test whether the vector exist or not. If not, create it.

			map_from_string_to_doubles_type::iterator it ;
			it = map_from_parameter_to_values.find(parameter_name) ;
			
			if(it == map_from_parameter_to_values.end())
			{
				vector<double> *ptr = new vector<double> ;
				map_from_parameter_to_values[parameter_name] = ptr ;
			}
		
                        map_from_parameter_to_values[parameter_name]->push_back(value) ;

                }else if(keyword.compare("<begin_if_setting>") == 0)
		{
			cout << "Begin of if setting part" << endl ;

			if(reading_in_if_setting_part == true)
			{
				cout << "Error in TProjectParameters::Load: there is a <begin_if_setting> without <end_if_setting>!" << endl ;
				exit(1) ;
			}

			string setting = "" ;
			project_file >> setting ;
			cout << "	Name of setting: " << setting << endl ;


			if(IsSettingDefined(setting))
			{
				string setting_required_value = "" ;

				project_file >> setting_required_value ;
				cout << "	setting_required_value: " << setting_required_value << endl ;

				bool negate = false ;

				if(setting_required_value.compare("not") == 0)
				{
					negate = true ;
					project_file >> setting_required_value ;
				}

				string setting_value = GetSettingValue(setting) ;

				if((setting_value.compare(setting_required_value) == 0 && !negate) || (setting_value.compare(setting_required_value) != 0 && negate))
				{
					reading_in_if_setting_part = true ;
				}
				else
				{
					string words_of_if_part = "" ;
					bool end_of_if_found = false ;

					while((!end_of_if_found && project_file >> words_of_if_part) )
					{
						if(words_of_if_part.compare("<end_if_setting>") == 0) end_of_if_found = true ;
						cout << words_of_if_part << endl ;
						reading_in_if_setting_part = false ;
					}
				}

			}
			else
			{
				cout << "Error in TProjectParameters::Load: the setting of the if is not defined !" << endl ;
				exit(1) ;
			}

		}
		else if(keyword.compare("<end_if_setting>") == 0)
		{
			if(reading_in_if_setting_part == false)
			{
				cout << "Error in TProjectParameters::Load: there is an <end_if_setting> keyword without <begin_if_setting> !" << endl ;
				exit(1) ;
			}

			reading_in_if_setting_part = false ;
		}
		else
                {
                        cout << "[TProjectParameters::Load] Undefined keyword: " << keyword << endl ;
                        exit(GetErrorCode("BadKeyword")) ;
                }
	}	

	if(!end_found && block_name.compare("") != 0)
	{
		cout << "Error: TProjectParameters::Load: the <end> keyword to close the actual block \"" << block_name << "\" has not been found !" << endl ;
		exit(1) ;
	}

	if(reading_in_if_setting_part == true)
	{
	       cout << "Error in TProjectParameters::Load: there is a <begin_if_setting> without <end_if_setting>!" << endl ;
	       exit(1) ;
	}
	
	return 0 ;
}


#endif // #ifndef TProjectParameters_h
