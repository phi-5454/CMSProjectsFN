#export FILENAME=/eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM20/110000/000E49AC-AB3D-E911-9749-0242AC130002.root
#cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFiles=srcfiles applyFilt=True

analyze_parallel() {
    # Use find command with -name option to find files matching the regex
    # Use parallel to run the program in parallel with each file as input
    find . -type f -name "TOTEM*" | parallel -j 8 --ungroup  cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFileList={}  applyFilt=True smallData=True outDir="intermediate_minimal/"
}


# cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFiles=root://eostotem/$FILENAME applyFilt=True

analyze_parallel
