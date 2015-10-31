#ifndef ALIGENFASTSAMPLINGEVENT_H
#define ALIGENFASTSAMPLINGEVENT_H

#include <TNamed.h>
//=============================================================================

class TF1;
class TH1D;
class TH2D;
class TString;
class TRandom3;

class AliGenFastModelingEvent : public TNamed {

 public :

  AliGenFastModelingEvent(const char *name);
  ~AliGenFastModelingEvent();
//=============================================================================

  Bool_t InitEvent();
  Bool_t Init(const TString sPath);
//=============================================================================

  void SetCentralityRange(const Double_t dMin, const Double_t dMax) { fCentMin = dMin; fCentMax = dMax; }
//=============================================================================

  void SetMultiplicityUser(const Bool_t b=kTRUE) { fUseMultUser = b; }
  void SetTrackMultiplicityRangeUser  (const Double_t dMin, const Double_t dMax) { fTrkMultUserMin = dMin; fTrkMultUserMax = dMax; }
  void SetClusterMultiplicityRangeUser(const Double_t dMin, const Double_t dMax) { fCluMultUserMin = dMin; fCluMultUserMax = dMax; }
//=============================================================================

  void SetUseBoltzmann (const Bool_t b=kTRUE) { fUseBoltzmann = b; }
  void SetTrackMeanPt  (const Double_t d=0.7) { fTrkMeanPt    = d; }
  void SetClusterMeanPt(const Double_t d=0.7) { fCluMeanPt    = d; }
//=============================================================================

  Double_t GetCentrality()       const { return fCent;    }
  Int_t GetTrackMultiplicity()   const { return fTrkMult; }
  Int_t GetClusterMultiplicity() const { return fCluMult; }
//=============================================================================

  Double_t GetTrackMeanPt()   const { return fTrkMeanPt; }
  Double_t GetClusterMeanPt() const { return fCluMeanPt; }
  
//=============================================================================

  Bool_t GetTrackPtEtaPhi (Double_t &dPt, Double_t &dEta, Double_t &dPhi);
  Bool_t GetClusterPtEtaPhi(Double_t &dPt, Double_t &dEta, Double_t &dPhi);
//=============================================================================

  Double_t GetTuneMeanPt() const { return fTuneMeanPt; }
  void     SetTuneMeanPt(Double_t v) {fTuneMeanPt = v;}
  Double_t GetTuneMult() const { return fTuneMult; }
  void     SetTuneMult(Double_t v) {fTuneMult = v;}

 private :

  AliGenFastModelingEvent(const AliGenFastModelingEvent &);
  AliGenFastModelingEvent& operator=(const AliGenFastModelingEvent &);

  Double_t Boltzmann(Double_t const *x,  Double_t const *par);
//=============================================================================

  Double_t fCentMin;
  Double_t fCentMax;
//=============================================================================

  Bool_t fUseMultUser;

  Double_t fTrkMultUserMin;
  Double_t fTrkMultUserMax;

  Double_t fCluMultUserMin;
  Double_t fCluMultUserMax;

  Double_t fTuneMult;
//=============================================================================

  Bool_t fUseBoltzmann;

  Double_t fTrkMeanPt;
  Double_t fCluMeanPt;

  Double_t fTuneMeanPt;
//=============================================================================

  Double_t fCent;
  Int_t    fCentBin;

  Int_t fTrkMult;
  Int_t fCluMult;

  TF1 *fFxnMtChMtEmVal;
  TF1 *fFxnCentMtChVal;
  TF1 *fFxnCentMtChRMS;

  TF1 *fTrkPtBoltzmann;
  TF1 *fCluPtBoltzmann;

  TH1D *fHistTrkPt[4];
  TH2D *fHistTrkPtEta[4];
  TH2D *fHistTrkPtPhi[4];

  TH1D *fHistCluPt[4];
  TH2D *fHistCluPtEta[4];
  TH2D *fHistCluPtPhi[4];

  TRandom3 *fRndm3;

  ClassDef(AliGenFastModelingEvent, 2)
};

#endif
