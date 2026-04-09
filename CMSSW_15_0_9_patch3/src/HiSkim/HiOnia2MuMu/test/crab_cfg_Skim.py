from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config
from CRABClient.ClientExceptions import ClientException
from http.client import HTTPException

config = config()
config.General.workArea     = 'OO_Data_MINIAOD_Jpsi_Skim'
config.General.transferLogs = True
config.General.transferOutputs = True
config.JobType.pluginName     = 'Analysis'
config.JobType.psetName       = 'onia2MuMuPAT_OO_DATA_MiniAOD_cfg.py'
config.Data.unitsPerJob       = 5
config.JobType.maxMemoryMB = 2500
config.Data.splitting         = 'LumiBased'
#config.JobType.inputFiles = ['CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run3v1302x04_offline_374289.db']                                                                              
config.Data.inputDBS          = 'global'
config.Data.publication = True
config.Data.useParent = False
config.Data.outLFNDirBase  = '/store/user/singhm'
config.Site.storageSite       = 'T2_US_Vanderbilt'
config.Data.outputDatasetTag = 'Onia_Jpsi_Skim_OO_Run2025MiniAOD'
config.Data.lumiMask = 'generated_json.json'

try:
    crabCommand('submit', config=config)
except HTTPException as hte:
    print("Failed submitting task: {}".format(hte))
except ClientException as cle:
    print("Failed submitting task: {}".format(cle))


for i in range(0, 1, 1):
    config.General.requestName = f'Onia_JPsi_OO_skim_8th_November_test_dataset{i}'
    config.Data.inputDataset = f'/IonPhysics{i}/OORun2025-PromptReco-v1/MINIAOD'
    config.Data.outputDatasetTag = config.General.requestName
    crabCommand('submit', config = config, dryrun=False)
