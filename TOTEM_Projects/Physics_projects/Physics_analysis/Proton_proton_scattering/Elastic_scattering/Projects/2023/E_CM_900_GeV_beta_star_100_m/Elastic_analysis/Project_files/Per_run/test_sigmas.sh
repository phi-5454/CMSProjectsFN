echo
echo "reference" $1
grep begin.*$1 /afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Cuts/Fill_7291/Cuts_test_left_bottom_right_top_4_sigma.prj  -A11 | grep " sigma" | awk '{ print $3 }'
echo
for i in `cat ../../../General_settings/List_of_runs.txt ` ; do echo -n $i"  " ; grep begin.*$1 Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_$i.prj -A3 | grep " sigma" | awk '{ print $3 }' ; done
