for i in `ls  -c1 *.prj` ; do diff $i Temp/$i ; done

