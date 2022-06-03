// ----------------------------------------------------------------------
double dedx(double p, int id) {
  double m(0.);
  double mel = 0.5109989461;
  double mmu = 105.6583745;
  double mpi = 139.57039;
  double mpr = 938.272081;
  double c = 299799.e3;

  
  if (211 == TMath::Abs(id)) {
    m  = mpi;
  } else  if (13 == TMath::Abs(id)) {
    m = mmu;
  } else  if (2212 == TMath::Abs(id)) {
    m = mpr;
  } else  if (11 == TMath::Abs(id)) {
    m = mel;
  }

  double beta  = p/TMath::Sqrt(p*p + m*m);
  double gamma = 1./TMath::Sqrt(1. - beta*beta);

  cout << "beta = " << beta << " gamma = " << gamma << endl;
  
  // -- scintillator: vinyltoluene based
  //    https://www.physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=216
  // double I = 64.700000; //eV
  // double Z = 1*0.085000 + 6*0.915000;
  // double A = 12;

  // -- copper
  double I = 322; //eV
  double Z = 29.;
  double A = 63.5;

  double K = 0.307075; // MeV / mol * cm2
  
  double Wmax = (2*mel*c*c*beta*beta*gamma*gamma)/
    (1. + (2*gamma*mel/m) + (mel/m)*(mel/m));

  double z = 1.;
  
  double result = K*z*z*(Z/A)*(1/(beta*beta))*0.5*
    TMath::Log((2*mel*c*c*beta*beta*gamma*gamma*Wmax)/(I*I) - beta*beta);

  return result;
 
}


// ----------------------------------------------------------------------
void scaling(double p1, double p2, int id = 211) {

  double m(0.);
  double mel = 0.5109989461;
  double mmu = 105.6583745;
  double mpi = 139.57039;
  double mpr = 938.272081;
  double c = 299799.e3;

  
  if (211 == TMath::Abs(id)) {
    m  = mpi;
  } else  if (13 == TMath::Abs(id)) {
    m = mmu;
  } else  if (2212 == TMath::Abs(id)) {
    m = mpr;
  } else  if (11 == TMath::Abs(id)) {
    m = mel;
  }

  double beta1 = p1/TMath::Sqrt(p1*p1 + m*m);
  double beta2 = p2/TMath::Sqrt(p2*p2 + m*m);

  cout << id << " dE/dx scaling from p1 = " << p1 << " to p2 = " << p2
       << " from " << TMath::Power(beta2, -1.4)/TMath::Power(beta1, -1.4)
       << " to " << TMath::Power(beta2, -1.7)/TMath::Power(beta1, -1.7) << endl;
  
}
