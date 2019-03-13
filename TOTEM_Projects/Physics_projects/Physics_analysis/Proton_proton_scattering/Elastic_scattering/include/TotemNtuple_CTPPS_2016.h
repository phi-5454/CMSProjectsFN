//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun 15 11:41:53 2016 by ROOT version 5.27/06b
// from TTree TotemNtuple/TotemNtuple
// found on file: root://eostotem.cern.ch//eos/totem/user/j/jkaspar/reco/alignment_sr/10077_EVB12_1_ntuple.root
//////////////////////////////////////////////////////////

#ifndef TotemNtuple_CTPPS_2016_h
#define TotemNtuple_CTPPS_2016_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
   const Int_t kMax_CTPPS_2016_event_info = 1;
   const Int_t kMax_CTPPS_2016_trigger_data = 1;
   const Int_t kMax_CTPPS_2016_track_rp_0 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_0 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_1 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_1 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_2 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_2 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_3 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_3 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_4 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_4 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_5 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_5 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_20 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_20 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_21 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_21 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_22 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_22 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_23 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_23 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_24 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_24 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_25 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_25 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_100 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_100 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_101 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_101 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_102 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_102 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_103 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_103 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_104 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_104 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_105 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_105 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_120 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_120 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_121 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_121 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_122 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_122 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_123 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_123 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_124 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_124 = 1;
   const Int_t kMax_CTPPS_2016_track_rp_125 = 1;
   const Int_t kMax_CTPPS_2016_multi_track_rp_125 = 1;
   const Int_t kMax_CTPPS_2016_rec_prot_left = 1;
   const Int_t kMax_CTPPS_2016_rec_prot_right = 1;
   const Int_t kMax_CTPPS_2016_rec_prot_pair = 1;

class TotemNtuple_CTPPS_2016 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //EventMetaData   *event_info.;
   ULong_t         event_info_run_no;
   ULong_t         event_info_event_no;
   ULong_t         event_info_daq_event_number;
   ULong64_t       event_info_timestamp;
   vector<unsigned int> event_info_optoRx_Id;
   vector<unsigned int> event_info_optoRx_BX;
   vector<unsigned int> event_info_optoRx_LV1;
 //TriggerData     *trigger_data.;
   UChar_t         trigger_data_type;
   UInt_t          trigger_data_event_num;
   UInt_t          trigger_data_bunch_num;
   UInt_t          trigger_data_src_id;
   UInt_t          trigger_data_orbit_num;
   UChar_t         trigger_data_revision_num;
   UInt_t          trigger_data_run_num;
   UInt_t          trigger_data_trigger_num;
   UInt_t          trigger_data_inhibited_triggers_num;
   UInt_t          trigger_data_input_status_bits;
 //RPRootDumpTrackInfo *track_rp_0.;
   Bool_t          track_rp_0_valid;
   Double_t        track_rp_0_x;
   Double_t        track_rp_0_y;
   Double_t        track_rp_0_z;
   Double_t        track_rp_0_thx;
   Double_t        track_rp_0_thy;
   Double_t        track_rp_0_chi2;
   Double_t        track_rp_0_chi2ndf;
   UInt_t          track_rp_0_entries;
   Double_t        track_rp_0_res_x;
   Double_t        track_rp_0_res_y;
   vector<int>     track_rp_0_u_sect;
   vector<int>     track_rp_0_v_sect;
   Int_t           track_rp_0_u_sect_no;
   Int_t           track_rp_0_v_sect_no;
   UInt_t          track_rp_0_u_id;
   UInt_t          track_rp_0_v_id;
   Int_t           multi_track_rp_0_;
   Bool_t          multi_track_rp_0_valid[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_x[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_y[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_z[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_thx[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_thy[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_chi2[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_chi2ndf[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   UInt_t          multi_track_rp_0_entries[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_res_x[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Double_t        multi_track_rp_0_res_y[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   vector<int>     multi_track_rp_0_u_sect[kMax_CTPPS_2016_multi_track_rp_0];
   vector<int>     multi_track_rp_0_v_sect[kMax_CTPPS_2016_multi_track_rp_0];
   Int_t           multi_track_rp_0_u_sect_no[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   Int_t           multi_track_rp_0_v_sect_no[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   UInt_t          multi_track_rp_0_u_id[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
   UInt_t          multi_track_rp_0_v_id[kMax_CTPPS_2016_multi_track_rp_0];   //[multi_track_rp_0_]
 //RPRootDumpTrackInfo *track_rp_1.;
   Bool_t          track_rp_1_valid;
   Double_t        track_rp_1_x;
   Double_t        track_rp_1_y;
   Double_t        track_rp_1_z;
   Double_t        track_rp_1_thx;
   Double_t        track_rp_1_thy;
   Double_t        track_rp_1_chi2;
   Double_t        track_rp_1_chi2ndf;
   UInt_t          track_rp_1_entries;
   Double_t        track_rp_1_res_x;
   Double_t        track_rp_1_res_y;
   vector<int>     track_rp_1_u_sect;
   vector<int>     track_rp_1_v_sect;
   Int_t           track_rp_1_u_sect_no;
   Int_t           track_rp_1_v_sect_no;
   UInt_t          track_rp_1_u_id;
   UInt_t          track_rp_1_v_id;
   Int_t           multi_track_rp_1_;
   Bool_t          multi_track_rp_1_valid[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_x[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_y[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_z[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_thx[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_thy[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_chi2[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_chi2ndf[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   UInt_t          multi_track_rp_1_entries[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_res_x[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Double_t        multi_track_rp_1_res_y[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   vector<int>     multi_track_rp_1_u_sect[kMax_CTPPS_2016_multi_track_rp_1];
   vector<int>     multi_track_rp_1_v_sect[kMax_CTPPS_2016_multi_track_rp_1];
   Int_t           multi_track_rp_1_u_sect_no[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   Int_t           multi_track_rp_1_v_sect_no[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   UInt_t          multi_track_rp_1_u_id[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
   UInt_t          multi_track_rp_1_v_id[kMax_CTPPS_2016_multi_track_rp_1];   //[multi_track_rp_1_]
 //RPRootDumpTrackInfo *track_rp_2.;
   Bool_t          track_rp_2_valid;
   Double_t        track_rp_2_x;
   Double_t        track_rp_2_y;
   Double_t        track_rp_2_z;
   Double_t        track_rp_2_thx;
   Double_t        track_rp_2_thy;
   Double_t        track_rp_2_chi2;
   Double_t        track_rp_2_chi2ndf;
   UInt_t          track_rp_2_entries;
   Double_t        track_rp_2_res_x;
   Double_t        track_rp_2_res_y;
   vector<int>     track_rp_2_u_sect;
   vector<int>     track_rp_2_v_sect;
   Int_t           track_rp_2_u_sect_no;
   Int_t           track_rp_2_v_sect_no;
   UInt_t          track_rp_2_u_id;
   UInt_t          track_rp_2_v_id;
   Int_t           multi_track_rp_2_;
   Bool_t          multi_track_rp_2_valid[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_x[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_y[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_z[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_thx[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_thy[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_chi2[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_chi2ndf[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   UInt_t          multi_track_rp_2_entries[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_res_x[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Double_t        multi_track_rp_2_res_y[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   vector<int>     multi_track_rp_2_u_sect[kMax_CTPPS_2016_multi_track_rp_2];
   vector<int>     multi_track_rp_2_v_sect[kMax_CTPPS_2016_multi_track_rp_2];
   Int_t           multi_track_rp_2_u_sect_no[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   Int_t           multi_track_rp_2_v_sect_no[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   UInt_t          multi_track_rp_2_u_id[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
   UInt_t          multi_track_rp_2_v_id[kMax_CTPPS_2016_multi_track_rp_2];   //[multi_track_rp_2_]
 //RPRootDumpTrackInfo *track_rp_3.;
   Bool_t          track_rp_3_valid;
   Double_t        track_rp_3_x;
   Double_t        track_rp_3_y;
   Double_t        track_rp_3_z;
   Double_t        track_rp_3_thx;
   Double_t        track_rp_3_thy;
   Double_t        track_rp_3_chi2;
   Double_t        track_rp_3_chi2ndf;
   UInt_t          track_rp_3_entries;
   Double_t        track_rp_3_res_x;
   Double_t        track_rp_3_res_y;
   vector<int>     track_rp_3_u_sect;
   vector<int>     track_rp_3_v_sect;
   Int_t           track_rp_3_u_sect_no;
   Int_t           track_rp_3_v_sect_no;
   UInt_t          track_rp_3_u_id;
   UInt_t          track_rp_3_v_id;
   Int_t           multi_track_rp_3_;
   Bool_t          multi_track_rp_3_valid[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_x[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_y[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_z[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_thx[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_thy[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_chi2[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_chi2ndf[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   UInt_t          multi_track_rp_3_entries[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_res_x[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Double_t        multi_track_rp_3_res_y[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   vector<int>     multi_track_rp_3_u_sect[kMax_CTPPS_2016_multi_track_rp_3];
   vector<int>     multi_track_rp_3_v_sect[kMax_CTPPS_2016_multi_track_rp_3];
   Int_t           multi_track_rp_3_u_sect_no[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   Int_t           multi_track_rp_3_v_sect_no[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   UInt_t          multi_track_rp_3_u_id[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
   UInt_t          multi_track_rp_3_v_id[kMax_CTPPS_2016_multi_track_rp_3];   //[multi_track_rp_3_]
 //RPRootDumpTrackInfo *track_rp_4.;
   Bool_t          track_rp_4_valid;
   Double_t        track_rp_4_x;
   Double_t        track_rp_4_y;
   Double_t        track_rp_4_z;
   Double_t        track_rp_4_thx;
   Double_t        track_rp_4_thy;
   Double_t        track_rp_4_chi2;
   Double_t        track_rp_4_chi2ndf;
   UInt_t          track_rp_4_entries;
   Double_t        track_rp_4_res_x;
   Double_t        track_rp_4_res_y;
   vector<int>     track_rp_4_u_sect;
   vector<int>     track_rp_4_v_sect;
   Int_t           track_rp_4_u_sect_no;
   Int_t           track_rp_4_v_sect_no;
   UInt_t          track_rp_4_u_id;
   UInt_t          track_rp_4_v_id;
   Int_t           multi_track_rp_4_;
   Bool_t          multi_track_rp_4_valid[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_x[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_y[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_z[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_thx[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_thy[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_chi2[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_chi2ndf[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   UInt_t          multi_track_rp_4_entries[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_res_x[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Double_t        multi_track_rp_4_res_y[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   vector<int>     multi_track_rp_4_u_sect[kMax_CTPPS_2016_multi_track_rp_4];
   vector<int>     multi_track_rp_4_v_sect[kMax_CTPPS_2016_multi_track_rp_4];
   Int_t           multi_track_rp_4_u_sect_no[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   Int_t           multi_track_rp_4_v_sect_no[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   UInt_t          multi_track_rp_4_u_id[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
   UInt_t          multi_track_rp_4_v_id[kMax_CTPPS_2016_multi_track_rp_4];   //[multi_track_rp_4_]
 //RPRootDumpTrackInfo *track_rp_5.;
   Bool_t          track_rp_5_valid;
   Double_t        track_rp_5_x;
   Double_t        track_rp_5_y;
   Double_t        track_rp_5_z;
   Double_t        track_rp_5_thx;
   Double_t        track_rp_5_thy;
   Double_t        track_rp_5_chi2;
   Double_t        track_rp_5_chi2ndf;
   UInt_t          track_rp_5_entries;
   Double_t        track_rp_5_res_x;
   Double_t        track_rp_5_res_y;
   vector<int>     track_rp_5_u_sect;
   vector<int>     track_rp_5_v_sect;
   Int_t           track_rp_5_u_sect_no;
   Int_t           track_rp_5_v_sect_no;
   UInt_t          track_rp_5_u_id;
   UInt_t          track_rp_5_v_id;
   Int_t           multi_track_rp_5_;
   Bool_t          multi_track_rp_5_valid[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_x[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_y[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_z[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_thx[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_thy[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_chi2[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_chi2ndf[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   UInt_t          multi_track_rp_5_entries[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_res_x[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Double_t        multi_track_rp_5_res_y[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   vector<int>     multi_track_rp_5_u_sect[kMax_CTPPS_2016_multi_track_rp_5];
   vector<int>     multi_track_rp_5_v_sect[kMax_CTPPS_2016_multi_track_rp_5];
   Int_t           multi_track_rp_5_u_sect_no[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   Int_t           multi_track_rp_5_v_sect_no[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   UInt_t          multi_track_rp_5_u_id[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
   UInt_t          multi_track_rp_5_v_id[kMax_CTPPS_2016_multi_track_rp_5];   //[multi_track_rp_5_]
 //RPRootDumpTrackInfo *track_rp_20.;
   Bool_t          track_rp_20_valid;
   Double_t        track_rp_20_x;
   Double_t        track_rp_20_y;
   Double_t        track_rp_20_z;
   Double_t        track_rp_20_thx;
   Double_t        track_rp_20_thy;
   Double_t        track_rp_20_chi2;
   Double_t        track_rp_20_chi2ndf;
   UInt_t          track_rp_20_entries;
   Double_t        track_rp_20_res_x;
   Double_t        track_rp_20_res_y;
   vector<int>     track_rp_20_u_sect;
   vector<int>     track_rp_20_v_sect;
   Int_t           track_rp_20_u_sect_no;
   Int_t           track_rp_20_v_sect_no;
   UInt_t          track_rp_20_u_id;
   UInt_t          track_rp_20_v_id;
   Int_t           multi_track_rp_20_;
   Bool_t          multi_track_rp_20_valid[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_x[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_y[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_z[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_thx[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_thy[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_chi2[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_chi2ndf[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   UInt_t          multi_track_rp_20_entries[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_res_x[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Double_t        multi_track_rp_20_res_y[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   vector<int>     multi_track_rp_20_u_sect[kMax_CTPPS_2016_multi_track_rp_20];
   vector<int>     multi_track_rp_20_v_sect[kMax_CTPPS_2016_multi_track_rp_20];
   Int_t           multi_track_rp_20_u_sect_no[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   Int_t           multi_track_rp_20_v_sect_no[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   UInt_t          multi_track_rp_20_u_id[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
   UInt_t          multi_track_rp_20_v_id[kMax_CTPPS_2016_multi_track_rp_20];   //[multi_track_rp_20_]
 //RPRootDumpTrackInfo *track_rp_21.;
   Bool_t          track_rp_21_valid;
   Double_t        track_rp_21_x;
   Double_t        track_rp_21_y;
   Double_t        track_rp_21_z;
   Double_t        track_rp_21_thx;
   Double_t        track_rp_21_thy;
   Double_t        track_rp_21_chi2;
   Double_t        track_rp_21_chi2ndf;
   UInt_t          track_rp_21_entries;
   Double_t        track_rp_21_res_x;
   Double_t        track_rp_21_res_y;
   vector<int>     track_rp_21_u_sect;
   vector<int>     track_rp_21_v_sect;
   Int_t           track_rp_21_u_sect_no;
   Int_t           track_rp_21_v_sect_no;
   UInt_t          track_rp_21_u_id;
   UInt_t          track_rp_21_v_id;
   Int_t           multi_track_rp_21_;
   Bool_t          multi_track_rp_21_valid[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_x[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_y[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_z[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_thx[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_thy[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_chi2[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_chi2ndf[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   UInt_t          multi_track_rp_21_entries[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_res_x[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Double_t        multi_track_rp_21_res_y[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   vector<int>     multi_track_rp_21_u_sect[kMax_CTPPS_2016_multi_track_rp_21];
   vector<int>     multi_track_rp_21_v_sect[kMax_CTPPS_2016_multi_track_rp_21];
   Int_t           multi_track_rp_21_u_sect_no[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   Int_t           multi_track_rp_21_v_sect_no[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   UInt_t          multi_track_rp_21_u_id[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
   UInt_t          multi_track_rp_21_v_id[kMax_CTPPS_2016_multi_track_rp_21];   //[multi_track_rp_21_]
 //RPRootDumpTrackInfo *track_rp_22.;
   Bool_t          track_rp_22_valid;
   Double_t        track_rp_22_x;
   Double_t        track_rp_22_y;
   Double_t        track_rp_22_z;
   Double_t        track_rp_22_thx;
   Double_t        track_rp_22_thy;
   Double_t        track_rp_22_chi2;
   Double_t        track_rp_22_chi2ndf;
   UInt_t          track_rp_22_entries;
   Double_t        track_rp_22_res_x;
   Double_t        track_rp_22_res_y;
   vector<int>     track_rp_22_u_sect;
   vector<int>     track_rp_22_v_sect;
   Int_t           track_rp_22_u_sect_no;
   Int_t           track_rp_22_v_sect_no;
   UInt_t          track_rp_22_u_id;
   UInt_t          track_rp_22_v_id;
   Int_t           multi_track_rp_22_;
   Bool_t          multi_track_rp_22_valid[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_x[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_y[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_z[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_thx[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_thy[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_chi2[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_chi2ndf[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   UInt_t          multi_track_rp_22_entries[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_res_x[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Double_t        multi_track_rp_22_res_y[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   vector<int>     multi_track_rp_22_u_sect[kMax_CTPPS_2016_multi_track_rp_22];
   vector<int>     multi_track_rp_22_v_sect[kMax_CTPPS_2016_multi_track_rp_22];
   Int_t           multi_track_rp_22_u_sect_no[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   Int_t           multi_track_rp_22_v_sect_no[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   UInt_t          multi_track_rp_22_u_id[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
   UInt_t          multi_track_rp_22_v_id[kMax_CTPPS_2016_multi_track_rp_22];   //[multi_track_rp_22_]
 //RPRootDumpTrackInfo *track_rp_23.;
   Bool_t          track_rp_23_valid;
   Double_t        track_rp_23_x;
   Double_t        track_rp_23_y;
   Double_t        track_rp_23_z;
   Double_t        track_rp_23_thx;
   Double_t        track_rp_23_thy;
   Double_t        track_rp_23_chi2;
   Double_t        track_rp_23_chi2ndf;
   UInt_t          track_rp_23_entries;
   Double_t        track_rp_23_res_x;
   Double_t        track_rp_23_res_y;
   vector<int>     track_rp_23_u_sect;
   vector<int>     track_rp_23_v_sect;
   Int_t           track_rp_23_u_sect_no;
   Int_t           track_rp_23_v_sect_no;
   UInt_t          track_rp_23_u_id;
   UInt_t          track_rp_23_v_id;
   Int_t           multi_track_rp_23_;
   Bool_t          multi_track_rp_23_valid[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_x[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_y[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_z[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_thx[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_thy[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_chi2[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_chi2ndf[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   UInt_t          multi_track_rp_23_entries[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_res_x[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Double_t        multi_track_rp_23_res_y[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   vector<int>     multi_track_rp_23_u_sect[kMax_CTPPS_2016_multi_track_rp_23];
   vector<int>     multi_track_rp_23_v_sect[kMax_CTPPS_2016_multi_track_rp_23];
   Int_t           multi_track_rp_23_u_sect_no[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   Int_t           multi_track_rp_23_v_sect_no[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   UInt_t          multi_track_rp_23_u_id[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
   UInt_t          multi_track_rp_23_v_id[kMax_CTPPS_2016_multi_track_rp_23];   //[multi_track_rp_23_]
 //RPRootDumpTrackInfo *track_rp_24.;
   Bool_t          track_rp_24_valid;
   Double_t        track_rp_24_x;
   Double_t        track_rp_24_y;
   Double_t        track_rp_24_z;
   Double_t        track_rp_24_thx;
   Double_t        track_rp_24_thy;
   Double_t        track_rp_24_chi2;
   Double_t        track_rp_24_chi2ndf;
   UInt_t          track_rp_24_entries;
   Double_t        track_rp_24_res_x;
   Double_t        track_rp_24_res_y;
   vector<int>     track_rp_24_u_sect;
   vector<int>     track_rp_24_v_sect;
   Int_t           track_rp_24_u_sect_no;
   Int_t           track_rp_24_v_sect_no;
   UInt_t          track_rp_24_u_id;
   UInt_t          track_rp_24_v_id;
   Int_t           multi_track_rp_24_;
   Bool_t          multi_track_rp_24_valid[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_x[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_y[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_z[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_thx[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_thy[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_chi2[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_chi2ndf[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   UInt_t          multi_track_rp_24_entries[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_res_x[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Double_t        multi_track_rp_24_res_y[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   vector<int>     multi_track_rp_24_u_sect[kMax_CTPPS_2016_multi_track_rp_24];
   vector<int>     multi_track_rp_24_v_sect[kMax_CTPPS_2016_multi_track_rp_24];
   Int_t           multi_track_rp_24_u_sect_no[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   Int_t           multi_track_rp_24_v_sect_no[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   UInt_t          multi_track_rp_24_u_id[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
   UInt_t          multi_track_rp_24_v_id[kMax_CTPPS_2016_multi_track_rp_24];   //[multi_track_rp_24_]
 //RPRootDumpTrackInfo *track_rp_25.;
   Bool_t          track_rp_25_valid;
   Double_t        track_rp_25_x;
   Double_t        track_rp_25_y;
   Double_t        track_rp_25_z;
   Double_t        track_rp_25_thx;
   Double_t        track_rp_25_thy;
   Double_t        track_rp_25_chi2;
   Double_t        track_rp_25_chi2ndf;
   UInt_t          track_rp_25_entries;
   Double_t        track_rp_25_res_x;
   Double_t        track_rp_25_res_y;
   vector<int>     track_rp_25_u_sect;
   vector<int>     track_rp_25_v_sect;
   Int_t           track_rp_25_u_sect_no;
   Int_t           track_rp_25_v_sect_no;
   UInt_t          track_rp_25_u_id;
   UInt_t          track_rp_25_v_id;
   Int_t           multi_track_rp_25_;
   Bool_t          multi_track_rp_25_valid[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_x[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_y[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_z[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_thx[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_thy[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_chi2[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_chi2ndf[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   UInt_t          multi_track_rp_25_entries[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_res_x[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Double_t        multi_track_rp_25_res_y[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   vector<int>     multi_track_rp_25_u_sect[kMax_CTPPS_2016_multi_track_rp_25];
   vector<int>     multi_track_rp_25_v_sect[kMax_CTPPS_2016_multi_track_rp_25];
   Int_t           multi_track_rp_25_u_sect_no[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   Int_t           multi_track_rp_25_v_sect_no[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   UInt_t          multi_track_rp_25_u_id[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
   UInt_t          multi_track_rp_25_v_id[kMax_CTPPS_2016_multi_track_rp_25];   //[multi_track_rp_25_]
 //RPRootDumpTrackInfo *track_rp_100.;
   Bool_t          track_rp_100_valid;
   Double_t        track_rp_100_x;
   Double_t        track_rp_100_y;
   Double_t        track_rp_100_z;
   Double_t        track_rp_100_thx;
   Double_t        track_rp_100_thy;
   Double_t        track_rp_100_chi2;
   Double_t        track_rp_100_chi2ndf;
   UInt_t          track_rp_100_entries;
   Double_t        track_rp_100_res_x;
   Double_t        track_rp_100_res_y;
   vector<int>     track_rp_100_u_sect;
   vector<int>     track_rp_100_v_sect;
   Int_t           track_rp_100_u_sect_no;
   Int_t           track_rp_100_v_sect_no;
   UInt_t          track_rp_100_u_id;
   UInt_t          track_rp_100_v_id;
   Int_t           multi_track_rp_100_;
   Bool_t          multi_track_rp_100_valid[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_x[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_y[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_z[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_thx[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_thy[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_chi2[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_chi2ndf[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   UInt_t          multi_track_rp_100_entries[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_res_x[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Double_t        multi_track_rp_100_res_y[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   vector<int>     multi_track_rp_100_u_sect[kMax_CTPPS_2016_multi_track_rp_100];
   vector<int>     multi_track_rp_100_v_sect[kMax_CTPPS_2016_multi_track_rp_100];
   Int_t           multi_track_rp_100_u_sect_no[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   Int_t           multi_track_rp_100_v_sect_no[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   UInt_t          multi_track_rp_100_u_id[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
   UInt_t          multi_track_rp_100_v_id[kMax_CTPPS_2016_multi_track_rp_100];   //[multi_track_rp_100_]
 //RPRootDumpTrackInfo *track_rp_101.;
   Bool_t          track_rp_101_valid;
   Double_t        track_rp_101_x;
   Double_t        track_rp_101_y;
   Double_t        track_rp_101_z;
   Double_t        track_rp_101_thx;
   Double_t        track_rp_101_thy;
   Double_t        track_rp_101_chi2;
   Double_t        track_rp_101_chi2ndf;
   UInt_t          track_rp_101_entries;
   Double_t        track_rp_101_res_x;
   Double_t        track_rp_101_res_y;
   vector<int>     track_rp_101_u_sect;
   vector<int>     track_rp_101_v_sect;
   Int_t           track_rp_101_u_sect_no;
   Int_t           track_rp_101_v_sect_no;
   UInt_t          track_rp_101_u_id;
   UInt_t          track_rp_101_v_id;
   Int_t           multi_track_rp_101_;
   Bool_t          multi_track_rp_101_valid[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_x[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_y[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_z[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_thx[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_thy[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_chi2[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_chi2ndf[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   UInt_t          multi_track_rp_101_entries[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_res_x[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Double_t        multi_track_rp_101_res_y[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   vector<int>     multi_track_rp_101_u_sect[kMax_CTPPS_2016_multi_track_rp_101];
   vector<int>     multi_track_rp_101_v_sect[kMax_CTPPS_2016_multi_track_rp_101];
   Int_t           multi_track_rp_101_u_sect_no[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   Int_t           multi_track_rp_101_v_sect_no[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   UInt_t          multi_track_rp_101_u_id[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
   UInt_t          multi_track_rp_101_v_id[kMax_CTPPS_2016_multi_track_rp_101];   //[multi_track_rp_101_]
 //RPRootDumpTrackInfo *track_rp_102.;
   Bool_t          track_rp_102_valid;
   Double_t        track_rp_102_x;
   Double_t        track_rp_102_y;
   Double_t        track_rp_102_z;
   Double_t        track_rp_102_thx;
   Double_t        track_rp_102_thy;
   Double_t        track_rp_102_chi2;
   Double_t        track_rp_102_chi2ndf;
   UInt_t          track_rp_102_entries;
   Double_t        track_rp_102_res_x;
   Double_t        track_rp_102_res_y;
   vector<int>     track_rp_102_u_sect;
   vector<int>     track_rp_102_v_sect;
   Int_t           track_rp_102_u_sect_no;
   Int_t           track_rp_102_v_sect_no;
   UInt_t          track_rp_102_u_id;
   UInt_t          track_rp_102_v_id;
   Int_t           multi_track_rp_102_;
   Bool_t          multi_track_rp_102_valid[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_x[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_y[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_z[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_thx[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_thy[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_chi2[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_chi2ndf[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   UInt_t          multi_track_rp_102_entries[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_res_x[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Double_t        multi_track_rp_102_res_y[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   vector<int>     multi_track_rp_102_u_sect[kMax_CTPPS_2016_multi_track_rp_102];
   vector<int>     multi_track_rp_102_v_sect[kMax_CTPPS_2016_multi_track_rp_102];
   Int_t           multi_track_rp_102_u_sect_no[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   Int_t           multi_track_rp_102_v_sect_no[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   UInt_t          multi_track_rp_102_u_id[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
   UInt_t          multi_track_rp_102_v_id[kMax_CTPPS_2016_multi_track_rp_102];   //[multi_track_rp_102_]
 //RPRootDumpTrackInfo *track_rp_103.;
   Bool_t          track_rp_103_valid;
   Double_t        track_rp_103_x;
   Double_t        track_rp_103_y;
   Double_t        track_rp_103_z;
   Double_t        track_rp_103_thx;
   Double_t        track_rp_103_thy;
   Double_t        track_rp_103_chi2;
   Double_t        track_rp_103_chi2ndf;
   UInt_t          track_rp_103_entries;
   Double_t        track_rp_103_res_x;
   Double_t        track_rp_103_res_y;
   vector<int>     track_rp_103_u_sect;
   vector<int>     track_rp_103_v_sect;
   Int_t           track_rp_103_u_sect_no;
   Int_t           track_rp_103_v_sect_no;
   UInt_t          track_rp_103_u_id;
   UInt_t          track_rp_103_v_id;
   Int_t           multi_track_rp_103_;
   Bool_t          multi_track_rp_103_valid[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_x[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_y[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_z[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_thx[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_thy[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_chi2[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_chi2ndf[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   UInt_t          multi_track_rp_103_entries[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_res_x[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Double_t        multi_track_rp_103_res_y[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   vector<int>     multi_track_rp_103_u_sect[kMax_CTPPS_2016_multi_track_rp_103];
   vector<int>     multi_track_rp_103_v_sect[kMax_CTPPS_2016_multi_track_rp_103];
   Int_t           multi_track_rp_103_u_sect_no[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   Int_t           multi_track_rp_103_v_sect_no[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   UInt_t          multi_track_rp_103_u_id[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
   UInt_t          multi_track_rp_103_v_id[kMax_CTPPS_2016_multi_track_rp_103];   //[multi_track_rp_103_]
 //RPRootDumpTrackInfo *track_rp_104.;
   Bool_t          track_rp_104_valid;
   Double_t        track_rp_104_x;
   Double_t        track_rp_104_y;
   Double_t        track_rp_104_z;
   Double_t        track_rp_104_thx;
   Double_t        track_rp_104_thy;
   Double_t        track_rp_104_chi2;
   Double_t        track_rp_104_chi2ndf;
   UInt_t          track_rp_104_entries;
   Double_t        track_rp_104_res_x;
   Double_t        track_rp_104_res_y;
   vector<int>     track_rp_104_u_sect;
   vector<int>     track_rp_104_v_sect;
   Int_t           track_rp_104_u_sect_no;
   Int_t           track_rp_104_v_sect_no;
   UInt_t          track_rp_104_u_id;
   UInt_t          track_rp_104_v_id;
   Int_t           multi_track_rp_104_;
   Bool_t          multi_track_rp_104_valid[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_x[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_y[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_z[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_thx[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_thy[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_chi2[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_chi2ndf[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   UInt_t          multi_track_rp_104_entries[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_res_x[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Double_t        multi_track_rp_104_res_y[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   vector<int>     multi_track_rp_104_u_sect[kMax_CTPPS_2016_multi_track_rp_104];
   vector<int>     multi_track_rp_104_v_sect[kMax_CTPPS_2016_multi_track_rp_104];
   Int_t           multi_track_rp_104_u_sect_no[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   Int_t           multi_track_rp_104_v_sect_no[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   UInt_t          multi_track_rp_104_u_id[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
   UInt_t          multi_track_rp_104_v_id[kMax_CTPPS_2016_multi_track_rp_104];   //[multi_track_rp_104_]
 //RPRootDumpTrackInfo *track_rp_105.;
   Bool_t          track_rp_105_valid;
   Double_t        track_rp_105_x;
   Double_t        track_rp_105_y;
   Double_t        track_rp_105_z;
   Double_t        track_rp_105_thx;
   Double_t        track_rp_105_thy;
   Double_t        track_rp_105_chi2;
   Double_t        track_rp_105_chi2ndf;
   UInt_t          track_rp_105_entries;
   Double_t        track_rp_105_res_x;
   Double_t        track_rp_105_res_y;
   vector<int>     track_rp_105_u_sect;
   vector<int>     track_rp_105_v_sect;
   Int_t           track_rp_105_u_sect_no;
   Int_t           track_rp_105_v_sect_no;
   UInt_t          track_rp_105_u_id;
   UInt_t          track_rp_105_v_id;
   Int_t           multi_track_rp_105_;
   Bool_t          multi_track_rp_105_valid[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_x[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_y[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_z[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_thx[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_thy[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_chi2[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_chi2ndf[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   UInt_t          multi_track_rp_105_entries[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_res_x[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Double_t        multi_track_rp_105_res_y[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   vector<int>     multi_track_rp_105_u_sect[kMax_CTPPS_2016_multi_track_rp_105];
   vector<int>     multi_track_rp_105_v_sect[kMax_CTPPS_2016_multi_track_rp_105];
   Int_t           multi_track_rp_105_u_sect_no[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   Int_t           multi_track_rp_105_v_sect_no[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   UInt_t          multi_track_rp_105_u_id[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
   UInt_t          multi_track_rp_105_v_id[kMax_CTPPS_2016_multi_track_rp_105];   //[multi_track_rp_105_]
 //RPRootDumpTrackInfo *track_rp_120.;
   Bool_t          track_rp_120_valid;
   Double_t        track_rp_120_x;
   Double_t        track_rp_120_y;
   Double_t        track_rp_120_z;
   Double_t        track_rp_120_thx;
   Double_t        track_rp_120_thy;
   Double_t        track_rp_120_chi2;
   Double_t        track_rp_120_chi2ndf;
   UInt_t          track_rp_120_entries;
   Double_t        track_rp_120_res_x;
   Double_t        track_rp_120_res_y;
   vector<int>     track_rp_120_u_sect;
   vector<int>     track_rp_120_v_sect;
   Int_t           track_rp_120_u_sect_no;
   Int_t           track_rp_120_v_sect_no;
   UInt_t          track_rp_120_u_id;
   UInt_t          track_rp_120_v_id;
   Int_t           multi_track_rp_120_;
   Bool_t          multi_track_rp_120_valid[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_x[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_y[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_z[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_thx[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_thy[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_chi2[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_chi2ndf[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   UInt_t          multi_track_rp_120_entries[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_res_x[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Double_t        multi_track_rp_120_res_y[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   vector<int>     multi_track_rp_120_u_sect[kMax_CTPPS_2016_multi_track_rp_120];
   vector<int>     multi_track_rp_120_v_sect[kMax_CTPPS_2016_multi_track_rp_120];
   Int_t           multi_track_rp_120_u_sect_no[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   Int_t           multi_track_rp_120_v_sect_no[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   UInt_t          multi_track_rp_120_u_id[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
   UInt_t          multi_track_rp_120_v_id[kMax_CTPPS_2016_multi_track_rp_120];   //[multi_track_rp_120_]
 //RPRootDumpTrackInfo *track_rp_121.;
   Bool_t          track_rp_121_valid;
   Double_t        track_rp_121_x;
   Double_t        track_rp_121_y;
   Double_t        track_rp_121_z;
   Double_t        track_rp_121_thx;
   Double_t        track_rp_121_thy;
   Double_t        track_rp_121_chi2;
   Double_t        track_rp_121_chi2ndf;
   UInt_t          track_rp_121_entries;
   Double_t        track_rp_121_res_x;
   Double_t        track_rp_121_res_y;
   vector<int>     track_rp_121_u_sect;
   vector<int>     track_rp_121_v_sect;
   Int_t           track_rp_121_u_sect_no;
   Int_t           track_rp_121_v_sect_no;
   UInt_t          track_rp_121_u_id;
   UInt_t          track_rp_121_v_id;
   Int_t           multi_track_rp_121_;
   Bool_t          multi_track_rp_121_valid[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_x[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_y[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_z[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_thx[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_thy[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_chi2[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_chi2ndf[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   UInt_t          multi_track_rp_121_entries[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_res_x[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Double_t        multi_track_rp_121_res_y[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   vector<int>     multi_track_rp_121_u_sect[kMax_CTPPS_2016_multi_track_rp_121];
   vector<int>     multi_track_rp_121_v_sect[kMax_CTPPS_2016_multi_track_rp_121];
   Int_t           multi_track_rp_121_u_sect_no[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   Int_t           multi_track_rp_121_v_sect_no[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   UInt_t          multi_track_rp_121_u_id[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
   UInt_t          multi_track_rp_121_v_id[kMax_CTPPS_2016_multi_track_rp_121];   //[multi_track_rp_121_]
 //RPRootDumpTrackInfo *track_rp_122.;
   Bool_t          track_rp_122_valid;
   Double_t        track_rp_122_x;
   Double_t        track_rp_122_y;
   Double_t        track_rp_122_z;
   Double_t        track_rp_122_thx;
   Double_t        track_rp_122_thy;
   Double_t        track_rp_122_chi2;
   Double_t        track_rp_122_chi2ndf;
   UInt_t          track_rp_122_entries;
   Double_t        track_rp_122_res_x;
   Double_t        track_rp_122_res_y;
   vector<int>     track_rp_122_u_sect;
   vector<int>     track_rp_122_v_sect;
   Int_t           track_rp_122_u_sect_no;
   Int_t           track_rp_122_v_sect_no;
   UInt_t          track_rp_122_u_id;
   UInt_t          track_rp_122_v_id;
   Int_t           multi_track_rp_122_;
   Bool_t          multi_track_rp_122_valid[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_x[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_y[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_z[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_thx[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_thy[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_chi2[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_chi2ndf[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   UInt_t          multi_track_rp_122_entries[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_res_x[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Double_t        multi_track_rp_122_res_y[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   vector<int>     multi_track_rp_122_u_sect[kMax_CTPPS_2016_multi_track_rp_122];
   vector<int>     multi_track_rp_122_v_sect[kMax_CTPPS_2016_multi_track_rp_122];
   Int_t           multi_track_rp_122_u_sect_no[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   Int_t           multi_track_rp_122_v_sect_no[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   UInt_t          multi_track_rp_122_u_id[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
   UInt_t          multi_track_rp_122_v_id[kMax_CTPPS_2016_multi_track_rp_122];   //[multi_track_rp_122_]
 //RPRootDumpTrackInfo *track_rp_123.;
   Bool_t          track_rp_123_valid;
   Double_t        track_rp_123_x;
   Double_t        track_rp_123_y;
   Double_t        track_rp_123_z;
   Double_t        track_rp_123_thx;
   Double_t        track_rp_123_thy;
   Double_t        track_rp_123_chi2;
   Double_t        track_rp_123_chi2ndf;
   UInt_t          track_rp_123_entries;
   Double_t        track_rp_123_res_x;
   Double_t        track_rp_123_res_y;
   vector<int>     track_rp_123_u_sect;
   vector<int>     track_rp_123_v_sect;
   Int_t           track_rp_123_u_sect_no;
   Int_t           track_rp_123_v_sect_no;
   UInt_t          track_rp_123_u_id;
   UInt_t          track_rp_123_v_id;
   Int_t           multi_track_rp_123_;
   Bool_t          multi_track_rp_123_valid[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_x[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_y[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_z[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_thx[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_thy[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_chi2[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_chi2ndf[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   UInt_t          multi_track_rp_123_entries[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_res_x[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Double_t        multi_track_rp_123_res_y[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   vector<int>     multi_track_rp_123_u_sect[kMax_CTPPS_2016_multi_track_rp_123];
   vector<int>     multi_track_rp_123_v_sect[kMax_CTPPS_2016_multi_track_rp_123];
   Int_t           multi_track_rp_123_u_sect_no[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   Int_t           multi_track_rp_123_v_sect_no[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   UInt_t          multi_track_rp_123_u_id[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
   UInt_t          multi_track_rp_123_v_id[kMax_CTPPS_2016_multi_track_rp_123];   //[multi_track_rp_123_]
 //RPRootDumpTrackInfo *track_rp_124.;
   Bool_t          track_rp_124_valid;
   Double_t        track_rp_124_x;
   Double_t        track_rp_124_y;
   Double_t        track_rp_124_z;
   Double_t        track_rp_124_thx;
   Double_t        track_rp_124_thy;
   Double_t        track_rp_124_chi2;
   Double_t        track_rp_124_chi2ndf;
   UInt_t          track_rp_124_entries;
   Double_t        track_rp_124_res_x;
   Double_t        track_rp_124_res_y;
   vector<int>     track_rp_124_u_sect;
   vector<int>     track_rp_124_v_sect;
   Int_t           track_rp_124_u_sect_no;
   Int_t           track_rp_124_v_sect_no;
   UInt_t          track_rp_124_u_id;
   UInt_t          track_rp_124_v_id;
   Int_t           multi_track_rp_124_;
   Bool_t          multi_track_rp_124_valid[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_x[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_y[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_z[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_thx[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_thy[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_chi2[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_chi2ndf[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   UInt_t          multi_track_rp_124_entries[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_res_x[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Double_t        multi_track_rp_124_res_y[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   vector<int>     multi_track_rp_124_u_sect[kMax_CTPPS_2016_multi_track_rp_124];
   vector<int>     multi_track_rp_124_v_sect[kMax_CTPPS_2016_multi_track_rp_124];
   Int_t           multi_track_rp_124_u_sect_no[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   Int_t           multi_track_rp_124_v_sect_no[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   UInt_t          multi_track_rp_124_u_id[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
   UInt_t          multi_track_rp_124_v_id[kMax_CTPPS_2016_multi_track_rp_124];   //[multi_track_rp_124_]
 //RPRootDumpTrackInfo *track_rp_125.;
   Bool_t          track_rp_125_valid;
   Double_t        track_rp_125_x;
   Double_t        track_rp_125_y;
   Double_t        track_rp_125_z;
   Double_t        track_rp_125_thx;
   Double_t        track_rp_125_thy;
   Double_t        track_rp_125_chi2;
   Double_t        track_rp_125_chi2ndf;
   UInt_t          track_rp_125_entries;
   Double_t        track_rp_125_res_x;
   Double_t        track_rp_125_res_y;
   vector<int>     track_rp_125_u_sect;
   vector<int>     track_rp_125_v_sect;
   Int_t           track_rp_125_u_sect_no;
   Int_t           track_rp_125_v_sect_no;
   UInt_t          track_rp_125_u_id;
   UInt_t          track_rp_125_v_id;
   Int_t           multi_track_rp_125_;
   Bool_t          multi_track_rp_125_valid[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_x[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_y[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_z[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_thx[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_thy[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_chi2[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_chi2ndf[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   UInt_t          multi_track_rp_125_entries[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_res_x[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Double_t        multi_track_rp_125_res_y[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   vector<int>     multi_track_rp_125_u_sect[kMax_CTPPS_2016_multi_track_rp_125];
   vector<int>     multi_track_rp_125_v_sect[kMax_CTPPS_2016_multi_track_rp_125];
   Int_t           multi_track_rp_125_u_sect_no[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   Int_t           multi_track_rp_125_v_sect_no[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   UInt_t          multi_track_rp_125_u_id[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
   UInt_t          multi_track_rp_125_v_id[kMax_CTPPS_2016_multi_track_rp_125];   //[multi_track_rp_125_]
 //RPRootDumpReconstructedProton *rec_prot_left.;
   Bool_t          rec_prot_left_valid;
   Double_t        rec_prot_left_thx;
   Double_t        rec_prot_left_thy;
   Double_t        rec_prot_left_phi;
   Double_t        rec_prot_left_t;
   Double_t        rec_prot_left_tx;
   Double_t        rec_prot_left_ty;
   Double_t        rec_prot_left_xi;
   Double_t        rec_prot_left_x0;
   Double_t        rec_prot_left_y0;
   Double_t        rec_prot_left_chi2;
   Double_t        rec_prot_left_chindf;
 //RPRootDumpReconstructedProton *rec_prot_right.;
   Bool_t          rec_prot_right_valid;
   Double_t        rec_prot_right_thx;
   Double_t        rec_prot_right_thy;
   Double_t        rec_prot_right_phi;
   Double_t        rec_prot_right_t;
   Double_t        rec_prot_right_tx;
   Double_t        rec_prot_right_ty;
   Double_t        rec_prot_right_xi;
   Double_t        rec_prot_right_x0;
   Double_t        rec_prot_right_y0;
   Double_t        rec_prot_right_chi2;
   Double_t        rec_prot_right_chindf;
 //RPRootDumpReconstructedProtonPair *rec_prot_pair.;
   Bool_t          rec_prot_pair_valid;
   Double_t        rec_prot_pair_thxr;
   Double_t        rec_prot_pair_thyr;
   Double_t        rec_prot_pair_xir;
   Double_t        rec_prot_pair_phir;
   Double_t        rec_prot_pair_thxl;
   Double_t        rec_prot_pair_thyl;
   Double_t        rec_prot_pair_xil;
   Double_t        rec_prot_pair_phil;
   Double_t        rec_prot_pair_x0;
   Double_t        rec_prot_pair_y0;
   Double_t        rec_prot_pair_z0;
   Double_t        rec_prot_pair_chi2;
   Double_t        rec_prot_pair_chindf;
   Double_t        rec_prot_pair_tr;
   Double_t        rec_prot_pair_txr;
   Double_t        rec_prot_pair_tyr;
   Double_t        rec_prot_pair_tl;
   Double_t        rec_prot_pair_txl;
   Double_t        rec_prot_pair_tyl;
   Double_t        rec_prot_pair_t;

   // List of branches
   TBranch        *b_event_info_run_no;   //!
   TBranch        *b_event_info_event_no;   //!
   TBranch        *b_event_info_daq_event_number;   //!
   TBranch        *b_event_info_timestamp;   //!
   TBranch        *b_event_info_optoRx_Id;   //!
   TBranch        *b_event_info_optoRx_BX;   //!
   TBranch        *b_event_info_optoRx_LV1;   //!
   TBranch        *b_trigger_data_type;   //!
   TBranch        *b_trigger_data_event_num;   //!
   TBranch        *b_trigger_data_bunch_num;   //!
   TBranch        *b_trigger_data_src_id;   //!
   TBranch        *b_trigger_data_orbit_num;   //!
   TBranch        *b_trigger_data_revision_num;   //!
   TBranch        *b_trigger_data_run_num;   //!
   TBranch        *b_trigger_data_trigger_num;   //!
   TBranch        *b_trigger_data_inhibited_triggers_num;   //!
   TBranch        *b_trigger_data_input_status_bits;   //!
   TBranch        *b_track_rp_0_valid;   //!
   TBranch        *b_track_rp_0_x;   //!
   TBranch        *b_track_rp_0_y;   //!
   TBranch        *b_track_rp_0_z;   //!
   TBranch        *b_track_rp_0_thx;   //!
   TBranch        *b_track_rp_0_thy;   //!
   TBranch        *b_track_rp_0_chi2;   //!
   TBranch        *b_track_rp_0_chi2ndf;   //!
   TBranch        *b_track_rp_0_entries;   //!
   TBranch        *b_track_rp_0_res_x;   //!
   TBranch        *b_track_rp_0_res_y;   //!
   TBranch        *b_track_rp_0_u_sect;   //!
   TBranch        *b_track_rp_0_v_sect;   //!
   TBranch        *b_track_rp_0_u_sect_no;   //!
   TBranch        *b_track_rp_0_v_sect_no;   //!
   TBranch        *b_track_rp_0_u_id;   //!
   TBranch        *b_track_rp_0_v_id;   //!
   TBranch        *b_multi_track_rp_0_;   //!
   TBranch        *b_multi_track_rp_0_valid;   //!
   TBranch        *b_multi_track_rp_0_x;   //!
   TBranch        *b_multi_track_rp_0_y;   //!
   TBranch        *b_multi_track_rp_0_z;   //!
   TBranch        *b_multi_track_rp_0_thx;   //!
   TBranch        *b_multi_track_rp_0_thy;   //!
   TBranch        *b_multi_track_rp_0_chi2;   //!
   TBranch        *b_multi_track_rp_0_chi2ndf;   //!
   TBranch        *b_multi_track_rp_0_entries;   //!
   TBranch        *b_multi_track_rp_0_res_x;   //!
   TBranch        *b_multi_track_rp_0_res_y;   //!
   TBranch        *b_multi_track_rp_0_u_sect;   //!
   TBranch        *b_multi_track_rp_0_v_sect;   //!
   TBranch        *b_multi_track_rp_0_u_sect_no;   //!
   TBranch        *b_multi_track_rp_0_v_sect_no;   //!
   TBranch        *b_multi_track_rp_0_u_id;   //!
   TBranch        *b_multi_track_rp_0_v_id;   //!
   TBranch        *b_track_rp_1_valid;   //!
   TBranch        *b_track_rp_1_x;   //!
   TBranch        *b_track_rp_1_y;   //!
   TBranch        *b_track_rp_1_z;   //!
   TBranch        *b_track_rp_1_thx;   //!
   TBranch        *b_track_rp_1_thy;   //!
   TBranch        *b_track_rp_1_chi2;   //!
   TBranch        *b_track_rp_1_chi2ndf;   //!
   TBranch        *b_track_rp_1_entries;   //!
   TBranch        *b_track_rp_1_res_x;   //!
   TBranch        *b_track_rp_1_res_y;   //!
   TBranch        *b_track_rp_1_u_sect;   //!
   TBranch        *b_track_rp_1_v_sect;   //!
   TBranch        *b_track_rp_1_u_sect_no;   //!
   TBranch        *b_track_rp_1_v_sect_no;   //!
   TBranch        *b_track_rp_1_u_id;   //!
   TBranch        *b_track_rp_1_v_id;   //!
   TBranch        *b_multi_track_rp_1_;   //!
   TBranch        *b_multi_track_rp_1_valid;   //!
   TBranch        *b_multi_track_rp_1_x;   //!
   TBranch        *b_multi_track_rp_1_y;   //!
   TBranch        *b_multi_track_rp_1_z;   //!
   TBranch        *b_multi_track_rp_1_thx;   //!
   TBranch        *b_multi_track_rp_1_thy;   //!
   TBranch        *b_multi_track_rp_1_chi2;   //!
   TBranch        *b_multi_track_rp_1_chi2ndf;   //!
   TBranch        *b_multi_track_rp_1_entries;   //!
   TBranch        *b_multi_track_rp_1_res_x;   //!
   TBranch        *b_multi_track_rp_1_res_y;   //!
   TBranch        *b_multi_track_rp_1_u_sect;   //!
   TBranch        *b_multi_track_rp_1_v_sect;   //!
   TBranch        *b_multi_track_rp_1_u_sect_no;   //!
   TBranch        *b_multi_track_rp_1_v_sect_no;   //!
   TBranch        *b_multi_track_rp_1_u_id;   //!
   TBranch        *b_multi_track_rp_1_v_id;   //!
   TBranch        *b_track_rp_2_valid;   //!
   TBranch        *b_track_rp_2_x;   //!
   TBranch        *b_track_rp_2_y;   //!
   TBranch        *b_track_rp_2_z;   //!
   TBranch        *b_track_rp_2_thx;   //!
   TBranch        *b_track_rp_2_thy;   //!
   TBranch        *b_track_rp_2_chi2;   //!
   TBranch        *b_track_rp_2_chi2ndf;   //!
   TBranch        *b_track_rp_2_entries;   //!
   TBranch        *b_track_rp_2_res_x;   //!
   TBranch        *b_track_rp_2_res_y;   //!
   TBranch        *b_track_rp_2_u_sect;   //!
   TBranch        *b_track_rp_2_v_sect;   //!
   TBranch        *b_track_rp_2_u_sect_no;   //!
   TBranch        *b_track_rp_2_v_sect_no;   //!
   TBranch        *b_track_rp_2_u_id;   //!
   TBranch        *b_track_rp_2_v_id;   //!
   TBranch        *b_multi_track_rp_2_;   //!
   TBranch        *b_multi_track_rp_2_valid;   //!
   TBranch        *b_multi_track_rp_2_x;   //!
   TBranch        *b_multi_track_rp_2_y;   //!
   TBranch        *b_multi_track_rp_2_z;   //!
   TBranch        *b_multi_track_rp_2_thx;   //!
   TBranch        *b_multi_track_rp_2_thy;   //!
   TBranch        *b_multi_track_rp_2_chi2;   //!
   TBranch        *b_multi_track_rp_2_chi2ndf;   //!
   TBranch        *b_multi_track_rp_2_entries;   //!
   TBranch        *b_multi_track_rp_2_res_x;   //!
   TBranch        *b_multi_track_rp_2_res_y;   //!
   TBranch        *b_multi_track_rp_2_u_sect;   //!
   TBranch        *b_multi_track_rp_2_v_sect;   //!
   TBranch        *b_multi_track_rp_2_u_sect_no;   //!
   TBranch        *b_multi_track_rp_2_v_sect_no;   //!
   TBranch        *b_multi_track_rp_2_u_id;   //!
   TBranch        *b_multi_track_rp_2_v_id;   //!
   TBranch        *b_track_rp_3_valid;   //!
   TBranch        *b_track_rp_3_x;   //!
   TBranch        *b_track_rp_3_y;   //!
   TBranch        *b_track_rp_3_z;   //!
   TBranch        *b_track_rp_3_thx;   //!
   TBranch        *b_track_rp_3_thy;   //!
   TBranch        *b_track_rp_3_chi2;   //!
   TBranch        *b_track_rp_3_chi2ndf;   //!
   TBranch        *b_track_rp_3_entries;   //!
   TBranch        *b_track_rp_3_res_x;   //!
   TBranch        *b_track_rp_3_res_y;   //!
   TBranch        *b_track_rp_3_u_sect;   //!
   TBranch        *b_track_rp_3_v_sect;   //!
   TBranch        *b_track_rp_3_u_sect_no;   //!
   TBranch        *b_track_rp_3_v_sect_no;   //!
   TBranch        *b_track_rp_3_u_id;   //!
   TBranch        *b_track_rp_3_v_id;   //!
   TBranch        *b_multi_track_rp_3_;   //!
   TBranch        *b_multi_track_rp_3_valid;   //!
   TBranch        *b_multi_track_rp_3_x;   //!
   TBranch        *b_multi_track_rp_3_y;   //!
   TBranch        *b_multi_track_rp_3_z;   //!
   TBranch        *b_multi_track_rp_3_thx;   //!
   TBranch        *b_multi_track_rp_3_thy;   //!
   TBranch        *b_multi_track_rp_3_chi2;   //!
   TBranch        *b_multi_track_rp_3_chi2ndf;   //!
   TBranch        *b_multi_track_rp_3_entries;   //!
   TBranch        *b_multi_track_rp_3_res_x;   //!
   TBranch        *b_multi_track_rp_3_res_y;   //!
   TBranch        *b_multi_track_rp_3_u_sect;   //!
   TBranch        *b_multi_track_rp_3_v_sect;   //!
   TBranch        *b_multi_track_rp_3_u_sect_no;   //!
   TBranch        *b_multi_track_rp_3_v_sect_no;   //!
   TBranch        *b_multi_track_rp_3_u_id;   //!
   TBranch        *b_multi_track_rp_3_v_id;   //!
   TBranch        *b_track_rp_4_valid;   //!
   TBranch        *b_track_rp_4_x;   //!
   TBranch        *b_track_rp_4_y;   //!
   TBranch        *b_track_rp_4_z;   //!
   TBranch        *b_track_rp_4_thx;   //!
   TBranch        *b_track_rp_4_thy;   //!
   TBranch        *b_track_rp_4_chi2;   //!
   TBranch        *b_track_rp_4_chi2ndf;   //!
   TBranch        *b_track_rp_4_entries;   //!
   TBranch        *b_track_rp_4_res_x;   //!
   TBranch        *b_track_rp_4_res_y;   //!
   TBranch        *b_track_rp_4_u_sect;   //!
   TBranch        *b_track_rp_4_v_sect;   //!
   TBranch        *b_track_rp_4_u_sect_no;   //!
   TBranch        *b_track_rp_4_v_sect_no;   //!
   TBranch        *b_track_rp_4_u_id;   //!
   TBranch        *b_track_rp_4_v_id;   //!
   TBranch        *b_multi_track_rp_4_;   //!
   TBranch        *b_multi_track_rp_4_valid;   //!
   TBranch        *b_multi_track_rp_4_x;   //!
   TBranch        *b_multi_track_rp_4_y;   //!
   TBranch        *b_multi_track_rp_4_z;   //!
   TBranch        *b_multi_track_rp_4_thx;   //!
   TBranch        *b_multi_track_rp_4_thy;   //!
   TBranch        *b_multi_track_rp_4_chi2;   //!
   TBranch        *b_multi_track_rp_4_chi2ndf;   //!
   TBranch        *b_multi_track_rp_4_entries;   //!
   TBranch        *b_multi_track_rp_4_res_x;   //!
   TBranch        *b_multi_track_rp_4_res_y;   //!
   TBranch        *b_multi_track_rp_4_u_sect;   //!
   TBranch        *b_multi_track_rp_4_v_sect;   //!
   TBranch        *b_multi_track_rp_4_u_sect_no;   //!
   TBranch        *b_multi_track_rp_4_v_sect_no;   //!
   TBranch        *b_multi_track_rp_4_u_id;   //!
   TBranch        *b_multi_track_rp_4_v_id;   //!
   TBranch        *b_track_rp_5_valid;   //!
   TBranch        *b_track_rp_5_x;   //!
   TBranch        *b_track_rp_5_y;   //!
   TBranch        *b_track_rp_5_z;   //!
   TBranch        *b_track_rp_5_thx;   //!
   TBranch        *b_track_rp_5_thy;   //!
   TBranch        *b_track_rp_5_chi2;   //!
   TBranch        *b_track_rp_5_chi2ndf;   //!
   TBranch        *b_track_rp_5_entries;   //!
   TBranch        *b_track_rp_5_res_x;   //!
   TBranch        *b_track_rp_5_res_y;   //!
   TBranch        *b_track_rp_5_u_sect;   //!
   TBranch        *b_track_rp_5_v_sect;   //!
   TBranch        *b_track_rp_5_u_sect_no;   //!
   TBranch        *b_track_rp_5_v_sect_no;   //!
   TBranch        *b_track_rp_5_u_id;   //!
   TBranch        *b_track_rp_5_v_id;   //!
   TBranch        *b_multi_track_rp_5_;   //!
   TBranch        *b_multi_track_rp_5_valid;   //!
   TBranch        *b_multi_track_rp_5_x;   //!
   TBranch        *b_multi_track_rp_5_y;   //!
   TBranch        *b_multi_track_rp_5_z;   //!
   TBranch        *b_multi_track_rp_5_thx;   //!
   TBranch        *b_multi_track_rp_5_thy;   //!
   TBranch        *b_multi_track_rp_5_chi2;   //!
   TBranch        *b_multi_track_rp_5_chi2ndf;   //!
   TBranch        *b_multi_track_rp_5_entries;   //!
   TBranch        *b_multi_track_rp_5_res_x;   //!
   TBranch        *b_multi_track_rp_5_res_y;   //!
   TBranch        *b_multi_track_rp_5_u_sect;   //!
   TBranch        *b_multi_track_rp_5_v_sect;   //!
   TBranch        *b_multi_track_rp_5_u_sect_no;   //!
   TBranch        *b_multi_track_rp_5_v_sect_no;   //!
   TBranch        *b_multi_track_rp_5_u_id;   //!
   TBranch        *b_multi_track_rp_5_v_id;   //!
   TBranch        *b_track_rp_20_valid;   //!
   TBranch        *b_track_rp_20_x;   //!
   TBranch        *b_track_rp_20_y;   //!
   TBranch        *b_track_rp_20_z;   //!
   TBranch        *b_track_rp_20_thx;   //!
   TBranch        *b_track_rp_20_thy;   //!
   TBranch        *b_track_rp_20_chi2;   //!
   TBranch        *b_track_rp_20_chi2ndf;   //!
   TBranch        *b_track_rp_20_entries;   //!
   TBranch        *b_track_rp_20_res_x;   //!
   TBranch        *b_track_rp_20_res_y;   //!
   TBranch        *b_track_rp_20_u_sect;   //!
   TBranch        *b_track_rp_20_v_sect;   //!
   TBranch        *b_track_rp_20_u_sect_no;   //!
   TBranch        *b_track_rp_20_v_sect_no;   //!
   TBranch        *b_track_rp_20_u_id;   //!
   TBranch        *b_track_rp_20_v_id;   //!
   TBranch        *b_multi_track_rp_20_;   //!
   TBranch        *b_multi_track_rp_20_valid;   //!
   TBranch        *b_multi_track_rp_20_x;   //!
   TBranch        *b_multi_track_rp_20_y;   //!
   TBranch        *b_multi_track_rp_20_z;   //!
   TBranch        *b_multi_track_rp_20_thx;   //!
   TBranch        *b_multi_track_rp_20_thy;   //!
   TBranch        *b_multi_track_rp_20_chi2;   //!
   TBranch        *b_multi_track_rp_20_chi2ndf;   //!
   TBranch        *b_multi_track_rp_20_entries;   //!
   TBranch        *b_multi_track_rp_20_res_x;   //!
   TBranch        *b_multi_track_rp_20_res_y;   //!
   TBranch        *b_multi_track_rp_20_u_sect;   //!
   TBranch        *b_multi_track_rp_20_v_sect;   //!
   TBranch        *b_multi_track_rp_20_u_sect_no;   //!
   TBranch        *b_multi_track_rp_20_v_sect_no;   //!
   TBranch        *b_multi_track_rp_20_u_id;   //!
   TBranch        *b_multi_track_rp_20_v_id;   //!
   TBranch        *b_track_rp_21_valid;   //!
   TBranch        *b_track_rp_21_x;   //!
   TBranch        *b_track_rp_21_y;   //!
   TBranch        *b_track_rp_21_z;   //!
   TBranch        *b_track_rp_21_thx;   //!
   TBranch        *b_track_rp_21_thy;   //!
   TBranch        *b_track_rp_21_chi2;   //!
   TBranch        *b_track_rp_21_chi2ndf;   //!
   TBranch        *b_track_rp_21_entries;   //!
   TBranch        *b_track_rp_21_res_x;   //!
   TBranch        *b_track_rp_21_res_y;   //!
   TBranch        *b_track_rp_21_u_sect;   //!
   TBranch        *b_track_rp_21_v_sect;   //!
   TBranch        *b_track_rp_21_u_sect_no;   //!
   TBranch        *b_track_rp_21_v_sect_no;   //!
   TBranch        *b_track_rp_21_u_id;   //!
   TBranch        *b_track_rp_21_v_id;   //!
   TBranch        *b_multi_track_rp_21_;   //!
   TBranch        *b_multi_track_rp_21_valid;   //!
   TBranch        *b_multi_track_rp_21_x;   //!
   TBranch        *b_multi_track_rp_21_y;   //!
   TBranch        *b_multi_track_rp_21_z;   //!
   TBranch        *b_multi_track_rp_21_thx;   //!
   TBranch        *b_multi_track_rp_21_thy;   //!
   TBranch        *b_multi_track_rp_21_chi2;   //!
   TBranch        *b_multi_track_rp_21_chi2ndf;   //!
   TBranch        *b_multi_track_rp_21_entries;   //!
   TBranch        *b_multi_track_rp_21_res_x;   //!
   TBranch        *b_multi_track_rp_21_res_y;   //!
   TBranch        *b_multi_track_rp_21_u_sect;   //!
   TBranch        *b_multi_track_rp_21_v_sect;   //!
   TBranch        *b_multi_track_rp_21_u_sect_no;   //!
   TBranch        *b_multi_track_rp_21_v_sect_no;   //!
   TBranch        *b_multi_track_rp_21_u_id;   //!
   TBranch        *b_multi_track_rp_21_v_id;   //!
   TBranch        *b_track_rp_22_valid;   //!
   TBranch        *b_track_rp_22_x;   //!
   TBranch        *b_track_rp_22_y;   //!
   TBranch        *b_track_rp_22_z;   //!
   TBranch        *b_track_rp_22_thx;   //!
   TBranch        *b_track_rp_22_thy;   //!
   TBranch        *b_track_rp_22_chi2;   //!
   TBranch        *b_track_rp_22_chi2ndf;   //!
   TBranch        *b_track_rp_22_entries;   //!
   TBranch        *b_track_rp_22_res_x;   //!
   TBranch        *b_track_rp_22_res_y;   //!
   TBranch        *b_track_rp_22_u_sect;   //!
   TBranch        *b_track_rp_22_v_sect;   //!
   TBranch        *b_track_rp_22_u_sect_no;   //!
   TBranch        *b_track_rp_22_v_sect_no;   //!
   TBranch        *b_track_rp_22_u_id;   //!
   TBranch        *b_track_rp_22_v_id;   //!
   TBranch        *b_multi_track_rp_22_;   //!
   TBranch        *b_multi_track_rp_22_valid;   //!
   TBranch        *b_multi_track_rp_22_x;   //!
   TBranch        *b_multi_track_rp_22_y;   //!
   TBranch        *b_multi_track_rp_22_z;   //!
   TBranch        *b_multi_track_rp_22_thx;   //!
   TBranch        *b_multi_track_rp_22_thy;   //!
   TBranch        *b_multi_track_rp_22_chi2;   //!
   TBranch        *b_multi_track_rp_22_chi2ndf;   //!
   TBranch        *b_multi_track_rp_22_entries;   //!
   TBranch        *b_multi_track_rp_22_res_x;   //!
   TBranch        *b_multi_track_rp_22_res_y;   //!
   TBranch        *b_multi_track_rp_22_u_sect;   //!
   TBranch        *b_multi_track_rp_22_v_sect;   //!
   TBranch        *b_multi_track_rp_22_u_sect_no;   //!
   TBranch        *b_multi_track_rp_22_v_sect_no;   //!
   TBranch        *b_multi_track_rp_22_u_id;   //!
   TBranch        *b_multi_track_rp_22_v_id;   //!
   TBranch        *b_track_rp_23_valid;   //!
   TBranch        *b_track_rp_23_x;   //!
   TBranch        *b_track_rp_23_y;   //!
   TBranch        *b_track_rp_23_z;   //!
   TBranch        *b_track_rp_23_thx;   //!
   TBranch        *b_track_rp_23_thy;   //!
   TBranch        *b_track_rp_23_chi2;   //!
   TBranch        *b_track_rp_23_chi2ndf;   //!
   TBranch        *b_track_rp_23_entries;   //!
   TBranch        *b_track_rp_23_res_x;   //!
   TBranch        *b_track_rp_23_res_y;   //!
   TBranch        *b_track_rp_23_u_sect;   //!
   TBranch        *b_track_rp_23_v_sect;   //!
   TBranch        *b_track_rp_23_u_sect_no;   //!
   TBranch        *b_track_rp_23_v_sect_no;   //!
   TBranch        *b_track_rp_23_u_id;   //!
   TBranch        *b_track_rp_23_v_id;   //!
   TBranch        *b_multi_track_rp_23_;   //!
   TBranch        *b_multi_track_rp_23_valid;   //!
   TBranch        *b_multi_track_rp_23_x;   //!
   TBranch        *b_multi_track_rp_23_y;   //!
   TBranch        *b_multi_track_rp_23_z;   //!
   TBranch        *b_multi_track_rp_23_thx;   //!
   TBranch        *b_multi_track_rp_23_thy;   //!
   TBranch        *b_multi_track_rp_23_chi2;   //!
   TBranch        *b_multi_track_rp_23_chi2ndf;   //!
   TBranch        *b_multi_track_rp_23_entries;   //!
   TBranch        *b_multi_track_rp_23_res_x;   //!
   TBranch        *b_multi_track_rp_23_res_y;   //!
   TBranch        *b_multi_track_rp_23_u_sect;   //!
   TBranch        *b_multi_track_rp_23_v_sect;   //!
   TBranch        *b_multi_track_rp_23_u_sect_no;   //!
   TBranch        *b_multi_track_rp_23_v_sect_no;   //!
   TBranch        *b_multi_track_rp_23_u_id;   //!
   TBranch        *b_multi_track_rp_23_v_id;   //!
   TBranch        *b_track_rp_24_valid;   //!
   TBranch        *b_track_rp_24_x;   //!
   TBranch        *b_track_rp_24_y;   //!
   TBranch        *b_track_rp_24_z;   //!
   TBranch        *b_track_rp_24_thx;   //!
   TBranch        *b_track_rp_24_thy;   //!
   TBranch        *b_track_rp_24_chi2;   //!
   TBranch        *b_track_rp_24_chi2ndf;   //!
   TBranch        *b_track_rp_24_entries;   //!
   TBranch        *b_track_rp_24_res_x;   //!
   TBranch        *b_track_rp_24_res_y;   //!
   TBranch        *b_track_rp_24_u_sect;   //!
   TBranch        *b_track_rp_24_v_sect;   //!
   TBranch        *b_track_rp_24_u_sect_no;   //!
   TBranch        *b_track_rp_24_v_sect_no;   //!
   TBranch        *b_track_rp_24_u_id;   //!
   TBranch        *b_track_rp_24_v_id;   //!
   TBranch        *b_multi_track_rp_24_;   //!
   TBranch        *b_multi_track_rp_24_valid;   //!
   TBranch        *b_multi_track_rp_24_x;   //!
   TBranch        *b_multi_track_rp_24_y;   //!
   TBranch        *b_multi_track_rp_24_z;   //!
   TBranch        *b_multi_track_rp_24_thx;   //!
   TBranch        *b_multi_track_rp_24_thy;   //!
   TBranch        *b_multi_track_rp_24_chi2;   //!
   TBranch        *b_multi_track_rp_24_chi2ndf;   //!
   TBranch        *b_multi_track_rp_24_entries;   //!
   TBranch        *b_multi_track_rp_24_res_x;   //!
   TBranch        *b_multi_track_rp_24_res_y;   //!
   TBranch        *b_multi_track_rp_24_u_sect;   //!
   TBranch        *b_multi_track_rp_24_v_sect;   //!
   TBranch        *b_multi_track_rp_24_u_sect_no;   //!
   TBranch        *b_multi_track_rp_24_v_sect_no;   //!
   TBranch        *b_multi_track_rp_24_u_id;   //!
   TBranch        *b_multi_track_rp_24_v_id;   //!
   TBranch        *b_track_rp_25_valid;   //!
   TBranch        *b_track_rp_25_x;   //!
   TBranch        *b_track_rp_25_y;   //!
   TBranch        *b_track_rp_25_z;   //!
   TBranch        *b_track_rp_25_thx;   //!
   TBranch        *b_track_rp_25_thy;   //!
   TBranch        *b_track_rp_25_chi2;   //!
   TBranch        *b_track_rp_25_chi2ndf;   //!
   TBranch        *b_track_rp_25_entries;   //!
   TBranch        *b_track_rp_25_res_x;   //!
   TBranch        *b_track_rp_25_res_y;   //!
   TBranch        *b_track_rp_25_u_sect;   //!
   TBranch        *b_track_rp_25_v_sect;   //!
   TBranch        *b_track_rp_25_u_sect_no;   //!
   TBranch        *b_track_rp_25_v_sect_no;   //!
   TBranch        *b_track_rp_25_u_id;   //!
   TBranch        *b_track_rp_25_v_id;   //!
   TBranch        *b_multi_track_rp_25_;   //!
   TBranch        *b_multi_track_rp_25_valid;   //!
   TBranch        *b_multi_track_rp_25_x;   //!
   TBranch        *b_multi_track_rp_25_y;   //!
   TBranch        *b_multi_track_rp_25_z;   //!
   TBranch        *b_multi_track_rp_25_thx;   //!
   TBranch        *b_multi_track_rp_25_thy;   //!
   TBranch        *b_multi_track_rp_25_chi2;   //!
   TBranch        *b_multi_track_rp_25_chi2ndf;   //!
   TBranch        *b_multi_track_rp_25_entries;   //!
   TBranch        *b_multi_track_rp_25_res_x;   //!
   TBranch        *b_multi_track_rp_25_res_y;   //!
   TBranch        *b_multi_track_rp_25_u_sect;   //!
   TBranch        *b_multi_track_rp_25_v_sect;   //!
   TBranch        *b_multi_track_rp_25_u_sect_no;   //!
   TBranch        *b_multi_track_rp_25_v_sect_no;   //!
   TBranch        *b_multi_track_rp_25_u_id;   //!
   TBranch        *b_multi_track_rp_25_v_id;   //!
   TBranch        *b_track_rp_100_valid;   //!
   TBranch        *b_track_rp_100_x;   //!
   TBranch        *b_track_rp_100_y;   //!
   TBranch        *b_track_rp_100_z;   //!
   TBranch        *b_track_rp_100_thx;   //!
   TBranch        *b_track_rp_100_thy;   //!
   TBranch        *b_track_rp_100_chi2;   //!
   TBranch        *b_track_rp_100_chi2ndf;   //!
   TBranch        *b_track_rp_100_entries;   //!
   TBranch        *b_track_rp_100_res_x;   //!
   TBranch        *b_track_rp_100_res_y;   //!
   TBranch        *b_track_rp_100_u_sect;   //!
   TBranch        *b_track_rp_100_v_sect;   //!
   TBranch        *b_track_rp_100_u_sect_no;   //!
   TBranch        *b_track_rp_100_v_sect_no;   //!
   TBranch        *b_track_rp_100_u_id;   //!
   TBranch        *b_track_rp_100_v_id;   //!
   TBranch        *b_multi_track_rp_100_;   //!
   TBranch        *b_multi_track_rp_100_valid;   //!
   TBranch        *b_multi_track_rp_100_x;   //!
   TBranch        *b_multi_track_rp_100_y;   //!
   TBranch        *b_multi_track_rp_100_z;   //!
   TBranch        *b_multi_track_rp_100_thx;   //!
   TBranch        *b_multi_track_rp_100_thy;   //!
   TBranch        *b_multi_track_rp_100_chi2;   //!
   TBranch        *b_multi_track_rp_100_chi2ndf;   //!
   TBranch        *b_multi_track_rp_100_entries;   //!
   TBranch        *b_multi_track_rp_100_res_x;   //!
   TBranch        *b_multi_track_rp_100_res_y;   //!
   TBranch        *b_multi_track_rp_100_u_sect;   //!
   TBranch        *b_multi_track_rp_100_v_sect;   //!
   TBranch        *b_multi_track_rp_100_u_sect_no;   //!
   TBranch        *b_multi_track_rp_100_v_sect_no;   //!
   TBranch        *b_multi_track_rp_100_u_id;   //!
   TBranch        *b_multi_track_rp_100_v_id;   //!
   TBranch        *b_track_rp_101_valid;   //!
   TBranch        *b_track_rp_101_x;   //!
   TBranch        *b_track_rp_101_y;   //!
   TBranch        *b_track_rp_101_z;   //!
   TBranch        *b_track_rp_101_thx;   //!
   TBranch        *b_track_rp_101_thy;   //!
   TBranch        *b_track_rp_101_chi2;   //!
   TBranch        *b_track_rp_101_chi2ndf;   //!
   TBranch        *b_track_rp_101_entries;   //!
   TBranch        *b_track_rp_101_res_x;   //!
   TBranch        *b_track_rp_101_res_y;   //!
   TBranch        *b_track_rp_101_u_sect;   //!
   TBranch        *b_track_rp_101_v_sect;   //!
   TBranch        *b_track_rp_101_u_sect_no;   //!
   TBranch        *b_track_rp_101_v_sect_no;   //!
   TBranch        *b_track_rp_101_u_id;   //!
   TBranch        *b_track_rp_101_v_id;   //!
   TBranch        *b_multi_track_rp_101_;   //!
   TBranch        *b_multi_track_rp_101_valid;   //!
   TBranch        *b_multi_track_rp_101_x;   //!
   TBranch        *b_multi_track_rp_101_y;   //!
   TBranch        *b_multi_track_rp_101_z;   //!
   TBranch        *b_multi_track_rp_101_thx;   //!
   TBranch        *b_multi_track_rp_101_thy;   //!
   TBranch        *b_multi_track_rp_101_chi2;   //!
   TBranch        *b_multi_track_rp_101_chi2ndf;   //!
   TBranch        *b_multi_track_rp_101_entries;   //!
   TBranch        *b_multi_track_rp_101_res_x;   //!
   TBranch        *b_multi_track_rp_101_res_y;   //!
   TBranch        *b_multi_track_rp_101_u_sect;   //!
   TBranch        *b_multi_track_rp_101_v_sect;   //!
   TBranch        *b_multi_track_rp_101_u_sect_no;   //!
   TBranch        *b_multi_track_rp_101_v_sect_no;   //!
   TBranch        *b_multi_track_rp_101_u_id;   //!
   TBranch        *b_multi_track_rp_101_v_id;   //!
   TBranch        *b_track_rp_102_valid;   //!
   TBranch        *b_track_rp_102_x;   //!
   TBranch        *b_track_rp_102_y;   //!
   TBranch        *b_track_rp_102_z;   //!
   TBranch        *b_track_rp_102_thx;   //!
   TBranch        *b_track_rp_102_thy;   //!
   TBranch        *b_track_rp_102_chi2;   //!
   TBranch        *b_track_rp_102_chi2ndf;   //!
   TBranch        *b_track_rp_102_entries;   //!
   TBranch        *b_track_rp_102_res_x;   //!
   TBranch        *b_track_rp_102_res_y;   //!
   TBranch        *b_track_rp_102_u_sect;   //!
   TBranch        *b_track_rp_102_v_sect;   //!
   TBranch        *b_track_rp_102_u_sect_no;   //!
   TBranch        *b_track_rp_102_v_sect_no;   //!
   TBranch        *b_track_rp_102_u_id;   //!
   TBranch        *b_track_rp_102_v_id;   //!
   TBranch        *b_multi_track_rp_102_;   //!
   TBranch        *b_multi_track_rp_102_valid;   //!
   TBranch        *b_multi_track_rp_102_x;   //!
   TBranch        *b_multi_track_rp_102_y;   //!
   TBranch        *b_multi_track_rp_102_z;   //!
   TBranch        *b_multi_track_rp_102_thx;   //!
   TBranch        *b_multi_track_rp_102_thy;   //!
   TBranch        *b_multi_track_rp_102_chi2;   //!
   TBranch        *b_multi_track_rp_102_chi2ndf;   //!
   TBranch        *b_multi_track_rp_102_entries;   //!
   TBranch        *b_multi_track_rp_102_res_x;   //!
   TBranch        *b_multi_track_rp_102_res_y;   //!
   TBranch        *b_multi_track_rp_102_u_sect;   //!
   TBranch        *b_multi_track_rp_102_v_sect;   //!
   TBranch        *b_multi_track_rp_102_u_sect_no;   //!
   TBranch        *b_multi_track_rp_102_v_sect_no;   //!
   TBranch        *b_multi_track_rp_102_u_id;   //!
   TBranch        *b_multi_track_rp_102_v_id;   //!
   TBranch        *b_track_rp_103_valid;   //!
   TBranch        *b_track_rp_103_x;   //!
   TBranch        *b_track_rp_103_y;   //!
   TBranch        *b_track_rp_103_z;   //!
   TBranch        *b_track_rp_103_thx;   //!
   TBranch        *b_track_rp_103_thy;   //!
   TBranch        *b_track_rp_103_chi2;   //!
   TBranch        *b_track_rp_103_chi2ndf;   //!
   TBranch        *b_track_rp_103_entries;   //!
   TBranch        *b_track_rp_103_res_x;   //!
   TBranch        *b_track_rp_103_res_y;   //!
   TBranch        *b_track_rp_103_u_sect;   //!
   TBranch        *b_track_rp_103_v_sect;   //!
   TBranch        *b_track_rp_103_u_sect_no;   //!
   TBranch        *b_track_rp_103_v_sect_no;   //!
   TBranch        *b_track_rp_103_u_id;   //!
   TBranch        *b_track_rp_103_v_id;   //!
   TBranch        *b_multi_track_rp_103_;   //!
   TBranch        *b_multi_track_rp_103_valid;   //!
   TBranch        *b_multi_track_rp_103_x;   //!
   TBranch        *b_multi_track_rp_103_y;   //!
   TBranch        *b_multi_track_rp_103_z;   //!
   TBranch        *b_multi_track_rp_103_thx;   //!
   TBranch        *b_multi_track_rp_103_thy;   //!
   TBranch        *b_multi_track_rp_103_chi2;   //!
   TBranch        *b_multi_track_rp_103_chi2ndf;   //!
   TBranch        *b_multi_track_rp_103_entries;   //!
   TBranch        *b_multi_track_rp_103_res_x;   //!
   TBranch        *b_multi_track_rp_103_res_y;   //!
   TBranch        *b_multi_track_rp_103_u_sect;   //!
   TBranch        *b_multi_track_rp_103_v_sect;   //!
   TBranch        *b_multi_track_rp_103_u_sect_no;   //!
   TBranch        *b_multi_track_rp_103_v_sect_no;   //!
   TBranch        *b_multi_track_rp_103_u_id;   //!
   TBranch        *b_multi_track_rp_103_v_id;   //!
   TBranch        *b_track_rp_104_valid;   //!
   TBranch        *b_track_rp_104_x;   //!
   TBranch        *b_track_rp_104_y;   //!
   TBranch        *b_track_rp_104_z;   //!
   TBranch        *b_track_rp_104_thx;   //!
   TBranch        *b_track_rp_104_thy;   //!
   TBranch        *b_track_rp_104_chi2;   //!
   TBranch        *b_track_rp_104_chi2ndf;   //!
   TBranch        *b_track_rp_104_entries;   //!
   TBranch        *b_track_rp_104_res_x;   //!
   TBranch        *b_track_rp_104_res_y;   //!
   TBranch        *b_track_rp_104_u_sect;   //!
   TBranch        *b_track_rp_104_v_sect;   //!
   TBranch        *b_track_rp_104_u_sect_no;   //!
   TBranch        *b_track_rp_104_v_sect_no;   //!
   TBranch        *b_track_rp_104_u_id;   //!
   TBranch        *b_track_rp_104_v_id;   //!
   TBranch        *b_multi_track_rp_104_;   //!
   TBranch        *b_multi_track_rp_104_valid;   //!
   TBranch        *b_multi_track_rp_104_x;   //!
   TBranch        *b_multi_track_rp_104_y;   //!
   TBranch        *b_multi_track_rp_104_z;   //!
   TBranch        *b_multi_track_rp_104_thx;   //!
   TBranch        *b_multi_track_rp_104_thy;   //!
   TBranch        *b_multi_track_rp_104_chi2;   //!
   TBranch        *b_multi_track_rp_104_chi2ndf;   //!
   TBranch        *b_multi_track_rp_104_entries;   //!
   TBranch        *b_multi_track_rp_104_res_x;   //!
   TBranch        *b_multi_track_rp_104_res_y;   //!
   TBranch        *b_multi_track_rp_104_u_sect;   //!
   TBranch        *b_multi_track_rp_104_v_sect;   //!
   TBranch        *b_multi_track_rp_104_u_sect_no;   //!
   TBranch        *b_multi_track_rp_104_v_sect_no;   //!
   TBranch        *b_multi_track_rp_104_u_id;   //!
   TBranch        *b_multi_track_rp_104_v_id;   //!
   TBranch        *b_track_rp_105_valid;   //!
   TBranch        *b_track_rp_105_x;   //!
   TBranch        *b_track_rp_105_y;   //!
   TBranch        *b_track_rp_105_z;   //!
   TBranch        *b_track_rp_105_thx;   //!
   TBranch        *b_track_rp_105_thy;   //!
   TBranch        *b_track_rp_105_chi2;   //!
   TBranch        *b_track_rp_105_chi2ndf;   //!
   TBranch        *b_track_rp_105_entries;   //!
   TBranch        *b_track_rp_105_res_x;   //!
   TBranch        *b_track_rp_105_res_y;   //!
   TBranch        *b_track_rp_105_u_sect;   //!
   TBranch        *b_track_rp_105_v_sect;   //!
   TBranch        *b_track_rp_105_u_sect_no;   //!
   TBranch        *b_track_rp_105_v_sect_no;   //!
   TBranch        *b_track_rp_105_u_id;   //!
   TBranch        *b_track_rp_105_v_id;   //!
   TBranch        *b_multi_track_rp_105_;   //!
   TBranch        *b_multi_track_rp_105_valid;   //!
   TBranch        *b_multi_track_rp_105_x;   //!
   TBranch        *b_multi_track_rp_105_y;   //!
   TBranch        *b_multi_track_rp_105_z;   //!
   TBranch        *b_multi_track_rp_105_thx;   //!
   TBranch        *b_multi_track_rp_105_thy;   //!
   TBranch        *b_multi_track_rp_105_chi2;   //!
   TBranch        *b_multi_track_rp_105_chi2ndf;   //!
   TBranch        *b_multi_track_rp_105_entries;   //!
   TBranch        *b_multi_track_rp_105_res_x;   //!
   TBranch        *b_multi_track_rp_105_res_y;   //!
   TBranch        *b_multi_track_rp_105_u_sect;   //!
   TBranch        *b_multi_track_rp_105_v_sect;   //!
   TBranch        *b_multi_track_rp_105_u_sect_no;   //!
   TBranch        *b_multi_track_rp_105_v_sect_no;   //!
   TBranch        *b_multi_track_rp_105_u_id;   //!
   TBranch        *b_multi_track_rp_105_v_id;   //!
   TBranch        *b_track_rp_120_valid;   //!
   TBranch        *b_track_rp_120_x;   //!
   TBranch        *b_track_rp_120_y;   //!
   TBranch        *b_track_rp_120_z;   //!
   TBranch        *b_track_rp_120_thx;   //!
   TBranch        *b_track_rp_120_thy;   //!
   TBranch        *b_track_rp_120_chi2;   //!
   TBranch        *b_track_rp_120_chi2ndf;   //!
   TBranch        *b_track_rp_120_entries;   //!
   TBranch        *b_track_rp_120_res_x;   //!
   TBranch        *b_track_rp_120_res_y;   //!
   TBranch        *b_track_rp_120_u_sect;   //!
   TBranch        *b_track_rp_120_v_sect;   //!
   TBranch        *b_track_rp_120_u_sect_no;   //!
   TBranch        *b_track_rp_120_v_sect_no;   //!
   TBranch        *b_track_rp_120_u_id;   //!
   TBranch        *b_track_rp_120_v_id;   //!
   TBranch        *b_multi_track_rp_120_;   //!
   TBranch        *b_multi_track_rp_120_valid;   //!
   TBranch        *b_multi_track_rp_120_x;   //!
   TBranch        *b_multi_track_rp_120_y;   //!
   TBranch        *b_multi_track_rp_120_z;   //!
   TBranch        *b_multi_track_rp_120_thx;   //!
   TBranch        *b_multi_track_rp_120_thy;   //!
   TBranch        *b_multi_track_rp_120_chi2;   //!
   TBranch        *b_multi_track_rp_120_chi2ndf;   //!
   TBranch        *b_multi_track_rp_120_entries;   //!
   TBranch        *b_multi_track_rp_120_res_x;   //!
   TBranch        *b_multi_track_rp_120_res_y;   //!
   TBranch        *b_multi_track_rp_120_u_sect;   //!
   TBranch        *b_multi_track_rp_120_v_sect;   //!
   TBranch        *b_multi_track_rp_120_u_sect_no;   //!
   TBranch        *b_multi_track_rp_120_v_sect_no;   //!
   TBranch        *b_multi_track_rp_120_u_id;   //!
   TBranch        *b_multi_track_rp_120_v_id;   //!
   TBranch        *b_track_rp_121_valid;   //!
   TBranch        *b_track_rp_121_x;   //!
   TBranch        *b_track_rp_121_y;   //!
   TBranch        *b_track_rp_121_z;   //!
   TBranch        *b_track_rp_121_thx;   //!
   TBranch        *b_track_rp_121_thy;   //!
   TBranch        *b_track_rp_121_chi2;   //!
   TBranch        *b_track_rp_121_chi2ndf;   //!
   TBranch        *b_track_rp_121_entries;   //!
   TBranch        *b_track_rp_121_res_x;   //!
   TBranch        *b_track_rp_121_res_y;   //!
   TBranch        *b_track_rp_121_u_sect;   //!
   TBranch        *b_track_rp_121_v_sect;   //!
   TBranch        *b_track_rp_121_u_sect_no;   //!
   TBranch        *b_track_rp_121_v_sect_no;   //!
   TBranch        *b_track_rp_121_u_id;   //!
   TBranch        *b_track_rp_121_v_id;   //!
   TBranch        *b_multi_track_rp_121_;   //!
   TBranch        *b_multi_track_rp_121_valid;   //!
   TBranch        *b_multi_track_rp_121_x;   //!
   TBranch        *b_multi_track_rp_121_y;   //!
   TBranch        *b_multi_track_rp_121_z;   //!
   TBranch        *b_multi_track_rp_121_thx;   //!
   TBranch        *b_multi_track_rp_121_thy;   //!
   TBranch        *b_multi_track_rp_121_chi2;   //!
   TBranch        *b_multi_track_rp_121_chi2ndf;   //!
   TBranch        *b_multi_track_rp_121_entries;   //!
   TBranch        *b_multi_track_rp_121_res_x;   //!
   TBranch        *b_multi_track_rp_121_res_y;   //!
   TBranch        *b_multi_track_rp_121_u_sect;   //!
   TBranch        *b_multi_track_rp_121_v_sect;   //!
   TBranch        *b_multi_track_rp_121_u_sect_no;   //!
   TBranch        *b_multi_track_rp_121_v_sect_no;   //!
   TBranch        *b_multi_track_rp_121_u_id;   //!
   TBranch        *b_multi_track_rp_121_v_id;   //!
   TBranch        *b_track_rp_122_valid;   //!
   TBranch        *b_track_rp_122_x;   //!
   TBranch        *b_track_rp_122_y;   //!
   TBranch        *b_track_rp_122_z;   //!
   TBranch        *b_track_rp_122_thx;   //!
   TBranch        *b_track_rp_122_thy;   //!
   TBranch        *b_track_rp_122_chi2;   //!
   TBranch        *b_track_rp_122_chi2ndf;   //!
   TBranch        *b_track_rp_122_entries;   //!
   TBranch        *b_track_rp_122_res_x;   //!
   TBranch        *b_track_rp_122_res_y;   //!
   TBranch        *b_track_rp_122_u_sect;   //!
   TBranch        *b_track_rp_122_v_sect;   //!
   TBranch        *b_track_rp_122_u_sect_no;   //!
   TBranch        *b_track_rp_122_v_sect_no;   //!
   TBranch        *b_track_rp_122_u_id;   //!
   TBranch        *b_track_rp_122_v_id;   //!
   TBranch        *b_multi_track_rp_122_;   //!
   TBranch        *b_multi_track_rp_122_valid;   //!
   TBranch        *b_multi_track_rp_122_x;   //!
   TBranch        *b_multi_track_rp_122_y;   //!
   TBranch        *b_multi_track_rp_122_z;   //!
   TBranch        *b_multi_track_rp_122_thx;   //!
   TBranch        *b_multi_track_rp_122_thy;   //!
   TBranch        *b_multi_track_rp_122_chi2;   //!
   TBranch        *b_multi_track_rp_122_chi2ndf;   //!
   TBranch        *b_multi_track_rp_122_entries;   //!
   TBranch        *b_multi_track_rp_122_res_x;   //!
   TBranch        *b_multi_track_rp_122_res_y;   //!
   TBranch        *b_multi_track_rp_122_u_sect;   //!
   TBranch        *b_multi_track_rp_122_v_sect;   //!
   TBranch        *b_multi_track_rp_122_u_sect_no;   //!
   TBranch        *b_multi_track_rp_122_v_sect_no;   //!
   TBranch        *b_multi_track_rp_122_u_id;   //!
   TBranch        *b_multi_track_rp_122_v_id;   //!
   TBranch        *b_track_rp_123_valid;   //!
   TBranch        *b_track_rp_123_x;   //!
   TBranch        *b_track_rp_123_y;   //!
   TBranch        *b_track_rp_123_z;   //!
   TBranch        *b_track_rp_123_thx;   //!
   TBranch        *b_track_rp_123_thy;   //!
   TBranch        *b_track_rp_123_chi2;   //!
   TBranch        *b_track_rp_123_chi2ndf;   //!
   TBranch        *b_track_rp_123_entries;   //!
   TBranch        *b_track_rp_123_res_x;   //!
   TBranch        *b_track_rp_123_res_y;   //!
   TBranch        *b_track_rp_123_u_sect;   //!
   TBranch        *b_track_rp_123_v_sect;   //!
   TBranch        *b_track_rp_123_u_sect_no;   //!
   TBranch        *b_track_rp_123_v_sect_no;   //!
   TBranch        *b_track_rp_123_u_id;   //!
   TBranch        *b_track_rp_123_v_id;   //!
   TBranch        *b_multi_track_rp_123_;   //!
   TBranch        *b_multi_track_rp_123_valid;   //!
   TBranch        *b_multi_track_rp_123_x;   //!
   TBranch        *b_multi_track_rp_123_y;   //!
   TBranch        *b_multi_track_rp_123_z;   //!
   TBranch        *b_multi_track_rp_123_thx;   //!
   TBranch        *b_multi_track_rp_123_thy;   //!
   TBranch        *b_multi_track_rp_123_chi2;   //!
   TBranch        *b_multi_track_rp_123_chi2ndf;   //!
   TBranch        *b_multi_track_rp_123_entries;   //!
   TBranch        *b_multi_track_rp_123_res_x;   //!
   TBranch        *b_multi_track_rp_123_res_y;   //!
   TBranch        *b_multi_track_rp_123_u_sect;   //!
   TBranch        *b_multi_track_rp_123_v_sect;   //!
   TBranch        *b_multi_track_rp_123_u_sect_no;   //!
   TBranch        *b_multi_track_rp_123_v_sect_no;   //!
   TBranch        *b_multi_track_rp_123_u_id;   //!
   TBranch        *b_multi_track_rp_123_v_id;   //!
   TBranch        *b_track_rp_124_valid;   //!
   TBranch        *b_track_rp_124_x;   //!
   TBranch        *b_track_rp_124_y;   //!
   TBranch        *b_track_rp_124_z;   //!
   TBranch        *b_track_rp_124_thx;   //!
   TBranch        *b_track_rp_124_thy;   //!
   TBranch        *b_track_rp_124_chi2;   //!
   TBranch        *b_track_rp_124_chi2ndf;   //!
   TBranch        *b_track_rp_124_entries;   //!
   TBranch        *b_track_rp_124_res_x;   //!
   TBranch        *b_track_rp_124_res_y;   //!
   TBranch        *b_track_rp_124_u_sect;   //!
   TBranch        *b_track_rp_124_v_sect;   //!
   TBranch        *b_track_rp_124_u_sect_no;   //!
   TBranch        *b_track_rp_124_v_sect_no;   //!
   TBranch        *b_track_rp_124_u_id;   //!
   TBranch        *b_track_rp_124_v_id;   //!
   TBranch        *b_multi_track_rp_124_;   //!
   TBranch        *b_multi_track_rp_124_valid;   //!
   TBranch        *b_multi_track_rp_124_x;   //!
   TBranch        *b_multi_track_rp_124_y;   //!
   TBranch        *b_multi_track_rp_124_z;   //!
   TBranch        *b_multi_track_rp_124_thx;   //!
   TBranch        *b_multi_track_rp_124_thy;   //!
   TBranch        *b_multi_track_rp_124_chi2;   //!
   TBranch        *b_multi_track_rp_124_chi2ndf;   //!
   TBranch        *b_multi_track_rp_124_entries;   //!
   TBranch        *b_multi_track_rp_124_res_x;   //!
   TBranch        *b_multi_track_rp_124_res_y;   //!
   TBranch        *b_multi_track_rp_124_u_sect;   //!
   TBranch        *b_multi_track_rp_124_v_sect;   //!
   TBranch        *b_multi_track_rp_124_u_sect_no;   //!
   TBranch        *b_multi_track_rp_124_v_sect_no;   //!
   TBranch        *b_multi_track_rp_124_u_id;   //!
   TBranch        *b_multi_track_rp_124_v_id;   //!
   TBranch        *b_track_rp_125_valid;   //!
   TBranch        *b_track_rp_125_x;   //!
   TBranch        *b_track_rp_125_y;   //!
   TBranch        *b_track_rp_125_z;   //!
   TBranch        *b_track_rp_125_thx;   //!
   TBranch        *b_track_rp_125_thy;   //!
   TBranch        *b_track_rp_125_chi2;   //!
   TBranch        *b_track_rp_125_chi2ndf;   //!
   TBranch        *b_track_rp_125_entries;   //!
   TBranch        *b_track_rp_125_res_x;   //!
   TBranch        *b_track_rp_125_res_y;   //!
   TBranch        *b_track_rp_125_u_sect;   //!
   TBranch        *b_track_rp_125_v_sect;   //!
   TBranch        *b_track_rp_125_u_sect_no;   //!
   TBranch        *b_track_rp_125_v_sect_no;   //!
   TBranch        *b_track_rp_125_u_id;   //!
   TBranch        *b_track_rp_125_v_id;   //!
   TBranch        *b_multi_track_rp_125_;   //!
   TBranch        *b_multi_track_rp_125_valid;   //!
   TBranch        *b_multi_track_rp_125_x;   //!
   TBranch        *b_multi_track_rp_125_y;   //!
   TBranch        *b_multi_track_rp_125_z;   //!
   TBranch        *b_multi_track_rp_125_thx;   //!
   TBranch        *b_multi_track_rp_125_thy;   //!
   TBranch        *b_multi_track_rp_125_chi2;   //!
   TBranch        *b_multi_track_rp_125_chi2ndf;   //!
   TBranch        *b_multi_track_rp_125_entries;   //!
   TBranch        *b_multi_track_rp_125_res_x;   //!
   TBranch        *b_multi_track_rp_125_res_y;   //!
   TBranch        *b_multi_track_rp_125_u_sect;   //!
   TBranch        *b_multi_track_rp_125_v_sect;   //!
   TBranch        *b_multi_track_rp_125_u_sect_no;   //!
   TBranch        *b_multi_track_rp_125_v_sect_no;   //!
   TBranch        *b_multi_track_rp_125_u_id;   //!
   TBranch        *b_multi_track_rp_125_v_id;   //!
   TBranch        *b_rec_prot_left_valid;   //!
   TBranch        *b_rec_prot_left_thx;   //!
   TBranch        *b_rec_prot_left_thy;   //!
   TBranch        *b_rec_prot_left_phi;   //!
   TBranch        *b_rec_prot_left_t;   //!
   TBranch        *b_rec_prot_left_tx;   //!
   TBranch        *b_rec_prot_left_ty;   //!
   TBranch        *b_rec_prot_left_xi;   //!
   TBranch        *b_rec_prot_left_x0;   //!
   TBranch        *b_rec_prot_left_y0;   //!
   TBranch        *b_rec_prot_left_chi2;   //!
   TBranch        *b_rec_prot_left_chindf;   //!
   TBranch        *b_rec_prot_right_valid;   //!
   TBranch        *b_rec_prot_right_thx;   //!
   TBranch        *b_rec_prot_right_thy;   //!
   TBranch        *b_rec_prot_right_phi;   //!
   TBranch        *b_rec_prot_right_t;   //!
   TBranch        *b_rec_prot_right_tx;   //!
   TBranch        *b_rec_prot_right_ty;   //!
   TBranch        *b_rec_prot_right_xi;   //!
   TBranch        *b_rec_prot_right_x0;   //!
   TBranch        *b_rec_prot_right_y0;   //!
   TBranch        *b_rec_prot_right_chi2;   //!
   TBranch        *b_rec_prot_right_chindf;   //!
   TBranch        *b_rec_prot_pair_valid;   //!
   TBranch        *b_rec_prot_pair_thxr;   //!
   TBranch        *b_rec_prot_pair_thyr;   //!
   TBranch        *b_rec_prot_pair_xir;   //!
   TBranch        *b_rec_prot_pair_phir;   //!
   TBranch        *b_rec_prot_pair_thxl;   //!
   TBranch        *b_rec_prot_pair_thyl;   //!
   TBranch        *b_rec_prot_pair_xil;   //!
   TBranch        *b_rec_prot_pair_phil;   //!
   TBranch        *b_rec_prot_pair_x0;   //!
   TBranch        *b_rec_prot_pair_y0;   //!
   TBranch        *b_rec_prot_pair_z0;   //!
   TBranch        *b_rec_prot_pair_chi2;   //!
   TBranch        *b_rec_prot_pair_chindf;   //!
   TBranch        *b_rec_prot_pair_tr;   //!
   TBranch        *b_rec_prot_pair_txr;   //!
   TBranch        *b_rec_prot_pair_tyr;   //!
   TBranch        *b_rec_prot_pair_tl;   //!
   TBranch        *b_rec_prot_pair_txl;   //!
   TBranch        *b_rec_prot_pair_tyl;   //!
   TBranch        *b_rec_prot_pair_t;   //!

   TotemNtuple_CTPPS_2016(string, string, TTree *tree=0);
   virtual ~TotemNtuple_CTPPS_2016();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TProjectParameters *, TProtonReconstruction *, map_from_string_to_PTPlotsCollection_type *, map<UInt_t, ULong64_t> *) ;
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TotemNtuple_CTPPS_2016_cxx
TotemNtuple_CTPPS_2016::TotemNtuple_CTPPS_2016(string root_file_name, string input_directory, TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eostotem.cern.ch//eos/totem/user/j/jkaspar/reco/alignment_sr/10077_EVB12_1_ntuple.root");

      string complete_filename = input_directory + root_file_name ;

      cout << root_file_name << endl ;      

      TFile *f = TFile::Open(complete_filename.c_str()); 

      if (!f) {
         f = new TFile("root://eostotem.cern.ch//eos/totem/user/j/jkaspar/reco/alignment_sr/10077_EVB12_1_ntuple.root");
      }
      tree = (TTree*)gDirectory->Get("TotemNtuple");

   }
   Init(tree);
}

TotemNtuple_CTPPS_2016::~TotemNtuple_CTPPS_2016()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TotemNtuple_CTPPS_2016::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TotemNtuple_CTPPS_2016::LoadTree(Long64_t entry)
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

void TotemNtuple_CTPPS_2016::Init(TTree *tree)
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

   fChain->SetBranchAddress("event_info.run_no", &event_info_run_no, &b_event_info_run_no);
   fChain->SetBranchAddress("event_info.event_no", &event_info_event_no, &b_event_info_event_no);
   fChain->SetBranchAddress("event_info.daq_event_number", &event_info_daq_event_number, &b_event_info_daq_event_number);
   fChain->SetBranchAddress("event_info.timestamp", &event_info_timestamp, &b_event_info_timestamp);
   fChain->SetBranchAddress("event_info.optoRx_Id", &event_info_optoRx_Id, &b_event_info_optoRx_Id);
   fChain->SetBranchAddress("event_info.optoRx_BX", &event_info_optoRx_BX, &b_event_info_optoRx_BX);
   fChain->SetBranchAddress("event_info.optoRx_LV1", &event_info_optoRx_LV1, &b_event_info_optoRx_LV1);
   fChain->SetBranchAddress("trigger_data.type", &trigger_data_type, &b_trigger_data_type);
   fChain->SetBranchAddress("trigger_data.event_num", &trigger_data_event_num, &b_trigger_data_event_num);
   fChain->SetBranchAddress("trigger_data.bunch_num", &trigger_data_bunch_num, &b_trigger_data_bunch_num);
   fChain->SetBranchAddress("trigger_data.src_id", &trigger_data_src_id, &b_trigger_data_src_id);
   fChain->SetBranchAddress("trigger_data.orbit_num", &trigger_data_orbit_num, &b_trigger_data_orbit_num);
   fChain->SetBranchAddress("trigger_data.revision_num", &trigger_data_revision_num, &b_trigger_data_revision_num);
   fChain->SetBranchAddress("trigger_data.run_num", &trigger_data_run_num, &b_trigger_data_run_num);
   fChain->SetBranchAddress("trigger_data.trigger_num", &trigger_data_trigger_num, &b_trigger_data_trigger_num);
   fChain->SetBranchAddress("trigger_data.inhibited_triggers_num", &trigger_data_inhibited_triggers_num, &b_trigger_data_inhibited_triggers_num);
   fChain->SetBranchAddress("trigger_data.input_status_bits", &trigger_data_input_status_bits, &b_trigger_data_input_status_bits);
   fChain->SetBranchAddress("track_rp_0.valid", &track_rp_0_valid, &b_track_rp_0_valid);
   fChain->SetBranchAddress("track_rp_0.x", &track_rp_0_x, &b_track_rp_0_x);
   fChain->SetBranchAddress("track_rp_0.y", &track_rp_0_y, &b_track_rp_0_y);
   fChain->SetBranchAddress("track_rp_0.z", &track_rp_0_z, &b_track_rp_0_z);
   fChain->SetBranchAddress("track_rp_0.thx", &track_rp_0_thx, &b_track_rp_0_thx);
   fChain->SetBranchAddress("track_rp_0.thy", &track_rp_0_thy, &b_track_rp_0_thy);
   fChain->SetBranchAddress("track_rp_0.chi2", &track_rp_0_chi2, &b_track_rp_0_chi2);
   fChain->SetBranchAddress("track_rp_0.chi2ndf", &track_rp_0_chi2ndf, &b_track_rp_0_chi2ndf);
   fChain->SetBranchAddress("track_rp_0.entries", &track_rp_0_entries, &b_track_rp_0_entries);
   fChain->SetBranchAddress("track_rp_0.res_x", &track_rp_0_res_x, &b_track_rp_0_res_x);
   fChain->SetBranchAddress("track_rp_0.res_y", &track_rp_0_res_y, &b_track_rp_0_res_y);
   fChain->SetBranchAddress("track_rp_0.u_sect", &track_rp_0_u_sect, &b_track_rp_0_u_sect);
   fChain->SetBranchAddress("track_rp_0.v_sect", &track_rp_0_v_sect, &b_track_rp_0_v_sect);
   fChain->SetBranchAddress("track_rp_0.u_sect_no", &track_rp_0_u_sect_no, &b_track_rp_0_u_sect_no);
   fChain->SetBranchAddress("track_rp_0.v_sect_no", &track_rp_0_v_sect_no, &b_track_rp_0_v_sect_no);
   fChain->SetBranchAddress("track_rp_0.u_id", &track_rp_0_u_id, &b_track_rp_0_u_id);
   fChain->SetBranchAddress("track_rp_0.v_id", &track_rp_0_v_id, &b_track_rp_0_v_id);
   fChain->SetBranchAddress("multi_track_rp_0", &multi_track_rp_0_, &b_multi_track_rp_0_);
   fChain->SetBranchAddress("multi_track_rp_0.valid", &multi_track_rp_0_valid, &b_multi_track_rp_0_valid);
   fChain->SetBranchAddress("multi_track_rp_0.x", &multi_track_rp_0_x, &b_multi_track_rp_0_x);
   fChain->SetBranchAddress("multi_track_rp_0.y", &multi_track_rp_0_y, &b_multi_track_rp_0_y);
   fChain->SetBranchAddress("multi_track_rp_0.z", &multi_track_rp_0_z, &b_multi_track_rp_0_z);
   fChain->SetBranchAddress("multi_track_rp_0.thx", &multi_track_rp_0_thx, &b_multi_track_rp_0_thx);
   fChain->SetBranchAddress("multi_track_rp_0.thy", &multi_track_rp_0_thy, &b_multi_track_rp_0_thy);
   fChain->SetBranchAddress("multi_track_rp_0.chi2", &multi_track_rp_0_chi2, &b_multi_track_rp_0_chi2);
   fChain->SetBranchAddress("multi_track_rp_0.chi2ndf", &multi_track_rp_0_chi2ndf, &b_multi_track_rp_0_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_0.entries", &multi_track_rp_0_entries, &b_multi_track_rp_0_entries);
   fChain->SetBranchAddress("multi_track_rp_0.res_x", &multi_track_rp_0_res_x, &b_multi_track_rp_0_res_x);
   fChain->SetBranchAddress("multi_track_rp_0.res_y", &multi_track_rp_0_res_y, &b_multi_track_rp_0_res_y);
   fChain->SetBranchAddress("multi_track_rp_0.u_sect", &multi_track_rp_0_u_sect, &b_multi_track_rp_0_u_sect);
   fChain->SetBranchAddress("multi_track_rp_0.v_sect", &multi_track_rp_0_v_sect, &b_multi_track_rp_0_v_sect);
   fChain->SetBranchAddress("multi_track_rp_0.u_sect_no", &multi_track_rp_0_u_sect_no, &b_multi_track_rp_0_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_0.v_sect_no", &multi_track_rp_0_v_sect_no, &b_multi_track_rp_0_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_0.u_id", &multi_track_rp_0_u_id, &b_multi_track_rp_0_u_id);
   fChain->SetBranchAddress("multi_track_rp_0.v_id", &multi_track_rp_0_v_id, &b_multi_track_rp_0_v_id);
   fChain->SetBranchAddress("track_rp_1.valid", &track_rp_1_valid, &b_track_rp_1_valid);
   fChain->SetBranchAddress("track_rp_1.x", &track_rp_1_x, &b_track_rp_1_x);
   fChain->SetBranchAddress("track_rp_1.y", &track_rp_1_y, &b_track_rp_1_y);
   fChain->SetBranchAddress("track_rp_1.z", &track_rp_1_z, &b_track_rp_1_z);
   fChain->SetBranchAddress("track_rp_1.thx", &track_rp_1_thx, &b_track_rp_1_thx);
   fChain->SetBranchAddress("track_rp_1.thy", &track_rp_1_thy, &b_track_rp_1_thy);
   fChain->SetBranchAddress("track_rp_1.chi2", &track_rp_1_chi2, &b_track_rp_1_chi2);
   fChain->SetBranchAddress("track_rp_1.chi2ndf", &track_rp_1_chi2ndf, &b_track_rp_1_chi2ndf);
   fChain->SetBranchAddress("track_rp_1.entries", &track_rp_1_entries, &b_track_rp_1_entries);
   fChain->SetBranchAddress("track_rp_1.res_x", &track_rp_1_res_x, &b_track_rp_1_res_x);
   fChain->SetBranchAddress("track_rp_1.res_y", &track_rp_1_res_y, &b_track_rp_1_res_y);
   fChain->SetBranchAddress("track_rp_1.u_sect", &track_rp_1_u_sect, &b_track_rp_1_u_sect);
   fChain->SetBranchAddress("track_rp_1.v_sect", &track_rp_1_v_sect, &b_track_rp_1_v_sect);
   fChain->SetBranchAddress("track_rp_1.u_sect_no", &track_rp_1_u_sect_no, &b_track_rp_1_u_sect_no);
   fChain->SetBranchAddress("track_rp_1.v_sect_no", &track_rp_1_v_sect_no, &b_track_rp_1_v_sect_no);
   fChain->SetBranchAddress("track_rp_1.u_id", &track_rp_1_u_id, &b_track_rp_1_u_id);
   fChain->SetBranchAddress("track_rp_1.v_id", &track_rp_1_v_id, &b_track_rp_1_v_id);
   fChain->SetBranchAddress("multi_track_rp_1", &multi_track_rp_1_, &b_multi_track_rp_1_);
   fChain->SetBranchAddress("multi_track_rp_1.valid", &multi_track_rp_1_valid, &b_multi_track_rp_1_valid);
   fChain->SetBranchAddress("multi_track_rp_1.x", &multi_track_rp_1_x, &b_multi_track_rp_1_x);
   fChain->SetBranchAddress("multi_track_rp_1.y", &multi_track_rp_1_y, &b_multi_track_rp_1_y);
   fChain->SetBranchAddress("multi_track_rp_1.z", &multi_track_rp_1_z, &b_multi_track_rp_1_z);
   fChain->SetBranchAddress("multi_track_rp_1.thx", &multi_track_rp_1_thx, &b_multi_track_rp_1_thx);
   fChain->SetBranchAddress("multi_track_rp_1.thy", &multi_track_rp_1_thy, &b_multi_track_rp_1_thy);
   fChain->SetBranchAddress("multi_track_rp_1.chi2", &multi_track_rp_1_chi2, &b_multi_track_rp_1_chi2);
   fChain->SetBranchAddress("multi_track_rp_1.chi2ndf", &multi_track_rp_1_chi2ndf, &b_multi_track_rp_1_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_1.entries", &multi_track_rp_1_entries, &b_multi_track_rp_1_entries);
   fChain->SetBranchAddress("multi_track_rp_1.res_x", &multi_track_rp_1_res_x, &b_multi_track_rp_1_res_x);
   fChain->SetBranchAddress("multi_track_rp_1.res_y", &multi_track_rp_1_res_y, &b_multi_track_rp_1_res_y);
   fChain->SetBranchAddress("multi_track_rp_1.u_sect", &multi_track_rp_1_u_sect, &b_multi_track_rp_1_u_sect);
   fChain->SetBranchAddress("multi_track_rp_1.v_sect", &multi_track_rp_1_v_sect, &b_multi_track_rp_1_v_sect);
   fChain->SetBranchAddress("multi_track_rp_1.u_sect_no", &multi_track_rp_1_u_sect_no, &b_multi_track_rp_1_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_1.v_sect_no", &multi_track_rp_1_v_sect_no, &b_multi_track_rp_1_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_1.u_id", &multi_track_rp_1_u_id, &b_multi_track_rp_1_u_id);
   fChain->SetBranchAddress("multi_track_rp_1.v_id", &multi_track_rp_1_v_id, &b_multi_track_rp_1_v_id);
   fChain->SetBranchAddress("track_rp_2.valid", &track_rp_2_valid, &b_track_rp_2_valid);
   fChain->SetBranchAddress("track_rp_2.x", &track_rp_2_x, &b_track_rp_2_x);
   fChain->SetBranchAddress("track_rp_2.y", &track_rp_2_y, &b_track_rp_2_y);
   fChain->SetBranchAddress("track_rp_2.z", &track_rp_2_z, &b_track_rp_2_z);
   fChain->SetBranchAddress("track_rp_2.thx", &track_rp_2_thx, &b_track_rp_2_thx);
   fChain->SetBranchAddress("track_rp_2.thy", &track_rp_2_thy, &b_track_rp_2_thy);
   fChain->SetBranchAddress("track_rp_2.chi2", &track_rp_2_chi2, &b_track_rp_2_chi2);
   fChain->SetBranchAddress("track_rp_2.chi2ndf", &track_rp_2_chi2ndf, &b_track_rp_2_chi2ndf);
   fChain->SetBranchAddress("track_rp_2.entries", &track_rp_2_entries, &b_track_rp_2_entries);
   fChain->SetBranchAddress("track_rp_2.res_x", &track_rp_2_res_x, &b_track_rp_2_res_x);
   fChain->SetBranchAddress("track_rp_2.res_y", &track_rp_2_res_y, &b_track_rp_2_res_y);
   fChain->SetBranchAddress("track_rp_2.u_sect", &track_rp_2_u_sect, &b_track_rp_2_u_sect);
   fChain->SetBranchAddress("track_rp_2.v_sect", &track_rp_2_v_sect, &b_track_rp_2_v_sect);
   fChain->SetBranchAddress("track_rp_2.u_sect_no", &track_rp_2_u_sect_no, &b_track_rp_2_u_sect_no);
   fChain->SetBranchAddress("track_rp_2.v_sect_no", &track_rp_2_v_sect_no, &b_track_rp_2_v_sect_no);
   fChain->SetBranchAddress("track_rp_2.u_id", &track_rp_2_u_id, &b_track_rp_2_u_id);
   fChain->SetBranchAddress("track_rp_2.v_id", &track_rp_2_v_id, &b_track_rp_2_v_id);
   fChain->SetBranchAddress("multi_track_rp_2", &multi_track_rp_2_, &b_multi_track_rp_2_);
   fChain->SetBranchAddress("multi_track_rp_2.valid", &multi_track_rp_2_valid, &b_multi_track_rp_2_valid);
   fChain->SetBranchAddress("multi_track_rp_2.x", &multi_track_rp_2_x, &b_multi_track_rp_2_x);
   fChain->SetBranchAddress("multi_track_rp_2.y", &multi_track_rp_2_y, &b_multi_track_rp_2_y);
   fChain->SetBranchAddress("multi_track_rp_2.z", &multi_track_rp_2_z, &b_multi_track_rp_2_z);
   fChain->SetBranchAddress("multi_track_rp_2.thx", &multi_track_rp_2_thx, &b_multi_track_rp_2_thx);
   fChain->SetBranchAddress("multi_track_rp_2.thy", &multi_track_rp_2_thy, &b_multi_track_rp_2_thy);
   fChain->SetBranchAddress("multi_track_rp_2.chi2", &multi_track_rp_2_chi2, &b_multi_track_rp_2_chi2);
   fChain->SetBranchAddress("multi_track_rp_2.chi2ndf", &multi_track_rp_2_chi2ndf, &b_multi_track_rp_2_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_2.entries", &multi_track_rp_2_entries, &b_multi_track_rp_2_entries);
   fChain->SetBranchAddress("multi_track_rp_2.res_x", &multi_track_rp_2_res_x, &b_multi_track_rp_2_res_x);
   fChain->SetBranchAddress("multi_track_rp_2.res_y", &multi_track_rp_2_res_y, &b_multi_track_rp_2_res_y);
   fChain->SetBranchAddress("multi_track_rp_2.u_sect", &multi_track_rp_2_u_sect, &b_multi_track_rp_2_u_sect);
   fChain->SetBranchAddress("multi_track_rp_2.v_sect", &multi_track_rp_2_v_sect, &b_multi_track_rp_2_v_sect);
   fChain->SetBranchAddress("multi_track_rp_2.u_sect_no", &multi_track_rp_2_u_sect_no, &b_multi_track_rp_2_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_2.v_sect_no", &multi_track_rp_2_v_sect_no, &b_multi_track_rp_2_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_2.u_id", &multi_track_rp_2_u_id, &b_multi_track_rp_2_u_id);
   fChain->SetBranchAddress("multi_track_rp_2.v_id", &multi_track_rp_2_v_id, &b_multi_track_rp_2_v_id);
   fChain->SetBranchAddress("track_rp_3.valid", &track_rp_3_valid, &b_track_rp_3_valid);
   fChain->SetBranchAddress("track_rp_3.x", &track_rp_3_x, &b_track_rp_3_x);
   fChain->SetBranchAddress("track_rp_3.y", &track_rp_3_y, &b_track_rp_3_y);
   fChain->SetBranchAddress("track_rp_3.z", &track_rp_3_z, &b_track_rp_3_z);
   fChain->SetBranchAddress("track_rp_3.thx", &track_rp_3_thx, &b_track_rp_3_thx);
   fChain->SetBranchAddress("track_rp_3.thy", &track_rp_3_thy, &b_track_rp_3_thy);
   fChain->SetBranchAddress("track_rp_3.chi2", &track_rp_3_chi2, &b_track_rp_3_chi2);
   fChain->SetBranchAddress("track_rp_3.chi2ndf", &track_rp_3_chi2ndf, &b_track_rp_3_chi2ndf);
   fChain->SetBranchAddress("track_rp_3.entries", &track_rp_3_entries, &b_track_rp_3_entries);
   fChain->SetBranchAddress("track_rp_3.res_x", &track_rp_3_res_x, &b_track_rp_3_res_x);
   fChain->SetBranchAddress("track_rp_3.res_y", &track_rp_3_res_y, &b_track_rp_3_res_y);
   fChain->SetBranchAddress("track_rp_3.u_sect", &track_rp_3_u_sect, &b_track_rp_3_u_sect);
   fChain->SetBranchAddress("track_rp_3.v_sect", &track_rp_3_v_sect, &b_track_rp_3_v_sect);
   fChain->SetBranchAddress("track_rp_3.u_sect_no", &track_rp_3_u_sect_no, &b_track_rp_3_u_sect_no);
   fChain->SetBranchAddress("track_rp_3.v_sect_no", &track_rp_3_v_sect_no, &b_track_rp_3_v_sect_no);
   fChain->SetBranchAddress("track_rp_3.u_id", &track_rp_3_u_id, &b_track_rp_3_u_id);
   fChain->SetBranchAddress("track_rp_3.v_id", &track_rp_3_v_id, &b_track_rp_3_v_id);
   fChain->SetBranchAddress("multi_track_rp_3", &multi_track_rp_3_, &b_multi_track_rp_3_);
   fChain->SetBranchAddress("multi_track_rp_3.valid", &multi_track_rp_3_valid, &b_multi_track_rp_3_valid);
   fChain->SetBranchAddress("multi_track_rp_3.x", &multi_track_rp_3_x, &b_multi_track_rp_3_x);
   fChain->SetBranchAddress("multi_track_rp_3.y", &multi_track_rp_3_y, &b_multi_track_rp_3_y);
   fChain->SetBranchAddress("multi_track_rp_3.z", &multi_track_rp_3_z, &b_multi_track_rp_3_z);
   fChain->SetBranchAddress("multi_track_rp_3.thx", &multi_track_rp_3_thx, &b_multi_track_rp_3_thx);
   fChain->SetBranchAddress("multi_track_rp_3.thy", &multi_track_rp_3_thy, &b_multi_track_rp_3_thy);
   fChain->SetBranchAddress("multi_track_rp_3.chi2", &multi_track_rp_3_chi2, &b_multi_track_rp_3_chi2);
   fChain->SetBranchAddress("multi_track_rp_3.chi2ndf", &multi_track_rp_3_chi2ndf, &b_multi_track_rp_3_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_3.entries", &multi_track_rp_3_entries, &b_multi_track_rp_3_entries);
   fChain->SetBranchAddress("multi_track_rp_3.res_x", &multi_track_rp_3_res_x, &b_multi_track_rp_3_res_x);
   fChain->SetBranchAddress("multi_track_rp_3.res_y", &multi_track_rp_3_res_y, &b_multi_track_rp_3_res_y);
   fChain->SetBranchAddress("multi_track_rp_3.u_sect", &multi_track_rp_3_u_sect, &b_multi_track_rp_3_u_sect);
   fChain->SetBranchAddress("multi_track_rp_3.v_sect", &multi_track_rp_3_v_sect, &b_multi_track_rp_3_v_sect);
   fChain->SetBranchAddress("multi_track_rp_3.u_sect_no", &multi_track_rp_3_u_sect_no, &b_multi_track_rp_3_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_3.v_sect_no", &multi_track_rp_3_v_sect_no, &b_multi_track_rp_3_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_3.u_id", &multi_track_rp_3_u_id, &b_multi_track_rp_3_u_id);
   fChain->SetBranchAddress("multi_track_rp_3.v_id", &multi_track_rp_3_v_id, &b_multi_track_rp_3_v_id);
   fChain->SetBranchAddress("track_rp_4.valid", &track_rp_4_valid, &b_track_rp_4_valid);
   fChain->SetBranchAddress("track_rp_4.x", &track_rp_4_x, &b_track_rp_4_x);
   fChain->SetBranchAddress("track_rp_4.y", &track_rp_4_y, &b_track_rp_4_y);
   fChain->SetBranchAddress("track_rp_4.z", &track_rp_4_z, &b_track_rp_4_z);
   fChain->SetBranchAddress("track_rp_4.thx", &track_rp_4_thx, &b_track_rp_4_thx);
   fChain->SetBranchAddress("track_rp_4.thy", &track_rp_4_thy, &b_track_rp_4_thy);
   fChain->SetBranchAddress("track_rp_4.chi2", &track_rp_4_chi2, &b_track_rp_4_chi2);
   fChain->SetBranchAddress("track_rp_4.chi2ndf", &track_rp_4_chi2ndf, &b_track_rp_4_chi2ndf);
   fChain->SetBranchAddress("track_rp_4.entries", &track_rp_4_entries, &b_track_rp_4_entries);
   fChain->SetBranchAddress("track_rp_4.res_x", &track_rp_4_res_x, &b_track_rp_4_res_x);
   fChain->SetBranchAddress("track_rp_4.res_y", &track_rp_4_res_y, &b_track_rp_4_res_y);
   fChain->SetBranchAddress("track_rp_4.u_sect", &track_rp_4_u_sect, &b_track_rp_4_u_sect);
   fChain->SetBranchAddress("track_rp_4.v_sect", &track_rp_4_v_sect, &b_track_rp_4_v_sect);
   fChain->SetBranchAddress("track_rp_4.u_sect_no", &track_rp_4_u_sect_no, &b_track_rp_4_u_sect_no);
   fChain->SetBranchAddress("track_rp_4.v_sect_no", &track_rp_4_v_sect_no, &b_track_rp_4_v_sect_no);
   fChain->SetBranchAddress("track_rp_4.u_id", &track_rp_4_u_id, &b_track_rp_4_u_id);
   fChain->SetBranchAddress("track_rp_4.v_id", &track_rp_4_v_id, &b_track_rp_4_v_id);
   fChain->SetBranchAddress("multi_track_rp_4", &multi_track_rp_4_, &b_multi_track_rp_4_);
   fChain->SetBranchAddress("multi_track_rp_4.valid", &multi_track_rp_4_valid, &b_multi_track_rp_4_valid);
   fChain->SetBranchAddress("multi_track_rp_4.x", &multi_track_rp_4_x, &b_multi_track_rp_4_x);
   fChain->SetBranchAddress("multi_track_rp_4.y", &multi_track_rp_4_y, &b_multi_track_rp_4_y);
   fChain->SetBranchAddress("multi_track_rp_4.z", &multi_track_rp_4_z, &b_multi_track_rp_4_z);
   fChain->SetBranchAddress("multi_track_rp_4.thx", &multi_track_rp_4_thx, &b_multi_track_rp_4_thx);
   fChain->SetBranchAddress("multi_track_rp_4.thy", &multi_track_rp_4_thy, &b_multi_track_rp_4_thy);
   fChain->SetBranchAddress("multi_track_rp_4.chi2", &multi_track_rp_4_chi2, &b_multi_track_rp_4_chi2);
   fChain->SetBranchAddress("multi_track_rp_4.chi2ndf", &multi_track_rp_4_chi2ndf, &b_multi_track_rp_4_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_4.entries", &multi_track_rp_4_entries, &b_multi_track_rp_4_entries);
   fChain->SetBranchAddress("multi_track_rp_4.res_x", &multi_track_rp_4_res_x, &b_multi_track_rp_4_res_x);
   fChain->SetBranchAddress("multi_track_rp_4.res_y", &multi_track_rp_4_res_y, &b_multi_track_rp_4_res_y);
   fChain->SetBranchAddress("multi_track_rp_4.u_sect", &multi_track_rp_4_u_sect, &b_multi_track_rp_4_u_sect);
   fChain->SetBranchAddress("multi_track_rp_4.v_sect", &multi_track_rp_4_v_sect, &b_multi_track_rp_4_v_sect);
   fChain->SetBranchAddress("multi_track_rp_4.u_sect_no", &multi_track_rp_4_u_sect_no, &b_multi_track_rp_4_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_4.v_sect_no", &multi_track_rp_4_v_sect_no, &b_multi_track_rp_4_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_4.u_id", &multi_track_rp_4_u_id, &b_multi_track_rp_4_u_id);
   fChain->SetBranchAddress("multi_track_rp_4.v_id", &multi_track_rp_4_v_id, &b_multi_track_rp_4_v_id);
   fChain->SetBranchAddress("track_rp_5.valid", &track_rp_5_valid, &b_track_rp_5_valid);
   fChain->SetBranchAddress("track_rp_5.x", &track_rp_5_x, &b_track_rp_5_x);
   fChain->SetBranchAddress("track_rp_5.y", &track_rp_5_y, &b_track_rp_5_y);
   fChain->SetBranchAddress("track_rp_5.z", &track_rp_5_z, &b_track_rp_5_z);
   fChain->SetBranchAddress("track_rp_5.thx", &track_rp_5_thx, &b_track_rp_5_thx);
   fChain->SetBranchAddress("track_rp_5.thy", &track_rp_5_thy, &b_track_rp_5_thy);
   fChain->SetBranchAddress("track_rp_5.chi2", &track_rp_5_chi2, &b_track_rp_5_chi2);
   fChain->SetBranchAddress("track_rp_5.chi2ndf", &track_rp_5_chi2ndf, &b_track_rp_5_chi2ndf);
   fChain->SetBranchAddress("track_rp_5.entries", &track_rp_5_entries, &b_track_rp_5_entries);
   fChain->SetBranchAddress("track_rp_5.res_x", &track_rp_5_res_x, &b_track_rp_5_res_x);
   fChain->SetBranchAddress("track_rp_5.res_y", &track_rp_5_res_y, &b_track_rp_5_res_y);
   fChain->SetBranchAddress("track_rp_5.u_sect", &track_rp_5_u_sect, &b_track_rp_5_u_sect);
   fChain->SetBranchAddress("track_rp_5.v_sect", &track_rp_5_v_sect, &b_track_rp_5_v_sect);
   fChain->SetBranchAddress("track_rp_5.u_sect_no", &track_rp_5_u_sect_no, &b_track_rp_5_u_sect_no);
   fChain->SetBranchAddress("track_rp_5.v_sect_no", &track_rp_5_v_sect_no, &b_track_rp_5_v_sect_no);
   fChain->SetBranchAddress("track_rp_5.u_id", &track_rp_5_u_id, &b_track_rp_5_u_id);
   fChain->SetBranchAddress("track_rp_5.v_id", &track_rp_5_v_id, &b_track_rp_5_v_id);
   fChain->SetBranchAddress("multi_track_rp_5", &multi_track_rp_5_, &b_multi_track_rp_5_);
   fChain->SetBranchAddress("multi_track_rp_5.valid", &multi_track_rp_5_valid, &b_multi_track_rp_5_valid);
   fChain->SetBranchAddress("multi_track_rp_5.x", &multi_track_rp_5_x, &b_multi_track_rp_5_x);
   fChain->SetBranchAddress("multi_track_rp_5.y", &multi_track_rp_5_y, &b_multi_track_rp_5_y);
   fChain->SetBranchAddress("multi_track_rp_5.z", &multi_track_rp_5_z, &b_multi_track_rp_5_z);
   fChain->SetBranchAddress("multi_track_rp_5.thx", &multi_track_rp_5_thx, &b_multi_track_rp_5_thx);
   fChain->SetBranchAddress("multi_track_rp_5.thy", &multi_track_rp_5_thy, &b_multi_track_rp_5_thy);
   fChain->SetBranchAddress("multi_track_rp_5.chi2", &multi_track_rp_5_chi2, &b_multi_track_rp_5_chi2);
   fChain->SetBranchAddress("multi_track_rp_5.chi2ndf", &multi_track_rp_5_chi2ndf, &b_multi_track_rp_5_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_5.entries", &multi_track_rp_5_entries, &b_multi_track_rp_5_entries);
   fChain->SetBranchAddress("multi_track_rp_5.res_x", &multi_track_rp_5_res_x, &b_multi_track_rp_5_res_x);
   fChain->SetBranchAddress("multi_track_rp_5.res_y", &multi_track_rp_5_res_y, &b_multi_track_rp_5_res_y);
   fChain->SetBranchAddress("multi_track_rp_5.u_sect", &multi_track_rp_5_u_sect, &b_multi_track_rp_5_u_sect);
   fChain->SetBranchAddress("multi_track_rp_5.v_sect", &multi_track_rp_5_v_sect, &b_multi_track_rp_5_v_sect);
   fChain->SetBranchAddress("multi_track_rp_5.u_sect_no", &multi_track_rp_5_u_sect_no, &b_multi_track_rp_5_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_5.v_sect_no", &multi_track_rp_5_v_sect_no, &b_multi_track_rp_5_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_5.u_id", &multi_track_rp_5_u_id, &b_multi_track_rp_5_u_id);
   fChain->SetBranchAddress("multi_track_rp_5.v_id", &multi_track_rp_5_v_id, &b_multi_track_rp_5_v_id);
   fChain->SetBranchAddress("track_rp_20.valid", &track_rp_20_valid, &b_track_rp_20_valid);
   fChain->SetBranchAddress("track_rp_20.x", &track_rp_20_x, &b_track_rp_20_x);
   fChain->SetBranchAddress("track_rp_20.y", &track_rp_20_y, &b_track_rp_20_y);
   fChain->SetBranchAddress("track_rp_20.z", &track_rp_20_z, &b_track_rp_20_z);
   fChain->SetBranchAddress("track_rp_20.thx", &track_rp_20_thx, &b_track_rp_20_thx);
   fChain->SetBranchAddress("track_rp_20.thy", &track_rp_20_thy, &b_track_rp_20_thy);
   fChain->SetBranchAddress("track_rp_20.chi2", &track_rp_20_chi2, &b_track_rp_20_chi2);
   fChain->SetBranchAddress("track_rp_20.chi2ndf", &track_rp_20_chi2ndf, &b_track_rp_20_chi2ndf);
   fChain->SetBranchAddress("track_rp_20.entries", &track_rp_20_entries, &b_track_rp_20_entries);
   fChain->SetBranchAddress("track_rp_20.res_x", &track_rp_20_res_x, &b_track_rp_20_res_x);
   fChain->SetBranchAddress("track_rp_20.res_y", &track_rp_20_res_y, &b_track_rp_20_res_y);
   fChain->SetBranchAddress("track_rp_20.u_sect", &track_rp_20_u_sect, &b_track_rp_20_u_sect);
   fChain->SetBranchAddress("track_rp_20.v_sect", &track_rp_20_v_sect, &b_track_rp_20_v_sect);
   fChain->SetBranchAddress("track_rp_20.u_sect_no", &track_rp_20_u_sect_no, &b_track_rp_20_u_sect_no);
   fChain->SetBranchAddress("track_rp_20.v_sect_no", &track_rp_20_v_sect_no, &b_track_rp_20_v_sect_no);
   fChain->SetBranchAddress("track_rp_20.u_id", &track_rp_20_u_id, &b_track_rp_20_u_id);
   fChain->SetBranchAddress("track_rp_20.v_id", &track_rp_20_v_id, &b_track_rp_20_v_id);
   fChain->SetBranchAddress("multi_track_rp_20", &multi_track_rp_20_, &b_multi_track_rp_20_);
   fChain->SetBranchAddress("multi_track_rp_20.valid", &multi_track_rp_20_valid, &b_multi_track_rp_20_valid);
   fChain->SetBranchAddress("multi_track_rp_20.x", &multi_track_rp_20_x, &b_multi_track_rp_20_x);
   fChain->SetBranchAddress("multi_track_rp_20.y", &multi_track_rp_20_y, &b_multi_track_rp_20_y);
   fChain->SetBranchAddress("multi_track_rp_20.z", &multi_track_rp_20_z, &b_multi_track_rp_20_z);
   fChain->SetBranchAddress("multi_track_rp_20.thx", &multi_track_rp_20_thx, &b_multi_track_rp_20_thx);
   fChain->SetBranchAddress("multi_track_rp_20.thy", &multi_track_rp_20_thy, &b_multi_track_rp_20_thy);
   fChain->SetBranchAddress("multi_track_rp_20.chi2", &multi_track_rp_20_chi2, &b_multi_track_rp_20_chi2);
   fChain->SetBranchAddress("multi_track_rp_20.chi2ndf", &multi_track_rp_20_chi2ndf, &b_multi_track_rp_20_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_20.entries", &multi_track_rp_20_entries, &b_multi_track_rp_20_entries);
   fChain->SetBranchAddress("multi_track_rp_20.res_x", &multi_track_rp_20_res_x, &b_multi_track_rp_20_res_x);
   fChain->SetBranchAddress("multi_track_rp_20.res_y", &multi_track_rp_20_res_y, &b_multi_track_rp_20_res_y);
   fChain->SetBranchAddress("multi_track_rp_20.u_sect", &multi_track_rp_20_u_sect, &b_multi_track_rp_20_u_sect);
   fChain->SetBranchAddress("multi_track_rp_20.v_sect", &multi_track_rp_20_v_sect, &b_multi_track_rp_20_v_sect);
   fChain->SetBranchAddress("multi_track_rp_20.u_sect_no", &multi_track_rp_20_u_sect_no, &b_multi_track_rp_20_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_20.v_sect_no", &multi_track_rp_20_v_sect_no, &b_multi_track_rp_20_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_20.u_id", &multi_track_rp_20_u_id, &b_multi_track_rp_20_u_id);
   fChain->SetBranchAddress("multi_track_rp_20.v_id", &multi_track_rp_20_v_id, &b_multi_track_rp_20_v_id);
   fChain->SetBranchAddress("track_rp_21.valid", &track_rp_21_valid, &b_track_rp_21_valid);
   fChain->SetBranchAddress("track_rp_21.x", &track_rp_21_x, &b_track_rp_21_x);
   fChain->SetBranchAddress("track_rp_21.y", &track_rp_21_y, &b_track_rp_21_y);
   fChain->SetBranchAddress("track_rp_21.z", &track_rp_21_z, &b_track_rp_21_z);
   fChain->SetBranchAddress("track_rp_21.thx", &track_rp_21_thx, &b_track_rp_21_thx);
   fChain->SetBranchAddress("track_rp_21.thy", &track_rp_21_thy, &b_track_rp_21_thy);
   fChain->SetBranchAddress("track_rp_21.chi2", &track_rp_21_chi2, &b_track_rp_21_chi2);
   fChain->SetBranchAddress("track_rp_21.chi2ndf", &track_rp_21_chi2ndf, &b_track_rp_21_chi2ndf);
   fChain->SetBranchAddress("track_rp_21.entries", &track_rp_21_entries, &b_track_rp_21_entries);
   fChain->SetBranchAddress("track_rp_21.res_x", &track_rp_21_res_x, &b_track_rp_21_res_x);
   fChain->SetBranchAddress("track_rp_21.res_y", &track_rp_21_res_y, &b_track_rp_21_res_y);
   fChain->SetBranchAddress("track_rp_21.u_sect", &track_rp_21_u_sect, &b_track_rp_21_u_sect);
   fChain->SetBranchAddress("track_rp_21.v_sect", &track_rp_21_v_sect, &b_track_rp_21_v_sect);
   fChain->SetBranchAddress("track_rp_21.u_sect_no", &track_rp_21_u_sect_no, &b_track_rp_21_u_sect_no);
   fChain->SetBranchAddress("track_rp_21.v_sect_no", &track_rp_21_v_sect_no, &b_track_rp_21_v_sect_no);
   fChain->SetBranchAddress("track_rp_21.u_id", &track_rp_21_u_id, &b_track_rp_21_u_id);
   fChain->SetBranchAddress("track_rp_21.v_id", &track_rp_21_v_id, &b_track_rp_21_v_id);
   fChain->SetBranchAddress("multi_track_rp_21", &multi_track_rp_21_, &b_multi_track_rp_21_);
   fChain->SetBranchAddress("multi_track_rp_21.valid", &multi_track_rp_21_valid, &b_multi_track_rp_21_valid);
   fChain->SetBranchAddress("multi_track_rp_21.x", &multi_track_rp_21_x, &b_multi_track_rp_21_x);
   fChain->SetBranchAddress("multi_track_rp_21.y", &multi_track_rp_21_y, &b_multi_track_rp_21_y);
   fChain->SetBranchAddress("multi_track_rp_21.z", &multi_track_rp_21_z, &b_multi_track_rp_21_z);
   fChain->SetBranchAddress("multi_track_rp_21.thx", &multi_track_rp_21_thx, &b_multi_track_rp_21_thx);
   fChain->SetBranchAddress("multi_track_rp_21.thy", &multi_track_rp_21_thy, &b_multi_track_rp_21_thy);
   fChain->SetBranchAddress("multi_track_rp_21.chi2", &multi_track_rp_21_chi2, &b_multi_track_rp_21_chi2);
   fChain->SetBranchAddress("multi_track_rp_21.chi2ndf", &multi_track_rp_21_chi2ndf, &b_multi_track_rp_21_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_21.entries", &multi_track_rp_21_entries, &b_multi_track_rp_21_entries);
   fChain->SetBranchAddress("multi_track_rp_21.res_x", &multi_track_rp_21_res_x, &b_multi_track_rp_21_res_x);
   fChain->SetBranchAddress("multi_track_rp_21.res_y", &multi_track_rp_21_res_y, &b_multi_track_rp_21_res_y);
   fChain->SetBranchAddress("multi_track_rp_21.u_sect", &multi_track_rp_21_u_sect, &b_multi_track_rp_21_u_sect);
   fChain->SetBranchAddress("multi_track_rp_21.v_sect", &multi_track_rp_21_v_sect, &b_multi_track_rp_21_v_sect);
   fChain->SetBranchAddress("multi_track_rp_21.u_sect_no", &multi_track_rp_21_u_sect_no, &b_multi_track_rp_21_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_21.v_sect_no", &multi_track_rp_21_v_sect_no, &b_multi_track_rp_21_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_21.u_id", &multi_track_rp_21_u_id, &b_multi_track_rp_21_u_id);
   fChain->SetBranchAddress("multi_track_rp_21.v_id", &multi_track_rp_21_v_id, &b_multi_track_rp_21_v_id);
   fChain->SetBranchAddress("track_rp_22.valid", &track_rp_22_valid, &b_track_rp_22_valid);
   fChain->SetBranchAddress("track_rp_22.x", &track_rp_22_x, &b_track_rp_22_x);
   fChain->SetBranchAddress("track_rp_22.y", &track_rp_22_y, &b_track_rp_22_y);
   fChain->SetBranchAddress("track_rp_22.z", &track_rp_22_z, &b_track_rp_22_z);
   fChain->SetBranchAddress("track_rp_22.thx", &track_rp_22_thx, &b_track_rp_22_thx);
   fChain->SetBranchAddress("track_rp_22.thy", &track_rp_22_thy, &b_track_rp_22_thy);
   fChain->SetBranchAddress("track_rp_22.chi2", &track_rp_22_chi2, &b_track_rp_22_chi2);
   fChain->SetBranchAddress("track_rp_22.chi2ndf", &track_rp_22_chi2ndf, &b_track_rp_22_chi2ndf);
   fChain->SetBranchAddress("track_rp_22.entries", &track_rp_22_entries, &b_track_rp_22_entries);
   fChain->SetBranchAddress("track_rp_22.res_x", &track_rp_22_res_x, &b_track_rp_22_res_x);
   fChain->SetBranchAddress("track_rp_22.res_y", &track_rp_22_res_y, &b_track_rp_22_res_y);
   fChain->SetBranchAddress("track_rp_22.u_sect", &track_rp_22_u_sect, &b_track_rp_22_u_sect);
   fChain->SetBranchAddress("track_rp_22.v_sect", &track_rp_22_v_sect, &b_track_rp_22_v_sect);
   fChain->SetBranchAddress("track_rp_22.u_sect_no", &track_rp_22_u_sect_no, &b_track_rp_22_u_sect_no);
   fChain->SetBranchAddress("track_rp_22.v_sect_no", &track_rp_22_v_sect_no, &b_track_rp_22_v_sect_no);
   fChain->SetBranchAddress("track_rp_22.u_id", &track_rp_22_u_id, &b_track_rp_22_u_id);
   fChain->SetBranchAddress("track_rp_22.v_id", &track_rp_22_v_id, &b_track_rp_22_v_id);
   fChain->SetBranchAddress("multi_track_rp_22", &multi_track_rp_22_, &b_multi_track_rp_22_);
   fChain->SetBranchAddress("multi_track_rp_22.valid", &multi_track_rp_22_valid, &b_multi_track_rp_22_valid);
   fChain->SetBranchAddress("multi_track_rp_22.x", &multi_track_rp_22_x, &b_multi_track_rp_22_x);
   fChain->SetBranchAddress("multi_track_rp_22.y", &multi_track_rp_22_y, &b_multi_track_rp_22_y);
   fChain->SetBranchAddress("multi_track_rp_22.z", &multi_track_rp_22_z, &b_multi_track_rp_22_z);
   fChain->SetBranchAddress("multi_track_rp_22.thx", &multi_track_rp_22_thx, &b_multi_track_rp_22_thx);
   fChain->SetBranchAddress("multi_track_rp_22.thy", &multi_track_rp_22_thy, &b_multi_track_rp_22_thy);
   fChain->SetBranchAddress("multi_track_rp_22.chi2", &multi_track_rp_22_chi2, &b_multi_track_rp_22_chi2);
   fChain->SetBranchAddress("multi_track_rp_22.chi2ndf", &multi_track_rp_22_chi2ndf, &b_multi_track_rp_22_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_22.entries", &multi_track_rp_22_entries, &b_multi_track_rp_22_entries);
   fChain->SetBranchAddress("multi_track_rp_22.res_x", &multi_track_rp_22_res_x, &b_multi_track_rp_22_res_x);
   fChain->SetBranchAddress("multi_track_rp_22.res_y", &multi_track_rp_22_res_y, &b_multi_track_rp_22_res_y);
   fChain->SetBranchAddress("multi_track_rp_22.u_sect", &multi_track_rp_22_u_sect, &b_multi_track_rp_22_u_sect);
   fChain->SetBranchAddress("multi_track_rp_22.v_sect", &multi_track_rp_22_v_sect, &b_multi_track_rp_22_v_sect);
   fChain->SetBranchAddress("multi_track_rp_22.u_sect_no", &multi_track_rp_22_u_sect_no, &b_multi_track_rp_22_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_22.v_sect_no", &multi_track_rp_22_v_sect_no, &b_multi_track_rp_22_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_22.u_id", &multi_track_rp_22_u_id, &b_multi_track_rp_22_u_id);
   fChain->SetBranchAddress("multi_track_rp_22.v_id", &multi_track_rp_22_v_id, &b_multi_track_rp_22_v_id);
   fChain->SetBranchAddress("track_rp_23.valid", &track_rp_23_valid, &b_track_rp_23_valid);
   fChain->SetBranchAddress("track_rp_23.x", &track_rp_23_x, &b_track_rp_23_x);
   fChain->SetBranchAddress("track_rp_23.y", &track_rp_23_y, &b_track_rp_23_y);
   fChain->SetBranchAddress("track_rp_23.z", &track_rp_23_z, &b_track_rp_23_z);
   fChain->SetBranchAddress("track_rp_23.thx", &track_rp_23_thx, &b_track_rp_23_thx);
   fChain->SetBranchAddress("track_rp_23.thy", &track_rp_23_thy, &b_track_rp_23_thy);
   fChain->SetBranchAddress("track_rp_23.chi2", &track_rp_23_chi2, &b_track_rp_23_chi2);
   fChain->SetBranchAddress("track_rp_23.chi2ndf", &track_rp_23_chi2ndf, &b_track_rp_23_chi2ndf);
   fChain->SetBranchAddress("track_rp_23.entries", &track_rp_23_entries, &b_track_rp_23_entries);
   fChain->SetBranchAddress("track_rp_23.res_x", &track_rp_23_res_x, &b_track_rp_23_res_x);
   fChain->SetBranchAddress("track_rp_23.res_y", &track_rp_23_res_y, &b_track_rp_23_res_y);
   fChain->SetBranchAddress("track_rp_23.u_sect", &track_rp_23_u_sect, &b_track_rp_23_u_sect);
   fChain->SetBranchAddress("track_rp_23.v_sect", &track_rp_23_v_sect, &b_track_rp_23_v_sect);
   fChain->SetBranchAddress("track_rp_23.u_sect_no", &track_rp_23_u_sect_no, &b_track_rp_23_u_sect_no);
   fChain->SetBranchAddress("track_rp_23.v_sect_no", &track_rp_23_v_sect_no, &b_track_rp_23_v_sect_no);
   fChain->SetBranchAddress("track_rp_23.u_id", &track_rp_23_u_id, &b_track_rp_23_u_id);
   fChain->SetBranchAddress("track_rp_23.v_id", &track_rp_23_v_id, &b_track_rp_23_v_id);
   fChain->SetBranchAddress("multi_track_rp_23", &multi_track_rp_23_, &b_multi_track_rp_23_);
   fChain->SetBranchAddress("multi_track_rp_23.valid", &multi_track_rp_23_valid, &b_multi_track_rp_23_valid);
   fChain->SetBranchAddress("multi_track_rp_23.x", &multi_track_rp_23_x, &b_multi_track_rp_23_x);
   fChain->SetBranchAddress("multi_track_rp_23.y", &multi_track_rp_23_y, &b_multi_track_rp_23_y);
   fChain->SetBranchAddress("multi_track_rp_23.z", &multi_track_rp_23_z, &b_multi_track_rp_23_z);
   fChain->SetBranchAddress("multi_track_rp_23.thx", &multi_track_rp_23_thx, &b_multi_track_rp_23_thx);
   fChain->SetBranchAddress("multi_track_rp_23.thy", &multi_track_rp_23_thy, &b_multi_track_rp_23_thy);
   fChain->SetBranchAddress("multi_track_rp_23.chi2", &multi_track_rp_23_chi2, &b_multi_track_rp_23_chi2);
   fChain->SetBranchAddress("multi_track_rp_23.chi2ndf", &multi_track_rp_23_chi2ndf, &b_multi_track_rp_23_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_23.entries", &multi_track_rp_23_entries, &b_multi_track_rp_23_entries);
   fChain->SetBranchAddress("multi_track_rp_23.res_x", &multi_track_rp_23_res_x, &b_multi_track_rp_23_res_x);
   fChain->SetBranchAddress("multi_track_rp_23.res_y", &multi_track_rp_23_res_y, &b_multi_track_rp_23_res_y);
   fChain->SetBranchAddress("multi_track_rp_23.u_sect", &multi_track_rp_23_u_sect, &b_multi_track_rp_23_u_sect);
   fChain->SetBranchAddress("multi_track_rp_23.v_sect", &multi_track_rp_23_v_sect, &b_multi_track_rp_23_v_sect);
   fChain->SetBranchAddress("multi_track_rp_23.u_sect_no", &multi_track_rp_23_u_sect_no, &b_multi_track_rp_23_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_23.v_sect_no", &multi_track_rp_23_v_sect_no, &b_multi_track_rp_23_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_23.u_id", &multi_track_rp_23_u_id, &b_multi_track_rp_23_u_id);
   fChain->SetBranchAddress("multi_track_rp_23.v_id", &multi_track_rp_23_v_id, &b_multi_track_rp_23_v_id);
   fChain->SetBranchAddress("track_rp_24.valid", &track_rp_24_valid, &b_track_rp_24_valid);
   fChain->SetBranchAddress("track_rp_24.x", &track_rp_24_x, &b_track_rp_24_x);
   fChain->SetBranchAddress("track_rp_24.y", &track_rp_24_y, &b_track_rp_24_y);
   fChain->SetBranchAddress("track_rp_24.z", &track_rp_24_z, &b_track_rp_24_z);
   fChain->SetBranchAddress("track_rp_24.thx", &track_rp_24_thx, &b_track_rp_24_thx);
   fChain->SetBranchAddress("track_rp_24.thy", &track_rp_24_thy, &b_track_rp_24_thy);
   fChain->SetBranchAddress("track_rp_24.chi2", &track_rp_24_chi2, &b_track_rp_24_chi2);
   fChain->SetBranchAddress("track_rp_24.chi2ndf", &track_rp_24_chi2ndf, &b_track_rp_24_chi2ndf);
   fChain->SetBranchAddress("track_rp_24.entries", &track_rp_24_entries, &b_track_rp_24_entries);
   fChain->SetBranchAddress("track_rp_24.res_x", &track_rp_24_res_x, &b_track_rp_24_res_x);
   fChain->SetBranchAddress("track_rp_24.res_y", &track_rp_24_res_y, &b_track_rp_24_res_y);
   fChain->SetBranchAddress("track_rp_24.u_sect", &track_rp_24_u_sect, &b_track_rp_24_u_sect);
   fChain->SetBranchAddress("track_rp_24.v_sect", &track_rp_24_v_sect, &b_track_rp_24_v_sect);
   fChain->SetBranchAddress("track_rp_24.u_sect_no", &track_rp_24_u_sect_no, &b_track_rp_24_u_sect_no);
   fChain->SetBranchAddress("track_rp_24.v_sect_no", &track_rp_24_v_sect_no, &b_track_rp_24_v_sect_no);
   fChain->SetBranchAddress("track_rp_24.u_id", &track_rp_24_u_id, &b_track_rp_24_u_id);
   fChain->SetBranchAddress("track_rp_24.v_id", &track_rp_24_v_id, &b_track_rp_24_v_id);
   fChain->SetBranchAddress("multi_track_rp_24", &multi_track_rp_24_, &b_multi_track_rp_24_);
   fChain->SetBranchAddress("multi_track_rp_24.valid", &multi_track_rp_24_valid, &b_multi_track_rp_24_valid);
   fChain->SetBranchAddress("multi_track_rp_24.x", &multi_track_rp_24_x, &b_multi_track_rp_24_x);
   fChain->SetBranchAddress("multi_track_rp_24.y", &multi_track_rp_24_y, &b_multi_track_rp_24_y);
   fChain->SetBranchAddress("multi_track_rp_24.z", &multi_track_rp_24_z, &b_multi_track_rp_24_z);
   fChain->SetBranchAddress("multi_track_rp_24.thx", &multi_track_rp_24_thx, &b_multi_track_rp_24_thx);
   fChain->SetBranchAddress("multi_track_rp_24.thy", &multi_track_rp_24_thy, &b_multi_track_rp_24_thy);
   fChain->SetBranchAddress("multi_track_rp_24.chi2", &multi_track_rp_24_chi2, &b_multi_track_rp_24_chi2);
   fChain->SetBranchAddress("multi_track_rp_24.chi2ndf", &multi_track_rp_24_chi2ndf, &b_multi_track_rp_24_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_24.entries", &multi_track_rp_24_entries, &b_multi_track_rp_24_entries);
   fChain->SetBranchAddress("multi_track_rp_24.res_x", &multi_track_rp_24_res_x, &b_multi_track_rp_24_res_x);
   fChain->SetBranchAddress("multi_track_rp_24.res_y", &multi_track_rp_24_res_y, &b_multi_track_rp_24_res_y);
   fChain->SetBranchAddress("multi_track_rp_24.u_sect", &multi_track_rp_24_u_sect, &b_multi_track_rp_24_u_sect);
   fChain->SetBranchAddress("multi_track_rp_24.v_sect", &multi_track_rp_24_v_sect, &b_multi_track_rp_24_v_sect);
   fChain->SetBranchAddress("multi_track_rp_24.u_sect_no", &multi_track_rp_24_u_sect_no, &b_multi_track_rp_24_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_24.v_sect_no", &multi_track_rp_24_v_sect_no, &b_multi_track_rp_24_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_24.u_id", &multi_track_rp_24_u_id, &b_multi_track_rp_24_u_id);
   fChain->SetBranchAddress("multi_track_rp_24.v_id", &multi_track_rp_24_v_id, &b_multi_track_rp_24_v_id);
   fChain->SetBranchAddress("track_rp_25.valid", &track_rp_25_valid, &b_track_rp_25_valid);
   fChain->SetBranchAddress("track_rp_25.x", &track_rp_25_x, &b_track_rp_25_x);
   fChain->SetBranchAddress("track_rp_25.y", &track_rp_25_y, &b_track_rp_25_y);
   fChain->SetBranchAddress("track_rp_25.z", &track_rp_25_z, &b_track_rp_25_z);
   fChain->SetBranchAddress("track_rp_25.thx", &track_rp_25_thx, &b_track_rp_25_thx);
   fChain->SetBranchAddress("track_rp_25.thy", &track_rp_25_thy, &b_track_rp_25_thy);
   fChain->SetBranchAddress("track_rp_25.chi2", &track_rp_25_chi2, &b_track_rp_25_chi2);
   fChain->SetBranchAddress("track_rp_25.chi2ndf", &track_rp_25_chi2ndf, &b_track_rp_25_chi2ndf);
   fChain->SetBranchAddress("track_rp_25.entries", &track_rp_25_entries, &b_track_rp_25_entries);
   fChain->SetBranchAddress("track_rp_25.res_x", &track_rp_25_res_x, &b_track_rp_25_res_x);
   fChain->SetBranchAddress("track_rp_25.res_y", &track_rp_25_res_y, &b_track_rp_25_res_y);
   fChain->SetBranchAddress("track_rp_25.u_sect", &track_rp_25_u_sect, &b_track_rp_25_u_sect);
   fChain->SetBranchAddress("track_rp_25.v_sect", &track_rp_25_v_sect, &b_track_rp_25_v_sect);
   fChain->SetBranchAddress("track_rp_25.u_sect_no", &track_rp_25_u_sect_no, &b_track_rp_25_u_sect_no);
   fChain->SetBranchAddress("track_rp_25.v_sect_no", &track_rp_25_v_sect_no, &b_track_rp_25_v_sect_no);
   fChain->SetBranchAddress("track_rp_25.u_id", &track_rp_25_u_id, &b_track_rp_25_u_id);
   fChain->SetBranchAddress("track_rp_25.v_id", &track_rp_25_v_id, &b_track_rp_25_v_id);
   fChain->SetBranchAddress("multi_track_rp_25", &multi_track_rp_25_, &b_multi_track_rp_25_);
   fChain->SetBranchAddress("multi_track_rp_25.valid", &multi_track_rp_25_valid, &b_multi_track_rp_25_valid);
   fChain->SetBranchAddress("multi_track_rp_25.x", &multi_track_rp_25_x, &b_multi_track_rp_25_x);
   fChain->SetBranchAddress("multi_track_rp_25.y", &multi_track_rp_25_y, &b_multi_track_rp_25_y);
   fChain->SetBranchAddress("multi_track_rp_25.z", &multi_track_rp_25_z, &b_multi_track_rp_25_z);
   fChain->SetBranchAddress("multi_track_rp_25.thx", &multi_track_rp_25_thx, &b_multi_track_rp_25_thx);
   fChain->SetBranchAddress("multi_track_rp_25.thy", &multi_track_rp_25_thy, &b_multi_track_rp_25_thy);
   fChain->SetBranchAddress("multi_track_rp_25.chi2", &multi_track_rp_25_chi2, &b_multi_track_rp_25_chi2);
   fChain->SetBranchAddress("multi_track_rp_25.chi2ndf", &multi_track_rp_25_chi2ndf, &b_multi_track_rp_25_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_25.entries", &multi_track_rp_25_entries, &b_multi_track_rp_25_entries);
   fChain->SetBranchAddress("multi_track_rp_25.res_x", &multi_track_rp_25_res_x, &b_multi_track_rp_25_res_x);
   fChain->SetBranchAddress("multi_track_rp_25.res_y", &multi_track_rp_25_res_y, &b_multi_track_rp_25_res_y);
   fChain->SetBranchAddress("multi_track_rp_25.u_sect", &multi_track_rp_25_u_sect, &b_multi_track_rp_25_u_sect);
   fChain->SetBranchAddress("multi_track_rp_25.v_sect", &multi_track_rp_25_v_sect, &b_multi_track_rp_25_v_sect);
   fChain->SetBranchAddress("multi_track_rp_25.u_sect_no", &multi_track_rp_25_u_sect_no, &b_multi_track_rp_25_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_25.v_sect_no", &multi_track_rp_25_v_sect_no, &b_multi_track_rp_25_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_25.u_id", &multi_track_rp_25_u_id, &b_multi_track_rp_25_u_id);
   fChain->SetBranchAddress("multi_track_rp_25.v_id", &multi_track_rp_25_v_id, &b_multi_track_rp_25_v_id);
   fChain->SetBranchAddress("track_rp_100.valid", &track_rp_100_valid, &b_track_rp_100_valid);
   fChain->SetBranchAddress("track_rp_100.x", &track_rp_100_x, &b_track_rp_100_x);
   fChain->SetBranchAddress("track_rp_100.y", &track_rp_100_y, &b_track_rp_100_y);
   fChain->SetBranchAddress("track_rp_100.z", &track_rp_100_z, &b_track_rp_100_z);
   fChain->SetBranchAddress("track_rp_100.thx", &track_rp_100_thx, &b_track_rp_100_thx);
   fChain->SetBranchAddress("track_rp_100.thy", &track_rp_100_thy, &b_track_rp_100_thy);
   fChain->SetBranchAddress("track_rp_100.chi2", &track_rp_100_chi2, &b_track_rp_100_chi2);
   fChain->SetBranchAddress("track_rp_100.chi2ndf", &track_rp_100_chi2ndf, &b_track_rp_100_chi2ndf);
   fChain->SetBranchAddress("track_rp_100.entries", &track_rp_100_entries, &b_track_rp_100_entries);
   fChain->SetBranchAddress("track_rp_100.res_x", &track_rp_100_res_x, &b_track_rp_100_res_x);
   fChain->SetBranchAddress("track_rp_100.res_y", &track_rp_100_res_y, &b_track_rp_100_res_y);
   fChain->SetBranchAddress("track_rp_100.u_sect", &track_rp_100_u_sect, &b_track_rp_100_u_sect);
   fChain->SetBranchAddress("track_rp_100.v_sect", &track_rp_100_v_sect, &b_track_rp_100_v_sect);
   fChain->SetBranchAddress("track_rp_100.u_sect_no", &track_rp_100_u_sect_no, &b_track_rp_100_u_sect_no);
   fChain->SetBranchAddress("track_rp_100.v_sect_no", &track_rp_100_v_sect_no, &b_track_rp_100_v_sect_no);
   fChain->SetBranchAddress("track_rp_100.u_id", &track_rp_100_u_id, &b_track_rp_100_u_id);
   fChain->SetBranchAddress("track_rp_100.v_id", &track_rp_100_v_id, &b_track_rp_100_v_id);
   fChain->SetBranchAddress("multi_track_rp_100", &multi_track_rp_100_, &b_multi_track_rp_100_);
   fChain->SetBranchAddress("multi_track_rp_100.valid", &multi_track_rp_100_valid, &b_multi_track_rp_100_valid);
   fChain->SetBranchAddress("multi_track_rp_100.x", &multi_track_rp_100_x, &b_multi_track_rp_100_x);
   fChain->SetBranchAddress("multi_track_rp_100.y", &multi_track_rp_100_y, &b_multi_track_rp_100_y);
   fChain->SetBranchAddress("multi_track_rp_100.z", &multi_track_rp_100_z, &b_multi_track_rp_100_z);
   fChain->SetBranchAddress("multi_track_rp_100.thx", &multi_track_rp_100_thx, &b_multi_track_rp_100_thx);
   fChain->SetBranchAddress("multi_track_rp_100.thy", &multi_track_rp_100_thy, &b_multi_track_rp_100_thy);
   fChain->SetBranchAddress("multi_track_rp_100.chi2", &multi_track_rp_100_chi2, &b_multi_track_rp_100_chi2);
   fChain->SetBranchAddress("multi_track_rp_100.chi2ndf", &multi_track_rp_100_chi2ndf, &b_multi_track_rp_100_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_100.entries", &multi_track_rp_100_entries, &b_multi_track_rp_100_entries);
   fChain->SetBranchAddress("multi_track_rp_100.res_x", &multi_track_rp_100_res_x, &b_multi_track_rp_100_res_x);
   fChain->SetBranchAddress("multi_track_rp_100.res_y", &multi_track_rp_100_res_y, &b_multi_track_rp_100_res_y);
   fChain->SetBranchAddress("multi_track_rp_100.u_sect", &multi_track_rp_100_u_sect, &b_multi_track_rp_100_u_sect);
   fChain->SetBranchAddress("multi_track_rp_100.v_sect", &multi_track_rp_100_v_sect, &b_multi_track_rp_100_v_sect);
   fChain->SetBranchAddress("multi_track_rp_100.u_sect_no", &multi_track_rp_100_u_sect_no, &b_multi_track_rp_100_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_100.v_sect_no", &multi_track_rp_100_v_sect_no, &b_multi_track_rp_100_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_100.u_id", &multi_track_rp_100_u_id, &b_multi_track_rp_100_u_id);
   fChain->SetBranchAddress("multi_track_rp_100.v_id", &multi_track_rp_100_v_id, &b_multi_track_rp_100_v_id);
   fChain->SetBranchAddress("track_rp_101.valid", &track_rp_101_valid, &b_track_rp_101_valid);
   fChain->SetBranchAddress("track_rp_101.x", &track_rp_101_x, &b_track_rp_101_x);
   fChain->SetBranchAddress("track_rp_101.y", &track_rp_101_y, &b_track_rp_101_y);
   fChain->SetBranchAddress("track_rp_101.z", &track_rp_101_z, &b_track_rp_101_z);
   fChain->SetBranchAddress("track_rp_101.thx", &track_rp_101_thx, &b_track_rp_101_thx);
   fChain->SetBranchAddress("track_rp_101.thy", &track_rp_101_thy, &b_track_rp_101_thy);
   fChain->SetBranchAddress("track_rp_101.chi2", &track_rp_101_chi2, &b_track_rp_101_chi2);
   fChain->SetBranchAddress("track_rp_101.chi2ndf", &track_rp_101_chi2ndf, &b_track_rp_101_chi2ndf);
   fChain->SetBranchAddress("track_rp_101.entries", &track_rp_101_entries, &b_track_rp_101_entries);
   fChain->SetBranchAddress("track_rp_101.res_x", &track_rp_101_res_x, &b_track_rp_101_res_x);
   fChain->SetBranchAddress("track_rp_101.res_y", &track_rp_101_res_y, &b_track_rp_101_res_y);
   fChain->SetBranchAddress("track_rp_101.u_sect", &track_rp_101_u_sect, &b_track_rp_101_u_sect);
   fChain->SetBranchAddress("track_rp_101.v_sect", &track_rp_101_v_sect, &b_track_rp_101_v_sect);
   fChain->SetBranchAddress("track_rp_101.u_sect_no", &track_rp_101_u_sect_no, &b_track_rp_101_u_sect_no);
   fChain->SetBranchAddress("track_rp_101.v_sect_no", &track_rp_101_v_sect_no, &b_track_rp_101_v_sect_no);
   fChain->SetBranchAddress("track_rp_101.u_id", &track_rp_101_u_id, &b_track_rp_101_u_id);
   fChain->SetBranchAddress("track_rp_101.v_id", &track_rp_101_v_id, &b_track_rp_101_v_id);
   fChain->SetBranchAddress("multi_track_rp_101", &multi_track_rp_101_, &b_multi_track_rp_101_);
   fChain->SetBranchAddress("multi_track_rp_101.valid", &multi_track_rp_101_valid, &b_multi_track_rp_101_valid);
   fChain->SetBranchAddress("multi_track_rp_101.x", &multi_track_rp_101_x, &b_multi_track_rp_101_x);
   fChain->SetBranchAddress("multi_track_rp_101.y", &multi_track_rp_101_y, &b_multi_track_rp_101_y);
   fChain->SetBranchAddress("multi_track_rp_101.z", &multi_track_rp_101_z, &b_multi_track_rp_101_z);
   fChain->SetBranchAddress("multi_track_rp_101.thx", &multi_track_rp_101_thx, &b_multi_track_rp_101_thx);
   fChain->SetBranchAddress("multi_track_rp_101.thy", &multi_track_rp_101_thy, &b_multi_track_rp_101_thy);
   fChain->SetBranchAddress("multi_track_rp_101.chi2", &multi_track_rp_101_chi2, &b_multi_track_rp_101_chi2);
   fChain->SetBranchAddress("multi_track_rp_101.chi2ndf", &multi_track_rp_101_chi2ndf, &b_multi_track_rp_101_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_101.entries", &multi_track_rp_101_entries, &b_multi_track_rp_101_entries);
   fChain->SetBranchAddress("multi_track_rp_101.res_x", &multi_track_rp_101_res_x, &b_multi_track_rp_101_res_x);
   fChain->SetBranchAddress("multi_track_rp_101.res_y", &multi_track_rp_101_res_y, &b_multi_track_rp_101_res_y);
   fChain->SetBranchAddress("multi_track_rp_101.u_sect", &multi_track_rp_101_u_sect, &b_multi_track_rp_101_u_sect);
   fChain->SetBranchAddress("multi_track_rp_101.v_sect", &multi_track_rp_101_v_sect, &b_multi_track_rp_101_v_sect);
   fChain->SetBranchAddress("multi_track_rp_101.u_sect_no", &multi_track_rp_101_u_sect_no, &b_multi_track_rp_101_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_101.v_sect_no", &multi_track_rp_101_v_sect_no, &b_multi_track_rp_101_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_101.u_id", &multi_track_rp_101_u_id, &b_multi_track_rp_101_u_id);
   fChain->SetBranchAddress("multi_track_rp_101.v_id", &multi_track_rp_101_v_id, &b_multi_track_rp_101_v_id);
   fChain->SetBranchAddress("track_rp_102.valid", &track_rp_102_valid, &b_track_rp_102_valid);
   fChain->SetBranchAddress("track_rp_102.x", &track_rp_102_x, &b_track_rp_102_x);
   fChain->SetBranchAddress("track_rp_102.y", &track_rp_102_y, &b_track_rp_102_y);
   fChain->SetBranchAddress("track_rp_102.z", &track_rp_102_z, &b_track_rp_102_z);
   fChain->SetBranchAddress("track_rp_102.thx", &track_rp_102_thx, &b_track_rp_102_thx);
   fChain->SetBranchAddress("track_rp_102.thy", &track_rp_102_thy, &b_track_rp_102_thy);
   fChain->SetBranchAddress("track_rp_102.chi2", &track_rp_102_chi2, &b_track_rp_102_chi2);
   fChain->SetBranchAddress("track_rp_102.chi2ndf", &track_rp_102_chi2ndf, &b_track_rp_102_chi2ndf);
   fChain->SetBranchAddress("track_rp_102.entries", &track_rp_102_entries, &b_track_rp_102_entries);
   fChain->SetBranchAddress("track_rp_102.res_x", &track_rp_102_res_x, &b_track_rp_102_res_x);
   fChain->SetBranchAddress("track_rp_102.res_y", &track_rp_102_res_y, &b_track_rp_102_res_y);
   fChain->SetBranchAddress("track_rp_102.u_sect", &track_rp_102_u_sect, &b_track_rp_102_u_sect);
   fChain->SetBranchAddress("track_rp_102.v_sect", &track_rp_102_v_sect, &b_track_rp_102_v_sect);
   fChain->SetBranchAddress("track_rp_102.u_sect_no", &track_rp_102_u_sect_no, &b_track_rp_102_u_sect_no);
   fChain->SetBranchAddress("track_rp_102.v_sect_no", &track_rp_102_v_sect_no, &b_track_rp_102_v_sect_no);
   fChain->SetBranchAddress("track_rp_102.u_id", &track_rp_102_u_id, &b_track_rp_102_u_id);
   fChain->SetBranchAddress("track_rp_102.v_id", &track_rp_102_v_id, &b_track_rp_102_v_id);
   fChain->SetBranchAddress("multi_track_rp_102", &multi_track_rp_102_, &b_multi_track_rp_102_);
   fChain->SetBranchAddress("multi_track_rp_102.valid", &multi_track_rp_102_valid, &b_multi_track_rp_102_valid);
   fChain->SetBranchAddress("multi_track_rp_102.x", &multi_track_rp_102_x, &b_multi_track_rp_102_x);
   fChain->SetBranchAddress("multi_track_rp_102.y", &multi_track_rp_102_y, &b_multi_track_rp_102_y);
   fChain->SetBranchAddress("multi_track_rp_102.z", &multi_track_rp_102_z, &b_multi_track_rp_102_z);
   fChain->SetBranchAddress("multi_track_rp_102.thx", &multi_track_rp_102_thx, &b_multi_track_rp_102_thx);
   fChain->SetBranchAddress("multi_track_rp_102.thy", &multi_track_rp_102_thy, &b_multi_track_rp_102_thy);
   fChain->SetBranchAddress("multi_track_rp_102.chi2", &multi_track_rp_102_chi2, &b_multi_track_rp_102_chi2);
   fChain->SetBranchAddress("multi_track_rp_102.chi2ndf", &multi_track_rp_102_chi2ndf, &b_multi_track_rp_102_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_102.entries", &multi_track_rp_102_entries, &b_multi_track_rp_102_entries);
   fChain->SetBranchAddress("multi_track_rp_102.res_x", &multi_track_rp_102_res_x, &b_multi_track_rp_102_res_x);
   fChain->SetBranchAddress("multi_track_rp_102.res_y", &multi_track_rp_102_res_y, &b_multi_track_rp_102_res_y);
   fChain->SetBranchAddress("multi_track_rp_102.u_sect", &multi_track_rp_102_u_sect, &b_multi_track_rp_102_u_sect);
   fChain->SetBranchAddress("multi_track_rp_102.v_sect", &multi_track_rp_102_v_sect, &b_multi_track_rp_102_v_sect);
   fChain->SetBranchAddress("multi_track_rp_102.u_sect_no", &multi_track_rp_102_u_sect_no, &b_multi_track_rp_102_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_102.v_sect_no", &multi_track_rp_102_v_sect_no, &b_multi_track_rp_102_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_102.u_id", &multi_track_rp_102_u_id, &b_multi_track_rp_102_u_id);
   fChain->SetBranchAddress("multi_track_rp_102.v_id", &multi_track_rp_102_v_id, &b_multi_track_rp_102_v_id);
   fChain->SetBranchAddress("track_rp_103.valid", &track_rp_103_valid, &b_track_rp_103_valid);
   fChain->SetBranchAddress("track_rp_103.x", &track_rp_103_x, &b_track_rp_103_x);
   fChain->SetBranchAddress("track_rp_103.y", &track_rp_103_y, &b_track_rp_103_y);
   fChain->SetBranchAddress("track_rp_103.z", &track_rp_103_z, &b_track_rp_103_z);
   fChain->SetBranchAddress("track_rp_103.thx", &track_rp_103_thx, &b_track_rp_103_thx);
   fChain->SetBranchAddress("track_rp_103.thy", &track_rp_103_thy, &b_track_rp_103_thy);
   fChain->SetBranchAddress("track_rp_103.chi2", &track_rp_103_chi2, &b_track_rp_103_chi2);
   fChain->SetBranchAddress("track_rp_103.chi2ndf", &track_rp_103_chi2ndf, &b_track_rp_103_chi2ndf);
   fChain->SetBranchAddress("track_rp_103.entries", &track_rp_103_entries, &b_track_rp_103_entries);
   fChain->SetBranchAddress("track_rp_103.res_x", &track_rp_103_res_x, &b_track_rp_103_res_x);
   fChain->SetBranchAddress("track_rp_103.res_y", &track_rp_103_res_y, &b_track_rp_103_res_y);
   fChain->SetBranchAddress("track_rp_103.u_sect", &track_rp_103_u_sect, &b_track_rp_103_u_sect);
   fChain->SetBranchAddress("track_rp_103.v_sect", &track_rp_103_v_sect, &b_track_rp_103_v_sect);
   fChain->SetBranchAddress("track_rp_103.u_sect_no", &track_rp_103_u_sect_no, &b_track_rp_103_u_sect_no);
   fChain->SetBranchAddress("track_rp_103.v_sect_no", &track_rp_103_v_sect_no, &b_track_rp_103_v_sect_no);
   fChain->SetBranchAddress("track_rp_103.u_id", &track_rp_103_u_id, &b_track_rp_103_u_id);
   fChain->SetBranchAddress("track_rp_103.v_id", &track_rp_103_v_id, &b_track_rp_103_v_id);
   fChain->SetBranchAddress("multi_track_rp_103", &multi_track_rp_103_, &b_multi_track_rp_103_);
   fChain->SetBranchAddress("multi_track_rp_103.valid", &multi_track_rp_103_valid, &b_multi_track_rp_103_valid);
   fChain->SetBranchAddress("multi_track_rp_103.x", &multi_track_rp_103_x, &b_multi_track_rp_103_x);
   fChain->SetBranchAddress("multi_track_rp_103.y", &multi_track_rp_103_y, &b_multi_track_rp_103_y);
   fChain->SetBranchAddress("multi_track_rp_103.z", &multi_track_rp_103_z, &b_multi_track_rp_103_z);
   fChain->SetBranchAddress("multi_track_rp_103.thx", &multi_track_rp_103_thx, &b_multi_track_rp_103_thx);
   fChain->SetBranchAddress("multi_track_rp_103.thy", &multi_track_rp_103_thy, &b_multi_track_rp_103_thy);
   fChain->SetBranchAddress("multi_track_rp_103.chi2", &multi_track_rp_103_chi2, &b_multi_track_rp_103_chi2);
   fChain->SetBranchAddress("multi_track_rp_103.chi2ndf", &multi_track_rp_103_chi2ndf, &b_multi_track_rp_103_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_103.entries", &multi_track_rp_103_entries, &b_multi_track_rp_103_entries);
   fChain->SetBranchAddress("multi_track_rp_103.res_x", &multi_track_rp_103_res_x, &b_multi_track_rp_103_res_x);
   fChain->SetBranchAddress("multi_track_rp_103.res_y", &multi_track_rp_103_res_y, &b_multi_track_rp_103_res_y);
   fChain->SetBranchAddress("multi_track_rp_103.u_sect", &multi_track_rp_103_u_sect, &b_multi_track_rp_103_u_sect);
   fChain->SetBranchAddress("multi_track_rp_103.v_sect", &multi_track_rp_103_v_sect, &b_multi_track_rp_103_v_sect);
   fChain->SetBranchAddress("multi_track_rp_103.u_sect_no", &multi_track_rp_103_u_sect_no, &b_multi_track_rp_103_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_103.v_sect_no", &multi_track_rp_103_v_sect_no, &b_multi_track_rp_103_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_103.u_id", &multi_track_rp_103_u_id, &b_multi_track_rp_103_u_id);
   fChain->SetBranchAddress("multi_track_rp_103.v_id", &multi_track_rp_103_v_id, &b_multi_track_rp_103_v_id);
   fChain->SetBranchAddress("track_rp_104.valid", &track_rp_104_valid, &b_track_rp_104_valid);
   fChain->SetBranchAddress("track_rp_104.x", &track_rp_104_x, &b_track_rp_104_x);
   fChain->SetBranchAddress("track_rp_104.y", &track_rp_104_y, &b_track_rp_104_y);
   fChain->SetBranchAddress("track_rp_104.z", &track_rp_104_z, &b_track_rp_104_z);
   fChain->SetBranchAddress("track_rp_104.thx", &track_rp_104_thx, &b_track_rp_104_thx);
   fChain->SetBranchAddress("track_rp_104.thy", &track_rp_104_thy, &b_track_rp_104_thy);
   fChain->SetBranchAddress("track_rp_104.chi2", &track_rp_104_chi2, &b_track_rp_104_chi2);
   fChain->SetBranchAddress("track_rp_104.chi2ndf", &track_rp_104_chi2ndf, &b_track_rp_104_chi2ndf);
   fChain->SetBranchAddress("track_rp_104.entries", &track_rp_104_entries, &b_track_rp_104_entries);
   fChain->SetBranchAddress("track_rp_104.res_x", &track_rp_104_res_x, &b_track_rp_104_res_x);
   fChain->SetBranchAddress("track_rp_104.res_y", &track_rp_104_res_y, &b_track_rp_104_res_y);
   fChain->SetBranchAddress("track_rp_104.u_sect", &track_rp_104_u_sect, &b_track_rp_104_u_sect);
   fChain->SetBranchAddress("track_rp_104.v_sect", &track_rp_104_v_sect, &b_track_rp_104_v_sect);
   fChain->SetBranchAddress("track_rp_104.u_sect_no", &track_rp_104_u_sect_no, &b_track_rp_104_u_sect_no);
   fChain->SetBranchAddress("track_rp_104.v_sect_no", &track_rp_104_v_sect_no, &b_track_rp_104_v_sect_no);
   fChain->SetBranchAddress("track_rp_104.u_id", &track_rp_104_u_id, &b_track_rp_104_u_id);
   fChain->SetBranchAddress("track_rp_104.v_id", &track_rp_104_v_id, &b_track_rp_104_v_id);
   fChain->SetBranchAddress("multi_track_rp_104", &multi_track_rp_104_, &b_multi_track_rp_104_);
   fChain->SetBranchAddress("multi_track_rp_104.valid", &multi_track_rp_104_valid, &b_multi_track_rp_104_valid);
   fChain->SetBranchAddress("multi_track_rp_104.x", &multi_track_rp_104_x, &b_multi_track_rp_104_x);
   fChain->SetBranchAddress("multi_track_rp_104.y", &multi_track_rp_104_y, &b_multi_track_rp_104_y);
   fChain->SetBranchAddress("multi_track_rp_104.z", &multi_track_rp_104_z, &b_multi_track_rp_104_z);
   fChain->SetBranchAddress("multi_track_rp_104.thx", &multi_track_rp_104_thx, &b_multi_track_rp_104_thx);
   fChain->SetBranchAddress("multi_track_rp_104.thy", &multi_track_rp_104_thy, &b_multi_track_rp_104_thy);
   fChain->SetBranchAddress("multi_track_rp_104.chi2", &multi_track_rp_104_chi2, &b_multi_track_rp_104_chi2);
   fChain->SetBranchAddress("multi_track_rp_104.chi2ndf", &multi_track_rp_104_chi2ndf, &b_multi_track_rp_104_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_104.entries", &multi_track_rp_104_entries, &b_multi_track_rp_104_entries);
   fChain->SetBranchAddress("multi_track_rp_104.res_x", &multi_track_rp_104_res_x, &b_multi_track_rp_104_res_x);
   fChain->SetBranchAddress("multi_track_rp_104.res_y", &multi_track_rp_104_res_y, &b_multi_track_rp_104_res_y);
   fChain->SetBranchAddress("multi_track_rp_104.u_sect", &multi_track_rp_104_u_sect, &b_multi_track_rp_104_u_sect);
   fChain->SetBranchAddress("multi_track_rp_104.v_sect", &multi_track_rp_104_v_sect, &b_multi_track_rp_104_v_sect);
   fChain->SetBranchAddress("multi_track_rp_104.u_sect_no", &multi_track_rp_104_u_sect_no, &b_multi_track_rp_104_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_104.v_sect_no", &multi_track_rp_104_v_sect_no, &b_multi_track_rp_104_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_104.u_id", &multi_track_rp_104_u_id, &b_multi_track_rp_104_u_id);
   fChain->SetBranchAddress("multi_track_rp_104.v_id", &multi_track_rp_104_v_id, &b_multi_track_rp_104_v_id);
   fChain->SetBranchAddress("track_rp_105.valid", &track_rp_105_valid, &b_track_rp_105_valid);
   fChain->SetBranchAddress("track_rp_105.x", &track_rp_105_x, &b_track_rp_105_x);
   fChain->SetBranchAddress("track_rp_105.y", &track_rp_105_y, &b_track_rp_105_y);
   fChain->SetBranchAddress("track_rp_105.z", &track_rp_105_z, &b_track_rp_105_z);
   fChain->SetBranchAddress("track_rp_105.thx", &track_rp_105_thx, &b_track_rp_105_thx);
   fChain->SetBranchAddress("track_rp_105.thy", &track_rp_105_thy, &b_track_rp_105_thy);
   fChain->SetBranchAddress("track_rp_105.chi2", &track_rp_105_chi2, &b_track_rp_105_chi2);
   fChain->SetBranchAddress("track_rp_105.chi2ndf", &track_rp_105_chi2ndf, &b_track_rp_105_chi2ndf);
   fChain->SetBranchAddress("track_rp_105.entries", &track_rp_105_entries, &b_track_rp_105_entries);
   fChain->SetBranchAddress("track_rp_105.res_x", &track_rp_105_res_x, &b_track_rp_105_res_x);
   fChain->SetBranchAddress("track_rp_105.res_y", &track_rp_105_res_y, &b_track_rp_105_res_y);
   fChain->SetBranchAddress("track_rp_105.u_sect", &track_rp_105_u_sect, &b_track_rp_105_u_sect);
   fChain->SetBranchAddress("track_rp_105.v_sect", &track_rp_105_v_sect, &b_track_rp_105_v_sect);
   fChain->SetBranchAddress("track_rp_105.u_sect_no", &track_rp_105_u_sect_no, &b_track_rp_105_u_sect_no);
   fChain->SetBranchAddress("track_rp_105.v_sect_no", &track_rp_105_v_sect_no, &b_track_rp_105_v_sect_no);
   fChain->SetBranchAddress("track_rp_105.u_id", &track_rp_105_u_id, &b_track_rp_105_u_id);
   fChain->SetBranchAddress("track_rp_105.v_id", &track_rp_105_v_id, &b_track_rp_105_v_id);
   fChain->SetBranchAddress("multi_track_rp_105", &multi_track_rp_105_, &b_multi_track_rp_105_);
   fChain->SetBranchAddress("multi_track_rp_105.valid", &multi_track_rp_105_valid, &b_multi_track_rp_105_valid);
   fChain->SetBranchAddress("multi_track_rp_105.x", &multi_track_rp_105_x, &b_multi_track_rp_105_x);
   fChain->SetBranchAddress("multi_track_rp_105.y", &multi_track_rp_105_y, &b_multi_track_rp_105_y);
   fChain->SetBranchAddress("multi_track_rp_105.z", &multi_track_rp_105_z, &b_multi_track_rp_105_z);
   fChain->SetBranchAddress("multi_track_rp_105.thx", &multi_track_rp_105_thx, &b_multi_track_rp_105_thx);
   fChain->SetBranchAddress("multi_track_rp_105.thy", &multi_track_rp_105_thy, &b_multi_track_rp_105_thy);
   fChain->SetBranchAddress("multi_track_rp_105.chi2", &multi_track_rp_105_chi2, &b_multi_track_rp_105_chi2);
   fChain->SetBranchAddress("multi_track_rp_105.chi2ndf", &multi_track_rp_105_chi2ndf, &b_multi_track_rp_105_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_105.entries", &multi_track_rp_105_entries, &b_multi_track_rp_105_entries);
   fChain->SetBranchAddress("multi_track_rp_105.res_x", &multi_track_rp_105_res_x, &b_multi_track_rp_105_res_x);
   fChain->SetBranchAddress("multi_track_rp_105.res_y", &multi_track_rp_105_res_y, &b_multi_track_rp_105_res_y);
   fChain->SetBranchAddress("multi_track_rp_105.u_sect", &multi_track_rp_105_u_sect, &b_multi_track_rp_105_u_sect);
   fChain->SetBranchAddress("multi_track_rp_105.v_sect", &multi_track_rp_105_v_sect, &b_multi_track_rp_105_v_sect);
   fChain->SetBranchAddress("multi_track_rp_105.u_sect_no", &multi_track_rp_105_u_sect_no, &b_multi_track_rp_105_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_105.v_sect_no", &multi_track_rp_105_v_sect_no, &b_multi_track_rp_105_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_105.u_id", &multi_track_rp_105_u_id, &b_multi_track_rp_105_u_id);
   fChain->SetBranchAddress("multi_track_rp_105.v_id", &multi_track_rp_105_v_id, &b_multi_track_rp_105_v_id);
   fChain->SetBranchAddress("track_rp_120.valid", &track_rp_120_valid, &b_track_rp_120_valid);
   fChain->SetBranchAddress("track_rp_120.x", &track_rp_120_x, &b_track_rp_120_x);
   fChain->SetBranchAddress("track_rp_120.y", &track_rp_120_y, &b_track_rp_120_y);
   fChain->SetBranchAddress("track_rp_120.z", &track_rp_120_z, &b_track_rp_120_z);
   fChain->SetBranchAddress("track_rp_120.thx", &track_rp_120_thx, &b_track_rp_120_thx);
   fChain->SetBranchAddress("track_rp_120.thy", &track_rp_120_thy, &b_track_rp_120_thy);
   fChain->SetBranchAddress("track_rp_120.chi2", &track_rp_120_chi2, &b_track_rp_120_chi2);
   fChain->SetBranchAddress("track_rp_120.chi2ndf", &track_rp_120_chi2ndf, &b_track_rp_120_chi2ndf);
   fChain->SetBranchAddress("track_rp_120.entries", &track_rp_120_entries, &b_track_rp_120_entries);
   fChain->SetBranchAddress("track_rp_120.res_x", &track_rp_120_res_x, &b_track_rp_120_res_x);
   fChain->SetBranchAddress("track_rp_120.res_y", &track_rp_120_res_y, &b_track_rp_120_res_y);
   fChain->SetBranchAddress("track_rp_120.u_sect", &track_rp_120_u_sect, &b_track_rp_120_u_sect);
   fChain->SetBranchAddress("track_rp_120.v_sect", &track_rp_120_v_sect, &b_track_rp_120_v_sect);
   fChain->SetBranchAddress("track_rp_120.u_sect_no", &track_rp_120_u_sect_no, &b_track_rp_120_u_sect_no);
   fChain->SetBranchAddress("track_rp_120.v_sect_no", &track_rp_120_v_sect_no, &b_track_rp_120_v_sect_no);
   fChain->SetBranchAddress("track_rp_120.u_id", &track_rp_120_u_id, &b_track_rp_120_u_id);
   fChain->SetBranchAddress("track_rp_120.v_id", &track_rp_120_v_id, &b_track_rp_120_v_id);
   fChain->SetBranchAddress("multi_track_rp_120", &multi_track_rp_120_, &b_multi_track_rp_120_);
   fChain->SetBranchAddress("multi_track_rp_120.valid", &multi_track_rp_120_valid, &b_multi_track_rp_120_valid);
   fChain->SetBranchAddress("multi_track_rp_120.x", &multi_track_rp_120_x, &b_multi_track_rp_120_x);
   fChain->SetBranchAddress("multi_track_rp_120.y", &multi_track_rp_120_y, &b_multi_track_rp_120_y);
   fChain->SetBranchAddress("multi_track_rp_120.z", &multi_track_rp_120_z, &b_multi_track_rp_120_z);
   fChain->SetBranchAddress("multi_track_rp_120.thx", &multi_track_rp_120_thx, &b_multi_track_rp_120_thx);
   fChain->SetBranchAddress("multi_track_rp_120.thy", &multi_track_rp_120_thy, &b_multi_track_rp_120_thy);
   fChain->SetBranchAddress("multi_track_rp_120.chi2", &multi_track_rp_120_chi2, &b_multi_track_rp_120_chi2);
   fChain->SetBranchAddress("multi_track_rp_120.chi2ndf", &multi_track_rp_120_chi2ndf, &b_multi_track_rp_120_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_120.entries", &multi_track_rp_120_entries, &b_multi_track_rp_120_entries);
   fChain->SetBranchAddress("multi_track_rp_120.res_x", &multi_track_rp_120_res_x, &b_multi_track_rp_120_res_x);
   fChain->SetBranchAddress("multi_track_rp_120.res_y", &multi_track_rp_120_res_y, &b_multi_track_rp_120_res_y);
   fChain->SetBranchAddress("multi_track_rp_120.u_sect", &multi_track_rp_120_u_sect, &b_multi_track_rp_120_u_sect);
   fChain->SetBranchAddress("multi_track_rp_120.v_sect", &multi_track_rp_120_v_sect, &b_multi_track_rp_120_v_sect);
   fChain->SetBranchAddress("multi_track_rp_120.u_sect_no", &multi_track_rp_120_u_sect_no, &b_multi_track_rp_120_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_120.v_sect_no", &multi_track_rp_120_v_sect_no, &b_multi_track_rp_120_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_120.u_id", &multi_track_rp_120_u_id, &b_multi_track_rp_120_u_id);
   fChain->SetBranchAddress("multi_track_rp_120.v_id", &multi_track_rp_120_v_id, &b_multi_track_rp_120_v_id);
   fChain->SetBranchAddress("track_rp_121.valid", &track_rp_121_valid, &b_track_rp_121_valid);
   fChain->SetBranchAddress("track_rp_121.x", &track_rp_121_x, &b_track_rp_121_x);
   fChain->SetBranchAddress("track_rp_121.y", &track_rp_121_y, &b_track_rp_121_y);
   fChain->SetBranchAddress("track_rp_121.z", &track_rp_121_z, &b_track_rp_121_z);
   fChain->SetBranchAddress("track_rp_121.thx", &track_rp_121_thx, &b_track_rp_121_thx);
   fChain->SetBranchAddress("track_rp_121.thy", &track_rp_121_thy, &b_track_rp_121_thy);
   fChain->SetBranchAddress("track_rp_121.chi2", &track_rp_121_chi2, &b_track_rp_121_chi2);
   fChain->SetBranchAddress("track_rp_121.chi2ndf", &track_rp_121_chi2ndf, &b_track_rp_121_chi2ndf);
   fChain->SetBranchAddress("track_rp_121.entries", &track_rp_121_entries, &b_track_rp_121_entries);
   fChain->SetBranchAddress("track_rp_121.res_x", &track_rp_121_res_x, &b_track_rp_121_res_x);
   fChain->SetBranchAddress("track_rp_121.res_y", &track_rp_121_res_y, &b_track_rp_121_res_y);
   fChain->SetBranchAddress("track_rp_121.u_sect", &track_rp_121_u_sect, &b_track_rp_121_u_sect);
   fChain->SetBranchAddress("track_rp_121.v_sect", &track_rp_121_v_sect, &b_track_rp_121_v_sect);
   fChain->SetBranchAddress("track_rp_121.u_sect_no", &track_rp_121_u_sect_no, &b_track_rp_121_u_sect_no);
   fChain->SetBranchAddress("track_rp_121.v_sect_no", &track_rp_121_v_sect_no, &b_track_rp_121_v_sect_no);
   fChain->SetBranchAddress("track_rp_121.u_id", &track_rp_121_u_id, &b_track_rp_121_u_id);
   fChain->SetBranchAddress("track_rp_121.v_id", &track_rp_121_v_id, &b_track_rp_121_v_id);
   fChain->SetBranchAddress("multi_track_rp_121", &multi_track_rp_121_, &b_multi_track_rp_121_);
   fChain->SetBranchAddress("multi_track_rp_121.valid", &multi_track_rp_121_valid, &b_multi_track_rp_121_valid);
   fChain->SetBranchAddress("multi_track_rp_121.x", &multi_track_rp_121_x, &b_multi_track_rp_121_x);
   fChain->SetBranchAddress("multi_track_rp_121.y", &multi_track_rp_121_y, &b_multi_track_rp_121_y);
   fChain->SetBranchAddress("multi_track_rp_121.z", &multi_track_rp_121_z, &b_multi_track_rp_121_z);
   fChain->SetBranchAddress("multi_track_rp_121.thx", &multi_track_rp_121_thx, &b_multi_track_rp_121_thx);
   fChain->SetBranchAddress("multi_track_rp_121.thy", &multi_track_rp_121_thy, &b_multi_track_rp_121_thy);
   fChain->SetBranchAddress("multi_track_rp_121.chi2", &multi_track_rp_121_chi2, &b_multi_track_rp_121_chi2);
   fChain->SetBranchAddress("multi_track_rp_121.chi2ndf", &multi_track_rp_121_chi2ndf, &b_multi_track_rp_121_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_121.entries", &multi_track_rp_121_entries, &b_multi_track_rp_121_entries);
   fChain->SetBranchAddress("multi_track_rp_121.res_x", &multi_track_rp_121_res_x, &b_multi_track_rp_121_res_x);
   fChain->SetBranchAddress("multi_track_rp_121.res_y", &multi_track_rp_121_res_y, &b_multi_track_rp_121_res_y);
   fChain->SetBranchAddress("multi_track_rp_121.u_sect", &multi_track_rp_121_u_sect, &b_multi_track_rp_121_u_sect);
   fChain->SetBranchAddress("multi_track_rp_121.v_sect", &multi_track_rp_121_v_sect, &b_multi_track_rp_121_v_sect);
   fChain->SetBranchAddress("multi_track_rp_121.u_sect_no", &multi_track_rp_121_u_sect_no, &b_multi_track_rp_121_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_121.v_sect_no", &multi_track_rp_121_v_sect_no, &b_multi_track_rp_121_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_121.u_id", &multi_track_rp_121_u_id, &b_multi_track_rp_121_u_id);
   fChain->SetBranchAddress("multi_track_rp_121.v_id", &multi_track_rp_121_v_id, &b_multi_track_rp_121_v_id);
   fChain->SetBranchAddress("track_rp_122.valid", &track_rp_122_valid, &b_track_rp_122_valid);
   fChain->SetBranchAddress("track_rp_122.x", &track_rp_122_x, &b_track_rp_122_x);
   fChain->SetBranchAddress("track_rp_122.y", &track_rp_122_y, &b_track_rp_122_y);
   fChain->SetBranchAddress("track_rp_122.z", &track_rp_122_z, &b_track_rp_122_z);
   fChain->SetBranchAddress("track_rp_122.thx", &track_rp_122_thx, &b_track_rp_122_thx);
   fChain->SetBranchAddress("track_rp_122.thy", &track_rp_122_thy, &b_track_rp_122_thy);
   fChain->SetBranchAddress("track_rp_122.chi2", &track_rp_122_chi2, &b_track_rp_122_chi2);
   fChain->SetBranchAddress("track_rp_122.chi2ndf", &track_rp_122_chi2ndf, &b_track_rp_122_chi2ndf);
   fChain->SetBranchAddress("track_rp_122.entries", &track_rp_122_entries, &b_track_rp_122_entries);
   fChain->SetBranchAddress("track_rp_122.res_x", &track_rp_122_res_x, &b_track_rp_122_res_x);
   fChain->SetBranchAddress("track_rp_122.res_y", &track_rp_122_res_y, &b_track_rp_122_res_y);
   fChain->SetBranchAddress("track_rp_122.u_sect", &track_rp_122_u_sect, &b_track_rp_122_u_sect);
   fChain->SetBranchAddress("track_rp_122.v_sect", &track_rp_122_v_sect, &b_track_rp_122_v_sect);
   fChain->SetBranchAddress("track_rp_122.u_sect_no", &track_rp_122_u_sect_no, &b_track_rp_122_u_sect_no);
   fChain->SetBranchAddress("track_rp_122.v_sect_no", &track_rp_122_v_sect_no, &b_track_rp_122_v_sect_no);
   fChain->SetBranchAddress("track_rp_122.u_id", &track_rp_122_u_id, &b_track_rp_122_u_id);
   fChain->SetBranchAddress("track_rp_122.v_id", &track_rp_122_v_id, &b_track_rp_122_v_id);
   fChain->SetBranchAddress("multi_track_rp_122", &multi_track_rp_122_, &b_multi_track_rp_122_);
   fChain->SetBranchAddress("multi_track_rp_122.valid", &multi_track_rp_122_valid, &b_multi_track_rp_122_valid);
   fChain->SetBranchAddress("multi_track_rp_122.x", &multi_track_rp_122_x, &b_multi_track_rp_122_x);
   fChain->SetBranchAddress("multi_track_rp_122.y", &multi_track_rp_122_y, &b_multi_track_rp_122_y);
   fChain->SetBranchAddress("multi_track_rp_122.z", &multi_track_rp_122_z, &b_multi_track_rp_122_z);
   fChain->SetBranchAddress("multi_track_rp_122.thx", &multi_track_rp_122_thx, &b_multi_track_rp_122_thx);
   fChain->SetBranchAddress("multi_track_rp_122.thy", &multi_track_rp_122_thy, &b_multi_track_rp_122_thy);
   fChain->SetBranchAddress("multi_track_rp_122.chi2", &multi_track_rp_122_chi2, &b_multi_track_rp_122_chi2);
   fChain->SetBranchAddress("multi_track_rp_122.chi2ndf", &multi_track_rp_122_chi2ndf, &b_multi_track_rp_122_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_122.entries", &multi_track_rp_122_entries, &b_multi_track_rp_122_entries);
   fChain->SetBranchAddress("multi_track_rp_122.res_x", &multi_track_rp_122_res_x, &b_multi_track_rp_122_res_x);
   fChain->SetBranchAddress("multi_track_rp_122.res_y", &multi_track_rp_122_res_y, &b_multi_track_rp_122_res_y);
   fChain->SetBranchAddress("multi_track_rp_122.u_sect", &multi_track_rp_122_u_sect, &b_multi_track_rp_122_u_sect);
   fChain->SetBranchAddress("multi_track_rp_122.v_sect", &multi_track_rp_122_v_sect, &b_multi_track_rp_122_v_sect);
   fChain->SetBranchAddress("multi_track_rp_122.u_sect_no", &multi_track_rp_122_u_sect_no, &b_multi_track_rp_122_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_122.v_sect_no", &multi_track_rp_122_v_sect_no, &b_multi_track_rp_122_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_122.u_id", &multi_track_rp_122_u_id, &b_multi_track_rp_122_u_id);
   fChain->SetBranchAddress("multi_track_rp_122.v_id", &multi_track_rp_122_v_id, &b_multi_track_rp_122_v_id);
   fChain->SetBranchAddress("track_rp_123.valid", &track_rp_123_valid, &b_track_rp_123_valid);
   fChain->SetBranchAddress("track_rp_123.x", &track_rp_123_x, &b_track_rp_123_x);
   fChain->SetBranchAddress("track_rp_123.y", &track_rp_123_y, &b_track_rp_123_y);
   fChain->SetBranchAddress("track_rp_123.z", &track_rp_123_z, &b_track_rp_123_z);
   fChain->SetBranchAddress("track_rp_123.thx", &track_rp_123_thx, &b_track_rp_123_thx);
   fChain->SetBranchAddress("track_rp_123.thy", &track_rp_123_thy, &b_track_rp_123_thy);
   fChain->SetBranchAddress("track_rp_123.chi2", &track_rp_123_chi2, &b_track_rp_123_chi2);
   fChain->SetBranchAddress("track_rp_123.chi2ndf", &track_rp_123_chi2ndf, &b_track_rp_123_chi2ndf);
   fChain->SetBranchAddress("track_rp_123.entries", &track_rp_123_entries, &b_track_rp_123_entries);
   fChain->SetBranchAddress("track_rp_123.res_x", &track_rp_123_res_x, &b_track_rp_123_res_x);
   fChain->SetBranchAddress("track_rp_123.res_y", &track_rp_123_res_y, &b_track_rp_123_res_y);
   fChain->SetBranchAddress("track_rp_123.u_sect", &track_rp_123_u_sect, &b_track_rp_123_u_sect);
   fChain->SetBranchAddress("track_rp_123.v_sect", &track_rp_123_v_sect, &b_track_rp_123_v_sect);
   fChain->SetBranchAddress("track_rp_123.u_sect_no", &track_rp_123_u_sect_no, &b_track_rp_123_u_sect_no);
   fChain->SetBranchAddress("track_rp_123.v_sect_no", &track_rp_123_v_sect_no, &b_track_rp_123_v_sect_no);
   fChain->SetBranchAddress("track_rp_123.u_id", &track_rp_123_u_id, &b_track_rp_123_u_id);
   fChain->SetBranchAddress("track_rp_123.v_id", &track_rp_123_v_id, &b_track_rp_123_v_id);
   fChain->SetBranchAddress("multi_track_rp_123", &multi_track_rp_123_, &b_multi_track_rp_123_);
   fChain->SetBranchAddress("multi_track_rp_123.valid", &multi_track_rp_123_valid, &b_multi_track_rp_123_valid);
   fChain->SetBranchAddress("multi_track_rp_123.x", &multi_track_rp_123_x, &b_multi_track_rp_123_x);
   fChain->SetBranchAddress("multi_track_rp_123.y", &multi_track_rp_123_y, &b_multi_track_rp_123_y);
   fChain->SetBranchAddress("multi_track_rp_123.z", &multi_track_rp_123_z, &b_multi_track_rp_123_z);
   fChain->SetBranchAddress("multi_track_rp_123.thx", &multi_track_rp_123_thx, &b_multi_track_rp_123_thx);
   fChain->SetBranchAddress("multi_track_rp_123.thy", &multi_track_rp_123_thy, &b_multi_track_rp_123_thy);
   fChain->SetBranchAddress("multi_track_rp_123.chi2", &multi_track_rp_123_chi2, &b_multi_track_rp_123_chi2);
   fChain->SetBranchAddress("multi_track_rp_123.chi2ndf", &multi_track_rp_123_chi2ndf, &b_multi_track_rp_123_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_123.entries", &multi_track_rp_123_entries, &b_multi_track_rp_123_entries);
   fChain->SetBranchAddress("multi_track_rp_123.res_x", &multi_track_rp_123_res_x, &b_multi_track_rp_123_res_x);
   fChain->SetBranchAddress("multi_track_rp_123.res_y", &multi_track_rp_123_res_y, &b_multi_track_rp_123_res_y);
   fChain->SetBranchAddress("multi_track_rp_123.u_sect", &multi_track_rp_123_u_sect, &b_multi_track_rp_123_u_sect);
   fChain->SetBranchAddress("multi_track_rp_123.v_sect", &multi_track_rp_123_v_sect, &b_multi_track_rp_123_v_sect);
   fChain->SetBranchAddress("multi_track_rp_123.u_sect_no", &multi_track_rp_123_u_sect_no, &b_multi_track_rp_123_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_123.v_sect_no", &multi_track_rp_123_v_sect_no, &b_multi_track_rp_123_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_123.u_id", &multi_track_rp_123_u_id, &b_multi_track_rp_123_u_id);
   fChain->SetBranchAddress("multi_track_rp_123.v_id", &multi_track_rp_123_v_id, &b_multi_track_rp_123_v_id);
   fChain->SetBranchAddress("track_rp_124.valid", &track_rp_124_valid, &b_track_rp_124_valid);
   fChain->SetBranchAddress("track_rp_124.x", &track_rp_124_x, &b_track_rp_124_x);
   fChain->SetBranchAddress("track_rp_124.y", &track_rp_124_y, &b_track_rp_124_y);
   fChain->SetBranchAddress("track_rp_124.z", &track_rp_124_z, &b_track_rp_124_z);
   fChain->SetBranchAddress("track_rp_124.thx", &track_rp_124_thx, &b_track_rp_124_thx);
   fChain->SetBranchAddress("track_rp_124.thy", &track_rp_124_thy, &b_track_rp_124_thy);
   fChain->SetBranchAddress("track_rp_124.chi2", &track_rp_124_chi2, &b_track_rp_124_chi2);
   fChain->SetBranchAddress("track_rp_124.chi2ndf", &track_rp_124_chi2ndf, &b_track_rp_124_chi2ndf);
   fChain->SetBranchAddress("track_rp_124.entries", &track_rp_124_entries, &b_track_rp_124_entries);
   fChain->SetBranchAddress("track_rp_124.res_x", &track_rp_124_res_x, &b_track_rp_124_res_x);
   fChain->SetBranchAddress("track_rp_124.res_y", &track_rp_124_res_y, &b_track_rp_124_res_y);
   fChain->SetBranchAddress("track_rp_124.u_sect", &track_rp_124_u_sect, &b_track_rp_124_u_sect);
   fChain->SetBranchAddress("track_rp_124.v_sect", &track_rp_124_v_sect, &b_track_rp_124_v_sect);
   fChain->SetBranchAddress("track_rp_124.u_sect_no", &track_rp_124_u_sect_no, &b_track_rp_124_u_sect_no);
   fChain->SetBranchAddress("track_rp_124.v_sect_no", &track_rp_124_v_sect_no, &b_track_rp_124_v_sect_no);
   fChain->SetBranchAddress("track_rp_124.u_id", &track_rp_124_u_id, &b_track_rp_124_u_id);
   fChain->SetBranchAddress("track_rp_124.v_id", &track_rp_124_v_id, &b_track_rp_124_v_id);
   fChain->SetBranchAddress("multi_track_rp_124", &multi_track_rp_124_, &b_multi_track_rp_124_);
   fChain->SetBranchAddress("multi_track_rp_124.valid", &multi_track_rp_124_valid, &b_multi_track_rp_124_valid);
   fChain->SetBranchAddress("multi_track_rp_124.x", &multi_track_rp_124_x, &b_multi_track_rp_124_x);
   fChain->SetBranchAddress("multi_track_rp_124.y", &multi_track_rp_124_y, &b_multi_track_rp_124_y);
   fChain->SetBranchAddress("multi_track_rp_124.z", &multi_track_rp_124_z, &b_multi_track_rp_124_z);
   fChain->SetBranchAddress("multi_track_rp_124.thx", &multi_track_rp_124_thx, &b_multi_track_rp_124_thx);
   fChain->SetBranchAddress("multi_track_rp_124.thy", &multi_track_rp_124_thy, &b_multi_track_rp_124_thy);
   fChain->SetBranchAddress("multi_track_rp_124.chi2", &multi_track_rp_124_chi2, &b_multi_track_rp_124_chi2);
   fChain->SetBranchAddress("multi_track_rp_124.chi2ndf", &multi_track_rp_124_chi2ndf, &b_multi_track_rp_124_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_124.entries", &multi_track_rp_124_entries, &b_multi_track_rp_124_entries);
   fChain->SetBranchAddress("multi_track_rp_124.res_x", &multi_track_rp_124_res_x, &b_multi_track_rp_124_res_x);
   fChain->SetBranchAddress("multi_track_rp_124.res_y", &multi_track_rp_124_res_y, &b_multi_track_rp_124_res_y);
   fChain->SetBranchAddress("multi_track_rp_124.u_sect", &multi_track_rp_124_u_sect, &b_multi_track_rp_124_u_sect);
   fChain->SetBranchAddress("multi_track_rp_124.v_sect", &multi_track_rp_124_v_sect, &b_multi_track_rp_124_v_sect);
   fChain->SetBranchAddress("multi_track_rp_124.u_sect_no", &multi_track_rp_124_u_sect_no, &b_multi_track_rp_124_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_124.v_sect_no", &multi_track_rp_124_v_sect_no, &b_multi_track_rp_124_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_124.u_id", &multi_track_rp_124_u_id, &b_multi_track_rp_124_u_id);
   fChain->SetBranchAddress("multi_track_rp_124.v_id", &multi_track_rp_124_v_id, &b_multi_track_rp_124_v_id);
   fChain->SetBranchAddress("track_rp_125.valid", &track_rp_125_valid, &b_track_rp_125_valid);
   fChain->SetBranchAddress("track_rp_125.x", &track_rp_125_x, &b_track_rp_125_x);
   fChain->SetBranchAddress("track_rp_125.y", &track_rp_125_y, &b_track_rp_125_y);
   fChain->SetBranchAddress("track_rp_125.z", &track_rp_125_z, &b_track_rp_125_z);
   fChain->SetBranchAddress("track_rp_125.thx", &track_rp_125_thx, &b_track_rp_125_thx);
   fChain->SetBranchAddress("track_rp_125.thy", &track_rp_125_thy, &b_track_rp_125_thy);
   fChain->SetBranchAddress("track_rp_125.chi2", &track_rp_125_chi2, &b_track_rp_125_chi2);
   fChain->SetBranchAddress("track_rp_125.chi2ndf", &track_rp_125_chi2ndf, &b_track_rp_125_chi2ndf);
   fChain->SetBranchAddress("track_rp_125.entries", &track_rp_125_entries, &b_track_rp_125_entries);
   fChain->SetBranchAddress("track_rp_125.res_x", &track_rp_125_res_x, &b_track_rp_125_res_x);
   fChain->SetBranchAddress("track_rp_125.res_y", &track_rp_125_res_y, &b_track_rp_125_res_y);
   fChain->SetBranchAddress("track_rp_125.u_sect", &track_rp_125_u_sect, &b_track_rp_125_u_sect);
   fChain->SetBranchAddress("track_rp_125.v_sect", &track_rp_125_v_sect, &b_track_rp_125_v_sect);
   fChain->SetBranchAddress("track_rp_125.u_sect_no", &track_rp_125_u_sect_no, &b_track_rp_125_u_sect_no);
   fChain->SetBranchAddress("track_rp_125.v_sect_no", &track_rp_125_v_sect_no, &b_track_rp_125_v_sect_no);
   fChain->SetBranchAddress("track_rp_125.u_id", &track_rp_125_u_id, &b_track_rp_125_u_id);
   fChain->SetBranchAddress("track_rp_125.v_id", &track_rp_125_v_id, &b_track_rp_125_v_id);
   fChain->SetBranchAddress("multi_track_rp_125", &multi_track_rp_125_, &b_multi_track_rp_125_);
   fChain->SetBranchAddress("multi_track_rp_125.valid", &multi_track_rp_125_valid, &b_multi_track_rp_125_valid);
   fChain->SetBranchAddress("multi_track_rp_125.x", &multi_track_rp_125_x, &b_multi_track_rp_125_x);
   fChain->SetBranchAddress("multi_track_rp_125.y", &multi_track_rp_125_y, &b_multi_track_rp_125_y);
   fChain->SetBranchAddress("multi_track_rp_125.z", &multi_track_rp_125_z, &b_multi_track_rp_125_z);
   fChain->SetBranchAddress("multi_track_rp_125.thx", &multi_track_rp_125_thx, &b_multi_track_rp_125_thx);
   fChain->SetBranchAddress("multi_track_rp_125.thy", &multi_track_rp_125_thy, &b_multi_track_rp_125_thy);
   fChain->SetBranchAddress("multi_track_rp_125.chi2", &multi_track_rp_125_chi2, &b_multi_track_rp_125_chi2);
   fChain->SetBranchAddress("multi_track_rp_125.chi2ndf", &multi_track_rp_125_chi2ndf, &b_multi_track_rp_125_chi2ndf);
   fChain->SetBranchAddress("multi_track_rp_125.entries", &multi_track_rp_125_entries, &b_multi_track_rp_125_entries);
   fChain->SetBranchAddress("multi_track_rp_125.res_x", &multi_track_rp_125_res_x, &b_multi_track_rp_125_res_x);
   fChain->SetBranchAddress("multi_track_rp_125.res_y", &multi_track_rp_125_res_y, &b_multi_track_rp_125_res_y);
   fChain->SetBranchAddress("multi_track_rp_125.u_sect", &multi_track_rp_125_u_sect, &b_multi_track_rp_125_u_sect);
   fChain->SetBranchAddress("multi_track_rp_125.v_sect", &multi_track_rp_125_v_sect, &b_multi_track_rp_125_v_sect);
   fChain->SetBranchAddress("multi_track_rp_125.u_sect_no", &multi_track_rp_125_u_sect_no, &b_multi_track_rp_125_u_sect_no);
   fChain->SetBranchAddress("multi_track_rp_125.v_sect_no", &multi_track_rp_125_v_sect_no, &b_multi_track_rp_125_v_sect_no);
   fChain->SetBranchAddress("multi_track_rp_125.u_id", &multi_track_rp_125_u_id, &b_multi_track_rp_125_u_id);
   fChain->SetBranchAddress("multi_track_rp_125.v_id", &multi_track_rp_125_v_id, &b_multi_track_rp_125_v_id);
   fChain->SetBranchAddress("rec_prot_left.valid", &rec_prot_left_valid, &b_rec_prot_left_valid);
   fChain->SetBranchAddress("rec_prot_left.thx", &rec_prot_left_thx, &b_rec_prot_left_thx);
   fChain->SetBranchAddress("rec_prot_left.thy", &rec_prot_left_thy, &b_rec_prot_left_thy);
   fChain->SetBranchAddress("rec_prot_left.phi", &rec_prot_left_phi, &b_rec_prot_left_phi);
   fChain->SetBranchAddress("rec_prot_left.t", &rec_prot_left_t, &b_rec_prot_left_t);
   fChain->SetBranchAddress("rec_prot_left.tx", &rec_prot_left_tx, &b_rec_prot_left_tx);
   fChain->SetBranchAddress("rec_prot_left.ty", &rec_prot_left_ty, &b_rec_prot_left_ty);
   fChain->SetBranchAddress("rec_prot_left.xi", &rec_prot_left_xi, &b_rec_prot_left_xi);
   fChain->SetBranchAddress("rec_prot_left.x0", &rec_prot_left_x0, &b_rec_prot_left_x0);
   fChain->SetBranchAddress("rec_prot_left.y0", &rec_prot_left_y0, &b_rec_prot_left_y0);
   fChain->SetBranchAddress("rec_prot_left.chi2", &rec_prot_left_chi2, &b_rec_prot_left_chi2);
   fChain->SetBranchAddress("rec_prot_left.chindf", &rec_prot_left_chindf, &b_rec_prot_left_chindf);
   fChain->SetBranchAddress("rec_prot_right.valid", &rec_prot_right_valid, &b_rec_prot_right_valid);
   fChain->SetBranchAddress("rec_prot_right.thx", &rec_prot_right_thx, &b_rec_prot_right_thx);
   fChain->SetBranchAddress("rec_prot_right.thy", &rec_prot_right_thy, &b_rec_prot_right_thy);
   fChain->SetBranchAddress("rec_prot_right.phi", &rec_prot_right_phi, &b_rec_prot_right_phi);
   fChain->SetBranchAddress("rec_prot_right.t", &rec_prot_right_t, &b_rec_prot_right_t);
   fChain->SetBranchAddress("rec_prot_right.tx", &rec_prot_right_tx, &b_rec_prot_right_tx);
   fChain->SetBranchAddress("rec_prot_right.ty", &rec_prot_right_ty, &b_rec_prot_right_ty);
   fChain->SetBranchAddress("rec_prot_right.xi", &rec_prot_right_xi, &b_rec_prot_right_xi);
   fChain->SetBranchAddress("rec_prot_right.x0", &rec_prot_right_x0, &b_rec_prot_right_x0);
   fChain->SetBranchAddress("rec_prot_right.y0", &rec_prot_right_y0, &b_rec_prot_right_y0);
   fChain->SetBranchAddress("rec_prot_right.chi2", &rec_prot_right_chi2, &b_rec_prot_right_chi2);
   fChain->SetBranchAddress("rec_prot_right.chindf", &rec_prot_right_chindf, &b_rec_prot_right_chindf);
   fChain->SetBranchAddress("rec_prot_pair.valid", &rec_prot_pair_valid, &b_rec_prot_pair_valid);
   fChain->SetBranchAddress("rec_prot_pair.thxr", &rec_prot_pair_thxr, &b_rec_prot_pair_thxr);
   fChain->SetBranchAddress("rec_prot_pair.thyr", &rec_prot_pair_thyr, &b_rec_prot_pair_thyr);
   fChain->SetBranchAddress("rec_prot_pair.xir", &rec_prot_pair_xir, &b_rec_prot_pair_xir);
   fChain->SetBranchAddress("rec_prot_pair.phir", &rec_prot_pair_phir, &b_rec_prot_pair_phir);
   fChain->SetBranchAddress("rec_prot_pair.thxl", &rec_prot_pair_thxl, &b_rec_prot_pair_thxl);
   fChain->SetBranchAddress("rec_prot_pair.thyl", &rec_prot_pair_thyl, &b_rec_prot_pair_thyl);
   fChain->SetBranchAddress("rec_prot_pair.xil", &rec_prot_pair_xil, &b_rec_prot_pair_xil);
   fChain->SetBranchAddress("rec_prot_pair.phil", &rec_prot_pair_phil, &b_rec_prot_pair_phil);
   fChain->SetBranchAddress("rec_prot_pair.x0", &rec_prot_pair_x0, &b_rec_prot_pair_x0);
   fChain->SetBranchAddress("rec_prot_pair.y0", &rec_prot_pair_y0, &b_rec_prot_pair_y0);
   fChain->SetBranchAddress("rec_prot_pair.z0", &rec_prot_pair_z0, &b_rec_prot_pair_z0);
   fChain->SetBranchAddress("rec_prot_pair.chi2", &rec_prot_pair_chi2, &b_rec_prot_pair_chi2);
   fChain->SetBranchAddress("rec_prot_pair.chindf", &rec_prot_pair_chindf, &b_rec_prot_pair_chindf);
   fChain->SetBranchAddress("rec_prot_pair.tr", &rec_prot_pair_tr, &b_rec_prot_pair_tr);
   fChain->SetBranchAddress("rec_prot_pair.txr", &rec_prot_pair_txr, &b_rec_prot_pair_txr);
   fChain->SetBranchAddress("rec_prot_pair.tyr", &rec_prot_pair_tyr, &b_rec_prot_pair_tyr);
   fChain->SetBranchAddress("rec_prot_pair.tl", &rec_prot_pair_tl, &b_rec_prot_pair_tl);
   fChain->SetBranchAddress("rec_prot_pair.txl", &rec_prot_pair_txl, &b_rec_prot_pair_txl);
   fChain->SetBranchAddress("rec_prot_pair.tyl", &rec_prot_pair_tyl, &b_rec_prot_pair_tyl);
   fChain->SetBranchAddress("rec_prot_pair.t", &rec_prot_pair_t, &b_rec_prot_pair_t);
   Notify();
}

Bool_t TotemNtuple_CTPPS_2016::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TotemNtuple_CTPPS_2016::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TotemNtuple_CTPPS_2016::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TotemNtuple_CTPPS_2016_cxx
