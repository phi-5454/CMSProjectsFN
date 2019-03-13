#ifndef TLuminosity_h
#define TLuminosity_h

#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TLine.h"
#include "TArrow.h"

#include "TConstants.h"
#include "TAnalysisObject.h"

#include <iostream>
#include <string> 
#include <sstream> 

#include <time.h>

typedef pair <ULong64_t, vector<double> *> timestamp_bunch_info_type ;
typedef	vector<timestamp_bunch_info_type> vector_of_timestamp_and_bunch_info_type ;

class TLuminosity : public TAnalysisObject
{
	TGraph *total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub ;

	TProjectParameters *ProjectParameters ;
	vector_of_timestamp_and_bunch_info_type vector_of_timestamp_and_bunch_info ;

	public:

	TLuminosity(string, string, TProjectParameters *) ;

	virtual void Print() ;

	unsigned int ConvertTOTEMBunchNumberToCMS(int TOTEMBunchNumber) { return ( TOTEMBunchNumber + 1 ) ;} 
	ULong64_t ConvertUTCTimeToTimeStamp(string) ;

	void CalculateGraph() ;
	double CalculateLuminosityInverseUb(ULong64_t, ULong64_t) ;
	double CalculateLuminosityInverseMb(ULong64_t, ULong64_t) ;

	void Save(map<UInt_t, ULong64_t> *) ;
} ;

TLuminosity::TLuminosity(string name, string id, TProjectParameters *ProjectParameters) :  TAnalysisObject(name, id), total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub(NULL), ProjectParameters(ProjectParameters)
{
	ifstream CMS_luminosity_file ;

	CMS_luminosity_file.open(ProjectParameters->GetSettingValue("CMS_luminosity_file_name").c_str()) ;
	
	if(!CMS_luminosity_file.fail())
	{
		string line = "" ;

		while(getline(CMS_luminosity_file, line))
		{
			// cout << line << endl ;
			if(line.substr(0,62).compare("#run:fill,ls,UTCTime,delivered(/ub),recorded(/ub),\"[bx,Hz/ub]\"") == 0) break ;
		}

		string CMS_run_number, lumi_section, UTC_time, delivered_inverse_ub_string, recorded_inverse_ub_string ;
		
		while(getline(CMS_luminosity_file, line))
		{
			std::stringstream iss(line);

			getline(iss, CMS_run_number, ',') ;
			getline(iss, lumi_section, ',') ;
			getline(iss, UTC_time, ',') ;
			
			getline(iss, delivered_inverse_ub_string, ',') ;
			getline(iss, recorded_inverse_ub_string, ',') ;

			// double delivered_inverse_ub = atof(delivered_inverse_ub_string.c_str()) ;
			// double recorded_inverse_ub = atof(recorded_inverse_ub_string.c_str()) ;

			// cout << "CMS_run_number:" << CMS_run_number << "  lumi_section:" << lumi_section << "  UTC_time:" << UTC_time << "  delivered_inverse_ub_string:" << delivered_inverse_ub_string << "  recorded_inverse_ub_string:" << recorded_inverse_ub_string << endl ;

			ULong64_t lumisection_timestamp = ConvertUTCTimeToTimeStamp(UTC_time) ;

			timestamp_bunch_info_type timestamp_bunch_info ;
			timestamp_bunch_info.first = lumisection_timestamp ;
			timestamp_bunch_info.second = new vector<double> ;

			vector_of_timestamp_and_bunch_info.push_back(timestamp_bunch_info) ;

			string bunch_word ;
			// double total_luminosity_of_LHC_colliding_bunches_Hz_inverse_ub = 0 ;
			double total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub_per_lumi_section = 0 ;

			getline(iss, bunch_word, ',') ;

			while(iss.good())
			{
				unsigned int bunch_number = atoi(bunch_word.c_str()) ;

				getline(iss, bunch_word, ',') ;

				if(bunch_number == ConvertTOTEMBunchNumberToCMS((unsigned int)ProjectParameters->GetParameterValue("trigger_data_bunch_num")))
				{
					// cout << "b:"<< bunch_number << " " ;

					double delivered_Hz_inverse_ub_per_bunch = atof(bunch_word.c_str()) ;

					if(vector_of_timestamp_and_bunch_info.back().first != lumisection_timestamp) // Just a check to be sure	
					{
						cout << "TLuminosity::TLuminosity:  ! : vector_of_timestamp_and_bunch_info.back().first != lumisection_timestamp !" << endl ;
						exit(1) ;
					}

					vector_of_timestamp_and_bunch_info.back().second->push_back(delivered_Hz_inverse_ub_per_bunch) ;
				}

				getline(iss, bunch_word, ',') ;
			}
		}
	}
	else
	{
		cout << "TLuminosity::TLuminosity: The CMS_luminosity_file cannot be opened ! : " << ProjectParameters->GetSettingValue("CMS_luminosity_file_name") << endl ;
		exit(1) ;
	}

	CMS_luminosity_file.close() ;

	CalculateGraph() ;
}

void  TLuminosity::CalculateGraph()
{
	Double_t *x = new Double_t[vector_of_timestamp_and_bunch_info.size()] ;
	Double_t *y = new Double_t[vector_of_timestamp_and_bunch_info.size()] ;

	for(int i = 0; i < vector_of_timestamp_and_bunch_info.size(); ++i)
	{
		double total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub = 0 ;

		for(vector<double>::iterator it = vector_of_timestamp_and_bunch_info[i].second->begin() ; it != vector_of_timestamp_and_bunch_info[i].second->end() ; ++it)
			total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub += (*it) ;

		x[i] = vector_of_timestamp_and_bunch_info[i].first ;
		y[i] = total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub ;
	}
	
	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub = new TGraph(vector_of_timestamp_and_bunch_info.size(), x, y) ;
}

double TLuminosity::CalculateLuminosityInverseUb(ULong64_t start_timestamp, ULong64_t end_timestamp)
{
	double start_luminosity_inverse_ub =	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->Eval(start_timestamp) ;
	double end_luminosity_inverse_ub =	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->Eval(end_timestamp) ;

	double elapsed_time_second = (end_timestamp - start_timestamp) ;
	
	return (elapsed_time_second * ((start_luminosity_inverse_ub + end_luminosity_inverse_ub) / 2.0)) ;
}

double TLuminosity::CalculateLuminosityInverseMb(ULong64_t start_timestamp, ULong64_t end_timestamp)
{
	return (TConstants::conversion_factor_from_mbarn_to_ubarn * CalculateLuminosityInverseUb(start_timestamp, end_timestamp)) ;
}

void TLuminosity::Save(map<UInt_t, ULong64_t> *timestamps)
{
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
   
	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->Draw("APL") ;

	for(vector<double>::iterator it = ProjectParameters->GetParametersVector("timeslice_start_end_pairs_second")->begin() ; it != ProjectParameters->GetParametersVector("timeslice_start_end_pairs_second")->end() ; ++it)
	{
		ULong64_t start_timestamp = (ULong64_t)(*it) ;
		++it ;
		ULong64_t end_timestamp = (ULong64_t)(*it) ;

		TArrow *arrow = new TArrow(start_timestamp, 0.01, end_timestamp,0.01, 0.005, "<|>") ;
		arrow->Draw() ;
	}

	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->SetTitle("Total Luminosity of TOTEM triggered bunches over the whole fill") ;

	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->GetYaxis()->SetTitleOffset(1.2) ;
	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->GetXaxis()->SetTitle("Timestamp [s]") ;
	total_delivered_luminosity_of_TOTEM_triggered_bunches_Hz_inverse_ub->GetYaxis()->SetTitle("Luminosity [Hz / #mub]") ;

	// Event information
	
	Double_t *x = new Double_t[timestamps->size()] ;
	Double_t *y = new Double_t[timestamps->size()] ;
	
	int index = 0 ;
	
	for(map<UInt_t, ULong64_t>::iterator it = timestamps->begin() ; it != timestamps->end(); ++it)
	{
		x[index] = it->second ;
		y[index] = 0.005 ;

		// cout << it->second << endl ;

		index++ ;
	}

	TGraph *bunch_information = new TGraph(timestamps->size(), x, y) ;
	
	bunch_information->SetMarkerColor(kRed) ;
	bunch_information->Draw("P same") ;

	string output_directory = ProjectParameters->GetSettingValue("Output_directory") ;

	canvas->SaveAs((output_directory + "/vector_of_timestamp_and_bunch_info.eps").c_str()) ;
	canvas->SaveAs((output_directory + "/vector_of_timestamp_and_bunch_info.root").c_str()) ;
}

ULong64_t TLuminosity::ConvertUTCTimeToTimeStamp(string UTC_time)
{
	tm t ;

	strptime(UTC_time.c_str(), "%m/%d/%y %H:%M:%S", &t) ;
	time_t t_of_day = (mktime(&t) + 3600) ;

	// cout << UTC_time << endl ;
	// cout << "Test it: Seconds since the Epoch: " << (((long) t_of_day)) << endl << endl ;
	
	return (ULong64_t) t_of_day ;
}

void TLuminosity::Print()
{
	cout << "CMS_luminosity_file_name:" << ProjectParameters->GetSettingValue("CMS_luminosity_file_name") << endl << endl ;

}

#endif // #ifndef TLuminosity_h
