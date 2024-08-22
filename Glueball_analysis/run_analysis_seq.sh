#!/bin/bash

#export FILENAME=/eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM20/110000/000E49AC-AB3D-E911-9749-0242AC130002.root
#cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFiles=srcfiles applyFilt=True

cd /afs/cern.ch/user/y/yelberke/scratch1/CMSSW_14_0_9/src/Projects/Glueball_analysis
cmsenv
echo "AAAA"

outdir="/eos/user/y/yelberke/TOTEM_2018_ADDEDVARS_OUT"
analysis_name=""

in_filelist=""

inter_dir="/eos/user/y/yelberke/intermediate"

build=false
analyze=false
ntuplize=false

njobs=8
n_analyze=-1

# Function to check if a value is an integer
is_integer() {
	[[ "$1" =~ ^-?[0-9]+$ ]]
}


# Function to print usage
usage() {
	echo "Usage: [--build] [--analyze] [--ntuplize] [--n-analyze <number of files to analyze from each folder>] [--n-jobs <number of jobs>] [--in-filelist <file that contains input files>] [--inter-dir <(sub)directory>] [--analysis-name <identifier for the given analysis>]"
	exit 1
}

analyze() {
	echo "LOOK: $inter_dir/$analysis_name/"
	# Use find command with -name option to find files matching the regex
	# Use parallel to run the program in parallel with each file as input
	cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFileList="$in_filelist" nFiles=$n_analyze applyFilt=True outDir="$inter_dir/$analysis_name/"
	#cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFileList=$in_filelist nFiles=$n_analyze applyFilt=True outDir="./"
}

ntuplize() {
	#totem_files=$(find intermediate -type f -name "TOTEM*" -exec basename {} \;)
	#echo "$totem_files"
	find intermediate -type f -name "TOTEM*.root" -exec basename {} \; | parallel -j 8  --ungroup analysisWrapper --in "$inter_dir/$analysis_name/"{} --out "$outdir"/"$analysis_name"/{} --method GlueballAnalysis
}

build(){
	scram b -j 10
}

# Empty argument list, program won't do anything
if [ "$#" -eq 0 ]; then
	usage
fi

# Parse named arguments
while [[ "$#" -gt 0 ]]; do
	case $1 in
		--build) build=true ;;
		--analyze) analyze=true ;;
		--ntuplize) ntuplize=true ;;
		--analysis-name) 
			if [[ -n "$2" && ! "$2" =~ ^-- ]]; then
				analysis_name="$2"
				shift
			else
				echo "Error: --analysis-name requires a non-empty option argument."
				usage
			fi
			;;
		--in-filelist) 
			if [[ -n "$2" && ! "$2" =~ ^-- ]]; then
				in_filelist="$2"
				shift
			else
				echo "Error: --in-filelist requires a non-empty option argument."
				usage
			fi
			;;
		--inter-sub) 
			if [[ -n "$2" && ! "$2" =~ ^-- ]]; then
				inter_sub=$2
				shift
			else
				echo "Error: --inter_sub requires a non-empty option argument."
				usage
			fi
			;;
		--n-analyze)
			if [[ -n "$2"  ]] && $(is_integer $2); then
				n_analyze=$2
				shift
			else
				echo "Error: --n-analyze requires an integer argument."
				usage
			fi
			;;
		--n-jobs)
			if [[ -n "$2" ]] && $(is_integer "$2") ; then
				njobs=$2
				shift
			else
				echo "Error: --n-jobs requires an integer argument."
				usage
			fi
			;;
		*)
			echo "Unknown option: $1"
			usage
			;;
	esac
	shift
done

#Check if name has been provided for our analysis
if [[ -z "$analysis_name" ]]; then
	echo "Error: --analysis-name is required."
	usage
fi


echo "STARTING"

if $build; then
	build
fi

if $analyze; then
	mkdir -p $inter_dir/$analysis_name/$(dirname "$in_filelist")
	analyze
fi

echo "CONTINUING"
if $ntuplize; then
	mkdir -p $outdir/$analysis_name
	ntuplize
fi
