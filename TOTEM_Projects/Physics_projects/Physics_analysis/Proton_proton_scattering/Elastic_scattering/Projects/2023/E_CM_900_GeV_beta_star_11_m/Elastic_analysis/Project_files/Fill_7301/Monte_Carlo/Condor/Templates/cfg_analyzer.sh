#!/bin/bash

cd YYYY
rm *.log
../../../../../../../../../../analyse Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference.prj 
rm *.eps
#source /cvmfs/cms.cern.ch/cmsset_default.sh
#eval `scramv1 runtime -sh`
#cmsRun cfg_analyzer.py &> log
#cp output.root /eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/output_0000.root
#rm output.root
#mv log /eos/cms/store/group/phys_diffraction/CMSTotemLowPU2018/skim/data2/TOTEM2/log/log_0000
