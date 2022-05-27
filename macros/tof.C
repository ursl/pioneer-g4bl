double f_beta(double *x, double *par) {
  return x[0]/TMath::Sqrt(par[0]*par[0] + x[0]*x[0]);
}

// ----------------------------------------------------------------------
void beta() {
  
  double mel = 0.5109989461;
  double mmu = 105.6583745;
  double mpi = 139.57039;
  double mpr = 938.272081;

  gStyle->SetOptTitle(false);

  double pmin(25.), pmax(85.);
  TF1 *fpi = new TF1("betaPi", f_beta, pmin, pmax, 1); fpi->SetParameter(0, mpi);
  fpi->SetLineColor(kGreen+2); fpi->SetLineWidth(4);
  TF1 *fmu = new TF1("betaMu", f_beta, pmin, pmax, 1); fmu->SetParameter(0, mmu);
  fmu->SetLineColor(kBlue); fmu->SetLineWidth(4);
  TF1 *fpr = new TF1("betaPr", f_beta, pmin, pmax, 1); fpr->SetParameter(0, mpr);
  fpr->SetLineColor(kBlack); fpr->SetLineWidth(4);

  gPad->SetGridx(1);
  gPad->SetGridy(1);
  gPad->SetLeftMargin(0.15);
  fpi->Draw();
  fpi->SetMinimum(0.);
  fpi->SetMaximum(0.7);
  fpi->GetXaxis()->SetTitle("p [MeV]");
  fpi->GetYaxis()->SetTitleOffset(1.8);
  fpi->GetYaxis()->SetNdivisions(528, "Y");
  fpi->GetXaxis()->SetNdivisions(528, "Y");
  fpi->GetYaxis()->SetTitle("#beta = |p|/#sqrt{m^{2} + |p|^{2}}");
  fmu->Draw("same");
  fpr->Draw("same");

  TLegend *t = newLegend(0.2, 0.6, 0.4, 0.8);
  t->AddEntry(fpi, "Pions");
  t->AddEntry(fmu, "Muons");
  t->AddEntry(fpr, "Protons");
  t->Draw();
  c0.SaveAs("beta-pimupr.pdf");
}

// ----------------------------------------------------------------------
void tofp(double p = 28.0, double beamline = 18.0, int newline = 1) {
  // -- from PDG 2017
  double c = 2.99792458e8; // m/s
  double mel = 0.5109989461;
  double mmu = 105.6583745;
  double mpi = 139.57039;
  double mpr = 938.272081;

  double bel = p / TMath::Sqrt(mel*mel +p*p);
  double vel    = bel*c;

  double bpi = p / TMath::Sqrt(mpi*mpi +p*p);
  double vpi    = bpi*c;

  double bmu = p / TMath::Sqrt(mmu*mmu +p*p);
  double vmu    = bmu*c;

  double bpr = p / TMath::Sqrt(mpr*mpr +p*p);
  double vpr    = bpr*c;

  double tofel = 1.e9*beamline/vel;
  double tofmu = 1.e9*beamline/vmu;
  double tofpi = 1.e9*beamline/vpi;
  double tofpr = 1.e9*beamline/vpr;
  double tofc = 1.e9*beamline/c;

  double bunch = 19.750034;

  double rel = bunch - (tofel - (static_cast<int>(tofel/bunch))*bunch);
  double rmu = bunch - (tofmu - (static_cast<int>(tofmu/bunch))*bunch);
  double rpi = bunch - (tofpi - (static_cast<int>(tofpi/bunch))*bunch);
  double rpr = bunch - (tofpr - (static_cast<int>(tofpr/bunch))*bunch);


  //      ** = pi,mu,e separation of :  >=      5ns
  //       * = pi,mu,e separation of :  =      4ns

  string qual("  ");
  if ((TMath::Abs(rel - rmu) > 4) && (TMath::Abs(rel - rpi) > 4) && (TMath::Abs(rpi - rmu) > 4)) {
    qual = "*";
  }
  if ((TMath::Abs(rel - rmu) > 5) && (TMath::Abs(rel - rpi) > 5) && (TMath::Abs(rpi - rmu) > 5)) {
    qual = "**";
  }
  
  cout << Form("p = %4.1f(pi:%4.2f/mu:%4.2f/el:%4.2f) z = %5.2f pr=%5.1f pi=%5.1f mu=%5.1f el=%5.1f %3s",
               p, bpi, bmu, bel, beamline, rpr, rpi, rmu, rel, qual.c_str());
  if (1 == newline) {
    cout << endl;
  } else {
    cout << "    ////   "; 
  }

}


// ----------------------------------------------------------------------
void tofpscan(double pstart = 28., double beamline = 17.19) {
  for (int i = 0; i < 60; ++i) {
    tofp(pstart + i*0.5,  beamline, i%2);
  }
    
}

// ----------------------------------------------------------------------
void tofzscan(double p = 28., double beamline = 17.0) {
  for (int i = 0; i < 100; ++i) {
    tofp(p,  beamline + i*0.1, i%2);
  }
    
}
