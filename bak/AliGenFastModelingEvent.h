#ifndef ALIGENFASTSAMPLINGEVENT_H
#define ALIGENFASTSAMPLINGEVENT_H

#include <TNamed.h>
//=============================================================================

class TF1;
class TH1D;
class TH2D;
class TString;

class AliGenFastModelingEvent : public TNamed {

 public :

  AliGenFastModelingEvent(const char *name);
  ~AliGenFastModelingEvent();

  Bool_t InitEvent();
  Bool_t Init(const Double_t dCentMin, const Double_t dCentMax, const TString sPath);

  Double_t GetCentrality()       const { return fCent;    }
  Int_t GetTrackMultiplicity()   const { return fTrkMult; }
  Int_t GetClusterMultiplicity() const { return fCluMult; }

  Bool_t GetTrackPtEtaPhi(Double_t &dPt, Double_t &dEta, Double_t &dPhi);
  Bool_t GetClusterPtEtaPhi(Double_t &dPt, Double_t &dEta, Double_t &dPhi);

 private :

  AliGenFastModelingEvent(const AliGenFastModelingEvent &);
  AliGenFastModelingEvent& operator=(const AliGenFastModelingEvent &);

  Double_t fCent;
  Double_t fCentMin;
  Double_t fCentMax;
  Int_t    fCentBin;

  TF1 *fFxnMtChMtEmVal;
  TF1 *fFxnCentMtChVal;
  TF1 *fFxnCentMtChRMS;

  Int_t fTrkMult;
  Int_t fCluMult;

  TH1D *fHistTrkPt[4];
  TH2D *fHistTrkPtEta[4];
  TH2D *fHistTrkPtPhi[4];

  TH1D *fHistCluPt[4];
  TH2D *fHistCluPtEta[4];
  TH2D *fHistCluPtPhi[4];

  TRandom3 *fRndm3;

  ClassDef(AliGenFastModelingEvent, 1)
};

#endif
