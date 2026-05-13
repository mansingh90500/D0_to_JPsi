from CRABAPI.RawCommand import crabCommand
from WMCore.Configuration import Configuration
from CRABClient.ClientExceptions import ClientException
from http.client import HTTPException

# Loop over datasets                                                                                                                                          
for i in range(1, 10): # start with 30 and Goal is to complete 40                                                                                            

    config = Configuration()

    # === General ===                                                                                                                                         
    config.section_('General')
    config.General.workArea = 'OO_Data_MINIAOD'
    config.General.requestName = f'2pc_corr_jpsi_vs_ch_20260323_v88_flow{i}'
    config.General.transferLogs = True
    config.General.transferOutputs = True

    # === JobType ===                                                                                                                                         
    config.section_('JobType')
    config.JobType.pluginName = 'Analysis'
    config.JobType.psetName = 'OOD0skim.py'
    config.JobType.maxMemoryMB = 3000
    # === Data ===                                                                                                                                            
    config.section_('Data')
    config.Data.inputDBS = 'phys03'
    config.Data.splitting = 'LumiBased'
    config.Data.unitsPerJob = 4
    config.Data.totalUnits = -1
    config.Data.publication = False
    config.Data.inputDataset = (
#        f'/IonPhysics{i}/singhm-Onia_JPsi_skim_dataset{i}_20251108_v01-c2dd480db12ad79ef29b6eb3d7408304/USER'                                                
        f'/IonPhysics{i}/singhm-Onia_JPsi_skim_dataset{i}_20251116_v01-c2dd480db12ad79ef29b6eb3d7408304/USER'
    )

    config.Data.secondaryInputDataset = (
        f'/IonPhysics{i}/OORun2025-PromptReco-v1/MINIAOD'
    )

    config.Data.outputDatasetTag = config.General.requestName
    #config.Data.outLFNDirBase = '/store/group/phys_heavyions/prverma/OO_Man/2pc_corr'
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/singhm/OO_Man/2pc_corr_D0_Jpsi'                                                                         
    #config.Data.outLFNDirBase = '/store/group/phys_heavyions/singhm/OO_Man/2pc_corr_CHvsCH_Cent'                                                             
    config.Data.lumiMask = 'golden.json'
    #config.Data.lumiMask = 'generated_json.json'
    config.section_('Site')
    #config.Site.storageSite = 'T3_CH_CERNBOX'                                                                                                                
    config.Site.storageSite = 'T2_CH_CERN'
    # Optional empty sections (CRAB allows them)                                                                                                              
    config.section_('User')
    config.section_('Debug')

    # === Submit task ===                                                                                                                                     
    try:
        crabCommand('submit', config=config, dryrun=False)
    except HTTPException as hte:
        print(f'Failed submitting task (HTTP): {hte}')
    except ClientException as cle:
        print(f'Failed submitting task (Client): {cle}')
    except Exception as e:
        print(f'Failed submitting task (Other): {e}')

