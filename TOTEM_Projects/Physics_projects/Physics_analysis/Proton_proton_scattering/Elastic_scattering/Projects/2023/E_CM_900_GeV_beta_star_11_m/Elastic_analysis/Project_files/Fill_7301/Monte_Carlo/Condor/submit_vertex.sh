cd MC_exp_with_vertex
condor_submit condor.sub
cd ..

sleep 5h
cd MC_exp_hor_with_vertex
condor_submit condor.sub
cd ..

