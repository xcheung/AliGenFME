void AnaFitEvH()
{
  TFile *file = TFile::Open("AnalysisResults_EmcalEvH.root", "READ");
  TH2D *hCentMtCh = (TH2D*)file->Get("hCentMtCh"); hCentMtCh->SetDirectory(0);
  TH2D *hMtChMtEm = (TH2D*)file->Get("hMtChMtEm"); hMtChMtEm->SetDirectory(0);
  file->Close();

  hCentMtCh->FitSlicesY();
  TH1D *hCentMtChVal = (TH1D*)gDirectory->Get(Form("%s_1",hCentMtCh->GetName())); hCentMtChVal->SetName("hCentMtChVal");
  TH1D *hCentMtChRMS = (TH1D*)gDirectory->Get(Form("%s_2",hCentMtCh->GetName())); hCentMtChRMS->SetName("hCentMtChRMS");

  hMtChMtEm->FitSlicesY();
  TH1D *hMtChMtEmVal = (TH1D*)gDirectory->Get(Form("%s_1",hMtChMtEm->GetName())); hMtChMtEmVal->SetName("hMtChMtEmVal");
  TH1D *hMtChMtEmRMS = (TH1D*)gDirectory->Get(Form("%s_2",hMtChMtEm->GetName())); hMtChMtEmRMS->SetName("hMtChMtEmRMS");
//=============================================================================

  hCentMtChVal->Fit("pol5", "R0", "", 0., 91.);
  hCentMtChRMS->Fit("pol4", "R0", "", 0., 91.);

  hMtChMtEmVal->Fit("pol3", "R0", "", 0., 2500.);
//hCentMtChRMS->Fit("pol4", "R", "", 0., 90.);

/*TFile *file = TFile::Open("AnalysisResults_EmcalFit.root", "NEW");
  hCentMtChVal->Write();
  hCentMtChRMS->Write();
  hMtChMtEmVal->Write();
  hMtChMtEmRMS->Write();
  file->Close();*/

  return;
}
