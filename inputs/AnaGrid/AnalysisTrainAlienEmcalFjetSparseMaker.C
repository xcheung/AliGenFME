const Bool_t bUseVtxAOD = kFALSE;

const TString versionAPI     = "V1.1x";
const TString versionROOT    = "v5-34-30-1";
const TString versionAliRoot = "v5-06-25";
const TString versionAliPhys = "vAN-20150617";
const TString taskName       = "AnaSubJetFF20150723";
//=============================================================================

class AliAnalysisGrid;
void AnalysisTrainAlienEmcalFjetSparseMaker()
{
  if (LoadLibraries()) {
    ::Error("AnalysisTrainAlienEmcalFjetSparseMaker.C::AnalysisTrainAlienEmcalFjetSparseMaker","Load libraries failed!");
    return;
  }

  if (!TGrid::Connect("alien://")) {
    ::Error("AnalysisTrainAlienEmcalFjetSparseMaker.C::AnalysisTrainAlienEmcalFjetSparseMaker","Can not connect to the Grid!");
    return;
  }

  AliAnalysisGrid *alienHandler = CreateAlienHandler();
  if (!alienHandler) {
    ::Error("AnalysisTrainAlienEmcalFjetSparseMaker::AnalysisTrainAlienEmcalFjetSparseMaker","no Grid handler is found!");
    return;
  }

  AliAnalysisManager *mgr = new AliAnalysisManager("AnalysisTrainAlienEmcalFjetSparseMaker", "Analysis Train Alien Ecmal Jet User");
  mgr->SetGridHandler(alienHandler);

  gROOT->LoadMacro("AddTasksEmcalFjetSparseMaker.C"); if (AddTasksEmcalFjetSparseMaker()) return;
  if (mgr->InitAnalysis()) { mgr->PrintStatus(); mgr->StartAnalysis("grid"); }
  return;
}

//_____________________________________________________________________________
AliAnalysisGrid *CreateAlienHandler()
{
//if (!AliAnalysisGrid::CreateToken()) return NULL;
  AliAnalysisAlien *plugin = new AliAnalysisAlien();
  plugin->SetOverwriteMode();
  plugin->SetRunMode("offline"); // "full", "test", "offline", "submit" or "terminate"
//plugin->SetNtestFiles(1);
//plugin->SetPreferedSE("ALICE::NIHAM::FILE");
  plugin->SetAPIVersion(versionAPI.Data());
  plugin->SetROOTVersion(versionROOT.Data());
  plugin->SetAliROOTVersion(versionAliRoot.Data());
  plugin->SetAliPhysicsVersion(versionAliPhys.Data());

  plugin->AddExternalPackage("boost::v1_53_0");
  plugin->AddExternalPackage("cgal::v4.4");
  plugin->AddExternalPackage("fastjet::v3.0.6_1.012");
//=============================================================================

//input method 1: dummy setting
  plugin->SetGridDataDir("/alice/sim/LHC08d7a");
  plugin->SetDataPattern("*AliAOD.root");
  if (bUseVtxAOD) plugin->SetFriendChainName("AliAOD.VertexingHF.root");

  plugin->AddRunNumber(122055);
//for (Int_t i=10000; i<=10015; i++) plugin->AddRunNumber(i);

//input method 2:
//plugin->AddDataFile("dataTest.xml");
//plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");

  plugin->SetGridWorkingDir(taskName.Data());
  plugin->SetGridOutputDir("output");
//=============================================================================

  TString sAddLibs = "libMinuit2.so";
  sAddLibs +=       " libCore.so";
  sAddLibs +=       " libXMLIO.so";
  sAddLibs +=       " libXMLParser.so";
  sAddLibs +=       " libProof.so";
  sAddLibs +=       " libProofPlayer.so";
  sAddLibs +=       " libGui.so";
  plugin->SetAdditionalRootLibs(sAddLibs.Data());

  sAddLibs  =  "libCDB.so";
  sAddLibs += " libRAWDatabase.so";
  sAddLibs += " libRAWDatarec.so";
  sAddLibs += " libSTEER.so";
  sAddLibs += " libITSbase.so";
  sAddLibs += " libITSrec.so";
  sAddLibs += " libTPCbase.so";
  sAddLibs += " libTPCrec.so";
  sAddLibs += " libTRDbase.so";
  sAddLibs += " libTRDrec.so";
  sAddLibs += " libTOFbase.so";
//sAddLibs += " libTOFrec.so";
  sAddLibs += " libHMPIDbase.so";
  sAddLibs += " libEMCALraw.so";
  sAddLibs += " libEMCALbase.so";
  sAddLibs += " libEMCALrec.so";
  sAddLibs += " libVZERObase.so";
  sAddLibs += " libVZEROrec.so";
  sAddLibs += " libTender.so";
  sAddLibs += " libTenderSupplies.so";

  sAddLibs += " libPWGTools.so";
//sAddLibs += " libPWGflowBase.so";
//sAddLibs += " libPWGflowTasks.so";
//sAddLibs += " libPWGHFbase.so";
//sAddLibs += " libPWGHFvertexingHF.so";

  sAddLibs += " libSTAT.so";
  sAddLibs += " libEMCALUtils.so";
//sAddLibs += " libPHOSUtils.so";

  sAddLibs += " libCGAL.so";
  sAddLibs += " libfastjet.so";
  sAddLibs += " libsiscone.so";
  sAddLibs += " libsiscone_spherical.so";
  sAddLibs += " libfastjetplugins.so";
  sAddLibs += " libfastjettools.so";
  sAddLibs += " libfastjetcontribfragile.so";

  sAddLibs += " libJETAN.so";
  sAddLibs += " libFASTJETAN.so";
  sAddLibs += " libPWGEMCAL.so";
//sAddLibs += " libPWGGAEMCALTasks.so";
  sAddLibs += " libPWGJEEMCALJetTasks.so";

  sAddLibs += " AliAnalysisTaskEmcalFjetSparseMaker.cxx";
  sAddLibs += " AliAnalysisTaskEmcalFjetSparseMaker.h";
  plugin->SetAdditionalLibs(sAddLibs.Data());

  TString sIncPath = "-Wno-deprecated";
  sIncPath += " -I$ALICE_ROOT/include";
  sIncPath += " -I$ALICE_PHYSICS/include";
  plugin->AddIncludePath(sIncPath.Data());

  TString sAnaSrc = "AliAnalysisTaskEmcalFjetSparseMaker.cxx";
  plugin->SetAnalysisSource(sAnaSrc.Data());
//=============================================================================

//plugin->SetDefaultOutputs("AliAOD.root,AliAOD.VertexingHF.root@ALICE::TORINO::DPM");
//plugin->SetMergeExcludes("AliAOD.root");
//plugin->SetOutputFiles("muonsHF.root@disk=1 AliAOD.MuonsHF.root@disk=1");
//plugin->SetOutputArchive("log_archive.zip:stdout,stderr@disk=1");
//=============================================================================

  plugin->SetAnalysisMacro(Form("%s.C",taskName.Data()));
  plugin->SetSplitMaxInputFileNumber(20);
//plugin->SetMaxInitFailed(5);
//plugin->SetMasterResubmitThreshold(90);
  plugin->SetTTL(30000);
  plugin->SetInputFormat("xml-single");
  plugin->SetJDLName(Form("%s.jdl",taskName.Data()));
  plugin->SetExecutable(Form("%s.sh",taskName.Data()));
  plugin->SetPrice(1);
  plugin->SetSplitMode("se");
  return plugin;
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
