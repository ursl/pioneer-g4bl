
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
  
  cout << Form("p = %4.1f z = %5.2f pr=%5.1f pi=%5.1f mu=%5.1f el=%5.1f %3s",
               p, beamline, rpr, rpi, rmu, rel, qual.c_str());
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
