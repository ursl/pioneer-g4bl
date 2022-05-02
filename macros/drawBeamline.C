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
void readPositions(string g4blPositions) {
  // -- reset
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    it->second.z = -99999.;
  }

  ifstream INS;
  string sline;
  INS.open(g4blPositions);
  while (getline(INS, sline)) {
    for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
      if (string::npos != sline.find(it->first)) {
        string sval = sline.substr(sline.find(it->first) + it->first.length() + 1);
        double dval = atof(sval.c_str());
        if (0) cout << "found " << it->first << " with sval ->" << sval << "<-"
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
void drawBeamline(string filename = "../pie5/Positions.txt", string pdfname = "pie5-zach-positions.pdf",
                  double zmax = 24000.) {
  
  TCanvas *c1 = new TCanvas("c1", "");
  c1->SetWindowSize(1200, 200); 
  c1->SetRightMargin(0.01); 
  c1->SetLeftMargin(0.02); 
  c1->Draw();

  gStyle->SetOptStat(0);
  gPad->SetGridx(1);
  
  TH1D *h1 = new TH1D("h1", "", static_cast<int>(zmax), 0., zmax);
  h1->SetMinimum(0.);
  h1->SetMaximum(1.);
  h1->SetNdivisions(0, "Y");
  h1->SetNdivisions(588, "X");
  h1->Draw();
  
  TLatex *tl  = new TLatex();

  cout << "reading positions from " << filename << endl;
  readPositions(filename);

  tl->SetNDC(kFALSE);
  tl->SetTextAngle(90.);
  tl->SetTextSize(0.03);
  tl->SetTextColor(kBlack);
  for (auto it = gBeamlinePositions.begin(); it != gBeamlinePositions.end(); ++it) {
    if (it->second.z > zmax) continue;
    tl->SetTextAlign(kVAlignCenter);
    tl->SetTextColor(it->second.col);
    tl->SetTextSize(it->second.tsiz);
    tl->DrawLatex(it->second.z, 0.11, it->first.c_str());

    pl->SetLineColor(it->second.col);
    pl->DrawLine(it->second.z, 0., it->second.z, 0.1);
  }

  c1->SaveAs(pdfname.c_str());
  
}


// ----------------------------------------------------------------------
void drawBeamlineAll() {
  drawBeamline("../pie5/Positions.txt", "pie5-zach-positions.pdf", 25000.);
  drawBeamline("../../CMBL_g4beamline/Settings/Positions.txt", "pie5-giovanni-positions.pdf", 12000.);
}
