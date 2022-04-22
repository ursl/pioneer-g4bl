#include <iostream>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------
// cd pioneer-g4bl/macros
// root
// root [0] .L anaProfile.C 
// root [1] anaProfile()
// root [2] profile->Draw("emitX:Z")
// ----------------------------------------------------------------------


// general ellipse equation
// ((x−x0)*cos(phi) + (y-y0))sin(phi))*((x−x0)*cos(phi) + (y-y0))sin(phi))/(a*a)
// +((x−x0)*sin(phi) - (y-y0)cos(phi))*((x−x0)*sin(phi) - (y-y0)cos(phi))/(b*b)
// = 1
//
// ellipse parameters in terms of sigmas and correlation
// tan(2*phi) = (2*rho*sigmaX*sigmaY)/(sigmaX*sigmaX - sigmaY*sigmaY)
// a          = ((cos2phi*sigmaX*sigmaX) - (sin2phi*sigmaY*sigmaY))/(cos2phi - sin2phi)
// b          = ((cos2phi*sigmaY*sigmaY) - (sin2phi*sigmaX*sigmaX))/(cos2phi - sin2phi)



// ----------------------------------------------------------------------
void drawPS(double meanX, double meanY, double sigmaX, double sigmaY, double rho) {
  TEllipse e; 
  e.SetLineColor(kBlue); 
  e.SetLineWidth(2); 
  e.SetFillStyle(0); 

  double phi     = 0.5*TMath::ATan((2*rho*sigmaX*sigmaY)/(sigmaX*sigmaX - sigmaY*sigmaY));
  double phiDeg  = phi*TMath::RadToDeg();
  cout << "phi = " << phiDeg << endl;
  cout << "tan(2phi) = " << TMath::Tan(2.*phi) << endl;
  cout << "calc:       " << (2.*rho*sigmaX*sigmaY)/(sigmaX*sigmaX - sigmaY*sigmaY) << endl;

  double cos2phi = TMath::Cos(phi)*TMath::Cos(phi);
  double sin2phi = TMath::Sin(phi)*TMath::Sin(phi);
  double a       = ((cos2phi*sigmaX*sigmaX) - (sin2phi*sigmaY*sigmaY))/(cos2phi - sin2phi);
  double b       = ((cos2phi*sigmaY*sigmaY) - (sin2phi*sigmaX*sigmaX))/(cos2phi - sin2phi);
  a = TMath::Sqrt(a);
  b = TMath::Sqrt(b);
  e.DrawEllipse(meanX, meanY, a, b, 0., 360., phi);
}


// ----------------------------------------------------------------------
void drawPS0(double mean, double meanPrime, double eps, double beta, double phi) {
  TEllipse e; 
  e.SetLineColor(kBlue); 
  e.SetLineWidth(2); 
  e.SetFillStyle(0); 
  // test1(600, 2.5, 15)
  //  e.DrawEllipse(mean, meanPrime, TMath::Sqrt(eps*beta), TMath::Sqrt(eps/beta), 0., 360., phi);
  e.DrawEllipse(mean, meanPrime, eps, beta, 0., 360., phi);
}



//----------------------------------------------------------------------
TTree* fillTree(string filename) {
  int VERBOSE(0);
  ifstream INS;
  vector<string> vlines; 
  string sline;
  INS.open(filename);
  while (getline(INS, sline)) {
    vlines.push_back(sline);
  }

  // -- parse beginning to set up tree
  int valFirstLine(1);
  string tname = vlines[0].substr(1); 
  if (string::npos != vlines[0].find("g4beamline")) {
    tname = vlines[0].substr(2); 
    valFirstLine = 2;
  }
  string dname = tname.substr(tname.find(" ") + 1); 

  vector<string> vars;
  map<string, double*> varDouble;
  map<string, int*> varInt;

  istringstream istring(vlines[1].substr(1)); 
  istring >> sline; 
  if (VERBOSE > 0) cout << "adding ";
  while (!istring.fail()) {
    if (VERBOSE > 0) cout << sline << " "; 
    vars.push_back(sline);
    if ((string::npos != sline.find("PDGid"))
        || (string::npos != sline.find("EventID"))
        || (string::npos != sline.find("TrackID"))
        || (string::npos != sline.find("ParentID"))
        ) {
      varInt.insert(make_pair(sline, new int));
    } else {
      varDouble.insert(make_pair(sline, new double));
    }
    istring >> sline; 
  }
  if (VERBOSE > 0) cout << endl;

  // -- set up tree  
  TTree *t = new TTree(dname.c_str(), tname.c_str());
  for (unsigned int i = 0; i < vars.size(); ++i) {
    if (VERBOSE > 1) cout << "vars[" << i << "] = " << vars[i] << ": "; 
    if (varDouble.end() != varDouble.find(vars[i])) {
      if (VERBOSE > 1) cout << varDouble[vars[i]] << " (double)";
      t->Branch(vars[i].c_str(), varDouble[vars[i]], Form("%s/D", vars[i].c_str()));
    }
    if (varInt.end() != varInt.find(vars[i])) {
      if (VERBOSE > 1) cout << varInt[vars[i]] << " (int)";
      t->Branch(vars[i].c_str(), varInt[vars[i]], Form("%s/I", vars[i].c_str()));
    }
    if (VERBOSE > 1) cout << endl;
  }


  // -- read rest of file and fill into tree
  for (unsigned int il = valFirstLine; il < vlines.size(); ++il) {
    if (VERBOSE > 9) cout << "line " << il << endl;
    istringstream values(vlines[il]); 
    for (unsigned int i = 0; i < vars.size(); ++i) {
      if (varDouble.end() != varDouble.find(vars[i])) {
        values >> *varDouble[vars[i]]; 
        if (VERBOSE > 9) cout << "parsed " << vars[i] << " = " << *varDouble[vars[i]] << endl;
      }
      if (varInt.end() != varInt.find(vars[i])) {
        values >> *varInt[vars[i]]; 
        if (VERBOSE > 9) cout << "parsed " << vars[i] << " = " << *varInt[vars[i]] << endl;
      }
    }
    t->Fill();
  }
  
  return t; 
}

// ----------------------------------------------------------------------
void anaProfile(string filename = "profile.txt") {
  TTree *t = fillTree(filename);
  t->Print();

  t->Draw("meanX:Z");
}


// ----------------------------------------------------------------------
void drawPad(double x = 100., double y = 50.) {

  gStyle->SetOptStat(0);
  
  TH2D *h2 = new TH2D("h2", "", 100., -x, x, 100., -y, y);

  h2->Draw("axis");
  gPad->SetGridx(1);
  gPad->SetGridy(1);

  //  drawPS(-21.7, -13.0, eps, beta, phi);
  
}
