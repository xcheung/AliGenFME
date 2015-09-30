const Bool_t bIsInfoAOD = kTRUE;
const Bool_t bAnaInfoMC = kFALSE;
//=============================================================================

void AnalysisTrainLocalEmcalFjetSparseMaker()
{
  const TString sDataset = "/DATA04/xzhang/alice/lists/LHC11hPass2AOD145.txt";
//=============================================================================

  if (LoadLibraries()) {
    ::Error("AnalysisTrainLocalEmcalFjetSparseMaker.C::AnalysisTrainLocalEmcalFjetSparseMaker","Load libraries failed!");
    return;
  }

  TChain *chain = CreateChain(sDataset);
  if (!chain) {
    ::Error("AnalysisTrainLocalEmcalFjetSparseMaker.C::AnalysisTrainLocalEmcalFjetSparseMaker", "Creating input chain failed!");
    return;
  }
//=============================================================================

  AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTrainLocalEmcalFjetSparseMaker", "Analysis Train Local EMCal Bkg SM");
//mgr->SetDebugLevel(3);

  gROOT->LoadMacro("AddTasksEmcalFjetSparseMaker.C"); if (AddTasksEmcalFjetSparseMaker()) return;
  if (mgr->InitAnalysis()) { mgr->PrintStatus(); mgr->StartAnalysis("local",chain); }
  return;
}

//_____________________________________________________________________________
TChain *CreateChain(TString dataset)
{
  TChain *chain = 0;
  if (bIsInfoAOD)
    chain = new TChain("aodTree");
  else
    chain = new TChain("esdTree");

  if (gSystem->AccessPathName(dataset.Data())) {
    ::Error("AnalysisTrainLocalEmcalFjetSparseMaker.C::CreateChain","Dataset %s does not exist!",dataset.Data());
    return NULL;
  }

  TString dataFile;
  ifstream dataList(dataset.Data(), ios::in);
  while (!dataList.eof()) {
    dataFile.ReadLine(dataList,kFALSE);
    if (!dataFile.EndsWith(".root")) continue;
    if (!gSystem->AccessPathName(dataFile.Data())) chain->Add(dataFile.Data());
  } dataList.close();

  return chain;
}

//_____________________________________________________________________________
TChain *CreateAODFriendChain(TString sDataset)
{ 
  if (!sDataset.EndsWith(".txt")) return 0;

  TChain *chain = new TChain("aodTree");
  TChain *cFrid = new TChain("aodTree");

  TString dataFile;
  ifstream dataList(sDataset.Data(), ios::in); 
  while (!dataList.eof()) {
    dataFile.ReadLine(dataList,kFALSE);
    if (!dataFile.EndsWith("AliAOD.root")) continue;
    if (!gSystem->AccessPathName(dataFile.Data())) chain->Add(dataFile.Data());

    dataFile.ReplaceAll("AliAOD.root","AliAOD.VertexingHF.root");
    if (!gSystem->AccessPathName(dataFile.Data())) cFrid->Add(dataFile.Data());
  } dataList.close();

  chain->AddFriend(cFrid);
  return chain;
}

//_____________________________________________________________________________
Bool_t LoadLibraries()
{
  if (gSystem->Load("libTree")       <0) return kTRUE;
  if (gSystem->Load("libGeom")       <0) return kTRUE;
  if (gSystem->Load("libPhysics")    <0) return kTRUE;
  if (gSystem->Load("libVMC")        <0) return kTRUE;
  if (gSystem->Load("libMinuit")     <0) return kTRUE;
  if (gSystem->Load("libMinuit2")    <0) return kTRUE;

  if (gSystem->Load("libCore")       <0) return kTRUE;
  if (gSystem->Load("libXMLIO")      <0) return kTRUE;
  if (gSystem->Load("libXMLParser")  <0) return kTRUE;
  if (gSystem->Load("libProof")      <0) return kTRUE;
  if (gSystem->Load("libProofPlayer")<0) return kTRUE;
  if (gSystem->Load("libGui")        <0) return kTRUE;
//=============================================================================

  if (gSystem->Load("libSTEERBase")         <0) return kTRUE;
  if (gSystem->Load("libESD")               <0) return kTRUE;
  if (gSystem->Load("libAOD")               <0) return kTRUE;
  if (gSystem->Load("libANALYSIS")          <0) return kTRUE;
  if (gSystem->Load("libOADB")              <0) return kTRUE;
  if (gSystem->Load("libANALYSISalice")     <0) return kTRUE;
  if (gSystem->Load("libCORRFW")            <0) return kTRUE;

  if (gSystem->Load("libCDB")               <0) return kTRUE;
  if (gSystem->Load("libRAWDatabase")       <0) return kTRUE;
  if (gSystem->Load("libRAWDatarec")        <0) return kTRUE;
  if (gSystem->Load("libSTEER")             <0) return kTRUE;
  if (gSystem->Load("libITSbase")           <0) return kTRUE;
  if (gSystem->Load("libITSrec")            <0) return kTRUE;
  if (gSystem->Load("libTPCbase")           <0) return kTRUE;
  if (gSystem->Load("libTPCrec")            <0) return kTRUE;
  if (gSystem->Load("libTRDbase")           <0) return kTRUE;
  if (gSystem->Load("libTRDrec")            <0) return kTRUE;
  if (gSystem->Load("libTOFbase")           <0) return kTRUE;
//if (gSystem->Load("libTOFrec")            <0) return kTRUE;
  if (gSystem->Load("libHMPIDbase")         <0) return kTRUE;
  if (gSystem->Load("libEMCALraw")          <0) return kTRUE;
  if (gSystem->Load("libEMCALbase")         <0) return kTRUE;
  if (gSystem->Load("libEMCALrec")          <0) return kTRUE;
  if (gSystem->Load("libVZERObase")         <0) return kTRUE;
  if (gSystem->Load("libVZEROrec")          <0) return kTRUE;
  if (gSystem->Load("libTender")            <0) return kTRUE;
  if (gSystem->Load("libTenderSupplies")    <0) return kTRUE;

  if (gSystem->Load("libPWGTools")          <0) return kTRUE;
//if (gSystem->Load("libPWGflowBase")       <0) return kTRUE;
//if (gSystem->Load("libPWGflowTasks")      <0) return kTRUE;
//if (gSystem->Load("libPWGHFbase")         <0) return kTRUE;
//if (gSystem->Load("libPWGHFvertexingHF")  <0) return kTRUE;

  if (gSystem->Load("libSTAT")              <0) return kTRUE;
  if (gSystem->Load("libEMCALUtils")        <0) return kTRUE;
//if (gSystem->Load("libPHOSUtils")         <0) return kTRUE;

  if (gSystem->Load("libCGAL")              <0) return kTRUE;
  if (gSystem->Load("libfastjet")           <0) return kTRUE;
//if (gSystem->Load("libSISConePlugin")     <0) return kTRUE;
//if (gSystem->Load("libCDFConesPlugin")    <0) return kTRUE;
  if (gSystem->Load("libsiscone")           <0) return kTRUE;
  if (gSystem->Load("libsiscone_spherical") <0) return kTRUE;
  if (gSystem->Load("libfastjetplugins")    <0) return kTRUE;
  if (gSystem->Load("libfastjettools")      <0) return kTRUE;
  if (gSystem->Load("libfastjetcontribfragile")<0) return kTRUE;

  if (gSystem->Load("libJETAN")             <0) return kTRUE;
  if (gSystem->Load("libFASTJETAN")         <0) return kTRUE;
  if (gSystem->Load("libPWGEMCAL")          <0) return kTRUE;
//if (gSystem->Load("libPWGGAEMCALTasks")   <0) return kTRUE;
  if (gSystem->Load("libPWGJEEMCALJetTasks")<0) return kTRUE;
//=============================================================================

  gSystem->AddIncludePath("-Wno-deprecated");
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include");
//=============================================================================

  if (gROOT->LoadMacro("AliAnalysisTaskEmcalFjetSparseMaker.cxx+")<0) return kTRUE;
//=============================================================================

  return kFALSE;
}
