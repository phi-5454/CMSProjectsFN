cd MC_reference_exp/
condor_submit condor.sub
cd ..

sleep 25m
cd MC_reference_with_beam_divergence_exp
condor_submit condor.sub
cd ..

sleep 25m
cd MC_exp
condor_submit condor.sub
cd ..

sleep 25m
cd MC_with_beam_divergence_exp
condor_submit condor.sub
cd ..

sleep 25m
cd MC_with_beam_divergence_stronger_cuts_exp
condor_submit condor.sub
cd ..

