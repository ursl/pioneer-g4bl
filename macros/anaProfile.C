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

// // -- map of beamline elements for which the positions should be read in 
// map<string, double> gBeamlinePositions = {
//   {"QSF41", -9999.},
//   {"QSF41", -9999.},
//   {"HSC41", -9999.},
//   {"QSF42", -9999.},
//   {"QSF43", -9999.},
//   {"FSH41", -9999.},
//   {"HSC42", -9999.},
//   {"QSF44", -9999.},
//   {"FS42H", -9999.},
//   {"FS42V", -9999.},
//   {"ColPiE5", -9999.},
//   {"QSF45", -9999.},
//   {"HSC43", -9999.},
//   {"FSH43", -9999.},
//   {"QSF46", -9999.},
//   {"QSF47", -9999.},
//   {"HSC44", -9999.},
//   {"QSF48", -9999.},  

//   {"zAST41", -9999.},  
//   //  {"frontarcAST41", -9999.},  
//   //  {"flangeASTASC", -9999.},  
//   {"zASC41", -9999.},  
//   //  {"frontarcASC41", -9999.},  

//   {"QSB41", -9999.},  
//   {"QSB42", -9999.},  
//   {"QSB43", -9999.},  
//   {"SEP41", -9999.},  
//   {"QSK41", -9999.},  
//   {"QSK42", -9999.},  
//   {"QSK43", -9999.},  
//   {"PILL1", -9999.},  
//   {"SML41", -9999.},  
//   {"MEGCOL", -9999.},  
//   {"zASL41", -9999.},  
//   //  {"frontarcASL41", -9999.},  
//   // {"QSO41", -9999.},  
//   // {"QSO42", -9999.},  
//   // {"zASK41", -9999.}  
// };

// -- struct for beamline element display
struct bl {
  double z;
  Color_t col;
  double tsiz;
};

// -- quadrupoles
double  qsiz(0.05);
Color_t qcol(kRed);

// -- sextupoles
double  ssiz(0.05);
Color_t scol(kYellow+2);

// -- dipoles
double  dsiz(0.05);
Color_t dcol(kBlue+1);

// -- AST/ASC dipoles
double  dassiz(0.05);
Color_t dascol(kBlue+2);

// -- separators
double  sesiz(0.05);
Color_t secol(kBlue+4);

// -- collimators/slits
double  csiz(0.05);
Color_t ccol(kGreen+2);

// -- stuff
double  stusiz(0.05);
Color_t stucol(kGray+2);

// -- det
double  detsiz(0.05);
Color_t detcol(kOrange-3);

// -- map of beamline elements for which the positions should be read in 
map<string, struct bl> gBeamlinePositions = {
  {"QSF41", {-9999., qcol, qsiz}}
  ,{"HSC41", {-9999., scol, ssiz}}
  ,{"QSF42", {-9999., qcol, qsiz}}
  ,{"QSF43", {-9999., qcol, qsiz}}
  ,{"FSH41", {-9999., ccol, csiz}}

  ,{"HSC42", {-9999., scol, ssiz}}
  ,{"QSF44", {-9999., qcol, qsiz}}
  ,{"FS42H", {-9999., ccol, csiz}}
  ,{"FS42V", {-9999., ccol, csiz}}

  ,{"ColPiE5", {-9999., ccol, csiz}}

  ,{"QSF45", {-9999., qcol, qsiz}}
  ,{"HSC43", {-9999., scol, ssiz}}
  ,{"FSH43", {-9999., ccol, csiz}}
  ,{"QSF46", {-9999., qcol, qsiz}}
  ,{"QSF47", {-9999., qcol, qsiz}}
  ,{"HSC44", {-9999., scol, ssiz}}
  ,{"QSF48", {-9999., qcol, qsiz}}  

  ,{"zAST41", {-9999., dascol, dassiz}}  
  ,{"frontarcAST41", {-9999., dascol, dassiz}}  
  ,{"zASC41", {-9999., dascol, dassiz}}  
  ,{"frontarcASC41", {-9999., dascol, dassiz}}  

  ,{"QSB41", {-9999., qcol, qsiz}}  
  ,{"QSB42", {-9999., qcol, qsiz}}  
  ,{"QSB43", {-9999., qcol, qsiz}}  
  ,{"SEP41", {-9999., secol, sesiz}}  
  ,{"QSK41", {-9999., qcol, qsiz}}  
  ,{"QSK42", {-9999., qcol, qsiz}}  
  ,{"QSK43", {-9999., qcol, qsiz}}  
  ,{"PILL1", {-9999., ccol, csiz}}  
  ,{"SML41", {-9999., stucol, stusiz}}  
  ,{"MEGCOL", {-9999., stucol, stusiz}}  
  ,{"zASL41", {-9999., stucol, stusiz}}  
  ,{"frontarcASL41", {-9999., stucol, stusiz}}  
  ,{"QSO41", {-9999., qcol, qsiz}}  
  ,{"QSO42", {-9999., qcol, qsiz}}  
  ,{"zASK41", {-9999., stucol, stusiz}}
  ,{"posQSM41",  {-9999., stucol, stusiz}}

  ,{"PILL0", {-9999., ccol, csiz}}  
  ,{"PILL1", {-9999., ccol, csiz}}  
  ,{"PILL2", {-9999., ccol, csiz}}  
  ,{"PILL3", {-9999., ccol, csiz}}

  ,{"poszMU3ESOLTUBI1", {-9999., detcol, detsiz}}  
  ,{"poszMU3ESOLTUBO1", {-9999., detcol, detsiz}}  
  
  
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
void readPositions(string filename) {
  // -- reset
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    it->second.z = -99999.;
  }
  ifstream INS;
  string sline;
  INS.open(filename);
  while (getline(INS, sline)) {
    for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
      if (string::npos != sline.find(it->first)) {
        string sval = sline.substr(sline.find(it->first) + it->first.length() + 1);
        double dval = atof(sval.c_str());
        cout << "found " << it->first << " with sval ->" << sval << "<-"
             << " resulting in double = " << dval
             << endl;
        if (dval > 0.) {
          it->second.z = dval;
        }
      }
    }
  }
}

// ----------------------------------------------------------------------
string varTitle(string var) {
  if ("Z" == var) {
    return string("Z [mm]");
  } else if ("meanX" == var) {
    return string("<X> [mm]");
  } else if ("meanY" == var) {
    return string("<Y> [mm]");
  } else if ("sigmaX" == var) {
    return string("#sigma(X) [mm]");
  } else if ("sigmaY" == var) {
    return string("#sigma(Y) [mm]");
  }
  return string("no title");
}


//----------------------------------------------------------------------
void markup(double ymax = 0., double ymin = -60., double xmax = 20000., string filename = "nada") {
  if (gBeamlinePositions["QSF41"].z < 0.) readPositions(filename);

  TLine  *pl = new TLine();
  pl->SetLineStyle(kDotted);
  pl->SetLineColor(kBlue+2);

  TLatex *tl = new TLatex();

  gPad->GetCanvas()->SetWindowSize(1200, 300); 
  
  tl->SetNDC(kFALSE);
  tl->SetTextAngle(90.);
  tl->SetTextSize(0.03);
  tl->SetTextColor(kBlack);
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    if (it->second.z > xmax) continue;
    tl->DrawLatex(it->second.z, 0.95*ymax, it->first.c_str());
    cout << "tl->DrawLatex(" << it->second.z << ", " << ymax << ", " << it->first.c_str() << ");" << endl;
    pl->DrawLine(it->second.z, ymin, it->second.z, 0.9*ymax);
  }

  pl->SetLineStyle(kDashed);
  pl->SetLineColor(kBlack);
  pl->DrawLine(0., 0., xmax, 0.);
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
void graphExtrema(TGraph *gr, double& xmin, double& xmax, double& ymin, double& ymax) {
  xmin = ymin = 1.e99;
  xmax = ymax = -1.e99;
  for (int i = 0; i < gr->GetN(); ++i) {
    int x = gr->GetPointX(i); 
    int y = gr->GetPointY(i); 
    if (x > xmax) xmax = x;
    if (y > ymax) ymax = y;
    if (x < xmin) xmin = x;
    if (y < ymin) ymin = y;
  }  
}

// ----------------------------------------------------------------------
TGraph* t2g(TTree *t, string sy, string sx) {
  double valx, valy;
  t->SetBranchAddress(sy.c_str(), &valy);
  t->SetBranchAddress(sx.c_str(), &valx);


  long long int n_entries = t->GetEntries();
  long long int i(0);

  TGraph *gr = new TGraph(n_entries);
  
  for (long long int entry = 0; entry < n_entries; ++entry) {
    t->GetEntry(entry);
    gr->AddPoint(valx, valy);
  }

  return gr;
}

// ----------------------------------------------------------------------
void anaProfile(string filename = "profile.txt") {
  if (string::npos != filename.find("~")) {
    string home = gSystem->Getenv("HOME");
    filename.replace(filename.find("~"), 1, home);
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

  // -- find placement files
  string g4blpioneer = gSystem->Getenv("G4BLPIONEER");
  string g4blPositions = g4blpioneer + "/pie5/Positions.txt";
  cout << g4blPositions << endl;
  
  gStyle->SetOptTitle(0);
 
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  
  TTree *t = fillTree(filename);
  t->Print();

  double xmin, xmax, ymin, ymax;
  TGraph *grX = t2g(t, "meanX", "Z");
  graphExtrema(grX, xmin, xmax, ymin, ymax);
  grX->Draw("alp");
  markup(ymax, ymin, xmax, g4blPositions);
  c1->SaveAs(Form("%s-meanX.pdf", pdfname.c_str()));

  TGraph *grY = t2g(t, "meanY", "Z");
  graphExtrema(grY, xmin, xmax, ymin, ymax);
  grY->Draw("alp");
  markup(ymax, ymin, xmax, g4blPositions);
  c1->SaveAs(Form("%s-meanY.pdf", pdfname.c_str()));
    
}


// ----------------------------------------------------------------------
void cmpProfile(string vary = "sigmaX", string varx = "Z", 
                string filename1 = "../../CMBL_g4beamline/profiles/CMBL2021_QSK41newLQ_final_profile.dat",
                string filename2 = "../../CMBL_g4beamline/profiles/profileCMBL2021_05_COSY_coll1_new.dat",
                string positions = "../../CMBL_g4beamline/Settings/Positions.txt") {
  if (string::npos != filename1.find("~")) {
    string home = gSystem->Getenv("HOME");
    filename1.replace(filename1.find("~"), 1, home);
  }
  if (string::npos != filename2.find("~")) {
    string home = gSystem->Getenv("HOME");
    filename2.replace(filename2.find("~"), 1, home);
  }
  string pdfname1 = filename1; 
  if (string::npos != pdfname1.rfind("/")) {
    pdfname1.replace(pdfname1.begin(), pdfname1.begin() + pdfname1.rfind("/") + 1, "");
  }

  if (string::npos != pdfname1.find(".dat")) {
    pdfname1.replace(pdfname1.find(".dat"), 4, "");
  }
  if (string::npos != pdfname1.find(".txt")) {
    pdfname1.replace(pdfname1.find(".txt"), 4, "");
  }
  string pdfname2 = filename2; 
  if (string::npos != pdfname2.rfind("/")) {
    pdfname2.replace(pdfname2.begin(), pdfname2.begin() + pdfname2.rfind("/") + 1, "");
  }

  if (string::npos != pdfname2.find(".dat")) {
    pdfname2.replace(pdfname2.find(".dat"), 4, "");
  }
  if (string::npos != pdfname2.find(".txt")) {
    pdfname2.replace(pdfname2.find(".txt"), 4, "");
  }
  
  string pdfname = pdfname1 + pdfname2;
  cout << "pdfname: ->" << pdfname << "<-" << endl;
  
  gStyle->SetOptTitle(0);

  TLatex *tl = new TLatex();
  tl->SetTextSize(0.07);
  Color_t mcol; 
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  
  TTree *t1 = fillTree(filename1);

  double xmin, xmax, ymin, ymax;
  TGraph *gr1 = t2g(t1, vary, varx);
  graphExtrema(gr1, xmin, xmax, ymin, ymax);
  xmax = 13000.;
  mcol = kBlue; 
  gr1->SetLineColor(mcol);
  gr1->SetMarkerColor(mcol);
  gr1->Draw("alp");
  gr1->GetYaxis()->SetTitleSize(0.05);
  gr1->GetYaxis()->SetTitle(varTitle(vary).c_str());
  gr1->GetXaxis()->SetTitleSize(0.05);
  gr1->GetXaxis()->SetTitle(varTitle(varx).c_str());
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.1, 0.92, pdfname1.c_str());

  TTree *t2 = fillTree(filename2);
  TGraph *gr2 = t2g(t2, vary, varx);
  mcol = kRed;
  gr2->SetLineColor(mcol);
  gr2->SetMarkerColor(mcol);
  gr2->Draw("lp");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.6, 0.92, pdfname2.c_str());

  markup(ymax, ymin, xmax, positions);
  c1->SaveAs(Form("cmp-%s-%s-%s.pdf", vary.c_str(), varx.c_str(), pdfname.c_str()));

    
}


// ----------------------------------------------------------------------
void cmpProfileAll() {
  cmpProfile("sigmaX");
  cmpProfile("sigmaY");
  cmpProfile("meanX");
  cmpProfile("meanY");
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
