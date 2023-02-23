
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include "math.h"

using namespace std;

#include "stdlib.h"

#include "TMath.h"

#include "TF1.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"


#include "TTree.h"
#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TProfile.h"

#include "TRandom3.h"
#include "TMultiGraph.h"

#include <iomanip>

double fku3(double p) { return  3.56575287- (-0.015964108* p) + exp(3.8129127 - (6.37289422*p)) ; } ;
double fkd3(double p) { return  2.09219508- (-0.0255047903* p) + exp(3.19090268 - (5.40106319*p)) ; } ;

double fpiu3(double p) { return  (44752.1802- (28507.4293* atan(p - -924.395828)) + exp(3.01618089 - (11.2923282 * p))) - 0.5 ; } ;
double fpid3(double p) { return  36.9120386- (-23.3399876* atan(p - 15.9443554)) + exp(2.64833668 - (16.6946182 * p)) ; } ;

void print_track_and_dedx_measurements_from_logfile()
{
	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;
	
	TH2D *hist1 = new TH2D("hist1", "hist1", 50, 0, 50, 50, 0, 50) ;
	TH2D *hist2 = new TH2D("hist2", "hist2", 50, 0, 50, 50, 0, 50) ;
	TH2D *hist3 = new TH2D("hist3", "hist3", 50, 0, 50, 50, 0, 50) ;
	TH2D *hist4 = new TH2D("hist4", "hist4", 50, 0, 50,  50, -5, 5) ;
	TH2D *hist5 = new TH2D("hist5", "hist5", 50, 0, 50, 50, 0, 10) ;
	TH2D *hist6 = new TH2D("hist6", "hist6", 50, 0, 50, 50, -5, 5) ;
	TH2D *hist7 = new TH2D("hist7", "hist7", 15, 0, 15, 50, -5, 5) ;
	TH2D *hist8 = new TH2D("hist8", "hist8", 10, -5, 5, 50, -5, 5) ;
	TH2D *hist9 = new TH2D("hist9", "hist9", 50, 0, 50,  50, -5, 5) ;
	TH2D *hist10 = new TH2D("hist10", "hist10", 100, 0, 1e9,  50, -5, 5) ;
	TH2D *hist11 = new TH2D("hist11", "hist11", 100, 0, 1e9,  50, -5, 5) ;

	map<long int, bool> map_of_all_det_ids ;
	map<long int, bool> map_of_det_ids ;
	map<long int, bool> map_of_det_ids_eta ;
	
	for(int i = 0 ; i < 1000 ; ++i)
	{
		stringstream ss ;
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		string filename = directory + "dir_" + prefix + ss.str() + "/log";
		// cout << filename << endl ;
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{
			string word = "" ;
			long int event_number = 0 ;

			while(logfile >> word)
			{
				if(word.compare("complete_printout") == 0) 
				{
					// cout << "found" << endl ;
					
					long int my_event ;
					int trackj ;
					int traje_measurements_size ;
					double momentum ;
					double track_pt  ;
					double track_eta  ;
					double track_phi ;
					int track_charge ;
					int track_pixelHits_size ;
					int track_stripHits_size ;
					int traje_foundHits ;
					int traje_foundPixelHits ;
					int traje_lostHits ;
								
					logfile  >>  word >>   my_event
					>>  word >>   trackj
					>>  word >>   traje_measurements_size
					>>  word  >>  momentum
					>>  word  >>  track_pt 
					>>  word  >>  track_eta 
					>>  word  >>  track_phi
					>>  word  >>  track_charge
					>>  word >>   track_pixelHits_size
					>>  word >>   track_stripHits_size
					>>  word >>   traje_foundHits
					>>  word >>   traje_foundPixelHits
					>>  word >>   traje_lostHits ;
					
					long int detid = 0 ;
					
					logfile >> detid ;
					
					while(detid != -1)
					{
						hist10->Fill(detid, track_eta) ;
						logfile >> detid ;

						map_of_all_det_ids[detid] = true ;
						
						if(traje_measurements_size < 10)
						{
							hist11->Fill(detid, track_eta) ;
							map_of_det_ids[detid] = true ;
							
							if((fabs(track_eta) > 2.6) && (fabs(track_eta) < 2.8))
							{
								map_of_det_ids_eta[detid] = true ;
							}
						}
												
					}
					
					
					hist1->Fill(traje_measurements_size, traje_foundHits) ;
					hist2->Fill(traje_measurements_size, traje_foundPixelHits) ;
					hist3->Fill(traje_measurements_size, traje_lostHits) ;

					hist4->Fill(traje_measurements_size, track_eta) ;
					hist5->Fill(traje_measurements_size, track_phi) ;

					hist6->Fill(traje_foundHits, track_eta) ;
					hist7->Fill(traje_foundPixelHits, track_eta) ;
					hist8->Fill(track_charge, track_eta) ;
					
					if(traje_measurements_size < 10) hist9->Fill(traje_measurements_size, track_eta) ;
					
					// cout << "found " << my_event << endl ;
					
				}
			}

			logfile.close() ;
		}
	}
	
	cout << "mapsize " <<  map_of_det_ids.size() << endl ;
	cout << "mapsize " <<  map_of_all_det_ids.size() << endl ;
	cout << "mapsize " <<  	map_of_det_ids_eta.size() << endl ;
	
	for(map<long int, bool>::iterator it = map_of_det_ids.begin() ; it != map_of_det_ids.end() ; ++it)
	{
		cout << (*it).first << endl ;
	}
	
	hist1->SaveAs("plots/hist1.root") ;
	hist2->SaveAs("plots/hist2.root") ;
	hist3->SaveAs("plots/hist3.root") ;
	hist4->SaveAs("plots/hist4.root") ;
	hist5->SaveAs("plots/hist5.root") ;
	hist6->SaveAs("plots/hist6.root") ;
	hist7->SaveAs("plots/hist7.root") ;
	hist8->SaveAs("plots/hist8.root") ;
	hist9->SaveAs("plots/hist9.root") ;
	hist10->SaveAs("plots/hist10.root") ;
	hist11->SaveAs("plots/hist11.root") ;
}

void test_dedx_and_gain_using_log()
{
	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;


	TH2F *histosTH2F_hdedx4PHImass_Ferenc =  new TH2F("hdedx4PHImass_Ferenc","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
	TH2F *histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr =  new TH2F("histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
	TH2F *histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr =  new TH2F("histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);
	TH2F *histosTH2F_hdedx4PHImass_Ferenc_new_gain =  new TH2F("histosTH2F_hdedx4PHImass_Ferenc_new_gain","dE/dx vs p 4trk", 500, 0., 5., 400, 0., 20.);

	TH2F *momentum_vs_gain =  new TH2F("momentum_vs_gain","momentum_vs_gain", 500, 0., 5., 100, 0., 4.) ;
	TH2F *momentum_vs_gain_corrected =  new TH2F("momentum_vs_gain_corrected","momentum_vs_gain_corrected", 500, 0., 5., 100, 0., 4.) ;

	TH2F *new_test =  new TH2F("new_test","new_test", 500, 0., 5., 400, 0., 20.);

	for(int i = 0 ; i < 1000 ; ++i)
	{
		stringstream ss ;
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		string filename = directory + "dir_" + prefix + ss.str() + "/log";
		// cout << filename << endl ;
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{
			string word = "" ;
			long int event_number = 0 ;
			
			double gain_correction_sum = 0 ;
			int number_of_gains = 0 ;
			
			vector<double> gain_corrections_per_event ;

			while(logfile >> word)
			{

				if(word.compare("testing5") == 0)
				{
					long int detid ;
					unsigned int chipid ;
					double gain_calibration ;
					
					logfile >> detid ;
					logfile >> chipid ;
					logfile >> gain_calibration ;
					
					// cout << gain_calibration << endl ;
					cout << "detid " << hex << detid << endl ;
					
					gain_correction_sum += gain_calibration ;
					++number_of_gains ;
					
					gain_corrections_per_event.push_back(gain_calibration) ;
				}

				if(word.compare("testing6") == 0)
				{
					long int myevent ;
					int trackindex ;
					double momentum ;
					int charge ;
					double dedx ;
				
					logfile >> word ;
					logfile >> myevent ;

					logfile >> word ;
					logfile >> trackindex ;

					logfile >> word ;
					logfile >> momentum ;

					logfile >> word ;
					logfile >> charge ;

					logfile >> word ;
					logfile >> dedx ;
					
					// cout << "." ;
					
					double average_gain_corr = (gain_correction_sum / number_of_gains) ;
					
					histosTH2F_hdedx4PHImass_Ferenc->Fill(momentum, dedx) ;
					momentum_vs_gain->Fill(momentum, average_gain_corr) ;
					if(average_gain_corr > 0.8) histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr->Fill(momentum, dedx) ;
					if(average_gain_corr < 0.8) histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr->Fill(momentum, dedx) ;
					
					double mpvkaon = (fku3(momentum) + fkd3(momentum)) / 2.0 ;

					double new_gain_correction = average_gain_corr ;
					if(average_gain_corr < 0.8) new_gain_correction = average_gain_corr * (mpvkaon / dedx) ;

					momentum_vs_gain_corrected->Fill(momentum, new_gain_correction) ;
					
					double new_dedx = (dedx / average_gain_corr) * new_gain_correction ;
					
					histosTH2F_hdedx4PHImass_Ferenc_new_gain->Fill(momentum, new_dedx) ;
					
					gain_correction_sum = 0 ;
					
					for(int ki = 0 ; ki < gain_corrections_per_event.size() ; ++ki)
					{
						new_test->Fill(momentum, gain_corrections_per_event[ki]) ;
					}
				}
			}
		}
	}
	
	histosTH2F_hdedx4PHImass_Ferenc->SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc.root") ;
	histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr->SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr.root") ;
	histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr->SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr.root") ;
	histosTH2F_hdedx4PHImass_Ferenc_new_gain->SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_new_gain.root") ;
	new_test->SaveAs("plots/dedx_and_gain/new_test.root") ;

	momentum_vs_gain->SaveAs("plots/dedx_and_gain/momentum_vs_gain.root") ;
	momentum_vs_gain_corrected->SaveAs("plots/dedx_and_gain/momentum_vs_gain_corrected.root") ;
	
	TCanvas c ;
	
	gStyle->SetLineScalePS(.3) ;
	c.SetLogz() ;

	histosTH2F_hdedx4PHImass_Ferenc->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc.pdf");

	histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_low_gain_corr.pdf");

	histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_high_gain_corr.pdf");

	histosTH2F_hdedx4PHImass_Ferenc_new_gain->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/histosTH2F_hdedx4PHImass_Ferenc_new_gain.pdf");

	momentum_vs_gain->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/momentum_vs_gain.pdf");

	momentum_vs_gain_corrected->Draw("colz") ;
	c.SaveAs("plots/dedx_and_gain/momentum_vs_gain_corrected.pdf");
}

void test_dedx_per_momentum_and_per_detector_using_log() 
{
	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;

	TH2F *test =  new TH2F("test","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);
	TH2F *test2 =  new TH2F("test2","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);
	TH2F *test3 =  new TH2F("test3","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);

	for(int i = 0 ; i < 1000 ; ++i)
	{
		stringstream ss ;
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		string filename = directory + "dir_" + prefix + ss.str() + "/log";
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{
			string word = "" ;

			while(logfile >> word)
			{
				if(word.compare("Detid:") == 0)
				{
					// cout << "Detid" << endl ;
					
					unsigned int detid = 0 ;
					double momentum = 0 ;
					double dedx = 0 ;
					
					logfile >> detid ;
					
					logfile >> word >> momentum >> word >> dedx ;
					
					if(detid == 303046664)
					{
						test->Fill(momentum, dedx) ;
					}
					else if(detid == 303046664)
					{
						test2->Fill(momentum, dedx) ;
					}
					else if(detid == 306315288)
					{
						test3->Fill(momentum, dedx) ;
					}
					else
					{
						cout << "Problem: " << detid << endl ;
						exit(1) ;
					}
				}
			}
		}
	}

	test->SaveAs("plots/dedx_per_detector/test.root") ;
	test2->SaveAs("plots/dedx_per_detector/test2.root") ;
	test3->SaveAs("plots/dedx_per_detector/test3.root") ;
}

void test_dedx_per_momentum_and_per_detector_using_log_with_string() 
{
	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;

	TH2F *test =  new TH2F("test","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);
	TH2F *test2 =  new TH2F("test2","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);
	TH2F *test3 =  new TH2F("test3","dE/dx vs p 4trk", 500, 0., 5., 1600, 0., 20.);

	TH2F *test_new_01 =  new TH2F("test_new_01","test_new_01", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_02 =  new TH2F("test_new_02","test_new_02", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_03 =  new TH2F("test_new_03","test_new_03", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_04 =  new TH2F("test_new_04","test_new_04", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_05 =  new TH2F("test_new_05","test_new_05", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_06 =  new TH2F("test_new_06","test_new_06", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_07 =  new TH2F("test_new_07","test_new_07", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_08 =  new TH2F("test_new_08","test_new_08", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_09 =  new TH2F("test_new_09","test_new_09", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_10 =  new TH2F("test_new_10","test_new_10", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_11 =  new TH2F("test_new_11","test_new_11", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_12 =  new TH2F("test_new_12","test_new_12", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_13 =  new TH2F("test_new_13","test_new_13", 100, 0., 5., 100, 0., 4.);
	TH2F *test_new_14 =  new TH2F("test_new_14","test_new_14", 100, 0., 5., 100, 0., 4.);
	
	typedef vector<TH1F *> vector_th1f ;
	typedef vector<TH2F *> vector_th2f ;
	typedef vector<TProfile *> vector_profile ;

	map<uint32_t, vector_th2f> mymap ;
	map<uint32_t, vector_th1f> mymap_thickness ;
	map<uint32_t, vector_th1f> mymap_path_length ;
	map<uint32_t, vector_th2f> mymap_correlation ;
	map<uint32_t, vector_th2f> mymap_normalized_with_path ;	
	map<uint32_t, vector_profile> mymap_normalized_with_path_profile ;	

	for(int i = 0 ; i < 16 ; ++i)
	{
		mymap[0x12101004].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1210100c].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x14841804].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x14842404].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1c045125].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1c045168].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1c0451a5].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1c0454a6].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x16005415].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1600541e].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x18002a92].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x18002aa2].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1a005025].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
		mymap[0x1a00503a].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 4.));
	}

	for(map<uint32_t, vector_th2f> ::iterator it = mymap.begin() ; it != mymap.end() ; ++it)
	{
		cout << "it->first " << dec << it->first << " " << hex << it->first << endl ;
	}
	
	for(map<uint32_t, vector_th2f> ::iterator it = mymap.begin() ; it != mymap.end() ; ++it)
	{
		int vectorindex = mymap[it->first].size() ;
		
		for(int kindex = 0 ; kindex < vectorindex ; ++kindex)
		{
			mymap_thickness[it->first].push_back(new TH1F("t1","t1", 10000, 0., 0.1));
			mymap_path_length[it->first].push_back(new TH1F("p1","p1", 1000, 0., 0.5));
//			mymap_correlation[it->first].push_back(new TH2F("1","1", 1000, 0., 0.5, 1000, 0., 4.0));
			mymap_correlation[it->first].push_back(new TH2F("1","1", 1000, 0., 0.2, 1000, 0., 1.0));
			mymap_normalized_with_path[it->first].push_back(new TH2F("1","1", 100, 0., 5., 100, 0., 10.));

			mymap_normalized_with_path_profile[it->first].push_back(new TProfile("1","1", 100, 0., 5.));
		}
	}


	for(int i = 0 ; i < 1500 ; ++i)
	{
		stringstream ss ;
		
		if((i % 100) == 0)
		{
			cout << dec << i << endl ;
		}
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		string filename = directory + "dir_" + prefix + ss.str() + "/log";
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{
			string word = "" ;

			while(logfile >> word)
			{
				if(word.compare("Detid:") == 0)
				{
					
					uint32_t detid = 0 ;
					double momentum = 0 ;
					double dedx = 0 ;
					
					double thickness = 0 ;
					double path = 0 ;
					int chipid = 0;
					
					logfile >> detid >> word >> chipid ;
					// cout << chipid << endl ;
					
					logfile >> word >> momentum >> word >> dedx >> word >> thickness >> word >> path ;

					// cout << "Detid " << detid << endl ;
					
					if((chipid > -1) && (chipid < 16))
					{
						mymap[detid][chipid]->Fill(momentum, dedx) ;
						mymap_thickness[detid][chipid]->Fill(thickness) ;
						mymap_path_length[detid][chipid]->Fill(path) ;

						mymap_correlation[detid][chipid]->Fill(path, dedx) ;
						mymap_normalized_with_path[detid][chipid]->Fill(momentum, (dedx / path)) ;

						// mymap[detid][0]->Fill(momentum, dedx) ;
						// mymap_thickness[detid][0]->Fill(thickness) ;
						// mymap_path_length[detid][0]->Fill(path) ;

						// mymap_correlation[detid][0]->Fill(path, dedx) ;
						// mymap_normalized_with_path[detid][0]->Fill(momentum, (dedx / path)) ;
					
						double dedx_norm = (dedx / path) ;
						if(dedx_norm < 4.0) mymap_normalized_with_path_profile[detid][chipid]->Fill(momentum, dedx_norm) ;
					}
					
				}
			}
		}
	}

	gStyle->SetOptStat("nemro") ;
	
	test->SaveAs("plots/dedx_per_detector/test.root") ;
	test2->SaveAs("plots/dedx_per_detector/test2.root") ;
	test3->SaveAs("plots/dedx_per_detector/test3.root") ;
	
	ofstream gain_results("results/gain_results.txt") ;
	ofstream gain_results_with_Landau_fit("results/gain_results_with_Landau_fit.txt") ;

	gStyle->SetLineScalePS(.3) ;
	
	TCanvas c ;
	c.SetLogz() ;
	
	for(map<uint32_t, vector_th2f>::iterator it = mymap.begin() ; it != mymap.end() ; ++it)
	{
		int vectorindex = mymap_thickness[it->first].size() ;
		
		for(int kindex = 0 ; kindex < vectorindex ; ++kindex)
		{
			stringstream ss ;
			stringstream ss2 ;

			ss << it->first ;
			ss2 << kindex ;

			mymap[it->first][kindex]->SaveAs(("plots/dedx_per_detector/det_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;

			mymap_thickness[it->first][kindex]->SaveAs(("plots/dedx_per_detector/det_thickness_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;
			
			mymap_path_length[it->first][kindex]->SetName(ss.str().c_str()) ;
			mymap_path_length[it->first][kindex]->SaveAs(("plots/dedx_per_detector/det_path_length_" + ss.str() + "_" + ss2.str()+ ".root").c_str()) ;
			
			mymap_correlation[it->first][kindex]->GetXaxis()->SetTitle("Path lenth (cm)") ;
			mymap_correlation[it->first][kindex]->GetYaxis()->SetTitle("#Delta E (MeV)") ;
			mymap_correlation[it->first][kindex]->GetXaxis()->SetTitleFont(132) ;
			mymap_correlation[it->first][kindex]->GetYaxis()->SetTitleFont(132) ;

			mymap_normalized_with_path[it->first][kindex]->GetXaxis()->SetTitle("Momentum (GeV)") ;
			mymap_normalized_with_path[it->first][kindex]->GetYaxis()->SetTitle("dE/dx (MeV/cm)") ;
			mymap_normalized_with_path[it->first][kindex]->GetXaxis()->SetTitleFont(132) ;
			mymap_normalized_with_path[it->first][kindex]->GetYaxis()->SetTitleFont(132) ;

			mymap_correlation[it->first][kindex]->SetTitle(ss.str().c_str()) ;
			mymap_correlation[it->first][kindex]->SaveAs(("plots/dedx_per_detector/det_corr_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;

			mymap_correlation[it->first][kindex]->Draw("colz") ;
			if(kindex == 0) c.SaveAs(("plots/dedx_per_detector/det_corr_" + ss.str() + "_" + ss2.str() + ".png").c_str()) ;


			mymap_normalized_with_path[it->first][kindex]->SetTitle((ss.str() + "_" + ss2.str()).c_str()) ;
			mymap_normalized_with_path[it->first][kindex]->SaveAs(("plots/dedx_per_detector/mymap_normalized_with_path_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;

			mymap_normalized_with_path[it->first][kindex]->Draw("colz") ;
			if((kindex == 0) || (it->first == 369120277)) c.SaveAs(("plots/dedx_per_detector/mymap_normalized_with_path_" + ss.str() + "_" + ss2.str() + ".png").c_str()) ;


			mymap_normalized_with_path_profile[it->first][kindex]->Fit("pol1", "", "", 2, 3) ;
			
			double value = 0 ;
			if(mymap_normalized_with_path_profile[it->first][kindex]->GetFunction("pol1") != NULL) value = mymap_normalized_with_path_profile[it->first][kindex]->GetFunction("pol1")->Eval(2.5) ;
			
			TH1D *projectiony = mymap_normalized_with_path[it->first][kindex]->ProjectionY("projy", 35, 45) ;
			projectiony->SetTitle((ss.str() + "_" + ss2.str()).c_str()) ;
			// TH1D *projectiony = mymap_normalized_with_path[it->first][kindex]->ProjectionY("projy", 20, 45) ; // Sometimes useful
			
			double mpv = 0 ;
			
			if(projectiony->GetEntries() > 0)
			{
			
				// Most cases
				TFitResultPtr ptr = projectiony->Fit("landau", "S", "", 2.5, 3.5) ; 
				
				// Sometimes better to lower the lower boundary
				// TFitResultPtr ptr = projectiony->Fit("landau", "S", "", 2.3, 3.5) ;
			
				mpv = ptr->Parameter(1) ;
			}
			
			gain_results << "gain_result " << it->first << " " << kindex << " " << value << endl ;
			gain_results_with_Landau_fit << "gain_result_with_Landau_fit " << it->first << " " << kindex << " " << mpv << endl ;
			
			projectiony->SaveAs(("plots/dedx_per_detector/mymap_normalized_with_path_projection_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;
			
			mymap_normalized_with_path_profile[it->first][kindex]->SetName(ss.str().c_str()) ;
			mymap_normalized_with_path_profile[it->first][kindex]->SaveAs(("plots/dedx_per_detector/mymap_normalized_with_path_profile_" + ss.str() + "_" + ss2.str() + ".root").c_str()) ;
		}
	}
	
	gain_results.close() ;
	gain_results_with_Landau_fit.close() ;
	
	c.SetLogz() ;

	test_new_01->Draw("colz") ;
	c.SaveAs("plots/dedx_per_detector/test_new_01.pdf") ;
	
	test_new_02->Draw("colz") ;
	c.SaveAs("plots/dedx_per_detector/test_new_02.pdf") ;
	
	test_new_03->Draw("colz") ;
	c.SaveAs("plots/dedx_per_detector/test_new_03.pdf") ;
	
	test_new_04->Draw("colz") ;
	c.SaveAs("plots/dedx_per_detector/test_new_04.pdf") ;

}


string filename_detectors_id = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/Scripts/list_of_detectors_ids_in_events_after_phi_phi_peak_mass_cut_diagonal.txt" ;

void test_dedx_per_momentum_and_per_detector_using_log_with_string_ntuple(int argc, char *argv[])
{

	if((argc != 4))
	{
		cout << "I need 3 arguments: low high outputindex." << endl ;
		exit(1) ;
	}
   
   string localdir(argv[0]) ;
   
   cout << localdir << endl ;
	
	int lowlimit =  atoi(argv[1]) ;
	int highlimit = atoi(argv[2]) ;
	string outputindex(argv[3]) ;

   stringstream ss_summary ;

	string command_directory = ("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/condor_summary/dir_" + outputindex)  ;

 	TFile* myfile = TFile::Open((command_directory + "/summary.root").c_str(), "RECREATE", "Generic") ;
	
	myfile->Write() ;
	myfile->cd() ;

	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;

	ifstream detectors(filename_detectors_id) ;

	typedef vector<TH2F *> vector_th2f ;	
	map<uint32_t, vector_th2f> mymap_normalized_with_path ;	
	
	uint32_t mydetid ;
	
	int numberofhistos = 0 ;
	
	while(detectors >> mydetid)
	{
		for(int kindex = 0 ; kindex < 16 ; ++kindex)
		{
			stringstream ss1 ;
			stringstream ss2 ;
		
			ss1 << mydetid ;
			ss2 << kindex ;
			
			string histname = "hist_" + ss1.str() + "_" + ss2.str() ;
		
			mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0., 5., 100, 0., 10.)) ;
			
		}

		++numberofhistos ;

		if((numberofhistos % 1000) == 0)
		{
			cout << dec << "histo: " << numberofhistos << endl ;
		}
			
	}
	
	detectors.close() ;
	
// 	for(int i = 0 ; i < 8 ; ++i)
	for(int i = 0 ; i < 1500 ; ++i)
	{
		if((i < lowlimit) || (i > highlimit)) continue ;   

		stringstream ss ;
		
		if((i % 100) == 0)
		{
			cout << dec << i << endl ;
		}
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		// string filename = directory + "dir_" + prefix + ss.str() + "/log";
		string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/log_" + prefix + ss.str() ;
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{

			cout << filename << endl ;
			string word = "" ;

			while(logfile >> word)
			{
				if(word.compare("Detid:") == 0)
				{
					
					uint32_t detid = 0 ;
					double momentum = 0 ;
					double dedx = 0 ;
					
					double thickness = 0 ;
					double path = 0 ;
					int chipid = 0;
					
					logfile >> detid >> word >> chipid ;
					// cout << chipid << endl ;
					
					logfile >> word >> momentum >> word >> dedx >> word >> thickness >> word >> path ;

					// cout << "Detid " << detid << endl ;
					
					if((chipid > -1) && (chipid < 16))
					{
						if(mymap_normalized_with_path[detid][chipid] != NULL) mymap_normalized_with_path[detid][chipid]->Fill(momentum, (dedx / path)) ;
						// cout << mymap_normalized_with_path[detid][chipid] << endl ;
					}
					
				}
			}
		}
	}
	
	cout << endl << endl << "Reading of log files: done..." << endl ;

	for(int i = 0 ; i < 10 ; ++i)
	{
		system("sleep 1s") ;
		cout << "."  ;
	}

	cout << endl << endl ;
	
	numberofhistos = 0 ;
	myfile->cd() ;
	
	
	for(map<uint32_t, vector_th2f>::iterator it = mymap_normalized_with_path.begin() ; it != mymap_normalized_with_path.end() ; ++it)
	{
		for(int kindex = 0 ; kindex < 16 ; ++kindex)
		{
			if(it->second[kindex]->GetEntries() > 10) cout << it->second[kindex]->GetName() << endl ;

		}
	}
	
	myfile->Write() ;
}



void further_analyse_gains_if_possible_to_incorporate_differences()
{
	ifstream gains_by_Ferenc("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/UserCode/EnergyLossPID/data/gain_totem.dat") ;
	
	string detid ;
	string chipid ;
	string somenumber ;
	double gain_calibration ;
	double some_other_number ;
	string detector_name ;
	
	TH1F *gain_calib =  new TH1F("gain_calib","gain_calib", 100, 0., 3.);
	TH1F *gain_calib_PXB =  new TH1F("gain_calib_PXB","gain_calib_PXB", 100, 0., 3.);
	TH1F *gain_calib_PXF =  new TH1F("gain_calib_PXF","gain_calib_PXF", 100, 0., 3.);
	
	while(gains_by_Ferenc >> detid)
	{
		gains_by_Ferenc >> chipid ;
		gains_by_Ferenc >> somenumber ;
		gains_by_Ferenc >> gain_calibration ;
		gains_by_Ferenc >> some_other_number ;
		gains_by_Ferenc >> detector_name ;
		
		if(detector_name.compare("PXB") == 0)
		{
			gain_calib_PXB->Fill(gain_calibration) ;
		}
		else if(detector_name.compare("PXF") == 0)
		{
			gain_calib_PXF->Fill(gain_calibration) ;
		}

		gain_calib->Fill(gain_calibration) ;
		
		
		// cout << detector_name << " " << gain_calibration << endl ;
	}
	
	gain_calib->SaveAs("plots/gains/gain_calib.root") ;
	gain_calib_PXB->SaveAs("plots/gains/gain_calib_PXB.root") ;
	gain_calib_PXF->SaveAs("plots/gains/gain_calib_PXF.root") ;
}

void convert_gain()
{
	ifstream gains_by_Ferenc("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/UserCode/EnergyLossPID/data/gain_totem.dat") ;
	
	uint32_t detid ;
	string chipid ;
	string somenumber ;
	double gain_calibration ;
	double some_other_number ;
	string detector_name ;

	while(gains_by_Ferenc >> hex >> detid)
	{
		gains_by_Ferenc >> chipid ;
		gains_by_Ferenc >> somenumber ;
		gains_by_Ferenc >> gain_calibration ;
		gains_by_Ferenc >> some_other_number ;
		gains_by_Ferenc >> detector_name ;
		
		cout << hex << detid << " " << dec << detid << " " << detector_name << endl ;
	}
	
}

void analyze_file(int argc, char *argv[])
{
   gErrorIgnoreLevel = 1 ;

	TFile *myfile = TFile::Open("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/condor_summary/summary.root", "READ") ;
	
	// cout << "ok" << endl ;
	
	TH1D *hist_mpv = new TH1D("hist", "hist", 100, 0, 10) ;
	TH1D *frame = new TH1D("frame", "frame", 100, 0, 10) ;
	
	TCanvas *c = new TCanvas() ;
	TCanvas *c1 = new TCanvas() ;

	c->cd() ;	
	frame->Draw("hist") ;
	frame->GetYaxis()->SetRangeUser(0, 1000) ;
	
	bool found = false ;
	
	if((argc != 4))
	{
		cout << "I need 3 arguments: low high outputindex." << endl ;
		exit(1) ;
	}
	
	int lowlimit =  atoi(argv[1]) ;
	int highlimit = atoi(argv[2]) ;
	int outputindex = atoi(argv[3]) ;
	
	// cout << "limits " << lowlimit << " " << highlimit << " " << outputindex << endl ;
	
	int plotindex = 0 ;
	int plotindex_2 = 0 ;

	double maximum = -1 ;
   
   bool save_png_for_latex = false ;
	
	TH1D *projectiony = NULL ;
	
	stringstream ss ;
	ss << outputindex ;
   
	ofstream latexfile(("latex/latexfile_" + ss.str() + ".tex").c_str()) ;
	ofstream error_file(("err/error_filef_" + ss.str() + ".tex").c_str()) ;
	ofstream gains(("results/gains/gain_" + ss.str() + ".txt").c_str()) ;
	
	for(int i = 0; i < myfile->GetListOfKeys()->GetEntries() ; ++i)
	{
		if((i < lowlimit) || (i > highlimit)) continue ;
      
      if((i % 1000) == 0)
      {
         // cout << "." << endl ;
         // system("date") ;
      }
	
		string histname = myfile->GetListOfKeys()->At(i)->GetName() ;

		// if(!found && (histname.compare("hist_436233306_1") != 0)) continue ;
		// else
		//	found = true ;
		
		if(histname.compare("hist_344279044_1") == 0) continue ;
		if(histname.compare("hist_436228345_0") == 0) continue ;
		if(histname.compare("hist_436229178_0") == 0) continue ;
		if(histname.compare("hist_436232378_2") == 0) continue ;
		if(histname.compare("hist_436233306_1") == 0) continue ;

		// if(i == 10000) break ;
		
		TH2F *hist = ((TH2F *) myfile->Get(histname.c_str())) ;
		
		// hist->Draw("colz") ;
		
		bool first = false ;
		
		if(hist->GetEntries() > 400)
		{
			projectiony = hist->ProjectionY((histname + "_projy").c_str(), 35, 45) ;
			
			// cout << histname << ": " << i << " " << hist->GetEntries() << " " << projectiony->GetEntries() << " " << endl ;

			double mpv = 0 ;

			if((projectiony->GetEntries() > 150) && (projectiony->GetMean() != 0))
			{
            double delta = 0.05 ;
            double lox = 2.0 ;
            double hix = 3.0  ;
            
            bool condition = true ;
            
            class TMyResults
            {
               public:
               double lox, hix, p_value ;
            } myresults ;
            
            
            vector<TMyResults> myresults_vector ;
            
            int iteration = 0 ;

            for(int vi = 0 ; vi < 20 ; ++vi)
            {
               c1->cd() ;
        			TFitResultPtr ptr = projectiony->Fit("gaus", "SQ", "", lox, hix) ;

               if(ptr != -1)
               {
                  mpv = ptr->Parameter(1) ;

                  double chi2 = ptr->Chi2() ;
                  int ndf = ptr->Ndf() ;
                  double p_value = TMath::Prob(chi2, ndf) ;

                  myresults.lox = lox ;
                  myresults.hix = hix ;
                  myresults.p_value = p_value ;

                  condition = ((mpv > lox) && (mpv < hix)) ;

                  if(condition) myresults_vector.push_back(myresults) ;
               }
               
               lox += delta ;
               hix += delta ;
               ++iteration ;
            }
            
            double min_p_value = 100 ;
            
            projectiony->SetLineWidth(1) ;

            if(myresults_vector.size() == 0)
            {
               error_file << "Warning: problematic fit" << endl ;
               projectiony->SetLineWidth(4) ;
               projectiony->SetLineColor(kOrange) ;
            }

            for(int myi = 0 ; myi < myresults_vector.size() ; ++myi)
            {
               if(myresults_vector[myi].p_value < min_p_value)
               {
                  min_p_value = myresults_vector[myi].p_value ;
                  
                  lox = myresults_vector[myi].lox ;
                  hix = myresults_vector[myi].hix ;
               }
            }
            
            c1->cd() ;            

            if(myresults_vector.size() != 0)            
            {
        			TFitResultPtr ptr = projectiony->Fit("gaus", "SQ", "", lox, hix) ;
               mpv = ptr->Parameter(1) ;

		   		hist_mpv->Fill(mpv) ;
               
               string detector_id_string = histname.substr(5, 9) ;
               string chip_id = histname.substr(15, 2) ;
               
               uint32_t detector_id = atoi(detector_id_string.c_str()) ;
               
               double gain_calib = (2.5 / mpv) ;
               
               // cout << dec << detector_id << " " << hex << detector_id << " " << chip_id << endl ;

               string space = "" ;
               
               if(chip_id.length() < 2) space = " " ;

               gains << hex << detector_id << dec << " " << chip_id << space << " " << mpv << " " << gain_calib << endl ;
            }
					
				
				c->cd() ;
				
				++plotindex ;
				
				if(plotindex == 10)
				{
					projectiony->Draw("same") ;
					double thismax = projectiony->GetBinContent(projectiony->GetMaximumBin()) ;
					if(maximum < thismax) maximum = thismax ;
					frame->GetYaxis()->SetRangeUser(0, maximum * 1.2) ;
					c->Update() ;
						
               string filename = "plots/temporary/" + histname + ".png" ;

   				if(save_png_for_latex) c->SaveAs(filename.c_str()) ;
						
					// c = new TCanvas() ;
						
					plotindex = 0 ;
               maximum = -1 ;

					frame->Draw("hist") ;
					frame->GetYaxis()->SetRangeUser(0, 1000) ;
               
               // Latex related
               
     				++plotindex_2 ;
               
               if(plotindex_2 == 1)
               {
                  latexfile << "\\begin{frame}\\tiny" << endl ;
	               latexfile << "   \\begin{block}{Slide}" << endl ;
               }

					latexfile << "    \\includegraphics[trim = 0mm 0mm 0mm 0mm, clip,width=0.19\\textwidth]{" + filename + "}" << endl ;
               
               if((plotindex_2 % 5) == 0)
               {
	               latexfile << "\\newline" << endl ;
               }
               
               if(plotindex_2 == 25)
               {
	               latexfile << "   \\end{block}" << endl ;
                  latexfile << "\\end{frame}" << endl ;
                  
                  plotindex_2 = 0 ;
               }
               
				}
				else
				{
						// cout << "." << histname << endl ;
						projectiony->Draw("same") ;
						
					  double thismax = projectiony->GetBinContent(projectiony->GetMaximumBin()) ;
					  if(maximum < thismax) maximum = thismax ;
				}

			}
		}
	
	} 

	// cout << endl ;
	
	c->SaveAs("plots/temporary/my_canvas.root") ;
	
	hist_mpv->Draw("colz") ;
	
	string filename = "results/mpv/hist_mpv_" + ss.str() + ".root" ;

	hist_mpv->SaveAs(filename.c_str()) ;
	// c.SaveAs(filename2.c_str()) ;
	
	latexfile.close() ;
	error_file.close() ;
	
	exit(0) ;

}

void analyze_correlation()
{
   // To use this you need the line 
   // 1. DataHandler.cc : line with "Detid..." printout for both pixel and strip
   // 2. EnergyLossProducer.cc : the lines "Track begin" and "Track end"  

   string directory = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log" ;
   
   int nbins = 20 ;
   
   TH2F *covariance = new TH2F("covariance", "covariance", nbins, 3e8, 5e8, nbins, 3e8, 5e8) ;
   
   TGraph2D *graph = new TGraph2D() ;
   
   const int save_trackindex = 101 ;
   const int tracks_to_save = 10 ;

   int trackindex = 0 ;
   int trackplotindex = 0 ;

	for(int i = 0 ; i < 1500 ; ++i)
	{
		stringstream ss ;
		
		if((i % 100) == 0)
		{
			cout << dec << i << endl ;
		}
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		// string filename = directory + "dir_" + prefix + ss.str() + "/log";
		// string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/log_" + prefix + ss.str() ;
		string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back_for_tracks_and_correlations/log_" + prefix + ss.str() ;
		
		ifstream logfile(filename.c_str()) ;
		
      vector<uint32_t> detids ;
      
      
		if(logfile.is_open())
		{
         cout << "hello" << endl ;
         
         string word = "" ;
         
         while(logfile >> word)
         {
            if(word.compare("Track") == 0)
            {
               // cout << "ok" << endl ;
               
               if(detids.size() > 0)
               {
                  for(int kj = 0 ; kj <  detids.size() ; ++kj)
                  for(int nj = 0 ; nj <  detids.size() ; ++nj)
                  {
                   covariance->Fill(detids[kj], detids[nj]) ;
                  }
               
               }

               detids.clear() ;
               
               logfile >> word ;
               
               if(word.compare("begin") == 0) ++trackindex ;

               if(word.compare("end") == 0) 
               if((trackindex >= save_trackindex) && (trackindex < (save_trackindex + tracks_to_save)))
               {
                  stringstream ssu ;
                  
                  ssu << trackplotindex ;
                  
                  graph->SaveAs(("plots/graph_" + ssu.str() +  ".root").c_str()) ;
                  graph->Clear() ;
                  
                  ++trackplotindex ;
               }
               
               // cout << "trackindex: " << trackindex << endl ;
            }

		      if(word.compare("Detid:") == 0)
		      {

			      uint32_t detid = 0 ;
			      double momentum = 0 ;
			      double dedx = 0 ;

			      double thickness = 0 ;
			      double path = 0 ;
			      int chipid = 0;

			      logfile >> detid >> word >> chipid ;
			      // cout << chipid << endl ;
               

			      logfile >> word >> momentum >> word >> dedx >> word >> thickness >> word >> path ;

               detids.push_back(detid) ;
            }
            

            if(word.compare("Measurement_global_coordinates:") == 0)
            {
               double x, y, z ;

               logfile >> x >> y >> z ;

               if((trackindex >= save_trackindex) && (trackindex < (save_trackindex + tracks_to_save)))
               {
                  cout << "save" << endl ;
                  int npoints = graph->GetN() ;
                  graph->AddPoint(x, y, z) ;
               }
            }
         }
         
         logfile.close() ;
      }
   }


   covariance->Scale((1.0 / 1.0110964e+08) * 100.0) ;
   covariance->SaveAs("plots/covariance.root") ;

   TH2F *correlation = new TH2F("correlation", "correlation", nbins, 3e8, 5e8, nbins, 3e8, 5e8) ;


   for(int kj = 0 ; kj <  nbins ; ++kj)
   for(int nj = 0 ; nj <  nbins ; ++nj)
   {
      double norm = sqrt(covariance->GetBinContent(kj, kj) * covariance->GetBinContent(nj, nj)) ;
      double value = covariance->GetBinContent(kj, nj) ;
   
      if(norm != 0) correlation->SetBinContent(kj, nj, 1e-2 * round(100.0 * (value / norm))) ;
      else
      {
         correlation->SetBinContent(kj, nj, 0) ;
      }
   }
   
   correlation->SaveAs("plots/correlation.root") ;
   
 
}

void analyze_frequencies()
{
   ifstream detectors_to_check("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/Scripts/list_of_detectors_ids_in_events_after_phi_phi_peak_mass_cut_diagonal.txt") ;

   map<uint32_t, int> frequencies ;

   uint32_t mydetid ;
   
   while(detectors_to_check >> mydetid)
   {
      frequencies[mydetid] = 0 ;
   }
   
   detectors_to_check.close() ;
	
//   for(int i = 0 ; i < 10 ; ++i)
   for(int i = 0 ; i < 1500 ; ++i)
   {

      stringstream ss ;

		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}

      string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/frequencies/frequency_" + prefix + ss.str() + ".txt" ;
      ifstream frequencies_file(filename.c_str()) ;

		if(frequencies_file.is_open())
      {
         cout << " ok to open " << filename << endl ;
         
         int occurances = 0 ;
         
         while(frequencies_file >> occurances >> mydetid)
         {
                // cout << detid << endl ;  
                frequencies[mydetid] += occurances ;  
         }
      }
      
      frequencies_file.close() ;

   }

   TH1F *hist = new TH1F("hist", "hist", 1e3, 3e8, 5e8) ;
   TH1F *hist2 = new TH1F("hist2", "hist2", frequencies.size(), 0, frequencies.size()) ;

   int index = 0 ;

   for(map<uint32_t, int>::iterator it = frequencies.begin() ; it != frequencies.end() ; ++it)
   {
      // cout << it->first << " " << it->second << endl ;
      if(it->second < 2000) cout << it->first << " " << it->second << endl ;
      
      hist->Fill(it->first, it->second) ;
      hist2->Fill(index, it->second) ;
      
      ++index ;
   }
   
   hist->SaveAs("plots/frequencies.root") ;
   hist2->SaveAs("plots/frequencies2.root") ;
}


// Useful tests
// grep "Event_starts:"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc ;  grep "Track begin"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc ; grep "global"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc
// grep "mytest2:"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc ;  grep "pidkaon"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc ; grep "nOKPhiCut"  Studies/perfect_ones_to_be_copied_back/log_0123  | wc

class TMeasurement
{
  private:

  int measurement_type ;

  uint32_t  detid ;
  int       chipid ;
  string    detector_type ;
  double    momentum ;
  double    dedx ;
  double    thickness ;
  double    measx ;
  double    measy ;
  double    measz ;
  double    path_length ;

  public:

  static const int measurement_type_coordinates ;
  static const int measurement_type_dedx_path_length ;

  TMeasurement(uint32_t, int, string, double, double, double, double, double, double) ;
  TMeasurement(uint32_t, int, string, double, double, double, double) ;
  
  double GetPathLength() { return path_length ; } ; 
  
  double GetdE() { return dedx ; } ; 
  double GetMomentum() { return momentum ; } ; 
  uint32_t GetDetectorID() { return detid ; } ; 
  int GetChipID() { return chipid ; } ; 
  double GetType() { return measurement_type ; } ; 

  double GetX() { return measx ; } ; 
  double GetY() { return measy ; } ; 
  double GetZ() { return measz ; } ; 

} ;

const int TMeasurement::measurement_type_coordinates = 1;
const int TMeasurement::measurement_type_dedx_path_length = 2;


TMeasurement::TMeasurement(uint32_t detid, int chipid, string detector_type, double momentum, double dedx, double thickness, double measx, double measy, double measz) :
measurement_type(measurement_type_coordinates) , detid(detid), chipid(chipid),  detector_type(detector_type),  momentum(momentum),  dedx(dedx),  thickness(thickness), measx(measx),  measy(measy),  measz(measz), path_length(0)
{
}

TMeasurement::TMeasurement(uint32_t detid, int chipid, string detector_type, double momentum, double dedx, double thickness, double path_length) :
measurement_type(measurement_type_dedx_path_length), detid(detid), chipid(chipid),  detector_type(detector_type),  momentum(momentum),  dedx(dedx),  thickness(thickness),  measx(0),  measy(0),  measz(0), path_length(path_length)
{
} 

class TAnalysisTrack
{
  bool pidkaon ;
  bool HasSaturatedMeasurement ;
  double momentum, eta, phi, charge, pt ;

  vector<TMeasurement *> measurements ;

  vector<pair<pair<uint32_t, int>, vector<double> *>> adc_strip_measurements ;  
  vector<pair<pair<uint32_t, int>, vector<double> *>> adc_pixel_measurements ;  

  public:

  TAnalysisTrack() ;
  ~TAnalysisTrack() ;
  
  void SetPidKaon() { pidkaon = true ; } ;
  void AddMeasurement(TMeasurement *measurement) { measurements.push_back(measurement) ; } ;

  void AddStripADCMeasurements(vector<double> *an_adc_measurement, uint32_t detid, int chipid) { pair<pair<uint32_t, int>, vector<double> *> ok ; ok.first.first = detid ; ok.first.second = chipid ; ok.second = an_adc_measurement ; adc_strip_measurements.push_back(ok) ; } ;
  void AddPixelADCMeasurements(vector<double> *an_adc_measurement, uint32_t detid, int chipid) { pair<pair<uint32_t, int>, vector<double> *> ok ; ok.first.first = detid ; ok.first.second = chipid ; ok.second = an_adc_measurement ; adc_pixel_measurements.push_back(ok) ; } ;

  void SetTrackMomentumEtaPhi(double a_charge, double a_eta, double a_phi, double a_pt, double a_momentum) { charge = a_charge, eta = a_eta,  phi = a_phi,  pt = a_pt,  momentum = a_momentum ; } ;

  vector<double> GetNumberOfStripAdcMeasurements() { vector<double> number_of_Adc_values ; for(int i = 0 ; i < adc_strip_measurements.size() ; ++i) number_of_Adc_values.push_back(adc_strip_measurements[i].second->size()) ; return number_of_Adc_values ; } ;
  vector<double> GetNumberOfPixelAdcMeasurements() { vector<double> number_of_Adc_values ; for(int i = 0 ; i < adc_pixel_measurements.size() ; ++i) number_of_Adc_values.push_back(adc_pixel_measurements[i].second->size()) ; return number_of_Adc_values ; } ;

  vector<pair<pair<uint32_t, int>, vector<double> *>> *GetPointerStripAdcMeasurements() { return &adc_strip_measurements ; } ;
  vector<pair<pair<uint32_t, int>, vector<double> *>> *GetPointerPixelAdcMeasurements() { return &adc_pixel_measurements ; } ;
  
  vector<TMeasurement *> *GetMeasurements() { return &measurements ; } ;
  bool isKaon() { return pidkaon ; } ;
  double GetMomentum() { return momentum ; } ;
  double GetEta() { return eta ; } ;
  double GetPt() { return pt ; } ;
  double GetP() { return momentum ; } ;

  void SetHasSaturatedMeasurement() { HasSaturatedMeasurement = true ; } ;
  bool IsSaturatedMeasurement() { return HasSaturatedMeasurement ; } ;
} ;

TAnalysisTrack::TAnalysisTrack(): pidkaon(false), HasSaturatedMeasurement(false), momentum(0), eta(0), phi(0), adc_strip_measurements(NULL), adc_pixel_measurements(NULL)
{
}

TAnalysisTrack::~TAnalysisTrack()
{
  // cout << "     I am here to delete2" << endl ;
  for(vector<TMeasurement *>::iterator it = measurements.begin() ; it != measurements.end() ; ++it)
  {
    delete (*it) ;
  }

  for(vector<pair<pair<uint32_t, int>, vector<double> *>>::iterator it = adc_strip_measurements.begin() ; it != adc_strip_measurements.end() ; ++it)
  {
    delete (it->second) ;
  }

  for(vector<pair<pair<uint32_t, int>, vector<double> *>>::iterator it = adc_pixel_measurements.begin() ; it != adc_pixel_measurements.end() ; ++it)
  {
    delete (it->second) ;
  }
}

class TAnalysisEvent
{
  long int event_number ;
  bool nOKPhiCut ;
  bool CleanRPConf ;
  bool var_GoodReferenceForKaon ;
  
  vector<TAnalysisTrack *> tracks;

  public:

  TAnalysisEvent(long int) ;
  ~TAnalysisEvent() ;
  
  void PhiCutWasOk() { nOKPhiCut = true ; } ;
  void GoodReferenceForKaon() { var_GoodReferenceForKaon = true ; } ;
  void SetCleanRPConf() { CleanRPConf = true ; } ;
  void AddTrack(TAnalysisTrack *track) { tracks.push_back(track) ; } ;

  bool GetCleanRPConf() { return CleanRPConf ; } ;
  bool GetGoodReferenceForKaon() { return var_GoodReferenceForKaon ; } ;

  long int GetEventNumber() { return event_number ; } ;
  
  int getNumberOfTracks() { return tracks.size() ; } ;
  
  vector<TAnalysisTrack *> *GetTracks() { return &tracks ; } ;
  void SetTrackAsKaon(int, double, bool) ;

  bool WasPhiCutOk() { return nOKPhiCut ; } ;
} ;

void TAnalysisEvent::SetTrackAsKaon(int track_index, double test_momentum, bool check)
{
  tracks[track_index]->SetPidKaon() ;
  
  if(check)
  {
    double deltap = fabs((tracks[track_index]->GetMomentum() - test_momentum) / tracks[track_index]->GetMomentum()) ;

    if(deltap > 0.01)
    {
      cout << tracks.size() << " " ;
      cout << tracks[0]->GetMomentum() << " " << tracks[1]->GetMomentum() << " "  << tracks[2]->GetMomentum() << " "  << tracks[3]->GetMomentum() << " " ;
      cout << "Warning: kaon and track momentum disagree more than 1 percent! " << tracks[track_index]->GetMomentum() << " " << test_momentum << endl ;
    }
    else
    {
      // cout << "kaon ok" << endl ;
    }
  }
}


TAnalysisEvent::TAnalysisEvent(long int event_number): tracks(NULL), event_number(event_number), nOKPhiCut(false), CleanRPConf(false), var_GoodReferenceForKaon(false)
{
}

TAnalysisEvent::~TAnalysisEvent()
{
  // cout << " I am here to delete" << endl ;
  for(vector<TAnalysisTrack *>::iterator it = tracks.begin() ; it != tracks.end() ; ++it)
  {
    delete (*it) ;
  }
}


bool check_if_file_has_momentum_after_pidkaon(string file_to_test)
{
  ifstream files_still_without_momentum_in_pidkaon("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/test/files_still_without_momentum_in_pidkaon.txt") ;
  
  string filename = "" ;
  
  while(files_still_without_momentum_in_pidkaon >> filename)
  {
    // cout << "checktest " << filename << endl ;
    if(filename.compare(file_to_test) == 0) return false ;
  }
  
  files_still_without_momentum_in_pidkaon.close() ;
  
  return true ;
}

TF1 *f1 = new TF1("f1", "([0]/(x*x)) + ([1]/x) + [2] + 0.5", 0, 0.7) ;
TF1 *f2 = new TF1("f2", "([0]/(x*x)) + ([1]/x) + [2] + 0.5", 0, 0.7) ;

void analyze_file_all_details(int argc, char *argv[])
{

  cout << "analyze_file_all_details" << endl ;

  int tracks_to_save = 4 ;

	if((argc != 4))
	{
		cout << "I need 3 arguments: low high outputindex." << endl ;
		exit(1) ;
	}
  
  f1->SetParameters( 0.137494,  1.26265 , 0.220616) ;
  f2->SetParameters(  1.64376, -3.07786 ,    5.361) ;
   
   string localdir(argv[0]) ;
   
   cout << localdir << endl ;
   
  TGraph2D *graph = new TGraph2D() ;
  graph->AddPoint( 1e2, 1e2, 3e2) ;
  graph->AddPoint(-1e2, 1e2, 3e2) ;
  graph->AddPoint( 1e2,-1e2, 3e2) ;
  graph->AddPoint(-1e2,-1e2, 3e2) ;
  graph->AddPoint( 1e2, 1e2,-3e2) ;
  graph->AddPoint(-1e2, 1e2,-3e2) ;
  graph->AddPoint( 1e2,-1e2,-3e2) ;
  graph->AddPoint(-1e2,-1e2,-3e2) ;
  
  TGraph2D *beam1 = new TGraph2D() ;
  beam1->AddPoint( 0, 0, 0) ;
  beam1->AddPoint( 0, 0, 3e2) ;

  TGraph2D *beam2 = new TGraph2D() ;
  beam2->AddPoint( 0, 0, 0) ;
  beam2->AddPoint( 0, 0, -3e2) ;
  
  beam1->SetLineColor(kBlue) ;
  beam2->SetLineColor(kRed) ;

	
	int lowlimit =  atoi(argv[1]) ;
	int highlimit = atoi(argv[2]) ;
	string outputindex(argv[3]) ;

   stringstream ss_summary ;

	string command_directory = ("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/condor_summary/dir_" + outputindex)  ;

 	TFile* myfile = TFile::Open((command_directory + "/summary.root").c_str(), "RECREATE", "Generic") ;

  TTree *mytree = new TTree("mytree", "mytree") ;

  UInt_t tree_detector_id = 0 ;
  Int_t tree_chip_id = 0 ;
  Float_t tree_momentum = 0 ;
  Float_t tree_pt = 0 ;
  Float_t tree_path_length = 0 ;
  Float_t tree_deltaE = 0 ;
  Float_t tree_eta = 0 ;
  Bool_t tree_kaon = false ;
  Bool_t tree_clean_RP_conf = false ;
  Bool_t tree_good_reference_for_kaon = false ;
  
  mytree->Branch("tree_detector_id", &tree_detector_id, "tree_detector_id/i") ;
  mytree->Branch("tree_chip_id", &tree_chip_id, "tree_chip_id/I") ;
  mytree->Branch("tree_momentum", &tree_momentum, "tree_momentum/F") ;
  mytree->Branch("tree_pt", &tree_pt, "tree_pt/F") ;
  mytree->Branch("tree_path_length", &tree_path_length, "tree_path_length/F") ;
  mytree->Branch("tree_deltaE", &tree_deltaE, "tree_deltaE/F") ;
  mytree->Branch("tree_eta", &tree_eta, "tree_eta/F") ;
  mytree->Branch("tree_kaon", &tree_kaon, "tree_kaon/O") ;
  mytree->Branch("tree_clean_RP_conf", &tree_clean_RP_conf, "tree_clean_RP_conf/O") ;
  mytree->Branch("tree_good_reference_for_kaon", &tree_good_reference_for_kaon, "tree_good_reference_for_kaon/O") ;
	
	myfile->Write() ;
	myfile->cd() ;

	string directory = "/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/cfg_files/for_analysis/TOTEM2/" ;

	ifstream detectors(filename_detectors_id) ;

	typedef vector<TH2F *> vector_th2f ;	
	map<uint32_t, vector_th2f> mymap_normalized_with_path ;	
  
 	vector<TGraph2D *> vector_graph2d_tracks ;	
	
  if(true)
  {  
  	uint32_t mydetid = 0 ;
	  int numberofhistos = 0 ;

	  while(detectors >> mydetid)
	  {
		  for(int kindex = 0 ; kindex < 16 ; ++kindex)
		  {
			  stringstream ss1 ;
			  stringstream ss2 ;

			  ss1 << mydetid ;
			  ss2 << kindex ;

			  string histname = "hist_" + ss1.str() + "_" + ss2.str() ;

//			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0., 5., 100, 0., 10.)) ; // for de/dx versus p
//			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0., 5., 100, 0., 1.)) ;

//			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0., 5., 100, 0., 4.0)) ; // for de/1 versus p
// 			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0., 5., 100, 0., 60.0)) ; // for 1/dx versus p
// 			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0, 4, 1000, 0, 1)) ; // for path length vs p

// 			  mymap_normalized_with_path[mydetid].push_back(new TH2F(histname.c_str(), histname.c_str(), 100, 0, 1, 100, -4, 4)) ; // for path length and eta
		  }

		  ++numberofhistos ;

		  if((numberofhistos % 1000) == 0)
		  {
			  cout << dec << "histo: " << numberofhistos << endl ;
		  }

	  }
  }
	
	detectors.close() ;

  int total_number_of_events = 0 ;
  int total_number_of_tracks = 0 ;
  int total_number_of_measurements = 0 ;
  
  int number_of_mytest = 0 ;
  int number_of_pidkaon = 0 ;
  int number_of_nOKPhiCut = 0 ;
  
  int number_of_tracks_per_event = 0 ;
  
  long int prev_event_number = 0 ;
  long int event_number = 0 ;

  TAnalysisEvent *event = NULL ;
  
  int mytest2_track_index = 0 ;
  
  myfile->cd() ;
  
  TH1F *hist_path_length_for_identified_kaons = new TH1F("hist_path_length_for_identified_kaons", "hist_path_length_for_identified_kaons", 400, 0, 1) ;
  TH1F *hist_detid_for_identified_kaons = new TH1F("hist_detid_for_identified_kaons", "hist_detid_for_identified_kaons", 400, 3e8, 5e8) ;

  TH1F *hist_path_length_for_identified_not_kaons = new TH1F("hist_path_length_for_identified_not_kaons", "hist_path_length_for_identified_not_kaons", 400, 0, 1) ;
  TH1F *hist_detid_for_identified_not_kaons = new TH1F("hist_detid_for_identified_not_kaons", "hist_detid_for_identified_not_kaons", 400, 3e8, 5e8) ;

  TH1F *hist_detid_for_identified_kaons_below_p_limit = new TH1F("hist_detid_for_identified_kaons_below_p_limit", "hist_detid_for_identified_kaons_below_p_limit", 400, 3e8, 5e8) ;
  TH1F *hist_detid_for_identified_not_kaons_below_p_limit = new TH1F("hist_detid_for_identified_not_kaons_below_p_limit", "hist_detid_for_identified_not_kaons_below_p_limit", 400, 3e8, 5e8) ;

  TH1F *hist_kaons_as_a_function_of_eta = new TH1F("hist_kaons_as_a_function_of_eta", "hist_kaons_as_a_function_of_eta", 400, -10, 10) ;
  TH1F *hist_not_kaons_as_a_function_of_eta = new TH1F("hist_not_kaons_as_a_function_of_eta", "hist_not_kaons_as_a_function_of_eta", 400, -10, 10) ;

  TH1F *hist_kaons_as_a_function_of_eta_detector_34_35 = new TH1F("hist_kaons_as_a_function_of_eta_detector_34_35", "hist_kaons_as_a_function_of_eta_detector_34_35", 400, -10, 10) ;
  TH1F *hist_kaons_as_a_function_of_eta_detector_not_34_35 = new TH1F("hist_kaons_as_a_function_of_eta_detector_not_34_35", "hist_kaons_as_a_function_of_eta_detector_not_34_35", 400, -10, 10) ;
  TH1F *hist_kaons_as_a_function_of_eta_detector_all = new TH1F("hist_kaons_as_a_function_of_eta_detector_all", "hist_kaons_as_a_function_of_eta_detector_all", 400, -10, 10) ;

  TH1F *hist_ref_as_a_function_of_eta_detector_34_35 = new TH1F("hist_ref_as_a_function_of_eta_detector_34_35", "hist_ref_as_a_function_of_eta_detector_34_35", 400, -10, 10) ;
  TH1F *hist_ref_as_a_function_of_eta_detector_not_34_35 = new TH1F("hist_ref_as_a_function_of_eta_detector_not_34_35", "hist_ref_as_a_function_of_eta_detector_not_34_35", 400, -10, 10) ;
  TH1F *hist_ref_as_a_function_of_eta_detector_all = new TH1F("hist_ref_as_a_function_of_eta_detector_all", "hist_ref_as_a_function_of_eta_detector_all", 400, -10, 10) ;

  TH1F *hist_kaon_as_a_function_of_eta_detector_30 = new TH1F("hist_kaons_as_a_function_of_eta_detector_30", "hist_kaons_as_a_function_of_eta_detector_30", 400, -10, 10) ;
  TH1F *hist_kaon_as_a_function_of_eta_detector_36 = new TH1F("hist_kaons_as_a_function_of_eta_detector_36", "hist_kaons_as_a_function_of_eta_detector_36", 400, -10, 10) ;

  TH1F *hist_ref_as_a_function_of_eta_detector_30 = new TH1F("hist_ref_as_a_function_of_eta_detector_30", "hist_ref_as_a_function_of_eta_detector_30", 400, -10, 10) ;
  TH1F *hist_ref_as_a_function_of_eta_detector_36 = new TH1F("hist_ref_as_a_function_of_eta_detector_36", "hist_ref_as_a_function_of_eta_detector_36", 400, -10, 10) ;

  TH1F *hist_path_length_for_identified_kaons_below_p_limit = new TH1F("hist_path_length_for_identified_kaons_below_p_limit", "hist_path_length_for_identified_kaons_below_p_limit", 400, 0, 1) ;
  TH1F *hist_path_length_for_identified_not_kaons_below_p_limit = new TH1F("hist_path_length_for_identified_not_kaons_below_p_limit", "hist_path_length_for_identified_not_kaons_below_p_limit", 400, 0, 1) ;

  TH1F *hist_number_of_strip_adc_for_identified_kaons_below_p_limit = new TH1F("hist_number_of_strip_adc_for_identified_kaons_below_p_limit", "hist_number_of_strip_adc_for_identified_kaons_below_p_limit", 100, 0, 100) ;
  TH1F *hist_number_of_strip_adc_for_identified_not_kaons_below_p_limit = new TH1F("hist_number_of_strip_adc_for_identified_not_kaons_below_p_limit", "hist_number_of_strip_adc_for_identified_not_kaons_below_p_limit", 100, 0, 100) ;

  TH1F *hist_number_of_pixel_adc_for_identified_kaons_below_p_limit = new TH1F("hist_number_of_pixel_adc_for_identified_kaons_below_p_limit", "hist_number_of_pixel_adc_for_identified_kaons_below_p_limit", 100, 0, 100) ;
  TH1F *hist_number_of_pixel_adc_for_identified_not_kaons_below_p_limit = new TH1F("hist_number_of_pixel_adc_for_identified_not_kaons_below_p_limit", "hist_number_of_pixel_adc_for_identified_not_kaons_below_p_limit", 100, 0, 100) ;

  TH1F *hist_path_length_303042564 = new TH1F("hist_path_length_303042564", "hist_path_length_303042564", 400, 0, 1) ;
  TH1F *hist_path_length_303042572 = new TH1F("hist_path_length_303042572", "hist_path_length_303042572", 400, 0, 1) ;
  TH1F *hist_path_length_369120277 = new TH1F("hist_path_length_369120277", "hist_path_length_369120277", 400, 0, 1) ;
  TH1F *hist_path_length_369120286 = new TH1F("hist_path_length_369120286", "hist_path_length_369120286", 400, 0, 1) ;
  
  TH1F *hist_path_length_344201220 = new TH1F("hist_path_length_344201220", "hist_path_length_344201220", 400, 0, 1) ;
  TH1F *hist_path_length_344204292 = new TH1F("hist_path_length_344204292", "hist_path_length_344204292", 400, 0, 1) ;
  TH1F *hist_path_length_402664082 = new TH1F("hist_path_length_402664082", "hist_path_length_402664082", 400, 0, 1) ;
  TH1F *hist_path_length_470044965 = new TH1F("hist_path_length_470044965", "hist_path_length_470044965", 400, 0, 1) ;
  
  TH2F *correlation_detid_eta = new TH2F("correlation_detid_eta", "correlation_detid_eta", 1000, 3e8, 3.1e8, 100, 0,1) ;

  TH2F *correlation_path_length_and_pt = new TH2F("correlation_path_length_and_pt", "correlation_path_length_and_pt", 100, 0, 1, 100, 0, 4) ;
  TH2F *correlation_path_length_and_p = new TH2F("correlation_path_length_and_p", "correlation_path_length_and_p", 100, 0, 1, 100, 0, 4) ;
  TH2F *correlation_path_length_and_eta = new TH2F("correlation_path_length_and_eta", "correlation_path_length_and_eta", 100, 0, 1, 100, -4, 4) ;

  TH2F *corr_path_length = new TH2F("corr_path_length", "corr_path_length", 100, 0, 1, 100, 0, 1) ;

  TH2F *corr_momentum_vs_adc_for_369120286 = new TH2F("corr_momentum_vs_adc_for_369120286", "corr_momentum_vs_adc_for_369120286", 100, 0, 4, 300, 0, 300) ;
  TH2F *corr_momentum_vs_adc_for_369120277 = new TH2F("corr_momentum_vs_adc_for_369120277", "corr_momentum_vs_adc_for_369120277", 100, 0, 4, 300, 0, 300) ;

  TH2F *corr_momentum_vs_adc_for_369120286_if_kaon = new TH2F("corr_momentum_vs_adc_for_369120286_if_kaon", "corr_momentum_vs_adc_for_369120286_if_kaon", 100, 0, 4, 300, 0, 300) ;
  TH2F *corr_momentum_vs_adc_for_369120277_if_kaon = new TH2F("corr_momentum_vs_adc_for_369120277_if_kaon", "corr_momentum_vs_adc_for_369120277_if_kaon", 100, 0, 4, 300, 0, 300) ;
  
  // ifstream detectors_to_test_for_adc("data/detectors_to_test_for_adc.txt") ;
  ifstream detectors_to_test_for_adc("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/data/detectors_to_test_for_adc2.txt") ;
  
  UInt_t detector_id_for_adc = 0 ;
  
  map<UInt_t, map<string, TH2F *>>  map_detector_ids_to_histogram ;

  while(detectors_to_test_for_adc >> detector_id_for_adc)
  {
    stringstream ss ;
    ss << detector_id_for_adc ;

    string histo_name = "id_" + ss.str() + "_momentum_vs_adc" ;

    map_detector_ids_to_histogram[detector_id_for_adc]["momentum_vs_adc"] = new TH2F(histo_name.c_str(), histo_name.c_str(), 100, 0, 4, 300, 0, 300) ;
  }
  
  TH1F *detector_if_see_kaons = new TH1F("detector_if_see_kaons", "detector_if_see_kaons", 10, 0, 10) ;
  
  const int amplitude_distribution_plot_limit = 100 ;
  vector<TH1F *> amplitude_distribution_369120286 ;
  vector<TH1F *> amplitude_distribution_369120277 ;
  
  for(int i = 0 ; i < amplitude_distribution_plot_limit ; ++i)
  {
    stringstream ss ;
    
    ss << i ;

    amplitude_distribution_369120286.push_back(new TH1F(("amplitude_distribution_369120286_" + ss.str()).c_str(), ("amplitude_distribution_369120286_" + ss.str()).c_str(), 15, 0, 15)) ;
    amplitude_distribution_369120277.push_back(new TH1F(("amplitude_distribution_369120277_" + ss.str()).c_str(), ("amplitude_distribution_369120277_" + ss.str()).c_str(), 15, 0, 15)) ;
  }
  
	bool save_track_study = false ;
	bool test_proton = true ;
	bool test_phi = false ;
  bool test_amplitude_distribution = true ;
  
  vector<int> logs_with_phi ;
  
  logs_with_phi.push_back(747) ;
  logs_with_phi.push_back(748) ;
  logs_with_phi.push_back(753) ;
  logs_with_phi.push_back(754) ;
  logs_with_phi.push_back(755) ;
  logs_with_phi.push_back(756) ;
  logs_with_phi.push_back(757) ;
  logs_with_phi.push_back(759) ;
  logs_with_phi.push_back(760) ;
  logs_with_phi.push_back(762) ;
  logs_with_phi.push_back(763) ;
  logs_with_phi.push_back(765) ;
  logs_with_phi.push_back(766) ;
  logs_with_phi.push_back(769) ;
  logs_with_phi.push_back(772) ;
  logs_with_phi.push_back(773) ;
  logs_with_phi.push_back(774) ;
  logs_with_phi.push_back(776) ;
  logs_with_phi.push_back(779) ;
  logs_with_phi.push_back(780) ;
  logs_with_phi.push_back(781) ;
  logs_with_phi.push_back(782) ;
  
  int index_369120286 = 0 ;
  int index_369120277 = 0 ;
  
  int processed_event = 0 ;
  
  bool print_debug = false ;
  bool useful_debug = false ;

  for(int i = 0 ; i < 1500 ; ++i)
	{
		if((i < lowlimit) || (i > highlimit)) continue ;   
    
    if(test_phi)
    {
      vector<int>::iterator myit2 = find(logs_with_phi.begin(), logs_with_phi.end(), i) ;
      
      if(myit2 == logs_with_phi.end())
      {
        cout << "Skipped log file " << i << endl ;
        // continue ;
      }
    }

		stringstream ss ;
		
		if((i % 100) == 0)
		{
			// cout << dec << i << endl ;
		}
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}

		// string filename = directory + "dir_" + prefix + ss.str() + "/log";
		// string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/log_" + prefix + ss.str() ;
		// string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/log_" + prefix + ss.str() ;
		string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/with_protons_and_adc/log_" + prefix + ss.str() ;

		// string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/with_protons/log_" + prefix + ss.str() ;
		
		ifstream logfile(filename.c_str()) ;
    
		if(logfile.is_open())
		{
      bool check_if_file_has_momentum_after_pidkaon_variable = check_if_file_has_momentum_after_pidkaon(filename) ;

			cout << filename << endl ;
			string word = "" ;

      // system("ps v") ;

			while(logfile >> word)
			{

        if(print_debug) cout << " " << word ; 

				if(word.compare("Event_starts:") == 0)
				{
          
          if(event != NULL)
          {

            // cout << event->GetTracks()->size() << endl ;
            if(useful_debug) cout << endl << "Processing event:" << event->GetEventNumber() << " " << (processed_event++) << endl ;
            
            // cout << prev_event_number << " " << event->GetEventNumber() << endl ;
            
             vector<TAnalysisTrack *> *mytracks = event->GetTracks() ;
             
            // cout  << "anothertest: " << (*mytracks)[0]->GetMomentum() << " " << (*mytracks)[1]->GetMomentum() << " "  << (*mytracks)[2]->GetMomentum() << " "  << (*mytracks)[3]->GetMomentum() << " " ;
            
            if(!test_proton || (test_proton && event->GetCleanRPConf())) 
            for(vector<TAnalysisTrack *>::iterator it = mytracks->begin() ; it != mytracks->end() ; ++it)
            {
            
                // cout << event->GetCleanRPConf() << endl ;

                if(test_phi)
                {
                  // cout << endl ;
                  if(event->WasPhiCutOk())
                  {/*
                    cout << "okphi" << endl ;

                    if((*it)->IsSaturatedMeasurement())
                      cout << "  oksat" << endl ;

                    if((*it)->GetMomentum() < 0.6)
                      cout << "  okmom" << endl ;

                    if(((*it)->IsSaturatedMeasurement()) && ((*it)->GetMomentum() < 0.6))
                      cout << "  okmomandsat" << endl ;*/

                  }
                  else
                  {
                    cout << "notokphi" << endl ;

                    if((*it)->IsSaturatedMeasurement())
                      cout << "  oksat" << endl ;

                    if((*it)->GetMomentum() < 0.6)
                      cout << "  okmom" << endl ;

                    if(((*it)->IsSaturatedMeasurement()) && ((*it)->GetMomentum() < 0.6))
                      cout << "  okmomandsat" << endl ;
                  }
                }
                
                vector<TMeasurement *> *measurements = (*it)->GetMeasurements() ;

                if((*it)->GetMomentum() < 0.6)
                {
                  if((*it)->isKaon())
                  {
                    hist_kaons_as_a_function_of_eta->Fill((*it)->GetEta()) ;
                  }
                  else
                  {
                    hist_not_kaons_as_a_function_of_eta->Fill((*it)->GetEta()) ;
                  }
                }
                
                // if((*it)->isKaon()) cout << endl ;

                vector<pair<pair<uint32_t, int>, vector<double> *>> *mypointer = (*it)->GetPointerStripAdcMeasurements() ;

                for(int i = 0 ; i < mypointer->size() ; ++i)
                {

                  map<UInt_t, map<string, TH2F *>> ::iterator it_new = map_detector_ids_to_histogram.find((*mypointer)[i].first.first) ;

                  if(it_new != map_detector_ids_to_histogram.end())
                  {
                    // cout << "okfind" << endl ;
                    
                    double dedx = 0 ;
                    int nnumber_of_measurements = 0 ;

                    for(vector<TMeasurement *>::iterator it2 = measurements->begin() ; it2 != measurements->end() ; ++it2)
                    {
                      if((*it2)->GetType() == TMeasurement::measurement_type_dedx_path_length)                    
                      {
                        dedx += ((*it2)->GetdE() / (*it2)->GetPathLength()) ;
                        ++nnumber_of_measurements ;
                      }
                    }
                    
                    dedx = (dedx / nnumber_of_measurements) ;

                    for(int jk = 0 ; jk < (*mypointer)[i].second->size() ; ++jk)
                    {
                      double ymin = f1->Eval((*it)->GetMomentum()) ;
                      double ymax = f2->Eval((*it)->GetMomentum()) ;

                      // cout << "veryok " << ymin << " " << ymax << " " << dedx << endl ;

                      if((dedx > ymin) && (dedx < ymax))
                      {
                        // cout << "veryok" << endl ;
                        map_detector_ids_to_histogram[(*mypointer)[i].first.first]["momentum_vs_adc"]->Fill((*it)->GetMomentum(), (*(*mypointer)[i].second)[jk]) ;
                      }
                    }
                  }

                  if((*mypointer)[i].first.first == 369120286)
                  {
                    bool plot_filled = false ;
                  
                    for(int jk = 0 ; jk < (*mypointer)[i].second->size() ; ++jk)
                    {
                      corr_momentum_vs_adc_for_369120286->Fill((*it)->GetMomentum(), (*(*mypointer)[i].second)[jk]) ;
                      if((*it)->isKaon()) corr_momentum_vs_adc_for_369120286_if_kaon->Fill((*it)->GetMomentum(), (*(*mypointer)[i].second)[jk]) ;
                      
                      if(test_amplitude_distribution && (index_369120286 < amplitude_distribution_plot_limit))
                      {
                        plot_filled = true ;
                        amplitude_distribution_369120286[index_369120286]->SetBinContent(jk, (*(*mypointer)[i].second)[jk]) ;
                      }
                    }

                    if(plot_filled) ++index_369120286 ;

                  }

                  if((*mypointer)[i].first.first == 369120277)
                  {
                    bool plot_filled = false ;

                    for(int jk = 0 ; jk < (*mypointer)[i].second->size() ; ++jk)
                    {
                      corr_momentum_vs_adc_for_369120277->Fill((*it)->GetMomentum(), (*(*mypointer)[i].second)[jk]) ;
                      if((*it)->isKaon()) corr_momentum_vs_adc_for_369120277_if_kaon->Fill((*it)->GetMomentum(), (*(*mypointer)[i].second)[jk]) ;

                      if(test_amplitude_distribution && (index_369120277 < amplitude_distribution_plot_limit))
                      {
                        plot_filled = true ;
                        amplitude_distribution_369120277[index_369120277]->SetBinContent(jk, (*(*mypointer)[i].second)[jk]) ;
                      }
                    }

                    if(plot_filled) ++index_369120277 ;

                  }
                }
                
                if((*it)->GetMomentum() < 0.6)
                {
                  vector<double> vector_of_strip_adc_measurements = (*it)->GetNumberOfStripAdcMeasurements() ;
                  

                  
                  for(int i = 0 ; i < vector_of_strip_adc_measurements.size() ; ++i)
                  {
                    if((*it)->isKaon()) hist_number_of_strip_adc_for_identified_kaons_below_p_limit->Fill(vector_of_strip_adc_measurements[i]) ;
                    hist_number_of_strip_adc_for_identified_not_kaons_below_p_limit->Fill(vector_of_strip_adc_measurements[i]) ;
                  }

                  vector<double> vector_of_pixel_adc_measurements = (*it)->GetNumberOfPixelAdcMeasurements() ;

                  for(int i = 0 ; i < vector_of_pixel_adc_measurements.size() ; ++i)
                  {
                    if((*it)->isKaon()) hist_number_of_pixel_adc_for_identified_kaons_below_p_limit->Fill(vector_of_pixel_adc_measurements[i]) ;
                    hist_number_of_pixel_adc_for_identified_not_kaons_below_p_limit->Fill(vector_of_pixel_adc_measurements[i]) ;
                  }
                }
                
                bool detector_34 = false ;
                bool detector_35 = false ;
                bool detector_30 = false ;
                bool detector_36 = false ;
                
                for(vector<TMeasurement *>::iterator it2 = measurements->begin() ; it2 != measurements->end() ; ++it2)
                {
                
                
               
                  // if((*it2)->GetType() == TMeasurement::measurement_type_dedx_path_length) if(((*it2)->GetMomentum() < 0.6) && ((*it2)->GetMomentum() > 0.3)) cout << "mykaondetid_check hist_" << (*it2)->GetDetectorID() << "_" << (*it2)->GetChipID() << endl ;

                  if((*it2)->GetType() == TMeasurement::measurement_type_dedx_path_length)
                  {

                    stringstream detid_string ;
                    detid_string << (*it2)->GetDetectorID() ;
                  
                    if(detid_string.str().substr(0, 2).compare("34") == 0) detector_34 = true ;
                    if(detid_string.str().substr(0, 2).compare("35") == 0) detector_35 = true ;

                    if(detid_string.str().substr(0, 2).compare("30") == 0) detector_30 = true ;
                    if(detid_string.str().substr(0, 2).compare("36") == 0) detector_36 = true ;
                  
                    // if((*it)->isKaon())
                    
                    if((*it)->isKaon())
                    {
                      // cout <<  "kaon " << (*it2)->GetDetectorID() << endl ;
                    }
                    
                    if((*it)->isKaon())
                    {
                      // cout << "hello " << (*it2)->GetMomentum() << endl ;
                      if((*it2)->GetDetectorID() == 369120277) detector_if_see_kaons->Fill(0) ;
                      if((*it2)->GetDetectorID() == 303042572) detector_if_see_kaons->Fill(1) ;
                      if((*it2)->GetDetectorID() == 369120286) detector_if_see_kaons->Fill(2) ;
                      if((*it2)->GetDetectorID() == 303042564) detector_if_see_kaons->Fill(3) ;
                    }

                    // cout << "test " << (*it2)->GetDetectorID() << endl ;
						        // if(mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()] != NULL) mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()]->Fill((*it2)->GetMomentum(), ((*it2)->GetdE() / (*it2)->GetPathLength())) ;					 
						        // if(mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()] != NULL) mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()]->Fill((*it2)->GetMomentum(), ((*it2)->GetPathLength())) ;					 
                    //if(mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()] != NULL) mymap_normalized_with_path[(*it2)->GetDetectorID()][(*it2)->GetChipID()]->Fill((*it2)->GetPathLength(), ((*it)->GetEta())) ;					 
                    
                    if(((*it)->GetNumberOfPixelAdcMeasurements().size()) <= 2)
                    {
                      tree_detector_id = (*it2)->GetDetectorID() ;
                      tree_chip_id = (*it2)->GetChipID() ;
                      tree_momentum = (*it2)->GetMomentum() ;
                      tree_pt = (*it)->GetPt() ;
                      tree_path_length = (*it2)->GetPathLength() ;
                      tree_deltaE = (*it2)->GetdE() ;
                      tree_eta = (*it)->GetEta() ;
                      tree_kaon = (*it)->isKaon() ;
                      tree_clean_RP_conf = event->GetCleanRPConf() ;
                      tree_good_reference_for_kaon = event->GetGoodReferenceForKaon() ;

                      // mytree->Fill() ;
                    }
                    
                    // cout << "test2 " << (*it2)->GetDetectorID() << endl ;

                    stringstream ss ;
                    
                    ss << (*it2)->GetDetectorID() ; 
                    // if(((*it2)->GetMomentum() < 0.6) && ((*it2)->GetMomentum() > 0.3)) cout << "mykaondetid_check2 " << ss.str().substr(0,2)  << " " << endl ;                  

                    // if((*it2)->GetPathLength() > 0.04) 
                    // if(ss.str().substr(0,2).compare("30") == 0) 
						  {
						  		// cout << "for_path_length_and_pt " << (*it)->GetPt() << endl ;
								
								correlation_path_length_and_pt->Fill((*it2)->GetPathLength(), (*it)->GetPt()) ;
								correlation_path_length_and_p->Fill((*it2)->GetPathLength(), (*it)->GetP()) ;
								correlation_path_length_and_eta->Fill((*it2)->GetPathLength(), (*it)->GetEta()) ;
						  
							  for(vector<TMeasurement *>::iterator it3 = measurements->begin() ; it3 != measurements->end() ; ++it3)
							  {
							  
	      	              stringstream ss2 ;
                    
		                    ss2 << (*it3)->GetDetectorID() ; 
							  
							  	if((*it3)->GetType() == TMeasurement::measurement_type_dedx_path_length)
								{
		                    // if(ss2.str().substr(0,2).compare("30") == 0) 								
								  {
									corr_path_length->Fill((*it2)->GetPathLength(), (*it3)->GetPathLength()) ;
									}
								}

							  	if((*it3)->GetType() == TMeasurement::measurement_type_coordinates)
								{
								  	if(((*it2)->GetDetectorID() == (*it3)->GetDetectorID()) && ((*it2)->GetChipID() == (*it3)->GetChipID()))
									{
										// cout << "found z " << (*it3)->GetZ() << endl ;
									}
								}
							  }
						  }
                  }

                  if((*it)->isKaon())
                  {
                    // cout << "mykaondetid hist_" << (*it2)->GetDetectorID() << "_" << (*it2)->GetChipID() << endl ;

                 
                    if((*it2)->GetType() == TMeasurement::measurement_type_dedx_path_length)
                    {
                      hist_path_length_for_identified_kaons->Fill((*it2)->GetPathLength()) ;
                      hist_detid_for_identified_kaons->Fill((*it2)->GetDetectorID()) ;
							 
							 // cout << "kaon test: " << event->GetEventNumber() << " " << (*it2)->GetPathLength() << endl ;
                      
                      // if(((*it2)->GetMomentum() < 0.6) && ((*it2)->GetMomentum() > 0.3)) cout << "mykaondetid_mom_check hist_" << (*it2)->GetDetectorID() << "_" << (*it2)->GetChipID() << endl ;
                      
                      if((*it2)->GetMomentum() < 0.6)
                      {
                        hist_detid_for_identified_kaons_below_p_limit->Fill((*it2)->GetDetectorID()) ;
                        hist_path_length_for_identified_kaons_below_p_limit->Fill((*it2)->GetPathLength()) ;
                      }
                    }
                  }
                  else
                  {
                    if((*it2)->GetType() == TMeasurement::measurement_type_dedx_path_length)
                    {
                      hist_path_length_for_identified_not_kaons->Fill((*it2)->GetPathLength()) ;
                      hist_detid_for_identified_not_kaons->Fill((*it2)->GetDetectorID()) ;

                      if((*it2)->GetMomentum() < 0.6)
                      {
                        hist_detid_for_identified_not_kaons_below_p_limit->Fill((*it2)->GetDetectorID()) ;
                        hist_path_length_for_identified_not_kaons_below_p_limit->Fill((*it2)->GetPathLength()) ;
                      }
                    }
                  }
                  
                  
                  if((*it2)->GetMomentum() < 0.6)
                  {
                    if((*it2)->GetDetectorID() == 303042564) hist_path_length_303042564->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 303042572) hist_path_length_303042572->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 369120277) hist_path_length_369120277->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 369120286) hist_path_length_369120286->Fill((*it2)->GetPathLength()) ;

                    if((*it2)->GetDetectorID() == 344201220) hist_path_length_344201220->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 344204292) hist_path_length_344204292->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 402664082) hist_path_length_402664082->Fill((*it2)->GetPathLength()) ;
                    if((*it2)->GetDetectorID() == 470044965) hist_path_length_470044965->Fill((*it2)->GetPathLength()) ;
                  }
                  
                  correlation_detid_eta->Fill((*it2)->GetDetectorID(), (*it2)->GetPathLength()) ;
                  
                }

                if(event->GetGoodReferenceForKaon() && ((*it)->GetMomentum() < 0.7))
                {

                  if(detector_30) hist_ref_as_a_function_of_eta_detector_30->Fill((*it)->GetEta()) ;
                  if(detector_36) hist_ref_as_a_function_of_eta_detector_36->Fill((*it)->GetEta()) ;

                  if(detector_34 || detector_35) hist_ref_as_a_function_of_eta_detector_34_35->Fill((*it)->GetEta()) ;
                  if(!(detector_34 || detector_35)) hist_ref_as_a_function_of_eta_detector_not_34_35->Fill((*it)->GetEta()) ;
                  hist_ref_as_a_function_of_eta_detector_all->Fill((*it)->GetEta()) ;

                  if((*it)->isKaon())
                  {
                    if(detector_34 || detector_35) hist_kaons_as_a_function_of_eta_detector_34_35->Fill((*it)->GetEta()) ;
                    if(!(detector_34 || detector_35)) hist_kaons_as_a_function_of_eta_detector_not_34_35->Fill((*it)->GetEta()) ;
                    hist_kaons_as_a_function_of_eta_detector_all->Fill((*it)->GetEta()) ;
                    
                    if(detector_30) hist_kaon_as_a_function_of_eta_detector_30->Fill((*it)->GetEta()) ;
                    if(detector_36) hist_kaon_as_a_function_of_eta_detector_36->Fill((*it)->GetEta()) ;
                    
                  }
                }

            }
            
            if(print_debug) cout << "End og track analysis" << endl ;

            // cout << endl ;
          
            delete event ;
          }

          if(print_debug) cout << "end track analysis2" << endl ;
          
          prev_event_number = event_number ;
          
          logfile >> event_number ;
			    if(useful_debug) cout << "ok:" << event_number << endl ;

          event = new TAnalysisEvent(event_number) ;
          mytest2_track_index = 0 ;

          // cout << endl << "event " << event_number << " total_number_of_events: " << total_number_of_events << endl ;
          // if((total_number_of_events % 1000) == 0) cout << "." << endl ;
          
          ++total_number_of_events ;
          
          if(number_of_tracks_per_event == 4)
          {
              
              if(save_track_study)
              {
                TCanvas c ;
                
                graph->Draw() ;
                beam1->Draw("same LINE") ;
                beam2->Draw("same LINE") ;
                
                beam1->SetLineStyle(kDashed) ;
                beam2->SetLineStyle(kDashed) ;
                
                int colorindex = 0 ;

                for(vector<TGraph2D *>::iterator it = vector_graph2d_tracks.begin() ; it != vector_graph2d_tracks.end() ; ++it)
                {
                  TGraph2D *copy = ((TGraph2D *)(*it)->Clone("myclone")) ;
                  (*it)->Draw("same LINE") ;
                  copy->Draw("same p0") ;
                  
                  EColor mycolors[] = {kRed, kBlue, kGreen, kMagenta} ;

                  (*it)->SetLineColor(mycolors[colorindex]) ;
                  copy->SetLineColor(mycolors[colorindex]) ;
                  
                  ++colorindex ;
                }
                
                if(tracks_to_save == total_number_of_events)
                {
                  c.SaveAs("tracks.root") ;
                  exit(0) ;
                }
              
                for(vector<TGraph2D *>::iterator it = vector_graph2d_tracks.begin() ; it != vector_graph2d_tracks.end() ; ++it)
                {
                  delete (*it) ;
                }
              
                vector_graph2d_tracks.clear() ;
              }
              
          }
          else
          {
            // cout << "Error: wrong number of tracks! " << number_of_tracks_per_event << " " << event_number << endl ;
          }
          
          // Process the 4 track of the event [start]

          // Process the 4 track of the event [end]
          
          number_of_tracks_per_event = 0 ;
          
        }
				else if(word.compare("Event_ends:") == 0)
        {
          long int end_event_number = 0 ;
        
          logfile >> end_event_number ;
          if(useful_debug) cout << "End of event:" << end_event_number ; 
        }
				else if(word.compare("Track") == 0)
				{
          // cout << "Track" << endl ;
          ++total_number_of_tracks ;
          
          logfile >> word ;
          
          TGraph2D *pointer = NULL ;

          if(save_track_study)
          {
            pointer = new TGraph2D() ;
            vector_graph2d_tracks.push_back(pointer) ;
          }

   				if(word.compare("begin") == 0)
          {
            // cout << " begin" << endl ;
            
            ++number_of_tracks_per_event ;
  
            bool finished = false ;

            int number_of_measurements = 0 ;
            
            TAnalysisTrack *track = new TAnalysisTrack() ;
            
            double charge, eta, phi, pt, momentum ;

            logfile >> charge >> eta >> phi >> pt >> momentum ;
            
            track->SetTrackMomentumEtaPhi(charge, eta, phi, pt, momentum) ;

      			while(!finished)
            {
                logfile >> word ;
  
          
                if(word.compare("Measurement_global_coordinates:") == 0)
                {
                  // cout << "Measurement_global_coordinates" << endl ;
                  
                  ++number_of_measurements ;
                  ++total_number_of_measurements ;

                  uint32_t detid = 0 ;
                  int chipid = 0 ;
                  string detector_type ;
                  double momentum = 0 ;
                  double dedx = 0 ;
                  double thickness = 0 ;
                  double measx = 0 ;
                  double measy = 0 ;
                  double measz = 0 ;

                  logfile >> detid >> chipid >> detector_type >> momentum >> dedx >> thickness >> measx >> measy >> measz ;
                  
                  TMeasurement *measurement = new TMeasurement(detid, chipid, detector_type, momentum, dedx, thickness, measx, measy, measz) ;
                  
                  track->AddMeasurement(measurement) ;
                  
                  if(save_track_study) pointer->AddPoint(measx, measy, measz) ;
                  
                  // cout << "anothertest1 " <<  detid << " " <<  chipid << " " <<  detector_type << " " <<  momentum << " " <<  dedx << " " <<  thickness << " " <<  measx << " " <<  measy << " " <<  measz ;
                }
                else if(word.compare("Detid:") == 0)
                {
                  // cout << "Detid" << endl ;

                  uint32_t detid = 0 ;
                  int chipid = 0 ;
                  string detector_type ;
                  double momentum = 0 ;
                  double dedx = 0 ;
                  double thickness = 0 ;
                  double path_length = 0 ;
                  string saturated_text = "" ;

                  logfile >> detid >> word >> chipid >> word >>  momentum >> word >> dedx >> word >> thickness >> word >> path_length >> saturated_text ;
                  
                  // cout << saturated_text << endl ;

                  TMeasurement *measurement = new TMeasurement(detid, chipid, detector_type, momentum, dedx, thickness, path_length) ;

                  track->AddMeasurement(measurement) ;

                  if(saturated_text.compare("saturated1") == 0)
                  {
                    track->SetHasSaturatedMeasurement() ;
                  }

                  // cout << "anothertest2 " <<  detid << " " <<  word << " " <<  chipid << " " <<  word << " " <<   momentum << " " <<  word << " " <<  dedx << " " <<  word << " " <<  thickness << " " <<  word << " " <<  path_length ;
                  
         					if((chipid > -1) && (chipid < 16))
            			{
        	          // if(mymap_normalized_with_path[detid][chipid] != NULL) mymap_normalized_with_path[detid][chipid]->Fill(momentum, (dedx / path_length)) ;
        	          // if(mymap_normalized_with_path[detid][chipid] != NULL) mymap_normalized_with_path[detid][chipid]->Fill(momentum, path_length) ;
        	          // if(mymap_normalized_with_path[detid][chipid] != NULL) mymap_normalized_with_path[detid][chipid]->Fill(momentum, (dedx / 1.0 )) ;
        	          // if(mymap_normalized_with_path[detid][chipid] != NULL) mymap_normalized_with_path[detid][chipid]->Fill(momentum, (1.0 / path_length)) ;
          				}

                }
        				else if(word.compare("Test") == 0)
                {
                  // cout << "Test of ADC" << endl ;
                  logfile >> word ;
                  
                  string pixel_or_strip ;
                  string chiptext ;
                  string pathtext ;
                  string adctext ;

 					        long int detid = 0 ;
                  int chipid = 0 ;
                  double path ;
                  int number_of_adc_values ;

                  logfile >> pixel_or_strip >> detid >> chiptext >> chipid >> pathtext >> path >> adctext >> number_of_adc_values ;
                  // cout << adctext << endl ;

                  if(adctext.compare("adc_values") == 0)
                  {
                    // cout << "detid " << detid << " chipd " << chipid << " pixel_or_strip " << pixel_or_strip << " path_length " << path << " nadc " << number_of_adc_values << endl ;
                    
                    vector<double> *adc_values = new vector<double> ;
                    
                    for(int j = 0 ; j < number_of_adc_values ; ++j)
                    {
                        double adc_value ;

                       logfile >> adc_value ;
                       
                       adc_values->push_back(adc_value) ;
                       
                       if(adc_value > 254) track->SetHasSaturatedMeasurement() ;

                       // cout << adc_value << " " ;
                    }

                    if(pixel_or_strip.compare("strip") == 0)
                    {
                      track->AddStripADCMeasurements(adc_values, detid, chipid) ;
                    }
                    if(pixel_or_strip.compare("pixel") == 0)
                    {
                      track->AddPixelADCMeasurements(adc_values, detid, chipid) ;
                    }

                  }
                }
                else if(word.compare("Track") == 0)
                {
                  logfile >> word ;
              
                  if(word.compare("end") == 0)
                  {
                    finished = true ;
                    event->AddTrack(track) ;
                  
                    // cout << "Catched Track end" << endl ;
                    // cout << " " << number_of_measurements << endl ;
  
                  }
                  else
                  {
                    // cout << "Error: end of track not found!" << endl ;
                  }
                }
                else
                {
                  // cout << "Error: begin of track not found!" << endl ;
                }
            }
          }
          else
          {
            // cout << "Error: begin of track not found!" << endl ;
          }
        }
				else if(word.compare("eventprintout") == 0)
        {
          long int event_number = 0 ;
          int run_number = 0 ;
          int lumi_number = 0 ;
          int track_index ;
          double extra_momentum = 0 ;
          
          logfile  >> word ;
          
          if(word.compare("pidkaon") == 0)
          {
            ++number_of_pidkaon ;

            logfile  >> word >> run_number >> word >> lumi_number >> word >> event_number >> track_index >> extra_momentum ;

            if(event->GetEventNumber() == event_number)
            {
              if(event->getNumberOfTracks() == 4) event->SetTrackAsKaon(track_index, extra_momentum, true) ;
            }
            else
            {
              cout << "Error: event number mismatch (2)!" << " " << event->GetEventNumber() << " " << event_number << endl ;
            }
          }
          else if(word.compare("nOKPhiCut") == 0)
          {
            ++number_of_nOKPhiCut ;

            logfile  >> word >> run_number >> word >> lumi_number >> word >> event_number  ; 

            if(event->GetEventNumber() == event_number)
            {
              event->PhiCutWasOk() ;
            }
            else
            {
              cout << "Error: event number mismatch (3)!" << " " << event->GetEventNumber() << " " << event_number << endl ;
            }
            
         }
         else if(word.compare("reference1") == 0)
         {
            logfile  >> word >> run_number >> word >> lumi_number >> word >> event_number >> track_index >> extra_momentum ;
            
            if(print_debug) cout << "test_reference1" << track_index << " " << extra_momentum << endl ;

            if(event->GetEventNumber() == event_number)
            {
              // event->PhiCutWasOk() ;
            }
            else
            {
              cout << "Error: event number mismatch (3)!" << " " << event->GetEventNumber() << " " << event_number << endl ;
            }
         }
         else if(word.compare("reference2") == 0)
         {
            logfile  >> word >> run_number >> word >> lumi_number >> word >> event_number >> track_index >> extra_momentum ;

            if(print_debug) cout << "test_reference2" << track_index << " " << extra_momentum << endl ;

            if(event->GetEventNumber() == event_number)
            {
              event->GoodReferenceForKaon() ;
            }
            else
            {
              cout << "Error: event number mismatch (3)!" << " " << event->GetEventNumber() << " " << event_number << endl ;
            }
         }
         else
         {
            cout << "Error: unknown keyword: " << word << endl ;
         }

        }
        else if(word.compare("Event_nConfClean_ok") == 0)
        {
          event->SetCleanRPConf() ;
			 	// cout << "Read: SetCleanRPConf" << endl ;
        }
        else
        {
           if(print_debug) cout << "Error: unknown keyword! " << word << endl ;
        }
      }
    }
    else
    {
      // cout << "Error: file cannot be opened!" << filename << endl ;
    }
  }
  
  
  myfile->cd() ;
	myfile->Write() ;
  
  cout << "total_number_of_events: " << total_number_of_events << endl ;
  cout << "total_number_of_tracks: " << total_number_of_tracks << endl ;
  cout << "total_number_of_measurements: " << total_number_of_measurements << endl ;

  cout << "test1: " << number_of_mytest << endl ;
  cout << "test2: " <<  number_of_pidkaon << endl ;
  cout << "test3: " <<  number_of_nOKPhiCut << endl ;
  
}

void analyze_adc(int argc, char *argv[])
{
	if((argc != 4))
	{
		cout << "I need 3 arguments: low high outputindex." << endl ;
		exit(1) ;
	}
  
  TH2D *histogram_path_length_vs_adc_pixel = new TH2D("histogram_path_length_vs_adc_pixel", "histogram_path_length_vs_adc_pixel", 200, 0, 0.5, 100, 0, 100) ;
  TH2D *histogram_path_length_vs_adc_strip = new TH2D("histogram_path_length_vs_adc_strip", "histogram_path_length_vs_adc_strip", 200, 0, 0.5, 100, 0, 100) ;
  TH2D *histogram_path_length_vs_adc_strip_36 = new TH2D("histogram_path_length_vs_adc_strip_36", "histogram_path_length_vs_adc_strip_36", 200, 0, 0.5, 100, 0, 100) ;

  TH2D *hist_path_length_vs_adc_303042564 = new TH2D("hist_path_length_vs_adc_303042564", "hist_path_length_vs_adc_303042564", 200, 0, 0.5, 300, 0, 300) ;
  TH2D *hist_path_length_vs_adc_303042572 = new TH2D("hist_path_length_vs_adc_303042572", "hist_path_length_vs_adc_303042572", 200, 0, 0.5, 300, 0, 300) ;

  TH2D *hist_path_length_vs_adc_369120286 = new TH2D("hist_path_length_vs_adc_369120286", "hist_path_length_vs_adc_369120286", 200, 0, 0.5, 300, 0, 300) ;
  TH2D *hist_path_length_vs_adc_369120277 = new TH2D("hist_path_length_vs_adc_369120277", "hist_path_length_vs_adc_369120277", 200, 0, 0.5, 300, 0, 300) ;

  TH1D *hist_adc_369120286 = new TH1D("hist_adc_369120286", "hist_adc_369120286", 300, 0, 300) ;
  TH1D *hist_adc_369120277 = new TH1D("hist_adc_369120277", "hist_adc_369120277", 300, 0, 300) ;

  TH1D *hist_adc_369120286_rebin = new TH1D("hist_adc_369120286_rebin", "hist_adc_369120286_rebin", 75, 0, 300) ;
  TH1D *hist_adc_369120277_rebin = new TH1D("hist_adc_369120277_rebin", "hist_adc_369120277_rebin", 75, 0, 300) ;
  
  hist_adc_369120286->Sumw2() ;
  hist_adc_369120277->Sumw2() ;

  hist_adc_369120286_rebin->Sumw2() ;
  hist_adc_369120277_rebin->Sumw2() ;
   
  string localdir(argv[0]) ;
	int lowlimit =  atoi(argv[1]) ;
	int highlimit = atoi(argv[2]) ;
	string outputindex(argv[3]) ;

	for(int i = 0 ; i < 1500 ; ++i)
	{
		if((i < lowlimit) || (i > highlimit)) continue ;   

		stringstream ss ;
		
		if((i % 100) == 0)
		{
			cout << dec << i << endl ;
		}
		
		ss << i ;
		string prefix = "" ;
		
		for(int j = 0 ; j < (4 - ss.str().length()) ; ++j) 
		{
			prefix += "0" ;
		}
		
		// string filename = directory + "dir_" + prefix + ss.str() + "/log";
		string filename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/with_protons_and_adc/log_" + prefix + ss.str() ;
		
		ifstream logfile(filename.c_str()) ;
		
		if(logfile.is_open())
		{
      cout << "ok " << filename << endl ;
      string word ;

			while(logfile >> word)
			{
				if(word.compare("ADC") == 0)
        {
          // cout << "  ok1" << endl ;

          string pixel_or_strip ;
          string chiptext ;
          string pathtext ;
          string adctext ;

 					long int detid = 0 ;
          int chipid = 0 ;
          double path ;
          int number_of_adc_values ;

          logfile >> pixel_or_strip >> detid >> chiptext >> chipid >> pathtext >> path >> adctext >> number_of_adc_values ;
          // cout << adctext << endl ;
          
          if(adctext.compare("adc_values") == 0)
          {
            // cout << "detid " << detid << " chipd " << chipid << " pixel_or_strip " << pixel_or_strip << " path_length " << path << " nadc " << number_of_adc_values << endl ;
            for(int j = 0 ; j < number_of_adc_values ; ++j)
            {
                double adc_value ;

               logfile >> adc_value ;
               
               // cout << adc_value << " " ;
               
               if(detid == 303042564)
               {
                    hist_path_length_vs_adc_303042564->Fill(path, adc_value) ;
               }
               else if(detid == 303042572)
               {
                    hist_path_length_vs_adc_303042572->Fill(path, adc_value) ;
               }
               else if(detid == 369120286)
               {
                    hist_adc_369120286->Fill(adc_value) ;
                    hist_adc_369120286_rebin->Fill(adc_value) ;
                    hist_path_length_vs_adc_369120286->Fill(path, adc_value) ;
               }
               else if(detid == 369120277)
               {
                    hist_adc_369120277->Fill(adc_value) ;
                    hist_adc_369120277_rebin->Fill(adc_value) ;
                    hist_path_length_vs_adc_369120277->Fill(path, adc_value) ;
                    
                    // cout << "adc_test_values: " << adc_value << " " ;
               }
            }
            
            if(pixel_or_strip.compare("pixel") == 0)
            {
              histogram_path_length_vs_adc_pixel->Fill(path, number_of_adc_values) ;
            }
            else if(pixel_or_strip.compare("strip") == 0)
            {
              histogram_path_length_vs_adc_strip->Fill(path, number_of_adc_values) ;
              
              stringstream detid_string ;
              
              detid_string << detid ;
              
              if(detid_string.str().substr(0, 2).compare("36") == 0)
              {
                histogram_path_length_vs_adc_strip_36->Fill(path, number_of_adc_values) ;
              }
            }
            else
            {
              cout << "Error: ADC not found" << endl ;
            }
            
            
            // cout << endl ;
          }
          else 
          {
            cout << "Error: ADC not found" << endl ;
          }
        }
      }
    
    }
    logfile.close() ;
  }

  histogram_path_length_vs_adc_pixel->GetXaxis()->SetTitle("Path length (cm)") ;
  histogram_path_length_vs_adc_strip->GetXaxis()->SetTitle("Path length (cm)") ;

  histogram_path_length_vs_adc_strip->GetYaxis()->SetTitle("Number of ADC measurements") ;
  histogram_path_length_vs_adc_strip->GetYaxis()->SetTitle("Number of ADC measurements") ;

  histogram_path_length_vs_adc_pixel->SaveAs("plots/histogram_path_length_vs_adc_pixel.root") ;
  histogram_path_length_vs_adc_strip->SaveAs("plots/histogram_path_length_vs_adc_strip.root") ;
  histogram_path_length_vs_adc_strip_36->SaveAs("plots/histogram_path_length_vs_adc_strip_36.root") ;

  TCanvas c1 ;
  c1.SetLogz() ;

  histogram_path_length_vs_adc_pixel->Draw("colz") ;
  c1.SaveAs("plots/histogram_path_length_vs_adc_pixel.pdf") ;

  histogram_path_length_vs_adc_strip->Draw("colz") ;
  c1.SaveAs("plots/histogram_path_length_vs_adc_strip.pdf") ;

  histogram_path_length_vs_adc_strip_36->Draw("colz") ;
  c1.SaveAs("plots/histogram_path_length_vs_adc_strip_36.pdf") ;

  hist_path_length_vs_adc_303042564->SaveAs("plots/hist_path_length_vs_adc_303042564.root") ;
  hist_path_length_vs_adc_303042572->SaveAs("plots/hist_path_length_vs_adc_303042572.root") ;
  hist_path_length_vs_adc_369120286->SaveAs("plots/hist_path_length_vs_adc_369120286.root") ;
  hist_path_length_vs_adc_369120277->SaveAs("plots/hist_path_length_vs_adc_369120277.root") ;

  hist_path_length_vs_adc_303042564->SaveAs("plots/hist_path_length_vs_adc_303042564.png") ;
  hist_path_length_vs_adc_303042572->SaveAs("plots/hist_path_length_vs_adc_303042572.png") ;
  hist_path_length_vs_adc_369120286->SaveAs("plots/hist_path_length_vs_adc_369120286.png") ;
  hist_path_length_vs_adc_369120277->SaveAs("plots/hist_path_length_vs_adc_369120277.png") ;

  hist_adc_369120286->SaveAs("plots/hist_adc_369120286.root") ;
  hist_adc_369120277->SaveAs("plots/hist_adc_369120277.root") ;

  hist_adc_369120286_rebin->SaveAs("plots/hist_adc_369120286_rebin.root") ;
  hist_adc_369120277_rebin->SaveAs("plots/hist_adc_369120277_rebin.root") ;
}

void analyze_the_ntuples()
{
  const string mydirectory = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/with_protons/Results/Ntuples/" ;

  TFile *resultfile = new TFile("resultfile.root", "RECREATE") ;
  
  TH1D *sum = NULL ;
  TH1D *sum2 = NULL ;
  
  for(int i = 0 ; i < 15 ; ++i)
  {
    stringstream ss ;
    ss << i ;
    string mystring = ss.str() ;

    if(i <= 9) mystring = "0" + mystring ;
    
    string myfile = mydirectory + "summary_" + mystring + ".root" ;
    TFile *file = TFile::Open(myfile.c_str()) ;
    cout << myfile << " ok" << endl ;

    TTree *atree = (TTree *)file->Get("mytree") ;
//    atree->Draw("tree_path_length>>pe(100,0,0.5)","tree_kaon&&tree_clean_RP_conf&&(tree_momentum<0.6)","") ;
    atree->Draw("tree_eta>>pe(100, -4, 4)","tree_kaon&&tree_clean_RP_conf&&(tree_momentum<0.6)","") ;
    TH1D *mype = (TH1D *)file->Get("pe") ;
    
    cout << "histentries " << mype->GetEntries() << endl ;
    
    if(i == 0)
    {
      resultfile->cd() ;
      sum = ((TH1D *)mype->Clone("sum")) ;
      sum->Sumw2() ;
    }
    else  sum->Add(mype) ;

    cout << "summentries " << sum->GetEntries() << endl ;
    file->Close() ;
  }


  for(int i = 0 ; i < 15 ; ++i)
  {
    stringstream ss ;
    ss << i ;
    string mystring = ss.str() ;

    if(i <= 9) mystring = "0" + mystring ;
    
    string myfile = mydirectory + "summary_" + mystring + ".root" ;
    TFile *file = TFile::Open(myfile.c_str()) ;
    cout << myfile << " ok" << endl ;

    TTree *atree = (TTree *)file->Get("mytree") ;
//    atree->Draw("tree_path_length>>pe(100,0,0.5)","tree_clean_RP_conf&&(tree_momentum<0.6)","") ;
    atree->Draw("tree_eta>>pe(100, -4, 4)","tree_clean_RP_conf&&(tree_momentum<0.6)","") ;
    TH1D *mype = (TH1D *)file->Get("pe") ;
    
    cout << "histentries " << mype->GetEntries() << endl ;
    
    if(i == 0)
    {
      resultfile->cd() ;
      sum2 = ((TH1D *)mype->Clone("sum2")) ;
      sum2->Sumw2() ;
    }
    else  sum2->Add(mype) ;

    cout << "summentries " << sum2->GetEntries() << endl ;
    file->Close() ;
  }

  resultfile->cd() ;
  sum->Write() ;
  sum2->Write() ;
  
    resultfile->Close() ;
}

void toy_mc_for_amplitude_threshold_effect_test()
{
	bool generate_pions = true ;
	bool generate_kaons = !generate_pions ;
	
	bool simulate_charge_sharing = true ;

	bool mips = false ;
  bool simulate_adc = true ;
  bool simulate_saturation_like_effects = true ;
  
	TH2D *dedx_ideal = new TH2D("dedx_ideal", "dedx_ideal", 100, 0, 5, 100, 0, 12) ;
	TH2D *dedx_reco_ideal = new TH2D("dedx_reco_ideal", "dedx_reco_ideal", 100, 0, 5, 100, 0, 12) ;
	TH2D *dedx_reco = new TH2D("dedx_reco", "dedx_reco", 100, 0, 5, 100, 0, 12) ;
	TH2D *de_vs_dx_ideal = new TH2D("de_vs_dx_ideal", "de_vs_dx_ideal", 100, 0, 0.2, 100, 0, 1) ;
	TH2D *de_vs_dx_reco = new TH2D("de_vs_dx_reco", "de_vs_dx_ideal", 100, 0, 0.2, 100, 0, 1) ;
	TH2D *dy_vs_number_of_strips = new TH2D("dy_vs_number_of_strips", "dy_vs_number_of_strips", 100, 0, 0.2, 100, 0, 1) ;
  
  TH2D *de_vs_momentum_reco = new TH2D("de_vs_momentum_reco", "de_vs_momentum_reco", 100, 0, 5,  100, 0, 1) ;

  TH1D *angle_distribution = new TH1D("angle_distribution", "angle_distribution", 100, -2*TMath::Pi(), 2*TMath::Pi()) ;
  TH1D *eta_distribution = new TH1D("eta_distribution", "eta_distribution", 100, -5, 5) ;
	
	dedx_ideal->GetXaxis()->SetTitle("p (GeV)") ;
	dedx_ideal->GetYaxis()->SetTitle("dE/dx (MeV/cm)") ;

	de_vs_dx_ideal->GetXaxis()->SetTitle("Path length (cm)") ;
	de_vs_dx_ideal->GetYaxis()->SetTitle("dE (MeV)") ;

	de_vs_dx_reco->GetXaxis()->SetTitle("Path length (cm)") ;
	de_vs_dx_reco->GetYaxis()->SetTitle("dE (MeV)") ;

	const string scenario_name = "toy_mc_for_amplitude_threshold_effect_test" ;
	cout << "Scenario name: " << scenario_name << endl ;

	const double x_min_um = 290 ;
	const double pitch_size_um = 100 ;
	const double distance_um = 20 ;
	const double um_to_cm = 1.0e-4 ;
  
  const double charge_sharing_radius_um = 150 ;

  int kaon_gen = 0 ; 
  int pion_gen = 0 ; 
  
  int kaon_reco = 0 ; 
  int pion_reco = 0 ; 
	
	const int number_of_mc_events = 1e6 ;

	TRandom3 random ;
  
  const double momentum_threshold_for_dEdx_to_separate_kaons_and_pions_GeV = 0.8 ;
  
	for(int i = 0 ; i < number_of_mc_events ; ++i)
	{
		// cout << "i:" <<i << endl ;
		
		// double p = 5.0 * random.Uniform() ;
		// double path_length = 0.06 * random.Uniform() ;
		
		// dedx_ideal->Fill(p, fku3(p)) ;
		// dedx_ideal->Fill(p, fpiu3(p)) ;
		
		// double angle_1_rad = random.Gaus(TMath::Pi() / 2.0, (TMath::Pi() / 2.0)/6.0) ;
	  double angle_1_rad = random.Gaus(TMath::Pi() / 2.0, (TMath::Pi() / 2.0)/2.0) ;
		// double angle_1_rad = random.Gaus(TMath::Pi() / 2.0, (TMath::Pi() / 2.0)/20.0) ;
    
    double eta = -log(tan(angle_1_rad / 2.0)) ;
    
    angle_distribution->Fill(angle_1_rad) ;
    eta_distribution->Fill(eta) ;

		double dx_um = x_min_um / sin(angle_1_rad) ;
		double dy_um = dx_um * cos(angle_1_rad) ;
		double p_GeV = 0 ;
		
		if(mips) p_GeV = random.Uniform(0.8, 5.0) ;
		else p_GeV = random.Uniform(0.01, 5.0) ;

		double dx_cm = um_to_cm * dx_um ;
		double dy_cm = um_to_cm * dy_um ;
    
    int number_of_strips = dy_um / (pitch_size_um + distance_um) ;
		
		double dE_MeV = 0.0 ; 
    
    // Review #0001 start
    
    double what_to_generate = random.Uniform(0.0, 2.0) ;
    // double what_to_generate = 0 ;
    
    if(what_to_generate < 1.0)
    {
      generate_pions = true ;
      generate_kaons = false ;
      
      if(p_GeV < momentum_threshold_for_dEdx_to_separate_kaons_and_pions_GeV) ++pion_gen ;
    }
    else
    {
      generate_pions = false ;
      generate_kaons = true ;

      if(p_GeV < momentum_threshold_for_dEdx_to_separate_kaons_and_pions_GeV) ++kaon_gen ;
    }

    // Review #0001 end
    
		
		if(generate_pions)      dE_MeV = (0.5 * (fpiu3(p_GeV) + fpid3(p_GeV))) * dx_cm ;
		else if(generate_kaons) dE_MeV = (0.5 * (fku3(p_GeV) + fkd3(p_GeV))) * dx_cm ;
		
		double dEdx_MeV_per_cm = dE_MeV / dx_cm ;
		dedx_ideal->Fill(p_GeV, dEdx_MeV_per_cm) ;
		de_vs_dx_ideal->Fill(dx_cm, dE_MeV) ;
    
    dy_vs_number_of_strips->Fill(dy_cm, number_of_strips) ;
    
    vector<double> dE_MeV_vector ;
    vector<double> adc_counts_vector ;
    vector<double> dE_MeV_reco_vector ;

    const int strips_to_charge = 5 ;

    if(simulate_charge_sharing)
    {
      for(int n_strip = 0 ; n_strip < strips_to_charge ; ++n_strip)
      {
	      if(n_strip == 0)  dE_MeV_vector.push_back(dE_MeV * 0.05) ;
	      if(n_strip == 1)  dE_MeV_vector.push_back(dE_MeV * 0.10) ;
	      if(n_strip == 2)  dE_MeV_vector.push_back(dE_MeV * 0.70) ;
	      if(n_strip == 3)  dE_MeV_vector.push_back(dE_MeV * 0.10) ;
	      if(n_strip == 4)  dE_MeV_vector.push_back(dE_MeV * 0.05) ;
      }

    }
    
    
    // RECO section
    
    if(simulate_adc)
    {
      const double energy_deposit_per_digit_MeV = (0.2 / 255.0) ;
      int adc_counts = dE_MeV / energy_deposit_per_digit_MeV ;
      // cout << adc_counts << endl ;

      double p_GeV_reco = p_GeV ;
      double dx_cm_reco = dx_cm ;
      double dE_MeV_reco_ideal = adc_counts * energy_deposit_per_digit_MeV ;
      double dEdx_MeV_per_cm_reco_ideal = dE_MeV_reco_ideal / dx_cm_reco ;

      if(simulate_charge_sharing)       
      {
        for(int n_strip = 0 ; n_strip < strips_to_charge ; ++n_strip)
        {
	        adc_counts_vector.push_back(dE_MeV_vector[n_strip]/ energy_deposit_per_digit_MeV) ;
        }
      }

  		dedx_reco_ideal->Fill(p_GeV_reco, dEdx_MeV_per_cm_reco_ideal) ;
      
      // int cutoff = 2*255 ;
      int cutoff = 255 ;

      if(!simulate_saturation_like_effects) cutoff = 255e5 ;
      
      if(1)
      {
        int adc_counts_with_cut = adc_counts ;

        if(adc_counts_with_cut > cutoff) adc_counts_with_cut = cutoff ;

        if(simulate_charge_sharing)       
        {
          for(int n_strip = 0 ; n_strip < strips_to_charge ; ++n_strip)
          {
            adc_counts_with_cut = adc_counts_vector[n_strip] ;
            if(adc_counts_with_cut > cutoff) adc_counts_with_cut = cutoff ;

	          dE_MeV_reco_vector.push_back(adc_counts_with_cut * energy_deposit_per_digit_MeV) ;
          }
        }

        
        double dE_MeV_reco = adc_counts_with_cut * energy_deposit_per_digit_MeV ;

        if(simulate_charge_sharing)       
        {
          dE_MeV_reco = 0 ;

          for(int n_strip = 0 ; n_strip < strips_to_charge ; ++n_strip)
          {
            dE_MeV_reco += dE_MeV_reco_vector[n_strip] ;
          }
        }
        
        double dEdx_MeV_per_cm_reco = dE_MeV_reco / dx_cm_reco ;
        
    		de_vs_dx_reco->Fill(dx_cm, dE_MeV_reco) ;           
    		de_vs_momentum_reco->Fill(p_GeV, dE_MeV_reco) ;           


    		dedx_reco->Fill(p_GeV_reco, dEdx_MeV_per_cm_reco) ;


        if(p_GeV < momentum_threshold_for_dEdx_to_separate_kaons_and_pions_GeV)        
        {
          if((fkd3(p_GeV) < dEdx_MeV_per_cm_reco) && (dEdx_MeV_per_cm_reco < fku3(p_GeV)))
          {
            ++kaon_reco ;
          } 

          if((fpid3(p_GeV) < dEdx_MeV_per_cm_reco) && (dEdx_MeV_per_cm_reco < fpiu3(p_GeV)))
          {
            ++pion_reco ;
          } 
        }
      }
    }
	}
  
  cout << "kaonpion " << kaon_reco << " " << pion_reco << endl ;
  cout << "kaonpion " << ((double)kaon_reco/kaon_gen) << " " << ((double)pion_reco/pion_gen) << endl ;

	TCanvas c ;
	c.SetLogz() ;
	
	dedx_ideal->SaveAs("plots/study/mc/dedx_ideal.root") ;
	dedx_ideal->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/dedx_ideal.pdf") ;

	de_vs_dx_ideal->SaveAs("plots/study/mc/de_vs_dx_ideal.root") ;
	de_vs_dx_ideal->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/de_vs_dx_ideal.pdf") ;

	de_vs_dx_reco->SaveAs("plots/study/mc/de_vs_dx_reco.root") ;
	de_vs_dx_reco->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/de_vs_dx_reco.pdf") ;

	dy_vs_number_of_strips->SaveAs("plots/study/mc/dy_vs_number_of_strips.root") ;
	dy_vs_number_of_strips->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/dy_vs_number_of_strips.pdf") ;

	dedx_reco_ideal->SaveAs("plots/study/mc/dedx_reco_ideal.root") ;
	dedx_reco_ideal->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/dedx_reco_ideal.pdf") ;

	dedx_reco->SaveAs("plots/study/mc/dedx_reco.root") ;
	dedx_reco->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/dedx_reco.pdf") ;

  angle_distribution->GetXaxis()->SetTitle("#theta (rad)") ;

	angle_distribution->SaveAs("plots/study/mc/angle_distribution.root") ;
	angle_distribution->Draw("") ;
	
	c.SaveAs("plots/study/mc/angle_distribution.pdf") ;

  eta_distribution->GetXaxis()->SetTitle("#eta") ;

	eta_distribution->SaveAs("plots/study/mc/eta_distribution.root") ;
	eta_distribution->Draw("") ;
	
	c.SaveAs("plots/study/mc/eta_distribution.pdf") ;
  
  de_vs_momentum_reco->GetXaxis()->SetTitle("p (GeV)") ;
  de_vs_momentum_reco->GetYaxis()->SetTitle("dE (MeV)") ;

	de_vs_momentum_reco->SaveAs("plots/study/mc/de_vs_momentum_reco.root") ;
	de_vs_momentum_reco->Draw("colz") ;
	
	c.SaveAs("plots/study/mc/de_vs_momentum_reco.pdf") ;
}

void open_and_analyze_tree(map<UInt_t, map<string, TH2F *>> &map_detector_ids_ref, string index, TFile *generic)
{

  string myfilename = "/eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/Studies/perfect_ones_to_be_copied_back/with_protons/Results/Ntuples/summary_" + index + ".root" ;

  TFile *myfile = TFile::Open(myfilename.c_str(), "READ") ;
  
  // TFile *myfile = TFile::Open("summary_01.root") ;

  TTree *mytree = (TTree *)myfile->Get("mytree") ;

  UInt_t tree_detector_id = 0 ;
  Int_t tree_chip_id = 0 ;
  Float_t tree_momentum = 0 ;
  Float_t tree_pt = 0 ;
  Float_t tree_path_length = 0 ;
  Float_t tree_deltaE = 0 ;
  Float_t tree_eta = 0 ;
  Bool_t tree_kaon = false ;
  Bool_t tree_clean_RP_conf = false ;
  Bool_t tree_good_reference_for_kaon = false ;
  
  mytree->SetBranchAddress("tree_detector_id", &tree_detector_id) ;
  mytree->SetBranchAddress("tree_chip_id", &tree_chip_id) ;
  mytree->SetBranchAddress("tree_momentum", &tree_momentum) ;
  mytree->SetBranchAddress("tree_pt", &tree_pt) ;
  mytree->SetBranchAddress("tree_path_length", &tree_path_length) ;
  mytree->SetBranchAddress("tree_deltaE", &tree_deltaE) ;
  mytree->SetBranchAddress("tree_eta", &tree_eta) ;
  mytree->SetBranchAddress("tree_kaon", &tree_kaon) ;
  mytree->SetBranchAddress("tree_clean_RP_conf", &tree_clean_RP_conf) ;
  // mytree->SetBranchAddress("tree_good_reference_for_kaon", &tree_good_reference_for_kaon) ;
  

  int i = 0 ;
  
  for(int iEntry = 0 ; mytree->LoadTree(iEntry) >= 0 ; ++iEntry, ++i)
  {
    mytree->GetEntry(iEntry) ;

    map<UInt_t, map<string, TH2F *>> ::iterator it = map_detector_ids_ref.find(tree_detector_id) ;

//    if(it != map_detector_ids_ref.end()  && (tree_kaon == 1.0))
    if(it != map_detector_ids_ref.end())
    {
      map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      map_detector_ids_ref[tree_detector_id]["eta_vs_path_length"]->Fill(tree_path_length, tree_eta) ;
/*
      if(fabs(tree_eta) < 0.5) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_eta"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_eta) > 0.5) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_high_eta"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

      if(fabs(tree_path_length) < 0.05)
      {
        map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

        if(fabs(tree_eta) < 0.5) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length_center_eta"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
        else
          map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length_side_eta"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

      }
      if(fabs(tree_path_length) > 0.05) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_high_path_length"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

      if(fabs(tree_path_length) > (0.029 + (14 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_00"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (13 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_01"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (12 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_02"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (11 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_03"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (10 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_04"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

      if(fabs(tree_path_length) > (0.029 + (9 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_05"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (8 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_06"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (7 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_07"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (6 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_08"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (5 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_09"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;

      if(fabs(tree_path_length) > (0.029 + (4 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_10"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (3 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_11"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (2 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_12"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (1 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_13"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      if(fabs(tree_path_length) > (0.029 + (0 * 0.004))) map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_14"]->Fill(tree_momentum, (tree_deltaE/tree_path_length)) ;
      */

    }
  }

  myfile->Close() ;
  
  generic->cd() ;
  
  for(map<UInt_t, map<string, TH2F *>> ::iterator it = map_detector_ids_ref.begin() ; it != map_detector_ids_ref.end() ; ++it)
  {
    map<string, TH2F *> *mypointer = &(it->second) ;
    
    for(map<string, TH2F *>::iterator it2 = (*mypointer).begin() ; it2 != (*mypointer).end() ; ++it2)
    {
      string name = it2->second->GetName() ;
      generic->cd() ;
      it2->second->Write() ;
      // it->second->SaveAs(("results/for_M_Musich/" + name + "_plain.root").c_str()) ;
      
      /*
//      TH1D *projection = it->second->ProjectionX("px", 38, 45) ;
//      TH1D *projection = it2->second->ProjectionX("px", 42, 45) ;
      TH1D *projection = it2->second->ProjectionX("px", 70, 72) ;
      
      generic->cd() ;
      projection->SetName((name + "_proj").c_str()) ; 
      projection->Write() ;
      */
      // projection->SaveAs(("results/for_M_Musich/" + name + "_proj.root").c_str()) ;
    }
  }


}


void read_detectors_to_analyze(map<UInt_t, map<string, TH2F *>> &map_detector_ids_ref)
{
//  ifstream detector_list("data/detectors_to_test_sample_for_PXF.txt") ;
//  ifstream detector_list("data/detectors_to_test.txt") ;
  ifstream detector_list("/afs/cern.ch/work/f/fnemes/main_workspace/2022/Version_2/CMSSW_10_1_7/src/analysis_glueballs_2018/PromptAnalyzer/analysis/output_new/Studies/Collect_number_of_dedx_measurements_per_event/data/detectors_to_test_for_adc2.txt") ;

  
  UInt_t tree_detector_id = 0 ;
  
  while(detector_list >> tree_detector_id)
  {
    stringstream ss ;
    ss << tree_detector_id ;
    string histo_name = "id_" + ss.str() + "_momentum_vs_dedx" ;
    string histo_name1 = "id_" + ss.str() + "_momentum_vs_dedx_low_eta" ;
    string histo_name2 = "id_" + ss.str() + "_momentum_vs_dedx_high_eta" ;

    string histo_name3 = "id_" + ss.str() + "_momentum_vs_dedx_low_path_length" ;
    string histo_name4 = "id_" + ss.str() + "_momentum_vs_dedx_high_path_length" ;
    string histo_name5 = "id_" + ss.str() + "_momentum_vs_dedx_low_path_length_center_eta" ;
    string histo_name6 = "id_" + ss.str() + "_momentum_vs_dedx_low_path_length_side_eta" ;

    string histo_name7 = "id_" + ss.str() + "_eta_vs_path_length" ;

    string histo_name08 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_00" ;
    string histo_name09 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_01" ;
    string histo_name10 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_02" ;
    string histo_name11 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_03" ;
    string histo_name12 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_04" ;
    string histo_name13 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_05" ;
    string histo_name14 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_06" ;
    string histo_name15 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_07" ;
    string histo_name16 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_08" ;
    string histo_name17 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_09" ;

    string histo_name18 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_10" ;
    string histo_name19 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_11" ;
    string histo_name20 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_12" ;
    string histo_name21 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_13" ;
    string histo_name22 = "id_" + ss.str() + "_momentum_vs_dedx_path_length_slice_14" ;

    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx"] = new TH2F(histo_name.c_str(), histo_name.c_str(), 200,0,4.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["eta_vs_path_length"] = new TH2F(histo_name7.c_str(), histo_name7.c_str(), 200,0,1.0,100, -4, 4) ;

    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx"]->GetXaxis()->SetTitle("p (GeV)") ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx"]->GetYaxis()->SetTitle("dE/dx (MeV/cm)") ;

    map_detector_ids_ref[tree_detector_id]["eta_vs_path_length"]->GetXaxis()->SetTitle("Path length (cm)") ;
    map_detector_ids_ref[tree_detector_id]["eta_vs_path_length"]->GetYaxis()->SetTitle("#eta") ;
    
    /*
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_eta"] = new TH2F(histo_name1.c_str(), histo_name1.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_high_eta"] = new TH2F(histo_name2.c_str(), histo_name2.c_str(), 200,0,5.0,100,0,12.0) ;

    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length"] = new TH2F(histo_name3.c_str(), histo_name3.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length_center_eta"] = new TH2F(histo_name5.c_str(), histo_name5.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_low_path_length_side_eta"] = new TH2F(histo_name6.c_str(), histo_name6.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_high_path_length"] = new TH2F(histo_name4.c_str(), histo_name4.c_str(), 200,0,5.0,100,0,12.0) ;


    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_00"] = new TH2F(histo_name08.c_str(), histo_name08.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_01"] = new TH2F(histo_name09.c_str(), histo_name09.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_02"] = new TH2F(histo_name10.c_str(), histo_name10.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_03"] = new TH2F(histo_name11.c_str(), histo_name11.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_04"] = new TH2F(histo_name12.c_str(), histo_name12.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_05"] = new TH2F(histo_name13.c_str(), histo_name13.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_06"] = new TH2F(histo_name14.c_str(), histo_name14.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_07"] = new TH2F(histo_name15.c_str(), histo_name15.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_08"] = new TH2F(histo_name16.c_str(), histo_name16.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_09"] = new TH2F(histo_name17.c_str(), histo_name17.c_str(), 200,0,5.0,100,0,12.0) ;

    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_10"] = new TH2F(histo_name18.c_str(), histo_name18.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_11"] = new TH2F(histo_name19.c_str(), histo_name19.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_12"] = new TH2F(histo_name20.c_str(), histo_name20.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_13"] = new TH2F(histo_name21.c_str(), histo_name21.c_str(), 200,0,5.0,100,0,12.0) ;
    map_detector_ids_ref[tree_detector_id]["momentum_vs_dedx_path_length_slice_14"] = new TH2F(histo_name22.c_str(), histo_name22.c_str(), 200,0,5.0,100,0,12.0) ;
    */
    
  }
}

void collect_fits_for_Marco_Musich(int argc, char *argv[])
{
	if((argc != 4))
	{
		cout << "I need 3 arguments: low high outputindex." << endl ;
		exit(1) ;
	}
  
	int lowlimit =  atoi(argv[1]) ;
	int highlimit = atoi(argv[2]) ;
	string outputindex_str1(argv[3]) ;
  
  
  // stringstream outputindex_str ;
  // outputindex_str << outputindex ;

  map<UInt_t, map<string, TH2F *>>  map_detector_ids_to_histogram ;

  string command_directory = "results/for_M_Musich/" ;

 	TFile* myresults = TFile::Open((command_directory + "/results_" + outputindex_str1 + ".root").c_str(), "RECREATE", "Generic") ;

  read_detectors_to_analyze(map_detector_ids_to_histogram) ;
  
	open_and_analyze_tree(map_detector_ids_to_histogram, outputindex_str1, myresults) ;
}

int main(int argc, char *argv[])
{
	// convert_gain() ;

	// print_track_and_dedx_measurements_from_logfile() ;
	// test_dedx_and_gain_using_log() ;
	// test_dedx_per_momentum_and_per_detector_using_log() ;
	// further_analyse_gains_if_possible_to_incorporate_differences() ;
	// test_dedx_per_momentum_and_per_detector_using_log_with_string() ;

   // Succesful code to read the log files of Detectors
	// test_dedx_per_momentum_and_per_detector_using_log_with_string_ntuple(argc, argv) ;

   // Successful code to analyze gain calibration from 
	// analyze_file(argc, argv) ;
   
   // analyze_correlation() ;
   
   // analyze_frequencies() ;
   

  // analyze_adc(argc, argv) ;
  
  // analyze_the_ntuples() ;
  
  // toy_mc_for_amplitude_threshold_effect_test() ;

  // collect_fits_for_Marco_Musich(argc, argv) ; ;

  analyze_file_all_details(argc, argv) ;

}	
