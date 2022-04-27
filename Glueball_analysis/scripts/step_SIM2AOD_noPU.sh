#!/bin/bash
#instructions from  https://twiki.cern.ch/twiki/bin/view/CMS/PdmVLegacy2017Analysis#Example_of_cmsDriver_AN1
#gen_aod_noPU.sh stepSIM.root

startMsg='script started on '`date`
echo $startMsg
echo ./gen_aod_noPU.sh $1

# check number of arguments
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 INPUTFILE" >&2
  echo "Example: $0 stepSIM.root" >&2
  exit 1
fi

####### INPUT SETTINGS ###########
inFile=`readlink -f ${1}`
##################################

echo "DIGI-AOD starting"
cmsDriver.py step2 --runUnscheduled --conditions 101X_upgrade2018_realistic_v7 \
-n -1 --era Run2_2018_highBetaStar --eventcontent RECOSIM \
-s DIGI,L1,DIGI2RAW,HLT:@fake2,RAW2DIGI,L1Reco,RECO --datatier GEN-SIM-DIGI-RECO \
--filein file:$inFile  --fileout file:stepAOD.root --mc --geometry DB:Extended \
--python_filename step2_cfg.py --customise Configuration/DataProcessing/Utils.addMonitoring

echo INFO: $startMsg
echo INFO: finished on `date`



