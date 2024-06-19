split -l 60 --additional-suffix=.sss condor.sub
for i in `ls *.sss -c1 | sed  "s/\.sss//g"` ; do cat ../condor_header.sub > $i.sub ; cat $i.sss >> $i.sub ; done ; rm *.sss
