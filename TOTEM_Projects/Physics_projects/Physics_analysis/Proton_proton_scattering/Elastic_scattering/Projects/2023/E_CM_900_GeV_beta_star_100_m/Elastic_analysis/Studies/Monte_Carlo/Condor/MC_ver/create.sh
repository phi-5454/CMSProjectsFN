rm alternative_submit.sh
for i in `seq -w 0 399`
do
	echo "cd dir_0$i" >> alternative_submit.sh
	echo "./cfg_analyzer.sh &> /dev/null "  >> alternative_submit.sh
	echo "cd .." >> alternative_submit.sh
	echo >> alternative_submit.sh
done
chmod +x alternative_submit.sh
