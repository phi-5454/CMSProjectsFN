#!/bin/bash

create_dirs() {

	local_target_dir=$1
	templatefile=$2

	home_dir="/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_11_m/Elastic_analysis/Project_files/Fill_7301/Monte_Carlo/Condor/"
	eos_dir="/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_11_m/Monte_Carlo/Condor/"
	global_target_dir=$home_dir/$local_target_dir
	global_eos_target_dir=$eos_dir/$local_target_dir
	global_templatefile=$home_dir/$templatefile
	number_of_particles=3200000
	#echo $global_templatefile
	prefix=00

	cat $home_dir/Templates/condor.sub | sed "s#XXXX#$1#g"  > $global_target_dir/condor.sub

	for i in `seq -w 0 99 `
	do
		mydir=$global_target_dir/"dir_"$prefix$i
		myeosdir=$global_eos_target_dir/"dir_"$prefix$i
	#	echo $mydir
	#	echo $myeosdir
		mkdir $mydir
		mkdir $myeosdir
		cat $home_dir/Templates/cfg_analyzer.sh | sed "s#YYYY#$mydir#g" | sed "s#XXXX#$myeosdir#g"  > $mydir/cfg_analyzer.sh
		cat $global_templatefile | sed "s/XXXX/$i/g" | sed "s/ZZZZ/$number_of_particles/g"  > $mydir/cfg.prj
		chmod +x $mydir/cfg_analyzer.sh
	done
}

#create_dirs MC_reference                              Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference.prj
#create_dirs MC_reference_with_beam_divergence         Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference_with_beam_divergence.prj
#create_dirs MC                                        Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor.prj
#create_dirs MC_with_beam_divergence                   Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence.prj
#create_dirs MC_with_beam_divergence_stronger_cuts     Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence_stronger_cuts_for_test.prj

#create_dirs MC_reference_exp                              Templates/Exponential_versions/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference.prj
#create_dirs MC_reference_with_beam_divergence_exp         Templates/Exponential_versions/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference_with_beam_divergence.prj
create_dirs MC_exp                                        Templates/Exponential_versions/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor.prj
#create_dirs MC_with_beam_divergence_exp                   Templates/Exponential_versions/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence.prj
#create_dirs MC_with_beam_divergence_stronger_cuts_exp     Templates/Exponential_versions/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence_stronger_cuts_for_test.prj



#create_dirs MC_exp_hor                                        Templates/Exponential_versions/With_horizontal_cuts_and_corrections/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor.prj
#create_dirs MC_with_beam_divergence_exp_hor                   Templates/Exponential_versions/With_horizontal_cuts_and_corrections/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence.prj
#create_dirs MC_with_beam_divergence_stronger_cuts_exp_hor     Templates/Exponential_versions/With_horizontal_cuts_and_corrections/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_with_beam_divergence_stronger_cuts_for_test.prj


