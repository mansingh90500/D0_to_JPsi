from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from http.client import HTTPException

# We want to put all the CRAB project directories from the tasks we submit here into one common directory.
# That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
from CRABClient.UserUtilities import config, getUsername
config = config()

config.section_("General")
config.General.workArea = '/eos/cms/store/group/phys_heavyions/jiateng/flow_OO_D0/crab_project'
#config.General.workArea = '/eos/user/j/jiateng/crab_project/NeNe_data'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'OOD0skim.py'
config.JobType.allowUndistributedCMSSW = True

config.section_("Data")
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.useParent = True
config.Data.publication = False

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T3_CH_CERNBOX'


def submit(config):
    try:
        crabCommand('submit', config=config, dryrun=False)
    except HTTPException as hte:
        print("Failed submitting task: %s" % (hte.headers))
    except ClientException as cle:
        print("Failed submitting task: %s" % (cle))


dataMap = {}
#for i in [0]:
for i in range(30,40):
    dataMap["OOdata%d" % i] = { "PD": "/IonPhysics%d/chenzh-RecoSkim2025_OO_IonPhysics%d_D0_v3-5bc5ab9f3e3736df36ecbc780cae2828/USER"% (i, i), "Units": 25, "Memory": 2500, "RunTime": 1800 , "LumiMask" : "Cert_Collisions2025OO_394153_394217_silver.json"}
## Submit the PDs
for key, val in dataMap.items():
    config.General.requestName = '07Apr2026_D0skim_cent40to80_part4_'+key
    config.Data.inputDataset = val["PD"]
    config.Data.lumiMask = val["LumiMask"]
    config.Data.unitsPerJob = val["Units"]
    config.JobType.maxMemoryMB = val["Memory"]
    config.JobType.maxJobRuntimeMin = val["RunTime"]
    config.Data.outputDatasetTag = config.General.requestName
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiateng/flow_OO_D0'
    print("Submitting CRAB job for: "+val["PD"])
    submit(config)
