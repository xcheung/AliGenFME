#include <TH2D.h>
#include <TVector2.h>
#include <THnSparse.h>
#include <TLorentzVector.h>

#include "AliLog.h"
#include "AliVTrack.h"
#include "AliVCluster.h"
#include "AliAODCaloCluster.h"
#include "AliESDCaloCluster.h"
#include "AliPicoTrack.h"
#include "AliEmcalJet.h"
#include "AliRhoParameter.h"
#include "AliJetContainer.h"
#include "AliParticleContainer.h"
#include "AliClusterContainer.h"

#include "AliVEventHandler.h"
#include "AliAnalysisManager.h"

#include "AliAnalysisTaskEmcalFjetSparseMaker.h"

ClassImp(AliAnalysisTaskEmcalFjetSparseMaker)

//_____________________________________________________________________________
AliAnalysisTaskEmcalFjetSparseMaker::AliAnalysisTaskEmcalFjetSparseMaker() :
AliAnalysisTaskEmcalJet(),
fMultCh(0.),
fMultEm(0.),
fContTrks(0),
fContClus(0),
fListEvH(0),
fListTrk(0),
fListClu(0)
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::AliAnalysisTaskEmcalFjetSparseMaker
//
}

//_____________________________________________________________________________
AliAnalysisTaskEmcalFjetSparseMaker::AliAnalysisTaskEmcalFjetSparseMaker(const char *name, const Bool_t bHistos) : 
AliAnalysisTaskEmcalJet(name, bHistos),
fMultCh(0.),
fMultEm(0.),
fContTrks(0),
fContClus(0),
fListEvH(0),
fListTrk(0),
fListClu(0)
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::AliAnalysisTaskEmcalFjetSparseMaker
//

  SetMakeGeneralHistograms(bHistos);

  DefineOutput(2, TList::Class());
  DefineOutput(3, TList::Class());
  DefineOutput(4, TList::Class());
}

//_____________________________________________________________________________
AliAnalysisTaskEmcalFjetSparseMaker::~AliAnalysisTaskEmcalFjetSparseMaker()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::~AliAnalysisTaskEmcalFjetSparseMaker
//

  if (fContTrks) { delete fContTrks; fContTrks = 0; }
  if (fContClus) { delete fContClus; fContClus = 0; }

  if (fListEvH) { delete fListEvH; fListEvH = 0; }
  if (fListTrk) { delete fListTrk; fListTrk = 0; }
  if (fListClu) { delete fListClu; fListClu = 0; }
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::Init()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::Init
//

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::UserCreateOutputObjects()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::UserCreateOutputObjects
//

  AliAnalysisTaskEmcalJet::UserCreateOutputObjects();
//=============================================================================

  fContTrks = GetParticleContainer(0); if (fContTrks) fContTrks->SetClassName("AliVTrack");
  fContClus = GetClusterContainer(0);  if (fContClus) fContClus->SetClassName("AliVCluster");
//=============================================================================

  if (fListEvH) { delete fListEvH; fListEvH = 0; } fListEvH = new TList();
  if (fListTrk) { delete fListTrk; fListTrk = 0; } fListTrk = new TList();
  if (fListClu) { delete fListClu; fListClu = 0; } fListClu = new TList();

  Bool_t bStatusTmpH = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  MakeSparseEveH();
  if (fContTrks) MakeSparseTrks();
  if (fContClus) MakeSparseClus();

  TH1::AddDirectory(bStatusTmpH);
//=============================================================================

  PostData(2, fListEvH);
  PostData(3, fListTrk);
  PostData(4, fListClu);
//=============================================================================

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::Terminate(Option_t *opt)
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::Terminate
//

  AliAnalysisTaskEmcalJet::Terminate(opt);

  return;
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskEmcalFjetSparseMaker::Run()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::Run
//

  if (fContTrks) FillSparseTrks();
  if (fContClus) FillSparseClus();
//=============================================================================

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskEmcalFjetSparseMaker::FillHistograms()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::FillHistograms
//

  if (!AliAnalysisTaskEmcalJet::FillHistograms()) return kFALSE;
//=============================================================================

  FillSparseEveH();
//=============================================================================

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskEmcalFjetSparseMaker::FillGeneralHistograms()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::FillGeneralHistograms
//

  if (!AliAnalysisTaskEmcalJet::FillGeneralHistograms()) return kFALSE;

  return kTRUE;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::ExecOnce()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::ExecOnce
//

  AliAnalysisTaskEmcalJet::ExecOnce();
  if (!fInitialized) return;
//=============================================================================

  if (fContTrks) if (fContTrks->GetArray()==0) fContTrks = 0x0;
  if (fContClus) if (fContClus->GetArray()==0) fContClus = 0x0;

  return;
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskEmcalFjetSparseMaker::RetrieveEventObjects()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::RetrieveEventObjects
//

  if (!AliAnalysisTaskEmcalJet::RetrieveEventObjects()) return kFALSE;
//=============================================================================

  if (fContTrks) fMultCh = fContTrks->GetNAcceptedParticles();
  if (fContClus) fMultEm = fContClus->GetNAcceptedClusters();

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliAnalysisTaskEmcalFjetSparseMaker::IsEventSelected()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::IsEventSelected
//

  if (!AliAnalysisTaskEmcalJet::IsEventSelected()) return kFALSE;

  return kTRUE;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::FillSparseEveH()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::FillSparseEveH
//

  if (!fListEvH) return;
  TH2D *hCentMtCh = (TH2D*)fListEvH->FindObject(Form("hCentMtCh_%d",fCentBin)); if (!hCentMtCh) return;
  TH2D *hMtChMtEm = (TH2D*)fListEvH->FindObject(Form("hMtChMtEm_%d",fCentBin)); if (!hMtChMtEm) return;
//=============================================================================

  hCentMtCh->Fill(fCent,   fMultCh);
  hMtChMtEm->Fill(fMultCh, fMultEm);

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::FillSparseTrks()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::FillSparseTrks
//

  if (!fContTrks) return;

  TH1D *hPt = (TH1D*)fListTrk->FindObject(Form("hPt_%d",fCentBin));       if (!hPt)    return;
  TH2D *hPtEta = (TH2D*)fListTrk->FindObject(Form("hPtEta_%d",fCentBin)); if (!hPtEta) return;
  TH2D *hPtPhi = (TH2D*)fListTrk->FindObject(Form("hPtPhi_%d",fCentBin)); if (!hPtPhi) return;
//=============================================================================

  AliVTrack *pTrk  = static_cast<AliVTrack*>(fContTrks->GetNextAcceptParticle(0));

  while (pTrk) {
    Double_t dPt = pTrk->Pt();

    hPt->Fill(dPt);
    hPtEta->Fill(dPt, pTrk->Eta());
    hPtPhi->Fill(dPt, pTrk->Phi());

    pTrk = static_cast<AliVTrack*>(fContTrks->GetNextAcceptParticle());
  }
//=============================================================================

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::FillSparseClus()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::FillSparseClus
//

  if (!fContClus) return;

  TH1D *hPt = (TH1D*)fListClu->FindObject(Form("hPt_%d",fCentBin));       if (!hPt)    return;
  TH2D *hPtEta = (TH2D*)fListClu->FindObject(Form("hPtEta_%d",fCentBin)); if (!hPtEta) return;
  TH2D *hPtPhi = (TH2D*)fListClu->FindObject(Form("hPtPhi_%d",fCentBin)); if (!hPtPhi) return;
//=============================================================================

  AliVCluster *pClu = static_cast<AliVCluster*>(fContClus->GetNextAcceptCluster(0));

  while (pClu) {
    TLorentzVector vClu;
    pClu->GetMomentum(vClu, fVertex);
    Double_t dPt = vClu.Pt();

    hPt->Fill(dPt);
    hPtEta->Fill(dPt, vClu.Eta());
    hPtPhi->Fill(dPt, vClu.Phi());

    pClu = static_cast<AliVCluster*>(fContClus->GetNextAcceptCluster());
  }
//=============================================================================

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseEveH()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseEveH
//

  if (!fListEvH) return;
//=============================================================================

  const Int_t ncb = fNcentBins;

  TH2D *hCentMtCh[ncb];
  TH2D *hMtChMtEm[ncb];

  for (Int_t i=0; i<ncb; i++) {
    hCentMtCh[i] = new TH2D(Form("hCentMtCh_%d",i), "", 100, 0.,  100., 600, 0., 3000.); fListEvH->Add(hCentMtCh[i]);
    hMtChMtEm[i] = new TH2D(Form("hMtChMtEm_%d",i), "", 600, 0., 3000., 600, 0., 3000.); fListEvH->Add(hMtChMtEm[i]);
  }

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseTrks()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseTrks
//

  if (!fListTrk) return;
//=============================================================================

  const Int_t ncb = fNcentBins;
  const Double_t dPhiMax = TMath::TwoPi()   + TMath::Pi()/50.;

  TH1D *hPt[ncb];
  TH2D *hPtEta[ncb];
  TH2D *hPtPhi[ncb];
  for (Int_t i=0; i<ncb; i++) {
    hPt[i] = new TH1D(Form("hPt_%d",i), "", 500, 0., 50.); fListTrk->Add(hPt[i]);
    hPtEta[i] = new TH2D(Form("hPtEta_%d",i), "", 500., 0., 50., 200, -1., 1.); fListTrk->Add(hPtEta[i]);
    hPtPhi[i] = new TH2D(Form("hPtPhi_%d",i), "", 500., 0., 50., 101,  0., dPhiMax); fListTrk->Add(hPtPhi[i]);
  }
//=============================================================================

  return;
}

//_____________________________________________________________________________
void AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseClus()
{
//
//  AliAnalysisTaskEmcalFjetSparseMaker::MakeSparseClus
//

  if (!fListClu) return;
//=============================================================================

  const Int_t ncb = fNcentBins;
  const Double_t dPhiMax = TMath::TwoPi()   + TMath::Pi()/50.;

  TH1D *hPt[ncb];
  TH2D *hPtEta[ncb];
  TH2D *hPtPhi[ncb];
  for (Int_t i=0; i<ncb; i++) {
    hPt[i] = new TH1D(Form("hPt_%d",i), "", 500, 0., 50.); fListClu->Add(hPt[i]);
    hPtEta[i] = new TH2D(Form("hPtEta_%d",i), "", 500., 0., 50., 200, -1., 1.); fListClu->Add(hPtEta[i]);
    hPtPhi[i] = new TH2D(Form("hPtPhi_%d",i), "", 500., 0., 50., 101,  0., dPhiMax); fListClu->Add(hPtPhi[i]);
  }
//=============================================================================

  return;
}
