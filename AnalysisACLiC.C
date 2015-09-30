#include <iostream>

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/FastJet3.h"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
#include "fastjet/tools/Subtractor.hh"
#include "fastjet/tools/Filter.hh"

#include "TFile.h"
#include "TList.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

#include "TRandom3.h"
#include "TApplication.h"

#include "AliGenFastModelingEvent.h"
//=============================================================================

using namespace std;
using namespace Pythia8;
//=============================================================================

const TString srcName = "AnalysisACLiC";
//=============================================================================

int main(int argc, char* argv[])
{
  TApplication theApp(srcName.Data(), &argc, argv);
//=============================================================================

//TODO set paramters from arguments

  const Int_t nEvents = 1000;  // # of events

  const Int_t dCentMin = 10.;  // centrality min
  const Int_t dCentMax = 20.;  // centrality max

  const Bool_t bUseBoltzmann = kTRUE;  // sample track/cluster pT from Boltzmann
  const Double_t dTrkMeanPt = 0.7;  // GeV/c, mean pT of tracks
  const Double_t dCluMeanPt = 0.7;  // GeV/c, mean pT of clusters

  const TString sPath = "inputs"; // input data path
//=============================================================================

// jet finder, fastjet

  const double dJetsRange  = 0.4;
  const double dJetsPtMin  = 0.10;
  const double dPseuPtMin  = 0.15;
  const double dCutEtaMax  = 0.9;
  const double dJetEtaMax  = 0.5;
  const double dJetAreaRef = TMath::Pi() * dJetsRange * dJetsRange;

  fastjet::GhostedAreaSpec areaSpc(dCutEtaMax);
  fastjet::JetDefinition   jetsDef(fastjet::antikt_algorithm, dJetsRange, fastjet::BIpt_scheme, fastjet::Best);

//fastjet::AreaDefinition  areaDef(fastjet::active_area,areaSpc);
  fastjet::AreaDefinition  areaDef(fastjet::active_area_explicit_ghosts,areaSpc);

  fastjet::JetDefinition   bkgsDef(fastjet::kt_algorithm, 0.2, fastjet::BIpt_scheme, fastjet::Best);
  fastjet::AreaDefinition  aBkgDef(fastjet::active_area_explicit_ghosts, areaSpc);

  fastjet::Selector selectJet = fastjet::SelectorAbsEtaMax(dJetEtaMax);
  fastjet::Selector selectRho = fastjet::SelectorAbsEtaMax(dCutEtaMax-0.2);
  fastjet::Selector selecHard = fastjet::SelectorNHardest(2);
  fastjet::Selector selectBkg = selectRho * (!(selecHard));
  fastjet::JetMedianBackgroundEstimator bkgsEstimator(selectBkg, bkgsDef, aBkgDef);
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

  if (gen->Init(sPath)) { cout << "Init failed!!!" << endl; return -1; }
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

  return 0;
}
