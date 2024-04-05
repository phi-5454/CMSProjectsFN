grep begin Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Project_files/Per_run/Left_bottom_right_top_4_sigma_all_root_files_to_define_cuts_run_324457.prj | wc
echo "Should be 7 cuts, 4 spect, 2 coll, 1 xvert"


for i in `ls Projects/2023/E_CM_900_GeV_beta_star_100_m/Elastic_analysis/Project_files/Per_run/Left_*.prj`
do 

	valami=`grep theta_x_star_left_rad_theta_x_star_right_rad_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi

	valami=`grep theta_y_star_left_rad_theta_y_star_right_rad_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi

	valami=`grep x_mm_near_dx_mm_left_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi

	valami=`grep x_mm_near_dx_mm_right_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi

	valami=`grep y_mm_near_dy_mm_left_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi

	valami=`grep y_mm_near_dy_mm_right_cut_block     $i | wc | awk '{ print $1}'`
	if [[ "$valami"=="2" ]]; then
		:
	else
		echo "ok"
	fi



done














