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
// -- from Giovanni
// ----------------------------------------------------------------------
void quadPars(double s11, double s12, double s22, double* results){
    double e = sqrt(s11*s22-s12*s12)*1e3;
    double a = -s12/e*1e3;
    double b = s11/e;
    double g = s22/e*1e6;
    cout << endl << "alpha = " << a << ", beta = " << b << ", gamma = " << g << endl;
    results[0] = b;
    results[1] = a;
    results[2] = g;
    results[3] = e;
}


// ----------------------------------------------------------------------
// -- from Giovanni
// ----------------------------------------------------------------------
void ellipse(double *pars, double *result){
    double A = pars[2];
    double B = 2*pars[1];
    double C = pars[0];
    double F = -pars[3];
    
    double a = -sqrt(2*(B*B-4*A*C)*F*((A+C)+sqrt((A-C)*(A-C)+B*B)))/(B*B-4*A*C);
    double b = -sqrt(2*(B*B-4*A*C)*F*((A+C)-sqrt((A-C)*(A-C)+B*B)))/(B*B-4*A*C);
    
    double theta = 0;
    
    if(B == 0 && A > C){
        theta = 90;
    }
    else if(B != 0){
        theta = 180*atan((C-A-sqrt((A-C)*(A-C)+B*B))/B)/TMath::Pi();
    }
    
    result[0] = a;
    result[1] = b;
    result[2] = theta;
}


// ----------------------------------------------------------------------
// 250 mm US of QSK41 center
void ellipse0() {
  TEllipse e; 
  e.SetLineColor(kBlue); 
  e.SetLineWidth(2); 
  e.SetFillStyle(0); 
  
  double resultsx[5] = {-21.7, -13.0, 0., 0.};
  double parsx[5] = {-21.7, -13.0, 0., 0.};
  double ellx[3] = {0};

  quadPars(parsx[0], parsx[1], parsx[2], resultsx);
  ellipse(resultsx, ellx);


  double sx = sqrt(resultsx[0]*resultsx[3]), sxp = sqrt(resultsx[2]*resultsx[3]);
  
  TGraph* Gx = new TGraph();
  Gx->SetPoint(0, 0, 0);
  Gx->GetXaxis()->SetLimits(-1.2*sx+parsx[3], 1.2*sx+parsx[3]);
  Gx->GetYaxis()->SetRangeUser(-1.2*sxp+parsx[4], 1.2*sxp+parsx[4]);
  
 
  TEllipse* X = new TEllipse(parsx[3], parsx[4], ellx[0], ellx[1], 0, 360, ellx[2]);
  X->SetFillColorAlpha(kBlack, 0);
  
  TCanvas* can = new TCanvas("can", "can", 1920, 1080);
  gPad->SetGrid();
  gPad->SetMargin(0.15, 0.15, 0.1, 0.1);
  
  Gx->Draw("alp");
  
  X->SetLineWidth(3);
  X->SetLineColor(kBlue);
  X->Draw("same");
   

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
  } else if ("emitX" == var) {
    return string("#epsilon(X) [mm rad]");
  } else if ("emitY" == var) {
    return string("#epsilon(Y) [mm rad]");
  } else if ("emitTrans" == var) {
    return string("#epsilon(T) [mm rad]");
  } else if ("betaX" == var) {
    return string("#beta(X) [mm]");
  } else if ("betaY" == var) {
    return string("#beta(Y) [mm]");
  } else if ("betaTrans" == var) {
    return string("#beta(T) [mm]");
  } else if ("alphaX" == var) {
    return string("#alpha(X) ");
  } else if ("alphaY" == var) {
    return string("#alpha(Y) ");
  } else if ("alphaTrans" == var) {
    return string("#alpha(T) ");
  } else if ("meanP" == var) {
    return string("<p> [MeV] ");
  } else if ("N" == var) {
    return string("N ");
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
  double dx = xmax*0.002;
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    if (it->second.z > xmax) continue;
    if (string::npos != it->first.find("frontarc")) continue;
    tl->DrawLatex(it->second.z - dx, 0.70*ymax, it->first.c_str());
    cout << "tl->DrawLatex(" << it->second.z << ", " << ymax << ", " << it->first.c_str() << ");" << endl;
    pl->DrawLine(it->second.z, ymin, it->second.z, 0.9*ymax);
  }

  pl->SetLineStyle(kDotted);
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
    valFirstLine = 3;
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
  for (int i = 0; i < gr->GetN(); ++i) {
    double x = gr->GetPointX(i); 
    double y = gr->GetPointY(i); 
    if (x > xmax) xmax = x;
    if (y > ymax) ymax = y;
    if (x < xmin) xmin = x;
    if (y < ymin) ymin = y;
    //    cout << "i = " << i << " ymin = " << ymin << " ymax = " << ymax << endl;
  }
  cout << "graphExtrema ymin = " << ymin <<  " ymax = " << ymax << endl;
}

// ----------------------------------------------------------------------
TGraph* t2g(TTree *t, string sy, string sx, double offsetX = 0.) {
  double valx, valy;
  t->SetBranchAddress(sy.c_str(), &valy);
  t->SetBranchAddress(sx.c_str(), &valx);

  long long int n_entries = t->GetEntries();
  long long int i(0);

  TGraph *gr = new TGraph();
  
  for (long long int entry = 0; entry < n_entries; ++entry) {
    t->GetEntry(entry);
    gr->AddPoint(valx + offsetX, valy);
  }

  cout << "DBX gr: " << gr->GetPointX(0) << "  " << gr->GetPointY(0) << endl;
  
  return gr;
}

// ----------------------------------------------------------------------
void anaProfile(string var1 = "meanX",
                string filename = "profile.txt",
                string positions = "../pie5/Positions.txt") {
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

  gStyle->SetOptTitle(0);
 
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  
  TTree *t = fillTree(filename);
  t->Print();

  double xmin(99999.), xmax(-99999.), ymin(99999.), ymax(-99999.);
  TGraph *grX = t2g(t, var1, "Z");
  graphExtrema(grX, xmin, xmax, ymin, ymax);
  grX->Draw("alp");
  markup(ymax, ymin, xmax, positions);
  c1->SaveAs(Form("%s-%s.pdf", pdfname.c_str(), var1.c_str()));
    
}


// ----------------------------------------------------------------------
void cmpProfile(string vary = "sigmaX", string varx = "Z", 
                string filename1 = "data/p65-0001-v0/11.prof",
                string filename2 = "data/p65-0001-v1/11.prof",
                double offset2   = 0.,
                string positions = "../pie5/Positions.txt") {

  vector<string> v1 = split(filename1, '/');
  vector<string> v2 = split(filename2, '/');

  string filename("");
  if (v1[v1.size()-1] == v2[v2.size()-1]) {
    filename = v1[v1.size()-1];
    cout << "filename = " << filename << endl;
  } else{
    cout << "v1[v1.size()-1] = ->" << v1[v1.size()-1]
         << "<- v2[v2.size()-1] = ->" << v2[v2.size()-1]
         << "<-" 
         << endl;
  }
  
  string pdfname1 = v1[v1.size()-2];
  string pdfname2 = v2[v1.size()-2];

  string pdfname = pdfname1 + "-" + pdfname2 + "-" + filename;
  cout << "pdfname: ->" << pdfname << "<-" << endl;
  
  gStyle->SetOptTitle(0);

  TLatex *tl = new TLatex();
  tl->SetTextSize(0.07);
  Color_t mcol; 
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  c1->SetRightMargin(0.01); 
  c1->SetLeftMargin(0.05); 
  
  TTree *t1 = fillTree(filename1);

  double xmin(99999.), xmax(-99999.), ymin(99999.), ymax(-99999.);
  TGraph *gr1 = t2g(t1, vary, varx);
  graphExtrema(gr1, xmin, xmax, ymin, ymax);

  gStyle->SetOptStat(0);
  TH1D *h1 = new TH1D("h1", "", 100, 0, xmax);
  double hmax(1.5*ymax);
  double hmin(ymin > 0.? 0.: 1.5*ymin);
  if (vary == "N") {
    hmin = 0.5;
    gPad->SetLogy(1);
  } else {
    gPad->SetLogy(0);
  }
  if (vary == "meanX") {
    hmin = -100.;
    hmax = 100.;
  } else if (vary == "meanY") {
    hmin = -40.;
    hmax = 40.;
  }
  h1->SetMinimum(hmin); 
  h1->SetMaximum(hmax);

  h1->GetXaxis()->SetLabelSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.05);

  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitleOffset(0.4);
  h1->GetYaxis()->SetTitle(varTitle(vary).c_str());
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetTitle(varTitle(varx).c_str());
  h1->Draw("axis");
  
  mcol = kBlue; 
  gr1->SetLineColor(mcol);
  gr1->SetMarkerColor(mcol);
  gr1->Draw("lp");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.1, 0.92, pdfname1.c_str());

  TTree *t2 = fillTree(filename2);
  TGraph *gr2 = t2g(t2, vary, varx, offset2);
    
  mcol = kRed;
  gr2->SetLineColor(mcol);
  gr2->SetMarkerColor(mcol);
  gr2->Draw("lp");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.6, 0.92, pdfname2.c_str());
  if (offset2 > 0.) {
    tl->SetTextColor(kBlack);
    tl->SetTextSize(0.05);
    tl->DrawLatexNDC(0.7, 0.18, Form("offset = %8.2f", offset2));
  }
  
  if (vary == "N") {
    markup(10., 0.5, xmax, positions);
  } else {
    markup(ymax, ymin, xmax, positions);
  }
  c1->SaveAs(Form("cmp-%s-%s-%s.pdf", vary.c_str(), varx.c_str(), pdfname.c_str()));

    
}


// ----------------------------------------------------------------------
// -- collect all particles into one comparison plot (i.e. six curves)
void cmpProfilesElMuPi(string vary = "sigmaX", string varx = "Z", 
                       string dir1 = "data/p65-0001-v0",
                       string dir2 = "data/p65-0001-v1",
                       int mode = 0, 
                       double offset2   = 0.,
                       string positions = "../pie5/Positions.txt") {

  vector<string> v1 = split(dir1, '/');
  vector<string> v2 = split(dir2, '/');

  string filename("cmpProfilesElMuPi");
  
  string pdfname1 = v1[v1.size()-1];
  string pdfname2 = v2[v1.size()-1];

  string pdfname = pdfname1 + "-" + pdfname2 + "-" + filename;
  cout << "pdfname: ->" << pdfname << "<-" << endl;
  
  gStyle->SetOptTitle(0);

  TLatex *tl = new TLatex();
  tl->SetTextSize(0.07);
  Color_t mcol; 
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(700, 800); 
  c1->SetRightMargin(0.01); 
  c1->SetLeftMargin(0.05); 
  
  string profilename = dir1 + "/11.prof"; 
  cout << "profilename = " << profilename << endl;
  TTree *t1El = fillTree(profilename);
  profilename = dir1 + "/13.prof"; 
  cout << "profilename = " << profilename << endl;
  TTree *t1Mu = fillTree(profilename);
  profilename = dir1 + "/211.prof"; 
  cout << "profilename = " << profilename << endl;
  TTree *t1Pi = fillTree(profilename);

  double xmin(99999.), xmax(-99999.), ymin(99999.), ymax(-99999.);
  TGraph *gr1El = t2g(t1El, vary, varx);
  graphExtrema(gr1El, xmin, xmax, ymin, ymax);
  TGraph *gr1Mu = t2g(t1Mu, vary, varx);
  graphExtrema(gr1Mu, xmin, xmax, ymin, ymax);
  TGraph *gr1Pi = t2g(t1Pi, vary, varx);
  graphExtrema(gr1Pi, xmin, xmax, ymin, ymax);
  ymax = +100;
  ymin = -100;
  
  gStyle->SetOptStat(0);
  TH1D *h1 = new TH1D("h1", "", 100, 0, xmax);
  gPad->SetLogy(0);
  if (vary == "N") {
    ymax = 20000;
    ymin = 0.5;
    gPad->SetLogy(1);
  } else if (vary == "meanX") {
    ymin = -1.;
    ymax = +1;
    if (3 == mode) {
      ymin = -10.;
      ymax = +10;
    }
  } else if (vary == "sigmaX") {
    ymin = -0.2;
    ymax = +0.2;
    if (3 == mode) {
      ymin = -50.;
      ymax = +50.;
    }
  } else if (vary == "meanY") {
    ymin = -200.;
    ymax = +200.;
  } else if (vary == "sigmaY") {
    ymin = -0.2;
    ymax = +0.2;
    if (3 == mode) {
      ymin = -50.;
      ymax = +50.;
    }
  }
  double hmax(ymax);
  double hmin(ymin);


  h1->SetMinimum(hmin); 
  h1->SetMaximum(hmax);

  h1->GetXaxis()->SetLabelSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.05);

  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetTitleOffset(0.4);
  h1->GetYaxis()->SetTitle(varTitle(vary).c_str());
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetTitle(varTitle(varx).c_str());
  h1->Draw("axis");

  tl->SetTextColor(kBlack);
  tl->DrawLatexNDC(0.1, 0.92, (pdfname1 + ":").c_str());
  
  mcol = kCyan; 
  gr1El->SetLineColor(mcol);
  gr1El->SetMarkerColor(mcol);
  gr1El->SetMarkerSize(0.1);
  gr1El->SetMarkerStyle(kOpenCircle);
  gr1El->Draw("p");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.20, 0.92, "El");

  mcol = kAzure-4; 
  gr1Mu->SetLineColor(mcol);
  gr1Mu->SetMarkerSize(0.1);
  gr1Mu->SetMarkerColor(mcol);
  gr1Mu->SetMarkerStyle(kOpenCircle);
  gr1Mu->Draw("p");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.25, 0.92, "Mu");

  mcol = kBlue + 1; 
  gr1Pi->SetLineColor(mcol);
  gr1Pi->SetMarkerSize(0.1);
  gr1Pi->SetMarkerColor(mcol);
  gr1Pi->SetMarkerStyle(kPlus);
  gr1Pi->Draw("p");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.30, 0.92, "Pi");

  profilename = dir2 + "/11.prof"; 
  TTree *t2El = fillTree(profilename);
  TGraph *gr2El = t2g(t2El, vary, varx, offset2);
  profilename = dir2 + "/13.prof"; 
  TTree *t2Mu = fillTree(profilename);
  TGraph *gr2Mu = t2g(t2Mu, vary, varx, offset2);
  profilename = dir2 + "/211.prof"; 
  TTree *t2Pi = fillTree(profilename);
  TGraph *gr2Pi = t2g(t2Pi, vary, varx, offset2);

  tl->SetTextColor(kBlack);
  tl->DrawLatexNDC(0.60, 0.92, (pdfname2 + ":").c_str());
  
  mcol = kMagenta - 7;
  gr2El->SetLineColor(mcol);
  gr2El->SetLineStyle(kDotted);
  gr2El->SetMarkerColor(mcol);
  gr2El->Draw("l");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.70, 0.92, "El");

  mcol = kPink;
  gr2Mu->SetLineColor(mcol);
  gr2Mu->SetLineStyle(kDashed);
  gr2Mu->SetMarkerColor(mcol);
  gr2Mu->Draw("l");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.75, 0.92, "Mu");

  mcol = kRed + 1;
  gr2Pi->SetLineColor(mcol);
  gr2Pi->SetLineStyle(kDashDotted);
  gr2Pi->SetMarkerColor(mcol);
  gr2Pi->Draw("l");
  tl->SetTextColor(mcol);
  tl->DrawLatexNDC(0.80, 0.92, "Pi");

  if (vary == "N") {
    markup(10., 0.5, xmax, positions);
  } else {
    markup(ymax, ymin, xmax, positions);
  }
  c1->SaveAs(Form("cmp-%s-%s-%s.pdf", vary.c_str(), varx.c_str(), pdfname.c_str()));

    
}


// ----------------------------------------------------------------------
void cmpProfileAll(string filename1 = "../../CMBL_g4beamline/profiles/CMBL2021_QSK41newLQ_final_profile.dat",
                   string filename2 = "../../CMBL_g4beamline/profiles/profileCMBL2021_05_COSY_coll1_new.dat"
                   ) {
  cmpProfile("sigmaX", "Z", filename1, filename2);
  cmpProfile("sigmaY", "Z", filename1, filename2);
  cmpProfile("meanX", "Z", filename1, filename2);
  cmpProfile("meanY", "Z", filename1, filename2);
}

// ----------------------------------------------------------------------
void cmpProfilesElMuPiAll(string filename1 = "data/p65-0001-v0",
                          string filename2 = "data/p65-0001-v2",
                          int mode = 0
                          ) {
  cmpProfilesElMuPi("sigmaX", "Z", filename1, filename2, mode);
  cmpProfilesElMuPi("sigmaY", "Z", filename1, filename2, mode);
  cmpProfilesElMuPi("meanX", "Z", filename1, filename2, mode);
  cmpProfilesElMuPi("meanY", "Z", filename1, filename2, mode);
  //  cmpProfilesElMuPi("N", "Z", filename1, filename2, mode);
}


// ----------------------------------------------------------------------
void allProfilesElMuPiAll(int mode = 0) {
  // -- mode = 3
  if (0) {
    cmpProfilesElMuPiAll("data/p65-0002-v0", "data/p65-0003-v0", mode);
    cmpProfilesElMuPiAll("data/p65-0002-v0", "data/p65-0003-v1", mode);
  }

  // -- mode = 0
  if (0) {
    cmpProfilesElMuPiAll("data/p65-0001-v1", "data/p65-0002-v0");
    cmpProfilesElMuPiAll("data/p65-0001-v1", "data/p65-0002-v1");
    cmpProfilesElMuPiAll("data/p65-0001-v1", "data/p65-0002-v2");

    cmpProfilesElMuPiAll("data/p65-0002-v0", "data/p65-0002-v3");
    cmpProfilesElMuPiAll("data/p65-0002-v0", "data/p65-0002-v4");
  }
  
  // -- mode = 0
  if (0) {
    cmpProfilesElMuPiAll("data/p65-0001-v0", "data/p65-0001-v1");
    cmpProfilesElMuPiAll("data/p65-0001-v1", "data/p65-0001-v2");
    cmpProfilesElMuPiAll("data/p65-0001-v2", "data/p65-0001-v3");
    cmpProfilesElMuPiAll("data/p65-0001-v2", "data/p65-0001-v4");
  }

}

// ----------------------------------------------------------------------
void cmpProfile1(double offset2 = 14634., string var1 = "nada") {
  string file1("/Users/ursl/data/pioneer/slurm/muontransport/m0003/m0003-profile.txt");
  string file2("../../CMBL_g4beamline/profiles/profileCMBL2021_05_COSY_coll1_new.dat");
  // 14634
  if (var1 != "nada") {
    cmpProfile(var1, "Z", file1, file2, offset2, "../pie5/Positions.txt");
  } else {
    cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");

    cmpProfile("emitX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("emitY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("emitTrans", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("betaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("betaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("betaTrans", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("alphaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("alphaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("alphaTrans", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  }
  
}

// ----------------------------------------------------------------------
void cmpProfile2(double offset2 = 0., string var1 = "nada") {
  string file1("/Users/ursl/data/pioneer/slurm/muontransport/m0000/m0000-profile.txt");
  string file2("/Users/ursl/data/pioneer/slurm/piontransport/m0002/m0002-profile.txt");
  if (var1 != "nada") {
    cmpProfile(var1, "Z", file1, file2, offset2, "../pie5/Positions.txt");
  } else {
    cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
    cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  }
  
}

// ----------------------------------------------------------------------
void cmpProfile3(double offset2 = 0, string var1 = "nada") {
  string file1("/Users/ursl/data/pioneer/slurm/muontransport/m0003/m0003-profile.txt");
  string file2("/Users/ursl/data/pioneer/slurm/muontransport/m0005/m0005-profile.txt");
  cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
}


// ----------------------------------------------------------------------
void cmpProfile4(double offset2 = 14634., string var1 = "nada") {
  string file1("/Users/ursl/data/pioneer/slurm/muontransport/m0005/m0005-profile.txt");
  string file2("../../CMBL_g4beamline/profiles/profileCMBL2021_05_COSY_coll1_new.dat");
  cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
}


// ----------------------------------------------------------------------
void cmpProfile5(string var1 = "p28_m0001", string var2 = "p65_m0001",
                 string dir1 = "muontransport", string dir2 = "muontransport") {

  double offset2(0.);
  
  string home("/Users/ursl/data/pioneer/slurm/");
  string file1 = home + dir1 + "/muprod0002-" + var1 + "/" + var1 + "-profile.txt";
  string file2 = home + dir2 + "/muprod0002-" + var2 + "/" + var2 + "-profile.txt";

  cout << "file1 = " << file1 << endl;
  cout << "file2 = " << file2 << endl;
  cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
}

// ----------------------------------------------------------------------
void cmpProfiles(string fil1 = "p28-muprod0003-megcol-pi0001-profile-211.txt",
                 string dir1 = "p28-muprod0003-megcol-pi0001",
                 string fil2 = "p28-muprod0003-megcol-pi0002-profile-211.txt", 
                 string dir2 = "p28-muprod0003-megcol-pi0002") {

  double offset2(0.);
  
  string home("/Users/ursl/data/pioneer/slurm/transport");
  string file1 = home + "/" + dir1 + "/" + fil1;
  string file2 = home + "/" + dir2 + "/" + fil2;

  cout << "file1 = " << file1 << endl;
  cout << "file2 = " << file2 << endl;
  cmpProfile("meanX",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("meanY",  "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaX", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("sigmaY", "Z", file1, file2, offset2, "../pie5/Positions.txt");
  cmpProfile("N", "Z", file1, file2, offset2, "../pie5/Positions.txt");
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
