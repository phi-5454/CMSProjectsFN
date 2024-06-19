rm alternative_submit_2.sh
for i in `seq -w 0 399`
do
	if [ -f /eos/cms/store/user/fnemes/Analysis/pp/E_CM_900_GeV_beta_star_100_m/Monte_Carlo/Condor/MC_reference/dir_0$i/Generic.root ]
	then
		:
	else
		echo "cd dir_0$i" >> alternative_submit_2.sh
		echo "./cfg_analyzer.sh &> /dev/null "  >> alternative_submit_2.sh
		echo "cd .." >> alternative_submit_2.sh
		echo >> alternative_submit_2.sh
	fi
done
chmod +x alternative_submit_2.sh
