#include <cstdarg>

void rangeScan(string pdfname, int wdb, int n, ...) {
  va_list vl;
  va_start(vl, n);
  vector<int> vrun;
  int a(0);
  for (int i = 0; i < n; ++i) {
    a = va_arg(vl, int);
    vrun.push_back(a);
  }
  va_end(vl);

  gROOT->ForceStyle();

  int col[] = {kCyan, kCyan-1, kBlue+2, kYellow+2, kGreen+1, kRed+1, kMagenta};

  TLegend *leg = newLegend(0.6, 0.5, 0.95, 0.89);
  
  TFile *f(0); 
  gStyle->SetOptStat(0);
  vector<TH1F*> vhist;
  for (unsigned int i = 0; i < vrun.size(); ++i) {
    cout << "run " << vrun[i] << endl;
    string filename = Form("../../2022/run%d-WD0%d_0_histos.root", vrun[i], wdb);
    cout << "==> " << filename << endl;
    f = TFile::Open(filename.c_str());
    TH1F *h = (TH1F*)f->Get("hAvT_35_5_py");
    vhist.push_back((TH1F*)h->Clone());
    vhist[i]->SetDirectory(gROOT);
    vhist[i]->SetLineColor(col[i]);
    vhist[i]->SetLineWidth(3);
    if (vhist[i]->GetMaximum() > vhist[0]->GetMaximum()) {
      vhist[0]->SetMaximum(1.2*vhist[i]->GetMaximum());
    }
    if (i > 0) {
      vhist[i]->DrawCopy("same");
    } else {
      vhist[i]->DrawCopy("");
    }
    leg->AddEntry(vhist[i], Form("Run %d", vrun[i]), "l");
    f->Close();
  }
  c0.Clear();
  gStyle->SetOptTitle(0);
  
  for (unsigned int i = 0; i < vrun.size(); ++i) {
    if (i > 0) {
      vhist[i]->DrawCopy("same");
    } else {
      vhist[i]->DrawCopy("");
    }
  }
  leg->Draw();
  c0.SaveAs(pdfname.c_str());
  string pngname(pdfname);
  replaceAll(pngname, ".pdf", ".png");
  c0.SaveAs(pngname.c_str());
}
