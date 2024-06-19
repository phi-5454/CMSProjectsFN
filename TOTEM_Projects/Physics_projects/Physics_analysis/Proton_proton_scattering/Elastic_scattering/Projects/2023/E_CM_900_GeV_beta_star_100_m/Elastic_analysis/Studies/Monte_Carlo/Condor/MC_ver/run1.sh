
mytest(){

jobs=`condor_q | grep fnemes | grep running | sed "s/.*idle, //g" | sed "s/ running.*//g"`

if [[ $jobs="0" ]]
  then
	echo "good" 
  else
	echo "not"
fi

sleep 10
}


mytest
mytest
mytest
