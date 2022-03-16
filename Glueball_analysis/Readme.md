# Glueball_analysis

## Installation instructions

To install execute the following in your work area.
If compilation fails for some reason repeat the scram b...

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

...

## Producing MC samples

The scripts in [scripts](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/scripts) are used to generate AOD files for MinimumBias events using pythia and CEP using DIME.
The production is done in two steps
   1. Simulation of the hard process (SIM step): see relevant sub-sections on how to generate events
   2. Reconstruction (SIM-AOD): a common code is used to simulate detector response and run CMS reconstruction algorithms

### MinimumBias

The pythia fragment of MinimumBias events can be found in [data](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/data) folder. 

To generate GEN samples the following code can be executed using the `cmsDriver.py` command:

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

Before producing SIM output, copy pythoa fragment for exclusive production processes, which is stored in [data](https://github.com/FrigyesNemes/Projects/tree/master/Glueball_analysis/data) folder. Then use the following code to produce SIM form LHE file:

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

To generate AOD output from `stepSIM.root` file the following code can be executed using the `cmsDriver.py` command:

```
cmsDriver.py step1 --runUnscheduled --conditions 101X_upgrade2018_realistic_v7 \
--era Run2_2018_highBetaStar --eventcontent RECOSIM -s DIGI,L1,DIGI2RAW,RAW2DIGI,L1Reco,RECO \
--datatier GEN-SIM-DIGI-RECO --filein file:stepSIM.root  --fileout file:stepAOD.root --mc \
--python_filename step2_cfg.py --customise Configuration/DataProcessing/Utils.addMonitoring --geometry DB:Extended
``` 

