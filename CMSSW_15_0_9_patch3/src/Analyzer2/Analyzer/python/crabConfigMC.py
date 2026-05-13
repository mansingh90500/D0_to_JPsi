from WMCore.Configuration import Configuration

config = Configuration()

# ===================== General =====================
config.section_('General')
config.General.requestName = '2pc_corr_MC_jpsi_vs_ch_20260425_v93_HM_flow_Nassoc'
config.General.workArea = '/eos/cms/store/group/phys_heavyions/singhm/Sayan_OO_MC_MINIAOD'
config.General.transferOutputs = True
config.General.transferLogs = True

# ===================== JobType =====================
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'OOD0skim.py'
config.JobType.maxMemoryMB = 3000

# ===================== Data =====================
config.section_('Data')
config.Data.inputDataset = '/ProJpsi_MC_Dataset_50m/singhm-Onia_Prompt_JPsi_MC_skim_20260421_v04-c398c79d42b64db1228bab90526751b2/USER'
config.Data.secondaryInputDataset = '/ProJpsi_MC_Dataset_50m/phys_heavyions-MC_Production_04_PAT-6b5e840910d4f3ac15e386875c2449a5/USER'

config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = -1

config.Data.outLFNDirBase = '/store/group/phys_heavyions/singhm/OO_Man/2pc_corr_MC'
config.Data.publication = False
#config.Data.outputDatasetTag = '2pc_corr_MC_jpsi_vs_ch_20260425_v90_flow'

# ===================== Site =====================
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

# ===================== User =====================
config.section_('User')

# ===================== Debug =====================
config.section_('Debug')
