import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from   Configuration.StandardSequences.Eras import eras


#----------------------------------------------------------------------------

# Setup Settings for ONIA SKIM:

isMC           = False     # if input is MONTECARLO: True or if it's DATA: False
applyEventSel  = False      # if we want to apply 2016 pPb Event Selection
muonSelection  = "Trk"     # Single muon selection: Glb(isGlobal), GlbTrk(isGlobal&&isTracker), Trk(isTracker) are availale

#----------------------------------------------------------------------------


# Print Onia Skim settings:
print( " " )
print( "[INFO] Settings used for ONIA SKIM: " )
print( "[INFO] isMC          = " + ("True" if isMC else "False") )
print( "[INFO] applyEventSel = " + ("True" if applyEventSel else "False") )
print( "[INFO] muonSelection = " + muonSelection )
print( " " )

# set up process
process = cms.Process("Onia2MuMuPAT")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
#options.inputFiles = 'file:/eos/user/s/singhm/OO_dataset.root'
#options.inputFiles = '/store/hidata/OORun2025/IonPhysics1/MINIAOD/PromptReco-v1/000/394/154/00000/35d3344a-07b5-4ed1-8c1d-6e1036c9ad4c.root'
#options.inputFiles = '/store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/1af51e5c-75d0-45b8-9161-49e8928939f5.root' 
options.inputFiles = 'root://cms-xrd-global.cern.ch///store/hidata/OORun2025/IonPhysics12/MINIAOD/PromptReco-v1/000/394/153/00000/1af51e5c-75d0-45b8-9161-49e8928939f5.root'
#options.inputFiles = '/store/hidata/OORun2025/IonPhysics0/MINIAOD/PromptReco-v1/000/394/183/00000/796c5018-38b9-41c9-bd12-5a60b3183c5c.root'
#options.inputFiles = [
#  '/store/hidata/OORun2025/IonPhysics0/MINIAOD/PromptReco-v1/000/394/183/00000/796c5018-38b9-41c9-bd12-5a60b3183c5c.root'
#]
options.outputFile = 'file:onia2MuMuPAT_DATA.root'

#options.maxEvents = -1 # -1 means all events
#options.maxEvents = 1000
# get and parse the command line arguments
options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.categories.extend(["HiOnia2MuMuPAT_muonLessSizeORpvTrkSize"])
#process.MessageLogger.cerr.HiOnia2MuMuPAT_muonLessSizeORpvTrkSize = cms.untracked.PSet( limit = cms.untracked.int32(5) )

# load the Geometry and Magnetic Field for the TransientTrackBuilder
process.load('Configuration.StandardSequences.Services_cff')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.Reconstruction_cff')

# Global Tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '150X_dataRun3_Prompt_v1', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

from HiSkim.HiOnia2MuMu.onia2MuMuPAT_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=False, HLT="HLT", Filter=False, useL1Stage2=True)
changeToMiniAOD(process)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring( options.inputFiles ))
process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents             = cms.untracked.PSet( input = cms.untracked.int32(88000) )
process.outOnia2MuMu.fileName = cms.untracked.string( options.outputFile )
process.e                     = cms.EndPath(process.outOnia2MuMu)
process.schedule              = cms.Schedule(process.Onia2MuMuPAT,process.e)
