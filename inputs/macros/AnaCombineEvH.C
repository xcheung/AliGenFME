void AnaCombineEvH()
{
  TFile *file = TFile::Open("AnalysisResults_EmcalEvH.root", "READ");
  TList *list = (TList*)file->Get("listEmcalEvH");
  file->Close();
//=============================================================================

  TH2D *hist2 = 0;
  TH2D *hCentMtCh = 0;
  TH2D *hMtChMtEm = 0;
  for (Int_t i=0; i<4; i++) {
    hist2 = (TH2D*)list->FindObject(Form("hCentMtCh_%d",i));
    if (i==0) { hCentMtCh = (TH2D*)hist2->Clone("hCentMtCh"); hCentMtCh->Reset(); } hCentMtCh->Add(hist2); hist2 = 0;

    hist2 = (TH2D*)list->FindObject(Form("hMtChMtEm_%d",i));
    if (i==0) { hMtChMtEm = (TH2D*)hist2->Clone("hMtChMtEm"); hMtChMtEm->Reset(); } hMtChMtEm->Add(hist2); hist2 = 0;
  }
//=============================================================================

/*file = TFile::Open("AnalysisResults_EmcalEvH.root", "UPDATE");
  hCentMtCh->Write();
  hMtChMtEm->Write();
  file->Close();*/
//=============================================================================

  return;
}
