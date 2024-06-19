ls Condor/MC_reference/dir_0*/Generic.root  -c1 > tmp/files.txt
lines=`wc tmp/files.txt | awk '{ print $1 }'`
echo $lines
z=$(((lines / 10) + 1))

cd tmp
split -l $z --additional-suffix=.aux files.txt
cd ..

for i in `cat tmp/xaa.aux` ; do echo -n $i" " ; done > tmp/lines0.txt ; lines0var=`cat tmp/lines0.txt` 
for i in `cat tmp/xab.aux` ; do echo -n $i" " ; done > tmp/lines1.txt ; lines1var=`cat tmp/lines0.txt` 
for i in `cat tmp/xac.aux` ; do echo -n $i" " ; done > tmp/lines2.txt ; lines2var=`cat tmp/lines0.txt` 
for i in `cat tmp/xad.aux` ; do echo -n $i" " ; done > tmp/lines3.txt ; lines3var=`cat tmp/lines0.txt` 
for i in `cat tmp/xae.aux` ; do echo -n $i" " ; done > tmp/lines4.txt ; lines4var=`cat tmp/lines0.txt` 
for i in `cat tmp/xaf.aux` ; do echo -n $i" " ; done > tmp/lines5.txt ; lines5var=`cat tmp/lines0.txt` 
for i in `cat tmp/xag.aux` ; do echo -n $i" " ; done > tmp/lines6.txt ; lines6var=`cat tmp/lines0.txt` 
for i in `cat tmp/xah.aux` ; do echo -n $i" " ; done > tmp/lines7.txt ; lines7var=`cat tmp/lines0.txt` 
for i in `cat tmp/xai.aux` ; do echo -n $i" " ; done > tmp/lines8.txt ; lines8var=`cat tmp/lines0.txt` 
for i in `cat tmp/xaj.aux` ; do echo -n $i" " ; done > tmp/lines9.txt ; lines9var=`cat tmp/lines0.txt` 

hadd -f file0.root $lines0var &> /dev/null &
hadd -f file1.root $lines1var &> /dev/null &
hadd -f file2.root $lines2var &> /dev/null &
hadd -f file3.root $lines3var &> /dev/null &
hadd -f file4.root $lines4var &> /dev/null &
hadd -f file5.root $lines5var &> /dev/null &
hadd -f file6.root $lines6var &> /dev/null &
hadd -f file7.root $lines7var &> /dev/null &
hadd -f file8.root $lines8var &> /dev/null &
hadd -f file9.root $lines9var &> /dev/null &

wait

hadd -f final.root file*.root 

rm file*.root
