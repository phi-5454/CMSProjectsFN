for i in `seq 0 1000` 
do
	./plot $i &> test.txt
	grep ParameterError test.txt
done
