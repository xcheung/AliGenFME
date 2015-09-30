const Bool_t bIsInfoAOD = kTRUE;
const Bool_t bAnaInfoMC = kFALSE;
const Bool_t bOutputAOD = kFALSE;

const Bool_t bIsPhysSel = kFALSE;
const Bool_t bIsCentSel = kFALSE;
const Bool_t bIsEvPnSel = kFALSE;
const Bool_t bIsRespPID = kFALSE;

const UInt_t wTriggerMask = AliVEvent::kMB;
//=============================================================================

const TString sPeriodIn = "LHC11h";
const TString sCentEsti = "V0M";  // "V0M"; "V0A"; "V0C"
const Double_t dCentMin =    0.;
const Double_t dCentMax =  100.;

const TString sInputTrkRD = (bIsInfoAOD ? "tracks"          : "Tracks");
const TString sInputClsRD = (bIsInfoAOD ? "caloClusters" : "CaloClusters");

const TString sFilterTrks = (bIsInfoAOD ? "AODFilterTracks" : "ESDFilterTracks");

const TString sUsedTrksRD = "PicoTracks";
const TString sUsedClusRD = "CaloClustersCorr";
const TString sUsedTrksMC = "MCParticlesSelected";
//=============================================================================

const Int_t   nJetType = 0;       // 0: FullJet; 1: ChargedJet; 2: NeutralJet
const Int_t   nLeading = 0;       // 0: charged; 1: neutral; 2: both
const TString sCutType = "EMCAL"; // "TPC", "EMCAL"

const Double_t dJetRadius  = 0.4;
const Double_t dJetPtCut   = 5.00;
const Double_t dJetAreaCut = 0.6;

const Double_t dTrkPtCut  = 0.15;
const Double_t dCluEnCut  = 0.30;
//=============================================================================

Bool_t AddTasksEmcalFjetSparseMaker()
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();

  if (bIsInfoAOD) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddAODHandler.C");
    AliAODInputHandler *aodIH = AddAODHandler();
  } else {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddESDHandler.C");
    AliESDInputHandler *esdIH = AddESDHandler();
//  esdIH->SetReadFriends(kFALSE);
  }

  if (bAnaInfoMC && (!bIsInfoAOD)) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddMCHandler.C");
    AliMCEventHandler *mctEH = AddMCHandler(kTRUE);
  }

  if (bOutputAOD) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddAODOutputHandler.C");
    AliAODHandler *aodH = AddAODOutputHandler();
    aodH->SetOutputFileName("AliAOD.PicoJets.root");
    aodH->SetFillAOD(kTRUE);
    aodH->SetCreateNonStandardAOD();
    mgr->SetOutputEventHandler(aodH);
  }
//=============================================================================

  if (bIsPhysSel) {
    gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalPhysicsSelection.C");
    AliEmcalPhysicsSelectionTask *taksPhysSel = AddTaskEmcalPhysicsSelection(kTRUE, kTRUE, wTriggerMask, 5., 5., 10., kTRUE, -1, -1, -1, -1);
    if (bAnaInfoMC) {
      AliEmcalPhysicsSelection *pPhysSel = static_cast<AliEmcalPhysicsSelection*>(taksPhysSel->GetPhysicsSelection());
      if (!pPhysSel) return kTRUE; pPhysSel->SetAnalyzeMC();
    }
  }

  if (bIsCentSel) {
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/macros/AddTaskCentrality.C");
    AliCentralitySelectionTask *taskCentSel = AddTaskCentrality(kTRUE, bIsAOD);
    if (bAnaInfoMC) taskCentSel->SetMCInput();
  }

  if (bIsEvPnSel) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskEventplane.C");
    AliEPSelectionTask *taskEventPlane = AddTaskEventplane();
    if (bAnaInfoMC) taskEventPlane->SetUseMCRP();
  }

  if (bIsRespPID) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
    AliAnalysisTaskSE *taskRespPID = AddTaskPIDResponse(bAnaInfoMC);
  }
//=============================================================================

  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalSetup.C");
  AliEmcalSetupTask *taskSetupEMCal = AddTaskEmcalSetup();
  taskSetupEMCal->SetGeoPath("$ALICE_PHYSICS/OADB/EMCAL");

  gROOT->LoadMacro("$ALICE_PHYSICS/PWG/EMCAL/macros/AddTaskEmcalPreparation.C");
  AliAnalysisTaskSE *taskEmcMaker = AddTaskEmcalPreparation(sPeriodIn);

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskJetPreparation.C");
  AliAnalysisTaskSE *taskHadCorr = AddTaskJetPreparation(sPeriodIn.Data(),
                                                         sUsedTrksRD.Data(),
                                                        (bAnaInfoMC ? sUsedTrksMC.Data() : ""),
                                                         sInputClsRD.Data(),
                                                         sUsedClusRD.Data(),
                                                         2., 0., 0.03, 0.015, 0.15,
                                                         wTriggerMask,
                                                         kTRUE,   // do trk clus
                                                         kTRUE,   // histos
                                                         kTRUE,   // make pico trk
                                                         kFALSE,  // make trigger
                                                         kFALSE,  // is EMCal train
                                                         1.,      // track eff
                                                         kTRUE,   // do AOD track prop
                                                         kTRUE,   // modify match
                                                         kFALSE,  // use old bit
                                                         kFALSE,  // trigger QA
                                                         4);      // cent bin
//=============================================================================

  gROOT->LoadMacro("$ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/AddTaskEmcalJet.C");

/*AliEmcalJetTask *taskAkt = AddTaskEmcalJet(sUsedTrksRD.Data(),
                                             sUsedClusRD.Data(),
                                             1, // anti-kT
                                             dJetRadius,
                                             nJetType,
                                             dTrkPtCut,
                                             dCluEnCut);*/
//=============================================================================

  AliAnalysisTaskEmcalFjetSparseMaker *taskEmcalSM = new AliAnalysisTaskEmcalFjetSparseMaker("AliAnalysisTaskEmcalFjetSM");
//taskEmcalSM->SetForceBeamType(0);
//taskEmcalSM->SetIsPythia(kTRUE);

/*taskEmcalSM->SetCaloTriggerPatchInfoName("EmcalTriggers");
  taskEmcalSM->SetTriggerTypeSel(AliAnalysisTaskEmcal::kJ2);
  taskEmcalSM->SetMainPatchType(AliAnalysisTaskEmcal::kTriggerLevel1Jet);*/

  AliParticleContainer *pContTrks = taskEmcalSM->AddParticleContainer(sUsedTrksRD.Data());
  pContTrks->SetParticlePtCut(dTrkPtCut);

  AliClusterContainer *pContClus = taskEmcalSM->AddClusterContainer(sUsedClusRD.Data());
  pContClus->SetClusPtCut(dCluEnCut);

/*AliJetContainer *pContJets = taskEmcalSM->AddJetContainer(taskAkt->GetName(), sCutType.Data(), dJetRadius);
  pContJets->SetPercAreaCut(dJetAreaCut);
  pContJets->SetJetPtCut(dJetPtCut);
//pContJets->SetRhoName(sUsedRho.Data());
//pContJets->SetLocalRhoName();
  pContJets->SetLeadingHadronType(nLeading);
  pContJets->ConnectParticleContainer(taskEmcalSM->GetParticleContainer());
  pContJets->ConnectClusterContainer(taskEmcalSM->GetClusterContainer());
  pContJets->SetNameTitle(taskEmcalSM->GetNameJet().Data(),
                          taskEmcalSM->GetNameJet().Data());*/

  mgr->AddTask(taskEmcalSM);
  mgr->ConnectInput(taskEmcalSM,  0, mgr->GetCommonInputContainer());

  mgr->ConnectOutput(taskEmcalSM, 1, mgr->CreateContainer("listGeneralH",
                                                          TList::Class(),
                                                          AliAnalysisManager::kOutputContainer,
                                                          AliAnalysisManager::GetCommonFileName()));

  mgr->ConnectOutput(taskEmcalSM, 2, mgr->CreateContainer("listEmcalEvH",
                                                          TList::Class(),
                                                          AliAnalysisManager::kOutputContainer,
                                                          "AnalysisResults_EmcalEvH.root"));

  mgr->ConnectOutput(taskEmcalSM, 3, mgr->CreateContainer("listEmcalTrk",
                                                          TList::Class(),
                                                          AliAnalysisManager::kOutputContainer,
                                                          "AnalysisResults_EmcalTrk.root"));

  mgr->ConnectOutput(taskEmcalSM, 4, mgr->CreateContainer("listEmcalClu",
                                                          TList::Class(),
                                                          AliAnalysisManager::kOutputContainer,
                                                          "AnalysisResults_EmcalClu.root"));

/*mgr->ConnectOutput(taskEmcalSM, 5, mgr->CreateContainer("listEmcalJet",
                                                          TList::Class(),
                                                          AliAnalysisManager::kOutputContainer,
                                                          "AnalysisResults_EmcalJet.root"));*/
//=============================================================================

  TObjArray *aTasks = mgr->GetTasks();
  for (Int_t i=0; i<aTasks->GetEntries(); i++) {
    AliAnalysisTaskEmcal *taskJE = dynamic_cast<AliAnalysisTaskEmcal*>(aTasks->At(i));
    if (taskJE) if (taskJE->InheritsFrom("AliAnalysisTaskEmcal")) {
      taskJE->SetVzRange(-10., 10.);

      if (!sCentEsti.IsNull()) {
        taskJE->SetCentralityEstimator(sCentEsti.Data());
        taskJE->SetCentRange(dCentMin, dCentMax);
       }
    }

    if (wTriggerMask) {
      AliAnalysisTaskSE *taskSE = dynamic_cast<AliAnalysisTaskSE*>(aTasks->At(i));
      if (taskSE) if (!(taskSE->InheritsFrom("AliPhysicsSelectionTask"))) {
        taskSE->SelectCollisionCandidates(wTriggerMask);
      }
    }
  }
//=============================================================================

  return kFALSE;
}
