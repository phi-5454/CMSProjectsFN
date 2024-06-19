#!/bin/bash

create_dirs() {

	local_target_dir=$1
	templatefile=$2

	home_dir="/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Studies/Monte_Carlo/Condor/"
	eos_dir="/eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_100_m/Monte_Carlo/Condor/"
	global_target_dir=$home_dir/$local_target_dir
	global_eos_target_dir=$eos_dir/$local_target_dir
	global_templatefile=$home_dir/$templatefile
	number_of_particles=4000000.0
	#echo $global_templatefile
	prefix=00

	cat $home_dir/Templates/condor.sub | sed "s#XXXX#$1#g"  > $global_target_dir/condor.sub

	for i in `seq -w 0 39`
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

#create_dirs MC_reference                              Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_324536_reference.prj
create_dirs MC_ver                              Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_324536_ver.prj

#create_dirs MC_reference_LTRB                    Templates/Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_run_324536_reference.prj
#create_dirs MC_ver_LTRB                           Templates/Left_top_right_bottom_4_sigma_all_root_files_to_define_cuts_run_324536_ver.prj

