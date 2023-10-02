./plot | grep most | sed "s/most://g" | awk '{ print $3 }' | sort -n  | uniq -c | sort -nr
