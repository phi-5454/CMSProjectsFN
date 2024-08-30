# Glueball_analysis

## Installation instructions

To install the package, execute the following in your work area.
If compilation fails for some reason, repeat the scram b...

```
#Setup CMSSW (done once)
cmsrel CMSSW_10_1_7
cd CMSSW_10_1_7/src
cmsenv

#Clone this package
cd $CMSSW_BASE/src
git clone https://github.com/FrigyesNemes/Projects.git
cd Projects/Glueball_analysis/

scram b -j 8
```

## Running the analysis

Analysis are performed in two steps
   1. process data/MC files from AOD format using updated [dEdX reconstruction](https://twiki.cern.ch/twiki/pub/CMS/EnergyLossPID/UserCode_EnergyLossPID.tgz 
)
   2. Ntuplize output files - at this stage trackID is computed together with some high-level variables.

# WARNING: the following commands will not work as they are. The base commands are the same, but the arguments are different. See the run scripts on what arguments are supplied.

### Processing the data

To process a single data file (stored in `/eos/totem/data/cmstotem/2018/90m/RECO_copy/`) run the following command:
```
cmsRun $CMSSW_BASE/src/Projects/Glueball_analysis/python/ConfFile_cfg.py inputFiles=root://eostotem/$FILENAME applyFilt=True
```
You can set `applyFilt=False` of no event pre-selection should be applied (usually used to debug). The _FILENAME_ argument can be any file from the EOS folder. For example:

```
FILENAME=/eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM20/110000/000E49AC-AB3D-E911-9749-0242AC130002.root
```

### Running ntuplizer

At this step, track identification is executed. The trackID is based on dEdX of the reconstructed track [[ref](https://indico.cern.ch/event/1154003/#4-ntuple-production-for-glueba)].

To execute this step, run the following command:
```
analysisWrapper --in output.root --out tracks.root --method GlueballAnalysis
```
Where `output.root` is the output file from the previous step. 

## Producing MC samples

The scripts in [scripts](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/scripts) are used to generate AOD files for MinimumBias events using Pythia and CEP using DIME.
The production is done in two steps
   1. Simulation of the hard process (SIM step): see relevant sub-sections on how to generate events
   2. Reconstruction (SIM-AOD): a common code is used to simulate detector response and run CMS reconstruction algorithms

### MinimumBias

The Pythia fragment of MinimumBias events can be found in [data](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/data) folder. 

To generate GEN samples, the following code can be executed using the `cmsDriver.py` command:

```
#copy the fragment to Configuration/GenProduction/python/MinBias_fragment.py
#produce SIM (50 events, using a random seed = 12345)
cmsDriver.py Configuration/GenProduction/python/MinBias_fragment.py --python_filename step1_cfg.py \
--eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM \
--fileout file:stepSIM.root --conditions 101X_upgrade2018_realistic_v7 --beamspot Realistic25ns13TeVEarly2017Collision \
--customise_commands process.RandomNumberGeneratorService.generator.initialSeed="cms.untracked.uint32(12345)" \
--step GEN,SIM --geometry DB:Extended --era Run2_2018_highBetaStar --runUnscheduled --mc -n 50
``` 

### CEP using DIME

Use the [Dime Monte Carlo](https://dimemc.hepforge.org/) event generator to produce LHE output:
```
wget https://dimemc.hepforge.org/dimemcv1.07.f
#edit the file with correct iseed=SEED, nev=NUMBER OF EVENTS, pflag=PROCESS, compile the code and produce LHE file
gfortran dimemcv1.07.f -o dime.exe
./dime.exe > code.stdout
#Fix the output (the current output not follows standard LHE format as in arxiv:0609017)
python scripts/FixDIME.py exrec.dat code.stdout
```

Before producing SIM output, copy Pythia fragment for exclusive production processes, which is stored in [data](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/data) folder. Then use the following code to produce SIM from the LHE file:

```
cmsDriver.py step1 --filein file:output.lhe  --fileout file:stepLHE.root --mc --eventcontent LHE --datatier LHE --conditions 101X_upgrade2018_realistic_v7 --step NONE -n -1
```

```
cmsDriver.py Configuration/GenProduction/python/CEP-fragment.py --filein file:stepLHE.root --fileout file:stepSIM.root \
--mc --eventcontent RAWSIM --datatier GEN-SIM --conditions 101X_upgrade2018_realistic_v7 \
--beamspot Realistic25ns13TeVEarly2017Collision --step GEN,SIM --nThreads 8 --geometry DB:Extended \
--era Run2_2018_highBetaStar --python_filename step1_cfg.py --customise Configuration/DataProcessing/Utils.addMonitoring -n -1
```


### SIM -> AOD step

The following code can be executed using the `cmsDriver.py` command to generate AOD output from the `stepSIM.root` file:

```
cmsDriver.py step1 --runUnscheduled --conditions 101X_upgrade2018_realistic_v7 \
--era Run2_2018_highBetaStar --eventcontent RECOSIM -s DIGI,L1,DIGI2RAW,RAW2DIGI,L1Reco,RECO \
--datatier GEN-SIM-DIGI-RECO --filein file:stepSIM.root  --fileout file:stepAOD.root --mc \
--python_filename step2_cfg.py --customise Configuration/DataProcessing/Utils.addMonitoring --geometry DB:Extended
``` 

