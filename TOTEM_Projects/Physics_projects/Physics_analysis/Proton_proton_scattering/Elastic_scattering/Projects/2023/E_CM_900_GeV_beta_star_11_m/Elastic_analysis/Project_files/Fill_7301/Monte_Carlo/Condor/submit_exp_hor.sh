#cd MC_exp_hor
#condor_submit condor.sub
#cd ..

#sleep 25m
cd MC_with_beam_divergence_exp_hor
condor_submit condor.sub
cd ..

sleep 25m
cd MC_with_beam_divergence_stronger_cuts_exp_hor
condor_submit condor.sub
cd ..

