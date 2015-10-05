#include <iostream>

#include <TFile.h>
#include <TList.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TString.h>
#include <TRandom3.h>

#include "AliGenFastModelingEvent.h"

ClassImp(AliGenFastModelingEvent)

using namespace std;

//_____________________________________________________________________________
AliGenFastModelingEvent::AliGenFastModelingEvent(const char *name) :
TNamed(name, name),
fCentMin(-1.),
fCentMax(101.),
fUseMultUser(kFALSE),
fTrkMultUserMin(-1.),
fTrkMultUserMax(-1.),
fCluMultUserMin(-1.),
fCluMultUserMax(-1.),
fUseBoltzmann(kTRUE),
fTrkMeanPt(0.7),
fCluMeanPt(0.7),
fCent(-1.),
fCentBin(-1),
fTrkMult(-1),
fCluMult(-1),
fFxnMtChMtEmVal(0),
fFxnCentMtChVal(0),
fFxnCentMtChRMS(0),
fTrkPtBoltzmann(0),
fCluPtBoltzmann(0),
fRndm3(0)
{
//
//  AliGenFastModelingEvent::AliGenFastModelingEvent
//

  for (Int_t i=0; i<4; i++) {
    fHistTrkPt[i] = 0; fHistTrkPtEta[i] = 0; fHistTrkPtPhi[i] = 0;
    fHistCluPt[i] = 0; fHistCluPtEta[i] = 0; fHistCluPtPhi[i] = 0;
  }
}

//_____________________________________________________________________________
AliGenFastModelingEvent::~AliGenFastModelingEvent()
{
//
//  AliGenFastModelingEvent::~AliGenFastModelingEvent
//

  if (fTrkPtBoltzmann) { delete fTrkPtBoltzmann; fTrkPtBoltzmann = 0; }
  if (fCluPtBoltzmann) { delete fCluPtBoltzmann; fCluPtBoltzmann = 0; }

  if (fFxnMtChMtEmVal) { delete fFxnMtChMtEmVal; fFxnMtChMtEmVal = 0; }
  if (fFxnCentMtChVal) { delete fFxnCentMtChVal; fFxnCentMtChVal = 0; }
  if (fFxnCentMtChRMS) { delete fFxnCentMtChRMS; fFxnCentMtChRMS = 0; }

  for (Int_t i=0; i<4; i++) {
    if (fHistTrkPt[i])    { delete fHistTrkPt[i];    fHistTrkPt[i]    = 0; }
    if (fHistTrkPtEta[i]) { delete fHistTrkPtEta[i]; fHistTrkPtEta[i] = 0; }
    if (fHistTrkPtPhi[i]) { delete fHistTrkPtPhi[i]; fHistTrkPtPhi[i] = 0; }

    if (fHistCluPt[i])    { delete fHistCluPt[i];    fHistCluPt[i]    = 0; }
    if (fHistCluPtEta[i]) { delete fHistCluPtEta[i]; fHistCluPtEta[i] = 0; }
    if (fHistCluPtPhi[i]) { delete fHistCluPtPhi[i]; fHistCluPtPhi[i] = 0; }
  }

   if (fRndm3) { delete fRndm3; fRndm3 = 0; }
}

//_____________________________________________________________________________
Bool_t AliGenFastModelingEvent::Init(const TString sPath)
{
//
//  AliGenFastModelingEvent::Init
//

  if (sPath.IsNull()) { cout << "AliGenFastModelingEvent::Init Input data path is missing" << endl; return kTRUE; }
//=============================================================================

  if (fCentMin>fCentMax) {
    cout << "AliGenFastModelingEvent::Init cent min (=" << fCentMin << ") > cent max (=" << fCentMax << ")! Wrong inputs!" << endl;
    return kTRUE;
  }
//=============================================================================

  if (fUseMultUser) {
    cout << "AliGenFastModelingEvent::Init Use user defined multiplicity" << endl;

    if ((fTrkMultUserMin<0) || (fTrkMultUserMax<0) || (fTrkMultUserMax<fTrkMultUserMin)) {
      cout << "AliGenFastModelingEvent::Init trk mult min = " << fTrkMultUserMin
                                       << ", trk mult max = " << fTrkMultUserMax << ": setting is incorrect!!!" << endl;
      return kTRUE;
    }

    if ((fCluMultUserMin<0) || (fCluMultUserMax<0) || (fCluMultUserMax<fCluMultUserMin)) {
      cout << "AliGenFastModelingEvent::Init clu mult min = " << fCluMultUserMin
                                       << ", clu mult max = " << fCluMultUserMax << ": setting is incorrect!!!" << endl;
      return kTRUE;
    }
  }
//=============================================================================

  TFile *file = 0;
  TList *list = 0;

  fRndm3 = new TRandom3(0);
//=============================================================================

  if (!fUseMultUser) {
    file = TFile::Open(Form("%s/AnalysisResults_EmcalFit.root",sPath.Data()), "READ");
    TH1D *hMtChMtEmVal = (TH1D*)file->Get("hMtChMtEmVal"); hMtChMtEmVal->SetDirectory(0);
    TH1D *hCentMtChVal = (TH1D*)file->Get("hCentMtChVal"); hCentMtChVal->SetDirectory(0);
    TH1D *hCentMtChRMS = (TH1D*)file->Get("hCentMtChRMS"); hCentMtChRMS->SetDirectory(0);
    file->Close();

    TF1 *fxnMtChMtEmVal = (TF1*)hMtChMtEmVal->GetListOfFunctions()->At(0); fxnMtChMtEmVal->SetName("fxnMtChMtEmVal"); hMtChMtEmVal = 0;
    TF1 *fxnCentMtChVal = (TF1*)hCentMtChVal->GetListOfFunctions()->At(0); fxnCentMtChVal->SetName("fxnCentMtChVal"); hCentMtChVal = 0;
    TF1 *fxnCentMtChRMS = (TF1*)hCentMtChRMS->GetListOfFunctions()->At(0); fxnCentMtChRMS->SetName("fxnCentMtChRMS"); hCentMtChRMS = 0;

    fFxnMtChMtEmVal = new TF1(*fxnMtChMtEmVal); fFxnMtChMtEmVal->SetName("fFxnMtChMtEmVal"); fxnMtChMtEmVal = 0; if (!fFxnMtChMtEmVal) return kTRUE;
    fFxnCentMtChVal = new TF1(*fxnCentMtChVal); fFxnCentMtChVal->SetName("fFxnCentMtChVal"); fxnCentMtChVal = 0; if (!fFxnCentMtChVal) return kTRUE;
    fFxnCentMtChRMS = new TF1(*fxnCentMtChRMS); fFxnCentMtChRMS->SetName("fFxnCentMtChRMS"); fxnCentMtChRMS = 0; if (!fFxnCentMtChRMS) return kTRUE;
  }
//=============================================================================

  if (fUseBoltzmann) {
    cout << "AliGenFastModelingEvent::Init Sample trk/clu pT from Boltzmann" << endl;
    if (fTrkMeanPt<0.) { cout << "AliGenFastModelingEvent::Init trk mean pT = " << fTrkMeanPt << ", is incorrect" << endl; return kTRUE; }
    if (fCluMeanPt<0.) { cout << "AliGenFastModelingEvent::Init clu mean pT = " << fTrkMeanPt << ", is incorrect" << endl; return kTRUE; }

    fTrkPtBoltzmann = new TF1("fTrkPtBoltzmann", this, &AliGenFastModelingEvent::Boltzmann, 0.15, 200., 1, "AliGenFastModelingEvent", "Boltzmann");
    fTrkPtBoltzmann->SetParameter(0, fTrkMeanPt);
    fTrkPtBoltzmann->SetNpx(2000);

    fCluPtBoltzmann = new TF1("fCluPtBoltzmann", this, &AliGenFastModelingEvent::Boltzmann, 0.30, 200., 1, "AliGenFastModelingEvent", "Boltzmann");
    fCluPtBoltzmann->SetParameter(0, fCluMeanPt);
    fCluPtBoltzmann->SetNpx(2000);
  }
//=============================================================================

  file = TFile::Open(Form("%s/AnalysisResults_EmcalTrk.root",sPath.Data()), "READ");
  list = (TList*)file->Get("listEmcalTrk");
  file->Close();

  TH1D *histo = 0;
  TH2D *hist2 = 0;
  for (Int_t i=0; i<4; i++) {
    hist2 = (TH2D*)list->FindObject(Form("hPtEta_%d",i)); hist2->SetName(Form("hTrkPtEta_%d",i));
    fHistTrkPtEta[i] = new TH2D(*hist2); fHistTrkPtEta[i]->SetName(Form("fHistTrkPtEta_%d",i)); hist2 = 0;
    if (!fHistTrkPtEta[i]) return kTRUE;

    hist2 = (TH2D*)list->FindObject(Form("hPtPhi_%d",i)); hist2->SetName(Form("hTrkPtPhi_%d",i));
    fHistTrkPtPhi[i] = new TH2D(*hist2); fHistTrkPtPhi[i]->SetName(Form("fHistTrkPtPhi_%d",i)); hist2 = 0;
    if (!fHistTrkPtPhi[i]) return kTRUE;

    if (!fUseBoltzmann) {
      histo = (TH1D*)list->FindObject(Form("hPt_%d",i)); histo->SetName(Form("hTrkPt_%d",i));
      fHistTrkPt[i] = new TH1D(*histo); fHistTrkPt[i]->SetName(Form("fHistTrkPt_%d",i)); histo = 0;
      if (!fHistTrkPt[i]) return kTRUE;
    }
  }
//=============================================================================

  file = TFile::Open(Form("%s/AnalysisResults_EmcalClu.root",sPath.Data()), "READ");
  list = (TList*)file->Get("listEmcalClu");
  file->Close();

  for (Int_t i=0; i<4; i++) {
    hist2 = (TH2D*)list->FindObject(Form("hPtEta_%d",i)); hist2->SetName(Form("hCluPtEta_%d",i));
    fHistCluPtEta[i] = new TH2D(*hist2); fHistCluPtEta[i]->SetName(Form("fHistCluPtEta_%d",i)); hist2 = 0;
    if (!fHistCluPtEta[i]) return kTRUE;

    hist2 = (TH2D*)list->FindObject(Form("hPtPhi_%d",i)); hist2->SetName(Form("hCluPtPhi_%d",i));
    fHistCluPtPhi[i] = new TH2D(*hist2); fHistCluPtPhi[i]->SetName(Form("fHistCluPtPhi_%d",i)); hist2 = 0;
    if (!fHistCluPtPhi[i]) return kTRUE;

    if (!fUseBoltzmann) {
      histo = (TH1D*)list->FindObject(Form("hPt_%d",i)); histo->SetName(Form("hCluPt_%d",i));
      fHistCluPt[i] = new TH1D(*histo); fHistCluPt[i]->SetName(Form("fHistCluPt_%d",i)); histo = 0;
      if (!fHistCluPt[i]) return kTRUE;
    }
  }
//=============================================================================

  return kFALSE;
}

//_____________________________________________________________________________
Bool_t AliGenFastModelingEvent::InitEvent()
{
//
//  AliGenFastModelingEvent::InitEvent
//

  fCent = fRndm3->Uniform(fCentMin, fCentMax);

  if ((fCent>= 0.) && (fCent < 10.)) fCentBin = 0;
  if ((fCent>=10.) && (fCent < 30.)) fCentBin = 1;
  if ((fCent>=30.) && (fCent < 50.)) fCentBin = 2;
  if ((fCent>=50.) && (fCent<=100.)) fCentBin = 3;
  if ((fCentBin<0) || (fCentBin>3)) return kFALSE;
//=============================================================================

  if (fUseMultUser) {
    fTrkMult = (Int_t)fRndm3->Uniform(fTrkMultUserMin, fTrkMultUserMax);
    fCluMult = (Int_t)fRndm3->Uniform(fCluMultUserMin, fCluMultUserMax);
  } else {
    Double_t dMultTrk = fFxnCentMtChVal->Eval(fCent>90. ? 90 : fCent);
    Double_t dMultRMS = fFxnCentMtChRMS->Eval(fCent>90. ? 90 : fCent);

    fTrkMult = (Int_t)fRndm3->Gaus(dMultTrk, dMultRMS);
    fCluMult = (Int_t)fFxnMtChMtEmVal->Eval(fTrkMult<10. ? 10. : fTrkMult);
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliGenFastModelingEvent::GetTrackPtEtaPhi(Double_t &dPt, Double_t &dEta, Double_t &dPhi)
{
//
//  AliGenFastModelingEvent::GetTrackPtEtaPhi
//

  if (fTrkMult<0) return kTRUE;
//=============================================================================

  if (fUseBoltzmann) {
    dPt = fTrkPtBoltzmann->GetRandom(0.15, 200.);
  } else {
    dPt = fHistTrkPt[fCentBin]->GetRandom();
  }
//=============================================================================

  Int_t kBinEta = fHistTrkPtEta[fCentBin]->GetXaxis()->FindBin(dPt);
  Int_t kBinPhi = fHistTrkPtPhi[fCentBin]->GetXaxis()->FindBin(dPt);

  TH1D *hTrkEta = fHistTrkPtEta[fCentBin]->ProjectionY("hTrkEta", kBinEta, kBinEta);
  TH1D *hTrkPhi = fHistTrkPtPhi[fCentBin]->ProjectionY("hTrkPhi", kBinPhi, kBinPhi);

  dEta = hTrkEta->GetRandom(); delete hTrkEta; hTrkEta = 0;
  dPhi = hTrkPhi->GetRandom(); delete hTrkPhi; hTrkPhi = 0;
//=============================================================================

  return kFALSE;
}

//_____________________________________________________________________________
Bool_t AliGenFastModelingEvent::GetClusterPtEtaPhi(Double_t &dPt, Double_t &dEta, Double_t &dPhi)
{
//
//  AliGenFastModelingEvent::GetClusterPtEtaPhi
//

  if (fCluMult<0) return kTRUE;
//=============================================================================

  if (fUseBoltzmann) {
    dPt = fCluPtBoltzmann->GetRandom(0.30, 200.);
  } else {
    dPt = fHistCluPt[fCentBin]->GetRandom();
  }
//=============================================================================

  Int_t kBinEta = fHistCluPtEta[fCentBin]->GetXaxis()->FindBin(dPt);
  Int_t kBinPhi = fHistCluPtPhi[fCentBin]->GetXaxis()->FindBin(dPt);

  TH1D *hCluEta = fHistCluPtEta[fCentBin]->ProjectionY("hCluEta", kBinEta, kBinEta);
  TH1D *hCluPhi = fHistCluPtPhi[fCentBin]->ProjectionY("hCluPhi", kBinPhi, kBinPhi);

  dEta = hCluEta->GetRandom(); delete hCluEta; hCluEta = 0;
  dPhi = hCluPhi->GetRandom(); delete hCluPhi; hCluPhi = 0;
//=============================================================================

  return kFALSE;
}

//_____________________________________________________________________________
Double_t AliGenFastModelingEvent::Boltzmann(Double_t const *x,  Double_t const *par)
{
//
// AliGenFastModelingEvent::Boltzmann
//

  double  dKernel = 2. * x[0] / par[0];
  return (dKernel * TMath::Exp(-1.*dKernel));
}
