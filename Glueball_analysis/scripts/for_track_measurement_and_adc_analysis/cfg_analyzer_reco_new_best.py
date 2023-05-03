import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING')
  )
)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("UserCode.EnergyLossPID.EnergyLossProducer_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag.globaltag = "101X_dataRun2_Express_v8"
process.GlobalTag.globaltag = "101X_dataRun2_Prompt_v11"

process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
  inputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_dedxHarmonic2_*_*',
    'keep *_dedxPixelHarmonic2_*_*',
    'keep *_offlinePrimaryVertices_*_*',
    'keep *_particleFlow_*_*',
    'keep *_totemRPRecHitProducer_*_*',
    'keep *_totemRPUVPatternFinder_*_*',
    'keep *_ctppsLocalTrackLiteProducer_*_*',
    'keep *_siPixelClusters_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_siStripClusters_*_*',
    'keep *_siPixelDigis_*_*',
    'keep *_siStripDigis_*_*',
    'keep *_hbheprereco_*_*',
    'keep *_horeco_*_*',
    'keep *_hfreco_*_*',
    'keep *_ecalRecHit_*_*',
    'keep *_standAloneMuons_*_*',
    'keep *_dt4DSegments_*_*',
    'keep *_cscSegments_*_*',
    'keep *_rpcRecHits_*_*',
  ),

  fileNames = cms.untracked.vstring(
"root://eostotem//eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM20/110000/000E49AC-AB3D-E911-9749-0242AC130002.root"
  )
)

#process.energyLossProducer.tag = cms.string('totem')

process.analyzer = cms.EDFilter("PromptAnalyzer",
  tracks = cms.InputTag('generalTracks'),
  dedxs = cms.InputTag('dedxHarmonic2'),
  dedxPIXs = cms.InputTag('dedxPixelHarmonic2'),
  #dedxpixels = cms.InputTag('dedxHitInfo'),
  vertices = cms.InputTag('offlinePrimaryVertices'),
  #triggers = cms.InputTag('TriggerResults','','HLT'),
  pflows = cms.InputTag('particleFlow'),
#  myenergyLossPixHits = cms.InputTag('energyLossProducer'),
  myenergyLossPixHits = cms.InputTag('energyLossProducer:energyLossPixHits'),
  myenergyLossStrHits = cms.InputTag('energyLossProducer:energyLossStrHits'),
  myenergyLossAllHits = cms.InputTag('energyLossProducer:energyLossAllHits'),
  #muons = cms.InputTag('muons'),

  rpRecHitTag = cms.InputTag('totemRPRecHitProducer'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  rpTrackTag = cms.InputTag('ctppsLocalTrackLiteProducer'),
  mymyvalue = cms.InputTag('energyLossProducer:myvalue'),

  outputFileName = cms.string("output.root")
)

process.myreco = cms.Path(process.siStripMatchedRecHits
			* process.siPixelRecHits
			* process.MeasurementTrackerEvent
                        * process.siPixelClusterShapeCache
                        * process.trackerClusterCheck
			* process.iterTrackingEarly
			* process.earlyGeneralTracks
                        * process.muonSeededStep
			* process.preDuplicateMergingGeneralTracks
			* process.generalTracksSequence)

process.p = cms.Path(process.refitterForEnergyLoss*process.energyLossProducer)

process.schedule = cms.Schedule(process.myreco,process.p)

#process.schedule = cms.Schedule(process.reco,
#                                process.produceEnergyLoss,process.p)


