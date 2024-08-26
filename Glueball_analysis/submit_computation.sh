#!/bin/bash

# Regenerate source file lists
./helperscripts/make_src_lists_abridged.sh

# Rebuild the project, in case this is forgotten
cmsenv
scram b -j 10


# FIRST ARGUMENT: name (a sort of ID) of the analysis
# SECOND ARGUMENT: Directory of the workspace

wdir="$(pwd)"

# Submit the jobs
for fnum in 20 21 22 23 40 41 42 43; do
       condor_submit -append "filelist=TOTEM$fnum.txt" -append "analysisname=$1" -append "analysisworkspace=$2" -append "workingdir=$wdir" addedvars.sub
done


#condor_submit -append "fileList=TestFileNames.txt" -append "analysisname=$1" -append "analysisworkspace=$2" -append "workingdir=$wdir" addedvars.sub
       

