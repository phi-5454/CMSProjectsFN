//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 22 17:05:44 2016 by ROOT version 5.27/06b
// from TTree TReducedNtuple/TReducedNtuple
// found on file: TReducedNtuple.root
//////////////////////////////////////////////////////////

class TProjectInformation
{
	public:

	bool 		valid ;
	ULong64_t       event_info_timestamp_start ;
	ULong64_t       event_info_timestamp_end ;

	map<int, int> trigger_bit_mask ;

	map<int, int> trigger_bits ;
	map<int, int> elastic_trigger_bits ;

	TProjectInformation() ;

	void PrintTriggerBits() ;
} ;

TProjectInformation::TProjectInformation()
{
	valid = false ;
	event_info_timestamp_start = 0 ;
	event_info_timestamp_end  = 0 ;

	// trigger_bit_mask

	trigger_bit_mask[0] = pow(2,0) ;
	trigger_bit_mask[1] = pow(2,1) ;
	trigger_bit_mask[2] = pow(2,2) ;
	trigger_bit_mask[3] = pow(2,3) ;
	trigger_bit_mask[4] = pow(2,4) ;
	trigger_bit_mask[5] = pow(2,5) ;
	trigger_bit_mask[6] = pow(2,6) ;
	trigger_bit_mask[7] = pow(2,7) ;

	trigger_bit_mask[8] = pow(2,8) ;
	trigger_bit_mask[9] = pow(2,9) ;
	trigger_bit_mask[10] = pow(2,10) ;
	trigger_bit_mask[11] = pow(2,11) ;
	trigger_bit_mask[12] = pow(2,12) ;
	trigger_bit_mask[13] = pow(2,13) ;
	trigger_bit_mask[14] = pow(2,14) ;
	trigger_bit_mask[15] = pow(2,15) ;

}

void TProjectInformation::PrintTriggerBits()
{
	cout << "Trigger bits" << endl ;
	cout << endl ;

	for(map<int, int>::iterator it = trigger_bits.begin() ; it != trigger_bits.end() ; ++it)
	{
		cout << it->first << ". bit appears " << it->second << " times." << endl ;
	}

	cout << "=====================" << endl ;

	for(map<int, int>::iterator it = elastic_trigger_bits.begin() ; it != elastic_trigger_bits.end() ; ++it)
	{
		cout << it->first << ". bit appears " << it->second << " times." << endl ;
	}
}

#ifndef TReducedNtuple_h
#define TReducedNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>

class TReducedNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   ULong64_t       event_info_timestamp;
   UInt_t          trigger_data_run_num;
   UInt_t          trigger_data_bunch_num;
   UInt_t          trigger_data_event_num;
   UInt_t          trigger_data_trigger_num;
   UInt_t          trigger_data_input_status_bits;

   Bool_t          track_left_near_valid;
   Double_t        track_left_near_x;
   Double_t        track_left_near_y;
   Double_t        track_left_near_thx;
   Double_t        track_left_near_thy;

   Bool_t          track_left_far_valid;
   Double_t        track_left_far_x;
   Double_t        track_left_far_y;
   Double_t        track_left_far_thx;
   Double_t        track_left_far_thy;

   Bool_t          track_right_near_valid;
   Double_t        track_right_near_x;
   Double_t        track_right_near_y;
   Double_t        track_right_near_thx;
   Double_t        track_right_near_thy;

   Bool_t          track_right_far_valid;
   Double_t        track_right_far_x;
   Double_t        track_right_far_y;
   Double_t        track_right_far_thx;
   Double_t        track_right_far_thy;

   Bool_t          track_left_near_horizontal_valid;
   Double_t        track_left_near_horizontal_x;
   Double_t        track_left_near_horizontal_y;
   Double_t        track_left_near_horizontal_thx;
   Double_t        track_left_near_horizontal_thy;
   UInt_t	   track_left_near_uPlanesOn;
   UInt_t	   track_left_near_vPlanesOn;

   Bool_t          track_left_far_horizontal_valid;
   Double_t        track_left_far_horizontal_x;
   Double_t        track_left_far_horizontal_y;
   Double_t        track_left_far_horizontal_thx;
   Double_t        track_left_far_horizontal_thy;
   UInt_t	   track_left_far_uPlanesOn;
   UInt_t	   track_left_far_vPlanesOn;

   Bool_t          track_right_near_horizontal_valid;
   Double_t        track_right_near_horizontal_x;
   Double_t        track_right_near_horizontal_y;
   Double_t        track_right_near_horizontal_thx;
   Double_t        track_right_near_horizontal_thy;
   UInt_t	   track_right_near_uPlanesOn;
   UInt_t	   track_right_near_vPlanesOn;

   Bool_t          track_right_far_horizontal_valid;
   Double_t        track_right_far_horizontal_x;
   Double_t        track_right_far_horizontal_y;
   Double_t        track_right_far_horizontal_thx;
   Double_t        track_right_far_horizontal_thy;
   UInt_t	   track_right_far_uPlanesOn;
   UInt_t	   track_right_far_vPlanesOn;

   UInt_t          number_of_occurances;

   Double_t	   track_left_near_z ;
   Double_t	   track_left_near_chi2 ;
   Double_t	   track_left_near_chi2ndf ;

   Double_t	   track_left_far_z ;
   Double_t	   track_left_far_chi2 ;
   Double_t	   track_left_far_chi2ndf ;

   Double_t	   track_right_near_z ;
   Double_t	   track_right_near_chi2 ;
   Double_t	   track_right_near_chi2ndf ;

   Double_t	   track_right_far_z ;
   Double_t	   track_right_far_chi2 ;
   Double_t	   track_right_far_chi2ndf ;

   // List of branches
   TBranch        *b_event_info_timestamp;   //!
   TBranch        *b_trigger_data_run_num;   //!
   TBranch        *b_trigger_data_bunch_num;   //!
   TBranch        *b_trigger_data_event_num;   //!
   TBranch        *b_trigger_data_trigger_num;   //!
   TBranch        *b_trigger_data_input_status_bits;   //!
   
   TBranch        *b_track_left_near_valid;   //!
   TBranch        *b_track_left_near_x;   //!
   TBranch        *b_track_left_near_y;   //!
   TBranch        *b_track_left_near_thx;   //!
   TBranch        *b_track_left_near_thy;   //!
   
   TBranch        *b_track_left_far_valid;   //!
   TBranch        *b_track_left_far_x;   //!
   TBranch        *b_track_left_far_y;   //!
   TBranch        *b_track_left_far_thx;   //!
   TBranch        *b_track_left_far_thy;   //!
   
   TBranch        *b_track_right_near_valid;   //!
   TBranch        *b_track_right_near_x;   //!
   TBranch        *b_track_right_near_y;   //!
   TBranch        *b_track_right_near_thx;   //!
   TBranch        *b_track_right_near_thy;   //!
   
   TBranch        *b_track_right_far_valid;   //!
   TBranch        *b_track_right_far_x;   //!
   TBranch        *b_track_right_far_y;   //!
   TBranch        *b_track_right_far_thx;   //!
   TBranch        *b_track_right_far_thy;   //!
   
   TBranch        *b_track_left_near_horizontal_valid;   //!
   TBranch        *b_track_left_near_horizontal_x;   //!
   TBranch        *b_track_left_near_horizontal_y;   //!
   TBranch        *b_track_left_near_horizontal_thx;   //!
   TBranch        *b_track_left_near_horizontal_thy;   //!

   TBranch        *b_track_left_far_horizontal_valid;   //!
   TBranch        *b_track_left_far_horizontal_x;   //!
   TBranch        *b_track_left_far_horizontal_y;   //!
   TBranch        *b_track_left_far_horizontal_thx;   //!
   TBranch        *b_track_left_far_horizontal_thy;   //!

   TBranch        *b_track_right_near_horizontal_valid;   //!
   TBranch        *b_track_right_near_horizontal_x;   //!
   TBranch        *b_track_right_near_horizontal_y;   //!
   TBranch        *b_track_right_near_horizontal_thx;   //!
   TBranch        *b_track_right_near_horizontal_thy;   //!

   TBranch        *b_track_right_far_horizontal_valid;   //!
   TBranch        *b_track_right_far_horizontal_x;   //!
   TBranch        *b_track_right_far_horizontal_y;   //!
   TBranch        *b_track_right_far_horizontal_thx;   //!
   TBranch        *b_track_right_far_horizontal_thy;   //!
   
   TBranch        *b_track_left_near_uPlanesOn ;
   TBranch        *b_track_left_near_vPlanesOn ;
   TBranch        *b_track_left_far_uPlanesOn ; 
   TBranch        *b_track_left_far_vPlanesOn ; 
   TBranch        *b_track_right_near_uPlanesOn ;
   TBranch        *b_track_right_near_vPlanesOn ;
   TBranch        *b_track_right_far_uPlanesOn ;
   TBranch        *b_track_right_far_vPlanesOn ;

   TBranch        *b_number_of_occurances;   //!
   
   TBranch        *b_track_left_near_z ;
   TBranch        *b_track_left_near_chi2 ;
   TBranch        *b_track_left_near_chi2ndf ;

   TBranch        *b_track_left_far_z ;
   TBranch        *b_track_left_far_chi2 ;
   TBranch        *b_track_left_far_chi2ndf ;

   TBranch        *b_track_right_near_z ;
   TBranch        *b_track_right_near_chi2 ;
   TBranch        *b_track_right_near_chi2ndf ;

   TBranch        *b_track_right_far_z ;
   TBranch        *b_track_right_far_chi2 ;
   TBranch        *b_track_right_far_chi2ndf ;

   TReducedNtuple(string, string, string,  bool, TTree *tree=0);
   virtual ~TReducedNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     InitMultitrack(TTree *tree, bool);
   virtual void     Loop(TProjectParameters *, TProtonReconstruction *, map_from_string_to_PTPlotsCollection_type *, string,  TProjectInformation *, int);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   string root_file_name ;
   string input_directory ;
   bool multitrack ;
};

#endif

#ifdef TReducedNtuple_cxx
TReducedNtuple::TReducedNtuple(string root_file_name, string input_directory, string treename, bool a_multitrack, TTree *tree): root_file_name(root_file_name), input_directory(input_directory), number_of_occurances(1), multitrack(a_multitrack)
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
      
      tree = (TTree*)gDirectory->Get(treename.c_str());

	if(tree == NULL)
	{
        	cout << "Error: TReducedNtuple::TReducedNtuple: input tree does not exist !" << endl ;
        	exit(1) ;
	}

   }

   Init(tree);
   InitMultitrack(tree, multitrack);
}

TReducedNtuple::~TReducedNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TReducedNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TReducedNtuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TReducedNtuple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;

   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event_info_timestamp", &event_info_timestamp, &b_event_info_timestamp);
   fChain->SetBranchAddress("trigger_data_run_num", &trigger_data_run_num, &b_trigger_data_run_num);
   fChain->SetBranchAddress("trigger_data_bunch_num", &trigger_data_bunch_num, &b_trigger_data_bunch_num);
   fChain->SetBranchAddress("trigger_data_event_num", &trigger_data_event_num, &b_trigger_data_event_num);
   fChain->SetBranchAddress("trigger_data_trigger_num", &trigger_data_trigger_num, &b_trigger_data_trigger_num);
   fChain->SetBranchAddress("trigger_data_input_status_bits", &trigger_data_input_status_bits, &b_trigger_data_input_status_bits);

   fChain->SetBranchAddress("track_left_near_valid", &track_left_near_valid, &b_track_left_near_valid);
   fChain->SetBranchAddress("track_left_near_x", &track_left_near_x, &b_track_left_near_x);
   fChain->SetBranchAddress("track_left_near_y", &track_left_near_y, &b_track_left_near_y);
   fChain->SetBranchAddress("track_left_near_thx", &track_left_near_thx, &b_track_left_near_thx);
   fChain->SetBranchAddress("track_left_near_thy", &track_left_near_thy, &b_track_left_near_thy);
   fChain->SetBranchAddress("track_left_near_uPlanesOn", &track_left_near_uPlanesOn, &b_track_left_near_uPlanesOn);
   fChain->SetBranchAddress("track_left_near_vPlanesOn", &track_left_near_vPlanesOn, &b_track_left_near_vPlanesOn);

   fChain->SetBranchAddress("track_left_far_valid", &track_left_far_valid, &b_track_left_far_valid);
   fChain->SetBranchAddress("track_left_far_x", &track_left_far_x, &b_track_left_far_x);
   fChain->SetBranchAddress("track_left_far_y", &track_left_far_y, &b_track_left_far_y);
   fChain->SetBranchAddress("track_left_far_thx", &track_left_far_thx, &b_track_left_far_thx);
   fChain->SetBranchAddress("track_left_far_thy", &track_left_far_thy, &b_track_left_far_thy);
   fChain->SetBranchAddress("track_left_far_uPlanesOn", &track_left_far_uPlanesOn, &b_track_left_far_uPlanesOn);
   fChain->SetBranchAddress("track_left_far_vPlanesOn", &track_left_far_vPlanesOn, &b_track_left_far_vPlanesOn);

   fChain->SetBranchAddress("track_right_near_valid", &track_right_near_valid, &b_track_right_near_valid);
   fChain->SetBranchAddress("track_right_near_x", &track_right_near_x, &b_track_right_near_x);
   fChain->SetBranchAddress("track_right_near_y", &track_right_near_y, &b_track_right_near_y);
   fChain->SetBranchAddress("track_right_near_thx", &track_right_near_thx, &b_track_right_near_thx);
   fChain->SetBranchAddress("track_right_near_thy", &track_right_near_thy, &b_track_right_near_thy);
   fChain->SetBranchAddress("track_right_near_uPlanesOn", &track_right_near_uPlanesOn, &b_track_right_near_uPlanesOn);
   fChain->SetBranchAddress("track_right_near_vPlanesOn", &track_right_near_vPlanesOn, &b_track_right_near_vPlanesOn);

   fChain->SetBranchAddress("track_right_far_valid", &track_right_far_valid, &b_track_right_far_valid);
   fChain->SetBranchAddress("track_right_far_x", &track_right_far_x, &b_track_right_far_x);
   fChain->SetBranchAddress("track_right_far_y", &track_right_far_y, &b_track_right_far_y);
   fChain->SetBranchAddress("track_right_far_thx", &track_right_far_thx, &b_track_right_far_thx);
   fChain->SetBranchAddress("track_right_far_thy", &track_right_far_thy, &b_track_right_far_thy);
   fChain->SetBranchAddress("track_right_far_uPlanesOn", &track_right_far_uPlanesOn, &b_track_right_far_uPlanesOn);
   fChain->SetBranchAddress("track_right_far_vPlanesOn", &track_right_far_vPlanesOn, &b_track_right_far_vPlanesOn);

   fChain->SetBranchAddress("track_left_near_horizontal_valid", &track_left_near_horizontal_valid, &b_track_left_near_horizontal_valid);
   fChain->SetBranchAddress("track_left_near_horizontal_x", &track_left_near_horizontal_x, &b_track_left_near_horizontal_x);
   fChain->SetBranchAddress("track_left_near_horizontal_y", &track_left_near_horizontal_y, &b_track_left_near_horizontal_y);
   fChain->SetBranchAddress("track_left_near_horizontal_thx", &track_left_near_horizontal_thx, &b_track_left_near_horizontal_thx);
   fChain->SetBranchAddress("track_left_near_horizontal_thy", &track_left_near_horizontal_thy, &b_track_left_near_horizontal_thy);
   fChain->SetBranchAddress("track_left_far_horizontal_valid", &track_left_far_horizontal_valid, &b_track_left_far_horizontal_valid);
   fChain->SetBranchAddress("track_left_far_horizontal_x", &track_left_far_horizontal_x, &b_track_left_far_horizontal_x);
   fChain->SetBranchAddress("track_left_far_horizontal_y", &track_left_far_horizontal_y, &b_track_left_far_horizontal_y);
   fChain->SetBranchAddress("track_left_far_horizontal_thx", &track_left_far_horizontal_thx, &b_track_left_far_horizontal_thx);
   fChain->SetBranchAddress("track_left_far_horizontal_thy", &track_left_far_horizontal_thy, &b_track_left_far_horizontal_thy);
   fChain->SetBranchAddress("track_right_near_horizontal_valid", &track_right_near_horizontal_valid, &b_track_right_near_horizontal_valid);
   fChain->SetBranchAddress("track_right_near_horizontal_x", &track_right_near_horizontal_x, &b_track_right_near_horizontal_x);
   fChain->SetBranchAddress("track_right_near_horizontal_y", &track_right_near_horizontal_y, &b_track_right_near_horizontal_y);
   fChain->SetBranchAddress("track_right_near_horizontal_thx", &track_right_near_horizontal_thx, &b_track_right_near_horizontal_thx);
   fChain->SetBranchAddress("track_right_near_horizontal_thy", &track_right_near_horizontal_thy, &b_track_right_near_horizontal_thy);
   fChain->SetBranchAddress("track_right_far_horizontal_valid", &track_right_far_horizontal_valid, &b_track_right_far_horizontal_valid);
   fChain->SetBranchAddress("track_right_far_horizontal_x", &track_right_far_horizontal_x, &b_track_right_far_horizontal_x);
   fChain->SetBranchAddress("track_right_far_horizontal_y", &track_right_far_horizontal_y, &b_track_right_far_horizontal_y);
   fChain->SetBranchAddress("track_right_far_horizontal_thx", &track_right_far_horizontal_thx, &b_track_right_far_horizontal_thx);
   fChain->SetBranchAddress("track_right_far_horizontal_thy", &track_right_far_horizontal_thy, &b_track_right_far_horizontal_thy);
   Notify();
}

void TReducedNtuple::InitMultitrack(TTree *tree, bool multitrack)
{
	fChain->SetMakeClass(0);

	fChain->SetBranchAddress("track_left_near_z",		 &track_left_near_z		 , &b_track_left_near_z) ;
	fChain->SetBranchAddress("track_left_near_chi2",	 &track_left_near_chi2  	 , &b_track_left_near_chi2) ;
	fChain->SetBranchAddress("track_left_near_chi2ndf",	 &track_left_near_chi2ndf	 , &b_track_left_near_chi2ndf) ;

	fChain->SetBranchAddress("track_left_far_z",		 &track_left_far_z		 , &b_track_left_far_z) ;
	fChain->SetBranchAddress("track_left_far_chi2", 	 &track_left_far_chi2		 , &b_track_left_far_chi2) ;
	fChain->SetBranchAddress("track_left_far_chi2ndf",	 &track_left_far_chi2ndf	 , &b_track_left_far_chi2ndf) ;

	fChain->SetBranchAddress("track_right_near_z",  	 &track_right_near_z		 , &b_track_right_near_z) ;
	fChain->SetBranchAddress("track_right_near_chi2",	 &track_right_near_chi2 	 , &b_track_right_near_chi2) ;
	fChain->SetBranchAddress("track_right_near_chi2ndf",	 &track_right_near_chi2ndf	 , &b_track_right_near_chi2ndf) ;

	fChain->SetBranchAddress("track_right_far_z",		 &track_right_far_z		 , &b_track_right_far_z) ;
	fChain->SetBranchAddress("track_right_far_chi2",	 &track_right_far_chi2  	 , &b_track_right_far_chi2) ;
	fChain->SetBranchAddress("track_right_far_chi2ndf",	 &track_right_far_chi2ndf	 , &b_track_right_far_chi2ndf) ;

}

Bool_t TReducedNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TReducedNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TReducedNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TReducedNtuple_cxx
