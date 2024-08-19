import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('python')
options.register('lumiJson', None,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 'apply this lumi json file'
                 )
options.register('applyFilt', True,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.bool,
                 'Apply filters'
                 )
                 # Two added options
options.register('nFiles', -1,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 'How many files to apply analysis to? Set -1 for all. (for debug/fast results)'
                 )
options.register('inputFileList', "",
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 'List of files to run program on'
                 )
options.register('outDir', None,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 'What directory to output the intermediate files to'
                 )
options.parseArguments()

from Configuration.StandardSequences.Eras import eras
process = cms.Process("Analysis",eras.Run2_2018_highBetaStar)

# import of standard configurations
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#message logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = cms.untracked.string('ERROR')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)


#files = [];
files = ["root://eostotem//eos/totem/data/cmstotem/2018/90m/RECO_copy/" + options.inputFileList.rstrip()];
'''
with open(options.inputFileList, encoding='us-ascii', errors='ignore') as file:
    for line in file:
        files.append("root://eostotem/" + line.rstrip())
        print(files[-1])

# select, Filter out empty values
if(options.nFiles != -1):
    files = files[:options.nFiles]
'''

#files = ["root://eostotem//eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM43/110000/004228F1-344D-E911-919B-F01FAFD35CA4.root"];

'''
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options.inputFiles),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 
                            )
'''

print(files[0])

if(options.nFiles != -1):
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring(*(files)),
                                duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 
                                )
else:
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring(*(files)),
                                duplicateCheckMode = cms.untracked.string('noDuplicateCheck') 
                                )

#configure for data/MC based on lumi json input (if passed in command line)
from Configuration.AlCa.GlobalTag import GlobalTag
if options.lumiJson:
  print('Running on data file:')
  process.GlobalTag.globaltag = "101X_dataRun2_Prompt_v11"
  print('Lumi sections will be selected with',options.lumiJson)
  from FWCore.PythonUtilities.LumiList import LumiList
  myLumis = LumiList(filename = options.lumiJson).getCMSSWString().split(',')
  process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
  process.source.lumisToProcess.extend(myLumis)
else: process.GlobalTag.globaltag = "101X_upgrade2018_realistic_v7"

outname = os.path.splitext(options.inputFileList)[0].lstrip("./")

outdir = options.outDir

print(outdir + outname + '.root')

#process.TFileService = cms.Service("TFileService",
                    #fileName = cms.string(outname + '.root'))

process.TFileService = cms.Service("TFileService",
                            fileName = cms.string(outdir + outname + '.root'))

					
process.analysis = cms.EDAnalyzer('RecoAnalyzer',
       tracks = cms.untracked.InputTag('generalTracks'),
       #tracks = cms.untracked.InputTag('refitterForEnergyLoss'),
       #DeDxData = cms.untracked.InputTag('energyLossProducer','energyLossAllHits'),
#       vertices = cms.InputTag('offlinePrimaryVertices'),	 
       vertices = cms.InputTag('offlinePrimaryVerticesWithBS'),	 
       rpTrackTag = cms.InputTag('ctppsLocalTrackLiteProducer'),
       genParticles = cms.InputTag('genParticles'),
       applyFilt = cms.bool(options.applyFilt),
)

process.anal = cms.Path(process.analysis)

'''
#dEdX
process.load("UserCode.EnergyLossPID.EnergyLossProducer_cff")
process.energyLossProducer.tag = cms.string('totem')
process.reco = cms.Path(process.MeasurementTrackerEvent
                      * process.siPixelClusterShapeCache)
                      '''


###############################################################################
# Schedule
'''
process.schedule = cms.Schedule(process.reco,
                                process.produceEnergyLoss,
								                process.anal)
                                '''
process.schedule = cms.Schedule(process.anal)
							
								
