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



//----------------------------------------------------------------------
TTree* fillTree(string filename) {
  int VERBOSE(0);
  ifstream INS;
  vector<string> vlines; 
  string sline;
  INS.open(filename);
  int cntLines(0);
  while (getline(INS, sline)) {
    vlines.push_back(sline);
    ++cntLines;
  }
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
void anaTracks(string filename = "../pioneer/PionTransport/CALOCNTR.txt") {
  if (string::npos != filename.find("~")) {
    string home = gSystem->Getenv("HOME");
    cout << "home ->" << home << "<-" << endl;
    filename.replace(filename.find("~"), 1, home);
    cout << "filename now ->" << filename << "<-" << endl;
  }
  TTree *t = fillTree(filename);
  t->Print();

  t->Draw("y:x", "", "colz");
}
