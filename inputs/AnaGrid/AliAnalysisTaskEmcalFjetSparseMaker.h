#ifndef ALIANALYSISTASKEMCALBKGSPARSEMAKER_H
#define ALIANALYSISTASKEMCALBKGSPARSEMAKER_H

#include <TString.h>

#include "AliAnalysisTaskEmcalJet.h"
//=============================================================================

class THnSparse;

class AliParticleContainer;
class AliClusterContainer;
//=============================================================================

class AliAnalysisTaskEmcalFjetSparseMaker : public AliAnalysisTaskEmcalJet {

 public :

  AliAnalysisTaskEmcalFjetSparseMaker();
  AliAnalysisTaskEmcalFjetSparseMaker(const char *name, const Bool_t bHistos=kTRUE);
  virtual ~AliAnalysisTaskEmcalFjetSparseMaker();
//=============================================================================

  virtual void Init();
  virtual void LocalInit() { Init(); }
  virtual void UserCreateOutputObjects();
  virtual void Terminate(Option_t *opt);
//=============================================================================

 protected :

  virtual void   ExecOnce();
  virtual Bool_t FillGeneralHistograms();
  virtual Bool_t FillHistograms();
  virtual Bool_t IsEventSelected();
  virtual Bool_t RetrieveEventObjects();
  virtual Bool_t Run();
//=============================================================================

 private :

  AliAnalysisTaskEmcalFjetSparseMaker(const AliAnalysisTaskEmcalFjetSparseMaker &);
  AliAnalysisTaskEmcalFjetSparseMaker &operator=(const AliAnalysisTaskEmcalFjetSparseMaker &);
//=============================================================================

  void MakeSparseEveH();
  void MakeSparseTrks();
  void MakeSparseClus();

  void FillSparseEveH();
  void FillSparseTrks();
  void FillSparseClus();
//=============================================================================

  Double_t fMultCh;
  Double_t fMultEm;
//=============================================================================

  TString fNameJet;

  AliParticleContainer *fContTrks;
  AliClusterContainer  *fContClus;

  TList *fListEvH;
  TList *fListTrk;
  TList *fListClu;
//=============================================================================

  ClassDef(AliAnalysisTaskEmcalFjetSparseMaker, 1)
};

#endif
