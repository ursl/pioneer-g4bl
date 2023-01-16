void pion2Production() {

  // -- dual pion production
  double x2[] = {176, 221, 250, 284, 293, 320, 350, 352, 398};
  double y2[] = {420, 210, 183, 178, 172, 175, 180, 173, 181}; 

  // -- 500/0.001
  double r0x = 115;
  double r0y = 455.;

  // -- 10000/0.001
  double r1x = 399;
  double r1y = 455.;

  // -- 500/10.
  double r2x = 115;
  double r2y = 103.;

  double scalex = (4 - 2.7)/(r1x - r0x);
  double scaley = (3 + 1)/(r2y - r0y);

  cout << "scalex = " << scalex << endl;
  cout << "scaley = " << scaley << endl;
  
  double E[9];
  double xs[9];
  TGraph* gr = new TGraph();
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(2);
    
  for (int i = 0; i < 9; ++i) {
    double coordx = (x2[i]-r0x)*scalex + 2.7;
    double coordy = (y2[i] - r0y)*scaley - 3;
    E[i]  = TMath::Power(10., coordx);
    xs[i] = TMath::Power(10., coordy);

    gr->AddPoint(E[i], xs[i]);

    cout << coordx << " " 
         << E[i] << "  y: " 
         << coordy << " " 
         << xs[i]
         << endl;

  }

  TH1F *h1 = gr->GetHistogram();
  h1->SetMinimum(1.e-4);
  h1->SetMaximum(100.);
  h1->GetXaxis()->SetTitle("E_{p} [MeV]");
  h1->GetXaxis()->SetTitleOffset(1.5);
  h1->GetYaxis()->SetTitle("Cross section [mb]");

  shrinkPad(0.13, 0.15);
  c0.SetLogx(1);
  c0.SetLogy(1);
  gr->Draw("ap");

}


// ----------------------------------------------------------------------
void pion1Production() {

  // -- single pion to p production
  double x1[] = {};
  double y1[] = {};

  // -- single pion to n production
  double x2[] = {};
  double y2[] = {};


  // -- 500/0.001
  double r0x = 115;
  double r0y = 455.;

  // -- 10000/0.001
  double r1x = 399;
  double r1y = 455.;

  // -- 500/10.
  double r2x = 115;
  double r2y = 103.;

  double scalex = (4 - 2.7)/(r1x - r0x);
  double scaley = (3 + 1)/(r2y - r0y);

  cout << "scalex = " << scalex << endl;
  cout << "scaley = " << scaley << endl;
  
  double E[9];
  double xs[9];
  TGraph* gr = new TGraph();
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(2);
    
  for (int i = 0; i < 9; ++i) {
    double coordx = (x2[i]-r0x)*scalex + 2.7;
    double coordy = (y2[i] - r0y)*scaley - 3;
    E[i]  = TMath::Power(10., coordx);
    xs[i] = TMath::Power(10., coordy);

    gr->AddPoint(E[i], xs[i]);

    cout << coordx << " " 
         << E[i] << "  y: " 
         << coordy << " " 
         << xs[i]
         << endl;

  }

  TH1F *h1 = gr->GetHistogram();
  h1->SetMinimum(1.e-4);
  h1->SetMaximum(100.);
  h1->GetXaxis()->SetTitle("E_{p} [MeV]");
  h1->GetXaxis()->SetTitleOffset(1.5);
  h1->GetYaxis()->SetTitle("Cross section [mb]");

  shrinkPad(0.13, 0.15);
  c0.SetLogx(1);
  c0.SetLogy(1);
  gr->Draw("ap");

}
