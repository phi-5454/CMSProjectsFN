./plot2 1 100000 &> /dev/null &
./plot2 2 100000 &> /dev/null &
wait
hadd hist1.root results/hist1_*
hadd hist2.root results/hist2_*
hadd hist3.root results/hist3_*


