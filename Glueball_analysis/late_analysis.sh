#analysisWrapper --in output.root --out tracks.root --method GlueballAnalysis


outdir="/eos/user/y/yelberke/TOTEM_2018_ADDEDVARS_OUT"
ntuplize_parallel() {
	#totem_files=$(find intermediate -type f -name "TOTEM*" -exec basename {} \;)
	#echo "$totem_files"
	find intermediate -type f -name "TOTEM*" -exec basename {} \; | parallel -j 8 analysisWrapper --in intermediate/{} --out "$outdir"/{} --method GlueballAnalysis
}


ntuplize_parallel
