find $2 -type d -name "TOTEM*" -printf '%f\n' | parallel -j 8 --ungroup hadd -f "$1/{}.root" "$2/{}/*/*"
