#include <iostream>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------
// cd pioneer-g4bl/macros
// root
// root [0] .L anaTracks.C 
// root [1] anaTracks("some BL2TrackFile")
// root [2] CALOCNTR->Draw("EventID")
// ----------------------------------------------------------------------

TTree *gTree(0);

//----------------------------------------------------------------------
void fillTree(string filename, int nlines) {
  int VERBOSE(0);
  ifstream INS;
  vector<string> vlines; 
  string sline;
  INS.open(filename);
  int cntLines(0);
  while (getline(INS, sline)) {
    vlines.push_back(sline);
    ++cntLines;
    if ((nlines > 0) && (cntLines > nlines)) {
      break;
    }
  }
  INS.close();
  cout << "read " << cntLines << " lines" << endl;
  
  // -- parse beginning to set up tree
  int valFirstLine(1);
  string tname = vlines[0].substr(1); 
  if (string::npos != vlines[0].find("g4beamline profile")) {
    tname = vlines[0].substr(2); 
    valFirstLine = 2;
  }
  string dname("tree");
  cout << "hallo tname->" << tname << "<-" << endl;
  if (string::npos != tname.find(" ")) {
    dname = tname.substr(tname.find(" ") + 1);
  } 
  cout << "dname ->" << dname << "<-" << endl;
  
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
  gTree = new TTree(dname.c_str(), tname.c_str());
  for (unsigned int i = 0; i < vars.size(); ++i) {
    if (VERBOSE > 1) cout << "vars[" << i << "] = " << vars[i] << ": "; 
    if (varDouble.end() != varDouble.find(vars[i])) {
      if (VERBOSE > 1) cout << varDouble[vars[i]] << " (double)";
      gTree->Branch(vars[i].c_str(), varDouble[vars[i]], Form("%s/D", vars[i].c_str()));
    }
    if (varInt.end() != varInt.find(vars[i])) {
      if (VERBOSE > 1) cout << varInt[vars[i]] << " (int)";
      gTree->Branch(vars[i].c_str(), varInt[vars[i]], Form("%s/I", vars[i].c_str()));
    }
    if (VERBOSE > 1) cout << endl;
  }


  // -- read rest of file and fill into tree
  for (unsigned int il = valFirstLine; il < vlines.size(); ++il) {
    if (0 == il%10000) {
      cout << "\r" << "reading line " << il << flush; 
    }
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
    gTree->Fill();
  }
  cout << endl;
    //  return t; 
}

// ----------------------------------------------------------------------
void anaTracks(string filename = "../project-g4bl/bl2/p0007-p65BLTrackFile2_PDGid0_DetPiE5.txt", int nlines = -1) {
  if (string::npos != filename.find("~")) {
    string home = gSystem->Getenv("HOME");
    cout << "home ->" << home << "<-" << endl;
    filename.replace(filename.find("~"), 1, home);
    cout << "filename now ->" << filename << "<-" << endl;
  }
  fillTree(filename, nlines);
  gTree->Print();

  gTree->Draw("y:x", "", "colz");
}


// ----------------------------------------------------------------------
void draw2d(string varx, string vary, string cuts, double xmin, double xmax, double ymin, double ymax, string opt = "colz") {
  TH2D *h2 = new TH2D("h2", cuts.c_str(), 200, xmin, xmax, 200, ymin, ymax);
  h2->SetXTitle(varx.c_str()); 
  h2->SetYTitle(vary.c_str()); 
  if (cuts == "") h2->SetTitle("(no cut)");
  
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.15);

  string drawVar = vary + ":" + varx + ">>h2";
  gTree->Draw(drawVar.c_str(), cuts.c_str(), "goff");


  h2->DrawCopy(opt.c_str());
}

// ----------------------------------------------------------------------
void draw1d(string varx, string cuts, double xmin, double xmax, string opt = "hist") {
  TH1D *h1 = new TH1D("h1", cuts.c_str(), 200, xmin, xmax);
  h1->SetXTitle(varx.c_str()); 
  if (cuts == "") h1->SetTitle("(no cut)");
  
  gPad->SetLeftMargin(0.15);
  gPad->SetRightMargin(0.15);

  string drawVar = varx + ">>h1";
  gTree->Draw(drawVar.c_str(), cuts.c_str(), "goff");

  h1->DrawCopy(opt.c_str());
}


// ----------------------------------------------------------------------
void plot1(string pdfname = "plot1.pdf") {
  zone(2,2);
  draw2d("x", "y", "PDGid==2212", -250., 250., -250., 250.);
  c0.cd(2);
  draw2d("x", "y", "PDGid==211", -250., 250., -250., 250.);
  c0.cd(3);
  draw2d("x", "y", "PDGid==-13", -250., 250., -250., 250.);
  c0.cd(4);
  draw2d("x", "y", "PDGid==-11", -250., 250., -250., 250.);

  c0.SaveAs(pdfname.c_str());
}


// ----------------------------------------------------------------------
void plot2(string pdfname = "plot2.pdf") {
  zone(2,2);
  draw1d("Pz", "PDGid==2212", 0., 120.);
  c0.cd(2);
  draw1d("Pz", "PDGid==211", 0., 120.);
  c0.cd(3);
  draw1d("Pz", "PDGid==-13", 0., 120.);
  c0.cd(4);
  draw1d("Pz", "PDGid==-11", 0., 120.);

  c0.SaveAs(pdfname.c_str());
}
