import FWCore.ParameterSet.Config as cms
process = cms.Process("ANASKIMOO")

process.load('Configuration.StandardSequences.Services_cff')
process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("clusterCompatibilityFilter_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

process.load('hffilterPF_cfi')

process.source = cms.Source ("PoolSource",
                fileNames=cms.untracked.vstring(
                        'root://cmsxrootd.fnal.gov//store/user/chenzh/IonPhysics0/RecoSkim2025_OO_IonPhysics0_D0_v3/251201_064024/0000/OO2025_D0Skim_1.root '

                ),
                secondaryFileNames=cms.untracked.vstring(
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics0/MINIAOD/PromptReco-v1/000/394/183/00000/2f678f5f-aa7f-495c-b4f1-6b3c734c5600.root',
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics0/MINIAOD/PromptReco-v1/000/394/183/00000/796c5018-38b9-41c9-bd12-5a60b3183c5c.root'
                )
        )

# =============== Other Statements =====================
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = '150X_dataRun3_Prompt_v1'

# =============== Import Sequences =====================
#Trigger Selection
### Comment out for the timing being assuming running on secondary dataset with trigger bit selected already

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlineSlimmedPrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)

#Reject beam scraping events standard pp configuration, not working in miniAOD
#process.NoScraping = cms.EDFilter("FilterOutScraping",
#    applyfilter = cms.untracked.bool(True),
#    debugOn = cms.untracked.bool(False),
#    numtrack = cms.untracked.uint32(10),
#    thresh = cms.untracked.double(0.25)
#)

from HeavyIonsAnalysis.TrackAnalysis.unpackedTracksAndVertices_cfi import *
process.unpackedTracksAndVertices = unpackedTracksAndVertices
process.load('HeavyIonsAnalysis.VertexAnalysis.pileupvertexfilter_cfi')
process.pileupvertexfilter.doOO = True
process.pileupvertexfilter.doNeNe = False

process.PAcollisionEventSelection = cms.Sequence(
                                         process.phfCoincFilterPF2Th4 *
                                         process.PAprimaryVertexFilter *
                                         process.clusterCompatibilityFilter *
                                         process.unpackedTracksAndVertices *
                                         process.pileupvertexfilter
#                                         process.NoScraping
                                         )

process.eventFilter_ZB = cms.Sequence(

    process.PAcollisionEventSelection
)

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(
    HLTPaths = [
       "HLT_MinimumBiasHF_OR_BptxAND_v*"
    ]
)

process.eventFilter_ZB_step = cms.Path( process.eventFilter_ZB )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('OOD0.root')
)

process.anaTracks = cms.EDAnalyzer("OOD0Skim",
    VertexCompositeCollection = cms.untracked.InputTag("generalD0CandidatesNew:D0"),
    VertexLabel = cms.InputTag('offlineSlimmedPrimaryVertices'),
    TrackLabel = cms.InputTag('packedPFCandidates')
    # VertexCompositeCollection = cms.untracked.InputTag("generalV0CandidatesNewKshort:Kshort")
)

process.anatrack_step = cms.Path(process.hltfilter * process.PAcollisionEventSelection *  process.anaTracks)

process.schedule = cms.Schedule(

    process.anatrack_step
)
