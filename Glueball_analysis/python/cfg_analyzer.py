import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
  statistics = cms.untracked.vstring(),
  destinations = cms.untracked.vstring('cout'),
  cout = cms.untracked.PSet(
    threshold = cms.untracked.string('WARNING'),
  )
)

#########################
#      Preskimming      #
#########################
# declare global tag
process.load("Configuration.StandardSequences.GeometryDB_cff") ## FIXME need to ensure that this is the good one                                     
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "106X_dataRun2_testPPS_v1")

#JH - for tests of 2018 alignment candidate
#from CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi import *
#process.ctppsRPAlignmentCorrectionsDataESSourceXML = cms.ESSource("CTPPSRPAlignmentCorrectionsDataESSourceXML",
#    verbosity = cms.untracked.uint32(0),
#
#    MeasuredFiles = cms.vstring(),
#    RealFiles = cms.vstring(),
#    MisalignedFiles = cms.vstring()
#)

#process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
  # internal pixel alignment
#  "CalibPPS/ESProducers/data/alignment/RPixGeometryCorrections-2017-2018.xml",

  # global alignment of tracking RPs (strips and pixels)
#  "CalibPPS/ESProducers/data/alignment/alignment_export_2019_05_09.1.xml",

  # alignment of timing RPs (diamonds and UFSDs)
#  "CalibPPS/ESProducers/data/alignment/timing_RP_2017_preTS2_v2.xml",
#  "CalibPPS/ESProducers/data/alignment/timing_RP_2017_postTS2_v2.xml",
#  "CalibPPS/ESProducers/data/alignment/timing_RP_2018_v2.xml"
#)

process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#process.content = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring('/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/D74928A8-9E4D-1343-B900-B27D84D7E9EC.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100001/841797A4-6969-5743-A3B7-DAC243B2208E.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/92C1314B-9048-C54B-82C2-F6156027274D.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/080E32CE-0ECF-C144-A166-BCAE6F1C19BF.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/B4A7D674-9579-E847-ADF1-FD02A10D5E1A.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/239848DE-6611-7449-B8B1-31D1E8487F04.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/8B185602-9782-EB44-9EDA-C063FDC95A76.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/98916CA2-2B32-164B-97C4-D83599A98224.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/C93DEC68-4567-ED49-8895-076219DA963B.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/2842257F-94B5-EA45-AA44-88924BCECC06.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/7AFA623A-8A5B-6243-B357-3936A66F0237.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/A09B5181-87D4-194B-BB57-4211E66B10F3.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/8A4BFC5C-8F5C-D646-AC0A-2A9D4667852C.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/94260770-024D-1744-9732-854D840BF0B2.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100001/153770D0-4879-9A43-BDBF-F5A725A5F1A9.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/2541093D-B040-0349-B6C6-52218498D55D.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/5D9B6883-E47E-CF4B-B1CD-0FD3B05BFEF1.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/5098188F-AE6B-8E40-9B16-AA986A06D88D.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/70088E6D-CEC7-A44F-BD0C-028EB9E71C5A.root',
'/store/data/Run2018A/DoubleMuon/AOD/12Nov2019_UL2018-v2/100002/797E5D05-070F-1D46-8FCD-689782AE9D9A.root',
))


#########################
#        Triggers       #
#########################

process.load("analysis_glueballs_2018.PromptAnalyzer.HLTFilter_cfi")
process.hltFilter.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltFilter.HLTPaths = cms.vstring(
    'HLT_DoubleMu43NoFiltersNoVtx_*',
    'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_*',
    'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_*'
#    'HLT_DoubleEle33_CaloIdL_MW_v*',
#    'HLT_Ele27_HighEta_Ele20_Mass55_v*',
#    'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v*',
)

process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

#process.energyLossProducer.tag = cms.string('totem')

# get LHCInfo from DB tag
#from CondCore.CondDB.CondDB_cfi import *
#CondDB.connect = 'frontier://FrontierProd/CMS_CONDITIONS'
#process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#    CondDB,
#    DumpStat = cms.untracked.bool(False),
#    toGet = cms.VPSet(cms.PSet(
#        record = cms.string('LHCInfoRcd'),
#        tag = cms.string("LHCInfoEndFill_prompt_v2")
#    ))
#)


process.analyzer = cms.EDFilter("PromptAnalyzer",
#  tracks = cms.InputTag('generalTracks'),
#  dedxs = cms.InputTag('dedxHarmonic2'),
#  dedxPIXs = cms.InputTag('dedxPixelHarmonic2'),
  #dedxpixels = cms.InputTag('dedxHitInfo'),
#  vertices = cms.InputTag('offlinePrimaryVertices'),
  #triggers = cms.InputTag('TriggerResults','','HLT'),
#  pflows = cms.InputTag('particleFlow'),
#  myenergyLossPixHits = cms.InputTag('energyLossProducer'),
#  myenergyLossPixHits = cms.InputTag('energyLossProducer:energyLossPixHits'),
#  myenergyLossStrHits = cms.InputTag('energyLossProducer:energyLossStrHits'),
#  myenergyLossAllHits = cms.InputTag('energyLossProducer:energyLossAllHits'),
  #muons = cms.InputTag('muons'),

  year = cms.string('2018'),
  rpRecHitTag = cms.InputTag('totemRPRecHitProducer'),
  rpPatternTag = cms.InputTag('totemRPUVPatternFinder'),
  rpTrackTag = cms.InputTag('ctppsLocalTrackLiteProducer'),
  ppsRecoProtonMultiRPTag = cms.InputTag("ctppsProtons", "multiRP"),
  muonTag = cms.InputTag("muons"), # RECO ; needs recompilation!
  trackTag = cms.InputTag('generalTracks'),

  outputFileName = cms.string("output.root")
)

#process.reco = cms.Path(process.MeasurementTrackerEvent * process.siPixelClusterShapeCache)

#process.schedule = cms.Schedule(process.reco,process.produceEnergyLoss)

#process.p = cms.Path(process.refitterForEnergyLoss*process.energyLossProducer*process.content*process.analyzer)
#process.p = cms.Path(process.refitterForEnergyLoss*process.energyLossProducer*process.analyzer)
process.p = cms.Path(process.analyzer)

#process.schedule = cms.Schedule(process.reco,
#                                process.produceEnergyLoss,process.p)


