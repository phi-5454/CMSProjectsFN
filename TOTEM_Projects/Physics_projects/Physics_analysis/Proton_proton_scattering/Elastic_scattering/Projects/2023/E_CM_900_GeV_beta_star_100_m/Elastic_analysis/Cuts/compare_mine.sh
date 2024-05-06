echo LBRT
for i in `ls  -c1 *LBRT*.prj` ; do diff $i Temp/$i ; done
echo LTRB
for i in `ls  -c1 *LTRB*.prj` ; do diff $i Temp/$i ; done
