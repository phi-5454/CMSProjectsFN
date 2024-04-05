for i in `ls -c1 *.prj`
do
	head -n 20 $i > Ordered/$i

	#echo >> Ordered/$i
	grep "begin.*theta_x_star_left_rad_theta_x_star_right_rad_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
	grep "begin.*theta_y_star_left_rad_theta_y_star_right_rad_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
	grep "begin.*x_mm_near_dx_mm_left_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
	grep "begin.*x_mm_near_dx_mm_right_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
	grep "begin.*y_mm_near_dy_mm_left_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
	grep "begin.*y_mm_near_dy_mm_right_cut_block" $i -A2 >> Ordered/$i ; echo >> Ordered/$i
done
