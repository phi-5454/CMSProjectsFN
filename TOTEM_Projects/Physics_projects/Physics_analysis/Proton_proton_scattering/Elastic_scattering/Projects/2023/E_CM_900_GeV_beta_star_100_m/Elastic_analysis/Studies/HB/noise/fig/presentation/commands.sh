cat status_header.tex > status_2.tex ; for i in `cat plots.txt ` ; do cat status_template.tex | sed "s/PLOTNAME/$i/g"  ; done >>  status_2.tex ; cat status_end.tex >> status_2.tex
cat status_header.tex > status_3.tex ; for i in `cat plots.txt ` ; do cat status_template.tex | sed "s/PLOTNAME/$i\_rotated/g"  ; done >>  status_3.tex ; cat status_end.tex >> status_3.tex
