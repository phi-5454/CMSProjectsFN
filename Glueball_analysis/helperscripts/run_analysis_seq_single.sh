#!/bin/bash

#export FILENAME=/eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM20/110000/000E49AC-AB3D-E911-9749-0242AC130002.root
#cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFiles=srcfiles applyFilt=True

# Working directory
wdir=""
src_dir="root://eostotem//eos/totem/data/cmstotem/2018/90m/RECO_copy/" 
analysis_workspace=""
analysis_name=""

in_filelist=""

inter_dir="intermediate_ntuples"
final_dir="ntuplized"

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
	echo "Usage: [--build] [--analyze] [--ntuplize] [--n-analyze <number of files to analyze from each folder>] [--n-jobs <number of jobs>] [--in-filelist <file that contains input files>] [--inter-dir <(sub)directory>] [--analysis-name <identifier for the given analysis> --analysis-name <workspace for the analysis, make sure it's empty>] --wdir <the project's base directory>"
	exit 1
}

analyze() {
	cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFileList="$in_filelist" nFiles=$n_analyze applyFilt=True srcDir="$src_dir" outDir="$analysis_workspace/$analysis_name/$inter_dir/"
}

ntuplize() {
	analysisWrapper --in "$analysis_workspace/$analysis_name/$inter_dir/$in_filelist" --out "$analysis_workspace/$analysis_name/$final_dir/$in_filelist" --method GlueballAnalysis
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
		--analysis-workspace) 
			if [[ -n "$2" && ! "$2" =~ ^-- ]]; then
				echo AAAA
				analysis_workspace="$2"
				shift
			else
				echo "Error: --analysis-workspace requires a non-empty option argument."
				usage
			fi
			;;
		--wdir) 
			if [[ -n "$2" && ! "$2" =~ ^-- ]]; then
				echo AAAA
				wdir="$2"
				shift
			else
				echo "Error: --wdir requires a non-empty option argument."
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

cd $wdir
cmsenv

if $build; then
	build
fi

if $analyze; then
	mkdir -p $analysis_workspace/$analysis_name/$inter_dir/$(dirname "$in_filelist")

	analyze
fi

echo "CONTINUING"
if $ntuplize; then
	mkdir -p $analysis_workspace/$analysis_name/$final_dir/$(dirname "$in_filelist")
	ntuplize
fi
