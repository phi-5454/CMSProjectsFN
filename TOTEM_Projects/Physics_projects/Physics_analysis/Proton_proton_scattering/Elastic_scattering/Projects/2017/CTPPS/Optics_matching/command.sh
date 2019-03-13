for i in `find | grep can".*"tx` ; do cat $i ; done | grep "elastic cand" | grep -v "default" | grep -v "data" | awk '{ print $5 $8 $11 $14 $17 $20 $23 $26}' | sed 's/,/ /g' > input.txt
