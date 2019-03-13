#include <iostream>
#include <vector>
#include <fstream>

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TFile.h>

using namespace std;

#include "stdlib.h"
#include "math.h"
#include <vector>
#include <map>
#include <sstream>

string directory = "/home/fnemes/Documents/Analysis/pp/CTPPS/2017/2017_september_Alignment_run_with_elastics/Optics_estimators/" ;

vector<string> detector_names ;
map<string, TH2D *> map_from_detector_name_to_histogram ;

double transport_matrix_near_vertical_Lx_m_b1 =   4.183490322179082e+00 ;
double transport_matrix_near_vertical_Ly_m_b1 =  1.171586308103988e+01 ;
double transport_matrix_near_vertical_vx_b1 = -3.917024633713078e+00 ;
double transport_matrix_near_vertical_vy_b1 = -3.813611699000242e+00 ;

double transport_matrix_far_vertical_Lx_m_b1 =   2.559531600907209e+00 ;
double transport_matrix_far_vertical_Ly_m_b1 = 1.046076838499243e+01 ;
double transport_matrix_far_vertical_vx_b1 = -4.069747275773810e+00 ;
double transport_matrix_far_vertical_vy_b1 = -4.002548372199880e+00 ;

double determinant_x_m_b1, determinant_y_m_b1 ;
double determinant_x_m_b2, determinant_y_m_b2 ;

TFile *generic_file ;
TTree *Monte_Carlo_ntuple ;

double L_2_F_x, L_2_F_y, L_1_F_x, L_1_F_y, R_1_F_x, R_1_F_y, R_2_F_x, R_2_F_y ;
Bool_t valid_MC = true ;

UInt_t          MC_track_num = 0 ;

ULong64_t       MC_event_info_timestamp = 0 ;
UInt_t          MC_trigger_data_run_num = 0 ;
UInt_t          MC_trigger_data_bunch_num = 0 ;
UInt_t          MC_trigger_data_event_num = 0 ;
UInt_t          MC_trigger_data_trigger_num = 0 ;
UInt_t          MC_trigger_data_input_status_bits = 1 ;


void init()
{
	generic_file = new TFile("generic.root","RECREATE","Generic");

	determinant_x_m_b1 = ((transport_matrix_near_vertical_vx_b1 * transport_matrix_far_vertical_Lx_m_b1) - (transport_matrix_far_vertical_vx_b1 * transport_matrix_near_vertical_Lx_m_b1)) ;
	determinant_y_m_b1 = ((transport_matrix_near_vertical_vy_b1 * transport_matrix_far_vertical_Ly_m_b1) - (transport_matrix_far_vertical_vy_b1 * transport_matrix_near_vertical_Ly_m_b1)) ;

	Monte_Carlo_ntuple = new TTree("TReducedNtuple", "TReducedNtuple") ;

	Monte_Carlo_ntuple->Branch("event_info_timestamp", 	       	&MC_event_info_timestamp,  	   	"event_info_timestamp/l") ;  	   
	Monte_Carlo_ntuple->Branch("trigger_data_run_num", 	       	&MC_trigger_data_run_num,  	   	"trigger_data_run_num/i") ;  	   
	Monte_Carlo_ntuple->Branch("trigger_data_bunch_num", 	       	&MC_trigger_data_bunch_num,	   	"trigger_data_bunch_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_event_num", 	       	&MC_trigger_data_event_num,	   	"trigger_data_event_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_trigger_num",		&MC_trigger_data_trigger_num,	   	"trigger_data_trigger_num/i") ;	   
	Monte_Carlo_ntuple->Branch("trigger_data_input_status_bits",	&MC_trigger_data_input_status_bits,   	"trigger_data_input_status_bits/i") ;
	
	Monte_Carlo_ntuple->Branch("track_left_far_x"	, &L_2_F_x, "track_left_far_x/D") ;
	Monte_Carlo_ntuple->Branch("track_left_far_y"	, &L_2_F_y, "track_left_far_y/D") ;
	Monte_Carlo_ntuple->Branch("track_left_far_valid"	, &valid_MC, "track_left_far_valid/O)") ;

	Monte_Carlo_ntuple->Branch("track_left_near_x"	, &L_1_F_x, "track_left_near_x/D") ;
	Monte_Carlo_ntuple->Branch("track_left_near_y"	, &L_1_F_y, "track_left_near_y/D") ;
	Monte_Carlo_ntuple->Branch("track_left_near_valid"	, &valid_MC, "track_left_near_valid/O") ;

	Monte_Carlo_ntuple->Branch("track_right_far_x"	, &R_2_F_x, "track_right_far_x/D") ;
	Monte_Carlo_ntuple->Branch("track_right_far_y"	, &R_2_F_y, "track_right_far_y/D") ;
	Monte_Carlo_ntuple->Branch("track_right_far_valid"	, &valid_MC, "track_right_far_valid/O") ;

	Monte_Carlo_ntuple->Branch("track_right_near_x"	, &R_1_F_x, "track_right_near_x/D") ;
	Monte_Carlo_ntuple->Branch("track_right_near_y"	, &R_1_F_y, "track_right_near_y/D") ;
	Monte_Carlo_ntuple->Branch("track_right_near_valid"	, &valid_MC, "track_right_near_valid/O") ;

}

double ReconstructXStarMm(double x_n_mm, double x_f_mm, double determinant_mm, double far_vertical_Lx_mm, double near_vertical_Lx_mm)
{
	return ((1/determinant_mm) * ((far_vertical_Lx_mm * (x_n_mm)) - (near_vertical_Lx_mm * (x_f_mm)))) ;
}

bool diagonal_LBRT = false ;
bool diagonal_LTRB = true ;

int main(int argc, char *argv[])
{

	if(diagonal_LBRT && diagonal_LTRB)
	{
		cout << "Error: both diagonals cannot be prepared at the same time !" << endl ;
		exit(1) ;
	}

	init() ;
	
	detector_names.push_back("left_far") ;
	detector_names.push_back("left_near") ;
	detector_names.push_back("right_far") ;
	detector_names.push_back("right_near") ;

	for(vector<string>::iterator it = detector_names.begin() ; it != detector_names.end() ; ++it) map_from_detector_name_to_histogram[*it] = new TH2D((*it + "_x_y").c_str(), it->c_str(), 100, -4, 4, 100, -2, 8) ;
	
	TH2D dy_left("dy_left", "dy_left",    1000,     -20, 20, 1000, -20, 20) ;
	TH2D dy_right("dy_right", "dy_right", 1000, 	-20, 20, 1000, -20, 20) ;

	TH2D dx_left("dx_left", "dx_left",    1000,     -20, 20, 1000, -20, 20) ;
	TH2D dx_right("dx_right", "dx_right", 1000, 	-20, 20, 1000, -20, 20) ;

	TH2D y_left_right_fars("y_left_right_fars", "y_left_right_fars",    1000,     -20, 20, 1000, -20, 20) ;
	TH2D y_left_right_nears("y_left_right_nears", "y_left_right_nears",    1000,     -20, 20, 1000, -20, 20) ;
	
	vector<string> filenames ;

	filenames.push_back("input.txt") ;

	int number_of_elastics = 0 ;

	for(int i = 0 ; i < filenames.size() ; ++i)
	{
		ifstream input_file(filenames[i].c_str()) ;

		string word ;

		while(input_file >> L_2_F_x)
		{
			number_of_elastics++ ;

			input_file >> L_2_F_y ;
			input_file >> L_1_F_x ;
			input_file >> L_1_F_y ;

			input_file >> R_1_F_x ;
			input_file >> R_1_F_y ;
			input_file >> R_2_F_x ;
			input_file >> R_2_F_y ;

			if(diagonal_LBRT && (L_2_F_y > 0.3)) continue ;
			if(diagonal_LTRB && (L_2_F_y < 0.3)) continue ;

			map_from_detector_name_to_histogram["left_far"]->Fill(L_2_F_x, L_2_F_y) ;
			if(fabs(L_1_F_x - 0.603) < 0.25) map_from_detector_name_to_histogram["left_near"]->Fill(L_1_F_x, L_1_F_y) ;
			map_from_detector_name_to_histogram["right_near"]->Fill(R_1_F_x, R_1_F_y) ;
			map_from_detector_name_to_histogram["right_far"]->Fill(R_2_F_x, R_2_F_y) ;

			double dy_l = (L_2_F_y - L_1_F_y) ;
			double dy_r = (R_2_F_y - R_1_F_y) ;

			double dx_l = (L_2_F_x - L_1_F_x) ;
			double dx_r = (R_2_F_x - R_1_F_x) ;

			if(fabs(L_1_F_x - 0.603) < 0.25) dy_left.Fill(L_2_F_y, dy_l) ;
			dy_right.Fill(R_2_F_y, dy_r) ;

			dx_left.Fill(L_2_F_x, dx_l) ;
			dx_right.Fill(R_2_F_x, dx_r) ;

			y_left_right_fars.Fill(L_2_F_y, R_2_F_y) ;
			y_left_right_nears.Fill(L_1_F_y, R_1_F_y) ;
			
			Monte_Carlo_ntuple->Fill() ;
			
			MC_trigger_data_event_num++ ;
			
			// =========================================================
			
			/*
			double x_l_n_mm =
			double x_l_f_mm =

			double x_r_n_mm =
			double x_r_f_mm =

			double y_l_n_mm =
			double y_l_f_mm =

			double y_r_n_mm =
			double y_r_f_mm =
		
			double x_star_left_mm  = ReconstructXStarMm(x_l_n_mm, x_l_f_mm, BeamOptics_Beam_2) ;
			double x_star_right_mm = ReconstructXStarMm(x_r_n_mm, x_r_f_mm, BeamOptics_Beam_1) ;

			double y_star_left_mm  = ReconstructYStarMm(y_l_n_mm, y_l_f_mm, BeamOptics_Beam_2) ;
			double y_star_right_mm = ReconstructYStarMm(y_r_n_mm, y_r_f_mm, BeamOptics_Beam_1) ;*/

		}
	}

	for(vector<string>::iterator it = detector_names.begin() ; it != detector_names.end() ; ++it) map_from_detector_name_to_histogram[*it]->SaveAs((directory + (*it) + ".root").c_str()) ;

	dy_left.SaveAs((directory + "dy_left.root").c_str()) ;
	dy_right.SaveAs((directory + "dy_right.root").c_str()) ;

	dx_left.SaveAs((directory + "dx_left.root").c_str()) ;
	dx_right.SaveAs((directory + "dx_right.root").c_str()) ;

	y_left_right_fars.SaveAs((directory + "y_left_right_fars.root").c_str()) ;
	y_left_right_nears.SaveAs((directory + "y_left_right_nears.root").c_str()) ;

	cout << "Number of elastic candidates:" << number_of_elastics << endl ;

	generic_file->Write();
}             
