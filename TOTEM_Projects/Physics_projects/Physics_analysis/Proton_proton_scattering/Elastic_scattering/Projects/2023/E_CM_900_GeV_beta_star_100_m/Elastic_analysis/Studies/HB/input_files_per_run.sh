for i in `cat ../../../../E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do grep $i ../../Input_files/Reduced_ntuples_left_top_right_bottom.prj > ../../Input_files/Reduced_ntuples_left_top_right_bottom_$i.prj ; echo ; done
for i in `cat ../../../../E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt` ; do grep $i ../../Input_files/Reduced_ntuples_left_bottom_right_top.prj > ../../Input_files/Reduced_ntuples_left_bottom_right_top_$i.prj ; echo ; done

