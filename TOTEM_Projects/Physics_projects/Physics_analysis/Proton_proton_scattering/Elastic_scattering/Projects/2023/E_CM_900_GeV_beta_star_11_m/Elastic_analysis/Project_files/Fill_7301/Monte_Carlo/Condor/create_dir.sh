home_dir="/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_11_m/Elastic_analysis/Project_files/Fill_7301/Monte_Carlo/Condor/"

local_target_dir=MC_reference
templatefile="Templates/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_aligned_first_optics_test_hor_cor_reference.prj"

global_target_dir=$home_dir/$local_target_dir

prefix=000

for i in `seq -w 0 4 `
do
	mydir=$global_target_dir/dir_$prefix$i
#	echo $mydir
	mkdir $mydir
#	cat Templates/cfg_analyzer.sh | sed "s/dir_YYYY/$mydir/g" > $mydir/cfg_analyzer.sh
#	cat $templatefile | sed "s/XXXX/$i/g"  > $mydir/cfg.prj
#	chmod +x $mydir/cfg_analyzer.sh
done
