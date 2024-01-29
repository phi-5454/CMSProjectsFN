./plot2 1 100000 &> /dev/null &
./plot2 2 100000 &> /dev/null &
./plot2 3 100000 &> /dev/null &
./plot2 4 100000 &> /dev/null &
wait
./plot2 5 100000 &> /dev/null &
./plot2 6 100000 &> /dev/null &
./plot2 7 100000 &> /dev/null &
./plot2 8 100000 &> /dev/null &
wait
./plot2 9 100000 &> /dev/null &
./plot2 10 100000 &> /dev/null &
./plot2 11 100000 &> /dev/null &
./plot2 12 100000 &> /dev/null &
wait
./plot2 13 100000 &> /dev/null &
./plot2 14 100000 &> /dev/null &
./plot2 15 100000 &> /dev/null &
./plot2 16 100000 &> /dev/null &
wait
hadd -f hist1.root results/hist1_*
hadd -f hist2.root results/hist2_*
hadd -f hist3.root results/hist3_*


