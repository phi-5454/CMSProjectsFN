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

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = "101X_dataRun2_Prompt_v11"

process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
"root://eostotem//eos/totem/data/cmstotem/2018/90m/RECO_copy/TOTEM21/120003/D2A64504-A750-E911-8CFF-0CC47A57D168.root"
  )
)


process.analyzer = cms.EDFilter("PromptAnalyzer",
  tracks = cms.InputTag('generalTracks'),
  rpRecHitTag = cms.InputTag('totemRPRecHitProducer'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  rpTrackTag = cms.InputTag('ctppsLocalTrackLiteProducer'),

  outputFileName = cms.string("output.root")
)

process.p = cms.Path(process.analyzer)


