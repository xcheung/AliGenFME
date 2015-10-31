void AnalysisCINT(
  const TString sPath = "inputs",      // input data path
  const Int_t nEvents = 1000,          // # of events
  const Int_t dCentMin = 10.,          // centrality min
  const Int_t dCentMax = 20.,          // centrality max
  const Bool_t bUseBoltzmann = kTRUE,  // sample track/cluster pT from Boltzmann
  const Double_t dTrkMeanPt = 0.7,     // GeV/c, mean pT of tracks
  const Double_t dCluMeanPt = 0.7)     // GeV/c, mean pT of clusters
{
  if (LoadLibraries()) return;
//=============================================================================

// Jet finder in AliRoot

  AliEmcalJetFinder *pJetFinder = new AliEmcalJetFinder("EmcalJetFinder");
//pJetFinder->SetRecombSheme(1);  // default "BEST"
  pJetFinder->SetJetAlgorithm(1); // 0: kT, 1: anti-kT
  pJetFinder->SetRadius(0.4);     // jet radius
  pJetFinder->SetJetMinPt(0.1);   // jet min pT cut
//=============================================================================

// user defined outputs: histograms, THnSparse, ntuples...

  TFile *file = TFile::Open("AnalysisResults.root", "NEW");
  TList *list = new TList();

  TH1D *hCentrality = new TH1D("hCentrality", "", 100., 0., 100.); list->Add(hCentrality);

  TH1D *hTrkPt = new TH1D("hTrkPt", "", 200, 0., 100.); list->Add(hTrkPt);
  TH2D *hTrkEtaPhi = new TH2D("hTrkEtaPhi", "", 20, -1., 1., 100, 0., TMath::TwoPi()); list->Add(hTrkEtaPhi);

  TH1D *hCluPt = new TH1D("hCluPt", "", 200, 0., 100.); list->Add(hCluPt);
  TH2D *hCluEtaPhi = new TH2D("hCluEtaPhi", "", 20, -1., 1., 100, 0., TMath::TwoPi()); list->Add(hCluEtaPhi);
//=============================================================================

  AliGenFastModelingEvent *gen = new AliGenFastModelingEvent("AliGenFastModelingEvents");
  gen->SetCentralityRange(dCentMin, dCentMax);  // set centrality range
                                                // get track/cluster multiplicity + the fluctuations from the given cent range in LHC11h data
                                                // get track/cluster pT, eta, phi distribution from the given cent range in LHC11h data

/* optional: user defined track/cluster multiplicity ranges

  gen->SetMultiplicityUser(kTRUE);
  gen->SetTrackMultiplicityRangeUser(dUserTrkMultMin, dUserTrkMultMax);
  gen->SetClusterMultiplicityRangeUser(dUserCluMultMin, dUserCluMultMax); */

  gen->SetUseBoltzmann(bUseBoltzmann);  // sample track/cluster pT from bUseBoltzmann, otherwise it will be sampled from data
  gen->SetTrackMeanPt(dTrkMeanPt);      // mean pT of tracks
  gen->SetClusterMeanPt(dCluMeanPt);    // mean pT of clusters
//=============================================================================

  if (gen->Init(sPath)) { cout << "Init failed!!!" << endl; return; }
//=============================================================================

  for (Int_t iEvent=0; iEvent<nEvents; iEvent++) if (gen->InitEvent()) {
    hCentrality->Fill(gen->GetCentrality());

    for (Int_t it=0; it<gen->GetTrackMultiplicity(); it++) {
      Double_t dPt=0., dEta=0., dPhi=0.; if (gen->GetTrackPtEtaPhi(dPt,dEta,dPhi)) continue;

      hTrkPt->Fill(dPt);
      hTrkEtaPhi->Fill(dEta, dPhi);
    }

    for (Int_t ic=0; ic<gen->GetClusterMultiplicity(); ic++) {
      Double_t dPt=0., dEta=0., dPhi=0.; if (gen->GetClusterPtEtaPhi(dPt,dEta,dPhi)) continue;

      hCluPt->Fill(dPt);
      hCluEtaPhi->Fill(dEta, dPhi);
    }
  }
//=============================================================================

  file->cd();
  list->Write();
  file->Close();
//=============================================================================

  cout << "DONE" << endl;
//=============================================================================

  return;
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

  if (gSystem->Load("libSTEERBase")    <0) return kTRUE;
  if (gSystem->Load("libESD")          <0) return kTRUE;
  if (gSystem->Load("libAOD")          <0) return kTRUE;
  if (gSystem->Load("libANALYSIS")     <0) return kTRUE;
  if (gSystem->Load("libOADB")         <0) return kTRUE;
  if (gSystem->Load("libANALYSISalice")<0) return kTRUE;
  if (gSystem->Load("libCORRFW")       <0) return kTRUE;

  if (gSystem->Load("libCDB")           <0) return kTRUE;
  if (gSystem->Load("libRAWDatabase")   <0) return kTRUE;
  if (gSystem->Load("libRAWDatarec")    <0) return kTRUE;
  if (gSystem->Load("libSTEER")         <0) return kTRUE;
  if (gSystem->Load("libITSbase")       <0) return kTRUE;
  if (gSystem->Load("libITSrec")        <0) return kTRUE;
  if (gSystem->Load("libTPCbase")       <0) return kTRUE;
  if (gSystem->Load("libTPCrec")        <0) return kTRUE;
  if (gSystem->Load("libTRDbase")       <0) return kTRUE;
  if (gSystem->Load("libTRDrec")        <0) return kTRUE;
  if (gSystem->Load("libTOFbase")       <0) return kTRUE;
//if (gSystem->Load("libTOFrec")        <0) return kTRUE;
  if (gSystem->Load("libHMPIDbase")     <0) return kTRUE;
  if (gSystem->Load("libEMCALraw")      <0) return kTRUE;
  if (gSystem->Load("libEMCALbase")     <0) return kTRUE;
  if (gSystem->Load("libEMCALrec")      <0) return kTRUE;
  if (gSystem->Load("libVZERObase")     <0) return kTRUE;
  if (gSystem->Load("libVZEROrec")      <0) return kTRUE;
  if (gSystem->Load("libTender")        <0) return kTRUE;
  if (gSystem->Load("libTenderSupplies")<0) return kTRUE;

  if (gSystem->Load("libPWGTools")        <0) return kTRUE;
//if (gSystem->Load("libPWGflowBase")     <0) return kTRUE;
//if (gSystem->Load("libPWGflowTasks")    <0) return kTRUE;
//if (gSystem->Load("libPWGHFbase")       <0) return kTRUE;
//if (gSystem->Load("libPWGHFvertexingHF")<0) return kTRUE;

  if (gSystem->Load("libSTAT")      <0) return kTRUE;
  if (gSystem->Load("libEMCALUtils")<0) return kTRUE;
//if (gSystem->Load("libPHOSUtils") <0) return kTRUE;

  if (gSystem->Load("libCGAL")                 <0) return kTRUE;
  if (gSystem->Load("libfastjet")              <0) return kTRUE;
//if (gSystem->Load("libSISConePlugin")        <0) return kTRUE;
//if (gSystem->Load("libCDFConesPlugin")       <0) return kTRUE;
  if (gSystem->Load("libsiscone")              <0) return kTRUE;
  if (gSystem->Load("libsiscone_spherical")    <0) return kTRUE;
  if (gSystem->Load("libfastjetplugins")       <0) return kTRUE;
  if (gSystem->Load("libfastjettools")         <0) return kTRUE;
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

  if (gROOT->LoadMacro("AliGenFastModelingEvent.cxx+")<0) return kTRUE;
//=============================================================================

  return kFALSE;
}
