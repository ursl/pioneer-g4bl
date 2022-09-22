void pionProduction() {

  double x1[] = {176, 221, 250, 284, 293, 320, 350, 352, 398};
  double y1[] = {420, 210, 183, 178, 172, 175, 180, 173, 181}; 

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
  
  for (int i = 0; i < 9; ++i) {
    double coordx = (x1[i]-r0x)*scalex + 2.7;
    double coordy = (y1[i] - r0y)*scaley - 3;
    cout << coordx << " " 
         << TMath::Power(10., coordx) << "  y: " 
         << coordy << " " 
         << TMath::Power(10., coordy)
         << endl;


  }
}
