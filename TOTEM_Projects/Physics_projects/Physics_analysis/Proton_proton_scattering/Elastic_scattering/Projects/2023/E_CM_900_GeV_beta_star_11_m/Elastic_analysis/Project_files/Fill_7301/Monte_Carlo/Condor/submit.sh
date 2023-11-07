#cd MC_reference/
#condor_submit condor.sub
#cd ..

#sleep 20m
cd MC_reference_with_beam_divergence
condor_submit condor.sub
cd ..

sleep 20m
cd MC
condor_submit condor.sub
cd ..

sleep 20m
cd MC_with_beam_divergence
condor_submit condor.sub
cd ..

sleep 20m
cd MC_with_beam_divergence_stronger_cuts
condor_submit condor.sub
cd ..

