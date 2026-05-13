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

process.source = cms.Source("PoolSource",
                fileNames=cms.untracked.vstring(
                        # J/psi skim file as primary input
                        'root://cms-xrd-global.cern.ch///store/user/singhm/IonPhysics12/Onia_JPsi_skim_dataset12_20251116_v01/251222_185212/0000/onia2MuMuPAT_DATA1000_1.root'
                ),
                secondaryFileNames=cms.untracked.vstring(
                        # Secondary MINIAOD files for associated charged hadrons
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/1af51e5c-75d0-45b8-9161-49e8928939f5.root',
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/1de7cccf-b5fc-4812-b549-a456094f185c.root',
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/2c129d7d-657a-4b0e-92b2-242894aff79f.root',
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/47c42a77-d334-4eb5-876c-522793bab430.root',
                        'root://cmsxrootd.fnal.gov//store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/48a873d1-3944-4712-a095-bb058ea91653.root'
                )
        )

# =============== Other Statements =====================
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = '150X_dataRun3_Prompt_v1'

# =============== Import Sequences =====================
# Trigger Selection
# Note: HLT filter already applied in skim, but re-applied here for safety

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlineSlimmedPrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)

# Reject beam scraping events - standard pp configuration, not working in miniAOD
# process.NoScraping = cms.EDFilter("FilterOutScraping",
#     applyfilter = cms.untracked.bool(True),
#     debugOn = cms.untracked.bool(False),
#     numtrack = cms.untracked.uint32(10),
#     thresh = cms.untracked.double(0.25)
# )

from HeavyIonsAnalysis.TrackAnalysis.unpackedTracksAndVertices_cfi import *
process.unpackedTracksAndVertices = unpackedTracksAndVertices
process.load('HeavyIonsAnalysis.VertexAnalysis.pileupvertexfilter_cfi')
process.pileupvertexfilter.doOO = True
process.pileupvertexfilter.doNeNe = False

process.PAcollisionEventSelection = cms.Sequence(
                                         process.phfCoincFilterPF2Th4 *
                                         process.PAprimaryVertexFilter *
                                         #process.clusterCompatibilityFilter *
                                         process.unpackedTracksAndVertices *
                                         process.pileupvertexfilter
                                         # process.NoScraping
                                         )

process.eventFilter_ZB = cms.Sequence(
    process.PAcollisionEventSelection
)

# HLT Filter for J/psi flow analysis
from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel
process.hltfilter = hltHighLevel.clone(
    HLTPaths = [
       "HLT_MinimumBiasHF_OR_BptxAND_v*"
    ]
)

process.eventFilter_ZB_step = cms.Path(process.eventFilter_ZB)

# =============== Output and Analyzer Settings =====================
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('OO_JPsi.root')
)

# OOJPsiSkim analyzer configuration for J/psi flow measurement
process.anaTracks = cms.EDAnalyzer("OOJPsiSkim",
    # J/psi composite candidates from skim (pat::CompositeCandidate)
    JPsiCollection = cms.untracked.InputTag("onia2MuMuPatGlbGlb"),
    # Muon collection for soft muon ID access
    MuonCollection = cms.untracked.InputTag("unpackedMuons"),
    # Primary vertex
    VertexLabel = cms.InputTag('offlineSlimmedPrimaryVertices'),
    # Associated charged hadrons (packed PF candidates)
    TrackLabel = cms.InputTag('packedPFCandidates')
)

process.anatrack_step = cms.Path(
    process.hltfilter * 
    process.PAcollisionEventSelection * 
    process.anaTracks
)

process.schedule = cms.Schedule(
    process.anatrack_step
)
