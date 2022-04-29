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

// -- map of beamline elements for which the positions should be read in 
map<string, double> gBeamlinePositions = {
  {"QSF41", -99.},
  {"QSF41", -99.},
  {"HSC41", -99.},
  {"QSF42", -99.},
  {"QSF43", -99.},
  {"FSH41", -99.},
  {"HSC42", -99.},
  {"QSF44", -99.},
  {"FS42H", -99.},
  {"FS42V", -99.},
  {"ColPiE5", -99.},
  {"QSF45", -99.},
  {"HSC43", -99.},
  {"FSH43", -99.},
  {"QSF46", -99.},
  {"QSF47", -99.},
  {"HSC44", -99.},
  {"QSF48", -99.},  

  {"zAST41", -99.},  
  {"frontarcAST41", -99.},  
  //  {"flangeASTASC", -99.},  
  {"zASC41", -99.},  
  //  {"frontarcASC41", -99.},  

  {"QSB41", -99.},  
  {"QSB42", -99.},  
  {"QSB43", -99.},  
  {"SEP41", -99.},  
  {"QSK41", -99.},  
  {"QSK42", -99.},  
  {"QSK43", -99.},  
  {"PILL1", -99.},  
  {"SML41", -99.},  
  {"MEGCOL", -99.},  
  {"zASL41", -99.},  
  //  {"frontarcASL41", -99.},  
  // {"QSO41", -99.},  
  // {"QSO42", -99.},  
  // {"zASK41", -99.}  
};


// ----------------------------------------------------------------------
void drawPS2(double meanX, double meanY, double sigma1, double sigma2, double rho) {
  TEllipse e; 
  e.SetLineColor(kBlue); 
  e.SetLineWidth(2); 
  e.SetFillStyle(0); 

  // double phi     = 0.5*TMath::ATan((2*rho*sigmaX*sigmaY)/(sigmaX*sigmaX - sigmaY*sigmaY));
  // double cos2phi = TMath::Cos(phi)*TMath::Cos(phi);
  // double sin2phi = TMath::Sin(phi)*TMath::Sin(phi);

  //  double a1 = sigma1*sigma1*sin2a
  


  
  // e.DrawEllipse(meanX, meanY, a, b, 0., 360., phi);
}


// ----------------------------------------------------------------------
void drawPS1(double meanX, double meanY, double sigmaX, double sigmaY, double rho) {
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
  e.DrawEllipse(meanX, meanY, a, b, 0., 360., phiDeg);
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

// ----------------------------------------------------------------------
void readPositions() {
  // -- find placement files
  string g4blpioneer = gSystem->Getenv("G4BLPIONEER");
  string g4blPositions = g4blpioneer + "/pie5/Positions.txt";
  cout << g4blPositions << endl;

  ifstream INS;
  string sline;
  INS.open(g4blPositions);
  while (getline(INS, sline)) {
    for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
      if (string::npos != sline.find(it->first)) {
        string sval = sline.substr(sline.find(it->first) + it->first.length() + 1);
        double dval = atof(sval.c_str());
        cout << "found " << it->first << " with sval ->" << sval << "<-"
             << " resulting in double = " << dval
             << endl;
        if (dval > 0.) {
          it->second = dval;
        }
      }
    }
  }
}


//----------------------------------------------------------------------
void markup(double y = 0., double ymin = -60.) {
  if (gBeamlinePositions["QSF41"] < 0.) readPositions();

  TLine  *pl = new TLine();
  pl->SetLineStyle(kDotted);
  pl->SetLineColor(kBlue+2);

  TLatex *tl = new TLatex();

  gPad->GetCanvas()->SetWindowSize(1200, 300); 
  
  tl->SetNDC(kFALSE);
  tl->SetTextAngle(45.);
  tl->SetTextSize(0.03);
  tl->SetTextColor(kBlack);
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    tl->DrawLatex(it->second, y, it->first.c_str());
    cout << "tl->DrawLatex(" << it->second << ", " << y << ", " << it->first.c_str() << ");" << endl;
    pl->DrawLine(it->second, ymin, it->second, 0.9*y);
  }

  pl->SetLineStyle(kDashed);
  pl->SetLineColor(kBlack);
  pl->DrawLine(0., 0., gBeamlinePositions["zASL41"], 0.);
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
  if (string::npos != vlines[0].find("g4beamline profile")) {
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
    varDouble.insert(make_pair(sline, new double));
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
    }
    t->Fill();
  }
  
  return t; 
}

// ----------------------------------------------------------------------
void anaProfile(string filename = "profile.txt") {
  if (string::npos != filename.find("~")) {
    string home = gSystem->Getenv("HOME");
    cout << "home ->" << home << "<-" << endl;
    filename.replace(filename.find("~"), 1, home);
    cout << "filename now ->" << filename << "<-" << endl;
  }
  string pdfname = filename; 
  if (string::npos != pdfname.rfind("/")) {
    pdfname.replace(pdfname.begin(), pdfname.begin() + pdfname.rfind("/") + 1, "");
  }

  if (string::npos != pdfname.find(".dat")) {
    pdfname.replace(pdfname.find(".dat"), 4, "");
  }
  if (string::npos != pdfname.find(".txt")) {
    pdfname.replace(pdfname.find(".txt"), 4, "");
  }
  cout << "pdfname: ->" << pdfname << "<-" << endl;
  
  gStyle->SetOptTitle(0);
  
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  
  TTree *t = fillTree(filename);
  t->Print();

  t->Draw("meanX:Z");
  TH2 *h2 = (TH2*)gPad->FindObject("htemp");
  cout << "h2: " << h2 << endl;
  if (h2) {
    cout << h2->GetMaximum() << endl;
    cout << h2->GetMinimum() << endl;
  }
  markup(75., -50.);
  c1->SaveAs(Form("%s-meanX.pdf", pdfname.c_str()));

  t->Draw("meanY:Z");
  h2 = (TH2*)gPad->FindObject("htemp");
  cout << "h2: " << h2 << endl;
  if (h2) {
    cout << h2->GetMaximum() << endl;
    cout << h2->GetMinimum() << endl;
  }
  markup(0.6, -0.6);
  c1->SaveAs(Form("%s-meanY.pdf", pdfname.c_str()));

  
    
}



// ----------------------------------------------------------------------
void drawPad(double x0 = -76., double y0 = -36., double x1 = 35., double y1 = 10.) {

  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
    
  TH2D *h2 = new TH2D("h2", "", 100., x0, x1, 100., y0, y1);

  h2->Draw("axis");
  gPad->SetGridx(1);
  gPad->SetGridy(1);


  
  //  drawPS(-21.7, -13.0, eps, beta, phi);
  
}
