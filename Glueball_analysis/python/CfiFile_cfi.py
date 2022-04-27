import FWCore.ParameterSet.Config as cms

demo = cms.EDAnalyzer('RecoAnalyzer'
     ,tracks = cms.untracked.InputTag('ctfWithMaterialTracks')
)
