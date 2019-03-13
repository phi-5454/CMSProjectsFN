#ifndef TBeamOptics_h
#define TBeamOptics_h

#include "dirent.h"

#include "TMatrixD.h"
#include "TConstants.h"

typedef struct
{
	int position_in_the_file ;
	int row, column ;
} optical_functions_file_to_matrix_type;

class TTransportMatrix 
{
	TMatrixD transport_matrix ;
	double beam_energy_GeV ;
	double deltap_fraction ;
	double horizontal_beam_position_m ;
	
	public:

	TTransportMatrix() : beam_energy_GeV(0.0), deltap_fraction(0.0), horizontal_beam_position_m(0.0) { transport_matrix.ResizeTo(1,5,1,5) ; } ;

	void SetOpticalFunction(int row, int column, double value) { transport_matrix(row, column) = value ; } ;
	void Print() { transport_matrix.Print() ; cout << "Energy [GeV]: " << beam_energy_GeV << endl << "Deltap [fraction]: " << deltap_fraction <<  " Horizontal beam position [m]: " << horizontal_beam_position_m << endl ;  } ;

	double Lx_m() { return transport_matrix(1,2) ; } ;
	double Lx_mm() { return (Lx_m() * TConstants::conversion_factor_from_m_to_mm) ; } ;
	double dLxds() { return transport_matrix(2,2) ; } ;
	double vx() { return transport_matrix(1,1) ; } ;
	double vy() { return transport_matrix(3,3) ; } ;	
	double dvxds() { return transport_matrix(2,1) ; } ;
	double Ly_m() { return transport_matrix(3,4) ; } ;
	double Ly_mm() {return (Ly_m() * TConstants::conversion_factor_from_m_to_mm) ; } ;
	double dLyds() { return transport_matrix(4,4) ; } ;

	TMatrixD GetTransportMatrix() { return transport_matrix ; } ;
	TMatrixD GetTransportMatrixWithoutCoupling() ;

	void SetBeamEnergyGeV(double aenergy_GeV) { beam_energy_GeV = aenergy_GeV ; } ;
	void SetDeltap(double adeltap_fraction) { deltap_fraction = adeltap_fraction ; } ;

	double GetBeamEnergyGeV() { return beam_energy_GeV ; } ;
	double GetDeltap() { return deltap_fraction ; } ;

	void SetHorizontalBeamPositionM(double ahorizontal_beam_position_m) { horizontal_beam_position_m = ahorizontal_beam_position_m ; } ;
	double GetHorizontalBeamPositionM() { return horizontal_beam_position_m ; } ;
} ;

TMatrixD TTransportMatrix::GetTransportMatrixWithoutCoupling()
{
	TMatrixD transport_matrix_without_coupling = transport_matrix ;
	
	transport_matrix_without_coupling(1,3) = 0.0 ;
	transport_matrix_without_coupling(1,4) = 0.0 ;
	transport_matrix_without_coupling(2,3) = 0.0 ;
	transport_matrix_without_coupling(2,4) = 0.0 ;

	transport_matrix_without_coupling(3,1) = 0.0 ;
	transport_matrix_without_coupling(4,1) = 0.0 ;
	transport_matrix_without_coupling(3,2) = 0.0 ;
	transport_matrix_without_coupling(4,2) = 0.0 ;

	return transport_matrix_without_coupling ;
}

class TBeamSizes
{
	public:

	double x_m, y_m ;
} ;

class TBeamElement : TAnalysisObject
{
	double position ;
	TBeamSizes beamsizes ;
	TTransportMatrix transport_matrix ;
	
	TBeamElement(string, string) ;
};

TBeamElement::TBeamElement(string name, string id) : TAnalysisObject(name, id)
{
}
 
typedef map<int, TTransportMatrix *> map_from_xi_times_10000_to_PTTransportMatrix_type ;

class TBeamOptics : TAnalysisObject
{
	double beam_energy_GeV ;
	
	string MADX_label_of_near_RP_unit ;
	string MADX_label_of_far_RP_unit ;

	double distance_from_IP5_near_vertical_m, distance_from_IP5_far_vertical_m ;
	TTransportMatrix transport_matrix_near_vertical, transport_matrix_far_vertical ;
	TBeamSizes beam_sizes_XRPV_A6RL5_B12, beam_sizes_XRPV_B6RL5_B12 ;

	map_from_xi_times_10000_to_PTTransportMatrix_type transport_matrix_near_horizontal_per_xi_fraction ;
	map_from_xi_times_10000_to_PTTransportMatrix_type transport_matrix_far_horizontal_per_xi_fraction ;

	double determinant_x_m, determinant_y_m ;
	
	bool xi_dependent ;
	bool determinant_valid ;

	ofstream *latexfile ;
	
	public:
	
	TBeamOptics(string, string, string, string, string, bool=false) ;
	
	virtual void Print() ;

	void InitOpticalFunctionsFileToMatrix(vector<optical_functions_file_to_matrix_type> &) ;

	void ReadTransportMatrix(string, TTransportMatrix *, TTransportMatrix *) ;
	void ReadTransportMatrices(string) ;
	
	void ReadBeamProperties(string) ;

	TTransportMatrix &Get_transport_matrix_near_vertical() { return transport_matrix_near_vertical ; } ;
	TTransportMatrix &Get_transport_matrix_far_vertical() { return transport_matrix_far_vertical ; } ;

	TTransportMatrix &GetNearestTransportMatrix(double, map_from_xi_times_10000_to_PTTransportMatrix_type *) ;
	TTransportMatrix &Get_transport_matrix_near_horizontal(double xi)	{ return GetNearestTransportMatrix(xi, &transport_matrix_near_horizontal_per_xi_fraction) ; } ;
	TTransportMatrix &Get_transport_matrix_far_horizontal(double xi)	{ return GetNearestTransportMatrix(xi, &transport_matrix_far_horizontal_per_xi_fraction) ; } ;

	TBeamSizes &Get_BeamSizes_near_vertical() { return beam_sizes_XRPV_A6RL5_B12 ; } ;
	TBeamSizes &Get_BeamSizes_far_vertical()  { return beam_sizes_XRPV_B6RL5_B12 ; } ;

	double GetBeamEnergyGeV() { return beam_energy_GeV ; } ;

	double GetDeterminant_x_m() ;
	double GetDeterminant_x_mm() ;

	double GetDeterminant_y_m() ;
	double GetDeterminant_y_mm() ;

	double GetNearFarDistanceM() { return (distance_from_IP5_far_vertical_m - distance_from_IP5_near_vertical_m) ; } ;
	double GetNearFarDistanceMM() { return ((distance_from_IP5_far_vertical_m - distance_from_IP5_near_vertical_m) * TConstants::conversion_factor_from_m_to_mm) ; } ;

	void SetLatexFile(ofstream *global_latexfile) { latexfile = global_latexfile ; }  ;
} ;

TBeamOptics::TBeamOptics(string name, string id, string twiss_file, string MADX_label_of_near_RP_unit, string MADX_label_of_far_RP_unit, bool xi_dependent) : TAnalysisObject(name, id), latexfile(NULL),
	distance_from_IP5_near_vertical_m(0), distance_from_IP5_far_vertical_m(0), 
	MADX_label_of_near_RP_unit(MADX_label_of_near_RP_unit),	MADX_label_of_far_RP_unit(MADX_label_of_far_RP_unit), determinant_x_m(0), determinant_y_m(0), determinant_valid(false), xi_dependent(xi_dependent)
{

	if(xi_dependent)
	{

		ReadTransportMatrices(twiss_file) ;

	}
	else
	{
		ReadBeamProperties(twiss_file) ;
		ReadTransportMatrix(twiss_file, &transport_matrix_near_vertical, &transport_matrix_far_vertical) ;

	}

	determinant_x_m = ((transport_matrix_near_vertical.vx() * transport_matrix_far_vertical.Lx_m()) - (transport_matrix_far_vertical.vx() * transport_matrix_near_vertical.Lx_m())) ;
	determinant_y_m = ((transport_matrix_near_vertical.vy() * transport_matrix_far_vertical.Ly_m()) - (transport_matrix_far_vertical.vy() * transport_matrix_near_vertical.Ly_m())) ;

	determinant_valid = true ;

}

TTransportMatrix &TBeamOptics::GetNearestTransportMatrix(double xi, map_from_xi_times_10000_to_PTTransportMatrix_type *map_from_xi_times_10000_to_PTTransportMatrix)
{
	map_from_xi_times_10000_to_PTTransportMatrix_type::iterator it ;

	int xi_to_index = (-xi) * TConstants::scale_between_xi_and_its_index ;

	for(int i = 0 ; i < TConstants::scale_between_xi_and_its_index ; ++i)
	{
		it = map_from_xi_times_10000_to_PTTransportMatrix->find(xi_to_index + i) ;

		if(it != map_from_xi_times_10000_to_PTTransportMatrix->end()) return (*(it->second)) ;

		it = map_from_xi_times_10000_to_PTTransportMatrix->find(xi_to_index - i) ;

		if(it != map_from_xi_times_10000_to_PTTransportMatrix->end()) return (*(it->second)) ;

	}

	cout << "Error: TTransportMatrix::GetNearestTransportMatrix: there is appropriate transport matrix !" << endl ;
}

double TBeamOptics::GetDeterminant_x_m()
{
	if(determinant_valid) return determinant_x_m ;
	else
	{
		cout << "TBeamOptics::GetDeterminant_x_m(): determinant is not valid !" << endl ;
		exit(1) ;
	}
} 

double TBeamOptics::GetDeterminant_x_mm()
{
	if(determinant_valid) return (GetDeterminant_x_m() * TConstants::conversion_factor_from_m_to_mm) ;
	else
	{
		cout << "TBeamOptics::GetDeterminant_x_mm(): determinant is not valid !" << endl ;
		exit(1) ;
	}
} 

double TBeamOptics::GetDeterminant_y_m()
{
	if(determinant_valid) return determinant_y_m ;
	else
	{
		cout << "TBeamOptics::GetDeterminant_y_m(): determinant is not valid !" << endl ;
		exit(1) ;
	}
} 

double TBeamOptics::GetDeterminant_y_mm()
{
	if(determinant_valid) return (GetDeterminant_y_m() * TConstants::conversion_factor_from_m_to_mm) ;
	else
	{
		cout << "TBeamOptics::GetDeterminant_y_mm(): determinant is not valid !" << endl ;
		exit(1) ;
	}
} 

void TBeamOptics::Print()
{
	if(xi_dependent)
	{
		map_from_xi_times_10000_to_PTTransportMatrix_type::iterator it ;

		for(it = transport_matrix_near_horizontal_per_xi_fraction.begin() ; 	it != transport_matrix_near_horizontal_per_xi_fraction.end() ; 	++it) it->second->Print() ;
		for(it = transport_matrix_far_horizontal_per_xi_fraction.begin() ;	it != transport_matrix_far_horizontal_per_xi_fraction.end() ;	++it) it->second->Print() ;

	}

	cout << "=====================================" << endl ;
	cout << "beam_energy [GeV] = " << beam_energy_GeV << endl ;

	transport_matrix_near_vertical.Print() ;
	transport_matrix_far_vertical.Print() ;

	cout << "determinant x [m] = " << GetDeterminant_x_m() << endl ;
	cout << "determinant y [m] = " << GetDeterminant_y_m() << endl ;

	(*latexfile) << "Beam energy [GeV] = " << beam_energy_GeV << " \\newline" << endl ;

}

void TBeamOptics::InitOpticalFunctionsFileToMatrix(vector<optical_functions_file_to_matrix_type> &optical_functions_file_to_matrix)
{
	optical_functions_file_to_matrix_type aux ;

	for(int row = 1 ; row < 5 ; ++row)
		for(int column = 1 ; column < 5 ; ++column)	
		{
			aux.row = row ;
			aux.column = column ;
			aux.position_in_the_file = 20 + (row-1)*4 + (column-1) ;
			
			optical_functions_file_to_matrix.push_back(aux) ;
		}

	aux.column = 5 ;
	for(int row = 1 ; row < 5 ; ++row)
	{
		aux.row = row ;
		aux.position_in_the_file = 16 + (row - 1) ;
		optical_functions_file_to_matrix.push_back(aux) ;		
	}

}

void TBeamOptics::ReadBeamProperties(string filename)
{
	ifstream beam_optics ;

	beam_optics.open(filename.c_str()) ;
	
	if(!beam_optics.fail())
	{
		string word = "" ;

		while(beam_optics >> word)
		{
			if(word == "ENERGY")
			{
				beam_optics >> word ;
				beam_optics >> beam_energy_GeV ;
			}
		}

	}			
	else
	{
		cout << "The twiss file cannot be opened ! : " << filename << endl ;
		exit(1) ;
	}

	beam_optics.close() ;
}

void TBeamOptics::ReadTransportMatrix(string filename, TTransportMatrix *Ptransport_matrix_near_vertical, TTransportMatrix *Ptransport_matrix_far_vertical)
{

	vector<optical_functions_file_to_matrix_type> optical_functions_file_to_matrix ;
	InitOpticalFunctionsFileToMatrix(optical_functions_file_to_matrix) ;

	const int number_of_words_in_line_of_twiss_file = 38 ;
	
	ifstream beam_optics ;
	
	beam_optics.open(filename.c_str()) ;
	
	string pattern_near_vertical, pattern_far_vertical ;
	
	pattern_near_vertical = "\"" + MADX_label_of_near_RP_unit + "\"" ;
	pattern_far_vertical  = "\"" + MADX_label_of_far_RP_unit + "\"" ;

	bool near_done = false ;
	bool far_done = false ;
	
	string word = "" ;
	
	if(!beam_optics.fail())
	{
		while(beam_optics >> word)
		{
		
			if(word.compare("ENERGY") == 0.0)
			{
				double transport_matrix_beam_energy_GeV ;
				beam_optics >> word ;
				beam_optics >> transport_matrix_beam_energy_GeV ;
				
				Ptransport_matrix_near_vertical->SetBeamEnergyGeV(transport_matrix_beam_energy_GeV) ;
				Ptransport_matrix_far_vertical->SetBeamEnergyGeV(transport_matrix_beam_energy_GeV) ;
			}
			
			if(word.compare("DELTAP") == 0.0)
			{
				double transport_matrix_deltap ;
				beam_optics >> word ;
				beam_optics >> transport_matrix_deltap ;
				
				Ptransport_matrix_near_vertical->SetDeltap(transport_matrix_deltap) ;
				Ptransport_matrix_far_vertical->SetDeltap(transport_matrix_deltap) ;
			}


			if(word == pattern_near_vertical)
			{
				double optical_function_value ;
				
				for(int i = 2 ; i < (number_of_words_in_line_of_twiss_file + 1) ; ++i)
				{
					beam_optics >> optical_function_value ;
					
					// cout << "optical_function_value: " << optical_function_value << "  " << endl ;
					
					for(unsigned int j = 0 ; j < optical_functions_file_to_matrix.size() ; ++j)
						if(optical_functions_file_to_matrix[j].position_in_the_file == i)
							Ptransport_matrix_near_vertical->SetOpticalFunction(optical_functions_file_to_matrix[j].row, optical_functions_file_to_matrix[j].column, optical_function_value) ;

					if(i == 2) distance_from_IP5_near_vertical_m = optical_function_value ;
					if(i == TConstants::twiss_position_in_line_of_horizontal_beam_position_m) Ptransport_matrix_near_vertical->SetHorizontalBeamPositionM(optical_function_value) ;
					if(i == (number_of_words_in_line_of_twiss_file-1)) 	beam_sizes_XRPV_A6RL5_B12.x_m = optical_function_value ;
					if(i == (number_of_words_in_line_of_twiss_file)) 	beam_sizes_XRPV_A6RL5_B12.y_m = optical_function_value ;

				}
				
				Ptransport_matrix_near_vertical->SetOpticalFunction(5,1,0) ;
				Ptransport_matrix_near_vertical->SetOpticalFunction(5,2,0) ;
				Ptransport_matrix_near_vertical->SetOpticalFunction(5,3,0) ;
				Ptransport_matrix_near_vertical->SetOpticalFunction(5,4,0) ;
				Ptransport_matrix_near_vertical->SetOpticalFunction(5,5,1) ;
				
				// cout << "beam_sizes_XRPV_A6RL5_B12.x" << beam_sizes_XRPV_A6RL5_B12.x << endl ;
				// cout << "beam_sizes_XRPV_A6RL5_B12.y" << beam_sizes_XRPV_A6RL5_B12.y << endl ;
				
				near_done = true ;

			}
			
			if(word == pattern_far_vertical) 
			{
				double optical_function_value ;
				
				for(int i = 2 ; i < (number_of_words_in_line_of_twiss_file + 1) ; ++i)
				{
					beam_optics >> optical_function_value ;
					
					// cout << "optical_function_value: " << optical_function_value << "  " << endl ;
					
					for(unsigned int j = 0 ; j < optical_functions_file_to_matrix.size() ; ++j)
						if(optical_functions_file_to_matrix[j].position_in_the_file == i)
							Ptransport_matrix_far_vertical->SetOpticalFunction(optical_functions_file_to_matrix[j].row, optical_functions_file_to_matrix[j].column, optical_function_value) ;

					if(i == 2) distance_from_IP5_far_vertical_m = optical_function_value ;
					if(i == TConstants::twiss_position_in_line_of_horizontal_beam_position_m) Ptransport_matrix_far_vertical->SetHorizontalBeamPositionM(optical_function_value) ;
					if(i == (number_of_words_in_line_of_twiss_file-1)) 	beam_sizes_XRPV_B6RL5_B12.x_m = optical_function_value ;
					if(i == (number_of_words_in_line_of_twiss_file)) 	beam_sizes_XRPV_B6RL5_B12.y_m = optical_function_value ;

				}
				
				Ptransport_matrix_far_vertical->SetOpticalFunction(5,1,0) ;
				Ptransport_matrix_far_vertical->SetOpticalFunction(5,2,0) ;
				Ptransport_matrix_far_vertical->SetOpticalFunction(5,3,0) ;
				Ptransport_matrix_far_vertical->SetOpticalFunction(5,4,0) ;												
				Ptransport_matrix_far_vertical->SetOpticalFunction(5,5,1) ;	
				
				// cout << "beam_sizes_XRPV_B6RL5_B12.x" << beam_sizes_XRPV_B6RL5_B12.x << endl ;
				// cout << "beam_sizes_XRPV_B6RL5_B12.y" << beam_sizes_XRPV_B6RL5_B12.y << endl ;
				
				far_done = true ;

			}
		}
		
		if(!near_done)
		{
			cout << "Error: TBeamOptics::ReadTransportMatrix: The near RP detector (" << pattern_near_vertical <<  ") could not be read in!" << endl ;
			exit(1) ;
		}

		if(!far_done)
		{
			cout << "Error: TBeamOptics::ReadTransportMatrix: The far RP detector (" << pattern_far_vertical <<  ") could not be read in!" << endl ;
			exit(1) ;
		}

	}
	else
	{
		cout << "The twiss file cannot be opened ! : " << filename << endl ;
		exit(1) ;
	}

	beam_optics.close() ;

	if(distance_from_IP5_far_vertical_m < distance_from_IP5_near_vertical_m)
	{
		cout << "Error: TBeamOptics::ReadTransportMatrix: The near distance is larger than the far !" << endl ;
		exit(1) ;
	}

}

void TBeamOptics::ReadTransportMatrices(string directory_name)
{
	DIR *dir ;
	struct dirent *ent ;

	cout << "directory_name" << directory_name << endl ;

	if((dir = opendir(directory_name.c_str())) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			cout << "Loading twiss file: " << ent->d_name << endl ;

			if(strstr(ent->d_name, ".tfs") != NULL)
			{
				string twiss_file = (directory_name + ent->d_name) ;

				TTransportMatrix *Ptransport_matrix_near_horizontal_per_xi_fraction  = new TTransportMatrix ;
				TTransportMatrix *Ptransport_matrix_far_horizontal_per_xi_fraction  = new TTransportMatrix ;
				
				ReadTransportMatrix(twiss_file, Ptransport_matrix_near_horizontal_per_xi_fraction, Ptransport_matrix_far_horizontal_per_xi_fraction) ;
								
				transport_matrix_near_horizontal_per_xi_fraction[Ptransport_matrix_near_horizontal_per_xi_fraction->GetDeltap() * (-1.0) * TConstants::scale_between_xi_and_its_index] = Ptransport_matrix_near_horizontal_per_xi_fraction ;
				transport_matrix_far_horizontal_per_xi_fraction[Ptransport_matrix_far_horizontal_per_xi_fraction->GetDeltap() * (-1.0) * TConstants::scale_between_xi_and_its_index]= Ptransport_matrix_far_horizontal_per_xi_fraction ;

				if(Ptransport_matrix_near_horizontal_per_xi_fraction->GetDeltap() == 0.0)
				{
					transport_matrix_near_vertical = *Ptransport_matrix_near_horizontal_per_xi_fraction ;
					beam_energy_GeV = Ptransport_matrix_near_horizontal_per_xi_fraction->GetBeamEnergyGeV() ;
				}

				if(Ptransport_matrix_far_horizontal_per_xi_fraction->GetDeltap() == 0.0)
				{
					transport_matrix_far_vertical = *Ptransport_matrix_far_horizontal_per_xi_fraction ;
					beam_energy_GeV = Ptransport_matrix_far_horizontal_per_xi_fraction->GetBeamEnergyGeV() ;
				}

			}
		}

		if(transport_matrix_near_horizontal_per_xi_fraction.size() == 0)
		{
			cout << "Error: TBeamOptics::ReadTransportMatrices: the vector transport_matrix_near_horizontal_per_xi_fraction remained empty in " << name << " !" << endl ;
			exit(1) ;
		}


		closedir(dir) ;
	}
	else
	{
		cout << "Error: TBeamOptics::ReadTransportMatrices: the directory does not exist !" << endl ;
		exit(1) ;
	}
	
}


#endif // #ifndef TBeamOptics_h
