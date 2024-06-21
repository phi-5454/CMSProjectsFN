
mytest(){

sleep 5
jobs=`condor_q | grep fnemes | grep running | sed "s/.*idle, //g" | sed "s/ running.*//g"`
jobs2=`condor_q | grep fnemes | grep running | sed "s/.*removed, //g" | sed "s/ idle.*//g"`

while [ "$jobs" -gt "5" ] || [ "$jobs2" != "0" ]
do
	echo waiting
	sleep 20
	jobs=`condor_q | grep fnemes | grep running | sed "s/.*idle, //g" | sed "s/ running.*//g"`
	jobs2=`condor_q | grep fnemes | grep running | sed "s/.*removed, //g" | sed "s/ idle.*//g"`
done

}


condor_submit xaa.sub
mytest
condor_submit xab.sub
mytest
condor_submit xac.sub
mytest
condor_submit xad.sub
mytest
condor_submit xae.sub
mytest
condor_submit xaf.sub
mytest
condor_submit xag.sub
mytest
condor_submit xah.sub
mytest
condor_submit xai.sub
mytest
condor_submit xaj.sub
mytest
condor_submit xak.sub
mytest
condor_submit xal.sub
mytest
condor_submit xam.sub
mytest
condor_submit xan.sub
mytest
condor_submit xao.sub
mytest
condor_submit xap.sub
mytest
condor_submit xaq.sub
mytest
condor_submit xar.sub
mytest
condor_submit xas.sub
mytest
condor_submit xat.sub
mytest

